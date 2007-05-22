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



#define SPELL_VENOMSPIT    16866
#define SPELL_DAZED    1604
#define SPELL_SLOWINGPOISON    7992

struct MANGOS_DLL_DECL mob_bile_slimeAI : public ScriptedAI
{
    mob_bile_slimeAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 VenomSpit_Timer;
    uint32 Dazed_Timer;
    uint32 SlowingPoison_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        VenomSpit_Timer = 3000;
        Dazed_Timer = 7000;
        SlowingPoison_Timer = 12000;
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

            //VenomSpit
            if (VenomSpit_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_VENOMSPIT);

                //22 seconds until we should cast this again
                VenomSpit_Timer = 22000;
            }else VenomSpit_Timer -= diff;

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 30) //30% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //10 seconds until we should cast this again
                Dazed_Timer = 10000;
            }else Dazed_Timer -= diff;

            //SlowingPoison
            if (SlowingPoison_Timer < diff)
            {
                //Cast
                if (rand()%100 < 30) //30% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_SLOWINGPOISON);
                }
                //15 seconds until we should cast this again
                SlowingPoison_Timer = 15000;
            }else SlowingPoison_Timer -= diff;

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
CreatureAI* GetAI_mob_bile_slime(Creature *_Creature)
{
    return new mob_bile_slimeAI (_Creature);
}


void AddSC_mob_bile_slime()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_bile_slime";
    newscript->GetAI = GetAI_mob_bile_slime;
    m_scripts[nrscripts++] = newscript;
}