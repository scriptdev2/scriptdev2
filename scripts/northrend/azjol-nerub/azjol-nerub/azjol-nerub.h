/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_AZJOL_NERUB_H
#define DEF_AZJOL_NERUB_H

enum
{
    MAX_ENCOUNTER               = 3,

    TYPE_KRIKTHIR               = 0,
    TYPE_HADRONOX               = 1,
    TYPE_ANUBARAK               = 2,

    NPC_KRIKTHIR                = 28684,

    SAY_SEND_GROUP_1            = -1601004,
    SAY_SEND_GROUP_2            = -1601005,
    SAY_SEND_GROUP_3            = -1601006,

    NPC_GASHRA                  = 28730,
    NPC_NARJIL                  = 28729,
    NPC_SILTHIK                 = 28731,

    GO_DOOR_KRIKTHIR            = 192395,
    GO_DOOR_ANUBARAK_1          = 192396,
    GO_DOOR_ANUBARAK_2          = 192397,
    GO_DOOR_ANUBARAK_3          = 192398,

    ACHIEV_START_ANUB_ID        = 20381,
};

static const uint32 aWatchers[] = {NPC_GASHRA, NPC_NARJIL, NPC_SILTHIK};

class MANGOS_DLL_DECL instance_azjol_nerub : public ScriptedInstance
{
    public:
        instance_azjol_nerub(Map* pMap);

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void Update(uint32 uiDiff);

    private:
        void DoSendWatcherOrKrikthir();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        ObjectGuid m_playerGuid;

        uint32 m_uiWatcherTimer;
};
#endif
