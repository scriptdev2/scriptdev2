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
SDName: Boss_Ambassador_Hellmaw
SD%Complete: 100
SDComment: 
SDCategory: Auchindoun, Shadow Labyrinth
EndScriptData */

#include "sc_creature.h"
#include "../../../creature/simple_ai.h"

#define CORROSIVE_ACID  23313
#define TERRIFYING_HOWL 30752

CreatureAI* GetAI_boss_ambassador_hellmaw(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = CORROSIVE_ACID;
    ai->Spell[0].Cooldown = 25000;
    ai->Spell[0].First_Cast = 25000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = TERRIFYING_HOWL;
    ai->Spell[1].Cooldown = 30000;
    ai->Spell[1].First_Cast = 40000;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    ai->Aggro_Text[0] = "Pathetic mortals! You will pay dearly!";
    ai->Aggro_Sound[0] = 10475;
    ai->Aggro_Text[1] = "I will break you!";
    ai->Aggro_Sound[1] = 10476;
    ai->Aggro_Text[2] = "Finally! Something to relieve the tedium!";
    ai->Aggro_Sound[2] = 10477;

    ai->Death_Text[0] = "Do not grow overconfident...mortal.";
    ai->Death_Sound[0] = 10480;
    ai->Death_Text[1] = "Do not grow overconfident...mortal.";
    ai->Death_Sound[1] = 10480;
    ai->Death_Text[2] = "Do not grow overconfident...mortal.";
    ai->Death_Sound[2] = 10480;

    ai->Kill_Text[0] = "This is the part I enjoy most.";
    ai->Kill_Sound[0] = 10479;
    ai->Kill_Text[1] = "Do you fear death?";
    ai->Kill_Sound[1] = 10478;

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_boss_ambassador_hellmaw()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ambassador_hellmaw";
    newscript->GetAI = GetAI_boss_ambassador_hellmaw;
    m_scripts[nrscripts++] = newscript;
}
