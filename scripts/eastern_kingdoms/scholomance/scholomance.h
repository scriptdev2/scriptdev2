/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SCHOLOMANCE_H
#define DEF_SCHOLOMANCE_H

enum
{
    MAX_ENCOUNTER           = 10,

    TYPE_KIRTONOS           = 0,
    TYPE_RATTLEGORE         = 1,
    TYPE_RAS_FROSTWHISPER   = 2,
    TYPE_MALICIA            = 3,
    TYPE_THEOLEN            = 4,
    TYPE_POLKELT            = 5,
    TYPE_RAVENIAN           = 6,
    TYPE_ALEXEI_BAROV       = 7,
    TYPE_ILLUCIA_BAROV      = 8,
    TYPE_GANDLING           = 9,

    NPC_KIRTONOS            = 10506,
    NPC_RATTLEGORE          = 11622,
    NPC_RAS_FROSTWHISPER    = 10508,
    NPC_THEOLEN_KRASTINOV   = 11261,
    NPC_LOREKEEPER_POLKELT  = 10901,
    NPC_RAVENIAN            = 10507,
    NPC_ILLUCIA_BAROV       = 10502,
    NPC_ALEXEI_BAROV        = 10504,
    NPC_INSTRUCTOR_MALICIA  = 10505,
    NPC_DARKMASTER_GANDLING = 1853,

    GO_GATE_KIRTONOS        = 175570,
    GO_VIEWING_ROOM_DOOR    = 175167,                       // Must be opened in reload case
    GO_GATE_RAS             = 177370,
    GO_GATE_MALICIA         = 177375,
    GO_GATE_THEOLEN         = 177377,
    GO_GATE_POLKELT         = 177376,
    GO_GATE_RAVENIAN        = 177372,
    GO_GATE_BAROV           = 177373,
    GO_GATE_ILLUCIA         = 177371,
    GO_GATE_GANDLING        = 177374,

    SAY_GANDLING_SPAWN      = -1289000,
};

struct sSpawnLocation
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static const sSpawnLocation m_aGandlingSpawnLocs[1] =
{
    {180.73f, -9.43856f, 75.507f, 1.61399f}
};

class MANGOS_DLL_DECL instance_scholomance : public ScriptedInstance
{
    public:
        instance_scholomance(Map* pMap);
        ~instance_scholomance() {}

        void Initialize();

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);
        void OnPlayerEnter(Player* pPlayer);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        void DoSpawnGandlingIfCan(bool bByPlayerEnter);

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint64 m_uiDarkmasterGandlingGUID;

        uint64 m_uiGateKirtonosGUID;
        uint64 m_uiGateRasGUID;
        uint64 m_uiGateMiliciaGUID;
        uint64 m_uiGateTheolenGUID;
        uint64 m_uiGatePolkeltGUID;
        uint64 m_uiGateRavenianGUID;
        uint64 m_uiGateBarovGUID;
        uint64 m_uiGateIlluciaGUID;
        uint64 m_uiGateGandlingGUID;
};

#endif
