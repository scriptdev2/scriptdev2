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
SDName: Instance_Blood_Furnace
SD%Complete: 75
SDComment:
SDCategory: Blood Furnace
EndScriptData */

#include "precompiled.h"
#include "blood_furnace.h"

instance_blood_furnace::instance_blood_furnace(Map* pMap) : ScriptedInstance(pMap),
    m_uiMakerGUID(0),
    m_uiBroggokGUID(0),
    m_uiKelidanGUID(0),

    m_uiDoorFinalExitGUID(0),
    m_uiDoorMakerFrontGUID(0),
    m_uiDoorMakerRearGUID(0),
    m_uiDoorBroggokFrontGUID(0),
    m_uiDoorBrokkokRearGUID(0),
    m_uiDoorKelidanExitGUID(0),

    m_uiPrisonCell1GUID(0),
    m_uiPrisonCell2GUID(0),
    m_uiPrisonCell3GUID(0),
    m_uiPrisonCell4GUID(0),
    m_uiPrisonCell5GUID(0),
    m_uiPrisonCell6GUID(0),
    m_uiPrisonCell7GUID(0),
    m_uiPrisonCell8GUID(0)
{
    Initialize();
}

void instance_blood_furnace::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_blood_furnace::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_THE_MAKER:         m_uiMakerGUID = pCreature->GetGUID();   break;
        case NPC_BROGGOK:           m_uiBroggokGUID = pCreature->GetGUID(); break;
        case NPC_KELIDAN_THE_MAKER: m_uiKelidanGUID = pCreature->GetGUID(); break;
    }
}

void instance_blood_furnace::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_DOOR_MAKER_FRONT:                           // the maker front door
            m_uiDoorMakerFrontGUID = pGo->GetGUID();
            break;
        case GO_DOOR_MAKER_REAR:                            // the maker rear door
            m_uiDoorMakerRearGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_THE_MAKER_EVENT] == DONE && pGo->GetGoState() == GO_STATE_READY)
                DoUseDoorOrButton(m_uiDoorMakerRearGUID);
            break;
        case GO_DOOR_BROGGOK_FRONT:                         // broggok front door
            m_uiDoorBroggokFrontGUID = pGo->GetGUID();
            break;
        case GO_DOOR_BROGGOK_REAR:                          // broggok rear door
            m_uiDoorBrokkokRearGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_BROGGOK_EVENT] == DONE && pGo->GetGoState() == GO_STATE_READY)
                DoUseDoorOrButton(m_uiDoorBrokkokRearGUID);
            break;
        case GO_DOOR_KELIDAN_EXIT:                          // kelidan exit door
            m_uiDoorKelidanExitGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_KELIDAN_EVENT] == DONE && pGo->GetGoState() == GO_STATE_READY)
                DoUseDoorOrButton(m_uiDoorKelidanExitGUID);
            break;
        case GO_DOOR_FINAL_EXIT:                            // final exit door
            m_uiDoorFinalExitGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_KELIDAN_EVENT] == DONE && pGo->GetGoState() == GO_STATE_READY)
                DoUseDoorOrButton(m_uiDoorFinalExitGUID);
            break;
        case GO_PRISON_CELL_MAKER1:   m_uiPrisonCell1GUID = pGo->GetGUID(); break;
        case GO_PRISON_CELL_MAKER2:   m_uiPrisonCell2GUID = pGo->GetGUID(); break;
        case GO_PRISON_CELL_MAKER3:   m_uiPrisonCell3GUID = pGo->GetGUID(); break;
        case GO_PRISON_CELL_MAKER4:   m_uiPrisonCell4GUID = pGo->GetGUID(); break;
        case GO_PRISON_CELL_BROGGOK1: m_uiPrisonCell5GUID = pGo->GetGUID(); break;
        case GO_PRISON_CELL_BROGGOK2: m_uiPrisonCell6GUID = pGo->GetGUID(); break;
        case GO_PRISON_CELL_BROGGOK3: m_uiPrisonCell7GUID = pGo->GetGUID(); break;
        case GO_PRISON_CELL_BROGGOK4: m_uiPrisonCell8GUID = pGo->GetGUID(); break;
    }
}

uint64 instance_blood_furnace::GetData64(uint32 uiData)
{
    switch (uiData)
    {
        case NPC_THE_MAKER:           return m_uiMakerGUID;
        case NPC_BROGGOK:             return m_uiBroggokGUID;
        case GO_PRISON_CELL_MAKER1:   return m_uiPrisonCell1GUID;
        case GO_PRISON_CELL_MAKER2:   return m_uiPrisonCell2GUID;
        case GO_PRISON_CELL_MAKER3:   return m_uiPrisonCell3GUID;
        case GO_PRISON_CELL_MAKER4:   return m_uiPrisonCell4GUID;
        case GO_PRISON_CELL_BROGGOK1: return m_uiPrisonCell5GUID;
        case GO_PRISON_CELL_BROGGOK2: return m_uiPrisonCell6GUID;
        case GO_PRISON_CELL_BROGGOK3: return m_uiPrisonCell7GUID;
        case GO_PRISON_CELL_BROGGOK4: return m_uiPrisonCell8GUID;

        default:
            return 0;
    }
}

void instance_blood_furnace::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_THE_MAKER_EVENT:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(m_uiDoorMakerFrontGUID);
            if (uiData == FAIL)
                DoUseDoorOrButton(m_uiDoorMakerFrontGUID);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiDoorMakerFrontGUID);
                DoUseDoorOrButton(m_uiDoorMakerRearGUID);
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_BROGGOK_EVENT:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(m_uiDoorBroggokFrontGUID);
            if (uiData == FAIL)
                DoUseDoorOrButton(m_uiDoorBroggokFrontGUID);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiDoorBroggokFrontGUID);
                DoUseDoorOrButton(m_uiDoorBrokkokRearGUID);
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_KELIDAN_EVENT:
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiDoorKelidanExitGUID);
                DoUseDoorOrButton(m_uiDoorFinalExitGUID);
            }
            m_auiEncounter[uiType] = uiData;
            break;
        default:
            error_log("SD2: Instance Blood Furnace SetData with Type %u Data %u, but this is not implemented.", uiType, uiData);
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

uint32 instance_blood_furnace::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_blood_furnace::Load(const char* chrIn)
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
        if (m_auiEncounter[i] == IN_PROGRESS || m_auiEncounter[i] == FAIL)
            m_auiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_blood_furnace(Map* pMap)
{
    return new instance_blood_furnace(pMap);
}

void AddSC_instance_blood_furnace()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_blood_furnace";
    pNewScript->GetInstanceData = &GetInstanceData_instance_blood_furnace;
    pNewScript->RegisterSelf();
}
