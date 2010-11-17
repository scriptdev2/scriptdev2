/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_STRATHOLME_H
#define DEF_STRATHOLME_H

enum
{
    MAX_ENCOUNTER               = 6,
    MAX_SILVERHAND              = 5,
    MAX_ZIGGURATS               = 3,

    TYPE_BARON_RUN              = 1,
    TYPE_BARONESS               = 2,
    TYPE_NERUB                  = 3,
    TYPE_PALLID                 = 4,
    TYPE_RAMSTEIN               = 5,
    TYPE_BARON                  = 6,

    //DATA_BARON                  = 10,                     // Wasn't used, verify that it really was never used!
    //DATA_YSIDA_TRIGGER          = 11,

    TYPE_SH_QUEST               = 20,
    TYPE_SH_CATHELA             = 21,
    TYPE_SH_GREGOR              = 22,
    TYPE_SH_NEMAS               = 23,
    TYPE_SH_VICAR               = 24,
    TYPE_SH_AELMAR              = 25,

    NPC_CRYSTAL                 = 10415,                    //three ziggurat crystals
    NPC_BARON                   = 10440,
    NPC_YSIDA_TRIGGER           = 16100,
    NPC_THUZADIN_ACOLYTE        = 10399,                    //acolytes in ziggurats
    NPC_RAMSTEIN                = 10439,
    NPC_ABOM_BILE               = 10416,
    NPC_ABOM_VENOM              = 10417,
    NPC_BLACK_GUARD             = 10394,
    NPC_YSIDA                   = 16031,

    GO_SERVICE_ENTRANCE         = 175368,
    GO_GAUNTLET_GATE1           = 175357,
    GO_ZIGGURAT_DOOR_1          = 175380,                   //baroness
    GO_ZIGGURAT_DOOR_2          = 175379,                   //nerub'enkan
    GO_ZIGGURAT_DOOR_3          = 175381,                   //maleki
    GO_ZIGGURAT_DOOR_4          = 175405,                   //rammstein
    GO_ZIGGURAT_DOOR_5          = 175796,                   //baron
    GO_PORT_GAUNTLET            = 175374,                   //port from gauntlet to slaugther
    GO_PORT_SLAUGTHER           = 175373,                   //port at slaugther
    GO_PORT_ELDERS              = 175377,                   //port at elders square

    QUEST_DEAD_MAN_PLEA         = 8945,
    SPELL_BARON_ULTIMATUM       = 27861,

    SAY_ANNOUNCE_ZIGGURAT_1     = -1329004,
    SAY_ANNOUNCE_ZIGGURAT_2     = -1329005,
    SAY_ANNOUNCE_ZIGGURAT_3     = -1329006,
    SAY_ANNOUNCE_RIVENDARE      = -1329007
};

struct MANGOS_DLL_DECL instance_stratholme : public ScriptedInstance
{
    public:
        instance_stratholme(Map* pMap);
        ~instance_stratholme() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

        void OnCreatureDeath(Creature* pCreature);

        void Update(uint32 uiDiff);

        bool StartSlaugtherSquare();
        void DoSortZiggurats();

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        bool m_bIsSilverHandDead[MAX_SILVERHAND];
        std::string strInstData;

        uint32 m_uiBaronRunTimer;
        uint32 m_uiSlaugtherSquareTimer;

        uint64 m_uiServiceEntranceGUID;
        uint64 m_uiGauntletGate1GUID;
        uint64 m_auiZigguratGUID[MAX_ZIGGURATS];
        uint64 m_auiRamsteinDoorGUID;
        uint64 m_auiRivendareDoorGUID;
        uint64 m_uiPortGauntletGUID;
        uint64 m_uiPortSlaugtherGUID;
        uint64 m_uiPortElderGUID;

        uint64 m_uiBaronGUID;
        uint64 m_uiYsidaTriggerGUID;
        uint64 m_uiAcolyteAnnouncerGUID;
        uint64 m_auiCrystalSortedGUID[MAX_ZIGGURATS];

        std::list<uint64> m_luiCrystalGUIDs;
        std::set<uint64> m_sAbomnationGUID;
        std::list<uint64> m_luiAcolyteGUIDs;
        std::list<uint64> m_alZigguratAcolyteGUID[MAX_ZIGGURATS];
};

#endif
