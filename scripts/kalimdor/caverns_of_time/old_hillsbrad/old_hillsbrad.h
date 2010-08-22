/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_OLD_HILLSBRAD_H
#define DEF_OLD_HILLSBRAD_H

enum
{
    MAX_ENCOUNTER                   = 6,

    TYPE_BARREL_DIVERSION           = 1,
    TYPE_THRALL_EVENT               = 2,
    TYPE_THRALL_PART1               = 3,
    TYPE_THRALL_PART2               = 4,
    TYPE_THRALL_PART3               = 5,
    TYPE_THRALL_PART4               = 6,

    NPC_THRALL                      = 17876,
    NPC_TARETHA                     = 18887,
    NPC_DRAKE                       = 17848,
    NPC_LODGE_QUEST_TRIGGER         = 20155,
    NPC_EPOCH                       = 18096,

    QUEST_ENTRY_HILLSBRAD           = 10282,
    QUEST_ENTRY_DIVERSION           = 10283,
    QUEST_ENTRY_ESCAPE              = 10284,
    QUEST_ENTRY_RETURN              = 10285,

    WORLD_STATE_OH                  = 2436,
};

class MANGOS_DLL_DECL instance_old_hillsbrad : public ScriptedInstance
{
    public:
        instance_old_hillsbrad(Map* pMap);
        ~instance_old_hillsbrad() {}

        void Initialize();

        Player* GetPlayerInMap();

        void OnCreatureCreate(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        void UpdateLodgeQuestCredit();

        Creature* GetThrall() { return instance->GetCreature(m_uiThrallGUID); }
        Creature* GetTaretha() { return instance->GetCreature(m_uiTarethaGUID); }
        Creature* GetScarloc() { return instance->GetCreature(m_uiScarlocGUID); }
        Creature* GetEpoch() { return instance->GetCreature(m_uiEpochGUID); }

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];

        uint32 m_uiBarrelCount;
        uint32 m_uiThrallEventCount;
        uint64 m_uiThrallGUID;
        uint64 m_uiTarethaGUID;
        uint64 m_uiScarlocGUID;
        uint64 m_uiEpochGUID;
};

#endif
