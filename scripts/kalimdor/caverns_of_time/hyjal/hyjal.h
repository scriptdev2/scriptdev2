/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_HYJAL_H
#define DEF_HYJAL_H

enum
{
    MAX_ENCOUNTER               = 5,

    TYPE_WINTERCHILL            = 0,
    TYPE_ANETHERON              = 1,
    // TYPE_ANETHERON_OLD       = 2,                        // Used in Acid
    TYPE_KAZROGAL               = 3,
    TYPE_AZGALOR                = 4,
    TYPE_ARCHIMONDE             = 5,

    // TYPE_AZGALOR_OLD         = 6,                        // Used in Acid
    TYPE_TRASH_COUNT            = 7,
    TYPE_RETREAT                = 8,

    // ACID scripted, don't touch id's (or provide update for ACID), in Acid scripted SetData types: 11, 2, 9, 6 and 14
    // TYPE_WINTERCHILL_OLD     = 11,
    // DATA_TRASH_OLD           = 14,
    // TYPE_KAZROGAL_OLD        = 9,

    WORLD_STATE_WAVES           = 2842,
    WORLD_STATE_ENEMY           = 2453,
    WORLD_STATE_ENEMYCOUNT      = 2454,

    NPC_JAINA                   = 17772,
    NPC_THRALL                  = 17852,
    NPC_TYRANDE                 = 17948,

    // Bosses summoned after every 8 waves
    NPC_WINTERCHILL             = 17767,
    NPC_ANETHERON               = 17808,
    NPC_KAZROGAL                = 17888,
    NPC_AZGALOR                 = 17842,
    NPC_ARCHIMONDE              = 17968,

    // Trash Mobs summoned in waves
    NPC_NECRO                   = 17899,
    NPC_ABOMI                   = 17898,
    NPC_GHOUL                   = 17895,
    NPC_BANSH                   = 17905,
    NPC_CRYPT                   = 17897,
    NPC_GARGO                   = 17906,
    NPC_FROST                   = 17907,
    NPC_GIANT                   = 17908,
    NPC_STALK                   = 17916,

    NPC_WATER_ELEMENTAL         = 18001,
    NPC_DIRE_WOLF               = 17854,

    GO_ANCIENT_GEM              = 185557,
};

class MANGOS_DLL_DECL instance_mount_hyjal : public ScriptedInstance
{
    public:
        instance_mount_hyjal(Map* pMap);

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strSaveData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strSaveData;

        GUIDList lAncientGemGUIDList;

        uint32 m_uiTrashCount;
};

#endif
