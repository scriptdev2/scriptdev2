/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_KARAZHAN_H
#define DEF_KARAZHAN_H

enum
{
    MAX_ENCOUNTER                   = 11,

    TYPE_ATTUMEN                    = 0,
    TYPE_MOROES                     = 1,
    TYPE_MAIDEN                     = 2,
    TYPE_OPERA                      = 3,
    TYPE_CURATOR                    = 4,
    TYPE_TERESTIAN                  = 5,
    TYPE_ARAN                       = 6,
    TYPE_NETHERSPITE                = 7,
    TYPE_CHESS                      = 8,
    TYPE_MALCHEZZAR                 = 9,
    TYPE_NIGHTBANE                  = 10,

    DATA_OPERA_PERFORMANCE          = 11,
    DATA_OPERA_OZ_DEATHCOUNT        = 12,

    NPC_MOROES                      = 15687,
    NPC_TERESTIAN                   = 15688,
    NPC_NIGHTBANE                   = 17225,

    GO_STAGE_CURTAIN                = 183932,
    GO_STAGE_DOOR_LEFT              = 184278,
    GO_STAGE_DOOR_RIGHT             = 184279,
    GO_PRIVATE_LIBRARY_DOOR         = 184517,
    GO_MASSIVE_DOOR                 = 185521,
    GO_GAMESMANS_HALL_DOOR          = 184276,
    GO_GAMESMANS_HALL_EXIT_DOOR     = 184277,
    GO_NETHERSPACE_DOOR             = 185134,
    GO_SIDE_ENTRANCE_DOOR           = 184275,
    GO_DUST_COVERED_CHEST           = 185119,

    // Opera event stage decoration
    GO_OZ_BACKDROP                  = 183442,
    GO_OZ_HAY                       = 183496,
    GO_HOOD_BACKDROP                = 183491,
    GO_HOOD_TREE                    = 183492,
    GO_HOOD_HOUSE                   = 183493,
    GO_RAJ_BACKDROP                 = 183443,
    GO_RAJ_MOON                     = 183494,
    GO_RAJ_BALCONY                  = 183495,
};

enum OperaEvents
{
    EVENT_OZ                        = 1,
    EVENT_HOOD                      = 2,
    EVENT_RAJ                       = 3
};

#define ERROR_INST_DATA(a)          error_log("SD2: Instance Data for Karazhan not set properly. Encounter for Creature Entry %u may not work properly.", a->GetEntry());

class MANGOS_DLL_DECL instance_karazhan : public ScriptedInstance
{
    public:
        instance_karazhan(Map* pMap);
        ~instance_karazhan() {}

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        void Load(const char* chrIn);
        const char* Save() { return m_strInstData.c_str(); }

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint32 m_uiOperaEvent;
        uint32 m_uiOzDeathCount;

        uint64 m_uiMoroesGUID;
        uint64 m_uiTerestianGUID;
        uint64 m_uiNightbaneGUID;

        uint64 m_uiCurtainGUID;
        uint64 m_uiStageDoorLeftGUID;
        uint64 m_uiStageDoorRightGUID;
        uint64 m_uiLibraryDoor;                             // Door at Shade of Aran
        uint64 m_uiMassiveDoor;                             // Door at Netherspite
        uint64 m_uiSideEntranceDoor;                        // Side Entrance
        uint64 m_uiGamesmansDoor;                           // Door before Chess
        uint64 m_uiGamesmansExitDoor;                       // Door after Chess
        uint64 m_uiNetherspaceDoor;                         // Door at Malchezaar
        uint64 m_uiDustCoveredChest;                        // Chest respawn at event complete
};

class MANGOS_DLL_DECL npc_fiendish_portalAI : public ScriptedAI
{
    public:
        npc_fiendish_portalAI(Creature* pCreature);
        ~npc_fiendish_portalAI() {}

        void Reset();
        void JustSummoned(Creature* pSummoned);
        void UpdateAI(const uint32 uiDiff);

        uint32 m_uiSummonTimer;
};

#endif
