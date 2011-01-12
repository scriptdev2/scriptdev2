/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_MOLTEN_CORE_H
#define DEF_MOLTEN_CORE_H

enum
{
    MAX_ENCOUNTER               = 10,

    TYPE_LUCIFRON               = 0,
    TYPE_MAGMADAR               = 1,
    TYPE_GEHENNAS               = 2,
    TYPE_GARR                   = 3,
    TYPE_SHAZZRAH               = 4,
    TYPE_GEDDON                 = 5,
    TYPE_GOLEMAGG               = 6,
    TYPE_SULFURON               = 7,
    TYPE_MAJORDOMO              = 8,
    TYPE_RAGNAROS               = 9,

    NPC_LUCIFRON                = 12118,
    NPC_MAGMADAR                = 11982,
    NPC_GEHENNAS                = 12259,
    NPC_GARR                    = 12057,
    NPC_SHAZZRAH                = 12264,
    NPC_GEDDON                  = 12056,
    NPC_GOLEMAGG                = 11988,
    NPC_SULFURON                = 12098,
    NPC_MAJORDOMO               = 12018,
    NPC_RAGNAROS                = 11502,

    // Adds
    // Used for respawn in case of wipe
    NPC_FLAMEWAKER_PROTECTOR    = 12119,                    // Lucifron
    NPC_FLAMEWAKER              = 11661,                    // Gehennas
    NPC_FIRESWORN               = 12099,                    // Garr
    NPC_CORE_RAGER              = 11672,                    // Golemagg
    NPC_FLAMEWAKER_PRIEST       = 11662,                    // Sulfuron
    NPC_FLAMEWAKER_HEALER       = 11663,                    // Majordomo
    NPC_FLAMEWAKER_ELITE        = 11664,                    // Majordomo

    GO_CACHE_OF_THE_FIRE_LORD   = 179703,
    GO_RUNE_KRESS               = 176956,                   // Magmadar
    GO_RUNE_MOHN                = 176957,                   // Gehennas
    GO_RUNE_BLAZ                = 176955,                   // Garr
    GO_RUNE_MAZJ                = 176953,                   // Shazzah
    GO_RUNE_ZETH                = 176952,                   // Geddon
    GO_RUNE_THERI               = 176954,                   // Golemagg
    GO_RUNE_KORO                = 176951,                   // Sulfuron

    MAX_MAJORDOMO_ADDS          = 8,
    FACTION_MAJORDOMO_FRIENDLY  = 1080,
    SAY_MAJORDOMO_SPAWN         = -1409004,
};

struct sSpawnLocation
{
    uint32 m_uiEntry;
    float m_fX, m_fY, m_fZ, m_fO;
};

static sSpawnLocation m_aBosspawnLocs[MAX_MAJORDOMO_ADDS] =
{
    {NPC_FLAMEWAKER_ELITE,  737.945f, -1156.48f, -118.945f, 4.46804f},
    {NPC_FLAMEWAKER_ELITE,  752.520f, -1191.02f, -118.218f, 2.49582f},
    {NPC_FLAMEWAKER_ELITE,  752.953f, -1163.94f, -118.869f, 3.70010f},
    {NPC_FLAMEWAKER_ELITE,  738.814f, -1197.40f, -118.018f, 1.83260f},
    {NPC_FLAMEWAKER_HEALER, 746.939f, -1194.87f, -118.016f, 2.21657f},
    {NPC_FLAMEWAKER_HEALER, 747.132f, -1158.87f, -118.897f, 4.03171f},
    {NPC_FLAMEWAKER_HEALER, 757.116f, -1170.12f, -118.793f, 3.40339f},
    {NPC_FLAMEWAKER_HEALER, 755.910f, -1184.46f, -118.449f, 2.80998f}
};

static sSpawnLocation m_aMajordomoLocations[2] =
{
    {NPC_MAJORDOMO, 758.089f, -1176.71f, -118.640f, 3.12414f},  // Summon fight position
    {NPC_MAJORDOMO, 847.103f, -816.153f, -229.775f, 4.344f}     // Summon and teleport location (near Ragnaros)
};

static const float RANGE_CALL_FOR_HELP     = 20.0f;

typedef std::list<uint64> GUIDList;

class MANGOS_DLL_DECL instance_molten_core : public ScriptedInstance
{
    public:
        instance_molten_core(Map* pMap);
        ~instance_molten_core() {}

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);
        void OnPlayerEnter(Player* pPlayer);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        // TODO Remove this, when creature linking implemented in MaNGOS
        void DoHandleAdds(GUIDList &m_luiAddsGUIDs, bool bRespawn = true);

    protected:
        void DoSpawnMajordomoIfCan(bool bByPlayerEnter);

        std::string m_strInstData;
        uint32 m_auiEncounter[MAX_ENCOUNTER];

        // Creatures
        uint64 m_uiGarrGUID;
        uint64 m_uiSulfuronGUID;
        uint64 m_uiMajordomoGUID;
        uint64 m_uiRagnarosGUID;

        // Runes
        uint64 m_uiRuneKoroGUID;
        uint64 m_uiRuneZethGUID;
        uint64 m_uiRuneMazjGUID;
        uint64 m_uiRuneTheriGUID;
        uint64 m_uiRuneBlazGUID;
        uint64 m_uiRuneKressGUID;
        uint64 m_uiRuneMohnGUID;

        // Chests
        uint64 m_uiFirelordCacheGUID;

        // Adds lists
        GUIDList m_luiProtectorGUIDs;
        GUIDList m_luiFlamewakerGUIDs;
        GUIDList m_luiFireswornGUIDs;
        GUIDList m_luiPriestGUIDs;
        GUIDList m_luiRagerGUIDs;
};

#endif
