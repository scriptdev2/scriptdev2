/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SHADOW_LABYRINTH_H
#define DEF_SHADOW_LABYRINTH_H

enum
{
    MAX_ENCOUNTER           = 5,

    TYPE_HELLMAW            = 1,
    TYPE_OVERSEER           = 2,
    TYPE_INCITER            = 3,
    TYPE_VORPIL             = 4,
    TYPE_MURMUR             = 5,

    NPC_VORPIL              = 18732,
    NPC_FEL_OVERSEER        = 18796,

    GO_REFECTORY_DOOR       = 183296,                       //door opened when blackheart the inciter dies
    GO_SCREAMING_HALL_DOOR  = 183295                        //door opened when grandmaster vorpil dies
};

class MANGOS_DLL_DECL instance_shadow_labyrinth : public ScriptedInstance
{
    public:
        instance_shadow_labyrinth(Map* pMap);

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint32 m_uiFelOverseerCount;
};

#endif
