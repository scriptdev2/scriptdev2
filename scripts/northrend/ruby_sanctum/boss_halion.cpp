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
SDName: boss_halion
SD%Complete: 20
SDComment: only yells
SDCategory: Ruby Sanctum
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_SPAWN                   = -1724024,
    SAY_AGGRO                   = -1724025,
    SAY_SLAY                    = -1724026,                 // There is an additonal sound entry related to SLAY: 17502
    SAY_DEATH                   = -1724027,
    SAY_BERSERK                 = -1724028,
    SAY_FIREBALL                = -1724029,
    SAY_SPHERES                 = -1724030,
    SAY_PHASE_2                 = -1724031,
    SAY_PHASE_3                 = -1724032,

    EMOTE_SPHERES               = -1724033,
    EMOTE_OUT_OF_TWILLIGHT      = -1724034,
    EMOTE_OUT_OF_PHYSICAL       = -1724035,
    EMOTE_INTO_TWILLIGHT        = -1724036,
    EMOTE_INTO_PHYSICAL         = -1724037,
    EMOTE_REGENERATE            = -1724038,
};

void AddSC_boss_halion()
{
}
