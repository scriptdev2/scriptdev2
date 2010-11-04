/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

instance_pit_of_saron::instance_pit_of_saron(Map* pMap) : ScriptedInstance(pMap),
    m_uiTyrannusIntroGUID(0),
    m_uiGafrostGUID(0),
    m_uiKrickGUID(0),
    m_uiIckGUID(0),
    m_uiTirannusGUID(0),
    m_uiRimefangGUID(0),
    m_uiIcewallGUID(0)
{
    Initialize();
}

 void instance_pit_of_saron::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_pit_of_saron::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_TYRANNUS_INTRO: m_uiTyrannusIntroGUID = pCreature->GetGUID(); break;
        case NPC_GAFROST:        m_uiGafrostGUID       = pCreature->GetGUID(); break;
        case NPC_KRICK:          m_uiKrickGUID         = pCreature->GetGUID(); break;
        case NPC_ICK:            m_uiIckGUID           = pCreature->GetGUID(); break;
        case NPC_TYRANNUS:       m_uiTirannusGUID      = pCreature->GetGUID(); break;
        case NPC_RIMEFANG:       m_uiRimefangGUID      = pCreature->GetGUID(); break;
    }
}

void instance_pit_of_saron::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_ICEWALL:
            m_uiIcewallGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_GAFROST] == DONE && m_auiEncounter[TYPE_KRICK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
    }
}

void instance_pit_of_saron::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_GAFROST:
            if (uiData == DONE && m_auiEncounter[TYPE_KRICK] == DONE)
                DoUseDoorOrButton(m_uiIcewallGUID);
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_KRICK:
            if (uiData == DONE && m_auiEncounter[TYPE_GAFROST] == DONE)
                DoUseDoorOrButton(m_uiIcewallGUID);
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

        strInstData = saveStream.str();

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

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_pit_of_saron::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_GAFROST:   return m_auiEncounter[uiType];
        case TYPE_KRICK:     return m_auiEncounter[uiType];
        case TYPE_TYRANNUS:  return m_auiEncounter[uiType];
        default:
            return 0;
    }
}

uint64 instance_pit_of_saron::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_TYRANNUS_INTRO: return m_uiTyrannusIntroGUID;
        case NPC_GAFROST:        return m_uiGafrostGUID;
        case NPC_KRICK:          return m_uiKrickGUID;
        case NPC_ICK:            return m_uiIckGUID;
        case NPC_TYRANNUS:       return m_uiTirannusGUID;
        case NPC_RIMEFANG:       return m_uiRimefangGUID;
        default:
            return 0;
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
