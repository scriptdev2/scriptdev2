/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Kaelthas
SD%Complete: 60
SDComment: SQL, weapon scripts, mind control, need correct spells(interruptible/uninterruptible), phoenix spawn location & animation, phoenix behaviour & spawn during gravity lapse
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "the_eye.h"
#include "WorldPacket.h"

enum
{
    //kael'thas Speech
    SAY_INTRO                           = -1550016,
    SAY_INTRO_CAPERNIAN                 = -1550017,
    SAY_INTRO_TELONICUS                 = -1550018,
    SAY_INTRO_THALADRED                 = -1550019,
    SAY_INTRO_SANGUINAR                 = -1550020,
    SAY_PHASE2_WEAPON                   = -1550021,
    SAY_PHASE3_ADVANCE                  = -1550022,
    SAY_PHASE4_INTRO2                   = -1550023,
    SAY_PHASE5_NUTS                     = -1550024,
    SAY_SLAY1                           = -1550025,
    SAY_SLAY2                           = -1550026,
    SAY_SLAY3                           = -1550027,
    SAY_MINDCONTROL1                    = -1550028,
    SAY_MINDCONTROL2                    = -1550029,
    SAY_GRAVITYLAPSE1                   = -1550030,
    SAY_GRAVITYLAPSE2                   = -1550031,
    SAY_SUMMON_PHOENIX1                 = -1550032,
    SAY_SUMMON_PHOENIX2                 = -1550033,
    SAY_DEATH                           = -1550034,

    //Thaladred the Darkener speech
    SAY_THALADRED_AGGRO                 = -1550035,
    SAY_THALADRED_DEATH                 = -1550036,
    EMOTE_THALADRED_GAZE                = -1550037,

    //Lord Sanguinar speech
    SAY_SANGUINAR_AGGRO                 = -1550038,
    SAY_SANGUINAR_DEATH                 = -1550039,

    //Grand Astromancer Capernian speech
    SAY_CAPERNIAN_AGGRO                 = -1550040,
    SAY_CAPERNIAN_DEATH                 = -1550041,

    //Master Engineer Telonicus speech
    SAY_TELONICUS_AGGRO                 = -1550042,
    SAY_TELONICUS_DEATH                 = -1550043,

    //Phase 2 spells
    SPELL_SUMMON_WEAPONS                = 36976,
    SPELL_SUMMON_WEAPONA                = 36958,
    SPELL_SUMMON_WEAPONB                = 36959,
    SPELL_SUMMON_WEAPONC                = 36960,
    SPELL_SUMMON_WEAPOND                = 36961,
    SPELL_SUMMON_WEAPONE                = 36962,
    SPELL_SUMMON_WEAPONF                = 36963,
    SPELL_SUMMON_WEAPONG                = 36964,
    SPELL_RES_VISUAL                    = 24171,

    //Phase 4 spells
    SPELL_FIREBALL                      = 22088,            //wrong but works with CastCustomSpell
    SPELL_PYROBLAST                     = 36819,
    SPELL_FLAME_STRIKE                  = 36735,            // summons
    SPELL_FLAME_STRIKE_DUMMY            = 36730,
    SPELL_ARCANE_DISRUPTION             = 36834,
    SPELL_SHOCK_BARRIER                 = 36815,
    SPELL_PHOENIX_ANIMATION             = 36723,
    SPELL_MIND_CONTROL                  = 32830,

    //Phase 5 spells
    SPELL_EXPLODE                       = 36092,
    SPELL_FULLPOWER                     = 36187,
    SPELL_KNOCKBACK                     = 11027,
    SPELL_GRAVITY_LAPSE                 = 34480,
    SPELL_GRAVITY_LAPSE_AURA            = 39432,
    SPELL_NETHER_BEAM                   = 35873,

    //Thaladred the Darkener spells
    SPELL_PSYCHIC_BLOW                  = 10689,
    SPELL_SILENCE                       = 30225,
    //Lord Sanguinar spells
    SPELL_BELLOWING_ROAR                = 40636,
    //Grand Astromancer Capernian spells

    SPELL_CAPERNIAN_FIREBALL            = 36971,
    SPELL_CONFLAGRATION                 = 37018,
    SPELL_ARCANE_EXPLOSION              = 36970,
    //Master Engineer Telonicus spells
    SPELL_BOMB                          = 37036,
    SPELL_REMOTE_TOY                    = 37027,
    //Nether Vapor spell
    SPELL_NETHER_VAPOR                  = 35859,
    //Phoenix spell
    SPELL_BURN                          = 36720,
    SPELL_EMBER_BLAST                   = 34341,
    SPELL_REBIRTH                       = 41587,

    //Creature IDs
    NPC_FLAME_STRIKE_TRIGGER            = 21369,
    NPC_PHOENIX                         = 21362,
    NPC_PHOENIX_EGG                     = 21364,

    //Phoenix egg and phoenix model
    MODEL_ID_PHOENIX                    = 19682,
    MODEL_ID_PHOENIX_EGG                = 20245,

    MAX_ADVISORS                        = 4
};

uint32 m_auiSpellSummonWeapon[]=
{
    SPELL_SUMMON_WEAPONA, SPELL_SUMMON_WEAPONB, SPELL_SUMMON_WEAPONC, SPELL_SUMMON_WEAPOND,
    SPELL_SUMMON_WEAPONE, SPELL_SUMMON_WEAPONF, SPELL_SUMMON_WEAPONG
};

