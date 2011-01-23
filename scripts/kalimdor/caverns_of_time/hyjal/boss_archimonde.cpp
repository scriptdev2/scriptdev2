/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Archimonde
SD%Complete: 85
SDComment: Doomfires not completely offlike due to core limitations for random moving. Tyrande and second phase not fully implemented.
SDCategory: Caverns of Time, Mount Hyjal
EndScriptData */

#include "precompiled.h"
#include "hyjal.h"
#include "SpellAuras.h"

//text id -1534018 are the text used when previous events complete. Not part of this script.
#define SAY_AGGRO                   -1534019
#define SAY_DOOMFIRE1               -1534020
#define SAY_DOOMFIRE2               -1534021
#define SAY_AIR_BURST1              -1534022
#define SAY_AIR_BURST2              -1534023
#define SAY_SLAY1                   -1534024
#define SAY_SLAY2                   -1534025
#define SAY_SLAY3                   -1534026
#define SAY_ENRAGE                  -1534027
#define SAY_DEATH                   -1534028
#define SAY_SOUL_CHARGE1            -1534029
#define SAY_SOUL_CHARGE2            -1534030

#define SPELL_DENOUEMENT_WISP       32124
#define SPELL_ANCIENT_SPARK         39349
#define SPELL_PROTECTION_OF_ELUNE   38528

#define SPELL_DRAIN_WORLD_TREE      39140
#define SPELL_DRAIN_WORLD_TREE_2    39141

#define SPELL_FINGER_OF_DEATH       31984
#define SPELL_HAND_OF_DEATH         35354
#define SPELL_AIR_BURST             32014
#define SPELL_GRIP_OF_THE_LEGION    31972
#define SPELL_DOOMFIRE_STRIKE       31903                   //summons two creatures
#define SPELL_DOOMFIRE_SPAWN        32074
#define SPELL_DOOMFIRE              31945
#define SPELL_SOUL_CHARGE_YELLOW    32045
#define SPELL_SOUL_CHARGE_GREEN     32051
#define SPELL_SOUL_CHARGE_RED       32052
#define SPELL_UNLEASH_SOUL_YELLOW   32054
#define SPELL_UNLEASH_SOUL_GREEN    32057
#define SPELL_UNLEASH_SOUL_RED      32053
#define SPELL_FEAR                  31970

#define CREATURE_ARCHIMONDE             17968
#define CREATURE_DOOMFIRE               18095
#define CREATURE_DOOMFIRE_SPIRIT        18104
#define CREATURE_ANCIENT_WISP           17946
#define CREATURE_CHANNEL_TARGET         22418

#define NORDRASSIL_X        5503.713f
#define NORDRASSIL_Y       -3523.436f
#define NORDRASSIL_Z        1608.781f

struct mob_ancient_wispAI : public ScriptedAI
{
    mob_ancient_wispAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        ArchimondeGUID = 0;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint64 ArchimondeGUID;
    uint32 CheckTimer;

    void Reset()
    {
        CheckTimer = 1000;

        if (m_pInstance)
            ArchimondeGUID = m_pInstance->GetData64(DATA_ARCHIMONDE);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageTaken(Unit* done_by, uint32 &damage) { damage = 0; }

    void UpdateAI(const uint32 diff)
    {
        if (CheckTimer < diff)
        {
            if (Creature* Archimonde = m_creature->GetMap()->GetCreature(ArchimondeGUID))
            {
                if (Archimonde->GetHealthPercent() < 2.0f || !Archimonde->isAlive())
                    DoCastSpellIfCan(m_creature, SPELL_DENOUEMENT_WISP);
                else
                    DoCastSpellIfCan(Archimonde, SPELL_ANCIENT_SPARK);
            }
            CheckTimer = 1000;
        }else CheckTimer -= diff;
    }
};

/* This script is merely a placeholder for the Doomfire that triggers Doomfire spell. It will
   MoveChase the Doomfire Spirit always, until despawn (AttackStart is called upon it's spawn) */
struct MANGOS_DLL_DECL mob_doomfireAI : public ScriptedAI
{
    mob_doomfireAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() { }

    void MoveInLineOfSight(Unit* who) { }
    void DamageTaken(Unit *done_by, uint32 &damage) { damage = 0; }
};

/* This is the script for the Doomfire Spirit Mob. This mob simply follow players or
   travels in random directions if target cannot be found. */
struct MANGOS_DLL_DECL mob_doomfire_targettingAI : public ScriptedAI
{
    mob_doomfire_targettingAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint64 TargetGUID;
    uint32 ChangeTargetTimer;

