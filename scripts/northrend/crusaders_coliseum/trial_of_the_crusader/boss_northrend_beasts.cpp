/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: trial_of_the_crusader
SD%Complete: 70
SDComment: Icehowl combat abilities NYI
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

/*######
## npc_beast_combat_stalker
######*/

enum
{
    SAY_TIRION_BEAST_2                  = -1649005,
    SAY_TIRION_BEAST_3                  = -1649006,

    EMOTE_JORMUNGAR_ENRAGE              = -1649076,

    SPELL_BERSERK                       = 26662,
    SPELL_JORMUNGAR_ENRAGE              = 68335,

    PHASE_GORMOK                        = 0,
    PHASE_WORMS                         = 1,
    PHASE_ICEHOWL                       = 2,
};

struct npc_beast_combat_stalkerAI : public Scripted_NoMovementAI
{
    npc_beast_combat_stalkerAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Reset();
        m_creature->SetInCombatWithZone();
    }

    instance_trial_of_the_crusader* m_pInstance;

    ObjectGuid m_aSummonedBossGuid[4];
    bool m_bFirstWormDied;
    uint32 m_uiBerserkTimer;
    uint32 m_uiAttackDelayTimer;
    uint32 m_uiNextBeastTimer;
    uint32 m_uiPhase;

    uint32 m_uiWormPhaseTimer;
    uint8 m_uiWormPhaseStage;

    void Reset() override
    {
        m_uiWormPhaseTimer   = 0;
        m_uiWormPhaseStage   = 0;
        m_uiAttackDelayTimer = 0;
        m_uiNextBeastTimer   = 0;
        m_bFirstWormDied     = false;
        m_uiPhase            = PHASE_GORMOK;

        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
            m_uiBerserkTimer    = 15 * MINUTE * IN_MILLISECONDS;
        else
            m_uiBerserkTimer    = 9 * MINUTE * IN_MILLISECONDS;
    }

    void EnterEvadeMode() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);

        for (uint8 i = 0; i < 4; ++i)
        {
            if (Creature* pBoss = m_creature->GetMap()->GetCreature(m_aSummonedBossGuid[i]))
                pBoss->ForcedDespawn();
        }

        m_creature->ForcedDespawn();
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_GORMOK:     m_uiPhase = PHASE_GORMOK;  break;
            case NPC_DREADSCALE: m_uiPhase = PHASE_WORMS;   break;
            case NPC_ICEHOWL:    m_uiPhase = PHASE_ICEHOWL; break;
            case NPC_ACIDMAW:
                // Cast emerge and delayed set in combat?
                pSummoned->SetInCombatWithZone();
                m_aSummonedBossGuid[3] = pSummoned->GetObjectGuid();
                return;
        }

        m_aSummonedBossGuid[m_uiPhase] = pSummoned->GetObjectGuid();

        pSummoned->SetWalk(false);
        pSummoned->GetMotionMaster()->MovePoint(m_uiPhase, aMovePositions[m_uiPhase][0], aMovePositions[m_uiPhase][1], aMovePositions[m_uiPhase][2]);

        // Next beasts are summoned only for heroic modes
        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
            m_uiNextBeastTimer = 150 * IN_MILLISECONDS;     // 2 min 30

        m_uiAttackDelayTimer = 10000;

        if (m_pInstance)
            m_pInstance->DoOpenMainGate();
    }

    // Only for Dreadscale and Icehowl
    void DoSummonNextBeast(uint32 uiBeastEntry)
    {
        if (uiBeastEntry == NPC_DREADSCALE)
        {
            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION_A))
                DoScriptText(SAY_TIRION_BEAST_2, pTirion);

            m_creature->SummonCreature(NPC_DREADSCALE, aSpawnPositions[2][0], aSpawnPositions[2][1], aSpawnPositions[2][2], aSpawnPositions[2][3], TEMPSUMMON_DEAD_DESPAWN, 0);
        }
        else
        {
            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION_A))
                DoScriptText(SAY_TIRION_BEAST_3, pTirion);

            m_creature->SummonCreature(NPC_ICEHOWL, aSpawnPositions[4][0], aSpawnPositions[4][1], aSpawnPositions[4][2], aSpawnPositions[4][3], TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned) override
    {
        if (!m_pInstance)
            return;

        switch (pSummoned->GetEntry())
        {
            case NPC_GORMOK:
                if (m_uiPhase == PHASE_GORMOK)
                    DoSummonNextBeast(NPC_DREADSCALE);
                break;

            case NPC_DREADSCALE:
            case NPC_ACIDMAW:
                if (m_bFirstWormDied && m_uiPhase == PHASE_WORMS)
                    DoSummonNextBeast(NPC_ICEHOWL);
                else
                {
                    m_bFirstWormDied = true;

                    // jormungar brother enrages
                    if (Creature* pWorm = m_pInstance->GetSingleCreatureFromStorage(pSummoned->GetEntry() == NPC_ACIDMAW ? NPC_DREADSCALE : NPC_ACIDMAW))
                    {
                        pWorm->CastSpell(pWorm, SPELL_JORMUNGAR_ENRAGE, true);
                        DoScriptText(EMOTE_JORMUNGAR_ENRAGE, pWorm);
                    }
                }
                break;

            case NPC_ICEHOWL:
                m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DONE);
                m_creature->ForcedDespawn();
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiNextBeastTimer)
        {
            if (m_uiNextBeastTimer <= uiDiff)
            {
                if (m_uiPhase == PHASE_GORMOK)
                    DoSummonNextBeast(NPC_DREADSCALE);
                else if (m_uiPhase == PHASE_WORMS)
                    DoSummonNextBeast(NPC_ICEHOWL);

                m_uiNextBeastTimer = 0;
            }
            else
                m_uiNextBeastTimer -= uiDiff;
        }

        if (m_uiAttackDelayTimer)
        {
            if (m_uiAttackDelayTimer <= uiDiff)
            {
                // for worm phase, summon brother on aggro
                if (m_uiPhase == PHASE_WORMS)
                {
                    m_creature->SummonCreature(NPC_ACIDMAW, aSpawnPositions[3][0], aSpawnPositions[3][1], aSpawnPositions[3][2], aSpawnPositions[3][3], TEMPSUMMON_DEAD_DESPAWN, 0);
                    m_uiWormPhaseTimer = 45000;
                }

                // start combat
                if (Creature* pBeast = m_creature->GetMap()->GetCreature(m_aSummonedBossGuid[m_uiPhase]))
                {
                    pBeast->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    pBeast->SetInCombatWithZone();
                }

                m_uiAttackDelayTimer = 0;
            }
            else
                m_uiAttackDelayTimer -= uiDiff;
        }

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                for (uint8 i = 0; i < 4; ++i)
                {
                    Creature* pBoss = m_creature->GetMap()->GetCreature(m_aSummonedBossGuid[i]);
                    if (pBoss && pBoss->isAlive())
                        pBoss->CastSpell(pBoss, SPELL_BERSERK, true);
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        // jormungars phase switch control
        if (m_uiWormPhaseTimer)
        {
            if (m_uiWormPhaseTimer <= uiDiff)
            {
                if (!m_pInstance)
                    return;

                ++m_uiWormPhaseStage;

                switch (m_uiWormPhaseStage)
                {
                    // submerge worms
                    case 1:
                        if (Creature* pWorm = m_pInstance->GetSingleCreatureFromStorage(NPC_ACIDMAW))
                        {
                            if (pWorm->isAlive())
                                SendAIEvent(AI_EVENT_CUSTOM_A, m_creature, pWorm);
                        }
                        if (Creature* pWorm = m_pInstance->GetSingleCreatureFromStorage(NPC_DREADSCALE))
                        {
                            if (pWorm->isAlive())
                                SendAIEvent(AI_EVENT_CUSTOM_A, m_creature, pWorm);
                        }

                        m_uiWormPhaseTimer = 4000;
                        break;

                    // change places
                    case 2:
                        float fX, fY, fZ;
                        if (Creature* pWorm = m_pInstance->GetSingleCreatureFromStorage(NPC_ACIDMAW))
                        {
                            if (pWorm->isAlive())
                            {
                                m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 45.0f, fX, fY, fZ);
                                pWorm->MonsterMoveWithSpeed(fX, fY, fZ, 7.7f);
                            }
                        }
                        if (Creature* pWorm = m_pInstance->GetSingleCreatureFromStorage(NPC_DREADSCALE))
                        {
                            if (pWorm->isAlive())
                            {
                                m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 45.0f, fX, fY, fZ);
                                pWorm->MonsterMoveWithSpeed(fX, fY, fZ, 7.7f);
                            }
                        }

                        m_uiWormPhaseTimer = 6000;
                        break;

                    // emerge and change phase
                    case 3:
                        if (Creature* pWorm = m_pInstance->GetSingleCreatureFromStorage(NPC_ACIDMAW))
                        {
                            if (pWorm->isAlive())
                                SendAIEvent(AI_EVENT_CUSTOM_B, m_creature, pWorm);
                        }
                        if (Creature* pWorm = m_pInstance->GetSingleCreatureFromStorage(NPC_DREADSCALE))
                        {
                            if (pWorm->isAlive())
                                SendAIEvent(AI_EVENT_CUSTOM_B, m_creature, pWorm);
                        }

                        m_uiWormPhaseStage = 0;
                        m_uiWormPhaseTimer = 45000;
                        break;
                }
            }
            else
                m_uiWormPhaseTimer -= uiDiff;
        }

        m_creature->SelectHostileTarget();
    }
};

