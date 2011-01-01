/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SUNKEN_TEMPLE_H
#define DEF_SUNKEN_TEMPLE_H

enum
{
    MAX_ENCOUNTER         = 4,

    TYPE_ATALARION        = 1,
    TYPE_PROTECTORS       = 2,
    TYPE_JAMMALAIN        = 3,
    TYPE_MALFURION        = 4,

    NPC_ATALARION         = 8580,
    NPC_DREAMSCYTH        = 5721,
    NPC_WEAVER            = 5720,
    NPC_AVATAR_OF_HAKKAR  = 8443,
    NPC_SHADE_OF_ERANIKUS = 5709,

    // Jammalain min-bosses
    NPC_ZOLO              = 5712,
    NPC_GASHER            = 5713,
    NPC_LORO              = 5714,
    NPC_HUKKU             = 5715,
    NPC_ZULLOR            = 5716,
    NPC_MIJAN             = 5717,

    NPC_MALFURION         = 15362,

    GO_ALTAR_OF_HAKKAR    = 148836,

    GO_ATALAI_STATUE_1    = 148830,
    GO_ATALAI_STATUE_2    = 148831,
    GO_ATALAI_STATUE_3    = 148832,
    GO_ATALAI_STATUE_4    = 148833,
    GO_ATALAI_STATUE_5    = 148834,
    GO_ATALAI_STATUE_6    = 148835,

    GO_ETERNAL_FLAME_1    = 148418,
    GO_ETERNAL_FLAME_2    = 148419,
    GO_ETERNAL_FLAME_3    = 148420,
    GO_ETERNAL_FLAME_4    = 148421,

    GO_JAMMALAIN_BARRIER  = 149431
};

class MANGOS_DLL_DECL instance_sunken_temple : public ScriptedInstance
{
    public:
        instance_sunken_temple(Map* pMap);
        ~instance_sunken_temple() {}

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiJammalainBarrierGUID;
        uint8 m_uiProtectorsRemaining;
};
#endif
