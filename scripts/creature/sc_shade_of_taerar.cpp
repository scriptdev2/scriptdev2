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

#include "../sc_defines.h"

#define SPELL_POSIONCLOUD                24840  	
#define SPELL_POSIONBREATH             20667              

struct MANGOS_DLL_DECL boss_shadeoftaerarAI : public ScriptedAI
{
    boss_shadeoftaerarAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 PoisonCloud_Timer;
    uint32 PosionBreath_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        PoisonCloud_Timer = 10000;
        PosionBreath_Timer = 18000;
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
            
            //PoisonCloud_Timer
            if (PoisonCloud_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_POSIONCLOUD);

                //30 seconds
                PoisonCloud_Timer = 30000;
            }else PoisonCloud_Timer -= diff;

            //PosionBreath_Timer
            if (PosionBreath_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_POSIONBREATH);

                //15 seconds until we should cast this agian
                PosionBreath_Timer = 15000;
            }else PosionBreath_Timer -= diff;

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
CreatureAI* GetAI_boss_shadeoftaerar(Creature *_Creature)
{
    return new boss_shadeoftaerarAI (_Creature);
}


void AddSC_boss_shadeoftaerar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_shade_of_taerar";
    newscript->GetAI = GetAI_boss_shadeoftaerar;
    m_scripts[nrscripts++] = newscript;
}
