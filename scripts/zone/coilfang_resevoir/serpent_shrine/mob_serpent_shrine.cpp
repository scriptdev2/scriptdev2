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
SDName: Mob_Serpent_Shrine
SD%Complete: 100
SDComment: All non-boss mobs in SSC
EndScriptData */

#include "sc_creature.h"
#include "../../../creature/simple_ai.h"

CreatureAI* GetAI_mob_serpentshrine_cavern(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID)
    {
        //Serpentshrine Sporebat
    case 21246:
       
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 22120;           //Charge
        ai->Spell[0].Cooldown = 20000 ;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].First_Cast = 10000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 38924;           //Spore Burst
        ai->Spell[1].Cooldown = 20000;
        ai->Spell[1].CooldownRandomAddition = 5000;
        ai->Spell[1].First_Cast = 15000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;
        break;
        //Coilfang Hate-Screamer
    case 21339:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 38913;         //Silence
        ai->Spell[0].Cooldown = 20000 ;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].First_Cast = 10000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 38538;         //Arcane Blast
        ai->Spell[1].Cooldown = 20000;
        ai->Spell[1].CooldownRandomAddition = 5000 ;
        ai->Spell[1].First_Cast = 15000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
        break;
        //Coilfang Beast-Tamer - TODO: Bestial Wrath
    case 21221:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 30014;         //Cleave
        ai->Spell[0].Cooldown = 10000;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].First_Cast = 10000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Tidewalker Depth-Seer
    case 21224:
        break;
        //Tidewalker Harpooner
    case 21227:
        break;
        //Tidewalker Hydromancer
    case 21228:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 29926;         //Frostbolt
        ai->Spell[0].Cooldown = 30000;
        ai->Spell[0].First_Cast = 10000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 32365;         //Frost Nova
        ai->Spell[1].Cooldown = 30000;
        ai->Spell[1].First_Cast = 20000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 34353;         //Frost Shock
        ai->Spell[2].Cooldown = 30000;
        ai->Spell[2].First_Cast = 30000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Tidewalker Shaman
    case 21226:
        break;
        //Tidewalker Warrior
    case 21225:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 43273;         //Cleave
        ai->Spell[0].Cooldown = 10000;
        ai->Spell[0].First_Cast = 5000;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 35949;         //Bloodthirst
        ai->Spell[1].Cooldown = 20000;
        ai->Spell[1].First_Cast = 15000;
        ai->Spell[1].CooldownRandomAddition = 5000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Coilfang Serpentguard
    case 21298:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 43273;         //Cleave
        ai->Spell[0].Cooldown = 10000;
        ai->Spell[0].First_Cast = 10000;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 38603;         //Corrupt Devotion Aura
        ai->Spell[1].Cooldown = -1;
        ai->Spell[1].First_Cast = 5000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Coilfang Fathom-Witch - TODO: Domination (mind control)
    case 21299:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 36127;         //Shadow Nova
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 5000;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_mob_serpentshrine_cavern()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_serpentshrine_cavern";
    newscript->GetAI = GetAI_mob_serpentshrine_cavern;
    m_scripts[nrscripts++] = newscript;
}