CreatureAI* GetAI_npc_beast_combat_stalker(Creature* pCreature)
{
    return new npc_beast_combat_stalkerAI(pCreature);
}

/*######
## boss_gormok, vehicle driven by 34800 (multiple times)
######*/

enum
{
    // gormok spells
    SPELL_IMPALE                = 66331,
    SPELL_STOMP                 = 66330,

    // snobold spells
    SPELL_JUMP_TO_HAND          = 66342,                // prepare snobold to jump
    SPELL_RISING_ANGER          = 66636,
    SPELL_SNOBOLLED             = 66406,                // throw snobold on players head
};

struct boss_gormokAI : public ScriptedAI
{
    boss_gormokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;

    uint32 m_uiStompTimer;
    uint32 m_uiImpaleTimer;
    uint32 m_uiJumpTimer;
    uint32 m_uiSnoboldTimer;

    uint8 m_uiSnoboldsBoarded;

    GuidVector m_vSnoboldGuidsVector;

    void Reset() override
    {
        m_uiStompTimer      = urand(20000, 25000);
        m_uiImpaleTimer     = 10000;
        m_uiJumpTimer       = 20000;
        m_uiSnoboldTimer    = 0;

        m_uiSnoboldsBoarded = 0;
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        for (GuidVector::const_iterator itr = m_vSnoboldGuidsVector.begin(); itr != m_vSnoboldGuidsVector.end(); ++itr)
        {
            if (Creature* pSnobold = m_creature->GetMap()->GetCreature(*itr))
            {
                if (!pSnobold->isAlive())
                    continue;

                // ToDo: check if there is any player vehicle mounting involved
                SendAIEvent(AI_EVENT_CUSTOM_EVENTAI_A, m_creature, pSnobold);
            }
        }
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_SNOBOLD_VASSAL)
        {
            m_vSnoboldGuidsVector.push_back(pSummoned->GetObjectGuid());
            ++m_uiSnoboldsBoarded;
        }
    }

    // get the current active snobold
    Creature* GetActiveSnobold() { return m_creature->GetMap()->GetCreature(m_vSnoboldGuidsVector[m_uiSnoboldsBoarded - 1]); }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // snobold related spells
        if (m_uiSnoboldsBoarded)
        {
            // snobold jumps from boss' back to boss' hand
            if (m_uiJumpTimer < uiDiff)
            {
                if (Creature* pSnobold = GetActiveSnobold())
                {
                    m_creature->RemoveAurasByCasterSpell(SPELL_RIDE_VEHICLE_HARDCODED, pSnobold->GetObjectGuid());
                    pSnobold->CastSpell(m_creature, SPELL_JUMP_TO_HAND, true);

                    m_uiSnoboldTimer = 3000;
                    m_uiJumpTimer = 20000;
                }
            }
            else
                m_uiJumpTimer -= uiDiff;
        }

        if (m_uiSnoboldTimer)
        {
            // snobold jumps from boss' hand to player's back
            if (m_uiSnoboldTimer <= uiDiff)
            {
                if (Creature* pSnobold = GetActiveSnobold())
                {
                    pSnobold->CastSpell(m_creature, SPELL_RISING_ANGER, true);

                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_SNOBOLLED, SELECT_FLAG_PLAYER))
                    {
                        // ToDo: change this to setup the player vehicle for the snobold. It seems that the spell that will handle this is missing
                        pSnobold->FixateTarget(pTarget);
                        // pTarget->SetVehicleId(pSnobold->GetCreatureInfo()->VehicleTemplateId, pSnobold->GetEntry());
                        // pTarget->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);

                        pSnobold->CastSpell(pTarget, SPELL_SNOBOLLED, true);
                        SendAIEvent(AI_EVENT_CUSTOM_EVENTAI_A, m_creature, pSnobold);
                        --m_uiSnoboldsBoarded;
                    }
                }
                m_uiSnoboldTimer = 0;
            }
            else
                m_uiSnoboldTimer -= uiDiff;
        }

        if (m_uiStompTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_STOMP) == CAST_OK)
                m_uiStompTimer = 20000;
        }
        else
            m_uiStompTimer -= uiDiff;

        if (m_uiImpaleTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_IMPALE) == CAST_OK)
                m_uiImpaleTimer = 10000;
        }
        else
            m_uiImpaleTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gormok(Creature* pCreature)
{
    return new boss_gormokAI(pCreature);
}