enum Phases
{
    PHASE_0_NOT_BEGUN       = 0,
    PHASE_1_ADVISOR         = 1,
    PHASE_2_WEAPON          = 2,
    PHASE_3_ADVISOR_ALL     = 3,
    PHASE_4_SOLO            = 4,
    PHASE_5_GRAVITY         = 5,
    PHASE_6_COMPLETE        = 6
};

const float CAPERNIAN_DISTANCE          = 20.0f;            //she casts away from the target
const float KAEL_VISIBLE_RANGE          = 50.0f;

const float afGravityPos[3]             = {795.0f, 0.0f, 70.0f};

#define TIME_PHASE_2_3      120000
#define TIME_PHASE_3_4      180000

//Base AI for Advisors
struct MANGOS_DLL_DECL advisorbase_ai : public ScriptedAI
{
    advisorbase_ai(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bDoubled_Health = false;
        Reset();
    }
    protected:
    uint32 m_uiAdvisor_Speech;

    public:
    ScriptedInstance* m_pInstance;
    bool m_bFakeDeath;
    bool m_bDoubled_Health;
    uint32 m_uiDelayRes_Timer;
    uint64 m_uiDelayRes_Target;

    void Reset()
    {
        if (m_bDoubled_Health)
        {
            m_creature->SetMaxHealth(m_creature->GetMaxHealth() / 2);
            m_bDoubled_Health = false;
        }

        m_bFakeDeath = false;
        m_uiDelayRes_Timer = 0;
        m_uiDelayRes_Target = 0;

        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        //reset encounter
        if (m_pInstance && (m_pInstance->GetData(TYPE_KAELTHAS_PHASE) == PHASE_1_ADVISOR || m_pInstance->GetData(TYPE_KAELTHAS_PHASE) == PHASE_3_ADVISOR_ALL))
        {
            if (Creature* pKaelthas = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_KAELTHAS)))
                pKaelthas->AI()->EnterEvadeMode();
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho || m_bFakeDeath || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || m_bFakeDeath || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        ScriptedAI::AttackStart(pWho);
    }

    void Revive(Unit* Target)
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        // double health for phase 3
        m_creature->SetMaxHealth(m_creature->GetMaxHealth() * 2);
        m_bDoubled_Health = true;
        m_creature->SetHealth(m_creature->GetMaxHealth());
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);

        DoCastSpellIfCan(m_creature, SPELL_RES_VISUAL);
        m_uiDelayRes_Timer = 2000;
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_KAELTHAS_PHASE) == PHASE_3_ADVISOR_ALL)
            DoScriptText(m_uiAdvisor_Speech, m_creature);
    }

    void DamageTaken(Unit* pKiller, uint32 &damage)
    {
        if (damage < m_creature->GetHealth())
            return;

        //Prevent glitch if in fake death
        if (m_bFakeDeath && m_pInstance && m_pInstance->GetData(TYPE_KAELTHAS_PHASE) != PHASE_0_NOT_BEGUN)
        {
            damage = 0;
            return;
        }

        //Don't really die in phase 1 & 3, only die after that
        if (m_pInstance && m_pInstance->GetData(TYPE_KAELTHAS_PHASE) != PHASE_0_NOT_BEGUN)
        {
            //prevent death
            damage = 0;
            m_bFakeDeath = true;

            m_creature->InterruptNonMeleeSpells(false);
            m_creature->SetHealth(0);
            m_creature->StopMoving();
            m_creature->ClearComboPointHolders();
            m_creature->RemoveAllAurasOnDeath();
            m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
            m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_35_PERCENT, false);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->ClearAllReactives();
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET,0);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
            JustDied(pKiller);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiDelayRes_Timer)
        {
            if (m_uiDelayRes_Timer <= uiDiff)
            {
                m_uiDelayRes_Timer = 0;
                m_bFakeDeath = false;

                Unit* pTarget = m_creature->GetMap()->GetUnit(m_uiDelayRes_Target);

                if (!pTarget)
                    pTarget = m_creature->getVictim();

                DoResetThreat();
                AttackStart(pTarget);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(pTarget);
                m_creature->AddThreat(pTarget);
            }
            else
                m_uiDelayRes_Timer -= uiDiff;
        }
    }

};

//Kael'thas AI
struct MANGOS_DLL_DECL boss_kaelthasAI : public ScriptedAI
{
    boss_kaelthasAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        memset(&m_auiAdvisorGuid, 0, sizeof(m_auiAdvisorGuid));
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiFireball_Timer;
    uint32 m_uiArcaneDisruption_Timer;
    uint32 m_uiPhoenix_Timer;
    uint32 m_uiShockBarrier_Timer;
    uint32 m_uiGravityLapse_Timer;
    uint32 m_uiGravityLapse_Phase;
    uint32 m_uiNetherBeam_Timer;
    uint32 m_uiNetherVapor_Timer;
    uint32 m_uiFlameStrike_Timer;
    uint32 m_uiMindControl_Timer;
    uint32 m_uiPhase;
    uint32 m_uiPhaseSubphase;                                   //generic
    uint32 m_uiPhase_Timer;                                     //generic timer
    uint32 m_uiPyrosCasted;

    bool m_bInGravityLapse;
    bool m_bIsCastingFireball;
    bool m_bChainPyros;

    uint64 m_auiAdvisorGuid[MAX_ADVISORS];

