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
SDName: Instance_Black_Temple
SD%Complete: 100
SDComment: Instance Data Scripts and functions to acquire mobs and set encounter status for use in various Black Temple Scripts
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "black_temple.h"

/* Black Temple encounters:
0 - High Warlord Naj'entus event
1 - Supremus Event
2 - Shade of Akama Event
3 - Teron Gorefiend Event
4 - Gurtogg Bloodboil Event
5 - Reliquary Of Souls Event
6 - Mother Shahraz Event
7 - Illidari Council Event
8 - Illidan Stormrage Event
*/

instance_black_temple::instance_black_temple(Map* pMap) : ScriptedInstance(pMap),
    m_uiNajentusGUID(0),
    m_uiAkamaGUID(0),
    m_uiAkama_ShadeGUID(0),
    m_uiShadeOfAkamaGUID(0),
    m_uiSupremusGUID(0),
    m_uiLadyMalandeGUID(0),
    m_uiGathiosTheShattererGUID(0),
    m_uiHighNethermancerZerevorGUID(0),
    m_uiVerasDarkshadowGUID(0),
    m_uiIllidariCouncilGUID(0),
    m_uiBloodElfCouncilVoiceGUID(0),
    m_uiIllidanStormrageGUID(0),

    m_uiNajentusGateGUID(0),
    m_uiMainTempleDoorsGUID(0),
    m_uiShadeAkamaDoorGUID(0),
    m_uiIllidanGateGUID(0),
    m_uiShahrazPreDoorGUID(0),
    m_uiShahrazPostDoorGUID(0),
    m_uiPreCouncilDoorGUID(0),
    m_uiCouncilDoorGUID(0)
{
    Initialize();
};

void instance_black_temple::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

    m_uiIllidanDoorGUID[0] = 0;
    m_uiIllidanDoorGUID[1] = 0;
}

bool instance_black_temple::IsEncounterInProgress() const
{
    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS) return true;

    return false;
}

void instance_black_temple::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_WARLORD_NAJENTUS:  m_uiNajentusGUID                = pCreature->GetGUID(); break;
        case NPC_AKAMA:             m_uiAkamaGUID                   = pCreature->GetGUID(); break;
        case NPC_AKAMA_SHADE:       m_uiAkama_ShadeGUID             = pCreature->GetGUID(); break;
        case NPC_SHADE_OF_AKAMA:    m_uiShadeOfAkamaGUID            = pCreature->GetGUID(); break;
        case NPC_SUPREMUS:          m_uiSupremusGUID                = pCreature->GetGUID(); break;
        case NPC_ILLIDAN_STORMRAGE: m_uiIllidanStormrageGUID        = pCreature->GetGUID(); break;
        case NPC_GATHIOS:           m_uiGathiosTheShattererGUID     = pCreature->GetGUID(); break;
        case NPC_ZEREVOR:           m_uiHighNethermancerZerevorGUID = pCreature->GetGUID(); break;
        case NPC_LADY_MALANDE:      m_uiLadyMalandeGUID             = pCreature->GetGUID(); break;
        case NPC_VERAS:             m_uiVerasDarkshadowGUID         = pCreature->GetGUID(); break;
        case NPC_ILLIDARI_COUNCIL:  m_uiIllidariCouncilGUID         = pCreature->GetGUID(); break;
        case NPC_COUNCIL_VOICE:     m_uiBloodElfCouncilVoiceGUID    = pCreature->GetGUID(); break;
    }
}

void instance_black_temple::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_NAJENTUS_GATE:                              // Gate past Naj'entus (at the entrance to Supermoose's courtyards)
            m_uiNajentusGateGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_NAJENTUS] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_SUPREMUS_DOORS:                             // Main Temple Doors - right past Supermoose (Supremus)
            m_uiMainTempleDoorsGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_SUPREMUS] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_SHADE_OF_AKAMA:                             // Door close during encounter
            m_uiShadeAkamaDoorGUID = pGo->GetGUID();
            break;
        case GO_PRE_SHAHRAZ_DOOR:                           // Door leading to Mother Shahraz
            m_uiShahrazPreDoorGUID = pGo->GetGUID();
            if (CanPreMotherDoorOpen())
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_POST_SHAHRAZ_DOOR:                           // Door after shahraz
            m_uiShahrazPostDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[6] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PRE_COUNCIL_DOOR:                           // Door leading to the Council (grand promenade)
            m_uiPreCouncilDoorGUID = pGo->GetGUID();
            break;
        case GO_COUNCIL_DOOR:                               // Door leading to the Council (inside)
            m_uiCouncilDoorGUID = pGo->GetGUID();
            break;
        case GO_ILLIDAN_GATE:                               // Gate leading to Temple Summit
            m_uiIllidanGateGUID = pGo->GetGUID();
            // TODO - dependend on council state
            break;
        case GO_ILLIDAN_DOOR_R:                             // Right door at Temple Summit
            m_uiIllidanDoorGUID[0] = pGo->GetGUID();
            break;
        case GO_ILLIDAN_DOOR_L:                             // Left door at Temple Summit
            m_uiIllidanDoorGUID[1] = pGo->GetGUID();
            break;
    }
}

