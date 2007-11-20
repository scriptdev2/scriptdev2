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

/* ScriptData
SDName: boss_kri
SD%Complete: 100
SDComment: Loot and creatures not linked
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

#define SPELL_CLEAVE        16044
#define SPELL_TOXIC_VOLLEY  25812

//Passive spell cast by the poison cloud 
//(note that this is definitly the wrong ID but th only spell that matches the damage)
#define SPELL_POISON_CLOUD  38718

CreatureAI* GetAI_Kri(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = SPELL_CLEAVE;
    ai->Spell[0].Cooldown = 6000;
    ai->Spell[0].First_Cast = 6000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = SPELL_TOXIC_VOLLEY;
    ai->Spell[1].Cooldown = 12000;
    ai->Spell[1].First_Cast = 12000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //This spell is wrong but oh well, best we can do for now
    ai->Death_Spell = 38718;
    ai->Death_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_Kri()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kri";
    newscript->GetAI = GetAI_Kri;
    m_scripts[nrscripts++] = newscript;
}
