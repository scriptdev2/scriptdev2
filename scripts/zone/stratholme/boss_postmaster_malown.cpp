/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "../../sc_defines.h"

// **** This script is still under Developement ****

//Status: needs revision

//Spell ID to summon this guy is 24627 "Summon Postmaster Malown"
//He should be spawned along with three other elites once the third postbox has been opened

#define SAY_MALOWNED	"You just got MALOWNED!"

#define SPELL_WAILINGDEAD    7713
#define SPELL_BACKHAND    6253
#define SPELL_CURSEOFWEAKNESS    8552
#define SPELL_CURSEOFTONGUES    12889
#define SPELL_CALLOFTHEGRAVE    17831

struct MANGOS_DLL_DECL boss_postmaster_malownAI : public ScriptedAI
{
    boss_postmaster_malownAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 WailingDead_Timer;
    uint32 Backhand_Timer;
    uint32 CurseOfWeakness_Timer;
    uint32 CurseOfTongues_Timer;
    uint32 CallOfTheGrave_Timer;
    bool HasYelled;
    bool InCombat;

    void EnterEvadeMode()
    {
        WailingDead_Timer = 19000; //lasts 6 sec
        Backhand_Timer = 8000; //2 sec stun
        CurseOfWeakness_Timer = 20000; //lasts 2 mins
        CurseOfTongues_Timer = 22000;
        CallOfTheGrave_Timer = 25000;
        HasYelled = false;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
            InCombat = true;
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
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!HasYelled)
                {
                    DoYell(SAY_MALOWNED,LANG_UNIVERSAL,NULL);
                    HasYelled = true;
                }

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //WailingDead
            if (WailingDead_Timer < diff)
            {
                //Cast
                if (rand()%100 < 65) //65% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_WAILINGDEAD);
                }
                //19 seconds until we should cast this again
                WailingDead_Timer = 19000;
            }else WailingDead_Timer -= diff;

            //Backhand
            if (Backhand_Timer < diff)
            {
                //Cast
                if (rand()%100 < 45) //45% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_BACKHAND);
                }
                //8 seconds until we should cast this again
                Backhand_Timer = 8000;
            }else Backhand_Timer -= diff;

            //CurseOfWeakness
            if (CurseOfWeakness_Timer < diff)
            {
                //Cast
                if (rand()%100 < 3) //3% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CURSEOFWEAKNESS);
                }
                //20 seconds until we should cast this again
                CurseOfWeakness_Timer = 20000;
            }else CurseOfWeakness_Timer -= diff;

            //CurseOfTongues
            if (CurseOfTongues_Timer < diff)
            {
                //Cast
                if (rand()%100 < 3) //3% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CURSEOFTONGUES);
                }
                //22 seconds until we should cast this again
                CurseOfTongues_Timer = 22000;
            }else CurseOfTongues_Timer -= diff;

            //CallOfTheGrave
            if (CallOfTheGrave_Timer < diff)
            {
                //Cast
                if (rand()%100 < 5) //5% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CALLOFTHEGRAVE);
                }
                //25 seconds until we should cast this again
                CallOfTheGrave_Timer = 25000;
            }else CallOfTheGrave_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
}; 
CreatureAI* GetAI_boss_postmaster_malown(Creature *_Creature)
{
    return new boss_postmaster_malownAI (_Creature);
}


void AddSC_boss_postmaster_malown()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_postmaster_malown";
    newscript->GetAI = GetAI_boss_postmaster_malown;
    m_scripts[nrscripts++] = newscript;
}