/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SUNKEN_TEMPLE_H
#define DEF_SUNKEN_TEMPLE_H

enum
{
    MAX_ENCOUNTER         = 4,
    MAX_STATUES           = 6,

    // Don't change types 1,2 and 3 (handled in ACID)
    TYPE_ATALARION_OBSOLET= 1,
    TYPE_PROTECTORS_OBS   = 2,
    TYPE_JAMMALAN_OBS     = 3,

    TYPE_ATALARION        = 4,
    TYPE_PROTECTORS       = 5,
    TYPE_JAMMALAN         = 6,
    TYPE_MALFURION        = 7,

    NPC_ATALARION         = 8580,
    NPC_DREAMSCYTH        = 5721,
    NPC_WEAVER            = 5720,
    NPC_JAMMALAN          = 5710,
    NPC_AVATAR_OF_HAKKAR  = 8443,
    NPC_SHADE_OF_ERANIKUS = 5709,

    // Jammalain mini-bosses
    NPC_ZOLO              = 5712,
    NPC_GASHER            = 5713,
    NPC_LORO              = 5714,
    NPC_HUKKU             = 5715,
    NPC_ZULLOR            = 5716,
    NPC_MIJAN             = 5717,

    NPC_MALFURION         = 15362,

    GO_ALTAR_OF_HAKKAR    = 148836,                         // Used in order to show the player the order of the statue activation
    GO_IDOL_OF_HAKKAR     = 148838,                         // Appears when atalarion is summoned; this was removed in 4.0.1

    GO_ATALAI_LIGHT       = 148883,                         // Green light, activates when the correct statue is chosen
    GO_ATALAI_LIGHT_BIG   = 148937,                         // Big light, used at the altar event

    GO_ATALAI_TRAP_1      = 177484,                         // Trapps triggered if the wrong statue is activated
    GO_ATALAI_TRAP_2      = 177485,                         // The traps are spawned in DB randomly around the statues (we don't know exactly which statue has which trap)
    GO_ATALAI_TRAP_3      = 148837,

    GO_ETERNAL_FLAME_1    = 148418,
    GO_ETERNAL_FLAME_2    = 148419,
    GO_ETERNAL_FLAME_3    = 148420,
    GO_ETERNAL_FLAME_4    = 148421,

    GO_JAMMALAN_BARRIER   = 149431,

    // Event ids related to the statue activation
    EVENT_ID_STATUE_1     = 3094,
    EVENT_ID_STATUE_2     = 3095,
    EVENT_ID_STATUE_3     = 3097,
    EVENT_ID_STATUE_4     = 3098,
    EVENT_ID_STATUE_5     = 3099,
    EVENT_ID_STATUE_6     = 3100,

    SAY_JAMMALAN_INTRO    = -1109005,
};

// This is also the needed order for activation: S, N, SW, SE, NW, NE
static const uint32 m_aAtalaiStatueEvents[MAX_STATUES] = {EVENT_ID_STATUE_1, EVENT_ID_STATUE_2, EVENT_ID_STATUE_3, EVENT_ID_STATUE_4, EVENT_ID_STATUE_5, EVENT_ID_STATUE_6};

struct SummonLocations
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static const SummonLocations aSunkenTempleLocation[] =
{
    {-466.5130f, 95.19820f, -189.646f, 0.0349f},            // Atalarion summon loc
    {-466.8673f,272.31204f, -90.7441f, 3.5255f},            // Shade of hakkar summon loc
    {-660.5277f, -16.7117f, -90.8357f, 1.6055f}             // Malfurion summon loc
};

class MANGOS_DLL_DECL instance_sunken_temple : public ScriptedInstance
{
    public:
        instance_sunken_temple(Map* pMap);
        ~instance_sunken_temple() {}

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        // No need currently to use OnEnterCombat or OnEvade
        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        bool ProcessStatueEvent(uint32 uiEventId);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        void DoSpawnAtalarionIfCan();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiAtalarionGUID;
        uint64 m_uiJammalanGUID;
        uint64 m_uiJammalanBarrierGUID;
        uint64 m_uiIdolOfHakkarGUID;
        uint8 m_uiStatueCounter;
        uint8 m_uiProtectorsRemaining;

        GUIDList m_luiBigLightGUIDs;
};

#endif
