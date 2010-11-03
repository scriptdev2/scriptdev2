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
SDName: Boss_Reliquary_of_Souls
SD%Complete: 90
SDComment: Persistent Area Auras for each Essence (Aura of Suffering, Aura of Desire, Aura of Anger) requires core support.
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "black_temple.h"

//Sound'n'speech
//Suffering
#define SUFF_SAY_FREED              -1564047
#define SUFF_SAY_AGGRO              -1564048
#define SUFF_SAY_SLAY1              -1564049
#define SUFF_SAY_SLAY2              -1564050
#define SUFF_SAY_SLAY3              -1564051
#define SUFF_SAY_RECAP              -1564052
#define SUFF_SAY_AFTER              -1564053
#define EMOTE_BOSS_GENERIC_ENRAGED  -1000006

//Desire
#define DESI_SAY_FREED              -1564055
#define DESI_SAY_SLAY1              -1564056
#define DESI_SAY_SLAY2              -1564057
#define DESI_SAY_SLAY3              -1564058
#define DESI_SAY_SPEC               -1564059
#define DESI_SAY_RECAP              -1564060
#define DESI_SAY_AFTER              -1564061

//Anger
#define ANGER_SAY_FREED             -1564062
#define ANGER_SAY_FREED2            -1564063
#define ANGER_SAY_SLAY1             -1564064
#define ANGER_SAY_SLAY2             -1564065
#define ANGER_SAY_SPEC              -1564066
#define ANGER_SAY_BEFORE            -1564067
#define ANGER_SAY_DEATH             -1564068

//Spells
#define AURA_OF_SUFFERING               41292
#define AURA_OF_SUFFERING_ARMOR         42017
#define ESSENCE_OF_SUFFERING_PASSIVE    41296
#define SPELL_ENRAGE                    41305
#define SPELL_SOUL_DRAIN                41303
#define SPELL_FIXATE                    41295

#define AURA_OF_DESIRE                  41350
#define SPELL_RUNE_SHIELD               41431
#define SPELL_DEADEN                    41410
#define SPELL_SOUL_SHOCK                41426

#define AURA_OF_ANGER                   41337
#define SPELL_SELF_SEETHE               41364
#define SPELL_ENEMY_SEETHE              41520
#define SPELL_SOUL_SCREAM               41545
#define SPELL_SPITE                     41377

#define ENSLAVED_SOUL_PASSIVE           41535
#define SPELL_SOUL_RELEASE              41542
#define SPELL_RESTORE_MANA              32848
#define SPELL_RESTORE_HEALTH            25329

#define CREATURE_ENSLAVED_SOUL          23469

struct ReliquaryPosition
{
    float x,y;
};

static ReliquaryPosition Coords[]=
{
    {450.4f, 212.3f},
    {542.1f, 212.3f},
    {542.1f, 168.3f},
    {542.1f, 137.4f},
    {450.4f, 137.4f},
    {450.4f, 168.3f}
};

struct MANGOS_DLL_DECL npc_enslaved_soulAI : public ScriptedAI
{
    npc_enslaved_soulAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 ReliquaryGUID;

    void Reset()
    {
        ReliquaryGUID = 0;
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (damage >= m_creature->GetHealth())
        {
            if (done_by->GetTypeId() == TYPEID_PLAYER)
            {
                done_by->CastSpell(done_by, SPELL_RESTORE_HEALTH, true);
                if (done_by->GetMaxPower(POWER_MANA) > 0)
                {
                    if ((done_by->GetPower(POWER_MANA) / done_by->GetMaxPower(POWER_MANA)) < 70)
                    {
                        uint32 mana = done_by->GetPower(POWER_MANA) + (uint32)(done_by->GetMaxPower(POWER_MANA)*0.3);
                        done_by->SetPower(POWER_MANA, mana);
                    }else done_by->SetPower(POWER_MANA, done_by->GetMaxPower(POWER_MANA));
                }
            }
            DoCastSpellIfCan(done_by, SPELL_SOUL_RELEASE);
        }
    }

    void JustDied(Unit *killer);
};

