/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_OCULUS_H
#define DEF_OCULUS_H

/* Encounters
 * Drakos           = 0
 * Varos            = 1
 * Urom             = 2
 * Eregos           = 3
 */

enum
{
    MAX_ENCOUNTER                   = 4,

    TYPE_DRAKOS                     = 0,
    TYPE_VAROS                      = 1,
    TYPE_UROM                       = 2,
    TYPE_EREGOS                     = 3,

    NPC_DRAKOS                      = 27654,
    NPC_VAROS                       = 27447,
    NPC_UROM                        = 27655,
    NPC_EREGOS                      = 27656,

    NPC_ETERNOS                     = 27659,    // bronze
    NPC_VERDISA                     = 27657,    // emerald
    NPC_BELGARISTRASZ               = 27658,    // ruby

    // Cages in which the friendly dragons are hold
    GO_DRAGON_CAGE_DOOR             = 193995,

    // Loot
    GO_CACHE_EREGOS                 = 191349,
    GO_CACHE_EREGOS_H               = 193603,

    // Yells after Drakos dies
    SAY_VAROS_INTRO                 = -1578020,
    SAY_BELGARISTRASZ_GREET         = -1578021,

    // world states to show how many constructs are still alive
    WORLD_STATE_CONSTRUCTS          = 3524,
    WORLD_STATE_CONSTRUCTS_COUNT    = 3486,

    ACHIEV_START_EREGOS_ID          = 18153,            // eregos timed kill achiev
};

class MANGOS_DLL_DECL instance_oculus : public ScriptedInstance
{
    public:
        instance_oculus(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;
};

#endif