    void Reset()
    {
        m_uiFireball_Timer = urand(5000, 15000);
        m_uiArcaneDisruption_Timer = 45000;
        m_uiMindControl_Timer = 40000;
        m_uiPhoenix_Timer = 50000;
        m_uiShockBarrier_Timer = 60000;
        m_uiFlameStrike_Timer = 30000;
        m_uiGravityLapse_Timer = 20000;
        m_uiGravityLapse_Phase = 0;
        m_uiNetherBeam_Timer = 8000;
        m_uiNetherVapor_Timer = 10000;
        m_uiPyrosCasted = 0;
        m_uiPhase = 0;
        m_bInGravityLapse = false;
        m_bIsCastingFireball = false;
        m_bChainPyros = false;

        if (m_creature->isInCombat())
            PrepareAdvisors();

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KAELTHAS_PHASE, PHASE_0_NOT_BEGUN);
    }

    void PrepareAdvisors()
    {
        for(uint8 i = 0; i < MAX_ADVISORS; ++i)
        {
            if (Creature* pCreature = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[i]))
            {
                pCreature->Respawn();
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pCreature->setFaction(m_creature->getFaction());
                pCreature->AI()->EnterEvadeMode();
            }
        }
    }

    void StartEvent()
    {
        if (!m_pInstance)
            return;

        m_auiAdvisorGuid[0] = m_pInstance->GetData64(DATA_THALADRED);
        m_auiAdvisorGuid[1] = m_pInstance->GetData64(DATA_SANGUINAR);
        m_auiAdvisorGuid[2] = m_pInstance->GetData64(DATA_CAPERNIAN);
        m_auiAdvisorGuid[3] = m_pInstance->GetData64(DATA_TELONICUS);

        if (!m_auiAdvisorGuid[0] || !m_auiAdvisorGuid[1] || !m_auiAdvisorGuid[2] || !m_auiAdvisorGuid[3])
        {
            error_log("SD2: Kael'Thas One or more advisors missing, Skipping Phases 1-3");

            DoScriptText(SAY_PHASE4_INTRO2, m_creature);

            m_uiPhase = PHASE_4_SOLO;

            m_pInstance->SetData(TYPE_KAELTHAS_PHASE, PHASE_4_SOLO);

            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                AttackStart(pTarget);

        }
        else
        {
            PrepareAdvisors();

            DoScriptText(SAY_INTRO, m_creature);

            m_pInstance->SetData(TYPE_KAELTHAS_PHASE, PHASE_1_ADVISOR);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            m_uiPhaseSubphase = 0;
            m_uiPhase_Timer = 23000;
            m_uiPhase = PHASE_1_ADVISOR;
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->isTargetableForAttack() &&
            m_creature->IsHostileTo(pWho) && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->CanFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(pWho);
            if (m_creature->IsWithinDistInMap(pWho, attackRadius) && m_creature->IsWithinLOSInMap(pWho))
            {
                if (!m_creature->getVictim() && m_uiPhase >= PHASE_4_SOLO)
                {
                    pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                    AttackStart(pWho);
                }
                else if (m_creature->GetMap()->IsDungeon())
                {
                    if (m_pInstance && m_pInstance->GetData(TYPE_KAELTHAS_PHASE) == PHASE_0_NOT_BEGUN && !m_uiPhase)
                        StartEvent();

                    pWho->SetInCombatWith(m_creature);
                    m_creature->AddThreat(pWho);
                }
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_KAELTHAS_PHASE) == PHASE_0_NOT_BEGUN && !m_uiPhase)
            StartEvent();
    }

    void KilledUnit(Unit* pUnit)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_FLAME_STRIKE_TRIGGER)
        {
            pSummoned->CastSpell(pSummoned, SPELL_FLAME_STRIKE_DUMMY, false, NULL, NULL, m_creature->GetGUID());
            return;
        }

        if (pSummoned->GetEntry() == NPC_PHOENIX)
        {
            return;
        }

        // if not phoenix or trigger, then it's one of the 7 weapons
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void JustDied(Unit* pKiller)
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KAELTHAS_PHASE, PHASE_6_COMPLETE);

        for(uint8 i = 0; i < MAX_ADVISORS; ++i)
        {
            if (Creature* pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[i]))
                pAdvisor->DealDamage(pAdvisor, pAdvisor->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Phase 1
        switch (m_uiPhase)
        {
            case PHASE_1_ADVISOR:
            {
                Unit* pTarget = NULL;
                Creature* pAdvisor = NULL;

                //Subphase switch
                switch(m_uiPhaseSubphase)
                {
                    //Subphase 1 - Start
                    case 0:
                        if (m_uiPhase_Timer < uiDiff)
                        {
                            DoScriptText(SAY_INTRO_THALADRED, m_creature);

                            //start advisor within 7 seconds
                            m_uiPhase_Timer = 7000;
                            ++m_uiPhaseSubphase;
                        }
                        else
                            m_uiPhase_Timer -= uiDiff;

                        break;

                    //Subphase 1 - Unlock advisor
                    case 1:
                        if (m_uiPhase_Timer < uiDiff)
                        {
                            pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[0]);

                            if (pAdvisor)
                            {
                                pAdvisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pAdvisor->setFaction(m_creature->getFaction());

                                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                                if (pTarget)
                                    pAdvisor->AI()->AttackStart(pTarget);
                            }

                            ++m_uiPhaseSubphase;
                        }
                        else
                            m_uiPhase_Timer -= uiDiff;

                        break;

                    //Subphase 2 - Start
                    case 2:
                        pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[0]);

                        if (pAdvisor && (pAdvisor->getStandState() == UNIT_STAND_STATE_DEAD))
                        {
                            DoScriptText(SAY_INTRO_SANGUINAR, m_creature);

                            //start advisor within 12.5 seconds
                            m_uiPhase_Timer = 12500;
                            ++m_uiPhaseSubphase;
                        }
                        break;

                    //Subphase 2 - Unlock advisor
                    case 3:
                        if (m_uiPhase_Timer < uiDiff)
                        {
                            pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[1]);

                            if (pAdvisor)
                            {
                                pAdvisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pAdvisor->setFaction(m_creature->getFaction());

                                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                                if (pTarget)
                                    pAdvisor->AI()->AttackStart(pTarget);
                            }

                            ++m_uiPhaseSubphase;
                        }
                        else
                            m_uiPhase_Timer -= uiDiff;

                        break;

                    //Subphase 3 - Start
                    case 4:
                        pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[1]);

                        if (pAdvisor && (pAdvisor->getStandState() == UNIT_STAND_STATE_DEAD))
                        {
                            DoScriptText(SAY_INTRO_CAPERNIAN, m_creature);

                            //start advisor within 7 seconds
                            m_uiPhase_Timer = 7000;
                            ++m_uiPhaseSubphase;
                        }
                        break;

                    //Subphase 3 - Unlock advisor
                    case 5:
                        if (m_uiPhase_Timer < uiDiff)
                        {
                            pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[2]);

                            if (pAdvisor)
                            {
                                pAdvisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pAdvisor->setFaction(m_creature->getFaction());

                                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                                if (pTarget)
                                    pAdvisor->AI()->AttackStart(pTarget);
                            }

                            ++m_uiPhaseSubphase;
                        }
                        else
                            m_uiPhase_Timer -= uiDiff;

                        break;

                    //Subphase 4 - Start
                    case 6:
                        pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[2]);

                        if (pAdvisor && (pAdvisor->getStandState() == UNIT_STAND_STATE_DEAD))
                        {
                            DoScriptText(SAY_INTRO_TELONICUS, m_creature);

                            //start advisor within 8.4 seconds
                            m_uiPhase_Timer = 8400;
                            ++m_uiPhaseSubphase;
                        }
                        break;

                    //Subphase 4 - Unlock advisor
                    case 7:
                        if (m_uiPhase_Timer < uiDiff)
                        {
                            pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[3]);

                            if (pAdvisor)
                            {
                                pAdvisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pAdvisor->setFaction(m_creature->getFaction());

                                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                                if (pTarget)
                                    pAdvisor->AI()->AttackStart(pTarget);
                            }

                            m_uiPhase_Timer = 3000;
                            ++m_uiPhaseSubphase;
                        }else m_uiPhase_Timer -= uiDiff;
                        break;

                    //End of phase 1
                    case 8:
                        pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[3]);

                        if (pAdvisor && (pAdvisor->getStandState() == UNIT_STAND_STATE_DEAD))
                        {
                            m_uiPhase = PHASE_2_WEAPON;
                            m_pInstance->SetData(TYPE_KAELTHAS_PHASE, PHASE_2_WEAPON);

                            DoScriptText(SAY_PHASE2_WEAPON, m_creature);

                            m_uiPhaseSubphase = 0;
                            m_uiPhase_Timer = 3500;
                            DoCastSpellIfCan(m_creature, SPELL_SUMMON_WEAPONS);
                        }
                        break;
                }

                break;
            }

            case PHASE_2_WEAPON:
            {
                if (m_uiPhaseSubphase == 0)
                {
                    if (m_uiPhase_Timer < uiDiff)
                    {
                        m_uiPhaseSubphase = 1;
                    }else m_uiPhase_Timer -= uiDiff;
                }

                //Spawn weapons
                if (m_uiPhaseSubphase == 1)
                {
                    m_creature->CastSpell(m_creature, SPELL_SUMMON_WEAPONS, false);

                    uint8 uiMaxWeapon = sizeof(m_auiSpellSummonWeapon)/sizeof(uint32);

                    for (uint32 i = 0; i < uiMaxWeapon; ++i)
                        m_creature->CastSpell(m_creature,m_auiSpellSummonWeapon[i],true);

                    m_uiPhaseSubphase = 2;
                    m_uiPhase_Timer = TIME_PHASE_2_3;
                }

                if (m_uiPhaseSubphase == 2)
                {
                    if (m_uiPhase_Timer < uiDiff)
                    {
                        DoScriptText(SAY_PHASE3_ADVANCE, m_creature);
                        m_pInstance->SetData(TYPE_KAELTHAS_PHASE, PHASE_3_ADVISOR_ALL);
                        m_uiPhase = PHASE_3_ADVISOR_ALL;
                        m_uiPhaseSubphase = 0;
                    }
                    else
                        m_uiPhase_Timer -= uiDiff;
                }

                break;
            }

            case PHASE_3_ADVISOR_ALL:
            {
                if (m_uiPhaseSubphase == 0)
                {
                    //Respawn advisors
                    Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                    for (uint32 i = 0; i < MAX_ADVISORS; ++i)
                    {
                        Creature* pAdvisor = m_creature->GetMap()->GetCreature(m_auiAdvisorGuid[i]);

                        if (!pAdvisor)
                            error_log("SD2: Kael'Thas Advisor %u does not exist. Possibly despawned? Incorrectly Killed?", i);
                        else
                        {
                            if (advisorbase_ai* pAdvisorAI = dynamic_cast<advisorbase_ai*>(pAdvisor->AI()))
                                pAdvisorAI->Revive(pTarget);
                        }
                    }

                    m_uiPhaseSubphase = 1;
                    m_uiPhase_Timer = TIME_PHASE_3_4;
                }

                if (m_uiPhase_Timer < uiDiff)
                {
                    DoScriptText(SAY_PHASE4_INTRO2, m_creature);
                    m_uiPhase = PHASE_4_SOLO;

                    m_pInstance->SetData(TYPE_KAELTHAS_PHASE, PHASE_4_SOLO);

                    // Sometimes people can collect Aggro in Phase 1-3. Reset threat before releasing Kael.
                    DoResetThreat();

                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        AttackStart(pTarget);

                    m_uiPhase_Timer = 30000;
                }
                else
                    m_uiPhase_Timer -= uiDiff;

                break;
            }

            case PHASE_4_SOLO:
            case 5:
            case 6:
            {
                //Return since we have no target
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                //m_uiFireball_Timer
                if (!m_bInGravityLapse && !m_bChainPyros && m_uiPhase != 5)
                {
                    if (m_uiFireball_Timer < uiDiff)
                    {
                        if (!m_bIsCastingFireball)
                        {
                            if (!m_creature->IsNonMeleeSpellCasted(false))
                            {
                                //interruptable
                                m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
                                int32 uiDmg = irand(20000, 25000);
                                m_creature->CastCustomSpell(m_creature->getVictim(), SPELL_FIREBALL, &uiDmg, 0, 0, false);
                                m_bIsCastingFireball = true;
                                m_uiFireball_Timer = 2500;
                            }
                        }
                        else
                        {
                            //apply resistance
                            m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
                            m_bIsCastingFireball = false;
                            m_uiFireball_Timer = urand(5000, 15000);
                        }
                    }
                    else
                        m_uiFireball_Timer -= uiDiff;

                    //m_uiArcaneDisruption_Timer
                    if (m_uiArcaneDisruption_Timer < uiDiff)
                    {
                        DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_DISRUPTION, CAST_TRIGGERED);
                        m_uiArcaneDisruption_Timer = 60000;
                    }
                    else
                        m_uiArcaneDisruption_Timer -= uiDiff;

                    //m_uiFlameStrike_Timer
                    if (m_uiFlameStrike_Timer < uiDiff)
                    {
                        if (Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            DoCastSpellIfCan(pUnit, SPELL_FLAME_STRIKE);

                        m_uiFlameStrike_Timer = 30000;
                    }
                    else
                        m_uiFlameStrike_Timer -= uiDiff;

                    if (m_uiMindControl_Timer < uiDiff)
                    {
                        if (m_creature->getThreatManager().getThreatList().size() >= 2)
                            for (uint32 i = 0; i < 3; ++i)
                        {
                            debug_log("SD2: Kael'Thas mind control not supported.");
                            //DoCastSpellIfCan(pUnit, SPELL_MIND_CONTROL);
                        }

                        m_uiMindControl_Timer = 60000;
                    }
                    else
                        m_uiMindControl_Timer -= uiDiff;
                }

                 // Summon Phoenix
                 if (m_uiPhoenix_Timer < uiDiff)
                 {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        DoCastSpellIfCan(pTarget, SPELL_PHOENIX_ANIMATION);
                        DoScriptText(urand(0, 1) ? SAY_SUMMON_PHOENIX1 : SAY_SUMMON_PHOENIX2, pTarget);
                    }

                    m_uiPhoenix_Timer = 60000;
                }
                else
                    m_uiPhoenix_Timer -= uiDiff;

                //Phase 4 specific spells
                if (m_uiPhase == PHASE_4_SOLO)
                {
                    if (m_creature->GetHealthPercent() < 50.0f)
                    {
                        m_pInstance->SetData(TYPE_KAELTHAS_PHASE, PHASE_5_GRAVITY);
                        m_uiPhase = PHASE_5_GRAVITY;
                        m_uiPhase_Timer = 10000;

                        DoScriptText(SAY_PHASE5_NUTS, m_creature);

                        m_creature->StopMoving();
                        m_creature->GetMotionMaster()->Clear();
                        m_creature->GetMotionMaster()->MoveIdle();

                        m_creature->NearTeleportTo(afGravityPos[0], afGravityPos[1], afGravityPos[2], 0.0f);

                        m_creature->InterruptNonMeleeSpells(false);
                        DoCastSpellIfCan(m_creature, SPELL_FULLPOWER);
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }

                    //m_uiShockBarrier_Timer
                    if (m_uiShockBarrier_Timer < uiDiff)
                    {
                        DoCastSpellIfCan(m_creature, SPELL_SHOCK_BARRIER);
                        m_bChainPyros = true;
                        m_uiPyrosCasted = 0;
                        m_uiShockBarrier_Timer = 60000;
                    }
                    else
                        m_uiShockBarrier_Timer -= uiDiff;

                    //Chain Pyros (3 of them max)
                    if (m_bChainPyros && !m_creature->IsNonMeleeSpellCasted(false))
                    {
                        if (m_uiPyrosCasted < 3)
                        {
                            DoCastSpellIfCan(m_creature->getVictim(), SPELL_PYROBLAST);
                            ++m_uiPyrosCasted;
                        }
                        else
                        {
                            m_bChainPyros = false;
                            m_uiFireball_Timer = 2500;
                            m_uiArcaneDisruption_Timer = 60000;
                        }
                    }
                }

                if (m_uiPhase == PHASE_5_GRAVITY)
                {
                    if (m_uiPhase_Timer < uiDiff)
                    {
                        m_creature->InterruptNonMeleeSpells(false);
                        m_creature->RemoveAurasDueToSpell(SPELL_FULLPOWER);

                        DoCastSpellIfCan(m_creature, SPELL_EXPLODE);

                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_uiPhase = 6;
                        AttackStart(m_creature->getVictim());
                    }
                    else
                        m_uiPhase_Timer -= uiDiff;
                }

                //Phase 5
                if (m_uiPhase == 6)
                {

                    //m_uiGravityLapse_Timer
                    if (m_uiGravityLapse_Timer < uiDiff)
                    {
                        switch(m_uiGravityLapse_Phase)
                        {
                            case 0:
                            {
                                m_creature->StopMoving();
                                m_creature->GetMotionMaster()->Clear();
                                m_creature->GetMotionMaster()->MoveIdle();

                                m_creature->GetMap()->CreatureRelocation(m_creature, afGravityPos[0], afGravityPos[1], afGravityPos[2], 0.0f);
                                m_creature->SendMonsterMove(afGravityPos[0], afGravityPos[1], afGravityPos[2], SPLINETYPE_NORMAL, SPLINEFLAG_NONE, 1);

                                // 1) Kael'thas will portal the whole raid right into his body
                                ThreatList const& tList = m_creature->getThreatManager().getThreatList();
                                for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
                                {
                                    Unit* pUnit = m_creature->GetMap()->GetUnit((*i)->getUnitGuid());

                                    if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
                                    {
                                        //Use work around packet to prevent player from being dropped from combat
                                        DoTeleportPlayer(pUnit, afGravityPos[0], afGravityPos[1], afGravityPos[2], pUnit->GetOrientation());
                                    }
                                }

                                m_uiGravityLapse_Timer = 500;
                                ++m_uiGravityLapse_Phase;
                                m_bInGravityLapse = true;
                                m_uiShockBarrier_Timer = 1000;
                                m_uiNetherBeam_Timer = 5000;
                                break;
                            }
                            case 1:
                            {
                                DoScriptText(urand(0, 1) ? SAY_GRAVITYLAPSE1 : SAY_GRAVITYLAPSE2, m_creature);

                                // 2) At that point he will put a Gravity Lapse debuff on everyone
                                ThreatList const& tList = m_creature->getThreatManager().getThreatList();
                                for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
                                {
                                    if (Unit* pUnit = m_creature->GetMap()->GetUnit((*i)->getUnitGuid()))
                                    {
                                        m_creature->CastSpell(pUnit, SPELL_KNOCKBACK, true);
                                        //Gravity lapse - needs an exception in Spell system to work

                                        pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE, true, 0, 0, m_creature->GetGUID());
                                        pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE_AURA, true, 0, 0, m_creature->GetGUID());
                                    }
                                }
                                m_uiGravityLapse_Timer = 10000;
                                ++m_uiGravityLapse_Phase;
                                break;
                            }
                            case 2:
                                //Cast nether vapor aura on self
                                m_creature->InterruptNonMeleeSpells(false);
                                DoCastSpellIfCan(m_creature, SPELL_NETHER_VAPOR);

                                m_uiGravityLapse_Timer = 20000;
                                ++m_uiGravityLapse_Phase;
                                break;

                            case 3:
                            {
                                //Remove flight
                                m_creature->RemoveAurasDueToSpell(SPELL_NETHER_VAPOR);
                                m_bInGravityLapse = false;
                                m_uiGravityLapse_Timer = 60000;
                                m_uiGravityLapse_Phase = 0;
                                AttackStart(m_creature->getVictim());
                                break;
                            }
                        }
                    }
                    else
                        m_uiGravityLapse_Timer -= uiDiff;

                    if (m_bInGravityLapse)
                    {
                        //m_uiShockBarrier_Timer
                        if (m_uiShockBarrier_Timer < uiDiff)
                        {
                            DoCastSpellIfCan(m_creature, SPELL_SHOCK_BARRIER);
                            m_uiShockBarrier_Timer = 20000;
                        }
                        else
                            m_uiShockBarrier_Timer -= uiDiff;

                        //m_uiNetherBeam_Timer
                        if (m_uiNetherBeam_Timer < uiDiff)
                        {
                            if (Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                DoCastSpellIfCan(pUnit, SPELL_NETHER_BEAM);

                            m_uiNetherBeam_Timer = 4000;
                        }
                        else
                            m_uiNetherBeam_Timer -= uiDiff;
                    }
                }

                if (!m_bInGravityLapse)
                    DoMeleeAttackIfReady();
            }
        }
    }
};

