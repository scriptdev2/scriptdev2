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
SDName: mob_the_eye
SD%Complete: 100
SDComment: Trash mobs for TK: The Eye
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "sc_creature.h"
#include "../../../creature/simple_ai.h"

CreatureAI* GetAI_mob_the_eye(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        //Apprentice Star Scryer
    case 20043: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 40424;      //Arcane Volley       
        ai->Spell[0].Cooldown = 250000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 27082;        //Arcane Explosion
        ai->Spell[1].Cooldown = 19000;
        ai->Spell[1].First_Cast = 17000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;
        break;
        //Star Scryer
    case 20034: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 37124;      //Starfall     
        ai->Spell[0].Cooldown = 350000;                 
        ai->Spell[0].First_Cast = 25000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
        //Astromancer
    case 20033: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 30482;        // Molten Armor
        ai->Spell[0].Cooldown = -1;                 
        ai->Spell[0].First_Cast = 100;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 29922;        // Fireball Volley
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 19000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 33933;        // Blast Wave
        ai->Spell[2].Cooldown = 22000;
        ai->Spell[2].First_Cast = 15000;
        ai->Spell[2].Cast_Target_Type = CAST_SELF;
        break;
        //Astromancer Lord
    case 20046: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 35265;        // Fire Shield
        ai->Spell[0].Cooldown = -1;                 
        ai->Spell[0].First_Cast = 100;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 29922;        // Fireball Volley
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 19000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 33933;        // Blast Wave
        ai->Spell[2].Cooldown = 22000;
        ai->Spell[2].First_Cast = 15000;
        ai->Spell[2].Cast_Target_Type = CAST_SELF;

        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 33043;        // Dragon's Breath
        ai->Spell[3].Cooldown = 29000;
        ai->Spell[3].First_Cast = 22000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Bloodwarder Vindicator
    case 20032: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 10308;        // Hammer of Justice
        ai->Spell[0].Cooldown = 35000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Bloodwarder Legionnaire
    case 20031: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 36981;        // Whirlwind
        ai->Spell[0].Cooldown = 27000;                 
        ai->Spell[0].First_Cast = 10000;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 26350;        // Cleave
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 20000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Bloodwarder Marshal
    case 20035: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 36981;        // Whirlwind
        ai->Spell[0].Cooldown = 29000;                 
        ai->Spell[0].First_Cast = 10000;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 26007;        // Uppercut
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 20000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Phoenix-Hawk
    case 20039: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 25380;        // Mana Burn Rank 7
        ai->Spell[0].Cooldown = 19000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 38110;        // Wing Buffet
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 20000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Crystalcore Sentinel
    case 20041: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 40340;        // Trample
        ai->Spell[0].Cooldown = 25000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 37106;        // Charged Arcane Explosion
        ai->Spell[1].Cooldown = 45000;
        ai->Spell[1].First_Cast = 25000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Crystalcore Mechanic
    case 20052: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 37123;        // Saw Blade
        ai->Spell[0].Cooldown = 35000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_mob_the_eye()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_the_eye";
    newscript->GetAI = GetAI_mob_the_eye;
    m_scripts[nrscripts++] = newscript;
}
