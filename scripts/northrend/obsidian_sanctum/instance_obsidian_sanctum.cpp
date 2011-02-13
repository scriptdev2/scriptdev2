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
SDName: Instance_Obsidian_Sanctum
SD%Complete: 80%
SDComment:
SDCategory: Obsidian Sanctum
EndScriptData */

#include "precompiled.h"
#include "obsidian_sanctum.h"

/* Obsidian Sanctum encounters:
0 - Sartharion
*/

instance_obsidian_sanctum::instance_obsidian_sanctum(Map* pMap) : ScriptedInstance(pMap),
    m_uiAliveDragons(0),
    m_uiSartharionGUID(0),
    m_uiTenebronGUID(0),
    m_uiShadronGUID(0),
    m_uiVesperonGUID(0)
    {
        Initialize();
    }

void instance_obsidian_sanctum::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_obsidian_sanctum::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_SARTHARION:
            m_uiSartharionGUID = pCreature->GetGUID();
            break;
        // The three dragons below set to active state once created.
        // We must expect bigger raid to encounter main boss, and then three dragons must be active due to grid differences
        case NPC_TENEBRON:
            m_uiTenebronGUID = pCreature->GetGUID();
            pCreature->SetActiveObjectState(true);
            break;
        case NPC_SHADRON:
            m_uiShadronGUID = pCreature->GetGUID();
            pCreature->SetActiveObjectState(true);
            break;
        case NPC_VESPERON:
            m_uiVesperonGUID = pCreature->GetGUID();
            pCreature->SetActiveObjectState(true);
            break;
    }
}

void instance_obsidian_sanctum::SetData(uint32 uiType, uint32 uiData)
{
    if (uiType == TYPE_SARTHARION_EVENT)
        m_auiEncounter[0] = uiData;
    else if (uiType == TYPE_ALIVE_DRAGONS)
        m_uiAliveDragons = uiData;

    // No need to save anything here
}

uint32 instance_obsidian_sanctum::GetData(uint32 uiType)
{
    if (uiType == TYPE_SARTHARION_EVENT)
        return m_auiEncounter[0];

    return 0;
}

uint64 instance_obsidian_sanctum::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_SARTHARION: return m_uiSartharionGUID;
        case NPC_TENEBRON:   return m_uiTenebronGUID;
        case NPC_SHADRON:    return m_uiShadronGUID;
        case NPC_VESPERON:   return m_uiVesperonGUID;
        default:
            return 0;
    }
}

bool instance_obsidian_sanctum::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_DRAGONS_ALIVE_1_N:
        case ACHIEV_DRAGONS_ALIVE_1_H:
            return m_uiAliveDragons >= 1;
        case ACHIEV_DRAGONS_ALIVE_2_N:
        case ACHIEV_DRAGONS_ALIVE_2_H:
            return m_uiAliveDragons >= 2;
        case ACHIEV_DRAGONS_ALIVE_3_N:
        case ACHIEV_DRAGONS_ALIVE_3_H:
            return m_uiAliveDragons >= 3;
        default:
            return false;
    }
}

InstanceData* GetInstanceData_instance_obsidian_sanctum(Map* pMap)
{
    return new instance_obsidian_sanctum(pMap);
}

void AddSC_instance_obsidian_sanctum()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_obsidian_sanctum";
    pNewScript->GetInstanceData = GetInstanceData_instance_obsidian_sanctum;
    pNewScript->RegisterSelf();
}
