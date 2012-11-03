/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_RUBY_SANCTUM_H
#define DEF_RUBY_SANCTUM_H

enum
{
    MAX_ENCOUNTER                   = 4,

    TYPE_SAVIANA                    = 0,
    TYPE_BALTHARUS                  = 1,
    TYPE_ZARITHRIAN                 = 2,
    TYPE_HALION                     = 3,

    NPC_HALION_REAL                 = 39863,            // Halion - Physical Realm NPC
    NPC_HALION_TWILIGHT             = 40142,            // Halion - Twilight Realm NPC
    NPC_HALION_CONTROLLER           = 40146,

    // NPC_SHADOW_PULSAR_N           = 40083,            // Halion event - Shadow orb
    // NPC_SHADOW_PULSAR_S           = 40100,
    // NPC_ORB_CARRIER               = 40081,
    // NPC_ORB_ROTATION_FOCUS        = 40091,

    NPC_SAVIANA                     = 39747,            // minibosses
    NPC_BALTHARUS                   = 39751,
    NPC_ZARITHRIAN                  = 39746,

    NPC_XERESTRASZA                 = 40429,            // friendly npc, used for some cinematic and quest
    NPC_ZARITHRIAN_SPAWN_STALKER    = 39794,

    // GO_HALION_PORTAL_1            = 202794,           // Portals used in the Halion encounter
    // GO_HALION_PORTAL_2            = 202795,
    // GO_HALION_PORTAL_3            = 202796,

    GO_FIRE_FIELD                   = 203005,           // Xerestrasza flame door
    GO_FLAME_WALLS                  = 203006,           // Zarithrian flame walls
    GO_FLAME_RING                   = 203007,           // Halion flame ring

    GO_BURNING_TREE_1               = 203036,           // Trees which burn when Halion appears
    GO_BURNING_TREE_2               = 203037,
    GO_BURNING_TREE_3               = 203035,
    GO_BURNING_TREE_4               = 203034,

    // Spells used to summon Halion
    SPELL_FIRE_PILLAR               = 76006,
    SPELL_FIERY_EXPLOSION           = 76010,

    SAY_HALION_SPAWN                = -1724024,

    // world state to show corporeality in Halion encounter - phase 3
    UPDATE_STATE_UI_SHOW            = 5049,
    UPDATE_STATE_UI_COUNT           = 5050,
};

class MANGOS_DLL_DECL instance_ruby_sanctum : public ScriptedInstance
{
    public:
        instance_ruby_sanctum(Map* pMap);

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnPlayerEnter(Player* pPlayer);
        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        void Update(uint32 uiDiff);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        void DoHandleZarithrianDoor();

        std::string strInstData;
        uint32 m_auiEncounter[MAX_ENCOUNTER];

        uint32 m_uiHalionSummonTimer;
        uint32 m_uiHalionSummonStage;
};

#endif
