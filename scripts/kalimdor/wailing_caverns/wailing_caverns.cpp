/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: wailing_caverns
SD%Complete: 0
SDComment: Placeholder
SDCategory: Wailing Caverns
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_PREPARE             = -1043001,
    SAY_FIRST_CORNER        = -1043002,
    SAY_CONTINUE            = -1043003,
    SAY_CIRCLE_BANISH       = -1043004,
    SAY_PURIFIED            = -1043005,
    SAY_NARALEX_CHAMBER     = -1043016,
    SAY_BEGIN_RITUAL        = -1043007,
    SAY_MUTANUS             = -1043012,
    SAY_NARALEX_AWAKE       = -1043013,
    SAY_AWAKE               = -1043014,
    SAY_NARALEX_THANKYOU    = -1043015,
    SAY_FAREWELL            = -1043016,
    SAY_AGGRO_1             = -1043017,                     // Random between the first 2
    SAY_AGGRO_2             = -1043018,
    SAY_AGGRO_3             = -1043019,                     // During the awakening ritual

    EMOTE_RITUAL_BEGIN      = -1043008,
    EMOTE_NARALEX_AWAKE     = -1043009,
    EMOTE_BREAK_THROUGH     = -1043010,
    EMOTE_VISION            = -1043011,

    GOSSIP_ITEM_BEGIN       = -3043000,
    TEXT_ID_DISCIPLE        = 698,

    SPELL_MARK              = 5232,                         // Buff before the fight. To be removed after 4.0.3
    SPELL_SLEEP             = 1090,
    SPELL_POTION            = 8141,
    SPELL_CLEANSING         = 6270,
    SPELL_AWAKENING         = 6271,
    SPELL_SHAPESHIFT        = 8153,

    NPC_DEVIATE_RAPTOR      = 3636,                         // 2 of them at the first stop
    NPC_DEVIATE_VIPER       = 5755,                         // 3 of them at the circle
    NPC_DEVIATE_MOCCASIN    = 5762,                         // 6 of them at Naralex chamber
    NPC_NIGHTMARE_ECTOPLASM = 5763,                         // 10 of them at Naralex chamber
    NPC_MUTANUS             = 3654
};

void AddSC_wailing_caverns()
{
}
