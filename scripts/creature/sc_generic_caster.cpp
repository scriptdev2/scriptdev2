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


#include "../sc_defines.h"

// **** This script is still under Developement ****

struct MANGOS_DLL_DECL generic_casterAI : public ScriptedAI
{
	generic_casterAI(Creature *c) : ScriptedAI(c) {rebufftimer = 0;}

	uint32 rebufftimer;

	void AttackStart(Unit *who)
	{
		if ( m_creature->m_currentSpell == NULL && m_creature->getVictim() == NULL)
		{
			DoSay("Spell cast from recive damage",LANG_UNIVERSAL);
			SpellEntry const *spell_info = m_creature->reachWithSpellAttack(who);
			if (spell_info)
			{
				DoCastSpell(who, spell_info);
				m_creature->Attack(who);
			}
			else DoStartAttack(who);
		}
	}

	void MoveInLineOfSight(Unit *who)
	{
		if (who->GetTypeId() == TYPEID_PLAYER )
		{
			if ( m_creature->getVictim() == NULL )
			{	
				if ( m_creature->m_currentSpell == NULL )
				{
					DoSay("Spell Cast from LOS",LANG_UNIVERSAL);
					SpellEntry const *spell_info = m_creature->reachWithSpellAttack(who);
					if (spell_info)
					{
						DoCastSpell(who, spell_info);
						m_creature->Attack(who);
					}
						else DoStartAttack(who);
				}
				else if ( !who->isInCombat() && !who->isAttacking() )
				{
					DoSay("Stop combat",LANG_UNIVERSAL);
					DoStopAttack();
					DoGoHome();
				}
			}
		}
	}

	void UpdateAI(const uint32 diff)
	{
		//Check if we need to buff if we are not in combat
		if ( !m_creature->isInCombat())
		{
			if (rebufftimer < diff)
			{
				//Cast self buffs here
				DoSay("I'm buffing, la'da'dee...",LANG_UNIVERSAL);

				//give ourselves 1 minute before rebuff agian
				rebufftimer = 60000;
			}else rebufftimer -=diff;
		}

		//Check if we have a current target
		if( m_creature->getVictim() )
		{
			//Check if we are dead or victim is dead
			/*if(needToStop())
			{
				DoStopAttack();
				DoSay("Need to stop combat",LANG_UNIVERSAL);
				if (!m_creature->isAlive())
					return;

				DoGoHome();
			}*/
			
			if( m_creature->IsStopped())
			{
				if( m_creature->isAttackReady())
				{
					Unit* newtarget =  m_creature->SelectHostilTarget();
					if(newtarget)
						m_creature->Attack(newtarget);

					if ( !m_creature->getVictim() )
						return;
					
					if(!m_creature->canReachWithAttack(m_creature->getVictim()))
					{

						SpellEntry const *spell_info = m_creature->reachWithSpellAttack(m_creature->getVictim());
						if (spell_info)
						{
							if ( m_creature->m_currentSpell != NULL)
								return;

							(*m_creature)->Clear(false);
							
							DoSay("Casting spell cuz I can",LANG_UNIVERSAL);
							DoCastSpell(m_creature->getVictim(), spell_info);
						}
						else {
							DoSay("Ran out of range",LANG_UNIVERSAL);
							m_creature->InterruptSpell();
							(*m_creature)->Mutate(new TargetedMovementGenerator(*m_creature->getVictim()));
						}

						m_creature->resetAttackTimer();

					}else
					{
						DoSay("Ran into Melee range",LANG_UNIVERSAL);
						(*m_creature)->Clear(false);
						m_creature->InterruptSpell();
						m_creature->AttackerStateUpdate(m_creature->getVictim());
						m_creature->resetAttackTimer();
					}

					/*if(needToStop())
					{
						DoStopAttack();
						DoGoHome();
					}*/
				}
			}
		}
	}


}; 
CreatureAI* GetAI_generic_caster(Creature *_Creature)
{
	return new generic_casterAI (_Creature);
}


void AddSC_generic_caster()
{
	UnitScript *newscript;
	newscript = new UnitScript;
	newscript->Name="generic_caster";
	newscript->GetAI = GetAI_generic_caster;
	m_scripts[nrscripts++] = newscript;
}