/*######
## twin_jormungars_common
######*/

enum
{
    // common spells
    SPELL_SLIME_POOL                = 66883,
    SPELL_SWEEP                     = 66794,
    SPELL_SPAWN_GROUND_VISUAL       = 68302,            // visual for stationary worm

    // transition spells
    SPELL_STATIC_WORM_SUBMERGE      = 66936,            // triggers 66969; long cast
    SPELL_MOBILE_WORM_SUBMERGE      = 66948,            // triggers 66969; short cast
    SPELL_MOBILE_WORM_EMERGE        = 66949,            // triggers 63984; short cast
    SPELL_STATIC_WORM_EMERGE        = 66947,            // triggers 63984; long cast
    SPELL_HATE_TO_ZERO              = 63984,

    // debuffs
    SPELL_PARALYTIC_TOXIN           = 66823,
    SPELL_BURNING_BILE              = 66869,

    // slime pool
    SPELL_SLIME_POOL_AURA           = 66882,
    NPC_SLIME_POOL                  = 35176,

    // phases
    PHASE_STATIONARY                = 0,
    PHASE_SUBMERGED                 = 1,
    PHASE_MOBILE                    = 2,
};

struct twin_jormungars_commonAI : public ScriptedAI
{
    twin_jormungars_commonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;

    uint8 m_uiPhase;
    uint8 m_uiPrevPhase;
    uint8 m_uiNextPhase;

