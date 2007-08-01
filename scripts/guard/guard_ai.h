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

// **** This script is for use within every single guard to save coding time ****

#define GENERIC_CREATURE_COOLDOWN 5000

struct MANGOS_DLL_DECL guardAI : public ScriptedAI
{
    guardAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 GlobalCooldown;      //This variable acts like the global cooldown that players have (1.5 seconds)
    uint32 BuffTimer;           //This variable keeps track of buffs
    bool InCombat;

    void EnterEvadeMode();

    void AttackStart(Unit *who);

    void MoveInLineOfSight(Unit *who);

    void UpdateAI(const uint32 diff);
}; 
