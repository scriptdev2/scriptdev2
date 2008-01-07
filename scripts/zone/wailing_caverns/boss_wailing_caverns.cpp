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
SDName: boss_wailing_caverns
SD%Complete: 70
SDComment: Wailing Caverns bosses
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

CreatureAI* GetAI_wailing_caverns(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        //Mutanus The Devourer
    case 3654: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 7803;      //Thunder Shock   
        ai->Spell[0].Cooldown = 10000;                 
        ai->Spell[0].First_Cast = 10000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;               
        ai->Spell[1].Spell_Id = 7399;      //Terrify
        ai->Spell[1].Cooldown = 50000;                 
        ai->Spell[1].First_Cast = 50000;             
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_boss_wailing_caverns()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_wailing_caverns";
    newscript->GetAI = GetAI_wailing_caverns;
    m_scripts[nrscripts++] = newscript;
}
