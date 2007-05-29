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

#define SPELL_HANDOFTHAURISSAN                17492            
#define SPELL_CLEAVE            20691
#define SPELL_MORTALSTRIKE               24573

struct MANGOS_DLL_DECL boss_draganthaurissanAI : public ScriptedAI
{
    boss_draganthaurissanAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 HandOfThaurissan_Timer;
    uint32 Cleave_Timer;
    uint32 MortalStrike_Timer;
    uint32 Counter;
    bool InCombat;

    void EnterEvadeMode()
    {       
        HandOfThaurissan_Timer = 3000;
        Cleave_Timer = 12000;
        MortalStrike_Timer = 18000;
        Counter= 0;
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

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

            if (HandOfThaurissan_Timer < diff)
            {

                 //Cast Hand of Thaurissan on a Random target
                 Unit* target = NULL;
 
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_HANDOFTHAURISSAN);

                               
                  //3 Hands of Thaurissan will be casted
                  if (Counter < 3)
                  {
                    HandOfThaurissan_Timer = 1000;
                    Counter++;
                  }
                  else {
                	
			//20 seconds until we should cast this again
                	HandOfThaurissan_Timer = 20000;
			Counter=0;
                  }

            }else HandOfThaurissan_Timer -= diff;

            //Cleave_Timer
            if (Cleave_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CLEAVE);

                //10 seconds until we should cast this agian
                Cleave_Timer = 14000;
            }else Cleave_Timer -= diff;

            //MortalStrike_Timer
            if (MortalStrike_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_MORTALSTRIKE);

                //16 seconds
                MortalStrike_Timer = 16000;
            }else MortalStrike_Timer -= diff;



            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DIST))
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
CreatureAI* GetAI_boss_draganthaurissan(Creature *_Creature)
{
    return new boss_draganthaurissanAI (_Creature);
}


void AddSC_boss_draganthaurissan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_emperor_dagran_thaurissan";
    newscript->GetAI = GetAI_boss_draganthaurissan;
    m_scripts[nrscripts++] = newscript;
}