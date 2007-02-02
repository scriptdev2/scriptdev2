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

#define SPELL_IMPENDINGDOOM 19702
#define SPELL_LUCIFRONCURSE 19703
#define SPELL_SHADOWSHOCK   17399

struct MANGOS_DLL_DECL boss_lucifronAI : public ScriptedAI
{
    boss_lucifronAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 ImpendingDoom_Timer;
    uint32 LucifronCurse_Timer;
    uint32 ShadowShock_Timer;

    void Reset()
    {
        pTarget = NULL;
        ImpendingDoom_Timer = 10000;        //Initial cast after 10 seconds so the debuffs alternate
        LucifronCurse_Timer = 20000;        //Initial cast after 20 seconds
        ShadowShock_Timer = 6000;           //6 seconds

        if (m_creature)
        {
            m_creature->RemoveAllAuras();
            DoStopAttack();
            DoGoHome();
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (m_creature->getVictim() == NULL && who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            pTarget = who;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            if ( m_creature->getVictim() == NULL)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                pTarget = who;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //If we had a target and it wasn't cleared then it means the player died from some unknown soruce
        //But we still need to reset
        if (m_creature->isAlive() && pTarget && !m_creature->getVictim())
        {
            Reset();
            return;
        }

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Check if we should stop attacking because our victim is no longer attackable
            if (needToStop())
            {
                Reset();
                return;
            }
            
            //Impending doom timer
            if (ImpendingDoom_Timer < diff)
            {
                //Cast impending doom
                DoCast(m_creature->getVictim(),SPELL_IMPENDINGDOOM);

                //20 seconds until we should cast this agian
                ImpendingDoom_Timer = 20000;
            }else ImpendingDoom_Timer -= diff;

            //Lucifron's curse timer
            if (LucifronCurse_Timer < diff)
            {
                //Cast Lucifron's curse
                DoCast(m_creature->getVictim(),SPELL_LUCIFRONCURSE);

                //20 seconds until we should cast this agian
                LucifronCurse_Timer = 20000;
            }else LucifronCurse_Timer -= diff;

            //Shadowshock
            if (ShadowShock_Timer < diff)
            {
                //Cast shadow shock
                DoCast(m_creature->getVictim(),SPELL_SHADOWSHOCK);

                //6 seconds until we should cast this agian
                ShadowShock_Timer = 6000;
            }else ShadowShock_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    Unit* newtarget = m_creature->SelectHostilTarget();
                    if(newtarget)
                        AttackStart(newtarget);

                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
            
            //If we are still alive and we lose our victim it means we killed them
            if(m_creature->isAlive() && !m_creature->getVictim())
            {
                Reset();
            }
        }
    }
}; 
CreatureAI* GetAI_boss_lucifron(Creature *_Creature)
{
    return new boss_lucifronAI (_Creature);
}


void AddSC_boss_lucifron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lucifron";
    newscript->GetAI = GetAI_boss_lucifron;
    m_scripts[nrscripts++] = newscript;
}