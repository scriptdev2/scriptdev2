/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_CULLING_OF_STRATHOLME_H
#define DEF_CULLING_OF_STRATHOLME_H

enum
{
    MAX_ENCOUNTER                   = 9,

    TYPE_GRAIN_EVENT                = 0,                    // crates with plagued grain identified
    TYPE_ARTHAS_INTRO_EVENT         = 1,                    // Arhas Speech and Walk to Gates and short intro with MalGanis
    TYPE_MEATHOOK_EVENT             = 2,                    // Waves 1-5
    TYPE_SALRAMM_EVENT              = 3,                    // Waves 6-10
    TYPE_EPOCH_EVENT                = 4,                    // Townhall Event, Boss Killed
    TYPE_ARTHAS_ESCORT_EVENT        = 5,                    // Townhall to Malganis
    TYPE_MALGANIS_EVENT             = 6,                    // Malganis
    TYPE_INFINITE_CORRUPTER_TIME    = 7,                    // Time for 25min Timer
    TYPE_INFINITE_CORRUPTER         = 8,

    // Main Encounter NPCs
    NPC_CHROMIE_INN                 = 26527,
    NPC_CHROMIE_ENTRANCE            = 27915,
    NPC_CHROMIE_END                 = 30997,
    // NPC_HOURGLASS                = 28656,
    NPC_ARTHAS                      = 26499,
    // NPC_MEATHOOK                 = 26529,
    // NPC_SALRAMM_THE_FLESHCRAFTER = 26530,
    // NPC_CHRONO_LORD_EPOCH        = 26532,
    // NPC_MALGANIS                 = 26533,
    NPC_INFINITE_CORRUPTER          = 32273,
    NPC_LORDAERON_CRIER             = 27913,
    NPC_ZOMBIE                      = 27737,

    // Inn Event related NPC
    NPC_MICHAEL_BELFAST             = 30571,
    NPC_HEARTHSINGER_FORRESTEN      = 30551,
    NPC_FRAS_SIABI                  = 30552,
    NPC_FOOTMAN_JAMES               = 30553,
    NPC_MAL_CORRICKS                = 31017,
    NPC_GRYAN_STOUTMANTLE           = 30561,

    // Grain Event NPCs
    NPC_ROGER_OWENS                 = 27903,
    NPC_SERGEANT_MORIGAN            = 27877,
    NPC_JENA_ANDERSON               = 27885,
    NPC_MALCOM_MOORE                = 27891,                // Not (yet?) spawned
    NPC_BARTLEBY_BATTSON            = 27907,
    NPC_GRAIN_CRATE_HELPER          = 27827,
    // NPC_CRATES_BUNNY             = 30996,

    // Intro Event NPCs
    NPC_LORDAERON_FOOTMAN           = 27745,
    NPC_STRATHOLME_CITIZEN          = 28167,
    NPC_STRATHOLME_RESIDENT         = 28169,

    // Mobs in Stratholme (to despawn) -- only here for sake of completeness handling remains open (mangos feature)
    NPC_MAGISTRATE_BARTHILAS        = 30994,
    NPC_STEPHANIE_SINDREE           = 31019,
    NPC_LEEKA_TURNER                = 31027,
    NPC_SOPHIE_AAREN                = 31021,
    NPC_ROBERT_PIERCE               = 31025,
    NPC_GEORGE_GOODMAN              = 31022,

    // Others NPCs in Stratholme
    NPC_EMERY_NEILL                 = 30570,
    NPC_EDWARD_ORRICK               = 31018,
    NPC_OLIVIA_ZENITH               = 31020,

    // Townhall Event NPCs
    NPC_AGIATED_STRATHOLME_CITIZEN  = 31126,
    NPC_AGIATED_STRATHOLME_RESIDENT = 31127,
    NPC_PATRICIA_O_REILLY           = 31028,

    // Gameobjects
    GO_DOOR_BOOKCASE                = 188686,
    GO_DARK_RUNED_CHEST             = 190663,
    GO_DARK_RUNED_CHEST_H           = 193597,

    GO_SUSPICIOUS_GRAIN_CRATE       = 190094,
    GO_CRATE_HIGHLIGHT              = 190117,
    GO_PLAGUE_GRAIN_CRATE           = 190095,

    // World States
    WORLD_STATE_CRATES              = 3479,
    WORLD_STATE_CRATES_COUNT        = 3480,
    WORLD_STATE_WAVE                = 3504,
    WORLD_STATE_TIME                = 3932,
    WORLD_STATE_TIME_COUNTER        = 3931,

    // Areatrigger
    AREATRIGGER_INN                 = 5291,
    /*
    5085 before bridge - could be Uther SpawnPos
    5148 ini entrance
    5181 ini exit
    5249 fras siabis store
    5250 leeking shields...(store)
    5251 bar in stratholme
    5252 Aaren flowers
    5253 Angelicas boutique
    5256 townhall
    5291 Inn */
};

enum eInstancePosition
{
    POS_ARTHAS_INTRO        = 1,
    POS_ARTHAS_WAVES        = 2,
    POS_ARTHAS_TOWNHALL     = 3,
    POS_ARTHAS_ESCORTING    = 4,
    POS_ARTHAS_MALGANIS     = 5,
    POS_INSTANCE_FINISHED   = 6
};

class instance_culling_of_stratholme : public ScriptedInstance
{
    public:
        instance_culling_of_stratholme(Map* pMap);
        ~instance_culling_of_stratholme() {}

        void Initialize() override;

        void OnPlayerEnter(Player* pPlayer) override;
        void OnCreatureCreate(Creature* pCreature) override;
        void OnObjectCreate(GameObject* pGo) override;

        void SetData(uint32 uiType, uint32 uiData) override;
        uint32 GetData(uint32 uiType) const override;

        const char* Save() const override { return m_strInstData.c_str(); }
        void Load(const char* chrIn) override;

        void Update(uint32 uiDiff) override;

        void GetStratAgiatedCitizenList(GuidList& lList) { lList = m_lAgiatedCitizenGUIDList; };
        void GetStratAgiatedResidentList(GuidList& lList) { lList = m_lAgiatedResidentGUIDList; };

        void GetCratesBunnyOrderedList(std::list<Creature*>& lList);
        Creature* GetStratIntroFootman();
        void GetResidentOrderedList(std::list<Creature*>& lList);

        void DoSpawnArthasIfNeeded(Unit* pSummoner);
        bool CanGrainEventProgress(Creature* pCrate);

        void DoEventAtTriggerIfCan(uint32 uiTriggerId);

    protected:
        void DoChromieWhisper(int32 iEntry);
        void DoSpawnChromieIfNeeded(Unit* pSummoner);
        uint8 GetInstancePosition();
        void ArthasJustDied();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        bool m_bStartedInnEvent;

        uint8 m_uiGrainCrateCount;
        uint32 m_uiRemoveCrateStateTimer;
        uint32 m_uiArthasRespawnTimer;

        GuidList m_luiCratesBunnyGUIDs;
        GuidList m_luiFootmanGUIDs;
        GuidList m_luiResidentGUIDs;

        GuidList m_lAgiatedCitizenGUIDList;
        GuidList m_lAgiatedResidentGUIDList;

        GuidSet m_sGrainCratesGuidSet;
};

#endif
