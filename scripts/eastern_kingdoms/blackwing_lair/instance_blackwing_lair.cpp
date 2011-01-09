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
SDName: Instance_Blackwing_Lair
SD%Complete: 0
SDComment:
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"


instance_blackwing_lair::instance_blackwing_lair(Map* pMap) : ScriptedInstance(pMap),
    m_uiRazorgoreEnterDoorGUID(0),
    m_uiRazorgoreExitDoorGUID(0),
    m_uiVaelastraszDoorGUID(0),
    m_uiLashlayerDoorGUID(0),
    m_uiChromaggusEnterDoorGUID(0),
    m_uiChromaggusExitDoorGUID(0),
    m_uiChromaggusSideDoorGUID(0),
    m_uiNefarianDoorGUID(0)
{
    Initialize();
}

void instance_blackwing_lair::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_blackwing_lair::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }
    return false;
}

void instance_blackwing_lair::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_RAZORGORE_ENTER:
            m_uiRazorgoreEnterDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_RAZORGORE_EXIT:
            m_uiRazorgoreExitDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_RAZORGORE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_NEFARIAN:
            m_uiNefarianDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_CHROMAGGUS_ENTER:
            m_uiChromaggusEnterDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_CHROMAGGUS_SIDE:
            m_uiChromaggusSideDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_CHROMAGGUS_EXIT:
            m_uiChromaggusExitDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_CHROMAGGUS] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_VAELASTRASZ:
            m_uiVaelastraszDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_VAELASTRASZ] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_LASHLAYER:
            m_uiLashlayerDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_LASHLAYER] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
    }
}

void instance_blackwing_lair::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_RAZORGORE:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiRazorgoreEnterDoorGUID);
            if(uiData == DONE)
                DoUseDoorOrButton(m_uiRazorgoreExitDoorGUID);
            break;
        case TYPE_VAELASTRASZ:
            m_auiEncounter[uiType] = uiData;
            // Prevent the players from running back to the first room
            DoUseDoorOrButton(m_uiRazorgoreExitDoorGUID);
            if(uiData == DONE)
                DoUseDoorOrButton(m_uiVaelastraszDoorGUID);
            break;
        case TYPE_LASHLAYER:
            m_auiEncounter[uiType] = uiData;
            if(uiData == DONE)
                DoUseDoorOrButton(m_uiLashlayerDoorGUID);
            break;
        case TYPE_FIREMAW:
        case TYPE_EBONROC:
        case TYPE_FLAMEGOR:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_CHROMAGGUS:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiChromaggusEnterDoorGUID);
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiChromaggusExitDoorGUID);
            break;
        case TYPE_NEFARIAN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiNefarianDoorGUID);
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_blackwing_lair::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_blackwing_lair::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

InstanceData* GetInstanceData_instance_blackwing_lair(Map* pMap)
{
    return new instance_blackwing_lair(pMap);
}

void AddSC_instance_blackwing_lair()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_blackwing_lair";
    pNewScript->GetInstanceData = &GetInstanceData_instance_blackwing_lair;
    pNewScript->RegisterSelf();
}