bool instance_black_temple::CanPreMotherDoorOpen()
{
    if (m_auiEncounter[TYPE_SHADE] == DONE && m_auiEncounter[TYPE_GOREFIEND] == DONE && m_auiEncounter[TYPE_BLOODBOIL] == DONE && m_auiEncounter[TYPE_RELIQUIARY] == DONE)
    {
        debug_log("SD2: Black Temple: door to Mother Shahraz can open");
        return true;
    }

    debug_log("SD2: Black Temple: Door data to Mother Shahraz requested, cannot open yet (Encounter data: %u %u %u %u)",m_auiEncounter[2],m_auiEncounter[3],m_auiEncounter[4],m_auiEncounter[5]);
    return false;
}

void instance_black_temple::SetData(uint32 uiType, uint32 uiData)
{
    debug_log("SD2: Instance Black Temple: SetData received for type %u with data %u",uiType,uiData);

    switch(uiType)
    {
        case TYPE_NAJENTUS:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiNajentusGateGUID);
            break;
        case TYPE_SUPREMUS:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiMainTempleDoorsGUID);
            break;
        case TYPE_SHADE:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE && CanPreMotherDoorOpen())
                DoUseDoorOrButton(m_uiShahrazPreDoorGUID);
            break;
        case TYPE_GOREFIEND:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE && CanPreMotherDoorOpen())
                DoUseDoorOrButton(m_uiShahrazPreDoorGUID);
            break;
        case TYPE_BLOODBOIL:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE && CanPreMotherDoorOpen())
                DoUseDoorOrButton(m_uiShahrazPreDoorGUID);
            break;
        case TYPE_RELIQUIARY:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE && CanPreMotherDoorOpen())
                DoUseDoorOrButton(m_uiShahrazPreDoorGUID);
            break;
        case TYPE_SHAHRAZ:
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiShahrazPostDoorGUID);
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_COUNCIL:
            DoUseDoorOrButton(m_uiCouncilDoorGUID);
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_ILLIDAN:    m_auiEncounter[uiType] = uiData; break;
        default:
            error_log("SD2: Instance Black Temple: ERROR SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_black_temple::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_NAJENTUS:   return m_auiEncounter[0];
        case TYPE_SUPREMUS:   return m_auiEncounter[1];
        case TYPE_SHADE:      return m_auiEncounter[2];
        case TYPE_GOREFIEND:  return m_auiEncounter[3];
        case TYPE_BLOODBOIL:  return m_auiEncounter[4];
        case TYPE_RELIQUIARY: return m_auiEncounter[5];
        case TYPE_SHAHRAZ:    return m_auiEncounter[6];
        case TYPE_COUNCIL:    return m_auiEncounter[7];
        case TYPE_ILLIDAN:    return m_auiEncounter[8];
        default:
            return 0;
    }
}

uint64 instance_black_temple::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_WARLORD_NAJENTUS:           return m_uiNajentusGUID;
        case NPC_AKAMA:                      return m_uiAkamaGUID;
        case NPC_AKAMA_SHADE:                return m_uiAkama_ShadeGUID;
        case NPC_SHADE_OF_AKAMA:             return m_uiShadeOfAkamaGUID;
        case NPC_SUPREMUS:                   return m_uiSupremusGUID;
        case NPC_ILLIDAN_STORMRAGE:          return m_uiIllidanStormrageGUID;
        case NPC_GATHIOS:                    return m_uiGathiosTheShattererGUID;
        case NPC_ZEREVOR:                    return m_uiHighNethermancerZerevorGUID;
        case NPC_LADY_MALANDE:               return m_uiLadyMalandeGUID;
        case NPC_VERAS:                      return m_uiVerasDarkshadowGUID;
        case NPC_ILLIDARI_COUNCIL:           return m_uiIllidariCouncilGUID;
        case GO_NAJENTUS_GATE:               return m_uiNajentusGateGUID;
        case GO_ILLIDAN_GATE:                return m_uiIllidanGateGUID;
        case GO_ILLIDAN_DOOR_R:              return m_uiIllidanDoorGUID[0];
        case GO_ILLIDAN_DOOR_L:              return m_uiIllidanDoorGUID[1];
        case GO_SUPREMUS_DOORS:              return m_uiMainTempleDoorsGUID;
        case NPC_COUNCIL_VOICE:              return m_uiBloodElfCouncilVoiceGUID;
        case GO_PRE_SHAHRAZ_DOOR:            return m_uiShahrazPreDoorGUID;
        case GO_POST_SHAHRAZ_DOOR:           return m_uiShahrazPostDoorGUID;
        case GO_PRE_COUNCIL_DOOR:            return m_uiPreCouncilDoorGUID;
        case GO_COUNCIL_DOOR:                return m_uiCouncilDoorGUID;
        default:
            return 0;
    }
}

void instance_black_temple::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)            // Do not load an encounter as "In Progress" - reset it instead.
            m_auiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_black_temple(Map* pMap)
{
    return new instance_black_temple(pMap);
}

void AddSC_instance_black_temple()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_black_temple";
    pNewScript->GetInstanceData = &GetInstanceData_instance_black_temple;
    pNewScript->RegisterSelf();
}