struct MANGOS_DLL_DECL boss_reliquary_of_soulsAI : public ScriptedAI
{
    boss_reliquary_of_soulsAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SufferingGUID = 0;
        DesireGUID = 0;
        AngerGUID = 0;
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 SufferingGUID;
    uint64 DesireGUID;
    uint64 AngerGUID;

    uint32 SoulDeathCount;
    // 0 = Out of Combat, 1 = Not started, 2 = Suffering, 3 = Souls, 4 = Desire, 5 = Souls, 6 = Anger
    uint32 Phase;
    uint32 SummonEssenceTimer;
    uint32 DespawnEssenceTimer;
    uint32 SoulCount;
    uint32 SummonSoulTimer;
    uint32 AnimationTimer;

    bool IsDead;
    bool EndingPhase;

    void Reset()
    {
        DespawnEssences();

        SoulDeathCount = 0;
        Phase = 0;
        SummonEssenceTimer = 8000;
        DespawnEssenceTimer = 2000;
        SoulCount = 0;
        SummonSoulTimer = 1000;
        AnimationTimer = 8000;

        IsDead = false;
        EndingPhase = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);
        m_creature->GetMotionMaster()->Clear(false);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RELIQUIARY, NOT_STARTED);
    }

    void DespawnEssences()
    {
        Creature* pEssence = NULL;

        if (SufferingGUID)
            pEssence = m_creature->GetMap()->GetCreature(SufferingGUID);
        else if (DesireGUID)
            pEssence = m_creature->GetMap()->GetCreature(DesireGUID);
        else if (AngerGUID)
            pEssence = m_creature->GetMap()->GetCreature(AngerGUID);

        if (pEssence && pEssence->isAlive())
            pEssence->ForcedDespawn();
    }

    void AttackStart(Unit* who) { }

    void MoveInLineOfSight(Unit *who)
    {
        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (m_creature->getThreatManager().getThreatList().empty())
                {
                    if (m_pInstance)
                        m_pInstance->SetData(TYPE_RELIQUIARY, IN_PROGRESS);

                    Phase = 1;

                    // I R ANNNGRRRY!
                    m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,375);
                    SummonEssenceTimer = 8000;
                    AnimationTimer = 5100;
                    m_creature->AddThreat(who);
                    //m_creature->SetInCombatWith(who);     // Don't know what is like retail
                    //who->SetInCombatWith(m_creature);
                    m_creature->SetInCombatWithZone();      // Same goes here, but setting to zone will prevent bug if the only player of threatList dies

                }
            }
        }
    }

    void SummonSoul()
    {
        uint32 random = urand(0, 5);
        float x = Coords[random].x;
        float y = Coords[random].y;

        Creature* Soul = m_creature->SummonCreature(CREATURE_ENSLAVED_SOUL, x, y, m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000);
        Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
        if (target && Soul)
        {
            if (npc_enslaved_soulAI* pSoulAI = dynamic_cast<npc_enslaved_soulAI*>(Soul->AI()))
                pSoulAI->ReliquaryGUID = m_creature->GetGUID();

            Soul->CastSpell(Soul, ENSLAVED_SOUL_PASSIVE, true);
            Soul->AddThreat(target);
            ++SoulCount;
        }
    }

    void MergeThreatList(Creature* target)
    {
        if (!target)
            return;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
        {
            if (Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
            {
                m_creature->AddThreat(pUnit);                // This is so that we make sure the unit is in Reliquary's threat list before we reset the unit's threat.
                m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);
                float threat = target->getThreatManager().getThreat(pUnit);
                m_creature->AddThreat(pUnit, threat);       // This makes it so that the unit has the same amount of threat in Reliquary's threatlist as in the target creature's (One of the Essences).
            }
        }
    }

    void JustDied(Unit* killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RELIQUIARY, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!Phase)
            return;

        // Reset if event is begun and we don't have a threatlist
        if (Phase && m_creature->getThreatManager().getThreatList().empty())
            EnterEvadeMode();

        if (Phase == 1)
        {
            if (AnimationTimer < diff)
            {
                // Release the cube
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);
                AnimationTimer = 8300;
            }else AnimationTimer -= diff;

            if (SummonEssenceTimer < diff)
            {
                // Ribs: open
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,373);

                Creature* EssenceSuffering = m_creature->SummonCreature(23418, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 1.57f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 10000);

                if (EssenceSuffering)
                {
                    DoScriptText(SUFF_SAY_FREED, EssenceSuffering);

                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                    {
                        EssenceSuffering->AddThreat(target);
                        EssenceSuffering->AI()->AttackStart(target);
                    }

                    SufferingGUID = EssenceSuffering->GetGUID();
                }

                EndingPhase = false;
                Phase = 2;
            }else SummonEssenceTimer -= diff;
        }

        if (Phase == 2)
        {
            if (SufferingGUID)
            {
                Creature* EssenceSuffering = m_creature->GetMap()->GetCreature(SufferingGUID);

                if (!EssenceSuffering || (!EssenceSuffering->isAlive()))
                    EnterEvadeMode();

                if (!EndingPhase)
                {
                    if (EssenceSuffering)
                    {
                        if (EssenceSuffering->GetHealthPercent() < 10.0f)
                        {
                            DoScriptText(SUFF_SAY_RECAP, EssenceSuffering);
                            MergeThreatList(EssenceSuffering);
                            EssenceSuffering->RemoveAllAuras();
                            EssenceSuffering->DeleteThreatList();
                            EssenceSuffering->GetMotionMaster()->MoveFollow(m_creature,0.0f,0.0f);
                            EssenceSuffering->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            DespawnEssenceTimer = 4000;
                            AnimationTimer = 2200;
                            EndingPhase = true;
                        }
                    }
                }

                if ((EndingPhase) && (EssenceSuffering) && (EssenceSuffering->isAlive()))
                {
                    if (AnimationTimer < diff)
                    {
                        // Return
                        EssenceSuffering->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);
                        AnimationTimer = 10000;
                    }else AnimationTimer -= diff;

                    if (DespawnEssenceTimer < diff)
                    {
                        DoScriptText(SUFF_SAY_AFTER, EssenceSuffering);

                        EssenceSuffering->DeleteThreatList();
                        EssenceSuffering->SetDisplayId(11686);
                        EssenceSuffering->setFaction(35);
                        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);
                        SummonEssenceTimer = 20000;         //60000;
                        AnimationTimer = 18200;             //58100;
                        SoulDeathCount = 0;
                        SoulCount = 0;
                        SummonSoulTimer = 1000;
                        EndingPhase = false;
                        Phase = 3;
                        SufferingGUID = 0;
                    }else DespawnEssenceTimer -= diff;
                }
            }
        }

        if (Phase == 3)
        {
            if (SoulCount < 36)
            {
                if (SummonSoulTimer < diff)
                {
                    SummonSoul();
                    SummonSoulTimer = 500;
                }else SummonSoulTimer -= diff;
            }

            if (SoulDeathCount >= SoulCount)
            {
                if (AnimationTimer < diff)
                {
                    // Release the cube
                    m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);
                    AnimationTimer = 10000;
                }else AnimationTimer -= diff;

                if (SummonEssenceTimer < diff)
                {
                    // Ribs: open
                    m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,373);

                    Creature* EssenceDesire = m_creature->SummonCreature(23419, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 1.57f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 10000);

                    if (EssenceDesire)
                    {
                        DoScriptText(DESI_SAY_FREED, EssenceDesire);

                        if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            EssenceDesire->AddThreat(target);
                            EssenceDesire->AI()->AttackStart(target);
                        }

                        DesireGUID = EssenceDesire->GetGUID();
                        SoulDeathCount = 0;
                    }

                    Phase = 4;
                }else SummonEssenceTimer -= diff;
            }
        }

        if (Phase == 4)
        {
            if (DesireGUID)
            {
                Creature* EssenceDesire = m_creature->GetMap()->GetCreature(DesireGUID);

                if (!EssenceDesire || !EssenceDesire->isAlive())
                    EnterEvadeMode();

                if (!EndingPhase && EssenceDesire)
                {
                    if (EssenceDesire->GetHealthPercent() < 10.0f)
                    {
                        MergeThreatList(EssenceDesire);
                        EssenceDesire->GetMotionMaster()->MoveFollow(m_creature,0.0f,0.0f);
                        EssenceDesire->RemoveAllAuras();
                        EssenceDesire->DeleteThreatList();

                        DoScriptText(DESI_SAY_RECAP, EssenceDesire);

                        EssenceDesire->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        DespawnEssenceTimer = 4000;
                        AnimationTimer = 2200;
                        EndingPhase = true;
                    }
                }

                if (EndingPhase && EssenceDesire)
                {
                    if (EssenceDesire->isAlive())
                    {
                        if (AnimationTimer < diff)
                        {
                            // Return
                            EssenceDesire->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);
                            AnimationTimer = 10000;
                        }else AnimationTimer -= diff;

                        if (DespawnEssenceTimer < diff)
                        {
                            EssenceDesire->DeleteThreatList();
                            EssenceDesire->setFaction(35);

                            DoScriptText(DESI_SAY_AFTER, EssenceDesire);

                            EssenceDesire->SetDisplayId(11686);
                            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);
                            SummonEssenceTimer = 20000;
                            AnimationTimer = 18200;
                            SoulDeathCount = 0;
                            SoulCount = 0;
                            SummonSoulTimer = 1000;
                            EndingPhase = false;
                            Phase = 5;
                            DesireGUID = 0;
                        }else DespawnEssenceTimer -= diff;
                    }
                }
            }
        }

        if (Phase == 5)
        {
            if (SoulCount < 36)
            {
                if (SummonSoulTimer < diff)
                {
                    SummonSoul();
                    SummonSoulTimer = 500;
                }else SummonSoulTimer -= diff;
            }

            if (SoulDeathCount >= SoulCount)
            {
                if (AnimationTimer < diff)
                {
                    // Release the cube
                    m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);
                    AnimationTimer = 10000;
                }else AnimationTimer -= diff;

                if (SummonEssenceTimer < diff)
                {
                    // Ribs: open
                    m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,373);

                    Creature* EssenceAnger = m_creature->SummonCreature(23420, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 1.57f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 45000);

                    if (EssenceAnger)
                    {
                        if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                        {
                            EssenceAnger->AddThreat(target);
                            EssenceAnger->AI()->AttackStart(target);
                        }

                        AngerGUID = EssenceAnger->GetGUID();
                        DoScriptText(ANGER_SAY_FREED, EssenceAnger);
                        SoulDeathCount = 0;
                    }

                    Phase = 6;
                }else SummonEssenceTimer -= diff;
            }
        }

        if (Phase == 6)
        {
            if (AngerGUID)
            {
                Creature* EssenceAnger = m_creature->GetMap()->GetCreature(AngerGUID);

                if (!EssenceAnger)
                    EnterEvadeMode();

                if (m_creature->isAlive() && EssenceAnger)
                {
                    if (!EssenceAnger->isAlive())
                    {
                        AngerGUID = 0;
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
                }
            }
        }
    }
};

