/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ONYXIA_H
#define DEF_ONYXIA_H

enum
{
    TYPE_ONYXIA                 = 0,

    // Special data fields for Onyxia
    DATA_LIFTOFF                = 4,
    DATA_PLAYER_TOASTED         = 5,

    NPC_ONYXIA_WHELP            = 11262,
    NPC_ONYXIA_TRIGGER          = 12758,

    // Achievement Related
    TIME_LIMIT_MANY_WHELPS      = 10,                       // 10s timeframe to kill 50 whelps after liftoff
    ACHIEV_CRIT_REQ_MANY_WHELPS = 50,

    ACHIEV_CRIT_MANY_WHELPS_N   = 12565,                    // Achievements 4403, 4406
    ACHIEV_CRIT_MANY_WHELPS_H   = 12568,
    ACHIEV_CRIT_NO_BREATH_N     = 12566,                    // Acheivements 4404, 4407
    ACHIEV_CRIT_NO_BREATH_H     = 12569,
};

class MANGOS_DLL_DECL instance_onyxias_lair : public ScriptedInstance
{
    public:
        instance_onyxias_lair(Map* pMap);
        ~instance_onyxias_lair() {}

        void Initialize();

        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);

        uint64 GetOnyxiaTriggerGUID() { return m_uiOnyxTriggerGUID; }

    protected:
        uint32 m_uiEncounter;
        uint32 m_uiAchievWhelpsCount;

        time_t m_tPhaseTwoStart;

        uint64 m_uiOnyxTriggerGUID;
};

#endif
