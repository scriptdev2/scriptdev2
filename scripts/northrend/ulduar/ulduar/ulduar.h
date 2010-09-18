/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ULDUAR_H
#define DEF_ULDUAR_H

enum
{
    MAX_ENCOUNTER               = 14,
    HARD_MODE_ENCOUNTER         = 9,
    KEEPER_ENCOUNTER            = 4,
    TELEPORTER_ENCOUNTER        = 3,

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
    TYPE_ASSEMBLY_HARD          = 16,
    TYPE_MIMIRON_HARD           = 17,
    TYPE_HODIR_HARD             = 18,
    TYPE_THORIM_HARD            = 19,
    TYPE_FREYA_HARD             = 20,
    TYPE_VEZAX_HARD             = 21,
    TYPE_YOGGSARON_HARD         = 22,

    // Keeper types
    // Used to store the keepers which will be used at yogg
    TYPE_KEEPER_HODIR           = 23,
    TYPE_KEEPER_FREYA           = 24,
    TYPE_KEEPER_THORIM          = 25,
    TYPE_KEEPER_MIMIRON         = 26,

    // Teleporter types
    // Some teleporters aren't depending directly on a boss, so we use a distinct type for them (maybe use areatrigger?)
    TYPE_LEVIATHAN_TP           = 27,
    TYPE_XT002_TP               = 28,
    TYPE_MIMIRON_TP             = 29,

    // The siege of ulduar
    NPC_LEVIATHAN               = 33113,
    NPC_IGNIS                   = 33118,
    NPC_RAZORSCALE              = 33186,
    NPC_COMMANDER               = 33210,
    NPC_XT002                   = 33293,

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

    // Loot chests
    // Kologarn
    GO_CACHE_OF_LIVING_STONE    = 195046,
    GO_CACHE_OF_LIVING_STONE_H  = 195047,

    // Hodir
    GO_CACHE_OF_WINTER          = 194307,
    GO_CACHE_OF_WINTER_H        = 194308,
    GO_CACHE_OF_RARE_WINTER     = 194200,
    GO_CACHE_OF_RARE_WINTER_H   = 194201,

    // Mimiron
    GO_CACHE_OF_INOV            = 194789,
    GO_CACHE_OF_INOV_H          = 194956,
    GO_CACHE_OF_INOV_HARD       = 194957,
    GO_CACHE_OF_INOV_HARD_H     = 194958,

    // Thorim
    GO_CACHE_OF_STORMS          = 194312,
    GO_CACHE_OF_STORMS_H        = 194314,
    GO_CACHE_OF_RARE_STORMS     = 194313,
    GO_CACHE_OF_RARE_STORMS_H   = 194315,

    // Alagon
    GO_GIFT_OF_OBSERVER_H       = 194821,
    GO_GIFT_OF_OBSERVER         = 194822,
    GO_GIFT_OF_OBSERVER_HH      = 194823,                   // Purpose Unknown

    // Doors and other Objects
    // The siege
    GO_SHIELD_WALL              = 194416,                   // Gate before Leviathan
    GO_LEVIATHAN_GATE           = 194630,                   // Gate after Leviathan -> this should be used before the boss enter the arena
    // There should be another gate after the Leviathan here which should be used during the Leviathan encounter
    GO_XT002_GATE               = 194631,                   // Gate before Xt002
    GO_BROKEN_HARPOON           = 194565,                   // Broken harpoon from Razorscale

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
    GO_LIGHTNING_DOOR           = 194905,                   // Arena exit door
    GO_LIGHTNING_FIELD          = 194559,                   // For the platform animation
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
};

class MANGOS_DLL_DECL instance_ulduar : public ScriptedInstance
{
    public:
        instance_ulduar(Map* pMap);
        ~instance_ulduar() {}

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

        // Dummy, leave till correct solution for hardmode found
        bool CheckConditionCriteriaMeet(Player const* pSource, uint32 uiMapId, uint32 uiInstanceConditionId);

        void DoOpenMadnessDoorIfCan();
        Player* GetPlayerInMap();
        void SpawnFriendlyKeeper(uint32 uiWho);

