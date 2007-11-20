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
SDName: boss_vem
SD%Complete: 100
SDComment: not linked
EndScriptData */


#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

//TODO: Cast SPELL_ENRAGE upon kri/yauj on death

#define SPELL_CHARGE 16636

//Enrage spell is casted upon kri/yauj upon death and upon self if in combat for 20 minutes
#define SPELL_ENRAGE        19953

CreatureAI* GetAI_Vem(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = SPELL_CHARGE;
    ai->Spell[0].Cooldown = 8000;
    ai->Spell[0].First_Cast = 10000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = SPELL_ENRAGE;
    ai->Spell[1].Cooldown = 1200000;
    ai->Spell[1].First_Cast = 1200000;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_Vem()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_vem";
    newscript->GetAI = GetAI_Vem;
    m_scripts[nrscripts++] = newscript;
}
