/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_RAMPARTS_H
#define DEF_RAMPARTS_H

enum
{
    MAX_ENCOUNTER               = 2,

    TYPE_VAZRUDEN               = 1,
    TYPE_NAZAN                  = 2,                        // Do not change, used in ACID (SetData(SPECIAL) on death of 17517

    NPC_HERALD                  = 17307,

    GO_FEL_IRON_CHEST           = 185168,
    GO_FEL_IRON_CHEST_H         = 185169,
};

class MANGOS_DLL_DECL instance_ramparts : public ScriptedInstance
{
    public:
        instance_ramparts(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        // No need to save and load this instance (only one encounter needs special handling, no doors used)

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];

        uint32 m_uiSentryCounter;
        uint64 m_uiChestGUID;
        uint64 m_uiHeraldGUID;
};

#endif
