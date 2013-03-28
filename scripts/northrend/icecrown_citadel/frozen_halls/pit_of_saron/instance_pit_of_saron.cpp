/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: instance_pit_of_saron
SD%Complete: 50%
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    // Ick and Krick outro
    SAY_JAINA_KRICK_1               = -1658036,
    SAY_SYLVANAS_KRICK_1            = -1658037,
    SAY_OUTRO_2                     = -1658038,
    SAY_JAINA_KRICK_2               = -1658039,
    SAY_SYLVANAS_KRICK_2            = -1658040,
    SAY_OUTRO_3                     = -1658041,
    SAY_TYRANNUS_KRICK_1            = -1658042,
    SAY_OUTRO_4                     = -1658043,
    SAY_TYRANNUS_KRICK_2            = -1658044,
    SAY_JAINA_KRICK_3               = -1658045,
    SAY_SYLVANAS_KRICK_3            = -1658046,

    // Ick and Krick outro spells
    SPELL_STRANGULATING             = 69413,
    SPELL_KRICK_KILL_CREDIT         = 71308,
    SPELL_SUICIDE                   = 7,
};

static const DialogueEntryTwoSide aPoSDialogues[] =
{
    // Ick and Krick outro
    {SAY_JAINA_KRICK_1,    NPC_JAINA_PART1,    SAY_SYLVANAS_KRICK_1, NPC_SYLVANAS_PART1, 6000},
    {SAY_OUTRO_2,          NPC_KRICK,          0,                    0,                  16000},
    {SAY_JAINA_KRICK_2,    NPC_JAINA_PART1,    SAY_SYLVANAS_KRICK_2, NPC_SYLVANAS_PART1, 7000},
    {SAY_OUTRO_3,          NPC_KRICK,          0,                    0,                  7000},
    {SAY_TYRANNUS_KRICK_1, NPC_TYRANNUS_INTRO, 0,                    0,                  3000},
    {SPELL_STRANGULATING,  0,                  0,                    0,                  3000},
    {SAY_OUTRO_4,          NPC_KRICK,          0,                    0,                  3000},
    {SAY_TYRANNUS_KRICK_2, NPC_TYRANNUS_INTRO, 0,                    0,                  11000},
    {SAY_JAINA_KRICK_3,    NPC_JAINA_PART1,    SAY_SYLVANAS_KRICK_3, NPC_SYLVANAS_PART1, 0},
    {0, 0, 0},
};

instance_pit_of_saron::instance_pit_of_saron(Map* pMap) : ScriptedInstance(pMap), DialogueHelper(aPoSDialogues),
    m_uiTeam(TEAM_NONE)
{
    Initialize();
}

void instance_pit_of_saron::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    InitializeDialogueHelper(this);

    for (uint8 i = 0; i < MAX_SPECIAL_ACHIEV_CRITS; ++i)
        m_abAchievCriteria[i] = false;
}

void instance_pit_of_saron::OnPlayerEnter(Player* pPlayer)
{
    if (!m_uiTeam)                                          // very first player to enter
    {
        m_uiTeam = pPlayer->GetTeam();
        SetDialogueSide(m_uiTeam == ALLIANCE);
    }
}

void instance_pit_of_saron::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_TYRANNUS_INTRO:
        case NPC_JAINA_PART1:
        case NPC_SYLVANAS_PART1:
        case NPC_GARFROST:
        case NPC_KRICK:
        case NPC_ICK:
        case NPC_TYRANNUS:
        case NPC_RIMEFANG:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_pit_of_saron::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_ICEWALL:
            if (m_auiEncounter[TYPE_GARFROST] == DONE && m_auiEncounter[TYPE_KRICK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HALLS_OF_REFLECT_PORT:
            break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_pit_of_saron::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_GARFROST:
            if (uiData == DONE && m_auiEncounter[TYPE_KRICK] == DONE)
                DoUseDoorOrButton(GO_ICEWALL);
            if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEV_DOESNT_GO_ELEVEN, true);
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_KRICK:
            if (uiData == DONE && m_auiEncounter[TYPE_GARFROST] == DONE)
                DoUseDoorOrButton(GO_ICEWALL);
            if (uiData == SPECIAL)
            {
                // Used just to start the epilogue
                StartNextDialogueText(SAY_JAINA_KRICK_1);
                return;
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_TYRANNUS:
            m_auiEncounter[uiType] = uiData;
            break;
        default:
            return;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_pit_of_saron::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2];

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_pit_of_saron::GetData(uint32 uiType) const
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_pit_of_saron::SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet)
{
    if (uiType < MAX_SPECIAL_ACHIEV_CRITS)
        m_abAchievCriteria[uiType] = bIsMet;
}

bool instance_pit_of_saron::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/) const
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_DOESNT_GO_ELEVEN:
            return m_abAchievCriteria[TYPE_ACHIEV_DOESNT_GO_ELEVEN];
        case ACHIEV_CRIT_DONT_LOOK_UP:
            return m_abAchievCriteria[TYPE_ACHIEV_DONT_LOOK_UP];

        default:
            return false;
    }
}

void instance_pit_of_saron::JustDidDialogueStep(int32 iEntry)
{
    switch (iEntry)
    {
        case SAY_OUTRO_3:
            // Move Tyrannus into position
            if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
            {
                pTyrannus->SetWalk(false);
                pTyrannus->GetMotionMaster()->MovePoint(0, 835.5887f, 139.4345f, 530.9526f);
            }
            break;
        case SPELL_STRANGULATING:
            if (Creature* pKrick = GetSingleCreatureFromStorage(NPC_KRICK))
            {
                pKrick->CastSpell(pKrick, SPELL_STRANGULATING, true);
                pKrick->SetLevitate(true);
                pKrick->GetMotionMaster()->MovePoint(0, pKrick->GetPositionX(), pKrick->GetPositionY(), pKrick->GetPositionZ() + 5.0f);
            }
            break;
        case SAY_TYRANNUS_KRICK_2:
            if (Creature* pKrick = GetSingleCreatureFromStorage(NPC_KRICK))
            {
                pKrick->CastSpell(pKrick, SPELL_KRICK_KILL_CREDIT, true);
                pKrick->CastSpell(pKrick, SPELL_SUICIDE, true);
            }
            break;
        case SAY_JAINA_KRICK_3:
            // Note: location needs to be confirmed
            if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
                pTyrannus->GetMotionMaster()->MovePoint(0, 948.649f, 152.921f, 672.42f);
            break;
    }
}

InstanceData* GetInstanceData_instance_pit_of_saron(Map* pMap)
{
    return new instance_pit_of_saron(pMap);
}

void AddSC_instance_pit_of_saron()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_pit_of_saron";
    pNewScript->GetInstanceData = &GetInstanceData_instance_pit_of_saron;
    pNewScript->RegisterSelf();
}
