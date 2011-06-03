/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ZULAMAN_H
#define DEF_ZULAMAN_H

enum InstanceZA
{
    MAX_ENCOUNTER           = 8,
    MAX_VENDOR              = 2,
    MAX_CHESTS              = 4,

    SAY_INST_RELEASE        = -1568067,                     // TODO Event NYI
    SAY_INST_BEGIN          = -1568068,
    SAY_INST_PROGRESS_1     = -1568069,
    SAY_INST_PROGRESS_2     = -1568070,
    SAY_INST_PROGRESS_3     = -1568071,
    SAY_INST_WARN_1         = -1568072,
    SAY_INST_WARN_2         = -1568073,
    SAY_INST_WARN_3         = -1568074,
    SAY_INST_WARN_4         = -1568075,
    SAY_INST_SACRIF1        = -1568076,
    SAY_INST_SACRIF2        = -1568077,
    SAY_INST_COMPLETE       = -1568078,

    WORLD_STATE_ID          = 3104,
    WORLD_STATE_COUNTER     = 3106,

    TYPE_EVENT_RUN          = 1,
    TYPE_AKILZON            = 2,
    TYPE_NALORAKK           = 3,
    TYPE_JANALAI            = 4,
    TYPE_HALAZZI            = 5,
    TYPE_MALACRASS          = 6,
    TYPE_ZULJIN             = 7,

    TYPE_RAND_VENDOR_1      = 8,
    TYPE_RAND_VENDOR_2      = 9,

    TYPE_RUN_EVENT_TIME     = 10,

    TYPE_J_EGGS_RIGHT       = 11,
    TYPE_J_EGGS_LEFT        = 12,

    NPC_AKILZON             = 23574,
    // NPC_NALORAKK         = 23576,
    // NPC_JANALAI          = 23578,
    NPC_HALAZZI             = 23577,
    NPC_MALACRASS           = 24239,
    // NPC_ZULJIN           = 23863,

    NPC_EGG                 = 23817,
    NPC_SPIRIT_LYNX         = 24143,

    NPC_HARRISON            = 24358,
    // Time Run Event NPCs
    NPC_TANZAR              = 23790,                        // at bear
    NPC_KRAZ                = 24024,                        // at phoenix
    NPC_ASHLI               = 24001,                        // at lynx
    NPC_HARKOR              = 23999,                        // at eagle
    // unused (TODO or TODO with DB-tools)
    NPC_TANZAR_CORPSE       = 24442,
    NPC_KRAZ_CORPSE         = 24444,
    NPC_ASHIL_CORPSE        = 24441,
    NPC_HARKOR_CORPSE       = 24443,

    GO_STRANGE_GONG         = 187359,
    GO_MASSIVE_GATE         = 186728,
    GO_WIND_DOOR            = 186858,
    GO_LYNX_TEMPLE_ENTRANCE = 186304,
    GO_LYNX_TEMPLE_EXIT     = 186303,
    GO_HEXLORD_ENTRANCE     = 186305,
    GO_WOODEN_DOOR          = 186306,
    GO_FIRE_DOOR            = 186859,

    // unused, expected to be possible to handle within Database!
    GO_TANZARS_TRUNK        = 186648,
    GO_KRAZS_PACKAGE        = 186667,
    GO_ASHLIS_BAG           = 186672,
    GO_HARKORS_SATCHEL      = 187021,
};

enum BossToChestIndex
{
    INDEX_NALORAKK          = 0,
    INDEX_JANALAI           = 1,
    INDEX_HALAZZI           = 2,
    INDEX_AKILZON           = 3
};

enum RunEventSteps
{
    RUN_START               = 1,
    RUN_FAIL                = 2,
    RUN_DONE                = 3,
    RUN_PROGRESS            = 4,
    RUN_FAIL_SOON           = 5
};

struct TimeEventNpcInfo
{
    TimeEventNpcInfo() : uiSavePosition(0) {}

    uint8 uiSavePosition;                                   // stores in what order this npc was saved (0 means unsaved)
    ObjectGuid npGuid;
};

class MANGOS_DLL_DECL instance_zulaman : public ScriptedInstance
{
    public:
        instance_zulaman(Map* pMap);

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void Update(uint32 uiDiff);

    private:
        uint8 GetKilledPreBosses();
        void DoTimeRunSay(RunEventSteps uiData);
        void DoChestEvent(BossToChestIndex uiIndex);

        std::string m_strInstData;
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        uint32 m_auiRandVendor[MAX_VENDOR];
        TimeEventNpcInfo m_aEventNpcInfo[MAX_CHESTS];

        uint32 m_uiEventTimer;
        uint32 m_uiGongCount;

        GUIDList m_lEggsGUIDList;
        uint32 m_uiEggsRemainingCount_Left;
        uint32 m_uiEggsRemainingCount_Right;
};

#endif
