/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: assembly_of_iron
SD%Complete: 0%
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_BRUNDIR_AGGRO                   = -1603056,
    SAY_BRUNDIR_WHIRL                   = -1603057,
    SAY_BRUNDIR_DEATH_1                 = -1603058,
    SAY_BRUNDIR_DEATH_2                 = -1603059,
    SAY_BRUNDIR_SLAY_1                  = -1603060,
    SAY_BRUNDIR_SLAY_2                  = -1603061,
    SAY_BRUNDIR_BERSERK                 = -1603062,
    SAY_BRUNDIR_FLY                     = -1603063,

    SAY_MOLGEIM_AGGRO                   = -1603064,
    SAY_MOLGEIM_DEATH_1                 = -1603065,
    SAY_MOLGEIM_DEATH_2                 = -1603066,
    SAY_MOLGEIM_DEATH_RUNE              = -1603067,
    SAY_MOLGEIM_SURGE                   = -1603068,
    SAY_MOLGEIM_SLAY_1                  = -1603069,
    SAY_MOLGEIM_SLAY_2                  = -1603070,
    SAY_MOLGEIM_BERSERK                 = -1603071,

    SAY_STEEL_AGGRO                     = -1603072,
    SAY_STEEL_DEATH_1                   = -1603073,
    SAY_STEEL_DEATH_2                   = -1603074,
    SAY_STEEL_SLAY_1                    = -1603075,
    SAY_STEEL_SLAY_2                    = -1603076,
    SAY_STEEL_OVERWHELM                 = -1603077,
    SAY_STEEL_BERSERK                   = -1603078,

    // Common spells
    SPELL_BERSERK                       = 62535,        // triggers 47008 after 15 min
    SPELL_SUPERCHARGE                   = 61920,
    SPELL_LIGHTNING_CHANNEL_PREFIGHT    = 61942,        // cast by Brundir on Steelbreaker
    SPELL_RUNE_OF_POWER_PREFIGHT        = 61975,        // cast by Molgeim on Stellbreaker
    //SPELL_COUNCIL_KILL_CREDIT         = 65195,        // currently missing from DBC

    // Steelbreaker
    SPELL_HIGH_VOLTAGE                  = 61890,        // phase 1 spells
    SPELL_HIGH_VOLTAGE_H                = 63498,        // probably related to 61892 - couldn't find any info regarding this one
    SPELL_FUSION_PUNCH                  = 61903,
    SPELL_FUSION_PUNCH_H                = 63493,
    SPELL_STATIC_DISRUPTION             = 61911,        // phase 2 spells
    SPELL_STATIC_DISRUPTION_H           = 63495,        // should be triggered by 64641
    SPELL_OVERWHELMING_POWER            = 61888,        // phase 3 spells
    SPELL_OVERWHELMING_POWER_H          = 64637,
    SPELL_ELECTRICAL_CHARGE             = 61900,        // triggers 61901 when target dies

    // Runemaster Molgeim
    SPELL_SHIELD                        = 62274,        // phase 1 spells
    SPELL_SHIELD_H                      = 63489,
    SPELL_RUNE_OF_POWER                 = 61973,
    SPELL_RUNE_OF_DEATH                 = 62269,        // phase 2 spells
    SPELL_RUNE_OF_DEATH_H               = 63490,
    SPELL_RUNE_OF_SUMMONING             = 62273,        // phase 3 spells

    // Stormcaller Brundir
    SPELL_CHAIN_LIGHTNING               = 61879,        // phase 1 spells
    SPELL_CHAIN_LIGHTNING_H             = 63479,
    SPELL_OVERLOAD                      = 61869,
    SPELL_LIGHTNING_WHIRL               = 61915,        // phase 2 spells
    SPELL_LIGHTNING_WHIRL_H             = 63483,
    SPELL_LIGHTNING_WHIRL_DAMAGE        = 61916,        // used to check achiev criterias
    SPELL_LIGHTNING_WHIRL_DAMAGE_H      = 63482,
    SPELL_STORMSHIELD                   = 64187,        // phase 3 spells
    SPELL_LIGHTNING_TENDRILS            = 61887,
    SPELL_LIGHTNING_TENDRILS_H          = 63486,
    SPELL_TENDRILS_VISUAL               = 61883,

    // Summoned spells
    SPELL_OVERLOAD_AURA                 = 61877,
    SPELL_RUNE_OF_POWER_AURA            = 61974,
    SPELL_RUNE_OF_SUMMONING_AURA        = 62019,        // triggers 62020 which summons 32958

    // summoned npcs
    NPC_OVERLOAD_VISUAL                 = 32866,
    NPC_RUNE_OF_POWER                   = 33705,
    NPC_RUNE_OF_SUMMONING               = 33051,
};

void AddSC_boss_assembly_of_iron()
{

}
