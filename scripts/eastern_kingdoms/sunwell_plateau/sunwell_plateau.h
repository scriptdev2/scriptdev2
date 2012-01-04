/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SUNWELLPLATEAU_H
#define DEF_SUNWELLPLATEAU_H

enum
{
    MAX_ENCOUNTER               = 6,

    TYPE_KALECGOS               = 0,
    TYPE_BRUTALLUS              = 1,
    TYPE_FELMYST                = 2,
    TYPE_EREDAR_TWINS           = 3,
    TYPE_MURU                   = 4,
    TYPE_KILJAEDEN              = 5,

    NPC_KALECGOS_DRAGON         = 24850,            // kalecgos blue dragon hostile
    NPC_KALECGOS_HUMAN          = 24891,            // kalecgos human form in spectral realm
    NPC_SATHROVARR              = 24892,
    NPC_MADRIGOSA               = 25160,
    NPC_FLIGHT_TRIGGER_LEFT     = 25357,            // Related to Felmyst flight path. Also the anchor to summon Madrigosa
    // NPC_FLIGHT_TRIGGER_RIGHT = 25358,            // related to Felmyst flight path
    NPC_BRUTALLUS               = 24882,
    NPC_FELMYST                 = 25038,
    NPC_ALYTHESS                = 25166,
    NPC_SACROLASH               = 25165,
    NPC_MURU                    = 25741,
    NPC_ENTROPIUS               = 25840,
    NPC_DECEIVER                = 25588,
    NPC_KILJAEDEN               = 25315,
    NPC_KILJAEDEN_CONTROLLER    = 25608,            // kiljaeden event controller
    NPC_ANVEENA                 = 26046,            // related to kiljaeden event
    NPC_KALECGOS                = 25319,            // related to kiljaeden event
    NPC_VELEN                   = 26246,
    NPC_LIADRIN                 = 26247,

    GO_FORCEFIELD               = 188421,           // kalecgos door + collisions
    GO_BOSS_COLLISION_1         = 188523,
    GO_BOSS_COLLISION_2         = 188524,
    GO_ICE_BARRIER              = 188119,           // used to block the players path during the Brutallus intro event
    GO_FIRE_BARRIER             = 188075,           // door after felmyst
    GO_FIRST_GATE               = 187766,           // door between felmyst and eredar twins
    GO_SECOND_GATE              = 187764,           // door after eredar twins
    GO_MURU_ENTER_GATE          = 187990,           // muru gates
    GO_MURU_EXIT_GATE           = 188118,
    GO_THIRD_GATE               = 187765,           // door after muru; why another?

    SAY_KALECGOS_OUTRO          = -1580043,
    SAY_TWINS_INTRO             = -1580044,

    AREATRIGGER_TWINS           = 4937,

    // Felmyst ouro spell
    SPELL_OPEN_BACK_DOOR        = 46650,            // Opens the fire barrier - script effect for 46652
    // used by both muru and entropius
    SPELL_MURU_BERSERK          = 26662,
    // visuals for Kiljaeden encounter
    SPELL_ANVEENA_DRAIN         = 46410,

    MAX_DECEIVERS               = 3
};

// Used to summon Felmyst in reload case. This is the place where Madrigosa is killed in the intro event
static const float aMadrigosaGroundLoc[4] = {1459.35f, 636.81f, 19.94f, 4.88f};
// Used as an anchor to move Madrigosa and Kalec during the cinematic
static const float aMadrigosaFlyLoc[3] = {1459.35f, 636.81f, 59.234f};

class MANGOS_DLL_DECL instance_sunwell_plateau : public ScriptedInstance, private DialogueHelper
{
    public:
        instance_sunwell_plateau(Map* pMap);
        ~instance_sunwell_plateau() {}

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnPlayerEnter(Player* pPlayer);
        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        void Update(uint32 uiDiff);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        void JustDidDialogueStep(int32 iEntry);

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        // Misc
        uint8 m_uiDeceiversKilled;
        uint32 m_uiSpectralRealmTimer;
        uint32 m_uiKalecRespawnTimer;
        uint32 m_uiMuruBerserkTimer;
};
#endif
