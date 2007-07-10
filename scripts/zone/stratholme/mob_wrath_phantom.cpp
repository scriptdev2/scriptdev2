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



#define SPELL_SCREAMSOFTHEPAST    7074
#define SPELL_DAZED    1604
#define SPELL_HAUNTINGPHANTOMS    16336

struct MANGOS_DLL_DECL mob_wrath_phantomAI : public ScriptedAI
{
    mob_wrath_phantomAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ScreamsOfThePast_Timer;
    uint32 Dazed_Timer;
    uint32 HauntingPhantoms_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ScreamsOfThePast_Timer = 4000;
        Dazed_Timer = 7000;
        HauntingPhantoms_Timer = 11000;
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
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

            //ScreamsOfThePast
            if (ScreamsOfThePast_Timer < diff)
            {
                //Cast
                if (rand()%100 < 70) //70% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_SCREAMSOFTHEPAST);
                }
                //13 seconds until we should cast this again
                ScreamsOfThePast_Timer = 13000;
            }else ScreamsOfThePast_Timer -= diff;

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 20) //20% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //15 seconds until we should try cast this again
                Dazed_Timer = 15000;
            }else Dazed_Timer -= diff;

            //HauntingPhantoms
            if (HauntingPhantoms_Timer < diff)
            {
                if (rand()%100 < 10) //10% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_HAUNTINGPHANTOMS);
                }
                //13 seconds until we should try cast this again
                HauntingPhantoms_Timer = 13000;
            }else HauntingPhantoms_Timer -= diff;

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
CreatureAI* GetAI_mob_wrath_phantom(Creature *_Creature)
{
    return new mob_wrath_phantomAI (_Creature);
}


void AddSC_mob_wrath_phantom()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_wrath_phantom";
    newscript->GetAI = GetAI_mob_wrath_phantom;
    m_scripts[nrscripts++] = newscript;
}