    void Reset()
    {
        TargetGUID = 0;
        ChangeTargetTimer = 5000;
    }

    void MoveInLineOfSight(Unit* who)
    {
        //will update once TargetGUID is 0. In case noone actually moves(not likely) and this is 0
        //when UpdateAI needs it, it will be forced to select randomPoint
        if (!TargetGUID && who->GetTypeId() == TYPEID_PLAYER)
            TargetGUID = who->GetGUID();
    }

    void DamageTaken(Unit *done_by, uint32 &damage) { damage = 0; }

    void UpdateAI(const uint32 diff)
    {
        if (ChangeTargetTimer < diff)
        {
            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(TargetGUID))
            {
                m_creature->GetMotionMaster()->MoveFollow(pPlayer, 0.0f, 0.0f);
                TargetGUID = 0;
            }
            else
            {
                float x,y,z = 0.0;
                m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 40, x, y, z);
                m_creature->GetMotionMaster()->MovePoint(0, x, y, z);
            }

            ChangeTargetTimer = 5000;
        }else ChangeTargetTimer -= diff;
    }
};

/* Finally, Archimonde's script. His script isn't extremely complex, most are simply spells on timers.
   The only complicated aspect of the battle is Finger of Death and Doomfire, with Doomfire being the
   hardest bit to code. Finger of Death is simply a distance check - if no one is in melee range, then
   select a random target and cast the spell on them. However, if someone IS in melee range, and this
   is NOT the main tank (creature's victim), then we aggro that player and they become the new victim.
   For Doomfire, we summon a mob (Doomfire Spirit) for the Doomfire mob to follow. It's spirit will
   randomly select it's target to follow and then we create the random movement making it unpredictable. */

struct MANGOS_DLL_DECL boss_archimondeAI : public ScriptedAI
{
    boss_archimondeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 DoomfireSpiritGUID;
    uint64 WorldTreeGUID;

    uint32 DrainNordrassilTimer;
    uint32 FearTimer;
    uint32 AirBurstTimer;
    uint32 GripOfTheLegionTimer;
    uint32 DoomfireTimer;
    uint32 MeleeRangeCheckTimer;
    uint32 HandOfDeathTimer;
    uint32 SummonWispTimer;
    uint32 WispCount;
    uint32 EnrageTimer;
    uint32 CheckDistanceTimer;

    bool Enraged;
    bool BelowTenPercent;
    bool HasProtected;
    bool IsChanneling;

