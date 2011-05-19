/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_GRUULS_LAIR_H
#define DEF_GRUULS_LAIR_H

enum
{
    MAX_ENCOUNTER                   = 2,

    // Encounter Status
    TYPE_MAULGAR_EVENT              = 1,
    TYPE_GRUUL_EVENT                = 2,

    GO_PORT_GRONN_1                 = 183817,               // 184468 not in use
    GO_PORT_GRONN_2                 = 184662,

    // NPC GUIDs
    NPC_MAULGAR                    = 18831,
    NPC_BLINDEYE                   = 18836,
    NPC_KIGGLER                    = 18835,
    NPC_KROSH                      = 18832,
    NPC_OLM                        = 18834,
};

struct MANGOS_DLL_DECL instance_gruuls_lair : public ScriptedInstance
{
    public:
        instance_gruuls_lair(Map *pMap);

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return m_strSaveData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strSaveData;

        uint64 m_uiMaulgarGUID;
        uint64 m_uiKigglerGUID;
        uint64 m_uiBlindeyeGUID;
        uint64 m_uiOlmGUID;
        uint64 m_uiKroshGUID;
        uint64 m_uiMaulgarDoorGUID;
        uint64 m_uiGruulEncounterDoorGUID;
};

#endif
