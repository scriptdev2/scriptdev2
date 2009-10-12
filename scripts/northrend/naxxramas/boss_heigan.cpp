/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Heigan
SD%Complete: 0
SDComment: Place Holder
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1      = -1533109,
    SAY_AGGRO2      = -1533110,
    SAY_AGGRO3      = -1533111,
    SAY_SLAY        = -1533112,
    SAY_TAUNT1      = -1533113,
    SAY_TAUNT2      = -1533114,
    SAY_TAUNT3      = -1533115,
    SAY_TAUNT4      = -1533116,
    SAY_TAUNT5      = -1533117,
    SAY_DEATH       = -1533118,

    //Spell used by floor peices to cause damage to players
    SPELL_ERUPTION  = 29371,

    //Spells by boss
    SPELL_WILT      = 23772,
    SPELL_FEAVER    = 29998,

    //Spell by eye stalks
    SPELL_MIND_FLAY = 26143
};
