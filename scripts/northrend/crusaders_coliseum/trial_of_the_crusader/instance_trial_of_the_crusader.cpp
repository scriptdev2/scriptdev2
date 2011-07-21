/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: instance_trial_of_the_crusader
SD%Complete: 100
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

/* Trial Of The Crusader encounters:
0 - Wipe Count
1 - Northrend Beasts
2 - Jaraxxus
3 - Faction Champions
4 - Twin Valkyr
5 - Anubarak
*/

enum
{
    SAY_TIRION_RAID_INTRO_LONG          = -1649000,
    SAY_RAID_TRIALS_INTRO               = -1649001,

    // Northrend Beasts
    SAY_TIRION_BEAST_1                  = -1649002,
    SAY_VARIAN_BEAST_1                  = -1649003,
    SAY_GARROSH_BEAST_1                 = -1649004,
    SAY_TIRION_BEAST_2                  = -1649005,
    SAY_TIRION_BEAST_3                  = -1649006,
    SAY_TIRION_BEAST_SLAY               = -1649007,
    SAY_TIRION_BEAST_WIPE               = -1649008,

    // Jaraxxus Encounter
    SAY_TIRION_JARAXXUS_INTRO_1         = -1649009,
    SAY_WILFRED_JARAXXUS_INTRO_1        = -1649010,
    SAY_WILFRED_JARAXXUS_INTRO_2        = -1649011,
    SAY_WILFRED_JARAXXUS_INTRO_3        = -1649012,
    SAY_JARAXXUS_JARAXXAS_INTRO_1       = -1649013,
    SAY_WILFRED_DEATH                   = -1649014,
    SAY_TIRION_JARAXXUS_INTRO_2         = -1649015,
    SAY_JARAXXUS_DEATH                  = -1649043,
    SAY_TIRION_JARAXXUS_EXIT_1          = -1649016,
    SAY_GARROSH_JARAXXUS_EXIT_1         = -1649017,
    SAY_VARIAN_JARAXXUS_SLAY            = -1649018,
    SAY_TIRION_JARAXXUS_EXIT_2          = -1649019,

    // Faction-Champions
    SAY_TIRION_PVP_INTRO_1              = -1649020,
    SAY_GARROSH_PVP_A_INTRO_1           = -1649021,
    SAY_VARIAN_PVP_H_INTRO_1            = -1649022,
    SAY_TIRION_PVP_INTRO_2              = -1649023,
    SAY_VARIAN_PVP_H_INTRO_2            = -1649024,
    SAY_GARROSH_PVP_A_INTRO_2           = -1649025,
    SAY_VARIAN_PVP_A_WIN                = -1649026,
    SAY_GARROSH_PVP_H_WIN               = -1649027,
    SAY_TIRION_PVP_WIN                  = -1649028,

    // Twin Valkyrs
    SAY_TIRION_TWINS_INTRO              = -1649029,
    SAY_RAID_INTRO_SHORT                = -1649030,
    SAY_VARIAN_TWINS_A_WIN              = -1649031,
    SAY_GARROSH_TWINS_H_WIN             = -1649032,
    SAY_TIRION_TWINS_WIN                = -1649033,

    // Anub'Arak Encounter
    SAY_LKING_ANUB_INTRO_1              = -1649034,
    SAY_TIRION_ABUN_INTRO_1             = -1649035,
    SAY_LKING_ANUB_INTRO_2              = -1649036,
    SAY_LKING_ANUB_INTRO_3              = -1649037,
    SAY_ANUB_ANUB_INTRO_1               = -1649038,         // NYI
};

