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
SDName: ulduar
SD%Complete: 0%
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

/*#####
## Teleporters
#####*/
enum teleporter_spells
{
    SPELL_TELE_EXPEDITION_BASE_CAMP         = 64014,
    SPELL_TELE_FORMATION_GROUNDS            = 64032,
    SPELL_TELE_COLOSSAL_FORGE               = 64028,
    SPELL_TELE_SCRAPYARD                    = 64031,
    SPELL_TELE_ANTECHAMBER_OF_ULDUAR        = 64030,
    SPELL_TELE_SHATTERED_WALKWAY            = 64029,
    SPELL_TELE_CONSERVATORY_OF_LIFE         = 64024,
    SPELL_TELE_SPARK_OF_IMAGINATION         = 65061,
    SPELL_TELE_PRISON_OF_YOGG               = 65042,
};

// Teleporter Gossip handled by SD2 because depending on Instance Data
enum teleporter_gossip_items
{
    GOSSIP_ITEM_TELE_BASE_CAMP              = -3603000,
    GOSSIP_ITEM_TELE_FORMATION_GROUNDS      = -3603001,
    GOSSIP_ITEM_TELE_COLOSSAR_FORGE         = -3603002,
    GOSSIP_ITEM_TELE_SCRAPYARD              = -3603003,
    GOSSIP_ITEM_TELE_ANTECHAMBER            = -3603004,
    GOSSIP_ITEM_TELE_WALKWAY                = -3603005,
    GOSSIP_ITEM_TELE_CONSERVATORY           = -3603006,
    GOSSIP_ITEM_TELE_SPARK_IMAGINATION      = -3603007,
    GOSSIP_ITEM_TELE_YOGG_SARON             = -3603008,
};
