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
SDName: Boss_Deadmines
SD%Complete: 50
SDComment: Deadmines bosses -- all except Miner Johnson in EAI
SDCategory: Deadmines
EndScriptData */

#include "../../creature/simple_ai.h"

CreatureAI* GetAI_boss_deadmines(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        // Miner Johnson -- Needs to be converted to EAI
    case 3586: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 12097;        // Peirce Armor
        ai->Spell[0].Cooldown = 20000;                 
        ai->Spell[0].First_Cast = 20000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_boss_deadmines()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_deadmines";
    newscript->GetAI = GetAI_boss_deadmines;
    m_scripts[nrscripts++] = newscript;
}