//Thaladred the Darkener AI
struct MANGOS_DLL_DECL boss_thaladred_the_darkenerAI : public advisorbase_ai
{
    boss_thaladred_the_darkenerAI(Creature* pCreature) : advisorbase_ai(pCreature)
    {
        m_uiAdvisor_Speech = SAY_THALADRED_DEATH;
    }

    uint32 m_uiGaze_Timer;
    uint32 m_uiSilence_Timer;
    uint32 m_uiPsychicBlow_Timer;

    void Reset()
    {
        m_uiGaze_Timer = 100;
        m_uiSilence_Timer = 20000;
        m_uiPsychicBlow_Timer = 10000;

        advisorbase_ai::Reset();
    }

    void Aggro(Unit* pWho)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (!pWho || m_bFakeDeath)
            return;

        DoScriptText(SAY_THALADRED_AGGRO, m_creature);
        m_creature->AddThreat(pWho, 5000000.0f);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        advisorbase_ai::UpdateAI(uiDiff);

        //Faking death, don't do anything
        if (m_bFakeDeath)
            return;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //m_uiGaze_Timer
        if (m_uiGaze_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoResetThreat();
                m_creature->AddThreat(pTarget, 5000000.0f);
                DoScriptText(EMOTE_THALADRED_GAZE, m_creature, pTarget);
            }
            m_uiGaze_Timer = 8500;
        }
        else
            m_uiGaze_Timer -= uiDiff;

        //m_uiSilence_Timer
        if (m_uiSilence_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SILENCE);
            m_uiSilence_Timer = 20000;
        }
        else
            m_uiSilence_Timer -= uiDiff;

        //m_uiPsychicBlow_Timer
        if (m_uiPsychicBlow_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_PSYCHIC_BLOW);
            m_uiPsychicBlow_Timer = urand(20000, 25000);
        }
        else
            m_uiPsychicBlow_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Lord Sanguinar AI
