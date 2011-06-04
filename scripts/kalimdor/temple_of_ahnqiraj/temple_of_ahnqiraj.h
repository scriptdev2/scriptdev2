/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_TEMPLE_OF_AHNQIRAJ_H
#define DEF_TEMPLE_OF_AHNQIRAJ_H

enum
{
    MAX_ENCOUNTER               = 4,

    TYPE_SKERAM                 = 0,
    TYPE_VEM                    = 1,
    TYPE_TWINS                  = 2,
    TYPE_CTHUN_PHASE            = 3,

    // NPC_SKERAM               = 15263,
    NPC_KRI                     = 15511,
    NPC_VEM                     = 15544,
    NPC_VEKLOR                  = 15276,
    NPC_VEKNILASH               = 15275,
    NPC_CTHUN                   = 15727,

    GO_SKERAM_GATE              = 180636,
    GO_TWINS_ENTER_DOOR         = 180634,
    GO_TWINS_EXIT_DOOR          = 180635,

    DATA_BUG_TRIO_DEATH         = 10,
};

enum CThunPhase
{
    PHASE_NOT_STARTED           = 0,
    PHASE_EYE_NORMAL            = 1,
    PHASE_EYE_DARK_GLARE        = 2,
    PHASE_TRANSITION            = 3,
    PHASE_CTHUN                 = 4,
    PHASE_CTHUN_WEAKENED        = 5,
    PHASE_FINISH                = 6,
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

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint32 m_uiBugTrioDeathCount;
};

#endif
