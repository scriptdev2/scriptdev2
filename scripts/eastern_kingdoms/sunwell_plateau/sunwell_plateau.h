/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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

    DATA_PLAYER_SPECTRAL_REALM  = 6,
    DATA_SET_SPECTRAL_CHECK     = 7,

    NPC_KALECGOS_DRAGON         = 24850,            // kalecgos blue dragon hostile
    NPC_KALECGOS_HUMAN          = 24891,            // kalecgos human form in spectral realm
    NPC_SATHROVARR              = 24892,
    NPC_BRUTALLUS               = 24882,
    NPC_FELMYST                 = 25038,
    NPC_ALYTHESS                = 25166,
    NPC_SACROLASH               = 25165,
    NPC_MURU                    = 25741,
    NPC_KILJAEDEN               = 25315,
    NPC_KILJAEDEN_CONTROLLER    = 25608,            // kiljaeden event controller
    NPC_ANVEENA                 = 26046,            // related to kiljaeden event
    NPC_KALECGOS                = 25319,            // related to kiljaeden event

    GO_FORCEFIELD               = 188421,           // kalecgos door + collisions
    GO_BOSS_COLLISION_1         = 188523,
    GO_BOSS_COLLISION_2         = 188524,
    GO_ICE_BARRIER              = 188119,           // use unk; related to brutallus intro
    GO_FIRE_BARRIER             = 188075,           // door after felmyst
    GO_FIRST_GATE               = 187766,           // door between felmyst and eredar twins
    GO_SECOND_GATE              = 187764,           // door after eredar twins
    GO_MURU_ENTER_GATE          = 187990,           // muru gates
    GO_MURU_EXIT_GATE           = 188118,
    GO_THIRD_GATE               = 187765,           // door after muru; why another?

    // spells related to kalecgos event
    SPELL_SPECTRAL_REALM                = 46021,
    SPELL_TELEPORT_NORMAL_REALM         = 46020,
    SPELL_TELEPORT_TO_SPECTRAL_REALM    = 46019,
    SPELL_SPECTRAL_EXHAUSTION           = 44867,
    SPELL_SPECTRAL_REALM_FORCE_FACTION  = 44852
};

class MANGOS_DLL_DECL instance_sunwell_plateau : public ScriptedInstance
{
    public:
        instance_sunwell_plateau(Map* pMap);
        ~instance_sunwell_plateau() {}

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        void SetData64(uint32 uiType, uint64 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiType);

        void Update(uint32 uiDiff);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        void EjectPlayer(Player* pPlayer);
        void EjectPlayers();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        // Creatures
        uint64 m_uiKalecgos_DragonGUID;
        uint64 m_uiKalecgos_HumanGUID;
        uint64 m_uiSathrovarrGUID;
        uint64 m_uiBrutallusGUID;
        uint64 m_uiFelmystGUID;
        uint64 m_uiAlythessGUID;
        uint64 m_uiSacrolashGUID;
        uint64 m_uiMuruGUID;
        uint64 m_uiKilJaedenGUID;
        uint64 m_uiKilJaedenControllerGUID;
        uint64 m_uiAnveenaGUID;
        uint64 m_uiKalecgosGUID;

        // GameObjects
        uint64 m_uiForceFieldGUID;
        uint64 m_uiBossCollision1GUID;
        uint64 m_uiBossCollision2GUID;
        uint64 m_uiIceBarrierGUID;
        uint64 m_uiDoorFireBarrierGUID;
        uint64 m_uiDoorTheFirstGateGUID;
        uint64 m_uiDoorTheSecondGateGUID;
        uint64 m_uiDoorMuruEnterGateGUID;
        uint64 m_uiDoorMuruExitGateGUID;
        uint64 m_uiDoorTheThirdGateGUID;

        // Misc
        uint32 m_uiSpectralRealmTimer;
        GUIDList SpectralRealmList;
};
#endif