struct MANGOS_DLL_DECL boss_lord_sanguinarAI : public advisorbase_ai
{
    boss_lord_sanguinarAI(Creature* pCreature) : advisorbase_ai(pCreature)
    {
        m_uiAdvisor_Speech = SAY_SANGUINAR_DEATH;
    }

    uint32 m_uiFear_Timer;

    void Reset()
    {
        m_uiFear_Timer = 20000;
        advisorbase_ai::Reset();
    }

    void Aggro(Unit* pWho)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (!pWho || m_bFakeDeath)
            return;

        DoScriptText(SAY_SANGUINAR_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        advisorbase_ai::UpdateAI(uiDiff);

        //Faking death, don't do anything
        if (m_bFakeDeath)
            return;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //m_uiFear_Timer
        if (m_uiFear_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_BELLOWING_ROAR);
            m_uiFear_Timer = urand(25000, 35000);           //approximately every 30 seconds
        }
        else
            m_uiFear_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Grand Astromancer Capernian AI
struct MANGOS_DLL_DECL boss_grand_astromancer_capernianAI : public advisorbase_ai
{
    boss_grand_astromancer_capernianAI(Creature* pCreature) : advisorbase_ai(pCreature)
    {
        m_uiAdvisor_Speech = SAY_CAPERNIAN_DEATH;
    }

