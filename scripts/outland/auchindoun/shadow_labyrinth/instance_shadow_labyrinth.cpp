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
SDName: Instance_Shadow_Labyrinth
SD%Complete: 85
SDComment: Some cleanup left along with save
SDCategory: Auchindoun, Shadow Labyrinth
EndScriptData */

#include "precompiled.h"
#include "shadow_labyrinth.h"

/* Shadow Labyrinth encounters:
1 - Ambassador Hellmaw event
2 - Blackheart the Inciter event
3 - Grandmaster Vorpil event
4 - Murmur event
*/

instance_shadow_labyrinth::instance_shadow_labyrinth(Map* pMap) : ScriptedInstance(pMap),
    m_uiFelOverseerCount(0)
{
    Initialize();
}

void instance_shadow_labyrinth::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_shadow_labyrinth::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_REFECTORY_DOOR:
            if (m_auiEncounter[2] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_SCREAMING_HALL_DOOR:
            if (m_auiEncounter[3] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

        default:
            return;
    }

    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_shadow_labyrinth::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_VORPIL:
            m_mNpcEntryGuidStore[NPC_VORPIL] = pCreature->GetObjectGuid();
            break;
        case NPC_FEL_OVERSEER:
            ++m_uiFelOverseerCount;                         // TODO should actually only count alive ones
            debug_log("SD2: Shadow Labyrinth: counting %u Fel Overseers.", m_uiFelOverseerCount);
            break;
    }
}

void instance_shadow_labyrinth::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_HELLMAW:
            m_auiEncounter[0] = uiData;
            break;

        case TYPE_OVERSEER:
            if (uiData != DONE)
            {
                error_log("SD2: Shadow Labyrinth: TYPE_OVERSEER did not expect other data than DONE");
                return;
            }
            if (m_uiFelOverseerCount)
            {
                --m_uiFelOverseerCount;

                if (m_uiFelOverseerCount)
                {
                    debug_log("SD2: Shadow Labyrinth: %u Fel Overseers left to kill.", m_uiFelOverseerCount);

                    // Skip save call
                    return;
                }
                else
                {
                    m_auiEncounter[1] = DONE;
                    debug_log("SD2: Shadow Labyrinth: TYPE_OVERSEER == DONE");
                }
            }
            break;

        case TYPE_INCITER:
            if (uiData == DONE)
                DoUseDoorOrButton(GO_REFECTORY_DOOR);
            m_auiEncounter[2] = uiData;
            break;

        case TYPE_VORPIL:
            if (uiData == DONE)
                DoUseDoorOrButton(GO_SCREAMING_HALL_DOOR);
            m_auiEncounter[3] = uiData;
            break;

        case TYPE_MURMUR:
            m_auiEncounter[4] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " "
            << m_auiEncounter[2] << " " << m_auiEncounter[3] << " " << m_auiEncounter[4];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_shadow_labyrinth::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_HELLMAW:  return m_auiEncounter[0];
        case TYPE_OVERSEER: return m_auiEncounter[1];

        default:
            return 0;
    }
}

void instance_shadow_labyrinth::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_shadow_labyrinth(Map* pMap)
{
    return new instance_shadow_labyrinth(pMap);
}

void AddSC_instance_shadow_labyrinth()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_shadow_labyrinth";
    pNewScript->GetInstanceData = &GetInstanceData_instance_shadow_labyrinth;
    pNewScript->RegisterSelf();
}
