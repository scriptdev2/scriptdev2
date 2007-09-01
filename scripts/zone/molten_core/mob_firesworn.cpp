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

#define SPELL_IMMOLATE                20294	
#define SPELL_ERUPTION                19497       
#define SPELL_DEATHTOUCH                  5              

    

struct MANGOS_DLL_DECL mob_fireswornAI : public ScriptedAI
{
    mob_fireswornAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Immolate_Timer;
    uint32 Eruption_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Immolate_Timer = 2000;
        Eruption_Timer = 0;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FIRE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BLEED, true);
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


    void JustDied(Unit* Killer)
    {
       DoCast(m_creature->getVictim(),SPELL_ERUPTION);
    }



    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            
            //Immolate_Timer
            if (Immolate_Timer < diff)
            {
                //Cast
                if (rand()%100 < 70) //70% chance to cast
                {
                 //Cast Immolate on a Random target
                 Unit* target = NULL;
 
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_IMMOLATE);
                }
                //5 seconds until we should cast this again
                Immolate_Timer = 5000;
            }else Immolate_Timer -= diff;


            //Casting Eruption on HP <= 3% and Deathtouch to kill the firesworn.
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 3 )
            {
               if (Eruption_Timer < diff)
               {
                 //Cast
                 DoCast(m_creature->getVictim(),SPELL_ERUPTION);
                 DoCast(m_creature,SPELL_DEATHTOUCH);

                 //12 seconds until we should cast this again
                 Eruption_Timer = 12000;
               }else Eruption_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    }
}; 

CreatureAI* GetAI_mob_firesworn(Creature *_Creature)
{
    return new mob_fireswornAI (_Creature);
}

void AddSC_mob_firesworn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_firesworn";
    newscript->GetAI = GetAI_mob_firesworn;
    m_scripts[nrscripts++] = newscript;
}
