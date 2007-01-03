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
//      SCRIPT IS FOR TESTING     //
////////////////////////////////////

struct MANGOS_DLL_DECL defaultAI : public ScriptedAI
{
	defaultAI(Creature *c) : ScriptedAI(c) {}

}; 
CreatureAI* GetAI_default(Creature *_Creature)
{
	return new defaultAI (_Creature);
}


void AddSC_default()
{
	UnitScript *newscript;
	newscript = new UnitScript;
	newscript->Name="default";
	newscript->GetAI = GetAI_default;
	m_scripts[nrscripts++] = newscript;
}
