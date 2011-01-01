/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: instance_draktharon_keep
SD%Complete: 50%
SDComment:
SDCategory: Drak'Tharon Keep
EndScriptData */

#include "precompiled.h"
#include "draktharon_keep.h"

instance_draktharon_keep::instance_draktharon_keep(Map* pMap) : ScriptedInstance(pMap),
    m_uiDreadAddsKilled(0),
    m_bNovosAddGrounded(false),
    m_bTrollgoreConsume(true)
{
    Initialize();
}

void instance_draktharon_keep::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_draktharon_keep::OnCreatureEnterCombat(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_KING_DRED)
        SetData(TYPE_KING_DRED, IN_PROGRESS);
}

void instance_draktharon_keep::OnCreatureEvade(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_KING_DRED)
        SetData(TYPE_KING_DRED, FAIL);
}

void instance_draktharon_keep::OnCreatureDeath(Creature* pCreature)
{
    if ((pCreature->GetEntry() == NPC_DRAKKARI_GUTRIPPER || pCreature->GetEntry() == NPC_DRAKKARI_SCYTHECLAW) && m_auiEncounter[TYPE_KING_DRED] == IN_PROGRESS)
        m_uiDreadAddsKilled++;

    if (pCreature->GetEntry() == NPC_KING_DRED)
        SetData(TYPE_KING_DRED, DONE);
}

bool instance_draktharon_keep::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_BETTER_OFF_DREAD: return m_uiDreadAddsKilled >= 6;
        case ACHIEV_CRIT_OH_NOVOS:         return !m_bNovosAddGrounded;
        case ACHIEV_CRIT_CONSUME_JUNCTION: return m_bTrollgoreConsume;
        default:
            return false;
    }
}

void instance_draktharon_keep::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_TROLLGORE:
            if (uiData == IN_PROGRESS)
                m_bTrollgoreConsume = true;
            if (uiData == SPECIAL)
                m_bTrollgoreConsume = false;
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_NOVOS:
            if (uiData == IN_PROGRESS)
                m_bNovosAddGrounded = false;
            if (uiData == SPECIAL)
                m_bNovosAddGrounded = true;
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_KING_DRED:
            if (uiData == IN_PROGRESS)
                m_uiDreadAddsKilled = 0;
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_THARONJA:
            m_auiEncounter[uiType] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_draktharon_keep::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_draktharon_keep::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_TROLLGORE: return m_auiEncounter[uiType];
        case TYPE_NOVOS:     return m_auiEncounter[uiType];
        case TYPE_KING_DRED: return m_auiEncounter[uiType];
        case TYPE_THARONJA:  return m_auiEncounter[uiType];
        default:
            return 0;
    }
}

InstanceData* GetInstanceData_instance_draktharon_keep(Map* pMap)
{
    return new instance_draktharon_keep(pMap);
}

void AddSC_instance_draktharon_keep()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_draktharon_keep";
    pNewScript->GetInstanceData = &GetInstanceData_instance_draktharon_keep;
    pNewScript->RegisterSelf();
}
