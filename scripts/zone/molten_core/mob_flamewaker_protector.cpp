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

#define SPELL_CLEAVE                20691	
#define SPELL_DOMINATEMIND                    20740                     

    

struct MANGOS_DLL_DECL mob_flamewaker_protectorAI : public ScriptedAI
{
    mob_flamewaker_protectorAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Cleave_Timer;
    uint32 DominateMind_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Cleave_Timer = 8000;
        DominateMind_Timer = 18000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
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
            
            //Cleave_Timer
            if (Cleave_Timer < diff)
            {
                //Cast
                if (rand()%100 < 60) //60% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CLEAVE);
                }
                //6 seconds until we should cast this again
                Cleave_Timer = 6000;
            }else Cleave_Timer -= diff;


            //DominateMind_Timer
            if (DominateMind_Timer < diff)
            {
                //Cast
                if (rand()%100 < 60) //60% chance to cast
                {
                 //Cast Dominate Mind on a Random target
                 Unit* target = NULL;
 
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_DOMINATEMIND);
                }
                //20 seconds until we should cast this agian
                DominateMind_Timer = 20000;
            }else DominateMind_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_flamewaker_protector(Creature *_Creature)
{
    return new mob_flamewaker_protectorAI (_Creature);
}


void AddSC_mob_flamewaker_protector()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_flamewaker_protector";
    newscript->GetAI = GetAI_mob_flamewaker_protector;
    m_scripts[nrscripts++] = newscript;
}
