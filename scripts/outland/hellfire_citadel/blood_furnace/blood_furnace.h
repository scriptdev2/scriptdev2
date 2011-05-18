/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BLOOD_FURNACE_H
#define DEF_BLOOD_FURNACE_H

enum
{
    MAX_ENCOUNTER                   = 3,

    TYPE_THE_MAKER_EVENT            = 0,
    TYPE_BROGGOK_EVENT              = 1,
    TYPE_KELIDAN_EVENT              = 2,

    NPC_THE_MAKER                   = 17381,
    NPC_BROGGOK                     = 17380,
    NPC_KELIDAN_THE_MAKER           = 17377,

    GO_DOOR_FINAL_EXIT              = 181766,
    GO_DOOR_MAKER_FRONT             = 181811,
    GO_DOOR_MAKER_REAR              = 181812,
    GO_DOOR_BROGGOK_FRONT           = 181822,
    GO_DOOR_BROGGOK_REAR            = 181819,
    GO_DOOR_KELIDAN_EXIT            = 181823,

    GO_PRISON_CELL_MAKER1           = 181813,               // The maker cell front right
    GO_PRISON_CELL_MAKER2           = 181814,               // The maker cell back right
    GO_PRISON_CELL_MAKER3           = 181816,               // The maker cell front left
    GO_PRISON_CELL_MAKER4           = 181815,               // The maker cell back left
    GO_PRISON_CELL_BROGGOK1         = 181821,               // Broggok cell front right
    GO_PRISON_CELL_BROGGOK2         = 181818,               // Broggok cell back right
    GO_PRISON_CELL_BROGGOK3         = 181820,               // Broggok cell front left
    GO_PRISON_CELL_BROGGOK4         = 181817,               // Broggok cell back left
};

class MANGOS_DLL_DECL instance_blood_furnace : public ScriptedInstance
{
    public:
        instance_blood_furnace(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        uint64 GetData64(uint32 uiData);
        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        void Load(const char* chrIn);
        const char* Save() { return m_strInstData.c_str(); }

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint64 m_uiMakerGUID;
        uint64 m_uiBroggokGUID;
        uint64 m_uiKelidanGUID;

        uint64 m_uiDoorFinalExitGUID;
        uint64 m_uiDoorMakerFrontGUID;
        uint64 m_uiDoorMakerRearGUID;
        uint64 m_uiDoorBroggokFrontGUID;
        uint64 m_uiDoorBrokkokRearGUID;
        uint64 m_uiDoorKelidanExitGUID;

        uint64 m_uiPrisonCell1GUID;
        uint64 m_uiPrisonCell2GUID;
        uint64 m_uiPrisonCell3GUID;
        uint64 m_uiPrisonCell4GUID;
        uint64 m_uiPrisonCell5GUID;
        uint64 m_uiPrisonCell6GUID;
        uint64 m_uiPrisonCell7GUID;
        uint64 m_uiPrisonCell8GUID;
};

#endif
