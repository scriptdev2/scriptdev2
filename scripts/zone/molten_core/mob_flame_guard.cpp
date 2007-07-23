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

#define SPELL_FIRESHIELD                19626	
#define SPELL_FLAME                     19628                     
#define SPELL_CONEOFFIRE                19630


struct MANGOS_DLL_DECL mob_flame_guardAI : public ScriptedAI
{
    mob_flame_guardAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FireShield_Timer;
    uint32 Flame_Timer;
    uint32 ConeOfFire_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        FireShield_Timer = 2500;
        Flame_Timer = 12000;
        ConeOfFire_Timer = 6000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);       
	m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
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
            
            //FireShield_Timer
            if (FireShield_Timer < diff)
            {

                    DoCast(m_creature,SPELL_FIRESHIELD);

                //60 seconds until we should cast this again
                FireShield_Timer = 60000;
            }else FireShield_Timer -= diff;


            //Flame_Timer
            if (Flame_Timer < diff)
            {
                //Cast
                if (rand()%100 < 65) //65% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_FLAME);
                }
                //7seconds until we should cast this agian
                Flame_Timer = 7000;
            }else Flame_Timer -= diff;

            //ConeOfFire_Timer
            if (ConeOfFire_Timer < diff)
            {
                //Cast
                if (rand()%100 < 65) //65% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CONEOFFIRE);
                }
                //11 seconds until we should cast this again
                ConeOfFire_Timer = 11000;
            }else ConeOfFire_Timer -= diff;


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
CreatureAI* GetAI_mob_flame_guard(Creature *_Creature)
{
    return new mob_flame_guardAI (_Creature);
}


void AddSC_mob_flame_guard()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_flame_guard";
    newscript->GetAI = GetAI_mob_flame_guard;
    m_scripts[nrscripts++] = newscript;
}
