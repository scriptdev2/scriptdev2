/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ULDUAR_H
#define DEF_ULDUAR_H

enum
{
    MAX_ENCOUNTER               = 14,
    HARD_MODE_ENCOUNTER         = 7,
    KEEPER_ENCOUNTER            = 4,

    // Main boss types
    TYPE_LEVIATHAN              = 0,
    TYPE_IGNIS                  = 1,
    TYPE_RAZORSCALE             = 2,
    TYPE_XT002                  = 3,
    TYPE_ASSEMBLY               = 4,
    TYPE_KOLOGARN               = 5,
    TYPE_AURIAYA                = 6,
    TYPE_MIMIRON                = 7,
    TYPE_HODIR                  = 8,
    TYPE_THORIM                 = 9,
    TYPE_FREYA                  = 10,
    TYPE_VEZAX                  = 11,
    TYPE_YOGGSARON              = 12,
    TYPE_ALGALON                = 13,

    // Hard mode boss types
    // Used for hard mode bosses only
    TYPE_LEVIATHAN_HARD         = 14,
    TYPE_XT002_HARD             = 15,
    TYPE_MIMIRON_HARD           = 17,
    TYPE_HODIR_HARD             = 18,
    TYPE_THORIM_HARD            = 19,
    TYPE_VEZAX_HARD             = 21,
    TYPE_YOGGSARON_HARD         = 22,

    // Keeper types
    // Used to store the keepers which will be used at yogg
    TYPE_KEEPER_HODIR           = 23,
    TYPE_KEEPER_FREYA           = 24,
    TYPE_KEEPER_THORIM          = 25,
    TYPE_KEEPER_MIMIRON         = 26,

    // Tower types
    // Used to store the towers which will be used at Leviathan encounter
    TYPE_TOWER_HODIR            = 27,
    TYPE_TOWER_FREYA            = 28,
    TYPE_TOWER_THORIM           = 29,
    TYPE_TOWER_MIMIRON          = 30,

    // Other types - not saved
    TYPE_LEVIATHAN_GAUNTLET     = 31,

    // The siege of ulduar
    NPC_LEVIATHAN               = 33113,
    // NPC_IGNIS                = 33118,
    NPC_RAZORSCALE              = 33186,
    NPC_XT002                   = 33293,
    NPC_HEART_DECONSTRUCTOR     = 33329,
    NPC_XT_TOY_PILE             = 33337,                    // robot spawner npc for XT002

    // Leviathan other npcs
    NPC_ULDUAR_COLOSSUS         = 33237,
    NPC_BRONZEBEARD_RADIO       = 34054,
    NPC_EXPLORER_DELLORAH       = 33701,
    NPC_BRANN_BRONZEBEARD       = 33579,
    NPC_ORBITAL_SUPPORT         = 34286,

    // Leviathan reinforcements
    // NPC_HIRED_ENGINEER       = 33626,
    // NPC_HIRED_DEMOLITIONIST  = 33627,
    // NPC_BATTLE_MAGE          = 33662,
    NPC_SALVAGED_SIEGE_ENGINE   = 33060,
    NPC_SALVAGED_CHOPPER        = 33062,
    NPC_SALVAGED_DEMOLISHER     = 33109,
    // NPC_LIQUID_PYRITE        = 33189,

    // Razorscale helper npcs
    NPC_EXPEDITION_COMMANDER    = 33210,
    NPC_EXPEDITION_ENGINEER     = 33287,                    // npc used to repair the Harpoons
    NPC_EXPEDITION_TRAPPER      = 33259,                    // npc used in Razorscale grounding phase
    NPC_EXPEDITION_DEFENDER     = 33816,                    // used to fight Razorscale
    NPC_RAZORSCALE_CONTROLLER   = 33233,                    // harpoon shot trigger npc for phase

    // The antechamber of ulduar
    NPC_STEELBREAKER            = 32867,
    NPC_MOLGEIM                 = 32927,
    NPC_BRUNDIR                 = 32857,
    NPC_KOLOGARN                = 32930,
    NPC_RIGHT_ARM               = 32934,
    NPC_LEFT_ARM                = 32933,
    NPC_AURIAYA                 = 33515,
    NPC_SANCTUM_SENTRY          = 34014,
    NPC_FERAL_DEFENDER          = 34035,

