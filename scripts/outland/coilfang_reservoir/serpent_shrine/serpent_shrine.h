/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SERPENT_SHRINE_H
#define DEF_SERPENT_SHRINE_H

enum
{
    MAX_ENCOUNTER                   = 6,
    MAX_GENERATOR                   = 4,

    TYPE_HYDROSS_EVENT              = 1,
    TYPE_KARATHRESS_EVENT           = 2,
    TYPE_LADYVASHJ_EVENT            = 3,
    TYPE_LEOTHERAS_EVENT            = 4,
    TYPE_MOROGRIM_EVENT             = 5,
    TYPE_THELURKER_EVENT            = 6,
    TYPE_VASHJ_PHASE3_CHECK         = 7,

    TYPE_SHIELDGENERATOR1           = 8,
    TYPE_SHIELDGENERATOR2           = TYPE_SHIELDGENERATOR1 + 1,
    TYPE_SHIELDGENERATOR3           = TYPE_SHIELDGENERATOR1 + 2,
    TYPE_SHIELDGENERATOR4           = TYPE_SHIELDGENERATOR1 + 3,

    DATA_KARATHRESS_STARTER         = 12,                   // Player who started the Karathress encounter

    NPC_KARATHRESS                  = 21214,
    NPC_CARIBDIS                    = 21964,
    NPC_SHARKKIS                    = 21966,
    NPC_TIDALVESS                   = 21965,
    NPC_LADYVASHJ                   = 21212,
};

class MANGOS_DLL_DECL instance_serpentshrine_cavern : public ScriptedInstance
{
    public:
        instance_serpentshrine_cavern(Map* pMap);

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        void SetData64(uint32 uiType, uint64 uiData);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        uint32 m_auiShieldGenerator[MAX_GENERATOR];
        std::string m_strInstData;

        ObjectGuid m_karathressEventStarterGuid;
};

#endif
