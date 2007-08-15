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

#define SPELL_KNOCKBACK                19813	
#define SPELL_STOMP                    4283                     

    

struct MANGOS_DLL_DECL mob_molten_giantAI : public ScriptedAI
{
    mob_molten_giantAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 KnockBack_Timer;
    uint32 Stomp_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        KnockBack_Timer = 8000;
        Stomp_Timer = 12000;
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
            
	    //KnockBack_Timer
            if (KnockBack_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_KNOCKBACK);

                //15 seconds until we should cast this again
                KnockBack_Timer = 15000;
            }else KnockBack_Timer -= diff;


            //Stomp_Timer
            if (Stomp_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_STOMP);

                //12 seconds until we should cast this agian
                Stomp_Timer = 12000;
            }else Stomp_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_molten_giant(Creature *_Creature)
{
    return new mob_molten_giantAI (_Creature);
}


void AddSC_mob_molten_giant()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_molten_giant";
    newscript->GetAI = GetAI_mob_molten_giant;
    m_scripts[nrscripts++] = newscript;
}
