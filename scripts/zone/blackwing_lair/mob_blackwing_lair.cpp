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
SDComment: 
SDCategory: Blackwing Lair
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

CreatureAI* GetAI_mob_blackwing_lair(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID)
    {
        //Black Drakonid
    case 14265:

        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22560;           //Brood Power: Black
        ai->Spell[0].Cooldown = 4000;
        ai->Spell[0].CooldownRandomAddition = 2000;
        ai->Spell[0].First_Cast = 10000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Blackwing Spellbinder
    case 12457:

        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 23603;           //Polymorph
        ai->Spell[0].Cooldown = 5000;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].First_Cast = 10000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 28478;           //Frostbolt
        ai->Spell[1].Cooldown = 2000;
        ai->Spell[1].First_Cast = (rand()%60)*1000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Blackwing Technician
    case 13996:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 40062;           //Bomb
        ai->Spell[0].Cooldown = 2000;
        ai->Spell[0].CooldownRandomAddition = 4000;
        ai->Spell[0].First_Cast = 4000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22335;           //Poison
        ai->Spell[0].Cooldown = 8000;
        ai->Spell[0].First_Cast = 2000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Blackwing Warlock
    case 12459:

        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 24669;           //Rain of fire
        ai->Spell[0].Cooldown = 5000;
        ai->Spell[0].CooldownRandomAddition = 2000;
        ai->Spell[0].First_Cast = 5000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 22677;           //Shadow Bolt
        ai->Spell[1].Cooldown = 3000;
        ai->Spell[1].CooldownRandomAddition = 4000;
        ai->Spell[1].First_Cast = 6000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 22392;           //Summon Enraged Felguard
        ai->Spell[2].Cooldown = 10000;
        ai->Spell[2].First_Cast = 10000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Blue Drakonid
    case 14261:

        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22559;           //Brood Power: Blue
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 1000+rand()%14000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Bronze Drakonid
    case 14263:

        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22642;           //Brood Power: Bronze
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 1000+rand()%14000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Death Talon Captain
    case 12467:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22423;           //Flame Shock
        ai->Spell[0].Cooldown = 6000;
        ai->Spell[0].CooldownRandomAddition = 4000;
        ai->Spell[0].First_Cast = 3000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 22540;           //Cleave
        ai->Spell[1].Cooldown = 5000;
        ai->Spell[1].First_Cast = 4000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 22436;           //Aura of Flames
        ai->Spell[2].Cooldown = 7500;
        ai->Spell[2].First_Cast = 7500;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 22439;           //Mark of Detonation
        ai->Spell[3].Cooldown = 20000;
        ai->Spell[3].First_Cast = 20000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Death Talon Flamescale
    case 12463:

        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22423;           //Flame Shock
        ai->Spell[0].Cooldown = 1000;
        ai->Spell[0].CooldownRandomAddition = 4000;
        ai->Spell[0].First_Cast = 3000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Death Talon Overseer
    case 12461:
        //random spell immunities

        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22540;           //Cleave
        ai->Spell[0].Cooldown = 2000;
        ai->Spell[0].CooldownRandomAddition = 4000;
        ai->Spell[0].First_Cast = 2000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 20623;           //Fire Blast
        ai->Spell[1].Cooldown = 3000;
        ai->Spell[1].CooldownRandomAddition = 3000;
        ai->Spell[1].First_Cast = 3000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        //Death Talon Seether
    case 12464:

        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22433;           //Flame Buffet
        ai->Spell[0].Cooldown = 7000;
        ai->Spell[0].CooldownRandomAddition = 4000;
        ai->Spell[0].First_Cast = 10000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 19812;           //Frenzy
        ai->Spell[1].Cooldown = 10000;
        ai->Spell[1].First_Cast = 10000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;
        break;
        //Death Talon Wyrmguard
    case 12460:
        //random spell immunities
        
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22540;           //Cleave
        ai->Spell[0].Cooldown = 8000;
        ai->Spell[0].CooldownRandomAddition = 2000;
        ai->Spell[0].First_Cast = 3000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 24375;           //War Stomp
        ai->Spell[1].Cooldown = 8000;
        ai->Spell[1].CooldownRandomAddition = 6000;
        ai->Spell[1].First_Cast = 8000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Death Talon Wyrmkin
    case 12465:
        
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 23512;           //Fireball Volley
        ai->Spell[0].Cooldown = 2000;
        ai->Spell[0].CooldownRandomAddition = 4000;
        ai->Spell[0].First_Cast = 2000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 22424;           //Blast Wave
        ai->Spell[1].Cooldown = 7000;
        ai->Spell[1].CooldownRandomAddition = 2000;
        ai->Spell[1].First_Cast = 7000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;
        break;
        //Green Drakonid
    case 14262:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22561;           //Brood Power: Green
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 1000+rand()%14000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Red Drakonid
    case 14264:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22558;           //Brood Power: Red
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 1000+rand()%14000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Grethlok the Controller
    case 12557:
        
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22356;           //Slow
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 5000+rand()%5000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 11446;           //Mind Control
        ai->Spell[1].Cooldown = 20000;
        ai->Spell[1].First_Cast = 1000+rand()%14000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

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