struct MANGOS_DLL_DECL boss_essence_of_sufferingAI : public ScriptedAI
{
    boss_essence_of_sufferingAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 StatAuraGUID;

    uint32 AggroYellTimer;
    uint32 FixateTimer;
    uint32 EnrageTimer;
    uint32 SoulDrainTimer;

    void Reset()
    {
        StatAuraGUID = 0;

        AggroYellTimer = 5000;
        FixateTimer = 5000;
        EnrageTimer = 30000;
        SoulDrainTimer = 150000;
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if ((damage >= m_creature->GetHealth()) && (done_by != m_creature))
        {
            damage = 0;
            // 10% of total health, signalling time to return
            m_creature->SetHealth(m_creature->GetMaxHealth()/10);
            if (StatAuraGUID)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(StatAuraGUID))
                    pPlayer->RemoveAurasDueToSpell(AURA_OF_SUFFERING_ARMOR);
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        DoCastSpellIfCan(pWho, AURA_OF_SUFFERING, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, ESSENCE_OF_SUFFERING_PASSIVE, CAST_TRIGGERED);
    }

    void KilledUnit(Unit *victim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SUFF_SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SUFF_SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SUFF_SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit* killer)
    {
    }

    void CastFixate()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        if (tList.empty())
            return;                                         // No point continuing if empty threatlist.

