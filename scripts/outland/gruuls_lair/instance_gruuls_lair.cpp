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
SDName: Instance_Gruuls_Lair
SD%Complete: 100
SDComment:
SDCategory: Gruul's Lair
EndScriptData */

#include "precompiled.h"
#include "gruuls_lair.h"

/* Gruuls Lair encounters:
1 - High King Maulgar event
2 - Gruul event
*/

instance_gruuls_lair::instance_gruuls_lair(Map *pMap) : ScriptedInstance(pMap),
    m_uiMaulgarGUID(0),
    m_uiKigglerGUID(0),
    m_uiBlindeyeGUID(0),
    m_uiOlmGUID(0),
    m_uiKroshGUID(0),

    m_uiMaulgarDoorGUID(0),
    m_uiGruulEncounterDoorGUID(0)
{
    Initialize();
}

void instance_gruuls_lair::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_gruuls_lair::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;

    return false;
}

void instance_gruuls_lair::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_MAULGAR:  m_uiMaulgarGUID  = pCreature->GetGUID(); break;
        case NPC_KROSH:    m_uiKroshGUID    = pCreature->GetGUID(); break;
        case NPC_OLM:      m_uiOlmGUID      = pCreature->GetGUID(); break;
        case NPC_KIGGLER:  m_uiKigglerGUID  = pCreature->GetGUID(); break;
        case NPC_BLINDEYE: m_uiBlindeyeGUID = pCreature->GetGUID(); break;
    }
}

void instance_gruuls_lair::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_PORT_GRONN_1:
            m_uiMaulgarDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[0] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PORT_GRONN_2:
            m_uiGruulEncounterDoorGUID = pGo->GetGUID();
            break;
    }
}

void instance_gruuls_lair::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_MAULGAR_EVENT:
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiMaulgarDoorGUID);
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_GRUUL_EVENT:
            DoUseDoorOrButton(m_uiGruulEncounterDoorGUID);
            m_auiEncounter[1] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1];

        m_strSaveData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_gruuls_lair::GetData(uint32 uiType)
{
    switch (uiType)
    {
    case TYPE_MAULGAR_EVENT: return m_auiEncounter[0];
    case TYPE_GRUUL_EVENT:   return m_auiEncounter[1];

    default:
        return 0;
    }
}

uint64 instance_gruuls_lair::GetData64(uint32 uiData)
{
    switch (uiData)
    {
        case NPC_MAULGAR:  return m_uiMaulgarGUID;
        case NPC_BLINDEYE: return m_uiBlindeyeGUID;
        case NPC_KIGGLER:  return m_uiKigglerGUID;
        case NPC_KROSH:    return m_uiKroshGUID;
        case NPC_OLM:      return m_uiOlmGUID;

        default:
            return 0;
    }
}

void instance_gruuls_lair::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);

    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_gruuls_lair(Map* pMap)
{
    return new instance_gruuls_lair(pMap);
}

void AddSC_instance_gruuls_lair()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_gruuls_lair";
    pNewScript->GetInstanceData = &GetInstanceData_instance_gruuls_lair;
    pNewScript->RegisterSelf();
}
