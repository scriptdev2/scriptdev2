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
SDName: Mob_Blackwing_Lair
SD%Complete: 100
SDComment: All except Death Talon Hatcher (12468) are verified and in EAI.
SDCategory: Blackwing Lair
EndScriptData */

#include "../../creature/simple_ai.h"

CreatureAI* GetAI_mob_blackwing_lair(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID)
    {        
        //Death Talon Hatcher -- Needs to be converted to EAI
    case 12468:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22442;           //Growing Flames
        ai->Spell[0].Cooldown = 4000;
        ai->Spell[0].CooldownRandomAddition = 3000;
        ai->Spell[0].First_Cast = 2000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_mob_blackwing_lair()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_blackwing_lair";
    newscript->GetAI = GetAI_mob_blackwing_lair;
    m_scripts[nrscripts++] = newscript;
}
