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

//*** NEEDS ADDITIOINAL REVIEW ***

#include "../../sc_defines.h"

#define SPELL_SHADOWFLAME        22539
#define SPELL_WINGBUFFET         23339
#define SPELL_FLAMEBUFFET         23341

struct MANGOS_DLL_DECL boss_firemawAI : public ScriptedAI
{
    boss_firemawAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 ShadowFlame_Timer;
    uint32 WingBuffet_Timer;
    uint32 FlameBuffet_Timer;
    

    void Reset()
    {
        pTarget = NULL;
        ShadowFlame_Timer = 45000;      //These times are probably wrong
        WingBuffet_Timer = 25000;
        FlameBuffet_Timer = 5000;
        

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
            
            //ShadowFlame_Timer
            if (ShadowFlame_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SHADOWFLAME);

                //45 seconds till recast
                ShadowFlame_Timer = 45000;
            }else ShadowFlame_Timer -= diff;

            //WingBuffet_Timer
            if (WingBuffet_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_WINGBUFFET);

                //25 seconds till recast
                WingBuffet_Timer = 25000;
            }else WingBuffet_Timer -= diff;
            
            //FlameBuffet_Timer
            if (FlameBuffet_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FLAMEBUFFET);

                //cast this every 5 seconds
                FlameBuffet_Timer = 5000;
            }else FlameBuffet_Timer -= diff;
            
            
            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
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
CreatureAI* GetAI_boss_firemaw(Creature *_Creature)
{
    return new boss_firemawAI (_Creature);
}


void AddSC_boss_firemaw()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_firemaw";
    newscript->GetAI = GetAI_boss_firemaw;
    m_scripts[nrscripts++] = newscript;
}