/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_RFK_H
#define DEF_RFK_H

enum
{
    MAX_ENCOUNTER     = 1,

    TYPE_AGATHELOS    = 1,

    GO_AGATHELOS_WARD = 21099,
    
    NPC_WARD_KEEPER   = 4625
};

class MANGOS_DLL_DECL instance_razorfen_kraul : public ScriptedInstance
{
    public:
        instance_razorfen_kraul(Map* pMap);
        ~instance_razorfen_kraul() {}

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

        uint64 m_uiAgathelosWardGUID;
        uint8 m_uiWardKeepersRemaining;
};
#endif
