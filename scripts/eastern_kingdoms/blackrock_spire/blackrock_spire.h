/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BLACKROCK_SPIRE_H
#define DEF_BLACKROCK_SPIRE_H

enum
{
    MAX_ENCOUNTER               = 5,
    MAX_ROOMS                   = 7,

    TYPE_ROOM_EVENT             = 1,
    TYPE_EMBERSEER              = 2,
    TYPE_FLAMEWREATH            = 3,                        // Only summon once per instance
    TYPE_GYTH                   = 4,
    TYPE_VALTHALAK              = 5,                        // Only summon once per instance

    NPC_SCARSHIELD_INFILTRATOR  = 10299,
    NPC_BLACKHAND_SUMMONER      = 9818,
    NPC_BLACKHAND_VETERAN       = 9819,
    NPC_PYROGUARD_EMBERSEER     = 9816,
    NPC_BLACKHAND_INCANCERATOR  = 10316,
    NPC_LORD_VICTOR_NEFARIUS    = 10162,
    NPC_GYTH                    = 10339,

    // Doors
    GO_EMBERSEER_IN             = 175244,
    GO_DOORS                    = 175705,
    GO_EMBERSEER_OUT            = 175153,
    GO_GYTH_ENTRY_DOOR          = 164726,
    GO_GYTH_COMBAT_DOOR         = 175185,                   // control in boss_script, because will auto-close after each wave
    GO_GYTH_EXIT_DOOR           = 175186,


    GO_ROOM_7_RUNE              = 175194,
    GO_ROOM_3_RUNE              = 175195,
    GO_ROOM_6_RUNE              = 175196,
    GO_ROOM_1_RUNE              = 175197,
    GO_ROOM_5_RUNE              = 175198,
    GO_ROOM_2_RUNE              = 175199,
    GO_ROOM_4_RUNE              = 175200,

    GO_ROOKERY_EGG              = 175124,
};

class MANGOS_DLL_DECL instance_blackrock_spire : public ScriptedInstance
{
    public:
        instance_blackrock_spire(Map* pMap);
        ~instance_blackrock_spire() {}

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        void SetData64(uint32 uiType, uint64 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiType);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

        void DoSortRoomEventMobs();
        void GetIncanceratorGUIDList(std::list<uint64> &lList) { lList = m_lIncanceratorGUIDList; }
        void GetRookeryEggGUIDList(std::list<uint64> &lList) { lList = m_lRookeryEggGUIDList; }

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;
        
        uint64 m_uiEmberseerGUID;
        uint64 m_uiNefariusGUID;
        uint64 m_uiGythGUID;
        uint64 m_uiInfiltratorGUID;

        uint64 m_uiEmberseerInDoorGUID;
        uint64 m_uiEmberseerCombatDoorGUID;
        uint64 m_uiEmberseerOutDoorGUID;
        uint64 m_uiGythEntryDoorGUID;
        uint64 m_uiGythCombatDoorGUID;
        uint64 m_uiGythExitDoorGUID;

        uint64 m_auiRoomRuneGUID[MAX_ROOMS];
        std::list<uint64> m_alRoomEventMobGUIDSorted[MAX_ROOMS];
        std::list<uint64> m_lRoomEventMobGUIDList;
        std::list<uint64> m_lIncanceratorGUIDList;
        std::list<uint64> m_lRookeryEggGUIDList;
};

#endif
