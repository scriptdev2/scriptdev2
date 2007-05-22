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

// Status: check for aggro, check for root ok, check for correct spellID

#define ROOTSELF   23973
#define SPELL_SHOOT	15620

struct MANGOS_DLL_DECL mob_crimson_riflemanAI : public ScriptedAI
{
    mob_crimson_riflemanAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Shoot_Timer;
    bool Rooted;
    bool InCombat;

    void EnterEvadeMode()
    {
        Shoot_Timer = 1000;
        Rooted = false;
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
            //Begin ranged attack if we are within range
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
            //double aggro range
            if (m_creature->IsWithinDistInMap(who, attackRadius*2) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin ranged attack if we are within range
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

            SpellEntry const* Shoot_S;
            Shoot_S = GetSpellStore()->LookupEntry(SPELL_SHOOT);
            //Shoot
            if (CanCast(m_creature,Shoot_S) && Shoot_Timer < diff)
            { 
                //Lock our position
                if (!Rooted)
                {
                    m_creature->CastSpell(m_creature,ROOTSELF,true);
                    Rooted = true;
                }
                DoCast(m_creature->getVictim(),SPELL_SHOOT);
                Shoot_Timer = 2000;
            }
            else 
                if (Rooted && !CanCast(m_creature->getVictim(),Shoot_S))
                {
                    m_creature->RemoveAurasDueToSpell(ROOTSELF);    
                    Rooted = false;
                }
                Shoot_Timer -= diff;


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
CreatureAI* GetAI_mob_crimson_rifleman(Creature *_Creature)
{
    return new mob_crimson_riflemanAI (_Creature);
}


void AddSC_mob_crimson_rifleman()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crimson_rifleman";
    newscript->GetAI = GetAI_mob_crimson_rifleman;
    m_scripts[nrscripts++] = newscript;
}