static const DialogueEntryTwoSide aTocDialogues[] =
{
    // Beasts related, summons in between not handled here
    {TYPE_NORTHREND_BEASTS, 0, 0, 0,                        24000},
    {SAY_TIRION_BEAST_1,            NPC_TIRION_A, 0, 0,     12000},
    {SAY_VARIAN_BEAST_1,            NPC_VARIAN,         SAY_GARROSH_BEAST_1,        NPC_GARROSH,  0},
    {SAY_TIRION_BEAST_SLAY,         NPC_TIRION_A, 0, 0,     8000},
    {NPC_RAMSEY_2, 0, 0, 0,                                 0},
    {SAY_TIRION_BEAST_WIPE,         NPC_TIRION_A, 0, 0,     8000},
    {NPC_RAMSEY_1, 0, 0, 0,                                 0},
    // Jaruxxus (Intro)
    {TYPE_JARAXXUS, 0, 0, 0,                                1000},
    {SAY_TIRION_JARAXXUS_INTRO_1,   NPC_TIRION_A, 0, 0,     6000},
    {NPC_FIZZLEBANG, 0, 0, 0,                               26000},
    {SAY_WILFRED_JARAXXUS_INTRO_1,  NPC_FIZZLEBANG, 0, 0,   10000},
    {SAY_WILFRED_JARAXXUS_INTRO_2,  NPC_FIZZLEBANG, 0, 0,   12000},
    {SAY_WILFRED_JARAXXUS_INTRO_3,  NPC_FIZZLEBANG, 0, 0,   12000}, // Summon also Jaraxxus
    {SAY_JARAXXUS_JARAXXAS_INTRO_1, NPC_JARAXXUS, 0, 0,     5000},
    {SAY_WILFRED_DEATH,             NPC_FIZZLEBANG, 0, 0,   1000},
    {1, 0, 0, 0,                                            5000},  // Kill Fizzlebang // TODO Either skip this line, or use something better than '1' as entry
    {SAY_TIRION_JARAXXUS_INTRO_2,   NPC_TIRION_A, 0, 0,     0},
    // Jaruxxus (Outro)
    {SAY_JARAXXUS_DEATH,            NPC_JARAXXUS, 0, 0,     6000},  // Jaraxxus Death
    {SAY_TIRION_JARAXXUS_EXIT_1,    NPC_TIRION_A, 0, 0,     5000},
    {SAY_GARROSH_JARAXXUS_EXIT_1,   NPC_GARROSH, 0, 0,      11000},
    {SAY_VARIAN_JARAXXUS_SLAY,      NPC_VARIAN, 0, 0,       8000},
    {SAY_TIRION_JARAXXUS_EXIT_2,    NPC_TIRION_A, 0, 0,     19000},
    {NPC_RAMSEY_3, 0, 0, 0,                                 0},
    // Grand Champions
    {SAY_TIRION_PVP_INTRO_1,        NPC_TIRION_A, 0, 0,     9000},
    {SAY_GARROSH_PVP_A_INTRO_1,     NPC_GARROSH,        SAY_VARIAN_PVP_H_INTRO_1,   NPC_VARIAN, 14000},
    {SAY_TIRION_PVP_INTRO_2,        NPC_TIRION_A, 0, 0,     1000},
    {TYPE_FACTION_CHAMPIONS, 0, 0, 0,                       5000},
    {SAY_GARROSH_PVP_A_INTRO_2,     NPC_GARROSH,        SAY_VARIAN_PVP_H_INTRO_2,   NPC_VARIAN, 0},
    {SAY_VARIAN_PVP_A_WIN,          NPC_VARIAN,         SAY_GARROSH_PVP_H_WIN,      NPC_GARROSH, 4000},
    {SAY_TIRION_PVP_WIN,            NPC_TIRION_A, 0, 0,     27000},
    {NPC_RAMSEY_4, 0, 0, 0,                                 0},
    // Twin Valkyrs
    {TYPE_TWIN_VALKYR, 0, 0, 0,                             17000},
    {NPC_FJOLA, 0, 0, 0,                                    0},     // Summon Twins
    {NPC_EYDIS, 0, 0, 0,                                    2000},  // Twins killed // TODO Find better entry
    {NPC_RAMSEY_5, 0, 0, 0,                                 4000},
    {SAY_VARIAN_TWINS_A_WIN,        NPC_VARIAN,         SAY_GARROSH_TWINS_H_WIN,    NPC_GARROSH, 1000},
    {SAY_TIRION_TWINS_WIN,          NPC_TIRION_A, 0, 0,     0},
    // Anub'arak
    {TYPE_ANUBARAK, 0, 0, 0,                                19000},
    {SAY_LKING_ANUB_INTRO_1,        NPC_THE_LICHKING, 0, 0, 9000},
    {SAY_TIRION_ABUN_INTRO_1,       NPC_TIRION_A, 0, 0,     6000},
    {SAY_LKING_ANUB_INTRO_2,        NPC_THE_LICHKING, 0, 0, 19000},
    {SAY_LKING_ANUB_INTRO_3,        NPC_THE_LICHKING, 0, 0, 0},
    {0, 0, 0, 0 ,0}
};

instance_trial_of_the_crusader::instance_trial_of_the_crusader(Map* pMap) : ScriptedInstance(pMap), DialogueHelper(aTocDialogues),
    m_uiTeam(TEAM_NONE)
{
    Initialize();
}

void instance_trial_of_the_crusader::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    InitializeDialogueHelper(this);
}

bool instance_trial_of_the_crusader::IsEncounterInProgress() const
{
    for (uint8 i = TYPE_NORTHREND_BEASTS; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_trial_of_the_crusader::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_TIRION_A:
        case NPC_TIRION_B:
        case NPC_VARIAN:
        case NPC_GARROSH:
        case NPC_FIZZLEBANG:
        case NPC_JARAXXUS:
        case NPC_THE_LICHKING:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
    }
}

void instance_trial_of_the_crusader::OnObjectCreate(GameObject* pGo)
{

}

void instance_trial_of_the_crusader::OnPlayerEnter(Player* pPlayer)
{
    if (m_uiTeam)
        return;

    m_uiTeam = pPlayer->GetTeam();
    SetDialogueSide(m_uiTeam == ALLIANCE);

    DoSummonRamsey(0);
}

