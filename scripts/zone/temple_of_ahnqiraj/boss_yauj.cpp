/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: boss_yauj
SD%Complete: 100
SDComment: not linked
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

//TODO: Add brood spawns upon death.

#define SPELL_HEAL      25807
#define SPELL_AOE_FEAR  19408

CreatureAI* GetAI_Yauj(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = SPELL_HEAL;
    ai->Spell[0].Cooldown = 25000;
    ai->Spell[0].First_Cast = 25000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = SPELL_AOE_FEAR;
    ai->Spell[1].Cooldown = 20000;
    ai->Spell[1].First_Cast = 20000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_Yauj()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_yauj";
    newscript->GetAI = GetAI_Yauj;
    m_scripts[nrscripts++] = newscript;
}
