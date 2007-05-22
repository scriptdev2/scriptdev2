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



#define SPELL_FROSTNOVA    14907
#define SPELL_DAZED    1604
#define SPELL_FROSTSHOCK    15089

struct MANGOS_DLL_DECL mob_vengeful_phantomAI : public ScriptedAI
{
    mob_vengeful_phantomAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FrostNova_Timer;
    uint32 Dazed_Timer;
    uint32 FrostShock_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        FrostNova_Timer = 6000;
        Dazed_Timer = 2000;
        FrostShock_Timer = 10000;
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

            //FrostNova
            if (FrostNova_Timer < diff)
            {
                //Cast
                if (rand()%100 < 35) //35% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_FROSTNOVA);
                }
                //21 seconds until we should cast this again
                FrostNova_Timer = 21000;
            }else FrostNova_Timer -= diff;

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 20) //20% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //12 seconds until we should try cast this again
                Dazed_Timer = 12000;
            }else Dazed_Timer -= diff;

            //FrostShock
            if (FrostShock_Timer < diff)
            {
                if (rand()%100 < 20) //20% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_FROSTSHOCK);
                }
                //15 seconds until we should try cast this again
                FrostShock_Timer = 15000;
            }else FrostShock_Timer -= diff;

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
CreatureAI* GetAI_mob_vengeful_phantom(Creature *_Creature)
{
    return new mob_vengeful_phantomAI (_Creature);
}


void AddSC_mob_vengeful_phantom()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_vengeful_phantom";
    newscript->GetAI = GetAI_mob_vengeful_phantom;
    m_scripts[nrscripts++] = newscript;
}