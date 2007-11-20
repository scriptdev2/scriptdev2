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
SDName: mob_uldaman
SD%Complete: 100
SDComment: Uldaman trash mobs
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

CreatureAI* GetAI_mob_uldaman(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        // ancient_stone_keeper
    case 7206: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 10093;   // Harsh Winds           
        ai->Spell[0].Cooldown = 11000;                 
        ai->Spell[0].First_Cast = 2000;            
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 6524; // Ground Tremor
        ai->Spell[1].Cooldown = 17000;
        ai->Spell[1].First_Cast = 12000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;
        break;
        //galgann_firehammer
    case 7291: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 10447;   // Flame Shock          
        ai->Spell[0].Cooldown = 30000;            
        ai->Spell[0].First_Cast = 6000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 11306;    // Fire Nova
        ai->Spell[1].Cooldown = 30000;
        ai->Spell[1].First_Cast = 11000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 18543; // Flame Lash
        ai->Spell[2].Cooldown = 30000;
        ai->Spell[2].First_Cast = 16000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 9482;    // Amplify Flames
        ai->Spell[3].Cooldown = 30000;
        ai->Spell[3].First_Cast = 1000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //revelosh
    case 6910: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 10392;    // Lightning Bolt        
        ai->Spell[0].Cooldown = 25000;           
        ai->Spell[0].First_Cast = 2000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 930;    // Chain Lightning
        ai->Spell[1].Cooldown = 31000;
        ai->Spell[1].First_Cast = 8000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //grimlock
    case 4854: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 10392;    // Lightning Bolt        
        ai->Spell[0].Cooldown = 25000;           
        ai->Spell[0].First_Cast = 2000;    
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 930;    // Chain Lightning
        ai->Spell[1].Cooldown = 31000;
        ai->Spell[1].First_Cast = 8000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //cleft_scopid
    case 7078: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 25645;     // Poison  
        ai->Spell[0].Cooldown = 15000;              
        ai->Spell[0].First_Cast = 2000;      
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
        //earthen_rocksmaher
    case 7011: 
        ai->Spell[0].Enabled = true;              
        ai->Spell[0].Spell_Id = 13446;   // Strike      
        ai->Spell[0].Cooldown = 17000;            
        ai->Spell[0].First_Cast = 2000;          
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //earthen_sculptor
    case 7012: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 2602;    // Fire Shield IV       
        ai->Spell[0].Cooldown = -1;          
        ai->Spell[0].First_Cast = 100;        
        ai->Spell[0].Cast_Target_Type = CAST_SELF;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 9574;    // Flame Buffet
        ai->Spell[1].Cooldown = 17000;
        ai->Spell[1].First_Cast = 8000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //shadowforge_geologist
    case 7030: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 8814;      // Flame Spike     
        ai->Spell[0].Cooldown = 22000;          
        ai->Spell[0].First_Cast = 2000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 18958;        // Flame Lash
        ai->Spell[1].Cooldown = 17000;
        ai->Spell[1].First_Cast = 8000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //shadowforge_relic_hunter
    case 4847: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 34941;         // Shadow Word: Pain  
        ai->Spell[0].Cooldown = 25000;          
        ai->Spell[0].First_Cast = 8000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
        //stone_steward
    case 4860: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 6524;      // Ground Tremor     
        ai->Spell[0].Cooldown = 19000;          
        ai->Spell[0].First_Cast = 9000;        
        ai->Spell[0].Cast_Target_Type = CAST_SELF;  
        break;
        //shrike_bat
    case 4861: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 8281;          // Sonic Burst 
        ai->Spell[0].Cooldown = 17000;          
        ai->Spell[0].First_Cast = 5000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
        //stonevault_geomancer
    case 4853: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 21549;    // Fireball       
        ai->Spell[0].Cooldown = 17000;          
        ai->Spell[0].First_Cast = 6000;        
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 9574;        // Flame Buffet
        ai->Spell[1].Cooldown = 35000;
        ai->Spell[1].First_Cast = 2000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //stonevault_oracle
    case 4852: 
        ai->Spell[0].Enabled = true;           
        ai->Spell[0].Spell_Id = 945;      // Lightning Shield     
        ai->Spell[0].Cooldown = 45000;          
        ai->Spell[0].First_Cast = 100;        
        ai->Spell[0].Cast_Target_Type = CAST_SELF;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 10392;        // Lightning Bolt
        ai->Spell[1].Cooldown = 19000;
        ai->Spell[1].First_Cast = 3000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
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
