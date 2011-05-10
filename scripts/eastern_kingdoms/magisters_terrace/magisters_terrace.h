/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_MAGISTERS_TERRACE_H
#define DEF_MAGISTERS_TERRACE_H

enum
{
    MAX_ENCOUNTER               = 4,

    TYPE_SELIN                  = 0,
    TYPE_VEXALLUS               = 1,
    TYPE_DELRISSA               = 2,
    TYPE_KAELTHAS               = 3,

    TYPE_FEL_CRYSTAL_SIZE       = 4,
    TYPE_DELRISSA_DEATH_COUNT   = 5,

    NPC_SELIN_FIREHEART         = 24723,
    NPC_DELRISSA                = 24560,
    NPC_FEL_CRYSTAL             = 24722,

    GO_VEXALLUS_DOOR            = 187896,
    GO_SELIN_DOOR               = 187979,                   // SunwellRaid Gate 02
    GO_DELRISSA_DOOR            = 187770,
    GO_SELIN_ENCOUNTER_DOOR     = 188065,                   // Assembly Chamber Door

    GO_KAEL_DOOR                = 188064,
    GO_KAEL_STATUE_LEFT         = 188165,
    GO_KAEL_STATUE_RIGHT        = 188166,
};

class MANGOS_DLL_DECL instance_magisters_terrace : public ScriptedInstance
{
    public:
        instance_magisters_terrace(Map* pMap);

        void Initialize();

        // Was used, likely wrong for normal dungeon
        //bool IsEncounterInProgress() const

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        uint32 GetData(uint32 uiType);
        void SetData(uint32 uiType, uint32 uiData);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint32 m_uiDelrissaDeathCount;

        GUIDList FelCrystals;
        GUIDList::iterator CrystalItr;

        uint64 m_uiSelinGUID;
        uint64 m_uiDelrissaGUID;
        uint64 m_uiVexallusDoorGUID;
        uint64 m_uiSelinDoorGUID;
        uint64 m_uiSelinEncounterDoorGUID;
        uint64 m_uiDelrissaDoorGUID;
        uint64 m_uiKaelDoorGUID;
        uint64 m_auiKaelStatue[2];

        bool m_bInitializedItr;
};

#endif
