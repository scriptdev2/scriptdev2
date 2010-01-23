/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_VIOLET_H
#define DEF_VIOLET_H

enum
{
    MAX_ENCOUNTER               = 2,

    TYPE_MAIN                   = 1,
    TYPE_SEAL                   = 2,

    WORLD_STATE_ID              = 3816,
    WORLD_STATE_SEAL            = 3815,
    WORLD_STATE_PORTALS         = 3810,

    GO_INTRO_CRYSTAL            = 193615,
    GO_PRISON_SEAL_DOOR         = 191723,

    GO_CELL_LAVANTHOR           = 191566,
    GO_CELL_MORAGG              = 191606,
    GO_CELL_ZURAMAT             = 191565,
    GO_CELL_EREKEM              = 191564,
    GO_CELL_EREKEM_GUARD_L      = 191563,
    GO_CELL_EREKEM_GUARD_R      = 191562,
    GO_CELL_XEVOZZ              = 191556,
    GO_CELL_ICHORON             = 191722,

    NPC_PORTAL_INTRO            = 31011,
    NPC_PORTAL_EVENT            = 30679,                    // two exist, 32174 in addition (elite)
    NPC_DOOR_SEAL               = 30896,

    NPC_SINCLARI                = 30658,                    // entry 32204, who is he? Possibly yeller for seal weakening.
    NPC_HOLD_GUARD              = 30659,

    NPC_EREKEM                  = 29315,
    NPC_EREKEM_GUARD            = 29395,
    NPC_MORAGG                  = 29316,
    NPC_ICHORON                 = 29313,
    NPC_XEVOZZ                  = 29266,
    NPC_LAVANTHOR               = 29312,
    NPC_ZURAMAT                 = 29314,
    NPC_CYANIGOSA               = 31134,

    NPC_PORTAL_GUARDIAN         = 30660,
    NPC_PORTAL_KEEPER           = 30695,

    NPC_AZURE_INVADER           = 30661,
    NPC_AZURE_SPELLBREAKER      = 30662,
    NPC_AZURE_BINDER            = 30663,
    NPC_AZURE_MAGE_SLAYER       = 30664,
    NPC_MAGE_HUNTER             = 30665,
    NPC_AZURE_CAPTAIN           = 30666,
    NPC_AZURE_SORCEROR          = 30667,
    NPC_AZURE_RAIDER            = 30668,

    // used for intro
    NPC_AZURE_BINDER_INTRO      = 31007,
    NPC_AZURE_INVADER_INTRO     = 31008,
    NPC_AZURE_SPELLBREAKER_INTRO= 31009,
    NPC_AZURE_MAGE_SLAYER_INTRO = 31010,

    NPC_AZURE_SABOTEUR          = 31079,

    NPC_DEFENSE_SYSTEM          = 30837,
    NPC_DEFENSE_DUMMY_TARGET    = 30857,

    SPELL_DEFENSE_SYSTEM_VISUAL = 57887,
    SPELL_DEFENSE_SYSTEM_SPAWN  = 57886,

    SPELL_DESTROY_DOOR_SEAL     = 58040,                    // spell periodic cast by misc

    SAY_SEAL_75                 = -1608002,
    SAY_SEAL_50                 = -1608003,
    SAY_SEAL_5                  = -1608004,

    EMOTE_GUARDIAN_PORTAL       = -1608005,
    EMOTE_DRAGONFLIGHT_PORTAL   = -1608006,
    EMOTE_KEEPER_PORTAL         = -1608007,
};

class MANGOS_DLL_DECL instance_violet_hold : public ScriptedInstance
{
    public:
        instance_violet_hold(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetCellForBoss(uint32 uiEntry);
        void SetWorldState(bool bEnable = true);

        void SetData(uint32 uiType, uint32 uiData);

        typedef std::multimap<uint32, uint64> BossToCellMap;

    protected:

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiSinclariGUID;
        uint64 m_uiErekemGUID;
        uint64 m_uiMoraggGUID;
        uint64 m_uiIchoronGUID;
        uint64 m_uiXevozzGUID;
        uint64 m_uiLavanthorGUID;
        uint64 m_uiZuramatGUID;

        uint64 m_uiCellErekemGuard_LGUID;
        uint64 m_uiCellErekemGuard_RGUID;
        uint64 m_uiIntroCrystalGUID;
        uint64 m_uiDoorSealGUID;

        uint32 m_uiWorldState;
        uint32 m_uiWorldStateSealCount;
        uint32 m_uiWorldStatePortalCount;

        BossToCellMap m_mBossToCellMap;
};

#endif