    // The keepers of ulduar
    NPC_MIMIRON                 = 33350,
    NPC_LEVIATHAN_MK            = 33432,
    NPC_VX001                   = 33651,
    NPC_AERIAL_UNIT             = 33670,
    NPC_HODIR                   = 32845,
    NPC_THORIM                  = 32865,
    NPC_RUNIC_COLOSSUS          = 32872,
    NPC_RUNE_GIANT              = 32873,
    NPC_JORMUNGAR_BEHEMOTH      = 32882,
    NPC_FREYA                   = 32906,
    NPC_ELDER_BRIGHTLEAF        = 32915,
    NPC_ELDER_IRONBRACH         = 32913,
    NPC_ELDER_STONEBARK         = 32914,

    // The descent into madness
    NPC_VEZAX                   = 33271,
    NPC_SARONITE_ANIMUS         = 33524,
    NPC_YOGGSARON               = 33288,
    NPC_SARA                    = 33134,
    NPC_YOGG_BRAIN              = 33890,

    // Celestial planetarium
    NPC_ALGALON                 = 32871,

    // Keepers images
    // They spawn in the central room after they are released from Yogg's enslavement
    // You may talk to them and ask them to help you fight Yogg-Saron
    NPC_THORIM_IMAGE            = 33413,
    NPC_MIMIRON_IMAGE           = 33412,
    NPC_HODIR_IMAGE             = 33411,
    NPC_FREYA_IMAGE             = 33410,

    // Keepers used to fight Yogg-Saron
    NPC_KEEPER_FREYA            = 33241,
    NPC_KEEPER_HODIR            = 33213,
    NPC_KEEPER_MIMIRON          = 33244,
    NPC_KEEPER_THORIM           = 33242,

    MAX_SPECIAL_ACHIEV_CRITS    = 9,

    TYPE_ACHIEV_CAT_LADY        = 0,
    TYPE_ACHIEV_NINE_LIVES      = 1,
    TYPE_ACHIEV_STEELBREAKER    = 2,
    TYPE_ACHIEV_BRUNDIR         = 3,
    TYPE_ACHIEV_MOLGEIM         = 4,
    TYPE_ACHIEV_STUNNED         = 5,
    TYPE_ACHIEV_SHATTERED       = 6,
    TYPE_ACHIEV_QUICK_SHAVE     = 7,
    TYPE_ACHIEV_SHUTOUT         = 8,

    // Loot chests
    // Kologarn
    GO_CACHE_OF_LIVING_STONE_10 = 195046,
    GO_CACHE_OF_LIVING_STONE_25 = 195047,

    // Hodir
    GO_CACHE_OF_WINTER_10       = 194307,
    GO_CACHE_OF_WINTER_25       = 194308,
    GO_CACHE_OF_RARE_WINTER_10  = 194200,
    GO_CACHE_OF_RARE_WINTER_25  = 194201,

    // Mimiron
    GO_CACHE_OF_INOV_10         = 194789,
    GO_CACHE_OF_INOV_25         = 194956,
    GO_CACHE_OF_INOV_10_H       = 194957,
    GO_CACHE_OF_INOV_25_H       = 194958,

    // Thorim
    GO_CACHE_OF_STORMS_10       = 194312,
    GO_CACHE_OF_STORMS_25       = 194315,
    GO_CACHE_OF_STORMS_10_H     = 194313,
    GO_CACHE_OF_STORMS_25_H     = 194314,

    // Alagon
    GO_GIFT_OF_OBSERVER_10      = 194821,
    GO_GIFT_OF_OBSERVER_25      = 194822,

    // Doors and other Objects
    // The siege
    GO_SHIELD_WALL              = 194416,                   // Gate before Leviathan
    GO_LIGHTNING_DOOR           = 194905,                   // Lightning gate after the Leviathan. It closes after the boss enters the arena
    GO_LEVIATHAN_GATE           = 194630,                   // Gate after Leviathan -> this will be broken when the boss enters the arena
    GO_XT002_GATE               = 194631,                   // Gate before Xt002
    GO_BROKEN_HARPOON           = 194565,                   // Broken harpoon from Razorscale
    GO_HARPOON_GUN_1            = 194542,                   // usable harpoonS - respawn when the broken one is repaired
    GO_HARPOON_GUN_2            = 194541,
    GO_HARPOON_GUN_3            = 194543,
    GO_HARPOON_GUN_4            = 194519,
    GO_FREYA_CRYSTAL            = 194704,                   // crystals which can be active during Leviathan encounter if hard mode towers are active
    GO_MIMIRON_CRYSTAL          = 194705,
    GO_THORIM_CRYSTAL           = 194706,
    GO_HODIR_CRYSTAL            = 194707,

