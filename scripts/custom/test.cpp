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

#include "../sc_defines.h"
#include "../creature/simple_ai.h"

#define AGGRO_YELL_1      "Me be killin you mon!"
#define AGGRO_YELL_2      "Some random lines"
#define AGGRO_YELL_3      "Aggro 3"

#define SPECIAL_YELL_1      "Now die!"

CreatureAI* GetAI_test(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    memset(ai->Spell,0,sizeof(ai->Spell));

    ai->AggroYell[0] = AGGRO_YELL_1;
    ai->AggroYell[1] = AGGRO_YELL_2;
    ai->AggroYell[2] = AGGRO_YELL_3;

    //Flay (10% double attack)
    ai->Spell[0].Enabled = true;                //Enabled
    ai->Spell[0].Spell_Id = 32732;              //Flay (10% double attack)
    ai->Spell[0].Cooldown = -1;                 //Infinite duration no need to cast again
    ai->Spell[0].First_Cast = 1000;             //Cast this 1 second after we go into combat
    ai->Spell[0].Cast_Target_Type = CAST_SELF;  //Self

    //19643 //ms
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 19643;
    ai->Spell[1].Cooldown = 7000;
    ai->Spell[1].First_Cast = 7000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //19471 // Charge
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 19471;
    ai->Spell[2].Cooldown = 10000;
    ai->Spell[2].First_Cast = 1000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_RANDOM;

    //41472 //Divine Wrath
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 41472;
    ai->Spell[3].Cooldown = 16000;
    ai->Spell[3].First_Cast = 16000;
    ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_LAST_AGGRO;
    ai->Spell[3].Yell[0] = SPECIAL_YELL_1;
    ai->Spell[3].Yell[1] = SPECIAL_YELL_1;
    ai->Spell[3].Yell[2] = SPECIAL_YELL_1;

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_test()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="test";
    newscript->GetAI = GetAI_test;
    m_scripts[nrscripts++] = newscript;
}
