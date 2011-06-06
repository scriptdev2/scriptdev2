/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BLOOD_FURNACE_H
#define DEF_BLOOD_FURNACE_H

enum
{
    MAX_ENCOUNTER                   = 3,
    MAX_ORC_WAVES                   = 4,

    TYPE_THE_MAKER_EVENT            = 0,
    TYPE_BROGGOK_EVENT              = 1,
    TYPE_KELIDAN_EVENT              = 2,

    // NPC_THE_MAKER                = 17381,
    NPC_BROGGOK                     = 17380,
    // NPC_KELIDAN_THE_BREAKER      = 17377,
    NPC_NASCENT_FEL_ORC             = 17398,                // Used in the Broggok event

    GO_DOOR_FINAL_EXIT              = 181766,
    GO_DOOR_MAKER_FRONT             = 181811,
    GO_DOOR_MAKER_REAR              = 181812,
    GO_DOOR_BROGGOK_FRONT           = 181822,
    GO_DOOR_BROGGOK_REAR            = 181819,
    GO_DOOR_KELIDAN_EXIT            = 181823,

    // GO_PRISON_CELL_MAKER1        = 181813,               // The maker cell front right
    // GO_PRISON_CELL_MAKER2        = 181814,               // The maker cell back right
    // GO_PRISON_CELL_MAKER3        = 181816,               // The maker cell front left
    // GO_PRISON_CELL_MAKER4        = 181815,               // The maker cell back left

    GO_PRISON_CELL_BROGGOK_1        = 181817,               // Broggok cell back left   (NE)
    GO_PRISON_CELL_BROGGOK_2        = 181818,               // Broggok cell back right  (SE)
    GO_PRISON_CELL_BROGGOK_3        = 181820,               // Broggok cell front left  (NW)
    GO_PRISON_CELL_BROGGOK_4        = 181821,               // Broggok cell front right (SW)

    SAY_BROGGOK_INTRO               = -1542015,
};

struct BroggokEventInfo
{
    BroggokEventInfo() : m_bIsCellOpened(false), m_uiKilledOrcCount(0) {}

    ObjectGuid m_cellGuid;
    bool m_bIsCellOpened;
    uint8 m_uiKilledOrcCount;
    GUIDSet m_sSortedOrcGuids;
};

class MANGOS_DLL_DECL instance_blood_furnace : public ScriptedInstance
{
    public:
        instance_blood_furnace(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void OnCreatureDeath(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        void Update(uint32 uiDiff);

        void Load(const char* chrIn);
        const char* Save() { return m_strInstData.c_str(); }

        void GetMovementDistanceForIndex(uint32 uiIndex, float& dx, float& dy);

    private:
        void DoSortBroggokOrcs();
        void DoNextBroggokEventPhase();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        BroggokEventInfo m_aBroggokEvent[MAX_ORC_WAVES];

        uint32 m_uiBroggokEventTimer;                       // Timer for opening the event cages; only on heroic mode = 30 secs
        uint32 m_uiBroggokEventPhase;

        GUIDList m_luiNascentOrcGUIDs;
};

#endif
