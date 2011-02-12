/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BRD_H
#define DEF_BRD_H

enum
{
    MAX_ENCOUNTER           = 6,
    TYPE_RING_OF_LAW        = 1,
    TYPE_VAULT              = 2,
    TYPE_BAR                = 3,
    TYPE_TOMB_OF_SEVEN      = 4,
    TYPE_LYCEUM             = 5,
    TYPE_IRON_HALL          = 6,

    NPC_EMPEROR             = 9019,
    NPC_PRINCESS            = 8929,
    NPC_PHALANX             = 9502,
    NPC_HATEREL             = 9034,
    NPC_ANGERREL            = 9035,
    NPC_VILEREL             = 9036,
    NPC_GLOOMREL            = 9037,
    NPC_SEETHREL            = 9038,
    NPC_DOOMREL             = 9039,
    NPC_DOPEREL             = 9040,

    GO_ARENA_1              = 161525,
    GO_ARENA_2              = 161522,
    GO_ARENA_3              = 161524,
    GO_ARENA_4              = 161523,

    GO_SHADOW_LOCK          = 161460,
    GO_SHADOW_MECHANISM     = 161461,
    GO_SHADOW_GIANT_DOOR    = 157923,
    GO_SHADOW_DUMMY         = 161516,
    GO_BAR_KEG_SHOT         = 170607,
    GO_BAR_KEG_TRAP         = 171941,
    GO_BAR_DOOR             = 170571,
    GO_TOMB_ENTER           = 170576,
    GO_TOMB_EXIT            = 170577,
    GO_LYCEUM               = 170558,
    GO_GOLEM_ROOM_N         = 170573,
    GO_GOLEM_ROOM_S         = 170574,
    GO_THRONE_ROOM          = 170575,

    GO_SPECTRAL_CHALICE     = 164869,
    GO_CHEST_SEVEN          = 169243,
    GO_ARENA_SPOILS         = 181074,
};

enum ArenaNPCs
{
    // Gladiators
    NPC_LEFTY               = 16049,
    NPC_ROTFANG             = 16050,
    NPC_SNOKH               = 16051,
    NPC_MALGEN              = 16052,
    NPC_KORV                = 16053,
    NPC_REZZNIK             = 16054,
    NPC_VAJASHNI            = 16055,
    NPC_VOLIDA              = 16058,
    NPC_THELDREN            = 16059,
    // Ring mobs
    NPC_WORM                = 8925,
    NPC_STINGER             = 8926,
    NPC_SCREECHER           = 8927,
    NPC_THUNDERSNOUT        = 8928,
    NPC_CREEPER             = 8933,
    NPC_BEETLE              = 8932,
    // Ring bosses
    NPC_GOROSH              = 9027,
    NPC_GRIZZLE             = 9028,
    NPC_EVISCERATOR         = 9029,
    NPC_OKTHOR              = 9030,
    NPC_ANUBSHIAH           = 9031,
    NPC_HEDRUM              = 9032
};

static const uint32 aArenaNPCs[] =
{
    // Gladiators
    NPC_LEFTY, NPC_ROTFANG, NPC_SNOKH, NPC_MALGEN, NPC_KORV, NPC_REZZNIK, NPC_VAJASHNI, NPC_VOLIDA, NPC_THELDREN,
    // Ring mobs
    NPC_WORM, NPC_STINGER, NPC_SCREECHER, NPC_THUNDERSNOUT, NPC_CREEPER, NPC_BEETLE,
    // Ring bosses
    NPC_GOROSH, NPC_GRIZZLE, NPC_EVISCERATOR, NPC_OKTHOR, NPC_ANUBSHIAH, NPC_HEDRUM
};

class MANGOS_DLL_DECL instance_blackrock_depths : public ScriptedInstance
{
    public:
        instance_blackrock_depths(Map* pMap);
        ~instance_blackrock_depths() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);
        void OnCreatureEvade(Creature* pCreature);

        // Arena Event
        void SetArenaCenterCoords(float fX, float fY, float fZ) { m_fArenaCenterX = fX; m_fArenaCenterY = fY; m_fArenaCenterZ = fZ; }
        void GetArenaCenterCoords(float &fX, float &fY, float &fZ) { fX = m_fArenaCenterX; fY = m_fArenaCenterY; fZ = m_fArenaCenterZ; }

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint64 m_uiEmperorGUID;
        uint64 m_uiPrincessGUID;
        uint64 m_uiPhalanxGUID;
        uint64 m_uiHaterelGUID;
        uint64 m_uiAngerrelGUID;
        uint64 m_uiVilerelGUID;
        uint64 m_uiGloomrelGUID;
        uint64 m_uiSeethrelGUID;
        uint64 m_uiDoomrelGUID;
        uint64 m_uiDoperelGUID;

        uint64 m_uiGoArena1GUID;
        uint64 m_uiGoArena2GUID;
        uint64 m_uiGoArena3GUID;
        uint64 m_uiGoArena4GUID;
        uint64 m_uiGoShadowLockGUID;
        uint64 m_uiGoShadowMechGUID;
        uint64 m_uiGoShadowGiantGUID;
        uint64 m_uiGoShadowDummyGUID;
        uint64 m_uiGoBarKegGUID;
        uint64 m_uiGoBarKegTrapGUID;
        uint64 m_uiGoBarDoorGUID;
        uint64 m_uiGoTombEnterGUID;
        uint64 m_uiGoTombExitGUID;
        uint64 m_uiGoLyceumGUID;
        uint64 m_uiGoGolemNGUID;
        uint64 m_uiGoGolemSGUID;
        uint64 m_uiGoThroneGUID;

        uint64 m_uiSpectralChaliceGUID;
        uint64 m_uiSevensChestGUID;
        uint64 m_uiArenaSpoilsGUID;

        uint32 m_uiBarAleCount;

        float m_fArenaCenterX, m_fArenaCenterY, m_fArenaCenterZ;
};

#endif
