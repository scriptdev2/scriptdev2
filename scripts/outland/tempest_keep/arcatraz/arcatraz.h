/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ARCATRAZ_H
#define DEF_ARCATRAZ_H

enum
{
    MAX_ENCOUNTER                   = 9,

    GO_CORE_SECURITY_FIELD_ALPHA    = 184318,               //door opened when Wrath-Scryer Soccothrates dies
    GO_CORE_SECURITY_FIELD_BETA     = 184319,               //door opened when Dalliah the Doomsayer dies
    GO_SEAL_SPHERE                  = 184802,               //shield 'protecting' mellichar
    GO_POD_ALPHA                    = 183961,               //pod first boss wave
    GO_POD_BETA                     = 183963,               //pod second boss wave
    GO_POD_DELTA                    = 183964,               //pod third boss wave
    GO_POD_GAMMA                    = 183962,               //pod fourth boss wave
    GO_POD_OMEGA                    = 183965,               //pod fifth boss wave

    NPC_MELLICHAR                   = 20904,                //skyriss will kill this unit

    TYPE_ZEREKETH                   = 1,
    TYPE_DALLIAH                    = 2,
    TYPE_SOCCOTHRATES               = 3,
    TYPE_HARBINGERSKYRISS           = 4,
    TYPE_WARDEN_1                   = 5,
    TYPE_WARDEN_2                   = 6,
    TYPE_WARDEN_3                   = 7,
    TYPE_WARDEN_4                   = 8,
    TYPE_WARDEN_5                   = 9,

    DATA_MELLICHAR                  = 10,
    DATA_SPHERE_SHIELD              = 11
};

#endif
