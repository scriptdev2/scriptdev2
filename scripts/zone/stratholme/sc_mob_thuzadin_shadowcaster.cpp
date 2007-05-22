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



#define SPELL_SHADOWBOLT    11660
#define SPELL_CRIPPLE    11443
#define SPELL_PIERCINGSHADOW    16429 //Curse, reduces Shadow Resistance by 100, lasts 30 mins.

struct MANGOS_DLL_DECL mob_thuzadin_shadowcasterAI : public ScriptedAI
{
    mob_thuzadin_shadowcasterAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ShadowBolt_Timer;
    uint32 Cripple_Timer;
    uint32 PiercingShadow_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ShadowBolt_Timer = 2000;
        Cripple_Timer = 11000;
        PiercingShadow_Timer = 16000;
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

            //ShadowBolt
            if (ShadowBolt_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SHADOWBOLT);
                //4 seconds until we should cast this again
                ShadowBolt_Timer = 4000;
            }else ShadowBolt_Timer -= diff;

            //Cripple
            if (Cripple_Timer < diff)
            {
                //Cast
                if (rand()%100 < 75) //75% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CRIPPLE);
                }
                //25 seconds until we should try cast this again
                Cripple_Timer = 25000;
            }else Cripple_Timer -= diff;

            //PiercingShadow
            if (PiercingShadow_Timer < diff)
            {
                //Cast
                if (rand()%100 < 55) //55% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_PIERCINGSHADOW);
                }
                //45 seconds until we should cast this again
                PiercingShadow_Timer = 45000;
            }else PiercingShadow_Timer -= diff;

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
CreatureAI* GetAI_mob_thuzadin_shadowcaster(Creature *_Creature)
{
    return new mob_thuzadin_shadowcasterAI (_Creature);
}


void AddSC_mob_thuzadin_shadowcaster()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_thuzadin_shadowcaster";
    newscript->GetAI = GetAI_mob_thuzadin_shadowcaster;
    m_scripts[nrscripts++] = newscript;
}