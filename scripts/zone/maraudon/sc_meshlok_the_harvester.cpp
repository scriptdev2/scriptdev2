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

#define SPELL_WARSTOMP             21080    	
#define SPELL_STRIKE                  8817              

struct MANGOS_DLL_DECL meshlok_the_harvesterAI : public ScriptedAI
{
    meshlok_the_harvesterAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 WarStomp_Timer;
    uint32 Strike_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        WarStomp_Timer = 9000;
        Strike_Timer = 7000;
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

            //WarStomp_Timer
            if (WarStomp_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_WARSTOMP);

                //18 seconds
                WarStomp_Timer = 11000;
            }else WarStomp_Timer -= diff;

            //Strike_Timer
            if (Strike_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_STRIKE);

                //12 seconds until we should cast this agian
                Strike_Timer = 9000;
            }else Strike_Timer -= diff;

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
CreatureAI* GetAI_meshlok_the_harvester(Creature *_Creature)
{
    return new meshlok_the_harvesterAI (_Creature);
}


void AddSC_meshlok_the_harvester()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="meshlok_the_harvester";
    newscript->GetAI = GetAI_meshlok_the_harvester;
    m_scripts[nrscripts++] = newscript;
}
