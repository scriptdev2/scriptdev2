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
SDName: Instance_Karazhan
SD%Complete: 70
SDComment: Instance Script for Karazhan to help in various encounters. TODO: GameObject visibility for Opera event.
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"

/*
0  - Attumen + Midnight (optional)
1  - Moroes
2  - Maiden of Virtue (optional)
3  - Opera Event
4  - Curator
5  - Terestian Illhoof (optional)
6  - Shade of Aran (optional)
7  - Netherspite (optional)
8  - Chess Event
9  - Prince Malchezzar
10 - Nightbane
*/

instance_karazhan::instance_karazhan(Map* pMap) : ScriptedInstance(pMap),
    m_uiOzDeathCount(0),

    m_uiMoroesGUID(0),
    m_uiTerestianGUID(0),
    m_uiNightbaneGUID(0),

    m_uiCurtainGUID(0),
    m_uiStageDoorLeftGUID(0),
    m_uiStageDoorRightGUID(0),
    m_uiLibraryDoor(0),
    m_uiMassiveDoor(0),
    m_uiSideEntranceDoor(0),
    m_uiGamesmansDoor(0),
    m_uiGamesmansExitDoor(0),
    m_uiNetherspaceDoor(0),
    m_uiDustCoveredChest(0)
{
    Initialize();
}

void instance_karazhan::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

    // 1 - OZ, 2 - HOOD, 3 - RAJ, this never gets altered.
    m_uiOperaEvent = urand(EVENT_OZ, EVENT_RAJ);
}

bool instance_karazhan::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;

    return false;
}

void instance_karazhan::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_TERESTIAN: m_uiTerestianGUID = pCreature->GetGUID(); break;
        case NPC_MOROES:    m_uiMoroesGUID    = pCreature->GetGUID(); break;
        case NPC_NIGHTBANE: m_uiNightbaneGUID = pCreature->GetGUID(); break;
    }
}

void instance_karazhan::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_STAGE_CURTAIN:
            m_uiCurtainGUID = pGo->GetGUID();
            break;
        case GO_STAGE_DOOR_LEFT:
            m_uiStageDoorLeftGUID = pGo->GetGUID();
            if (m_auiEncounter[4] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_STAGE_DOOR_RIGHT:
            m_uiStageDoorRightGUID = pGo->GetGUID();
            if (m_auiEncounter[4] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PRIVATE_LIBRARY_DOOR:
            m_uiLibraryDoor = pGo->GetGUID();
            break;
        case GO_MASSIVE_DOOR:
            m_uiMassiveDoor = pGo->GetGUID();
            break;
        case GO_GAMESMANS_HALL_DOOR:
            m_uiGamesmansDoor = pGo->GetGUID();
            break;
        case GO_GAMESMANS_HALL_EXIT_DOOR:
            m_uiGamesmansExitDoor = pGo->GetGUID();
            break;
        case GO_NETHERSPACE_DOOR:
            m_uiNetherspaceDoor = pGo->GetGUID();
            break;
        case GO_SIDE_ENTRANCE_DOOR:
            m_uiSideEntranceDoor = pGo->GetGUID();
            if (m_auiEncounter[4] != DONE)
                pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED);
            else
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED);
            break;
        case GO_DUST_COVERED_CHEST:
            m_uiDustCoveredChest = pGo->GetGUID();
            break;

        case GO_OZ_BACKDROP:
        case GO_OZ_HAY:
            // if (m_uiOperaEvent == EVENT_OZ)              // TODO - respawn, store for later respawn?
            break;
        case GO_HOOD_BACKDROP:
        case GO_HOOD_TREE:
        case GO_HOOD_HOUSE:
            // if (m_uiOperaEvent == EVENT_HOOD)            // TODO - respawn, store for later respawn?
            break;
        case GO_RAJ_BACKDROP:
        case GO_RAJ_MOON:
        case GO_RAJ_BALCONY:
            // if (m_uiOperaEvent == EVENT_RAJ)             // TODO - respawn, store for later respawn?
            break;
    }
}