    // mobile
    uint32 m_uiSpewTimer;
    uint32 m_uiBiteTimer;
    uint32 m_uiSlimePoolTimer;

    // stationary
    uint32 m_uiSpitTimer;
    uint32 m_uiSprayTimer;
    uint32 m_uiSweepTimer;

    void Reset() override
    {
        m_uiSpewTimer       = 5000;
        m_uiBiteTimer       = urand(5000, 10000);
        m_uiSlimePoolTimer  = urand(12000, 15000);

        m_uiSpitTimer       = 3000;
        m_uiSprayTimer      = urand(7000, 13000);
        m_uiSweepTimer      = urand(12000, 15000);
    }

    void JustSummoned(Creature* pSummned) override
    {
        if (pSummned->GetEntry() == NPC_SLIME_POOL)
            pSummned->CastSpell(pSummned, SPELL_SLIME_POOL_AURA, false);
    }

    void ReceiveAIEvent(AIEventType eventType, Creature* /*pSender*/, Unit* /*pInvoker*/, uint32 /*uiMiscValue*/) override
    {
        // inform when submerged
        if (eventType == AI_EVENT_CUSTOM_A)
        {
            // cast submerge spells
            if (m_uiPhase == PHASE_MOBILE)
                DoCastSpellIfCan(m_creature, SPELL_MOBILE_WORM_SUBMERGE, CAST_INTERRUPT_PREVIOUS);
            else if (m_uiPhase == PHASE_STATIONARY)
                DoCastSpellIfCan(m_creature, SPELL_STATIC_WORM_SUBMERGE, CAST_INTERRUPT_PREVIOUS);

            // stop movement
            SetCombatMovement(false);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            m_uiPrevPhase = m_uiPhase;
            m_uiPhase = PHASE_SUBMERGED;
        }
        // inform when emerge start
        else if (eventType == AI_EVENT_CUSTOM_B)
        {
            // prepare emerge
            if (m_uiPrevPhase == PHASE_MOBILE)
            {
                DoCastSpellIfCan(m_creature, SPELL_STATIC_WORM_EMERGE, CAST_INTERRUPT_PREVIOUS);
                m_creature->RemoveAurasDueToSpell(SPELL_MOBILE_WORM_SUBMERGE);
                m_uiNextPhase = PHASE_STATIONARY;
            }
            else if (m_uiPrevPhase == PHASE_STATIONARY)
            {
                DoCastSpellIfCan(m_creature, SPELL_MOBILE_WORM_EMERGE, CAST_INTERRUPT_PREVIOUS);
                m_creature->RemoveAurasDueToSpell(SPELL_STATIC_WORM_SUBMERGE);
                m_uiNextPhase = PHASE_MOBILE;
            }

            // inform the worm to change display id
            OnJormungarPhaseChanged(m_uiNextPhase);
        }
        // inform when emerge complete
        else if (eventType == AI_EVENT_CUSTOM_C)
        {
            DoCastSpellIfCan(m_creature, SPELL_HATE_TO_ZERO, CAST_TRIGGERED);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_uiPhase = m_uiNextPhase;

            // for mobile worm follow target
            if (m_uiPhase == PHASE_MOBILE)
            {
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            // for stationary set visual
            else if (m_uiPhase == PHASE_STATIONARY)
                DoCastSpellIfCan(m_creature, SPELL_SPAWN_GROUND_VISUAL, CAST_TRIGGERED);
        }
    }

    // Handle phase changed display id
    virtual void OnJormungarPhaseChanged(uint8 uiPhase) { }

    // Return the specific spell
    virtual uint32 GetSplitSpell() { return 0; }
    virtual uint32 GetSpraySpell() { return 0; }
    virtual uint32 GetSpewSpell() { return 0; }
    virtual uint32 GetBiteSpell() { return 0; }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_uiPhase)
        {
            // abilities for stationary phase
            case PHASE_STATIONARY:

                if (m_uiSpitTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, GetSplitSpell()) == CAST_OK)
                            m_uiSpitTimer = 3000;
                    }
                }
                else
                    m_uiSpitTimer -= uiDiff;

                if (m_uiSprayTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), GetSpraySpell()) == CAST_OK)
                        m_uiSprayTimer = 21000;
                }
                else
                    m_uiSprayTimer -= uiDiff;

                if (m_uiSweepTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SWEEP) == CAST_OK)
                        m_uiSweepTimer = urand(10000, 15000);
                }
                else
                    m_uiSweepTimer -= uiDiff;

                break;

            // abilities for mobile phase
            case PHASE_MOBILE:

                if (m_uiBiteTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), GetBiteSpell()) == CAST_OK)
                        m_uiBiteTimer = urand(5000, 7000);
                }
                else
                    m_uiBiteTimer -= uiDiff;

                if (m_uiSpewTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, GetSpewSpell()) == CAST_OK)
                        m_uiSpewTimer = 21000;
                }
                else
                    m_uiSpewTimer -= uiDiff;

                if (m_uiSlimePoolTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SLIME_POOL) == CAST_OK)
                        m_uiSlimePoolTimer = urand(17000, 23000);
                }
                else
                    m_uiSlimePoolTimer -= uiDiff;

                break;

            // combat break
            case PHASE_SUBMERGED:
                return;
        }

        DoMeleeAttackIfReady();
    }
};

