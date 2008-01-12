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

#include "../sc_defines.h"

#define MAX_EVENTS      5000        //60 bytes each (approx 292kb)
#define MAX_ACTIONS     3

enum Target
{
    TARGET_T_SELF = 0,                  //Self cast
    TARGET_T_HOSTILE,                   //Our current target (ie: highest aggro)
    TARGET_T_HOSTILE_SECOND_AGGRO,      //Second highest aggro (generaly used for cleaves and some special attacks)
    TARGET_T_HOSTILE_LAST_AGGRO,        //Dead last on aggro (no idea what this could be used for)
    TARGET_T_HOSTILE_RANDOM,            //Just any random target on our threat list
    TARGET_T_HOSTILE_RANDOM_NOT_TOP,    //Any random target except top threat

    TARGET_T_END
    //CAST_FRIENDLY_RANDOM,             //NOT YET IMPLEMENTED
};

enum Event_Types
{
    EVENT_T_NONE = 0,               //No params
    EVENT_T_TIMER_REPEAT,           //Param1 = Time
    EVENT_T_TIMER_SINGLE,           //Param1 = Time
    EVENT_T_HP_SINGLE,              //Param1 = HP%
    EVENT_T_MANA_SINGLE,            //Param1 = Mana%
    EVENT_T_AGGRO,                  //No Params
    EVENT_T_KILL,                   //No Params
    EVENT_T_DEATH,                  //No Params

    EVENT_T_END,
};

enum Action_Types
{
    ACTION_T_NONE = 0,              //No Params
    ACTION_T_SAY,                   //Param1 = TextId
    ACTION_T_YELL,                  //Param1 = TextId
    ACTION_T_EMOTE,                 //Param1 = TextId
    ACTION_T_SOUND,                 //Param1 = SoundID
    ACTION_T_CAST,                  //Param1 = SpellId, Param2 = Target, Param3 = (false = don't inturrupt prev cast, true = intrurrupt)
    ACTION_T_SUMMON,                //Param1 = CreatureID, Param2 = Target, Param3 = Duration in ms

    ACTION_T_END,
};

struct EventAI_Event
{
    uint32 creature_id;

    uint16 event_type;
    uint32 event_param1;
    uint32 event_param2;
    uint32 event_param3;

    struct _action
    {
        uint16 type;
        uint32 param1;
        uint32 param2;
        uint32 param3;
    }action[MAX_ACTIONS];
};

extern EventAI_Event EventAI_Events[MAX_EVENTS];