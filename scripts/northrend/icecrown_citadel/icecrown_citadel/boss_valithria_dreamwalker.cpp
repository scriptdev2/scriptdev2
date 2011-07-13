/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: boss_valithria
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"

enum SvalnaTexts                                            // TODO Maybe need own file?
{
    SAY_SVALNA_EVENT_1          = 1631130,
    SAY_SVALNA_EVENT_2          = 1631131,
    SAY_SVALNA_EVENT_3          = 1631132,
    SAY_SVALNA_EVENT_4          = 1631133,
    SAY_KILLING_CRUSADERS       = 1631134,
    SAY_RESSURECT               = 1631135,
    SAY_SVALNA_AGGRO            = 1631136,
    SAY_KILL_CAPTAIN            = 1631137,
    SAY_KILL_PLAYER             = 1631138,
    SAY_DEATH                   = 1631139,
};

enum
{
    SAY_AGGRO                   = 1631140,
    SAY_PORTAL                  = 1631141,
    SAY_75_HEALTH               = 1631142,
    SAY_25_HEALTH               = 1631143,
    SAY_0_HEALTH                = 1631144,
    SAY_PLAYER_DIES             = 1631145,
    SAY_BERSERK                 = 1631146,
    SAY_VICTORY                 = 1631147,
};

void AddSC_boss_valithria_dreamwalker()
{
}
