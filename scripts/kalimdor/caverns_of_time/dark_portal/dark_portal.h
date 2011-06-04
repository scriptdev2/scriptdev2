/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_DARKPORTAL_H
#define DEF_DARKPORTAL_H

enum
{
    MAX_ENCOUNTER           = 2,

    TYPE_MEDIVH             = 1,
    TYPE_RIFT               = 2,

    DATA_PORTAL_COUNT       = 11,
    DATA_SHIELD             = 12,

    WORLD_STATE_BM          = 2541,
    WORLD_STATE_BM_SHIELD   = 2540,
    WORLD_STATE_BM_RIFT     = 2784,

    QUEST_OPENING_PORTAL    = 10297,
    QUEST_MASTER_TOUCH      = 9836,

    NPC_TIME_KEEPER         = 17918,
    NPC_RKEEP               = 21104,
    NPC_RLORD               = 17839,
    NPC_DEJA                = 17879,
    NPC_TEMPO               = 17880,
    NPC_AEONUS              = 17881,
    NPC_ASSAS               = 17835,
    NPC_WHELP               = 21818,
    NPC_CHRON               = 17892,
    NPC_EXECU               = 18994,
    NPC_VANQU               = 18995,
    NPC_MEDIVH              = 15608,
    NPC_TIME_RIFT           = 17838,

    SPELL_RIFT_CHANNEL      = 31387,

    RIFT_BOSS               = 1
};

class MANGOS_DLL_DECL instance_dark_portal : public ScriptedInstance
{
    public:
        instance_dark_portal(Map* pMap);

        void Initialize();
        void OnPlayerEnter(Player* pPlayer);
        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        void Update(uint32 uiDiff);

    private:
        Creature* SummonedPortalBoss(Creature* pSource);
        void DoSpawnPortal();
        bool CanProgressEvent();
        uint8 GetRiftWaveId();
        void Clear();
        void InitWorldState(bool bEnable = true);

        uint32 m_auiEncounter[MAX_ENCOUNTER];

        uint32 m_uiRiftPortalCount;
        uint32 m_uiShieldPercent;
        uint8 m_uiRiftWaveCount;
        uint8 m_uiRiftWaveId;

        uint32 m_uiNextPortalTimer;
        uint8 m_uiCurrentRiftId;
};

#endif
