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
// 	  ****  made by NuRRi ****
    
#define SPELL_FLAMESHOCK                10448            
#define SPELL_EARTHSHOCK            10414
#define SPELL_GROUNDTREMOR               6524

     

      
struct MANGOS_DLL_DECL boss_lord_roccorAI : public ScriptedAI
{
    boss_lord_roccorAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FlameShock_Timer;
    uint32 EarthShock_Timer;
    uint32 GroundTremor_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        FlameShock_Timer = 18000;
        EarthShock_Timer = 7000;
        GroundTremor_Timer = 14000;
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
            
            //FlameShock_Timer
            if (FlameShock_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FLAMESHOCK);

                //8 seconds until we should cast this agian
                FlameShock_Timer = 8000;
            }else FlameShock_Timer -= diff;

            //EarthShock_Timer
            if (EarthShock_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_EARTHSHOCK);

                //12 seconds until we should cast this agian
                EarthShock_Timer = 12000;
            }else EarthShock_Timer -= diff;

            //GroundTremor_Timer
            if (GroundTremor_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_GROUNDTREMOR);

                //15 seconds
               GroundTremor_Timer = 15000;
            }else GroundTremor_Timer -= diff;



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
CreatureAI* GetAI_boss_lord_roccor(Creature *_Creature)
{
    return new boss_lord_roccorAI (_Creature);
}


void AddSC_boss_lord_roccor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_roccor";
    newscript->GetAI = GetAI_boss_lord_roccor;
    m_scripts[nrscripts++] = newscript;
}
