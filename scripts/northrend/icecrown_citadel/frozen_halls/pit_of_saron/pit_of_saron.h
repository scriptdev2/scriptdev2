/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ICECROWN_PIT_H
#define DEF_ICECROWN_PIT_H

enum
{
    MAX_ENCOUNTER                   = 3,
    MAX_SPECIAL_ACHIEV_CRITS        = 2,

    TYPE_GARFROST                   = 0,
    TYPE_KRICK                      = 1,
    TYPE_TYRANNUS                   = 2,

    TYPE_ACHIEV_DOESNT_GO_ELEVEN    = 0,
    TYPE_ACHIEV_DONT_LOOK_UP        = 1,

    NPC_TYRANNUS_INTRO              = 36794,
    NPC_GARFROST                    = 36494,
    NPC_KRICK                       = 36477,
    NPC_ICK                         = 36476,
    NPC_TYRANNUS                    = 36658,
    NPC_RIMEFANG                    = 36661,
    NPC_SINDRAGOSA                  = 37755,

    NPC_SYLVANAS_PART1              = 36990,
    NPC_SYLVANAS_PART2              = 38189,
    NPC_JAINA_PART1                 = 36993,
    NPC_JAINA_PART2                 = 38188,
    NPC_KILARA                      = 37583,
    NPC_ELANDRA                     = 37774,
    NPC_LORALEN                     = 37779,
    NPC_KORELN                      = 37582,
    NPC_CHAMPION_1_HORDE            = 37584,
    NPC_CHAMPION_2_HORDE            = 37587,
    NPC_CHAMPION_3_HORDE            = 37588,
    NPC_CHAMPION_1_ALLIANCE         = 37496,
    NPC_CHAMPION_2_ALLIANCE         = 37497,
    NPC_CHAMPION_3_ALLIANCE         = 37498,
    NPC_CORRUPTED_CHAMPION          = 36796,
    NPC_IRONSKULL_PART1             = 37592,
    NPC_IRONSKULL_PART2             = 37592,
    NPC_VICTUS_PART1                = 37591,
    NPC_VICTUS_PART2                = 37580,

    GO_ICEWALL                      = 201885,               // open after gafrost/krick
    GO_HALLS_OF_REFLECT_PORT        = 201848,               // unlocked by jaina/sylvanas at last outro

    ACHIEV_CRIT_DOESNT_GO_ELEVEN    = 12993,                // Garfrost, achiev 4524
    ACHIEV_CRIT_DONT_LOOK_UP        = 12994,                // Gauntlet, achiev 4525
};

static const float afTyrannusMovePos[3][3] =
{
    {922.6365f, 145.877f, 643.2216f},                       // Hide position
    {835.5887f, 139.4345f, 530.9526f},                      // Ick position
    {906.9048f, -49.03813f, 618.8016f},                     // Tunnel position
};

struct EventNpcLocations
{
    uint32 uiEntryHorde, uiEntryAlliance;
    float fX, fY, fZ, fO;
    float fMoveX, fMoveY, fMoveZ;
};

const EventNpcLocations aEventBeginLocations[3] =
{
    {NPC_SYLVANAS_PART1, NPC_JAINA_PART1,   430.3012f, 212.204f,  530.1146f, 0.042f, 440.7882f, 213.7587f, 528.7103f},
    {NPC_KILARA,         NPC_ELANDRA,       429.7142f, 212.3021f, 530.2822f, 0.14f,  438.9462f, 215.4271f, 528.7087f},
    {NPC_LORALEN,        NPC_KORELN,        429.5675f, 211.7748f, 530.3246f, 5.972f, 438.5052f, 211.5399f, 528.7085f},
    // ToDo: add the soldiers here when proper waypoint movement is supported
};

class MANGOS_DLL_DECL instance_pit_of_saron : public ScriptedInstance, private DialogueHelper
{
    public:
        instance_pit_of_saron(Map* pMap);
        ~instance_pit_of_saron() {}

        void Initialize() override;

        void OnCreatureCreate(Creature* pCreature) override;
        void OnObjectCreate(GameObject* pGo) override;

        void OnPlayerEnter(Player* pPlayer) override;

        void SetData(uint32 uiType, uint32 uiData) override;
        uint32 GetData(uint32 uiType) const override;

        uint32 GetPlayerTeam() { return m_uiTeam; }

        void SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet);
        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/) const override;

        const char* Save() const override { return m_strInstData.c_str(); }
        void Load(const char* chrIn) override;

        void Update(uint32 uiDiff) { DialogueUpdate(uiDiff); }

    protected:
        void JustDidDialogueStep(int32 iEntry) override;
        void ProcessIntroEventNpcs(Player* pPlayer);

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        bool m_abAchievCriteria[MAX_SPECIAL_ACHIEV_CRITS];

        uint32 m_uiTeam;                                    // Team of first entered player, used to set if Jaina or Silvana to spawn
};

#endif
