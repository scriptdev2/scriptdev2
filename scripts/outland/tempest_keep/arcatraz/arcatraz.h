/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ARCATRAZ_H
#define DEF_ARCATRAZ_H

enum
{
    MAX_ENCOUNTER                   = 9,

    TYPE_ZEREKETH                   = 1,
    TYPE_DALLIAH                    = 2,                    // Handled in ACID (20885 - Dalliah the Doomsayer)
    TYPE_SOCCOTHRATES               = 3,                    // Handled in ACID (20886 - Wrath-Scryer Soccothrates)
    TYPE_HARBINGERSKYRISS           = 4,                    // Handled with ACID (FAIL of 20905, 20906, 20908, 20909, 20910, 20911)
    TYPE_WARDEN_1                   = 5,                    // Handled with ACID (20905 - Blazing Trickster, 20906 - Phase-Hunter)
    TYPE_WARDEN_2                   = 6,
    TYPE_WARDEN_3                   = 7,                    // Handled with ACID (20908 - Akkiris Lightning-Waker, 20909 - Sulfuron Magma-Thrower)
    TYPE_WARDEN_4                   = 8,                    // Handled with ACID (20910 - Twilight Drakonaar, 20911 - Blackwing Drakonaar)
    TYPE_WARDEN_5                   = 9,

    NPC_MELLICHAR                   = 20904,                // Skyriss will kill this unit

    GO_CORE_SECURITY_FIELD_ALPHA    = 184318,               // Door opened when Wrath-Scryer Soccothrates dies
    GO_CORE_SECURITY_FIELD_BETA     = 184319,               // Door opened when Dalliah the Doomsayer dies
    GO_SEAL_SPHERE                  = 184802,               // Shield 'protecting' mellichar
    GO_POD_ALPHA                    = 183961,               // Pod first boss wave
    GO_POD_BETA                     = 183963,               // Pod second boss wave
    GO_POD_DELTA                    = 183964,               // Pod third boss wave
    GO_POD_GAMMA                    = 183962,               // Pod fourth boss wave
    GO_POD_OMEGA                    = 183965,               // Pod fifth boss wave
};

#endif
