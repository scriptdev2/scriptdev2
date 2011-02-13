/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_OBSIDIAN_SANCTUM_H
#define DEF_OBSIDIAN_SANCTUM_H

enum
{
    MAX_ENCOUNTER               = 1,

    TYPE_SARTHARION_EVENT       = 1,
    // internal used types for achievement
    TYPE_ALIVE_DRAGONS          = 2,

    NPC_SARTHARION              = 28860,
    NPC_TENEBRON                = 30452,
    NPC_SHADRON                 = 30451,
    NPC_VESPERON                = 30449,

    GO_TWILIGHT_PORTAL          = 193988,

    // Achievement related
    ACHIEV_CRIT_VOLCANO_BLOW_N  = 7326,                     // achievs 2047, 2048 (Go When the Volcano Blows) -- This is individual achievement!
    ACHIEV_CRIT_VOLCANO_BLOW_H  = 7327,
    ACHIEV_DRAGONS_ALIVE_1_N    = 7328,                     // achievs 2049, 2052 (Twilight Assist)
    ACHIEV_DRAGONS_ALIVE_1_H    = 7331,
    ACHIEV_DRAGONS_ALIVE_2_N    = 7329,                     // achievs 2050, 2053 (Twilight Duo)
    ACHIEV_DRAGONS_ALIVE_2_H    = 7332,
    ACHIEV_DRAGONS_ALIVE_3_N    = 7330,                     // achievs 2051, 2054 (The Twilight Zone)
    ACHIEV_DRAGONS_ALIVE_3_H    = 7333,
};

class MANGOS_DLL_DECL instance_obsidian_sanctum : public ScriptedInstance
{
    public:
        instance_obsidian_sanctum(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        uint64 m_uiSartharionGUID;
        uint64 m_uiTenebronGUID;
        uint64 m_uiShadronGUID;
        uint64 m_uiVesperonGUID;

        uint8 m_uiAliveDragons;
};

#endif
