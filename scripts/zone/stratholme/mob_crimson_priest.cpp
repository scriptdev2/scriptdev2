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

//Status: ready for SVN

#define SPELL_HOLYSMITE    25054
#define SPELL_HOLYFIRE    18165
#define SPELL_HOLYLIGHT    15493

struct MANGOS_DLL_DECL mob_crimson_priestAI : public ScriptedAI
{
    mob_crimson_priestAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 HolySmite_Timer;
    uint32 HolyFire_Timer;
    uint32 HolyLight_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        HolySmite_Timer = 2000;
        HolyFire_Timer = 13000;
        HolyLight_Timer = 15000;
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

            //HolySmite
            if (HolySmite_Timer < diff)
            {
                //Cast
                if (rand()%100 < 90) //90% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_HOLYSMITE);
                }
                //4 seconds until we should cast this again
                HolySmite_Timer = 4000;
            }else HolySmite_Timer -= diff;

            //HolyFire
            if (HolyFire_Timer < diff)
            {
                //Cast
                if (rand()%100 < 85) //85% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_HOLYFIRE);
                }
                //13 seconds until we should cast this again
                HolyFire_Timer = 13000;
            }else HolyFire_Timer -= diff;

            //HolyLight
            if (HolyLight_Timer < diff)
            {
                if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 90)
                {
                    DoCast(m_creature,SPELL_HOLYLIGHT);
                }
                //15 seconds until we should cast this again
                HolyLight_Timer = 15000;
            } else HolyLight_Timer-=diff;

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
CreatureAI* GetAI_mob_crimson_priest(Creature *_Creature)
{
    return new mob_crimson_priestAI (_Creature);
}


void AddSC_mob_crimson_priest()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crimson_priest";
    newscript->GetAI = GetAI_mob_crimson_priest;
    m_scripts[nrscripts++] = newscript;
}