    uint32 m_uiFireball_Timer;
    uint32 m_uiConflagration_Timer;
    uint32 m_uiArcaneExplosion_Timer;
    uint32 m_uiYell_Timer;
    bool m_bYell;

    void Reset()
    {
        m_uiFireball_Timer = 2000;
        m_uiConflagration_Timer = 20000;
        m_uiArcaneExplosion_Timer = 5000;
        m_uiYell_Timer = 2000;
        m_bYell = false;

        advisorbase_ai::Reset();
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || m_bFakeDeath || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, CAPERNIAN_DISTANCE);
        }
    }

    void Aggro(Unit *pWho)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (!pWho || m_bFakeDeath)
            return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        advisorbase_ai::UpdateAI(uiDiff);

        //Faking Death, don't do anything
        if (m_bFakeDeath)
            return;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //m_uiYell_Timer
        if (!m_bYell)
        {
            if (m_uiYell_Timer < uiDiff)
            {
                DoScriptText(SAY_CAPERNIAN_AGGRO, m_creature);
                m_bYell = true;
            }
            else
                m_uiYell_Timer -= uiDiff;
        }

        //m_uiFireball_Timer
        if (m_uiFireball_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CAPERNIAN_FIREBALL);
            m_uiFireball_Timer = 4000;
        }
        else
            m_uiFireball_Timer -= uiDiff;

        //m_uiConflagration_Timer
        if (m_uiConflagration_Timer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

            if (pTarget && m_creature->IsWithinDistInMap(pTarget, 30.0f))
                DoCastSpellIfCan(pTarget, SPELL_CONFLAGRATION);
            else
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CONFLAGRATION);

            m_uiConflagration_Timer = urand(10000, 15000);
        }
        else
            m_uiConflagration_Timer -= uiDiff;

        //m_uiArcaneExplosion_Timer
        if (m_uiArcaneExplosion_Timer < uiDiff)
        {
            bool m_bInMeleeRange = false;
            Unit* pTarget = NULL;
            ThreatList const& tList = m_creature->getThreatManager().getThreatList();
            for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
            {
                Unit* pUnit = m_creature->GetMap()->GetUnit((*i)->getUnitGuid());

                //if in melee range
                if (pUnit && pUnit->IsWithinDistInMap(m_creature, ATTACK_DISTANCE))
                {
                    m_bInMeleeRange = true;
                    pTarget = pUnit;
                    break;
                }
            }

            if (m_bInMeleeRange)
                DoCastSpellIfCan(pTarget, SPELL_ARCANE_EXPLOSION);

            m_uiArcaneExplosion_Timer = urand(4000, 6000);
        }
        else
            m_uiArcaneExplosion_Timer -= uiDiff;

        //Do NOT deal any melee damage.
    }
};

