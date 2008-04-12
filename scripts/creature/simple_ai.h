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

#include "sc_creature.h"

enum CastTarget
{
    CAST_SELF = 0,                  //Self cast
    CAST_HOSTILE_TARGET,            //Our current target (ie: highest aggro)
    CAST_HOSTILE_SECOND_AGGRO,      //Second highest aggro (generaly used for cleaves and some special attacks)
    CAST_HOSTILE_LAST_AGGRO,        //Dead last on aggro (no idea what this could be used for)
    CAST_HOSTILE_RANDOM,            //Just any random target on our threat list
    CAST_FRIENDLY_RANDOM,           //NOT YET IMPLEMENTED

    //Special cases
    CAST_KILLEDUNIT_VICTIM,         //Only works within KilledUnit function
    CAST_JUSTDIED_KILLER,           //Only works within JustDied function
};

struct MANGOS_DLL_DECL SimpleAI : public ScriptedAI
{
    SimpleAI(Creature *c);// : ScriptedAI(c);

    void Reset();

    void Aggro(Unit *who);

    void KilledUnit(Unit *victim);

    void DamageTaken(Unit *killer, uint32 &damage);

    void UpdateAI(const uint32 diff);

public:

    char* Aggro_Text[3];
    bool Aggro_Say[3];
    uint32 Aggro_Sound[3];

    char* Death_Text[3];
    bool Death_Say[3];
    uint32 Death_Sound[3];
    uint32 Death_Spell;
    uint32 Death_Target_Type;

    char* Kill_Text[3];
    bool Kill_Say[3];
    uint32 Kill_Sound[3];
    uint32 Kill_Spell;
    uint32 Kill_Target_Type;

    struct SimpleAI_Spell
    {
        uint32 Spell_Id;                //Spell ID to cast
        int32 First_Cast;               //Delay for first cast
        uint32 Cooldown;                //Cooldown between casts
        uint32 CooldownRandomAddition;  //Random addition to cooldown (in range from 0 - CooldownRandomAddition)
        uint32 Cast_Target_Type;        //Target type (note that certain spells may ignore this)
        bool InterruptPreviousCast;     //Interrupt a previous cast if this spell needs to be cast
        bool Enabled;                   //Spell enabled or disabled (default: false)

        //3 texts to many?
        char* Text[3];
        bool Say[3];
        uint32 Text_Sound[3];
    }Spell[10];

protected:
    uint32 Spell_Timer[10];
};