    protected:
        std::string strInstData;
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        uint32 m_auiHardBoss[HARD_MODE_ENCOUNTER];
        uint32 m_auiUlduarKeepers[KEEPER_ENCOUNTER];
        uint32 m_auiUlduarTeleporters[TELEPORTER_ENCOUNTER];

        // Creatures
        uint64 m_uiLeviathanGUID;
        uint64 m_uiIgnisGUID;
        uint64 m_uiRazorscaleGUID;
        uint64 m_uiCommanderGUID;
        uint64 m_uiXT002GUID;
        uint64 m_uiBrundirGUID;
        uint64 m_uiMolgeimGUID;
        uint64 m_uiSteelbreakerGUID;
        uint64 m_uiKologarnGUID;
        uint64 m_uiAuriayaGUID;
        uint64 m_uiMimironGUID;
        uint64 m_uiHodirGUID;
        uint64 m_uiThorimGUID;
        uint64 m_uiFreyaGUID;
        uint64 m_uiVezaxGUID;
        uint64 m_uiYoggSaronGUID;
        uint64 m_uiAlgalonGUID;
        uint64 m_uiRightArmGUID;
        uint64 m_uiLeftArmGUID;
        uint64 m_uiFeralDefenderGUID;
        uint64 m_uiElderBrightleafGUID;
        uint64 m_uiElderStonebarkGUID;
        uint64 m_uiElderIronbrachGUID;
        uint64 m_uiSaroniteAnimusGUID;
        uint64 m_uiRunicColossusGUID;
        uint64 m_uiRuneGiantGUID;
        uint64 m_uiJormungarGUID;
        uint64 m_uiLeviathanMkGUID;
        uint64 m_uiSaraGUID;
        uint64 m_uiYoggBrainGUID;

        // Doors & Objects
        // The siege
        uint64 m_uiShieldWallGUID;
        uint64 m_uiLeviathanGateGUID;
        uint64 m_uiXT002GateGUID;
        uint64 m_uiBrokenHarpoonGUID;
        // Archivum
        uint64 m_uiIronCouncilDoorGUID;
        uint64 m_uiArchivumDoorGUID;
        uint64 m_uiArchivumConsoleGUID;
        uint64 m_uiUniverseFloorArchivumGUID;
        // Celestial planetarium
        uint64 m_uiCelestialDoorGUID;
        uint64 m_uiCelestialConsoleGUID;
        uint64 m_uiUniverseFloorCelestialGUID;
        uint64 m_uiAzerothGlobeGUID;
        // Kologarn
        uint64 m_uiShatteredHallsDoorGUID;
        uint64 m_uiKologarnBridgeGUID;
        // Hodir
        uint64 m_uiHodirEnterDoorGUID;
        uint64 m_uiHodirWallGUID;
        uint64 m_uiHodirExitDoorGUID;
        // Mimiron
        uint64 m_uiMimironButtonGUID;
        uint64 m_uiMimironDoor1GUID;
        uint64 m_uiMimironDoor2GUID;
        uint64 m_uiMimironDoor3GUID;
        uint64 m_uiMimironElevatorGUID;
        uint64 m_auiMimironTelGUID[9];
        // Thorim
        uint64 m_uiArenaEnterDoorGUID;
        uint64 m_uiArenaExitDoorGUID;
        uint64 m_uiHallwayDoorGUID;
        uint64 m_uiThorimEnterDoorGUID;
        uint64 m_uiThorimLeverGUID;
        // Prison
        uint64 m_uiAncientGateGUID;
        uint64 m_uiVezaxGateGUID;
        uint64 m_uiYoggGateGUID;
        uint64 m_uiBrainDoor1GUID;
        uint64 m_uiBrainDoor2GUID;
        uint64 m_uiBrainDoor3GUID;

        // Chests
        uint64 m_uiKologarnLootGUID;
        uint64 m_uiHodirLootGUID;
        uint64 m_uiHodirRareLootGUID;
        uint64 m_uiThorimLootGUID;
        uint64 m_uiThorimRareLootGUID;
        uint64 m_uiMimironLootGUID;
        uint64 m_uiMimironHardLootGUID;
        uint64 m_uiAlagonLootGUID;
};

#endif
