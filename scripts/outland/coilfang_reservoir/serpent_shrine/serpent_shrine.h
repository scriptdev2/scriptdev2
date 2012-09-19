/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SERPENT_SHRINE_H
#define DEF_SERPENT_SHRINE_H

enum
{
    MAX_ENCOUNTER                   = 6,
    MAX_GENERATOR                   = 4,
    MAX_SPELLBINDERS                = 3,

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

    //NPC_KARATHRESS                = 21214,
    NPC_CARIBDIS                    = 21964,
    NPC_SHARKKIS                    = 21966,
    NPC_TIDALVESS                   = 21965,
    NPC_LEOTHERAS                   = 21215,
    NPC_LADYVASHJ                   = 21212,
    NPC_GREYHEART_SPELLBINDER       = 21806,

    SPELL_LEOTHERAS_BANISH          = 37546,
};

class MANGOS_DLL_DECL instance_serpentshrine_cavern : public ScriptedInstance
{
    public:
        instance_serpentshrine_cavern(Map* pMap);

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        uint32 m_auiShieldGenerator[MAX_GENERATOR];
        std::string m_strInstData;

        uint32 m_uiSpellBinderCount;

        GuidList m_lSpellBindersGUIDList;
};

#endif