    // Antechamber
    GO_KOLOGARN_BRIDGE          = 194232,
    GO_SHATTERED_DOOR           = 194553,                   // Door before kologarn
    GO_IRON_ENTRANCE_DOOR       = 194554,                   // Door before iron council
    GO_ARCHIVUM_DOOR            = 194556,                   // Entrance door to the archivum
    GO_ARCHIVUM_CONSOLE         = 194555,                   // Used at some sort of cinematic
    GO_UNIVERSE_FLOOR_ARCHIVUM  = 194715,                   // Used for animation

    // Planetarium
    GO_CELESTIAL_ACCES          = 194628,                   // Acces console for 10 man mode
    GO_CELESTIAL_ACCES_H        = 194752,                   // Acces console for 25 man mode
    GO_CELESTIAL_DOOR           = 194767,                   // Entrance door to the planetarium
    GO_UNIVERSE_FLOOR_CELESTIAL = 194716,                   // For animation
    GO_AZEROTH_GLOBE            = 194148,                   // For animation

    // The keepers
    // Hodir
    GO_HODIR_EXIT               = 194634,
    GO_HODIR_ICE_WALL           = 194441,
    GO_HODIR_ENTER              = 194442,
    // Mimiron
    G0_MIMIRON_BUTTON           = 194739,                   // Used to start hard mode
    GO_MIMIRON_DOOR_1           = 194774,
    GO_MIMIRON_DOOR_2           = 194775,
    GO_MIMIRON_DOOR_3           = 194776,
    GO_MIMIRON_TEL1             = 194741,                   // Used to summon mobs in phase 3
    GO_MIMIRON_TEL2             = 194742,
    GO_MIMIRON_TEL3             = 194743,
    GO_MIMIRON_TEL4             = 194744,
    GO_MIMIRON_TEL5             = 194740,
    GO_MIMIRON_TEL6             = 194746,
    GO_MIMIRON_TEL7             = 194747,
    GO_MIMIRON_TEL8             = 194748,
    GO_MIMIRON_TEL9             = 194745,
    GO_MIMIRON_ELEVATOR         = 194749,                   // Central elevator
    // Thorim
    GO_DARK_IRON_PORTCULIS      = 194560,                   // Door from the arena to the hallway
    GO_RUNED_STONE_DOOR         = 194557,                   // Door after the runic colossus
    GO_THORIM_STONE_DOOR        = 194558,                   // Door after the ancient rune giant
    GO_LIGHTNING_FIELD          = 194559,                   // Arena exit door
    GO_DOOR_LEVER               = 194264,                   // In front of the door

    // Descent to madness
    GO_ANCIENT_GATE             = 194255,                   // Door upstairs before vezax, opens when all keepers are freed
    GO_VEZAX_GATE               = 194750,                   // Door after vezax
    GO_YOGG_GATE                = 194773,                   // Yogg-Saron chamber door
    GO_BRAIN_DOOR1              = 194635,                   // Brain chamber doors
    GO_BRAIN_DOOR2              = 194636,
    GO_BRAIN_DOOR3              = 194637,

    // World state used for algalon timer
    WORLD_STATE_TIMER           = 4132,
    WORLD_STATE_TIMER_COUNT     = 4131,

    // events
    EVENT_ID_SHUTDOWN           = 21605,
    EVENT_ID_SPELL_SHATTER      = 21620,
    EVENT_ID_TOWER_LIFE         = 21030,                    // events checked when a tower is destroyed
    EVENT_ID_TOWER_FLAME        = 21033,
    EVENT_ID_TOWER_FROST        = 21032,
    EVENT_ID_TOWER_STORMS       = 21031,

    // area triggers
    AREATRIGGER_ID_INTRO        = 5388,                     // starts the intro dialogue
    AREATRIGGER_ID_REPAIR_1     = 5369,                     // related to vehicle repair
    AREATRIGGER_ID_REPAIR_2     = 5423,

    // Achievement related
    ACHIEV_START_IGNIS_ID       = 20951,                    // Ignis timed achievs 2930, 2929
    ACHIEV_START_XT002_ID       = 21027,                    // XT-002 timed achievs 2937, 2938

