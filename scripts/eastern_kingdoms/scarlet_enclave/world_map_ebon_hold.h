/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_EBON_HOLD_H
#define DEF_EBON_HOLD_H

enum
{
    TYPE_BATTLE                         = 0,

    // npcs
    // death knights
    NPC_HIGHLORD_DARION_MOGRAINE        = 29173,
    NPC_KOLTIRA_DEATHWEAVER             = 29199,
    NPC_ORBAZ_BLOODBANE                 = 29204,
    NPC_THASSARIAN                      = 29200,

    // scourge warriors - summond during the event
    NPC_FLESH_BEHEMOTH                  = 29190,
    NPC_RAMPAGING_ABOMINATION           = 29186,
    NPC_VOLATILE_GHOUL                  = 29219,
    NPC_WARRIOR_OF_THE_FROZEN_WASTES    = 29206,

    // argent dawn commanders
    NPC_HIGHLORD_TIRION_FORDRING        = 29175,
    NPC_KORFAX_CHAMPION_OF_THE_LIGHT    = 29176,
    NPC_LORD_MAXWELL_TYROSUS            = 29178,
    NPC_COMMANDER_ELIGOR_DAWNBRINGER    = 29177,
    NPC_LEONID_BARTHALOMEW_THE_REVERED  = 29179,
    NPC_DUKE_NICHOLAS_ZVERENHOFF        = 29180,
    NPC_RIMBLAT_EARTHSHATTER            = 29182,
    NPC_RAYNE                           = 29181,

    // argent warriors
    NPC_DEFENDER_OF_THE_LIGHT           = 29174,

    // cinematic
    NPC_THE_LICH_KING                   = 29183,
    NPC_HIGHLORD_ALEXANDROS_MOGRAINE    = 29227,
    NPC_DARION_MOGRAINE                 = 29228,

    // object
    GO_LIGHT_OF_DAWN                    = 191330,

    // spells
    SPELL_THE_LIGHT_OF_DAWN_DAMAGE      = 53645,            // kill each scourge
    SPELL_CHAPTER_IV                    = 53405,            // phase aura

    // variables
    MAX_LIGHT_CHAMPIONS                 = 7,                // the number of the light champions
    MAX_WARRIORS_SUMMONED_PER_TURN      = 5,                // summoned warriors (light and death) per turn
    MAX_LIGHT_GUARDS                    = 4,                // guards summond for the outro

    // event variables
    MAX_BATTLE_INTRO_TIMER              = 5,
    MAX_FORCES_LIGHT                    = 300,
    MAX_FORCES_SCOURGE                  = 10000,

    // world states
    // basically world states should be shown to all players with phase mask = 128 as stated in DBC
    // because we don't have the possibility to do that we'll just iterate through the players and set the phase mask manually based on the battle status
    WORLD_STATE_FORCES_SHOW             = 3592,             // show the remaining units
    WORLD_STATE_FORCES_SCOURGE          = 3591,
    WORLD_STATE_FORCES_LIGHT            = 3590,
    WORLD_STATE_BATTLE_TIMER_SHOW       = 3603,             // countdown timer
    WORLD_STATE_BATTLE_TIMER_TIME       = 3604,
    WORLD_STATE_BATTLE_BEGIN            = 3605,             // battle has begun
};

struct sSpawnLocation
{
    float m_fX, m_fY, m_fZ, m_fO;
    uint32 m_uiEntry;
};

// light champions
static sSpawnLocation aLightArmySpawnLoc[MAX_LIGHT_CHAMPIONS] =
{
    {2285.80f, -5308.82f, 87.04f, 1.67f, NPC_KORFAX_CHAMPION_OF_THE_LIGHT},
    {2276.96f, -5309.36f, 86.66f, 1.61f, NPC_LORD_MAXWELL_TYROSUS},
    {2279.82f, -5322.61f, 88.95f, 1.54f, NPC_LEONID_BARTHALOMEW_THE_REVERED},
    {2287.96f, -5313.96f, 88.27f, 1.63f, NPC_DUKE_NICHOLAS_ZVERENHOFF},
    {2276.84f, -5313.78f, 87.62f, 1.61f, NPC_COMMANDER_ELIGOR_DAWNBRINGER},
    {2275.80f, -5322.51f, 88.62f, 1.68f, NPC_RAYNE},
    {2282.47f, -5319.84f, 88.83f, 1.74f, NPC_RIMBLAT_EARTHSHATTER}
};

