/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SUNWELLPLATEAU_H
#define DEF_SUNWELLPLATEAU_H

enum InstanceSWP
{
    MAX_ENCOUNTER               = 6,

    TYPE_KALECGOS               = 0,
    TYPE_BRUTALLUS              = 1,
    TYPE_FELMYST                = 2,
    TYPE_EREDAR_TWINS           = 3,
    TYPE_MURU                   = 4,
    TYPE_KILJAEDEN              = 5,

    DATA_KALECGOS_DRAGON        = 6,
    DATA_KALECGOS_HUMAN         = 7,
    DATA_SATHROVARR             = 8,
    DATA_BRUTALLUS              = 9,
    DATA_FELMYST                = 10,
    DATA_ALYTHESS               = 11,
    DATA_SACROLASH              = 12,
    DATA_MURU                   = 13,
    DATA_KILJAEDEN              = 14,
    DATA_KILJAEDEN_CONTROLLER   = 15,
    DATA_ANVEENA                = 16,
    DATA_KALECGOS               = 17,

    DATA_GO_FORCEFIELD          = 18,
    DATA_GO_FIRE_BARRIER        = 19,
    DATA_GO_FIRST_GATE          = 20,
    DATA_GO_SECOND_GATE         = 21,
    DATA_GO_RAID_GATE_07        = 22,
    DATA_GO_RAID_GATE_08        = 23,
    DATA_GO_THIRD_GATE          = 24,

    DATA_PLAYER_SPECTRAL_REALM  = 25,
    DATA_SET_SPECTRAL_CHECK     = 26,

    SPELL_SPECTRAL_REALM                = 46021,
    SPELL_TELEPORT_NORMAL_REALM         = 46020,
    SPELL_TELEPORT_TO_SPECTRAL_REALM    = 46019,
    SPELL_SPECTRAL_EXHAUSTION           = 44867,
    SPELL_SPECTRAL_REALM_FORCE_FACTION  = 44852
};
#endif
