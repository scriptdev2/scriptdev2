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
SDComment: more Trashes + Timers
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

CreatureAI* GetAI_mobs_zulgurub(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID)
    {
        //Gurubashi Axe Thrower
    case 11350:
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 24018;      //24020,24071; 24018 should be a mixture out of the others       
        ai->Spell[0].Cooldown = 15000;                 
        ai->Spell[0].First_Cast = 5000;
        ai->Spell[0].CooldownRandomAddition = 3000; 
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 
        break;
        //Hakkari Priest
    case 11830:
        ai->Spell[0].Enabled = true;     
        ai->Spell[0].Spell_Id = 25820;      //AOE Fear, spell should be right one
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 8000;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 24021;      //Magic immunity
        ai->Spell[1].Cooldown = 15000;   
        ai->Spell[1].First_Cast = 2000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;

        /*ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = ;      //Couldnt find Spell (Summon Frogs)
        ai->Spell[2].Cooldown = ;
        ai->Spell[2].First_Cast =;
        ai->Spell[2].Cast_Target_Type = ;*/
        break;
        //Razzashi Adder
    case 11372:

        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 24840;      //AOE Poison       
        ai->Spell[0].Cooldown = 15000;                 
        ai->Spell[0].First_Cast = 5000;
        ai->Spell[0].CooldownRandomAddition = 3000; 
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 
        break;
        //Gurubashi Headhunter
    case 11351:
        ai->Spell[0].Enabled = true;     
        ai->Spell[0].Spell_Id = 24053;      //Sheep, hope right
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 5000;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 29896;      //Wrong!, but root player on ground
        ai->Spell[1].Cooldown = 15000;   
        ai->Spell[1].First_Cast = 2000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        //Gurubashi Berserker
    case 11352:
        ai->Spell[0].Enabled = true;     
        ai->Spell[0].Spell_Id = 23931;      // Thunderclap
        ai->Spell[0].Cooldown = 13000;
        ai->Spell[0].First_Cast = 5000;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 25820;      //AOE Fear, only one i can think of
        ai->Spell[1].Cooldown = 15000;   
        ai->Spell[1].First_Cast = 2000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;
        break;
        //Hakkari Blood Priest
    case 11340:
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 24617;      //Blood Funnel, not working :/       
        ai->Spell[0].Cooldown = 15000;                 
        ai->Spell[0].First_Cast = 5000;
        ai->Spell[0].CooldownRandomAddition = 3000; 
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Gurubashi Bat Rider
    case 14750:
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 25699;      //Explosion       
        ai->Spell[0].Cooldown = 15000;                 
        ai->Spell[0].First_Cast = -50;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
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
