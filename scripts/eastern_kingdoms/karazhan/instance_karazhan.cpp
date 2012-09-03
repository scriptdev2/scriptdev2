/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
    m_uiOperaEvent(0)
{
    Initialize();
}

void instance_karazhan::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_karazhan::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_karazhan::OnPlayerEnter(Player* pPlayer)
{
    // If the opera event is already set, return
    if (GetData(TYPE_OPERA_PERFORMANCE) != 0)
        return;

    // Set the Opera Performance type on the first player enter
    SetData(TYPE_OPERA_PERFORMANCE, urand(OPERA_EVENT_WIZARD_OZ, OPERA_EVENT_ROMULO_AND_JUL));
}

void instance_karazhan::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ATTUMEN:
        case NPC_MIDNIGHT:
        case NPC_MOROES:
        case NPC_BARNES:
        case NPC_NIGHTBANE:
        case NPC_JULIANNE:
        case NPC_ROMULO:
        case NPC_LADY_KEIRA_BERRYBUCK:
        case NPC_LADY_CATRIONA_VON_INDI:
        case NPC_LORD_CRISPIN_FERENCE:
        case NPC_BARON_RAFE_DREUGER:
        case NPC_BARONESS_DOROTHEA_MILLSTIPE:
        case NPC_LORD_ROBIN_DARIS:
        case NPC_IMAGE_OF_MEDIVH:
        case NPC_IMAGE_OF_ARCANAGOS:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
        case NPC_NIGHTBANE_HELPER:
            if (pCreature->GetPositionZ() < 100.0f)
                m_lNightbaneGroundTriggers.push_back(pCreature->GetObjectGuid());
            else
                m_lNightbaneAirTriggers.push_back(pCreature->GetObjectGuid());
            break;
    }
}

void instance_karazhan::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_STAGE_DOOR_LEFT:
        case GO_STAGE_DOOR_RIGHT:
            if (m_auiEncounter[3] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_GAMESMANS_HALL_EXIT_DOOR:
            if (m_auiEncounter[8] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_SIDE_ENTRANCE_DOOR:
            if (m_auiEncounter[3] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED);
            break;
        case GO_STAGE_CURTAIN:
        case GO_PRIVATE_LIBRARY_DOOR:
        case GO_MASSIVE_DOOR:
        case GO_GAMESMANS_HALL_DOOR:
        case GO_NETHERSPACE_DOOR:
        case GO_DUST_COVERED_CHEST:
        case GO_MASTERS_TERRACE_DOOR_1:
        case GO_MASTERS_TERRACE_DOOR_2:
            break;

        // Opera event backgrounds
        case GO_OZ_BACKDROP:
        case GO_HOOD_BACKDROP:
        case GO_HOOD_HOUSE:
        case GO_RAJ_BACKDROP:
        case GO_RAJ_MOON:
        case GO_RAJ_BALCONY:
            break;
        case GO_OZ_HAY:
            m_lOperaHayGuidList.push_back(pGo->GetObjectGuid());
            return;
        case GO_HOOD_TREE:
            m_lOperaTreeGuidList.push_back(pGo->GetObjectGuid());
            return;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_karazhan::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ATTUMEN:
            m_auiEncounter[uiType] = uiData;
            if (uiData == FAIL)
            {
                // Respawn Midnight on Fail
                if (Creature* pMidnight = GetSingleCreatureFromStorage(NPC_MIDNIGHT))
                {
                    if (!pMidnight->isAlive())
                    {
                        pMidnight->Respawn();
                        pMidnight->GetMotionMaster()->MoveTargetedHome();
                    }
                }
            }
            break;
        case TYPE_MOROES:
        case TYPE_MAIDEN:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_OPERA:
            // Don't store the same data twice
            if (uiData == m_auiEncounter[uiType])
                break;
            m_auiEncounter[uiType] = uiData;
            if (uiData == IN_PROGRESS)
                m_uiOzDeathCount = 0;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_STAGE_DOOR_LEFT);
                DoUseDoorOrButton(GO_STAGE_DOOR_RIGHT);
                DoToggleGameObjectFlags(GO_SIDE_ENTRANCE_DOOR, GO_FLAG_LOCKED, false);
            }
            // use curtain only for event start or fail
            else
                DoUseDoorOrButton(GO_STAGE_CURTAIN);
            break;
        case TYPE_CURATOR:
        case TYPE_TERESTIAN:
        case TYPE_ARAN:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_NETHERSPITE:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_MASSIVE_DOOR);
            break;
        case TYPE_CHESS:
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_GAMESMANS_HALL_EXIT_DOOR);
                DoRespawnGameObject(GO_DUST_COVERED_CHEST, DAY);
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_MALCHEZZAR:
            DoUseDoorOrButton(GO_NETHERSPACE_DOOR);
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_NIGHTBANE:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_MASTERS_TERRACE_DOOR_1);
            DoUseDoorOrButton(GO_MASTERS_TERRACE_DOOR_2);
            break;
        // Store the event type for the Opera
        case TYPE_OPERA_PERFORMANCE:
            m_uiOperaEvent = uiData;
            break;
    }

    // Also save the opera performance, once it's set
    if (uiData == DONE || uiType == TYPE_OPERA_PERFORMANCE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
            << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_uiOperaEvent;

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
        case TYPE_OPERA_PERFORMANCE:    return m_uiOperaEvent;

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
        >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_uiOperaEvent;

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)               // Do not load an encounter as "In Progress" - reset it instead.
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_karazhan::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_DOROTHEE:
        case NPC_ROAR:
        case NPC_TINHEAD:
        case NPC_STRAWMAN:
            ++m_uiOzDeathCount;
            // Summon Chrone when all 4 Oz mobs are killed
            if (m_uiOzDeathCount == MAX_OZ_OPERA_MOBS)
            {
                if (Creature* pCrone = pCreature->SummonCreature(NPC_CRONE, afChroneSpawnLoc[0], afChroneSpawnLoc[1], afChroneSpawnLoc[2], afChroneSpawnLoc[3], TEMPSUMMON_DEAD_DESPAWN, 0))
                {
                    if (pCreature->getVictim())
                        pCrone->AI()->AttackStart(pCreature->getVictim());
                }
            }
            break;
    }
}

