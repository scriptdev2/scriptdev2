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
SDName: Mob_Blacktemple
SD%Complete: 100
SDComment: All trash mobs for BT
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

CreatureAI* GetAI_mob_blacktemple(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        //Aqueous Lord
    case 22878: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 40100;        // CRASHINGWAVE      
        ai->Spell[0].Cooldown = 35000;                 
        ai->Spell[0].First_Cast = 30000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  


        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 40099;        // VILESLIME
        ai->Spell[1].Cooldown = 45000;
        ai->Spell[1].First_Cast = 55000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;


        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 40099;        // VILESLIME
        ai->Spell[2].Cooldown = 10000;
        ai->Spell[2].First_Cast = 55000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_RANDOM;


        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 41472;
        ai->Spell[3].Cooldown = 16000;
        ai->Spell[3].First_Cast = 16000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_LAST_AGGRO;
        break;
        //Aqueous Surger
    case 22881: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 24099;        // POISONBOLTVOLLEY      
        ai->Spell[0].Cooldown = 35000;                 
        ai->Spell[0].First_Cast = 30000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Coilskar General
    case 22873: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 24099;        // POISONBOLTVOLLEY      
        ai->Spell[0].Cooldown = 45000;                 
        ai->Spell[0].First_Cast = 30000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Coilskar Sea-Caller
    case 22875: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 27012;        // HURRICANE     
        ai->Spell[0].Cooldown = 45000;                 
        ai->Spell[0].First_Cast = 30000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  


        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 22945;        // FORKEDLIGHTNING
        ai->Spell[1].Cooldown = 30000;
        ai->Spell[1].First_Cast = 45000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Coilskar Soothsayer
    case 22876: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 25331;        // HOLYNOVA     
        ai->Spell[0].Cooldown = 45000;                 
        ai->Spell[0].First_Cast = 25000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Coilskar Wrangler
    case 22877: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 40066;        // LIGHTNINGPROD      
        ai->Spell[0].Cooldown = 35000;                 
        ai->Spell[0].First_Cast = 30000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  


        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 38260;        // CLEAVE
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 35000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;


        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 35392;        // POISONSPIT
        ai->Spell[2].Cooldown = 39000;
        ai->Spell[2].First_Cast = 15000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_RANDOM;


        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 40079;        // DEBILITATINGSPRAY
        ai->Spell[3].Cooldown = 46000;
        ai->Spell[3].First_Cast = 66000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Leviathan
    case 22884: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 35392;        // POISONSPIT     
        ai->Spell[0].Cooldown = 35000;                 
        ai->Spell[0].First_Cast = 28000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;

        //Illidari Defiler
    case 22853: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 40946;        // Rain of Chaos   
        ai->Spell[0].Cooldown = 45000;                 
        ai->Spell[0].First_Cast = 35000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 11713;        // Curse of Agony Rank 6
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 25000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        //Illidari Fearbringer
    case 22954: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 40946;        // Rain of Chaos   
        ai->Spell[0].Cooldown = 55000;                 
        ai->Spell[0].First_Cast = 10000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 41534;        // War Stomp
        ai->Spell[1].Cooldown = 45000;
        ai->Spell[1].First_Cast = 17000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 40938;        // Illidari Flames
        ai->Spell[2].Cooldown = 49000;
        ai->Spell[2].First_Cast = 25000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Illidari Nightlord
    case 22855: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 6215;        // Fear
        ai->Spell[0].Cooldown = 35000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 39645;        // Shadow Inferno
        ai->Spell[1].Cooldown = 10000;
        ai->Spell[1].First_Cast = 27000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;
        break;
        //Illidari Centurion
    case 23337: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 41168;        // Sonic Strike
        ai->Spell[0].Cooldown = 45000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 40946;        // Rain of Chaos   
        ai->Spell[1].Cooldown = 55000;
        ai->Spell[1].First_Cast = 37000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Illidari Boneslicer
    case 22869: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 38764;        // Gouge
        ai->Spell[0].Cooldown = 45000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Illidari Heartseeker
    case 23339: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 41172;        // Rapid Shot   
        ai->Spell[0].Cooldown = 25000;                 
        ai->Spell[0].First_Cast = 25000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 41171;        // Skeleton Shot
        ai->Spell[1].Cooldown = 65000;
        ai->Spell[1].First_Cast = 45000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 41093;        // Shoot
        ai->Spell[2].Cooldown = 35000;
        ai->Spell[2].First_Cast = 25000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        //Ashtongue Stormcaller
    case 22846: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 25472;        // Lightning Shield Rank 9  
        ai->Spell[0].Cooldown = -1;                 
        ai->Spell[0].First_Cast = 100;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 25442;        // Chain Lightning Rank 6
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 25000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 25449;        // Lightning Bolt Rank 12
        ai->Spell[2].Cooldown = 22000;
        ai->Spell[2].First_Cast = 15000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Ashtongue Feral Spirit
    case 22849: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 36058;        // Charge 
        ai->Spell[0].Cooldown = 33000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Ashtongue Mystic
    case 22845: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 25457;        // Flame Shock Rank 7
        ai->Spell[0].Cooldown = 25000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 25464;        // Frost Shock Rank 5
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 25000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        //Ashtongue Battlelord
    case 22844: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 26350;        // Cleave
        ai->Spell[0].Cooldown = 18000;                 
        ai->Spell[0].First_Cast = 19000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Aqueous Spawn
    case 22883: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 40103;        // Sludge Nova
        ai->Spell[0].Cooldown = 18000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Wrathbone Flayer
    case 22953: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 26350;        // Cleave
        ai->Spell[0].Cooldown = 10000;                 
        ai->Spell[0].First_Cast = 5000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Shadowmoon Champion
    case 22880: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 41063;        // Chaotic Light
        ai->Spell[0].Cooldown = 18000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Shadowmoon Blood Mage
    case 22945: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 41068;        // Blood Siphon
        ai->Spell[0].Cooldown = 24000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

        ai->Spell[1].Enabled = true;                
        ai->Spell[1].Spell_Id = 41229;        // Bloodbolt
        ai->Spell[1].Cooldown = 25000;                 
        ai->Spell[1].First_Cast = 20000;             
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Shadowmoon Deathshaper
    case 22882: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 41068;        // Death Coil
        ai->Spell[0].Cooldown = 24000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

        ai->Spell[1].Enabled = true;                
        ai->Spell[1].Spell_Id = 41229;        // Shadow Bolt Rank 11
        ai->Spell[1].Cooldown = 10000;                 
        ai->Spell[1].First_Cast = 20000;             
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
    case 23216:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 41178;         // Debilitating Strike
        ai->Spell[0].Cooldown = 18000;
        ai->Spell[0].First_Cast = 10000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
    case 23523:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 42023;         // Rain of Fire
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 18000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
    case 23524:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 42027;         // Chain Heal
        ai->Spell[0].Cooldown = 18000;
        ai->Spell[0].First_Cast = 8000;
        ai->Spell[0].Cast_Target_Type = CAST_SELF;
        break;
    case 23318:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 41978;         // Debilitating Poison
        ai->Spell[0].Cooldown = 22000;
        ai->Spell[0].First_Cast = 15000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
    }
    ai->EnterEvadeMode();

    return ai;
}

void AddSC_mob_blacktemple()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_blacktemple";
    newscript->GetAI = GetAI_mob_blacktemple;
    m_scripts[nrscripts++] = newscript;
}