    void Reset()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARCHIMONDE, NOT_STARTED);

        DoomfireSpiritGUID = 0;
        WorldTreeGUID = 0;

        DrainNordrassilTimer = 0;
        FearTimer = 42000;
        AirBurstTimer = 30000;
        GripOfTheLegionTimer = urand(5000, 25000);
        DoomfireTimer = 20000;
        MeleeRangeCheckTimer = 15000;
        HandOfDeathTimer = 2000;
        WispCount = 0;                                      // When ~30 wisps are summoned, Archimonde dies
        EnrageTimer = 600000;                               // 10 minutes
        CheckDistanceTimer = 30000;                         // This checks if he's too close to the World Tree (75 yards from a point on the tree), if true then he will enrage

        Enraged = false;
        BelowTenPercent = false;
        HasProtected = false;
        IsChanneling = false;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARCHIMONDE, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }

        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        switch(pVictim->getClass())
        {
            case CLASS_PRIEST:
            case CLASS_PALADIN:
            case CLASS_WARLOCK:
                pVictim->CastSpell(m_creature, SPELL_SOUL_CHARGE_RED, true);
                break;
            case CLASS_MAGE:
            case CLASS_ROGUE:
            case CLASS_WARRIOR:
                pVictim->CastSpell(m_creature, SPELL_SOUL_CHARGE_YELLOW, true);
                break;
            case CLASS_DRUID:
            case CLASS_SHAMAN:
            case CLASS_HUNTER:
                pVictim->CastSpell(m_creature, SPELL_SOUL_CHARGE_GREEN, true);
                break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARCHIMONDE, DONE);
    }

    bool CanUseFingerOfDeath()
    {
        // First we check if our current victim is in melee range or not.
        Unit* victim = m_creature->getVictim();
        if (victim && m_creature->IsWithinDistInMap(victim, m_creature->GetAttackDistance(victim)))
            return false;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        if (tList.empty())
            return false;

        std::list<Unit*> targets;

        for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());

            if (pUnit && pUnit->isAlive())
                targets.push_back(pUnit);
        }

        if (targets.empty())
            return false;

        targets.sort(ObjectDistanceOrder(m_creature));
        Unit* target = targets.front();
        if (target)
        {
            if (!m_creature->IsWithinDistInMap(target, m_creature->GetAttackDistance(target)))
                return true;                                // Cast Finger of Death
            else                                            // This target is closest, he is our new tank
                m_creature->AddThreat(target, m_creature->getThreatManager().getThreat(m_creature->getVictim()));
        }

        return false;
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == CREATURE_ANCIENT_WISP)
            pSummoned->AI()->AttackStart(m_creature);
        else
        {
            pSummoned->setFaction(m_creature->getFaction());
            pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        if (pSummoned->GetEntry() == CREATURE_DOOMFIRE_SPIRIT)
        {
            DoomfireSpiritGUID = pSummoned->GetGUID();
        }

        if (pSummoned->GetEntry() == CREATURE_DOOMFIRE)
        {
            pSummoned->CastSpell(pSummoned,SPELL_DOOMFIRE_SPAWN,false);
            pSummoned->CastSpell(pSummoned,SPELL_DOOMFIRE,true,0,0,m_creature->GetGUID());

            if (Creature* pDoomfireSpirit = m_creature->GetMap()->GetCreature(DoomfireSpiritGUID))
            {
                pSummoned->GetMotionMaster()->MoveFollow(pDoomfireSpirit,0.0f,0.0f);
                DoomfireSpiritGUID = 0;
            }
        }
    }

    //this is code doing close to what the summoning spell would do (spell 31903)
    void SummonDoomfire(Unit* target)
    {
        m_creature->SummonCreature(CREATURE_DOOMFIRE_SPIRIT,
            target->GetPositionX()+15.0,target->GetPositionY()+15.0,target->GetPositionZ(),0,
            TEMPSUMMON_TIMED_DESPAWN, 27000);

        m_creature->SummonCreature(CREATURE_DOOMFIRE,
            target->GetPositionX()-15.0,target->GetPositionY()-15.0,target->GetPositionZ(),0,
            TEMPSUMMON_TIMED_DESPAWN, 27000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->isInCombat())
        {
            if (m_pInstance)
            {
                // Do not let the raid skip straight to Archimonde. Visible and hostile ONLY if Azagalor is finished.
                if ((m_pInstance->GetData(TYPE_AZGALOR) < DONE) && ((m_creature->GetVisibility() != VISIBILITY_OFF) || (m_creature->getFaction() != 35)))
                {
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    m_creature->setFaction(35);
                }
                else if ((m_pInstance->GetData(TYPE_AZGALOR) >= DONE) && ((m_creature->GetVisibility() != VISIBILITY_ON) || (m_creature->getFaction() == 35)))
                {
                    m_creature->setFaction(1720);
                    m_creature->SetVisibility(VISIBILITY_ON);
                }
            }

            if (DrainNordrassilTimer < diff)
            {
                if (!IsChanneling)
                {
                    Creature *temp = m_creature->SummonCreature(CREATURE_CHANNEL_TARGET, NORDRASSIL_X, NORDRASSIL_Y, NORDRASSIL_Z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 1200000);

                    if (temp)
                        WorldTreeGUID = temp->GetGUID();

                    if (Creature *Nordrassil = m_creature->GetMap()->GetCreature(WorldTreeGUID))
                    {
                        Nordrassil->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        Nordrassil->SetDisplayId(11686);
                        DoCastSpellIfCan(Nordrassil, SPELL_DRAIN_WORLD_TREE);
                        IsChanneling = true;
                    }
                }

                if (Creature *Nordrassil = m_creature->GetMap()->GetCreature(WorldTreeGUID))
                {
                    Nordrassil->CastSpell(m_creature, SPELL_DRAIN_WORLD_TREE_2, true);
                    DrainNordrassilTimer = 1000;
                }
            }else DrainNordrassilTimer -= diff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealthPercent() < 10.0f && !BelowTenPercent && !Enraged)
            BelowTenPercent = true;

        if (!Enraged)
        {
            if (EnrageTimer < diff)
            {
                if (m_creature->GetHealthPercent() > 10.0f)
                {
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->MoveIdle();
                    Enraged = true;
                    DoScriptText(SAY_ENRAGE, m_creature);
                }
            }else EnrageTimer -= diff;

            if (CheckDistanceTimer < diff)
            {
                // To simplify the check, we simply summon a creature in the location and then check how far we are from the creature
                Creature* Check = m_creature->SummonCreature(CREATURE_CHANNEL_TARGET, NORDRASSIL_X, NORDRASSIL_Y, NORDRASSIL_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 2000);
                if (Check)
                {
                    Check->SetVisibility(VISIBILITY_OFF);

                    if (m_creature->IsWithinDistInMap(Check, 75))
                    {
                        m_creature->GetMotionMaster()->Clear(false);
                        m_creature->GetMotionMaster()->MoveIdle();
                        Enraged = true;
                        DoScriptText(SAY_ENRAGE, m_creature);
                    }
                }
                CheckDistanceTimer = 5000;
            }else CheckDistanceTimer -= diff;
        }

        if (BelowTenPercent)
        {
            if (!HasProtected)
            {
                m_creature->GetMotionMaster()->Clear(false);
                m_creature->GetMotionMaster()->MoveIdle();

                //all members of raid must get this buff
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_PROTECTION_OF_ELUNE);
                HasProtected = true;
                Enraged = true;
            }

            if (SummonWispTimer < diff)
            {
                DoSpawnCreature(CREATURE_ANCIENT_WISP, rand()%40, rand()%40, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                SummonWispTimer = 1500;
                ++WispCount;
            }else SummonWispTimer -= diff;

            if (WispCount >= 30)
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }

        if (Enraged)
        {
            if (HandOfDeathTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_HAND_OF_DEATH);
                HandOfDeathTimer = 2000;
            }else HandOfDeathTimer -= diff;
            return;                                         // Don't do anything after this point.
        }

        if (GripOfTheLegionTimer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_GRIP_OF_THE_LEGION);

            GripOfTheLegionTimer = urand(5000, 25000);
        }else GripOfTheLegionTimer -= diff;

        if (AirBurstTimer < diff)
        {
            if (!urand(0, 1))
                DoScriptText(SAY_AIR_BURST1, m_creature);
            else
                DoScriptText(SAY_AIR_BURST2, m_creature);

            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            DoCastSpellIfCan(pTarget, SPELL_AIR_BURST);
            AirBurstTimer = urand(25000, 40000);
        }else AirBurstTimer -= diff;

        if (FearTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FEAR);
            FearTimer = 42000;
        }else FearTimer -= diff;

        if (DoomfireTimer < diff)
        {
            if (!urand(0, 1))
                DoScriptText(SAY_DOOMFIRE1, m_creature);
            else
                DoScriptText(SAY_DOOMFIRE2, m_creature);

            Unit *temp = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
            if (!temp)
                temp = m_creature->getVictim();

            //replace with spell cast 31903 once implicitTarget 73 implemented
            SummonDoomfire(temp);

            //supposedly three doomfire can be up at the same time
            DoomfireTimer = 20000;
        }else DoomfireTimer -= diff;

        if (MeleeRangeCheckTimer < diff)
        {
            if (CanUseFingerOfDeath())
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_FINGER_OF_DEATH);

                MeleeRangeCheckTimer = 1000;
            }

            MeleeRangeCheckTimer = 5000;
        }else MeleeRangeCheckTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_archimonde(Creature* pCreature)
{
    return new boss_archimondeAI(pCreature);
}

CreatureAI* GetAI_mob_doomfire(Creature* pCreature)
{
    return new mob_doomfireAI(pCreature);
}

CreatureAI* GetAI_mob_doomfire_targetting(Creature* pCreature)
{
    return new mob_doomfire_targettingAI(pCreature);
}

CreatureAI* GetAI_mob_ancient_wisp(Creature* pCreature)
{
    return new mob_ancient_wispAI(pCreature);
}

void AddSC_boss_archimonde()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_archimonde";
    newscript->GetAI = &GetAI_boss_archimonde;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_doomfire";
    newscript->GetAI = &GetAI_mob_doomfire;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_doomfire_targetting";
    newscript->GetAI = &GetAI_mob_doomfire_targetting;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ancient_wisp";
    newscript->GetAI = &GetAI_mob_ancient_wisp;
    newscript->RegisterSelf();
}
