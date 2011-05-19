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
SDName: Instance_Serpent_Shrine
SD%Complete: 90
SDComment:
SDCategory: Coilfang Resevoir, Serpent Shrine Cavern
EndScriptData */

#include "precompiled.h"
#include "serpent_shrine.h"

/* Serpentshrine cavern encounters:
0 - Hydross The Unstable event
1 - Leotheras The Blind Event
2 - The Lurker Below Event
3 - Fathom-Lord Karathress Event
4 - Morogrim Tidewalker Event
5 - Lady Vashj Event
*/

instance_serpentshrine_cavern::instance_serpentshrine_cavern(Map* pMap) : ScriptedInstance(pMap),
    m_uiSharkkis(0),
    m_uiTidalvess(0),
    m_uiCaribdis(0),
    m_uiLadyVashj(0),
    m_uiKarathress(0),
    m_uiKarathressEvent_Starter(0)
{
    Initialize();
}

void instance_serpentshrine_cavern::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiShieldGenerator, 0, sizeof(m_auiShieldGenerator));
}

bool instance_serpentshrine_cavern::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_serpentshrine_cavern::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_LADYVASHJ:  m_uiLadyVashj  = pCreature->GetGUID(); break;
        case NPC_KARATHRESS: m_uiKarathress = pCreature->GetGUID(); break;
        case NPC_SHARKKIS:   m_uiSharkkis   = pCreature->GetGUID(); break;
        case NPC_TIDALVESS:  m_uiTidalvess  = pCreature->GetGUID(); break;
        case NPC_CARIBDIS:   m_uiCaribdis   = pCreature->GetGUID(); break;
    }
}

void instance_serpentshrine_cavern::SetData64(uint32 uiType, uint64 uiData)
{
    if (uiType == DATA_KARATHRESS_STARTER)
        m_uiKarathressEvent_Starter = uiData;
}

uint64 instance_serpentshrine_cavern::GetData64(uint32 uiData)
{
    switch (uiData)
    {
        case NPC_SHARKKIS:            return m_uiSharkkis;
        case NPC_TIDALVESS:           return m_uiTidalvess;
        case NPC_CARIBDIS:            return m_uiCaribdis;
        case NPC_LADYVASHJ:           return m_uiLadyVashj;
        case NPC_KARATHRESS:          return m_uiKarathress;
        case DATA_KARATHRESS_STARTER: return m_uiKarathressEvent_Starter;

        default:
            return 0;
    }
}

void instance_serpentshrine_cavern::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_HYDROSS_EVENT:
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_LEOTHERAS_EVENT:
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_THELURKER_EVENT:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_KARATHRESS_EVENT:
            m_auiEncounter[3] = uiData;
            break;
        case TYPE_MOROGRIM_EVENT:
            m_auiEncounter[4] = uiData;
            break;
        case TYPE_LADYVASHJ_EVENT:
            if (uiData == NOT_STARTED)
                memset(&m_auiShieldGenerator, 0, sizeof(m_auiShieldGenerator));
            m_auiEncounter[5] = uiData;
            break;
        case TYPE_SHIELDGENERATOR1:
        case TYPE_SHIELDGENERATOR2:
        case TYPE_SHIELDGENERATOR3:
        case TYPE_SHIELDGENERATOR4:
            m_auiShieldGenerator[uiType - TYPE_SHIELDGENERATOR1] = uiData;
            return;
    }

    if (uiData == DONE)
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

void instance_serpentshrine_cavern::Load(const char* chrIn)
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

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_serpentshrine_cavern::GetData(uint32 uiType)
{
    switch (uiType)
    {
        case TYPE_HYDROSS_EVENT:    return m_auiEncounter[0];
        case TYPE_LEOTHERAS_EVENT:  return m_auiEncounter[1];
        case TYPE_THELURKER_EVENT:  return m_auiEncounter[2];
        case TYPE_KARATHRESS_EVENT: return m_auiEncounter[3];
        case TYPE_MOROGRIM_EVENT:   return m_auiEncounter[4];
        case TYPE_LADYVASHJ_EVENT:  return m_auiEncounter[5];

        case TYPE_SHIELDGENERATOR1: return m_auiShieldGenerator[0];
        case TYPE_SHIELDGENERATOR2: return m_auiShieldGenerator[1];
        case TYPE_SHIELDGENERATOR3: return m_auiShieldGenerator[2];
        case TYPE_SHIELDGENERATOR4: return m_auiShieldGenerator[3];

        case TYPE_VASHJ_PHASE3_CHECK:
            for(uint8 i = 0; i < MAX_GENERATOR; ++i)
            {
                if (m_auiShieldGenerator[i] != DONE)
                    return NOT_STARTED;
            }
            return DONE;

        default:
            return 0;
    }
}

InstanceData* GetInstanceData_instance_serpentshrine_cavern(Map* pMap)
{
    return new instance_serpentshrine_cavern(pMap);
}

void AddSC_instance_serpentshrine_cavern()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_serpent_shrine";
    pNewScript->GetInstanceData = &GetInstanceData_instance_serpentshrine_cavern;
    pNewScript->RegisterSelf();
}
