/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Gruul
SD%Complete: 50
SDComment: Ground Slam seriously messed up due to core problem
SDCategory: Gruul's Lair
EndScriptData */

#include "precompiled.h"
#include "def_gruuls_lair.h"

enum
{
    SAY_AGGRO                   = -1565010,
    SAY_SLAM1                   = -1565011,
    SAY_SLAM2                   = -1565012,
    SAY_SHATTER1                = -1565013,
    SAY_SHATTER2                = -1565014,
    SAY_SLAY1                   = -1565015,
    SAY_SLAY2                   = -1565016,
    SAY_SLAY3                   = -1565017,
    SAY_DEATH                   = -1565018,

    EMOTE_GROW                  = -1565019,

    SPELL_GROWTH                = 36300,
    SPELL_CAVE_IN               = 36240,
    SPELL_GROUND_SLAM           = 33525,                    //AoE Ground Slam applying Ground Slam to everyone with a script effect (most likely the knock back, we can code it to a set knockback)
    SPELL_REVERBERATION         = 36297,
    SPELL_SHATTER               = 33654,

    SPELL_SHATTER_EFFECT        = 33671,
    SPELL_HURTFUL_STRIKE        = 33813,
    SPELL_STONED                = 33652,                    //Spell is self cast by target
    SPELL_MAGNETIC_PULL         = 28337,
    SPELL_KNOCK_BACK            = 24199,                    //Knockback spell until correct implementation is made

    SPELL_GRONN_LORDS_GRASP     = 33572                     //Triggered by Ground Slam
};

struct MANGOS_DLL_DECL boss_gruulAI : public ScriptedAI
{
    boss_gruulAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiGrowth_Timer;
    uint32 m_uiCaveIn_Timer;
    uint32 m_uiCaveIn_StaticTimer;
    uint32 m_uiGroundSlamTimer;
    uint32 m_uiGroundSlamStage;
    uint32 m_uiHurtfulStrike_Timer;
    uint32 m_uiReverberation_Timer;

    bool m_bPerformingGroundSlam;

    void Reset()
    {
        m_uiGrowth_Timer            = 30000;
        m_uiCaveIn_Timer            = 27000;
        m_uiCaveIn_StaticTimer      = 30000;
        m_uiGroundSlamTimer         = 35000;
        m_uiGroundSlamStage         = 0;
        m_uiHurtfulStrike_Timer     = 8000;
        m_uiReverberation_Timer     = 60000+45000;
        m_bPerformingGroundSlam     = false;
    }

    void Aggro(Unit *pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_GRUUL_EVENT, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GRUUL_EVENT, NOT_STARTED);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_GRUUL_EVENT, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        // Growth
        // Gruul can cast this spell up to 30 times
        if (m_uiGrowth_Timer < uiDiff)
        {
            DoScriptText(EMOTE_GROW, m_creature);
            DoCast(m_creature,SPELL_GROWTH);
            m_uiGrowth_Timer = 30000;
        }
        else
            m_uiGrowth_Timer -= uiDiff;