        std::list<Unit*> targets;

        for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());

            // Only alive players
            if (pUnit && pUnit->isAlive() && pUnit->GetTypeId() == TYPEID_PLAYER)
                targets.push_back(pUnit);
        }

        if (targets.empty())
            return;                                         // No targets added for some reason. No point continuing.

        targets.sort(ObjectDistanceOrder(m_creature));      // Sort players by distance.
        targets.resize(1);                                  // Only need closest target.
        Unit* target = targets.front();                     // Get the first target.

        // Add threat equivalent to threat on victim.
        m_creature->AddThreat(target, m_creature->getThreatManager().getThreat(m_creature->getVictim()));
        DoCastSpellIfCan(target, SPELL_FIXATE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealthPercent() <= 10.0f)
        {
            if (StatAuraGUID)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(StatAuraGUID))
                    pPlayer->RemoveAurasDueToSpell(AURA_OF_SUFFERING_ARMOR);
            }
        }

        if (m_creature->GetHealthPercent() <= 10.0f)
        {
            if (m_creature->getVictim())
                m_creature->DeleteThreatList();             // Delete our threatlist if below 10% as we should no longer attack.
            return;
        }

        // Prevent overlapping yells
        if (AggroYellTimer)
        {
            if (AggroYellTimer <= diff)
            {
                DoScriptText(SUFF_SAY_AGGRO, m_creature);
                AggroYellTimer = 0;
            }else AggroYellTimer -= diff;
        }

        //Supposed to be cast on nearest target
        if (FixateTimer < diff)
        {
            CastFixate();
            FixateTimer = 5000;
        }else FixateTimer -= diff;

        if (EnrageTimer < diff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
            {
                DoScriptText(EMOTE_BOSS_GENERIC_ENRAGED, m_creature);
                EnrageTimer = 60000;
            }
        }else EnrageTimer -= diff;

        if (SoulDrainTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target, SPELL_SOUL_DRAIN);
            SoulDrainTimer = 60000;
        }else SoulDrainTimer -= diff;

        DoMeleeAttackIfReady();
    }
};
struct MANGOS_DLL_DECL boss_essence_of_desireAI : public ScriptedAI
{
    boss_essence_of_desireAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 AggroYellTimer;
    uint32 RuneShieldTimer;
    uint32 DeadenTimer;
    uint32 SoulShockTimer;

