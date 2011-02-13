/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_TEMPLE_OF_AHNQIRAJ_H
#define DEF_TEMPLE_OF_AHNQIRAJ_H

enum
{
    MAX_ENCOUNTER               = 3,

    TYPE_SKERAM                 = 0,
    TYPE_VEM                    = 1,
    TYPE_TWINS                  = 2,

    NPC_SKERAM                  = 15263,
    NPC_KRI                     = 15511,
    NPC_VEM                     = 15544,
    NPC_VEKLOR                  = 15276,
    NPC_VEKNILASH               = 15275,

    GO_SKERAM_GATE              = 180636,
    GO_TWINS_ENTER_DOOR         = 180634,
    GO_TWINS_EXIT_DOOR          = 180635,

    DATA_BUG_TRIO_DEATH         = 10,

    TYPE_CTHUN_PHASE            = 20
};

class MANGOS_DLL_DECL instance_temple_of_ahnqiraj : public ScriptedInstance
{
    public:
        instance_temple_of_ahnqiraj(Map* pMap);

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
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        // Storing Skeram, Vem and Kri.
        uint64 m_uiSkeramGUID;
        uint64 m_uiVemGUID;
        uint64 m_uiKriGUID;
        uint64 m_uiVeklorGUID;
        uint64 m_uiVeknilashGUID;

        // Doors
        uint64 m_uiSkeramGateGUID;
        uint64 m_uiTwinsEnterDoorGUID;
        uint64 m_uiTwinsExitDoorGUID;

        uint32 m_uiBugTrioDeathCount;

        uint32 m_uiCthunPhase;
};

#endif
