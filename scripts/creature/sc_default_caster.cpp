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

////////////////////////////////////
//      SCRIPT NEEDS TESTING      //
////////////////////////////////////

struct MANGOS_DLL_DECL default_casterAI : public ScriptedAI
{
	default_casterAI(Creature *c) : ScriptedAI(c) {}
	void AttackStart(Unit *who)
	{
		if ( m_creature->m_currentSpell == NULL )
		{
			SpellEntry const *spell_info = m_creature->reachWithSpellAttack(who);
			DoCastSpell(who, spell_info);
		}
		DoStartAttack(who);
	}

	void MoveInLineOfSight(Unit *who)
	{
		if (who->GetTypeId() == TYPEID_PLAYER )
		{
			if ( m_creature->getVictim() != NULL )
			{	
				if ( m_creature->m_currentSpell == NULL )
				{
					SpellEntry const *spell_info = m_creature->reachWithSpellAttack(who);
					DoCastSpell(who, spell_info);
				}
				else if ( !who->isInCombat() && !who->isAttacking() )
				{
					m_creature->CombatStop();
					DoGoHome();
				}
			}
			DoStartAttack(who);
		}
	}


}; 
CreatureAI* GetAI_default_caster(Creature *_Creature)
{
	return new default_casterAI (_Creature);
}


void AddSC_default_caster()
{
	UnitScript *newscript;
	newscript = new UnitScript;
	newscript->Name="default_caster";
	newscript->GetAI = GetAI_default_caster;
	m_scripts[nrscripts++] = newscript;
}