void instance_karazhan::DoPrepareOperaStage(Creature* pOrganizer)
{
    if (!pOrganizer)
        return;

    debug_log("SD2: Barnes Opera Event - Introduction complete - preparing encounter %d", GetData(TYPE_OPERA_PERFORMANCE));

    // summon the bosses and respawn the stage background
    switch (GetData(TYPE_OPERA_PERFORMANCE))
    {
        case OPERA_EVENT_WIZARD_OZ:
            for (uint8 i = 0; i < MAX_OZ_OPERA_MOBS; ++i)
                pOrganizer->SummonCreature(aOperaLocOz[i].uiEntry, aOperaLocOz[i].fX, aOperaLocOz[i].fY, aOperaLocOz[i].fZ, aOperaLocOz[i].fO, TEMPSUMMON_DEAD_DESPAWN, 0);
            DoRespawnGameObject(GO_OZ_BACKDROP, 12*HOUR);
            for (GuidList::const_iterator itr = m_lOperaHayGuidList.begin(); itr != m_lOperaHayGuidList.end(); ++itr)
                DoRespawnGameObject(*itr, 12*HOUR);
            break;
        case OPERA_EVENT_RED_RIDING_HOOD:
            pOrganizer->SummonCreature(aOperaLocWolf.uiEntry, aOperaLocWolf.fX, aOperaLocWolf.fY, aOperaLocWolf.fZ, aOperaLocWolf.fO, TEMPSUMMON_DEAD_DESPAWN, 0);
            DoRespawnGameObject(GO_HOOD_BACKDROP, 12*HOUR);
            DoRespawnGameObject(GO_HOOD_HOUSE,    12*HOUR);
            for (GuidList::const_iterator itr = m_lOperaTreeGuidList.begin(); itr != m_lOperaTreeGuidList.end(); ++itr)
                DoRespawnGameObject(*itr, 12*HOUR);
            break;
        case OPERA_EVENT_ROMULO_AND_JUL:
            pOrganizer->SummonCreature(aOperaLocJul.uiEntry, aOperaLocJul.fX, aOperaLocJul.fY, aOperaLocJul.fZ, aOperaLocJul.fO, TEMPSUMMON_DEAD_DESPAWN, 0);
            DoRespawnGameObject(GO_RAJ_BACKDROP, 12*HOUR);
            DoRespawnGameObject(GO_RAJ_MOON,     12*HOUR);
            DoRespawnGameObject(GO_RAJ_BALCONY,  12*HOUR);
            break;
    }

    SetData(TYPE_OPERA, IN_PROGRESS);
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
