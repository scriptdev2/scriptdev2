/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_GUNDRAK_H
#define DEF_GUNDRAK_H
/* Encounters
 * Slad'ran          = 1
 * Drakkari Colossus = 2
 * Moorabi           = 3
 * Gal'darah         = 4
 * Eck the Ferocious = 5
*/
enum
{
    MAX_ENCOUNTER          = 5,

    TYPE_SLADRAN           = 1,
    TYPE_COLOSSUS          = 2,
    TYPE_MOORABI           = 3,
    TYPE_GALDARAH          = 4,
    TYPE_ECK               = 5,

    NPC_SLADRAN            = 29304,
    NPC_MOORABI            = 29307,
    NPC_COLOSSUS           = 29305,
    NPC_GALDARAH           = 29306,
    NPC_ECK                = 29932,

    GO_ECK_DOOR            = 192632,
    GO_ECK_UNDERWATER_DOOR = 192569,
    GO_GALDARAH_DOOR       = 192568,
    GO_EXIT_DOOR_L         = 193208,
    GO_EXIT_DOOR_R         = 193207,

    GO_ALTAR_OF_SLADRAN    = 192518,
    GO_ALTAR_OF_MOORABI    = 192519,
    GO_ALTAR_OF_COLOSSUS   = 192520,

    GO_SNAKE_KEY           = 192564,
    GO_TROLL_KEY           = 192567,
    GO_MAMMOTH_KEY         = 192565,

    GO_BRIDGE              = 193188
};

#endif
