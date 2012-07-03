/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_AHNKAHET_H
#define DEF_AHNKAHET_H
/* Encounters
 * Elder Nadox         = 1
 * Prince Taldram      = 2
 * Jedoga Shadowseeker = 3
 * Herald Volazj       = 4
 * Amanitar            = 5
*/
enum
{
    MAX_ENCOUNTER               = 5,
    MAX_INITIATES               = 15,

    TYPE_NADOX                  = 0,
    TYPE_TALDARAM               = 1,
    TYPE_JEDOGA                 = 2,
    TYPE_VOLAZJ                 = 3,
    TYPE_AMANITAR               = 4,

    GO_DOOR_TALDARAM            = 192236,
    GO_ANCIENT_DEVICE_L         = 193093,
    GO_ANCIENT_DEVICE_R         = 193094,
    GO_VORTEX                   = 193564,

    NPC_ELDER_NADOX             = 29309,
    NPC_TALDARAM                = 29308,
    NPC_JEDOGA_SHADOWSEEKER     = 29310,
    NPC_AHNKAHAR_GUARDIAN_EGG   = 30173,
    NPC_AHNKAHAR_SWARM_EGG      = 30172,
    NPC_JEDOGA_CONTROLLER       = 30181,
    NPC_TWILIGHT_INITIATE       = 30114,

    ACHIEV_START_VOLAZJ_ID      = 20382,

    ACHIEV_CRIT_RESPECT_ELDERS  = 7317,             // Nadox, achiev 2038
    ACHIEV_CRIT_VOLUNTEER_WORK  = 7359,             // Jedoga, achiev 2056
};

static const float aTaldaramLandingLoc[4] = {528.734f, -845.998f, 11.54f, 0.68f};
static const float aJedogaLandingLoc[4] = {375.4977f, -707.3635f, -16.094f, 5.42f};

class MANGOS_DLL_DECL instance_ahnkahet : public ScriptedInstance
{
    public:
        instance_ahnkahet(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        ObjectGuid SelectRandomGuardianEggGuid();
        ObjectGuid SelectRandomSwarmerEggGuid();
        ObjectGuid SelectJedogaSacrificeControllerGuid() { return m_jedogaSacrificeController; }

        void GetJedogaControllersList(GuidList &lList) { lList = m_lJedogaControllersGuidList; }
        void GetJedogaEventControllersList(GuidList &lList) {lList = m_lJedogaEventControllersGuidList; }

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        bool m_bRespectElders;
        bool m_bVolunteerWork;

        uint8 m_uiDevicesActivated;
        uint8 m_uiInitiatesKilled;

        ObjectGuid m_jedogaSacrificeController;

        GuidList m_GuardianEggList;
        GuidList m_SwarmerEggList;
        GuidList m_lJedogaControllersGuidList;
        GuidList m_lJedogaEventControllersGuidList;
};

#endif