    void Reset()
    {
        AggroYellTimer = 5000;
        RuneShieldTimer = 60000;
        DeadenTimer = 15000;
        SoulShockTimer = 40000;
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if ((damage >= m_creature->GetHealth()) && (done_by != m_creature))
        {
            damage = 0;
            // 10% of total health, signalling time to return
            m_creature->SetHealth(m_creature->GetMaxHealth()/10);
        }
        else
        {
            if (done_by && (done_by->GetTypeId() == TYPEID_PLAYER) && done_by->isAlive())
                done_by->DealDamage(done_by, damage/2, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void KilledUnit(Unit *victim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(DESI_SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(DESI_SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(DESI_SAY_SLAY3, m_creature); break;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if (!m_creature->isInCombat())
                {
                    DoCastSpellIfCan(who, AURA_OF_DESIRE);
                }

                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                AttackStart(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealthPercent() <= 10.0f)
        {
            if (m_creature->getVictim())
                m_creature->DeleteThreatList();             // Delete our threatlist if below 10% as we should no longer attack.
            return;
        }

        if (RuneShieldTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_RUNE_SHIELD);
            RuneShieldTimer = 60000;
        }else RuneShieldTimer -= diff;

        if (DeadenTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEADEN);
            DeadenTimer = urand(30000, 60000);
        }else DeadenTimer -= diff;

        if (SoulShockTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SOUL_SHOCK);
            SoulShockTimer = 40000;

            if (urand(0, 1))
                DoScriptText(DESI_SAY_SPEC, m_creature);

        }else SoulShockTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_essence_of_angerAI : public ScriptedAI
{
    boss_essence_of_angerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 AggroTargetGUID;

    uint32 AggroYellTimer;
    uint32 CheckTankTimer;
    uint32 SoulScreamTimer;
    uint32 SpiteTimer;

    bool CheckedAggro;

    void Reset()
    {
        AggroTargetGUID = 0;

        AggroYellTimer = 5000;
        CheckTankTimer = 5000;
        SoulScreamTimer = 10000;
        SpiteTimer = 30000;

        CheckedAggro = false;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        DoCastSpellIfCan(m_creature->getVictim(), AURA_OF_ANGER, CAST_TRIGGERED);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if (!m_creature->isInCombat())
                {
                    DoCastSpellIfCan(who, AURA_OF_ANGER);
                }

                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                AttackStart(who);
            }
        }
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(ANGER_SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0, 1) ? ANGER_SAY_SLAY1 : ANGER_SAY_SLAY2, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!CheckedAggro)
        {
            AggroTargetGUID = m_creature->getVictim()->GetGUID();
            CheckedAggro = true;
        }

        if (AggroYellTimer)
        {
            if (AggroYellTimer <= diff)
            {
                DoScriptText(ANGER_SAY_FREED2, m_creature);
                AggroYellTimer = 0;
            }else AggroYellTimer -= diff;
        }

        if (CheckTankTimer < diff)
        {
            if (m_creature->getVictim()->GetGUID() != AggroTargetGUID)
            {
                DoScriptText(ANGER_SAY_BEFORE, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_SELF_SEETHE);
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_ENEMY_SEETHE, CAST_TRIGGERED);
                AggroTargetGUID = m_creature->getVictim()->GetGUID();
            }
            CheckTankTimer = 2000;
        }else CheckTankTimer -= diff;

        if (SoulScreamTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SOUL_SCREAM);
            SoulScreamTimer = 10000;
        }else SoulScreamTimer -= diff;