/*######
## boss_acidmaw
######*/

enum
{
    // mobile
    SPELL_ACID_SPEW                 = 66818,
    SPELL_PARALYTIC_BITE            = 66824,

    // stationary
    SPELL_ACID_SPIT                 = 66880,
    SPELL_PARALYTIC_SPRAY           = 66901,

    // display ids
    DISPLAY_ID_ACIDMAW_FIXED        = 29815,
    DISPLAY_ID_ACIDMWA_MOBILE       = 29816,
};

struct boss_acidmawAI : public twin_jormungars_commonAI
{
    boss_acidmawAI(Creature* pCreature) : twin_jormungars_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        m_uiPhase = PHASE_STATIONARY;
        SetCombatMovement(false);

        twin_jormungars_commonAI::Reset();

        // ToDo: research if there is a spawn animation involved
        DoCastSpellIfCan(m_creature, SPELL_SPAWN_GROUND_VISUAL);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpellEntry) override
    {
        if (pTarget->GetTypeId() != TYPEID_PLAYER)
            return;

        // remove burning bile and add paralytic toxin
        if (pSpellEntry->Id == SPELL_PARALYTIC_BITE || pSpellEntry->Id == SPELL_PARALYTIC_SPRAY)
        {
            pTarget->RemoveAurasDueToSpell(SPELL_BURNING_BILE);
            pTarget->CastSpell(pTarget, SPELL_PARALYTIC_TOXIN, true);
        }
    }

    void OnJormungarPhaseChanged(uint8 uiPhase)
    {
        if (uiPhase == PHASE_STATIONARY)
            m_creature->SetDisplayId(DISPLAY_ID_ACIDMAW_FIXED);
        else if (uiPhase == PHASE_MOBILE)
            m_creature->SetDisplayId(DISPLAY_ID_ACIDMWA_MOBILE);
    }

    uint32 GetSplitSpell() { return SPELL_ACID_SPIT; }
    uint32 GetSpewSpell() { return SPELL_ACID_SPEW; }
    uint32 GetSpraySpell() { return SPELL_PARALYTIC_SPRAY; }
    uint32 GetBiteSpell() { return SPELL_PARALYTIC_BITE; }
};

