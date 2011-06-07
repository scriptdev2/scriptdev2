/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_RUINS_OF_AHNQIRAJ_H
#define DEF_RUINS_OF_AHNQIRAJ_H

enum
{
    MAX_ENCOUNTER               = 6,
    MAX_HELPERS                 = 5,

    TYPE_KURINNAXX              = 0,
    TYPE_RAJAXX                 = 1,
    TYPE_MOAM                   = 2,
    TYPE_BURU                   = 3,
    TYPE_AYAMISS                = 4,
    TYPE_OSSIRIAN               = 5,

    NPC_KURINNAXX               = 15348,
    NPC_MOAM                    = 15340,
    NPC_BURU                    = 15370,
    NPC_AYAMISS                 = 15369,
    NPC_OSSIRIAN                = 15339,
    NPC_GENERAL_ANDOROV         = 15471,                    // The general and the kaldorei are escorted for the rajaxx encounter
    NPC_KALDOREI_ELITE          = 15473,
    NPC_RAJAXX                  = 15341,                    // All of the following are used in the rajaxx encounter
    // NPC_COLONEL_ZERRAN       = 15385,
    // NPC_MAJOR_PAKKON         = 15388,
    // NPC_MAJOR_YEGGETH        = 15386,
    // NPC_CAPTAIN_XURREM       = 15390,
    // NPC_CAPTAIN_DRENN        = 15389,
    // NPC_CAPTAIN_TUUBID       = 15392,
    // NPC_CAPTAIN_QEEZ         = 15391,

    GO_OSSIRIAN_CRYSTAL         = 180619,                   // Used in the ossirian encounter

    SAY_OSSIRIAN_INTRO          = -1509022                  // Yelled after Kurinnax dies
};

struct SpawnLocation
{
    uint32 m_uiEntry;
    float m_fX, m_fY, m_fZ, m_fO;
};

static  const SpawnLocation aAndorovSpawnLocs[MAX_HELPERS] =
{
    {NPC_GENERAL_ANDOROV, -8660.4f,  1510.29f, 32.449f,  2.2184f},
    {NPC_KALDOREI_ELITE,  -8655.84f, 1509.78f, 32.462f,  2.33341f},
    {NPC_KALDOREI_ELITE,  -8657.39f, 1506.28f, 32.418f,  2.33346f},
    {NPC_KALDOREI_ELITE,  -8660.96f, 1504.9f,  32.1567f, 2.33306f},
    {NPC_KALDOREI_ELITE,  -8664.45f, 1506.44f, 32.0944f, 2.33302f}
};

class MANGOS_DLL_DECL instance_ruins_of_ahnqiraj : public ScriptedInstance
{
    public:
        instance_ruins_of_ahnqiraj(Map* pMap);
        ~instance_ruins_of_ahnqiraj() {}

        void Initialize();

        // bool IsEncounterInProgress() const;              // not active in AQ20

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);
        void OnPlayerEnter(Player* pPlayer);

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        void DoSapwnAndorovIfCan();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;
};
#endif
