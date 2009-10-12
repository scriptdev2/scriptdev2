/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_NAXXRAMAS_H
#define DEF_NAXXRAMAS_H

enum
{
    MAX_ENCOUNTER               = 15,

    TYPE_ANUB_REKHAN            = 1,
    TYPE_FAERLINA               = 2,
    TYPE_MAEXXNA                = 3,

    TYPE_NOTH                   = 4,
    TYPE_HEIGAN                 = 5,
    TYPE_LOATHEB                = 6,

    TYPE_RAZUVIOUS              = 7,
    TYPE_GOTHIK                 = 8,
    TYPE_FOUR_HORSEMEN          = 9,

    TYPE_PATCHWERK              = 10,
    TYPE_GROBBULUS              = 11,
    TYPE_GLUTH                  = 12,
    TYPE_THADDIUS               = 13,
    TYPE_STALAGG                = 14,
    TYPE_FEUGEN                 = 15,

    TYPE_SAPPHIRON              = 16,
    TYPE_KELTHUZAD              = 17,

    NPC_ANUB_REKHAN             = 15956,
    NPC_FAERLINA                = 15953,

    NPC_THADDIUS                = 15928,
    NPC_STALAGG                 = 15929,
    NPC_FEUGEN                  = 15930,

    NPC_ZELIEK                  = 16063,
    NPC_THANE                   = 16064,
    NPC_BLAUMEUX                = 16065,
    NPC_RIVENDARE               = 30549,

    // Arachnid Quarter
    GO_ARAC_ANUB_DOOR           = 181126,                   //encounter door
    GO_ARAC_ANUB_GATE           = 181195,                   //open after boss is dead
    GO_ARAC_FAER_WEB            = 181235,                   //encounter door
    GO_ARAC_FAER_DOOR           = 194022,                   //after faerlina, to outer ring
    GO_ARAC_MAEX_INNER_DOOR     = 181197,                   //encounter door
    GO_ARAC_MAEX_OUTER_DOOR     = 181209,                   //right before maex

    // Plague Quarter
    GO_PLAG_SLIME01_DOOR        = 181198,                   //not used
    GO_PLAG_SLIME02_DOOR        = 181199,                   //not used
    GO_PLAG_NOTH_ENTRY_DOOR     = 181200,                   //encounter door
    GO_PLAG_NOTH_EXIT_DOOR      = 181201,                   //exit, open when boss dead
    GO_PLAG_HEIG_ENTRY_DOOR     = 181202,
    GO_PLAG_HEIG_EXIT_DOOR      = 181203,                   //exit, open when boss dead
    GO_PLAG_LOAT_DOOR           = 181241,                   //encounter door

    // Military Quarter
    GO_MILI_GOTH_ENTRY_GATE     = 181124,                   //open after razuvious died
    GO_MILI_GOTH_EXIT_GATE      = 181125,                   //exit, open at boss dead
    GO_MILI_GOTH_COMBAT_GATE    = 181170,                   //used while encounter is in progress
    GO_MILI_HORSEMEN_DOOR       = 181119,                   //encounter door

    GO_CHEST_HORSEMEN_NORM      = 181366,                   //four horsemen event, DoRespawnGameObject() when event == DONE
    GO_CHEST_HORSEMEN_HERO      = 193426,

    // Construct Quarter
    GO_CONS_PATH_EXIT_DOOR      = 181123,
    GO_CONS_GLUT_EXIT_DOOR      = 181120,
    GO_CONS_THAD_DOOR           = 181121,                   // Thaddius enc door

    // Frostwyrm Lair
    GO_KELTHUZAD_WATERFALL_DOOR = 181225,                   // exit, open after sapphiron is dead

    // Eyes
    GO_ARAC_EYE_RAMP            = 181212,
    GO_PLAG_EYE_RAMP            = 181211,
    GO_MILI_EYE_RAMP            = 181210,
    GO_CONS_EYE_RAMP            = 181213,

    // Portals
    GO_ARAC_PORTAL              = 181575,
    GO_PLAG_PORTAL              = 181577,
    GO_MILI_PORTAL              = 181578,
    GO_CONS_PORTAL              = 181576,

    AREATRIGGER_FROSTWYRM       = 4120                     //not needed here, but AT to be scripted
};

#endif