//Master Engineer Telonicus AI
struct MANGOS_DLL_DECL boss_master_engineer_telonicusAI : public advisorbase_ai
{
    boss_master_engineer_telonicusAI(Creature* pCreature) : advisorbase_ai(pCreature)
    {
        m_uiAdvisor_Speech = SAY_TELONICUS_DEATH;
    }

    uint32 m_uiBomb_Timer;
    uint32 m_uiRemoteToy_Timer;

    void Reset()
    {
        m_uiBomb_Timer = 10000;
        m_uiRemoteToy_Timer = 5000;

        advisorbase_ai::Reset();
    }

    void Aggro(Unit *pWho)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (!pWho || m_bFakeDeath)
            return;

        DoScriptText(SAY_TELONICUS_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        advisorbase_ai::UpdateAI(uiDiff);

        //Faking Death, do nothing
        if (m_bFakeDeath)
            return;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //m_uiBomb_Timer
        if (m_uiBomb_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_BOMB);
            m_uiBomb_Timer = 25000;
        }
        else
            m_uiBomb_Timer -= uiDiff;

        //m_uiRemoteToy_Timer
        if (m_uiRemoteToy_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_REMOTE_TOY);

            m_uiRemoteToy_Timer = urand(10000, 15000);
        }
        else
            m_uiRemoteToy_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Phoenix AI
