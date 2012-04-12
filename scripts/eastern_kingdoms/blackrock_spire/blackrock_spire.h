/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
    TYPE_STADIUM                = 4,
    TYPE_VALTHALAK              = 5,                        // Only summon once per instance

    NPC_SCARSHIELD_INFILTRATOR  = 10299,
    NPC_BLACKHAND_SUMMONER      = 9818,
    NPC_BLACKHAND_VETERAN       = 9819,
    NPC_PYROGUARD_EMBERSEER     = 9816,
    NPC_BLACKHAND_INCARCERATOR  = 10316,
    NPC_LORD_VICTOR_NEFARIUS    = 10162,
    NPC_REND_BLACKHAND          = 10429,
    NPC_GYTH                    = 10339,
    NPC_DRAKKISATH              = 10363,
    NPC_FIRE_TONGUE             = 10372,                    // related to Gyth arena event
    NPC_CHROMATIC_WHELP         = 10442,
    NPC_CHROMATIC_DRAGON        = 10447,
    NPC_BLACKHAND_ELITE         = 10317,

    // Doors
    GO_EMBERSEER_IN             = 175244,
    GO_DOORS                    = 175705,
    GO_EMBERSEER_OUT            = 175153,
    GO_GYTH_ENTRY_DOOR          = 164726,
    GO_GYTH_COMBAT_DOOR         = 175185,                   // control in boss_script, because will auto-close after each wave
    GO_GYTH_EXIT_DOOR           = 175186,
    GO_DRAKKISATH_DOOR_1        = 175946,
    GO_DRAKKISATH_DOOR_2        = 175947,

    GO_ROOM_7_RUNE              = 175194,
    GO_ROOM_3_RUNE              = 175195,
    GO_ROOM_6_RUNE              = 175196,
    GO_ROOM_1_RUNE              = 175197,
    GO_ROOM_5_RUNE              = 175198,
    GO_ROOM_2_RUNE              = 175199,
    GO_ROOM_4_RUNE              = 175200,

    GO_ROOKERY_EGG              = 175124,

    GO_EMBERSEER_RUNE_1         = 175266,
    GO_EMBERSEER_RUNE_2         = 175267,
    GO_EMBERSEER_RUNE_3         = 175268,
    GO_EMBERSEER_RUNE_4         = 175269,
    GO_EMBERSEER_RUNE_5         = 175270,
    GO_EMBERSEER_RUNE_6         = 175271,
    GO_EMBERSEER_RUNE_7         = 175272,
};

class MANGOS_DLL_DECL instance_blackrock_spire : public ScriptedInstance
{
    public:
        instance_blackrock_spire(Map* pMap);
        ~instance_blackrock_spire() {}

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureEnterCombat(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        void SetData64(uint32 uiType, uint64 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void DoUseEmberseerRunes(bool bReset = false);
        void DoProcessEmberseerEvent();

        void DoSortRoomEventMobs();
        void GetIncarceratorGUIDList(GUIDList &lList) { lList = m_lIncarceratorGUIDList; }
        void GetRookeryEggGUIDList(GUIDList &lList) { lList = m_lRookeryEggGUIDList; }

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        ObjectGuid m_aRoomRuneGuid[MAX_ROOMS];
        GUIDList m_alRoomEventMobGUIDSorted[MAX_ROOMS];
        GUIDList m_lRoomEventMobGUIDList;
        GUIDList m_lIncarceratorGUIDList;
        GUIDList m_lRookeryEggGUIDList;
        GUIDList m_lEmberseerRunesGUIDList;
};

#endif
