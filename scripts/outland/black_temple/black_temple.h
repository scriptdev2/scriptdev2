/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BLACK_TEMPLE_H
#define DEF_BLACK_TEMPLE_H

enum
{
    MAX_ENCOUNTER                   = 9,

    TYPE_NAJENTUS                   = 0,
    TYPE_SUPREMUS                   = 1,
    TYPE_SHADE                      = 2,
    TYPE_GOREFIEND                  = 3,
    TYPE_BLOODBOIL                  = 4,
    TYPE_RELIQUIARY                 = 5,
    TYPE_SHAHRAZ                    = 6,
    TYPE_COUNCIL                    = 7,
    TYPE_ILLIDAN                    = 8,

    NPC_WARLORD_NAJENTUS            = 22887,
    NPC_SUPREMUS                    = 22898,
    NPC_SHADE_OF_AKAMA              = 22841,
    NPC_AKAMA_SHADE                 = 22990,
    NPC_ILLIDARI_COUNCIL            = 23426,
    NPC_COUNCIL_VOICE               = 23499,
    NPC_LADY_MALANDE                = 22951,
    NPC_ZEREVOR                     = 22950,
    NPC_GATHIOS                     = 22949,
    NPC_VERAS                       = 22952,
    NPC_AKAMA                       = 23089,
    NPC_ILLIDAN_STORMRAGE           = 22917,

    GO_NAJENTUS_GATE                = 185483,
    GO_SUPREMUS_DOORS               = 185882,
    GO_SHADE_OF_AKAMA               = 185478,
    GO_PRE_SHAHRAZ_DOOR             = 185479,
    GO_POST_SHAHRAZ_DOOR            = 185482,
    GO_PRE_COUNCIL_DOOR             = 185481,
    GO_COUNCIL_DOOR                 = 186152,
    GO_ILLIDAN_GATE                 = 185905,
    GO_ILLIDAN_DOOR_R               = 186261,
    GO_ILLIDAN_DOOR_L               = 186262,
};

class MANGOS_DLL_DECL instance_black_temple : public ScriptedInstance
{
    public:
        instance_black_temple(Map* pMap);

        void Initialize();

        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        bool CanPreMotherDoorOpen();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint64 m_uiNajentusGUID;
        uint64 m_uiAkamaGUID;                               // This is the Akama that starts the Illidan encounter.
        uint64 m_uiAkama_ShadeGUID;                         // This is the Akama that starts the Shade of Akama encounter.
        uint64 m_uiShadeOfAkamaGUID;
        uint64 m_uiSupremusGUID;
        uint64 m_uiLadyMalandeGUID;
        uint64 m_uiGathiosTheShattererGUID;
        uint64 m_uiHighNethermancerZerevorGUID;
        uint64 m_uiVerasDarkshadowGUID;
        uint64 m_uiIllidariCouncilGUID;
        uint64 m_uiBloodElfCouncilVoiceGUID;
        uint64 m_uiIllidanStormrageGUID;

        uint64 m_uiNajentusGateGUID;
        uint64 m_uiMainTempleDoorsGUID;
        uint64 m_uiShadeAkamaDoorGUID;
        uint64 m_uiIllidanGateGUID;
        uint64 m_uiIllidanDoorGUID[2];
        uint64 m_uiShahrazPreDoorGUID;
        uint64 m_uiShahrazPostDoorGUID;
        uint64 m_uiPreCouncilDoorGUID;
        uint64 m_uiCouncilDoorGUID;
};

#endif
