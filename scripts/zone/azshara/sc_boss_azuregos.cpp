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

#define SPELL_MARKOFFROST                23183            
#define SPELL_MANASTORM            21097
#define SPELL_REFLECT            21118               //Perhaps not right ID
#define SPELL_CLEAVE           8255                //Perhaps not right ID
#define SPELL_ENRAGE             23537

// TELEPORT SPELL NOT INCLUDED

struct MANGOS_DLL_DECL boss_azuregosAI : public ScriptedAI
{
    boss_azuregosAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 MarkOfFrost_Timer;
    uint32 ManaStorm_Timer;
    uint32 Reflect_Timer;
    uint32 Cleave_Timer;
    uint32 Enrage_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        MarkOfFrost_Timer = 45000;
        ManaStorm_Timer = 25000;
        Reflect_Timer = 30000;
        Cleave_Timer = 15000;
        Enrage_Timer = 0;
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

            //MarkOfFrost_Timer
            if (MarkOfFrost_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_MARKOFFROST);

                //30 seconds
                MarkOfFrost_Timer = 30000;
            }else MarkOfFrost_Timer -= diff;

            //ManaStorm_Timer
            if (ManaStorm_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_MANASTORM);

                //15 seconds until we should cast this agian
                ManaStorm_Timer = 15000;
            }else ManaStorm_Timer -= diff;


            //Reflect_Timer
            if (Reflect_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_REFLECT);

                //45 seconds until we should cast this agian
                Reflect_Timer = 45000;
            }else Reflect_Timer -= diff;

            //Cleave_Timer
            if (Cleave_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CLEAVE);

                //12 seconds until we should cast this agian
                Cleave_Timer = 12000;
            }else Cleave_Timer -= diff;


            //Enrage_Timer
            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 26)
            {
                //Cast
                DoCast(m_creature,SPELL_ENRAGE);                    

                //22 seconds until we should cast this agian
                Enrage_Timer = 30000;  
            }else Enrage_Timer -= diff;


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
CreatureAI* GetAI_boss_azuregos(Creature *_Creature)
{
    return new boss_azuregosAI (_Creature);
}


void AddSC_boss_azuregos()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_azuregos";
    newscript->GetAI = GetAI_boss_azuregos;
    m_scripts[nrscripts++] = newscript;
}
