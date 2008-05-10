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
SDName: mobs_Zulgurub
SD%Complete: 60
SDComment: Zul'Gurub trash mobs. ALl mobs except Voodoo slave converted to EventAI.
SDCategory: Zul'Gurub
EndScriptData */

#include "../../creature/simple_ai.h"

CreatureAI* GetAI_mobs_zulgurub(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID)
    {
        //Voodoo Slave
    case 14883:
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 24669;      //Fire Rain       
        ai->Spell[0].Cooldown = 10000;                 
        ai->Spell[0].First_Cast = 7000;
        ai->Spell[0].CooldownRandomAddition = 15000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

        ai->Spell[1].Enabled = true;               
        ai->Spell[1].Spell_Id = 24670;      //Summon Inferno       
        ai->Spell[1].Cooldown = 60000;                 
        ai->Spell[1].First_Cast = 30000;
        ai->Spell[1].CooldownRandomAddition = 30000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM; 
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_mobs_zulgurub()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mobs_zulgurub";
    newscript->GetAI = GetAI_mobs_zulgurub;
    m_scripts[nrscripts++] = newscript;
}
