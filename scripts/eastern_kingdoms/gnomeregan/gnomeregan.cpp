/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: gnomeregan
SD%Complete: 80
SDComment:  Grubbis Encounter
SDCategory: Gnomeregan
EndScriptData */

/* ContentData
npc_blastmaster_emi_shortfuse
EndContentData */

#include "precompiled.h"
#include "gnomeregan.h"
#include "escort_ai.h"

/*######
## npc_blastmaster_emi_shortfuse
######*/

enum
{
    SAY_START                   = -1090000,
    SAY_INTRO_1                 = -1090001,
    SAY_INTRO_2                 = -1090002,
    SAY_INTRO_3                 = -1090003,
    SAY_INTRO_4                 = -1090004,
    SAY_LOOK_1                  = -1090005,
    SAY_HEAR_1                  = -1090006,
    SAY_AGGRO                   = -1090007,
    SAY_CHARGE_1                = -1090008,
    SAY_CHARGE_2                = -1090009,
    SAY_BLOW_1_10               = -1090010,
    SAY_BLOW_1_5                = -1090011,
    SAY_BLOW_1                  = -1090012,
    SAY_FINISH_1                = -1090013,
    SAY_LOOK_2                  = -1090014,
    SAY_HEAR_2                  = -1090015,
    SAY_CHARGE_3                = -1090016,
    SAY_CHARGE_4                = -1090017,
    SAY_BLOW_2_10               = -1090018,
    SAY_BLOW_2_5                = -1090019,
    SAY_BLOW_SOON               = -1090020,
    SAY_BLOW_2                  = -1090021,
    SAY_FINISH_2                = -1090022,

    SAY_GRUBBIS_SPAWN           = -1090023,

    GOSSIP_ITEM_START           = -3090000,

    SPELL_EXPLOSION_NORTH       = 12158,
    SPELL_EXPLOSION_SOUTH       = 12159,
    SPELL_FIREWORKS_RED         = 11542,

    MAX_SUMMON_POSITIONS        = 33,

    NPC_GRUBBIS                 = 7361,
    NPC_CHOMPER                 = 6215,
    NPC_CAVERNDEEP_BURROWER     = 6206,
    NPC_CAVERNDEEP_AMBUSHER     = 6207
};

void AddSC_gnomeregan()
{
}
