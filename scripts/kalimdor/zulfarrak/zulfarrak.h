/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ZULFARRAK_H
#define DEF_ZULFARRAK_H

enum
{
    MAX_ENCOUNTER                   = 8,

    TYPE_VELRATHA                   = 0,
    TYPE_GAHZRILLA                  = 1,
    TYPE_ANTUSUL                    = 2,
    TYPE_THEKA                      = 3,
    TYPE_ZUMRAH                     = 4,
    TYPE_NEKRUM                     = 5,
    TYPE_SEZZZIZ                    = 6,
    TYPE_CHIEF_SANDSCALP            = 7,

    NPC_VELRATHA                    = 7795,
    NPC_GAHZRILLA                   = 7273,
    NPC_ANTUSUL                     = 8127,
    NPC_THEKA                       = 7272,
    NPC_ZUMRAH                      = 7271,
    NPC_NEKRUM                      = 7796,
    NPC_SEZZZIZ                     = 7275,
    NPC_CHIEF_SANDSCALP             = 7267,

    AREATRIGGER_ANTUSUL             = 1447,
};

class MANGOS_DLL_DECL instance_zulfarrak : public ScriptedInstance
{
    public:
        instance_zulfarrak(Map* pMap);
        ~instance_zulfarrak() {}

        void Initialize();

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiAntuSulGUID;
};

#endif
