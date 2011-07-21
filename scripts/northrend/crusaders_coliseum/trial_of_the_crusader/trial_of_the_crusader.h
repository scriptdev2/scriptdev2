/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_TRIAL_OF_THE_CRUSADER_H
#define DEF_TRIAL_OF_THE_CRUSADER_H

enum
{
    MAX_ENCOUNTER               = 6,

    TYPE_WIPE_COUNT             = 0,
    TYPE_NORTHREND_BEASTS       = 1,
    TYPE_JARAXXUS               = 2,
    TYPE_FACTION_CHAMPIONS      = 3,
    TYPE_TWIN_VALKYR            = 4,
    TYPE_ANUBARAK               = 5,

    NPC_BEAST_COMBAT_STALKER    = 36549,
    NPC_GORMOK                  = 34796,
    NPC_ACIDMAW                 = 35144,
    NPC_DREADSCALE              = 34799,
    NPC_ICEHOWL                 = 34797,
    NPC_JARAXXUS                = 34780,
    NPC_FJOLA                   = 34497,
    NPC_EYDIS                   = 34496,
    NPC_ANUBARAK                = 34564,

    NPC_TIRION_A                = 34996,
    NPC_TIRION_B                = 36095,                    // Summoned after his text (Champions, you're alive! Not only have you defeated every challenge of the Trial of the Crusader, but also thwarted Arthas' plans! Your skill and cunning will prove to be a powerful weapon against the Scourge. Well done! Allow one of the Crusade's mages to transport you to the surface!) is said..
    NPC_VARIAN                  = 34990,
    NPC_GARROSH                 = 34995,
    NPC_FIZZLEBANG              = 35458,
    NPC_THE_LICHKING            = 16980,
    NPC_RAMSEY_1                = 34816,
    NPC_RAMSEY_2                = 35035,
    NPC_RAMSEY_3                = 35766,
    NPC_RAMSEY_4                = 35770,
    NPC_RAMSEY_5                = 35771,
    NPC_RAMSEY_6                = 35895,                    // Unknown what these three NPCs are used for, maybe horde events?
    NPC_RAMSEY_7                = 35909,
    NPC_RAMSEY_8                = 35910,
};

static const float aRamsayPositions[2][4] =
{
    {559.1528f, 90.55729f, 395.2734f, 5.078908f},           // Summon Position
    {563.556f,  78.72571f, 395.2125f, 0.0f}                 // Movement Position
};

class MANGOS_DLL_DECL instance_trial_of_the_crusader : public ScriptedInstance, private DialogueHelper
{
    public:
        instance_trial_of_the_crusader(Map* pMap);

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void OnPlayerEnter(Player* pPlayer);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void Update(uint32 uiDiff) { DialogueUpdate(uiDiff); }

    private:
        void DoSummonRamsey(uint32 uiEntry);
        void JustDidDialogueStep(int32 iEntry);

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        Team m_uiTeam;
};

#endif