// four guards spawned for the outro
static sSpawnLocation aGuardsSpawnLoc[MAX_LIGHT_GUARDS] =
{
    {2287.581f, -5284.991f, 82.535f, 2.60f},
    {2287.856f, -5281.127f, 82.225f, 3.44f},
    {2275.964f, -5282.389f, 82.301f, 5.80f},
    {2275.471f, -5277.668f, 82.058f, 5.79f}
};

// Tirion is spawned at the edge of the battle and runs toward the chapel
// When he reach the chapel he cast some powerfull light spell and the battle ends
static sSpawnLocation aEventLocations[] =
{
    {2181.36f, -5234.03f, 83.15f, 0.0f},            // 0 Tirion spawn location
    {2281.39f, -5299.98f, 85.07f, 1.61f},           // 1 Tirion move location
    {2289.259f, -5280.350f, 82.11f, 0.0f},          // 2 Koltira chapel loc
    {2273.289f, -5273.675f, 81.70f, 0.0f},          // 3 Thassarian chapel loc
    {2281.198f, -5257.397f, 80.22f, 4.66f},         // 4 Alexandros summon location
    {2281.156f, -5259.934f, 80.64f, 0.0f},          // 5 Alexandros move loc
    {2281.294f, -5281.895f, 82.44f, 1.35f},         // 6 Young Darion spawn
    {2281.093f, -5263.013f, 81.12f, 0.0f},          // 7 Young Darion move
    {2281.313f, -5250.282f, 79.32f, 4.69f},         // 8 Lich King spawn
    {2281.523f, -5261.058f, 80.87f, 0.0f},          // 9 Lich King move
    {2259.963f, -5299.262f, 82.16f, 0.99f},         // 10 Darion relocation on Lich King kick
    {2261.234f, -5296.016f, 82.16f, 0.0f},          // 11 Tirion goes to Darion
    {2273.071f, -5293.428f, 83.06f, 0.0f},          // 12 Tirion final point
    {2282.579f, -5270.912f, 81.83f, 1.28f},         // 13 workaround - Lich King attack
    {2307.055f, -5292.831f, 81.99f, 0.0f},          // 14 workaround - Lich King kick
    {2252.441f, -5311.426f, 82.16f, 0.0f},          // 15 workaround - Lich King kick
};

class MANGOS_DLL_DECL world_map_ebon_hold : public ScriptedInstance
{
    public:
        world_map_ebon_hold(Map* pMap);

        void Initialize() override;

        void OnCreatureCreate(Creature* pCreature) override;
        void OnCreatureDeath(Creature* pCreature) override;
        void OnCreatureEvade(Creature* pCreature) override;
        void OnObjectCreate(GameObject* pGo) override;

        void SetData(uint32 uiType, uint32 uiData) override;
        uint32 GetData(uint32 uiType) override;

        void Update(uint32 uiDiff) override;

        bool CanAndToggleGothikYell();

        void DoUpdateBattleWorldState(uint32 uiStateId, uint32 uiStateData);

        // Move the behemots and abominations and make them attack
        void DoMoveArmy();
        void DoDespawnArmy();

    protected:
        void DoResetBattle();

        uint32 m_uiGothikYellTimer;                         // Timer to check if Gothik can yell (related q 12698)
        uint32 m_uiBattleEncounter;                         // Store state of the battle around  "The Light of Dawn"

        GuidList m_lArmyGuids;
};

#endif
