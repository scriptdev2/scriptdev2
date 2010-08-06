/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ULDAMAN_H
#define DEF_ULDAMAN_H

enum
{
    MAX_ENCOUNTER               = 2,

    TYPE_ALTAR_EVENT            = 1,
    TYPE_ARCHAEDAS              = 2,
    DATA_EVENT_STARTER          = 3,

    GO_TEMPLE_DOOR_UPPER        = 124367,
    GO_TEMPLE_DOOR_LOWER        = 141869,
    GO_ANCIENT_VAULT            = 124369,

    MOB_CUSTODIAN               = 7309,
    MOB_GUARDIAN                = 7076,
    MOB_HALLSHAPER              = 7077,
    MOB_VAULT_WARDER            = 10120,
    NPC_STONE_KEEPER            = 4857,

    SPELL_STONED                = 10255,
    SPELL_USE_ALTAR_VISUAL      = 11206,

    FACTION_TITAN_HOSTILE       = 415,
    FACTION_TITAN_NEUTRAL       = 416,

    EVENT_ID_ALTAR_KEEPER       = 2228,                     // spell 11568
    EVENT_ID_ALTAR_ARCHAEDAS    = 2268                      // spell 10340
};

class MANGOS_DLL_DECL instance_uldaman : public ScriptedInstance
{
    public:
        instance_uldaman(Map* pMap);
        ~instance_uldaman() {}

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        void Update(uint32 uiDiff);

        void SetData(uint32 uiType, uint32 uiData);
        void SetData64(uint32 uiData, uint64 uiGuid);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        void StartEvent(uint32 uiEventId, Player* pPlayer);

        Creature* GetDwarf();
        // hack to remove
        void SimulateSpellHit(uint32 uiCreatureEntry, uint32 uiSpellEntry, Unit* pCaster);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiTempleDoorUpperGUID;
        uint64 m_uiTempleDoorLowerGUID;
        uint64 m_uiAncientVaultGUID;
        uint64 m_uiPlayerGUID;
        uint8 m_uiStoneKeepersFallen;

        std::list<uint64> m_lWardens;
        std::list<uint64> m_lKeeperList;
};
#endif
