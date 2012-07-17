/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_EYE_ETERNITY_H
#define DEF_EYE_ETERNITY_H

enum
{
    TYPE_MALYGOS                    = 0,

    NPC_MALYGOS                     = 28859,
    NPC_ALEXSTRASZA                 = 32295,
    NPC_LARGE_TRIGGER               = 22517,

    GO_EXIT_PORTAL                  = 193908,
    GO_PLATFORM                     = 193070,

    GO_HEART_OF_MAGIC               = 194158,
    GO_HEART_OF_MAGIC_H             = 194159,
    GO_ALEXSTRASZAS_GIFT            = 193905,
    GO_ALEXSTRASZAS_GIFT_H          = 193967,

    ACHIEV_START_MALYGOS_ID         = 20387,
};

class MANGOS_DLL_DECL instance_eye_of_eternity : public ScriptedInstance
{
    public:
        instance_eye_of_eternity(Map* pMap);
        ~instance_eye_of_eternity() {}

        void Initialize();

        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);

    protected:
        uint32 m_uiEncounter;
};

#endif
