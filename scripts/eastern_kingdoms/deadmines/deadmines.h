/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_DEADMINES_H
#define DEF_DEADMINES_H

enum
{
    MAX_ENCOUNTER           = 2,

    TYPE_DEFIAS_ENDDOOR     = 1,
    TYPE_RHAHKZOR           = 2,

    DATA_DEFIAS_DOOR        = 2,

    INST_SAY_ALARM1         = -1036000,
    INST_SAY_ALARM2         = -1036001,

    GO_FACTORY_DOOR         = 13965,
    GO_DOOR_LEVER           = 101833,
    GO_IRON_CLAD            = 16397,
    GO_DEFIAS_CANNON        = 16398,
    GO_SMITE_CHEST          = 144111,                       // use to get correct location of mr.smites equipment changes

    NPC_RHAHKZOR            = 644,
    NPC_MR_SMITE            = 646,
    NPC_PIRATE              = 657
};

#endif