CreatureAI* GetAI_boss_acidmaw(Creature* pCreature)
{
    return new boss_acidmawAI(pCreature);
}

/*######
## boss_dreadscale
######*/

enum
{
    // mobile
    SPELL_MOLTEN_SPEW               = 66821,
    SPELL_BURNING_BITE              = 66879,

    // stationary
    SPELL_FIRE_SPIT                 = 66796,
    SPELL_BURNING_SPRAY             = 66902,

    // display ids
    DISPLAY_ID_DREADSCALE_FIXED     = 26935,
    DISPLAY_ID_DREADSCALE_MOBILE    = 24564,
};

struct boss_dreadscaleAI : public twin_jormungars_commonAI
{
    boss_dreadscaleAI(Creature* pCreature) : twin_jormungars_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        m_uiPhase = PHASE_MOBILE;

        twin_jormungars_commonAI::Reset();
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        // don't attack during intro
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
            return;

        twin_jormungars_commonAI::MoveInLineOfSight(pWho);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpellEntry) override
    {
        if (pTarget->GetTypeId() != TYPEID_PLAYER)
            return;

        // remove paralytic toxin and add burning bile
        if (pSpellEntry->Id == SPELL_BURNING_BITE || pSpellEntry->Id == SPELL_BURNING_SPRAY)
        {
            pTarget->RemoveAurasDueToSpell(SPELL_PARALYTIC_TOXIN);
            pTarget->CastSpell(pTarget, SPELL_BURNING_BILE, true);
        }
    }

    void OnJormungarPhaseChanged(uint8 uiPhase)
    {
        if (uiPhase == PHASE_STATIONARY)
            m_creature->SetDisplayId(DISPLAY_ID_DREADSCALE_FIXED);
        else if (uiPhase == PHASE_MOBILE)
            m_creature->SetDisplayId(DISPLAY_ID_DREADSCALE_MOBILE);
    }

    uint32 GetSplitSpell() { return SPELL_FIRE_SPIT; }
    uint32 GetSpewSpell() { return SPELL_MOLTEN_SPEW; }
    uint32 GetSpraySpell() { return SPELL_BURNING_SPRAY; }
    uint32 GetBiteSpell() { return SPELL_BURNING_BITE; }
};

CreatureAI* GetAI_boss_dreadscale(Creature* pCreature)
{
    return new boss_dreadscaleAI(pCreature);
}

bool EffectDummyCreature_worm_emerge(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget, ObjectGuid /*originalCasterGuid*/)
{
    // always check spellid and effectindex
    if ((uiSpellId == SPELL_STATIC_WORM_EMERGE || uiSpellId == SPELL_MOBILE_WORM_EMERGE) && uiEffIndex == EFFECT_INDEX_0)
    {
        pCreatureTarget->AI()->SendAIEvent(AI_EVENT_CUSTOM_C, pCaster, pCreatureTarget);
        return true;
    }

    return false;
}

/*######
## boss_icehowl
######*/

enum
{
    EMOTE_MASSIVE_CRASH                 = -1649039,
};

struct boss_icehowlAI : public ScriptedAI
{
    boss_icehowlAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() override {}

    void JustReachedHome() override
    {
    }

    void Aggro(Unit* /*pWho*/) override
    {
    }

    void UpdateAI(const uint32 /*uiDiff*/) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_icehowl(Creature* pCreature)
{
    return new boss_icehowlAI(pCreature);
}

void AddSC_northrend_beasts()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_beast_combat_stalker";
    pNewScript->GetAI = &GetAI_npc_beast_combat_stalker;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_gormok";
    pNewScript->GetAI = &GetAI_boss_gormok;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_acidmaw";
    pNewScript->GetAI = &GetAI_boss_acidmaw;
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_worm_emerge;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_dreadscale";
    pNewScript->GetAI = &GetAI_boss_dreadscale;
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_worm_emerge;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_icehowl";
    pNewScript->GetAI = &GetAI_boss_icehowl;
    pNewScript->RegisterSelf();
}
