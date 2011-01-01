/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_HYJAL_H
#define DEF_HYJAL_H

#define ERROR_INST_DATA     "SD2: Instance data not set properly for Mount Hyjal. Encounters will be buggy"

enum eNPC
{
    NPC_JAINA               = 17772,
    NPC_THRALL              = 17852,
    NPC_TYRANDE             = 17948,

    // Bosses summoned after every 8 waves
    NPC_WINTERCHILL         = 17767,
    NPC_ANETHERON           = 17808,
    NPC_KAZROGAL            = 17888,
    NPC_AZGALOR             = 17842,
    NPC_ARCHIMONDE          = 17968,

    // Trash Mobs summoned in waves
    NPC_NECRO               = 17899,
    NPC_ABOMI               = 17898,
    NPC_GHOUL               = 17895,
    NPC_BANSH               = 17905,
    NPC_CRYPT               = 17897,
    NPC_GARGO               = 17906,
    NPC_FROST               = 17907,
    NPC_GIANT               = 17908,
    NPC_STALK               = 17916,

    NPC_WATER_ELEMENTAL     = 18001,
    NPC_DIRE_WOLF           = 17854,

    GO_ANCIENT_GEM          = 185557
};

enum
{
    MAX_ENCOUNTER               = 5,

    WORLD_STATE_WAVES           = 2842,
    WORLD_STATE_ENEMY           = 2453,
    WORLD_STATE_ENEMYCOUNT      = 2454,

    // ACID scripted, don't touch id's (or provide update for ACID)
    TYPE_WINTERCHILL            = 11,
    TYPE_ANETHERON              = 2,
    TYPE_KAZROGAL               = 9,
    TYPE_AZGALOR                = 6,

    TYPE_ARCHIMONDE             = 4,

    DATA_ANETHERON              = 1,
    DATA_RAGEWINTERCHILL        = 10,
    DATA_AZGALOR                = 5,
    DATA_KAZROGAL               = 8,

    DATA_ARCHIMONDE             = 3,
    DATA_JAINAPROUDMOORE        = 7,
    DATA_THRALL                 = 12,
    DATA_TYRANDEWHISPERWIND     = 13,

    DATA_TRASH                  = 14,
    DATA_RESET_TRASH_COUNT      = 15,
    TYPE_RETREAT                = 16
};

#endif