        if (SpiteTimer < diff)
        {
            for(uint8 i = 0; i < 4; ++i)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(target, SPELL_SPITE);
            }

            SpiteTimer = 30000;
            DoScriptText(ANGER_SAY_SPEC, m_creature);
        }else SpiteTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

void npc_enslaved_soulAI::JustDied(Unit *killer)
{
    if (ReliquaryGUID)
    {
        if (Creature* pReliquary = m_creature->GetMap()->GetCreature(ReliquaryGUID))
        {
            if (boss_reliquary_of_soulsAI* pReliqAI = dynamic_cast<boss_reliquary_of_soulsAI*>(pReliquary->AI()))
                pReliqAI->SoulDeathCount++;
        }
    }
}

CreatureAI* GetAI_boss_reliquary_of_souls(Creature* pCreature)
{
    return new boss_reliquary_of_soulsAI(pCreature);
}

CreatureAI* GetAI_boss_essence_of_suffering(Creature* pCreature)
{
    return new boss_essence_of_sufferingAI(pCreature);
}

CreatureAI* GetAI_boss_essence_of_desire(Creature* pCreature)
{
    return new boss_essence_of_desireAI(pCreature);
}

CreatureAI* GetAI_boss_essence_of_anger(Creature* pCreature)
{
    return new boss_essence_of_angerAI(pCreature);
}

CreatureAI* GetAI_npc_enslaved_soul(Creature* pCreature)
{
    return new npc_enslaved_soulAI(pCreature);
}

void AddSC_boss_reliquary_of_souls()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_reliquary_of_souls";
    newscript->GetAI = &GetAI_boss_reliquary_of_souls;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_essence_of_suffering";
    newscript->GetAI = &GetAI_boss_essence_of_suffering;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_essence_of_desire";
    newscript->GetAI = &GetAI_boss_essence_of_desire;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_essence_of_anger";
    newscript->GetAI = &GetAI_boss_essence_of_anger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_enslaved_soul";
    newscript->GetAI = &GetAI_npc_enslaved_soul;
    newscript->RegisterSelf();
}
