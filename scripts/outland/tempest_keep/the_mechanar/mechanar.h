/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_MECHANAR_H
#define DEF_MECHANAR_H

enum
{
    MAX_ENCOUNTER           = 5,
    MAX_BRIDGE_LOCATIONS    = 7,
    MAX_BRIDGE_TRASH        = 4,

    TYPE_GYRO_KILL          = 0,
    TYPE_IRON_HAND          = 1,
    TYPE_CAPACITUS          = 2,
    TYPE_SEPETHREA          = 3,
    TYPE_PATHALEON          = 4,

    NPC_GYRO_KILL           = 19218,
    NPC_IRON_HAND           = 19710,
    NPC_LORD_CAPACITUS      = 19219,
    //NPC_SEPETHREA         = 19221,
    NPC_PATHALEON           = 19220,

    // bridge event related
    NPC_ASTROMAGE           = 19168,
    NPC_PHYSICIAN           = 20990,
    NPC_CENTURION           = 19510,
    NPC_ENGINEER            = 20988,
    NPC_NETHERBINDER        = 20059,
    NPC_FORGE_DESTROYER     = 19735,

    GO_FACTORY_ELEVATOR     = 183788,

    SPELL_SIMPLE_TELEPORT   = 12980,

    SAY_PATHALEON_INTRO     = -1554028,
};

struct SpawnLocation
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static const SpawnLocation aBridgeLocs[MAX_BRIDGE_LOCATIONS] =
{
    {246.29f, -22.41f, 26.32f, 0},
    {207.52f, -23.13f, 24.87f, 0},
    {181.58f, -23.01f, 24.87f, 0},
    {137.87f,  28.67f, 24.87f, 4.65f},
    {137.95f,  52.72f, 24.87f, 4.65f},
    {138.25f,  93.51f, 26.37f, 4.65f},
    {138.44f, 150.28f, 25.57f, 4.63f}           // Pathaleon summon location
};

static const uint32 aBridgeEventNpcs[MAX_BRIDGE_LOCATIONS][MAX_BRIDGE_TRASH] =
{
    {NPC_ASTROMAGE,       NPC_ASTROMAGE, NPC_PHYSICIAN, NPC_CENTURION},
    {NPC_FORGE_DESTROYER, 0, 0, 0},
    {NPC_ENGINEER,        NPC_PHYSICIAN, NPC_ENGINEER,  0},
    {NPC_ASTROMAGE,       NPC_PHYSICIAN, NPC_ENGINEER,  0},
    {NPC_FORGE_DESTROYER, 0, 0, 0},
    {NPC_PHYSICIAN,       NPC_ASTROMAGE, NPC_ENGINEER,  NPC_NETHERBINDER},
    {NPC_PATHALEON,       0, 0, 0}
};

class MANGOS_DLL_DECL instance_mechanar : public ScriptedInstance
{
    public:
        instance_mechanar(Map* pMap);

        void Initialize();

        void OnPlayerEnter(Player* pPlayer);
        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void Update(uint32 uiDiff);

    private:
        void DoSpawnBridgeWave();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint32 m_uiBridgeEventTimer;
        uint8 m_uiBridgeEventPhase;

        GUIDSet m_sBridgeTrashGuidSet;
};

#endif
