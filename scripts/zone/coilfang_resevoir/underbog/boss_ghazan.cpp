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

#include "../../../sc_defines.h"
#include "../../../creature/simple_ai.h"

/*
--== Ghaz'an ==--
*Acid Spit - 34290; timer: 8sec 
*Enrage - 20% hp; 40683
*Tail Sweep - 34267; timer: ~10sec
*Acid Breath - 34268; timer: 5sec
*/

CreatureAI* GetAI_boss_ghazan(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    // should be immune to all CC, as any other instance boss
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

    // Acid Spit - 34290; timer: 8sec 
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 34290;
    ai->Spell[0].Cooldown = 8000;
    ai->Spell[0].First_Cast = 8000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    // Acid Breath - 34268; timer: 5sec
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 34268;
    ai->Spell[1].Cooldown = 5000;
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    // Tail Sweep - 34267
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 34267;
    ai->Spell[2].Cooldown = 10000;
    ai->Spell[2].First_Cast = 10000;
    ai->Spell[2].Cast_Target_Type = CAST_SELF;

    // Enrage - 20% hp; 40683
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 40683;
    ai->Spell[3].Cooldown = -1;
    ai->Spell[3].First_Cast = -80;
    ai->Spell[3].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_boss_ghazan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ghazan";
    newscript->GetAI = GetAI_boss_ghazan;
    m_scripts[nrscripts++] = newscript;
}