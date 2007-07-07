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

//Status: flash heal doesnt seem to work. Ready for SVN

#define SPELL_MINDBLAST    17194
#define SPELL_FLASHHEAL    17843
#define SPELL_RENEW    10928

struct MANGOS_DLL_DECL mob_crimson_initiateAI : public ScriptedAI
{
    mob_crimson_initiateAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 MindBlast_Timer;
    uint32 FlashHeal_Timer;
    uint32 Renew_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        MindBlast_Timer = 3000;
        FlashHeal_Timer = 5000;
        Renew_Timer = 12000;
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

            //MindBlast
            if (MindBlast_Timer < diff)
            {
                //Cast
                if (rand()%100 < 90) //90% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_MINDBLAST);
                }
                //7 seconds until we should cast this again
                MindBlast_Timer = 7000;
            }else MindBlast_Timer -= diff;

            //FlashHeal
            if (FlashHeal_Timer < diff && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 20)
            {
                //Cast
                if (rand()%100 < 85) //85% chance to cast
                {
                    DoCast(m_creature,SPELL_FLASHHEAL);
                }
                //6 seconds until we should cast this again
                FlashHeal_Timer = 6000;
            }else FlashHeal_Timer -= diff;

            //Renew
            if (Renew_Timer < diff)
            {
                //Cast
                if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 20)
                {
                    DoCast(m_creature,SPELL_RENEW);
                }
                //12 seconds until we should try cast this again
                Renew_Timer = 12000;
            }else Renew_Timer -= diff;

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
CreatureAI* GetAI_mob_crimson_initiate(Creature *_Creature)
{
    return new mob_crimson_initiateAI (_Creature);
}


void AddSC_mob_crimson_initiate()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crimson_initiate";
    newscript->GetAI = GetAI_mob_crimson_initiate;
    m_scripts[nrscripts++] = newscript;
}