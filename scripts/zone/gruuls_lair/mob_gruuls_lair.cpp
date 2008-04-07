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
SDName: Mob_Gruuls_Lair
SD%Complete: 100
SDComment: Trash mobs
EndScriptData */

#include "sc_creature.h"
#include "../../creature/simple_ai.h"

CreatureAI* GetAI_mob_gruuls_lair(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID)
    {
        //Lair Brute
    case 19389:
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 38461;      //Charge       
        ai->Spell[0].Cooldown = 7000;                 
        ai->Spell[0].First_Cast = 7000;
        ai->Spell[0].CooldownRandomAddition = 3000; 
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 31911;      //Mortal Strike
        ai->Spell[1].Cooldown = 15000;
        ai->Spell[1].First_Cast = 8000;
        ai->Spell[1].CooldownRandomAddition = 5000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 31345;      //Cleave
        ai->Spell[2].Cooldown = 7000;
        ai->Spell[2].First_Cast = 5500;
        ai->Spell[2].CooldownRandomAddition = 5000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Gronn Priest
    case 21350:
        ai->Spell[0].Enabled = true;     
        ai->Spell[0].Spell_Id = 36678;      //Heal
        ai->Spell[0].Cooldown = 20000;
        ai->Spell[0].First_Cast = 7500;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 36679;      //Renew
        ai->Spell[1].Cooldown = 20000;   
        ai->Spell[1].First_Cast = 2000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;

        /*ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 13704;      //Psichic Scream - disabled: can cause many problems without vmaps
        ai->Spell[2].Cooldown = 15000;
        ai->Spell[2].First_Cast = 6000;
        ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;*/
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_mob_gruuls_lair()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_gruuls_lair";
    newscript->GetAI = GetAI_mob_gruuls_lair;
    m_scripts[nrscripts++] = newscript;
}