    ACHIEV_CRIT_SARONITE_N      = 10451,                    // General Vezax, achievs 3181, 3188
    ACHIEV_CRIT_SARONITE_H      = 10462,
    ACHIEV_CRIT_CAT_LADY_N      = 10400,                    // Auriaya, achievs 3006, 3007
    ACHIEV_CRIT_CAT_LADY_H      = 10184,
    ACHIEV_CRIT_NINE_LIVES_N    = 10399,                    // Auriaya, achievs 3076, 3077
    ACHIEV_CRIT_NINE_LIVES_H    = 10243,
    ACHIEV_CRIT_STEELBREAKER_N  = 10084,                    // Iron council, achievs 2941, 2944
    ACHIEV_CRIT_STEELBREAKER_H  = 10087,
    ACHIEV_CRIT_MOLGEIM_N       = 10082,                    // Iron council, achievs 2939, 2942
    ACHIEV_CRIT_MOLGEIM_H       = 10085,
    ACHIEV_CRIT_BRUNDIR_N       = 10083,                    // Iron council, achievs 2940, 2943
    ACHIEV_CRIT_BRUNDIR_H       = 10086,
    ACHIEV_CRIT_STUNNED_BRUND_N = 10090,                    // Iron council, achiev 2947
    ACHIEV_CRIT_STUNNED_STEEL_N = 10422,
    ACHIEV_CRIT_STUNNED_MOLG_N  = 10423,
    ACHIEV_CRIT_STUNNED_BRUND_H = 10091,                    // Iron council, achiev 2948
    ACHIEV_CRIT_STUNNED_STEEL_H = 10424,
    ACHIEV_CRIT_STUNNED_MOLG_H  = 10425,
    ACHIEV_CRIT_SHATTERED_N     = 10068,                    // Ignis, achievs 2925, 2926
    ACHIEV_CRIT_SHATTERED_H     = 10069,
    ACHIEV_CRIT_HEARTBREAKER_N  = 10221,                    // XT-002, achievs 3058, 3059
    ACHIEV_CRIT_HEARTBREAKER_H  = 10220,
    ACHIEV_CRIT_NERF_ENG_N      = 10074,                    // XT-002, achievs 2931, 2932
    ACHIEV_CRIT_NERF_ENG_H      = 10075,
    ACHIEV_CRIT_NERF_GRAVITY_N  = 10077,                    // XT-002, achievs 2934, 2936
    ACHIEV_CRIT_NERF_GRAVITY_H  = 10079,
    ACHIEV_CRIT_QUICK_SHAVE_N   = 10062,                    // Razorscale, achievs 2919, 2921
    ACHIEV_CRIT_QUICK_SHAVE_H   = 10063,
    ACHIEV_CRIT_ORB_BOMB_N      = 10056,                    // Flame Leviathan, achievs 2913, 2918 (one tower)
    ACHIEV_CRIT_ORB_BOMB_H      = 10061,
    ACHIEV_CRIT_ORB_DEV_N       = 10057,                    // Flame Leviathan, achievs 2914, 2916 (two towers)
    ACHIEV_CRIT_ORB_DEV_H       = 10059,
    ACHIEV_CRIT_ORB_NUKED_N     = 10058,                    // Flame Leviathan, achievs 2915, 2917 (three towers)
    ACHIEV_CRIT_ORB_NUKED_H     = 10060,
    ACHIEV_CRIT_ORBITUARY_N     = 10218,                    // Flame Leviathan, achievs 3056, 3057 (four towers)
    ACHIEV_CRIT_ORBITUARY_H     = 10219,
    ACHIEV_CRIT_SHUTOUT_N       = 10054,                    // Flame Leviathan, achievs 2911, 2913
    ACHIEV_CRIT_SHUTOUT_H       = 10055,
    ACHIEV_CRIT_UNBROKEN_N      = 10044,                    // Flame Leviathan, achievs 2905, 2906
    ACHIEV_CRIT_UNBROKEN_H      = 10045,
};

struct UlduarSpawns
{
    float fX, fY, fZ, fO;
    uint32 uiEntry;
};

