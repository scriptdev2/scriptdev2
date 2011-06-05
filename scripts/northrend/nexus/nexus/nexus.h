/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_NEXUS_H
#define DEF_NEXUS_H

enum
{
    MAX_ENCOUNTER                  = 4,

    TYPE_TELESTRA                  = 0,
    TYPE_ANOMALUS                  = 1,
    TYPE_ORMOROK                   = 2,
    TYPE_KERISTRASZA               = 3,

    NPC_TELESTRA                   = 26731,
    NPC_ANOMALUS                   = 26763,
    NPC_ORMOROK                    = 26794,
    NPC_KERISTRASZA                = 26723,

    GO_CONTAINMENT_SPHERE_TELESTRA = 188526,
    GO_CONTAINMENT_SPHERE_ANOMALUS = 188527,
    GO_CONTAINMENT_SPHERE_ORMOROK  = 188528,

    SPELL_FROZEN_PRISON             = 47854,                // may not be correct spell
};

class MANGOS_DLL_DECL instance_nexus : public ScriptedInstance
{
    public:
        instance_nexus(Map* pMap);

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        uint32 GetData(uint32 uiType);
        void SetData(uint32 uiType, uint32 uiData);

        const char* Save() { return m_strInstData.c_str(); }

        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;
};

#endif
