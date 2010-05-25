/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_WAILING_CAVERNS_H
#define DEF_WAILING_CAVERNS_H

enum
{
    MAX_ENCOUNTER  = 6,

    TYPE_ANACONDRA = 0,
    TYPE_COBRAHN   = 1,
    TYPE_PYTHAS    = 2,
    TYPE_SERPENTIS = 3,
    TYPE_DISCIPLE  = 4,
    TYPE_MUTANOUS  = 5,

    DATA_NARALEX   = 6,

    NPC_NARLEX     = 3679
};

class MANGOS_DLL_DECL instance_wailing_caverns : public ScriptedInstance
{
    public:
        instance_wailing_caverns(Map* pMap);
        ~instance_wailing_caverns() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiNaralexGUID;
};
#endif