// Note: coordinates are guessed, but pretty close to what they should be
// ToDo: spawn additional Engineers, Demolitionists, Mages and Liquid Pyrite near the columns
static const UlduarSpawns afReinforcementsNormal[] =
{
    {118.797f, -26.9963f,409.80f, 3.14f, NPC_SALVAGED_SIEGE_ENGINE},
    {118.847f, -43.758f, 409.80f, 3.15f, NPC_SALVAGED_SIEGE_ENGINE},
    {116.602f, 8.464f, 409.80f, 3.10f, NPC_SALVAGED_CHOPPER},
    {116.859f, -4.199f, 409.80f, 3.12f, NPC_SALVAGED_CHOPPER},
    {122.479f, 25.093f, 410.60f, 3.10f, NPC_SALVAGED_DEMOLISHER},
    {123.022f, 39.671f, 409.80f, 3.10f, NPC_SALVAGED_DEMOLISHER},
};
static const UlduarSpawns afReinforcementsHeroic[] =
{
    {106.359f, -35.269f, 409.80f, 3.12f, NPC_SALVAGED_SIEGE_ENGINE},
    {135.351f, -20.767f, 409.80f, 3.15f, NPC_SALVAGED_SIEGE_ENGINE},
    {135.408f, -50.178f, 409.80f, 3.12f, NPC_SALVAGED_SIEGE_ENGINE},
    {116.429f, 4.036f, 409.79f, 3.10f, NPC_SALVAGED_CHOPPER},
    {116.272f, -0.013f, 409.79f, 3.10f, NPC_SALVAGED_CHOPPER},
    {116.948f, -8.351f, 409.79f, 3.10f, NPC_SALVAGED_CHOPPER},
    {137.523f, 32.346f, 409.80f, 3.12f, NPC_SALVAGED_DEMOLISHER},
    {112.818f, 18.981f, 409.83f, 3.10f, NPC_SALVAGED_DEMOLISHER},
    {112.700f, 47.884f, 409.79f, 3.10f, NPC_SALVAGED_DEMOLISHER},
};

// note: original spawn loc is 607.9199f, -12.90516f, 409.887f but we won't use it because it's too far and grid won't be loaded that far
static const float afLeviathanSpawnPos[4] = { 422.8898f, -13.32677f, 409.8839f, 3.12f };
static const float afLeviathanMovePos[4] = { 296.5809f, -11.55668f, 409.8278f, 3.12f };

class MANGOS_DLL_DECL instance_ulduar : public ScriptedInstance, private DialogueHelper
{
    public:
        instance_ulduar(Map* pMap);
        ~instance_ulduar() {}

        void Initialize() override;
        bool IsEncounterInProgress() const override;

        void OnPlayerEnter(Player* pPlayer) override;
        void OnCreatureCreate(Creature* pCreature) override;
        void OnCreatureDeath(Creature* pCreature) override;
        void OnObjectCreate(GameObject* pGo) override;

        void SetData(uint32 uiType, uint32 uiData) override;
        uint32 GetData(uint32 uiType) const override;

        const char* Save() const override { return m_strInstData.c_str(); }
        void Load(const char* chrIn) override;

        void SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet);
        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/) const override;
        bool CheckConditionCriteriaMeet(Player const* pPlayer, uint32 uiInstanceConditionId, WorldObject const* pConditionSource, uint32 conditionSourceType) const override;

        void DoOpenMadnessDoorIfCan();

        void SpawnFriendlyKeeper(uint32 uiWho);

        void DoProcessShatteredEvent();

        void GetDefenderGuids(GuidList& lDefenders) { lDefenders = m_lDefendersGuids; }
        void GetEngineersGuids(GuidList& lEngineers) { lEngineers = m_lEngineersGuids; }
        void GetTrappersGuids(GuidList& lTrappers) { lTrappers = m_lTrappersGuids; }
        void GetHarpoonsGuids(GuidVector& vHarpoons) { vHarpoons = m_vBrokenHarpoonsGuids; }
        void GetToyPileGuids(GuidVector& vToyPiles) { vToyPiles = m_vToyPileGuidVector; }

        void Update(uint32 uiDiff);

    protected:
        void JustDidDialogueStep(int32 iEntry) override;
        void DoCallLeviathanHelp();

        std::string m_strInstData;
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        uint32 m_auiHardBoss[HARD_MODE_ENCOUNTER];
        uint32 m_auiUlduarKeepers[KEEPER_ENCOUNTER];
        uint32 m_auiUlduarTowers[KEEPER_ENCOUNTER];
        bool m_abAchievCriteria[MAX_SPECIAL_ACHIEV_CRITS];

        uint32 m_uiShatterAchievTimer;
        uint32 m_uiGauntletStatus;

        GuidVector m_vToyPileGuidVector;
        GuidVector m_vBrokenHarpoonsGuids;
        GuidList m_lEngineersGuids;
        GuidList m_lTrappersGuids;
        GuidList m_lDefendersGuids;
        GuidList m_lHarpoonDummyGuids;
        GuidList m_lRepairedHarpoonsGuids;
        GuidSet m_sColossusGuidSet;
};

#endif
