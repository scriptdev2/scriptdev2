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
SDName: Boss_Deadmines
SD%Complete: 50
SDComment: Deadmines bosses
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

CreatureAI* GetAI_boss_deadmines(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

    switch (CreatureID) 
    {
        //Cookie
    case 645: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 6306;      //Acid Splash     
        ai->Spell[0].Cooldown = 30000;                 
        ai->Spell[0].First_Cast = 30000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;

        //Gilnid
    case 1763: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 5213;      //MOLTEN_METAL    
        ai->Spell[0].Cooldown = 15000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;               
        ai->Spell[1].Spell_Id = 5159;      //MELT_ORE   
        ai->Spell[1].Cooldown = 20000;                 
        ai->Spell[1].First_Cast = 20000;             
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;

        //Marisa du paige
    case 599: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 3140;        // Fireball
        ai->Spell[0].Cooldown = 8000;                 
        ai->Spell[0].First_Cast = 8000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;

        // Miner Johnson
    case 3586: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 12097;        // Peirce Armor
        ai->Spell[0].Cooldown = 20000;                 
        ai->Spell[0].First_Cast = 20000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_boss_deadmines()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_deadmines";
    newscript->GetAI = GetAI_boss_deadmines;
    m_scripts[nrscripts++] = newscript;
}
