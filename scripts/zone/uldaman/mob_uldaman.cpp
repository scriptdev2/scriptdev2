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
SDName: mob_uldaman
SD%Complete: 100
SDComment: Uldaman trash mobs. Cleft Scorpid needs to be converted to EventAI
SDCategory: Uldaman
EndScriptData */

#include "../../creature/simple_ai.h"

CreatureAI* GetAI_mob_uldaman(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        //cleft_scopid - needs conversion
    case 7078: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 25645;     // Poison  
        ai->Spell[0].Cooldown = 15000;              
        ai->Spell[0].First_Cast = 2000;      
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_mob_uldaman()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_uldaman";
    newscript->GetAI = GetAI_mob_uldaman;
    m_scripts[nrscripts++] = newscript;
}