void instance_trial_of_the_crusader::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_WIPE_COUNT:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_NORTHREND_BEASTS:
            if (uiData == SPECIAL)
            {
                if (Creature* pTirion = GetSingleCreatureFromStorage(NPC_TIRION_A))
                    DoScriptText(m_auiEncounter[uiType] != FAIL ? SAY_TIRION_RAID_INTRO_LONG : SAY_RAID_TRIALS_INTRO, pTirion);
                StartNextDialogueText(TYPE_NORTHREND_BEASTS);
            }
            else if (uiData == FAIL)
            {
                SetData(TYPE_WIPE_COUNT, m_auiEncounter[TYPE_WIPE_COUNT] + 1);
                StartNextDialogueText(SAY_TIRION_BEAST_WIPE);
            }
            else if (uiData == DONE)
                StartNextDialogueText(SAY_TIRION_BEAST_SLAY);
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_JARAXXUS:
            if (uiData == SPECIAL)
                // TODO - What happen in wipe case?
                StartNextDialogueText(TYPE_JARAXXUS);
            else if (uiData == FAIL)
            {
                SetData(TYPE_WIPE_COUNT, m_auiEncounter[TYPE_WIPE_COUNT] + 1);
                StartNextDialogueText(NPC_RAMSEY_2);
            }
            else if (uiData == DONE)
                StartNextDialogueText(SAY_JARAXXUS_DEATH);
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_FACTION_CHAMPIONS:
            if (uiData == SPECIAL)
                StartNextDialogueText(m_auiEncounter[uiType] != FAIL ? SAY_TIRION_PVP_INTRO_1 : TYPE_FACTION_CHAMPIONS);
            else if (uiData == FAIL)
            {
                SetData(TYPE_WIPE_COUNT, m_auiEncounter[TYPE_WIPE_COUNT] + 1);
                StartNextDialogueText(NPC_RAMSEY_3);
            }
            else if (uiData == DONE)
                StartNextDialogueText(SAY_VARIAN_PVP_A_WIN);
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_TWIN_VALKYR:
            if (uiData == SPECIAL)
            {
                if (Creature* pTirion = GetSingleCreatureFromStorage(NPC_TIRION_A))
                    DoScriptText(m_auiEncounter[uiType] != FAIL ? SAY_TIRION_TWINS_INTRO : SAY_RAID_INTRO_SHORT, pTirion);

            }
            else if (uiData == FAIL)
            {
                SetData(TYPE_WIPE_COUNT, m_auiEncounter[TYPE_WIPE_COUNT] + 1);
                StartNextDialogueText(NPC_RAMSEY_4);
            }
            else if (uiData == DONE)
                StartNextDialogueText(NPC_EYDIS);
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_ANUBARAK:
            if (uiData == SPECIAL)
                StartNextDialogueText(TYPE_ANUBARAK);
            else if (uiData == FAIL)
                SetData(TYPE_WIPE_COUNT, m_auiEncounter[TYPE_WIPE_COUNT] + 1);
            m_auiEncounter[uiType] = uiData;
            break;
        default:
            error_log("SD2: Instance Trial of The Crusader: ERROR SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
            return;
    }

    if (uiData == DONE || uiType == TYPE_WIPE_COUNT)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_trial_of_the_crusader::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_trial_of_the_crusader::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5];

    for(uint8 i = TYPE_NORTHREND_BEASTS; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)            // Do not load an encounter as "In Progress" - reset it instead.
            m_auiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_trial_of_the_crusader::DoSummonRamsey(uint32 uiEntry)
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    if (uiEntry)
        ;
    // For initial case, figure which Ramsay to summon
    else if (m_auiEncounter[TYPE_TWIN_VALKYR] == DONE)
        uiEntry = NPC_RAMSEY_5;
    else if (m_auiEncounter[TYPE_FACTION_CHAMPIONS] == DONE)
        uiEntry = NPC_RAMSEY_4;
    else if (m_auiEncounter[TYPE_JARAXXUS] == DONE)
        uiEntry = NPC_RAMSEY_3;
    else if (m_auiEncounter[TYPE_NORTHREND_BEASTS] == DONE)
        uiEntry = NPC_RAMSEY_2;
    else
        uiEntry = NPC_RAMSEY_1;

    pPlayer->SummonCreature(uiEntry, aRamsayPositions[0][0], aRamsayPositions[0][1], aRamsayPositions[0][2], aRamsayPositions[0][3], TEMPSUMMON_DEAD_DESPAWN, 0);
}

void instance_trial_of_the_crusader::JustDidDialogueStep(int32 iEntry)
{
    switch (iEntry)
    {
        case NPC_RAMSEY_1:
        case NPC_RAMSEY_2:
        case NPC_RAMSEY_3:
        case NPC_RAMSEY_4:
        case NPC_RAMSEY_5:
            DoSummonRamsey(iEntry);
            break;
    }
}

InstanceData* GetInstanceData_instance_trial_of_the_crusader(Map* pMap)
{
    return new instance_trial_of_the_crusader(pMap);
}

void AddSC_instance_trial_of_the_crusader()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_trial_of_the_crusader";
    pNewScript->GetInstanceData = &GetInstanceData_instance_trial_of_the_crusader;
    pNewScript->RegisterSelf();
}