struct MANGOS_DLL_DECL mob_phoenix_tkAI : public ScriptedAI
{
    mob_phoenix_tkAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiCycle_Timer;

    void Reset()
    {
        m_uiCycle_Timer = 2000;
        m_creature->CastSpell(m_creature,SPELL_BURN,true);
    }

    void JustDied(Unit* pKiller)
    {
        //is this spell in use anylonger?
        //m_creature->CastSpell(m_creature,SPELL_EMBER_BLAST,true);
        m_creature->SummonCreature(NPC_PHOENIX_EGG,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN,16000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCycle_Timer < uiDiff)
        {
            //spell Burn should possible do this, but it doesn't, so do this for now.
            uint32 uiDmg = urand(4500,5500);

            if (m_creature->GetHealth() > uiDmg)
                m_creature->SetHealth(uint32(m_creature->GetHealth()-uiDmg));

            m_uiCycle_Timer = 2000;
        }
        else
            m_uiCycle_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Phoenix Egg AI
struct MANGOS_DLL_DECL mob_phoenix_egg_tkAI : public ScriptedAI
{
    mob_phoenix_egg_tkAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiRebirth_Timer;

    void Reset()
    {
        m_uiRebirth_Timer = 15000;
    }

    //ignore any
    void MoveInLineOfSight(Unit* pWho) { return; }

    void AttackStart(Unit* pWho)
    {
        if (m_creature->Attack(pWho, false))
        {
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            DoStartNoMovement(pWho);
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AddThreat(m_creature->getVictim());
        pSummoned->CastSpell(pSummoned,SPELL_REBIRTH,false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_uiRebirth_Timer)
            return;

        if (m_uiRebirth_Timer <= uiDiff)
        {
            m_creature->SummonCreature(NPC_PHOENIX,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_CORPSE_DESPAWN,5000);
            m_uiRebirth_Timer = 0;
        }
        else
            m_uiRebirth_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_kaelthas(Creature* pCreature)
{
    return new boss_kaelthasAI(pCreature);
}

CreatureAI* GetAI_boss_thaladred_the_darkener(Creature* pCreature)
{
    return new boss_thaladred_the_darkenerAI(pCreature);
}

CreatureAI* GetAI_boss_lord_sanguinar(Creature* pCreature)
{
    return new boss_lord_sanguinarAI(pCreature);
}

CreatureAI* GetAI_boss_grand_astromancer_capernian(Creature* pCreature)
{
    return new boss_grand_astromancer_capernianAI(pCreature);
}

CreatureAI* GetAI_boss_master_engineer_telonicus(Creature* pCreature)
{
    return new boss_master_engineer_telonicusAI(pCreature);
}

CreatureAI* GetAI_mob_phoenix_tk(Creature* pCreature)
{
    return new mob_phoenix_tkAI(pCreature);
}

CreatureAI* GetAI_mob_phoenix_egg_tk(Creature* pCreature)
{
    return new mob_phoenix_egg_tkAI(pCreature);
}

void AddSC_boss_kaelthas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kaelthas";
    newscript->GetAI = &GetAI_boss_kaelthas;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_thaladred_the_darkener";
    newscript->GetAI = &GetAI_boss_thaladred_the_darkener;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_lord_sanguinar";
    newscript->GetAI = &GetAI_boss_lord_sanguinar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_grand_astromancer_capernian";
    newscript->GetAI = &GetAI_boss_grand_astromancer_capernian;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_master_engineer_telonicus";
    newscript->GetAI = &GetAI_boss_master_engineer_telonicus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_phoenix_tk";
    newscript->GetAI = &GetAI_mob_phoenix_tk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_phoenix_egg_tk";
    newscript->GetAI = &GetAI_mob_phoenix_egg_tk;
    newscript->RegisterSelf();
}