        if (m_bPerformingGroundSlam)
        {
            if (m_uiGroundSlamTimer < uiDiff)
            {
                switch(m_uiGroundSlamStage)
                {
                    case 0:
                    {
                        //Begin the whole ordeal
                        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();

                        std::vector<Unit*> knockback_targets;

                        //First limit the list to only players
                        for(std::list<HostilReference*>::iterator itr = m_threatlist.begin(); itr!= m_threatlist.end(); ++itr)
                        {
                            Unit *pTarget = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());

                            if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                                knockback_targets.push_back(pTarget);
                        }

                        //Now to totally disorient those players
                        for(std::vector<Unit*>::iterator itr = knockback_targets.begin(); itr!= knockback_targets.end(); ++itr)
                        {
                            Unit *pTarget = *itr;
                            Unit *pTarget2 = *(knockback_targets.begin() + rand()%knockback_targets.size());

                            if (pTarget && pTarget2)
                            {
                                switch(rand()%2)
                                {
                                    case 0: pTarget2->CastSpell(pTarget, SPELL_MAGNETIC_PULL, true, NULL, NULL, m_creature->GetGUID()); break;
                                    case 1: pTarget2->CastSpell(pTarget, SPELL_KNOCK_BACK, true, NULL, NULL, m_creature->GetGUID()); break;
                                }
                            }
                        }

                        m_uiGroundSlamTimer = 7000;
                        break;
                    }

                    case 1:
                    {
                        //Players are going to get stoned
                        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();

                        for(std::list<HostilReference*>::iterator itr = m_threatlist.begin(); itr!= m_threatlist.end(); ++itr)
                        {
                            if (Unit *pTarget = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid()))
                            {
                                pTarget->RemoveAurasDueToSpell(SPELL_GRONN_LORDS_GRASP);
                                pTarget->CastSpell(pTarget, SPELL_STONED, true, NULL, NULL, m_creature->GetGUID());
                            }
                        }

                        m_uiGroundSlamTimer = 5000;
                        break;
                    }

                    case 2:
                    {
                        //The dummy shatter spell is cast
                        DoCast(m_creature, SPELL_SHATTER);
                        m_uiGroundSlamTimer = 1000;
                        break;
                    }

                    case 3:
                    {
                        //Shatter takes effect
                        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();

                        for(std::list<HostilReference*>::iterator itr = m_threatlist.begin(); itr!= m_threatlist.end(); ++itr)
                        {
                            if (Unit *pTarget = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid()))
                            {
                                pTarget->RemoveAurasDueToSpell(SPELL_STONED);

                                if (pTarget->GetTypeId() == TYPEID_PLAYER)
                                    pTarget->CastSpell(pTarget, SPELL_SHATTER_EFFECT, false, NULL, NULL, m_creature->GetGUID());
                            }
                        }

                        m_creature->GetMotionMaster()->Clear();

                        if (Unit *pVictim = m_creature->getVictim())
                        {
                            m_creature->GetMotionMaster()->MoveChase(pVictim);
                            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pVictim->GetGUID());
                        }

                        m_bPerformingGroundSlam = false;
                        m_uiGroundSlamTimer     = 120000;
                        m_uiHurtfulStrike_Timer = 8000;

                        //Give a little time to the players to undo the damage from shatter
                        if (m_uiReverberation_Timer < 10000)
                            m_uiReverberation_Timer += 10000;

                        break;
                    }
                }

                m_uiGroundSlamStage++;
            }
            else
                m_uiGroundSlamTimer-= uiDiff;
        }
        else
        {
            // Hurtful Strike
            if (m_uiHurtfulStrike_Timer < uiDiff)
            {
                // Find 2nd-aggro target within melee range.
                Unit *pTarget = NULL;
                std::list<HostilReference *> t_list = m_creature->getThreatManager().getThreatList();
                std::list<HostilReference *>::iterator itr = t_list.begin();
                std::advance(itr, 1);
                for(; itr!= t_list.end(); ++itr)
                {
                    pTarget = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                    // exclude pets, totems & player out of melee range
                    if (pTarget->GetTypeId() != TYPEID_PLAYER || !pTarget->IsWithinDist(m_creature, ATTACK_DISTANCE, false))
                    {
                        pTarget = NULL;
                        continue;
                    }
                    //we've found someone
                    break;
                }

                if (pTarget)
                    DoCast(pTarget,SPELL_HURTFUL_STRIKE);
                else
                    DoCast(m_creature->getVictim(),SPELL_HURTFUL_STRIKE);

                m_uiHurtfulStrike_Timer = 8000;
            }
            else
                m_uiHurtfulStrike_Timer -= uiDiff;

            // Reverberation
            if (m_uiReverberation_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_REVERBERATION, true);
                m_uiReverberation_Timer = 15000 + rand()%10000;
            }
            else
                m_uiReverberation_Timer -= uiDiff;

            // Cave In
            if (m_uiCaveIn_Timer < uiDiff)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(pTarget,SPELL_CAVE_IN);

                if (m_uiCaveIn_StaticTimer >= 4000)
                    m_uiCaveIn_StaticTimer -= 2000;

                    m_uiCaveIn_Timer = m_uiCaveIn_StaticTimer;

            }
            else
                m_uiCaveIn_Timer -= uiDiff;

            // Ground Slam, Gronn Lord's Grasp, Stoned, Shatter
            if (m_uiGroundSlamTimer < uiDiff)
            {
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);

                m_bPerformingGroundSlam = true;
                m_uiGroundSlamTimer     = 0;
                m_uiGroundSlamStage     = 0;
                DoCast(m_creature->getVictim(), SPELL_GROUND_SLAM);
            }
            else
                m_uiGroundSlamTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_gruul(Creature* pCreature)
{
    return new boss_gruulAI(pCreature);
}

void AddSC_boss_gruul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_gruul";
    newscript->GetAI = &GetAI_boss_gruul;
    newscript->RegisterSelf();
}