void instance_karazhan::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ATTUMEN:
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_MOROES:
            if (m_auiEncounter[1] != DONE)
                m_auiEncounter[1] = uiData;
            break;
        case TYPE_MAIDEN:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_OPERA:
            m_auiEncounter[3] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiStageDoorLeftGUID);
                DoUseDoorOrButton(m_uiStageDoorRightGUID);
                if (GameObject* pSideEntrance = instance->GetGameObject(m_uiSideEntranceDoor))
                    pSideEntrance->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED);
            }
            break;
        case TYPE_CURATOR:
            m_auiEncounter[4] = uiData;
            break;
        case TYPE_TERESTIAN:
            m_auiEncounter[5] = uiData;
            break;
        case TYPE_ARAN:
            m_auiEncounter[6] = uiData;
            if (uiData != IN_PROGRESS)
                DoUseDoorOrButton(m_uiLibraryDoor);
            break;
        case TYPE_NETHERSPITE:
            m_auiEncounter[7] = uiData;
            DoUseDoorOrButton(m_uiMassiveDoor);
            break;
        case TYPE_CHESS:
            if (uiData == DONE)
                DoRespawnGameObject(m_uiDustCoveredChest, DAY);
            m_auiEncounter[8] = uiData;
            break;
        case TYPE_MALCHEZZAR:
            m_auiEncounter[9] = uiData;
            break;
        case TYPE_NIGHTBANE:
            m_auiEncounter[10] = uiData;
            break;

        case DATA_OPERA_OZ_DEATHCOUNT:
            if (uiData == SPECIAL)
                ++m_uiOzDeathCount;
            else if (uiData == IN_PROGRESS)
                m_uiOzDeathCount = 0;
            return;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
            << m_auiEncounter[9] << " " << m_auiEncounter[10];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_karazhan::GetData(uint32 uiType)
{
    switch (uiType)
    {
        case TYPE_ATTUMEN:              return m_auiEncounter[0];
        case TYPE_MOROES:               return m_auiEncounter[1];
        case TYPE_MAIDEN:               return m_auiEncounter[2];
        case TYPE_OPERA:                return m_auiEncounter[3];
        case TYPE_CURATOR:              return m_auiEncounter[4];
        case TYPE_TERESTIAN:            return m_auiEncounter[5];
        case TYPE_ARAN:                 return m_auiEncounter[6];
        case TYPE_NETHERSPITE:          return m_auiEncounter[7];
        case TYPE_CHESS:                return m_auiEncounter[8];
        case TYPE_MALCHEZZAR:           return m_auiEncounter[9];
        case TYPE_NIGHTBANE:            return m_auiEncounter[10];

        case DATA_OPERA_PERFORMANCE:    return m_uiOperaEvent;
        case DATA_OPERA_OZ_DEATHCOUNT:  return m_uiOzDeathCount;

        default:
            return 0;
    }
}

uint64 instance_karazhan::GetData64(uint32 uiData)
{
    switch (uiData)
    {
        case NPC_MOROES:                    return m_uiMoroesGUID;
        case NPC_TERESTIAN:                 return m_uiTerestianGUID;
        case NPC_NIGHTBANE:                 return m_uiNightbaneGUID;

        case GO_STAGE_DOOR_LEFT:            return m_uiStageDoorLeftGUID;
        case GO_STAGE_DOOR_RIGHT:           return m_uiStageDoorRightGUID;
        case GO_STAGE_CURTAIN:              return m_uiCurtainGUID;
        case GO_PRIVATE_LIBRARY_DOOR:       return m_uiLibraryDoor;
        case GO_MASSIVE_DOOR:               return m_uiMassiveDoor;
        case GO_SIDE_ENTRANCE_DOOR:         return m_uiSideEntranceDoor;
        case GO_GAMESMANS_HALL_DOOR:        return m_uiGamesmansDoor;
        case GO_GAMESMANS_HALL_EXIT_DOOR:   return m_uiGamesmansExitDoor;
        case GO_NETHERSPACE_DOOR:           return m_uiNetherspaceDoor;

        default:
            return 0;
    }
}

void instance_karazhan::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);

    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
        >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)               // Do not load an encounter as "In Progress" - reset it instead.
            m_auiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_karazhan(Map* pMap)
{
    return new instance_karazhan(pMap);
}

void AddSC_instance_karazhan()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_karazhan";
    pNewScript->GetInstanceData = &GetInstanceData_instance_karazhan;
    pNewScript->RegisterSelf();
}
