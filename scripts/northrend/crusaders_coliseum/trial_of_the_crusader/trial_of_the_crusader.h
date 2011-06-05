/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_TRIAL_OF_THE_CRUSADER_H
#define DEF_TRIAL_OF_THE_CRUSADER_H

enum
{
    MAX_ENCOUNTER               = 5,

    TYPE_NORTHREND_BEASTS       = 0,
    TYPE_JARAXXUS               = 1,
    TYPE_FACTION_CHAMPIONS      = 2,
    TYPE_TWIN_VALKYR            = 3,
    TYPE_ANUBARAK               = 4,

    NPC_GORMOK                  = 34796,
    NPC_ACIDMAW                 = 35144,
    NPC_DREADSCALE              = 34799,
    NPC_ICEHOWL                 = 34797,
    NPCJARAXXUS                 = 34780,
    NPC_FJOLA                   = 34497,
    NPC_EYDIS                   = 34496,
    NPC_ANUBARAK                = 34564,
};

class MANGOS_DLL_DECL instance_trial_of_the_crusader : public ScriptedInstance
{
    public:
        instance_trial_of_the_crusader(Map* pMap);

        void Initialize();
        bool IsEncounterInProgress() const;

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;
};

#endif
