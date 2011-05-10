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
SDName: instance_sunken_temple
SD%Complete: 50
SDComment: Hakkar Summon Event missing
SDCategory: Sunken Temple
EndScriptData */

#include "precompiled.h"
#include "sunken_temple.h"

instance_sunken_temple::instance_sunken_temple(Map* pMap) : ScriptedInstance(pMap),
    m_uiAtalarionGUID(0),
    m_uiJammalanGUID(0),
    m_uiJammalanBarrierGUID(0),
    m_uiIdolOfHakkarGUID(0),
    m_uiStatueCounter(0),
    m_uiProtectorsRemaining(0)
{
    Initialize();
}

void instance_sunken_temple::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_sunken_temple::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_JAMMALAN_BARRIER:
            m_uiJammalanBarrierGUID = pGo->GetGUID();
            if (m_auiEncounter[1] == DONE)
                DoUseDoorOrButton(m_uiJammalanBarrierGUID);
            break;
        case GO_IDOL_OF_HAKKAR:
            m_uiIdolOfHakkarGUID = pGo->GetGUID();
            break;
        case GO_ATALAI_LIGHT_BIG:
            m_luiBigLightGUIDs.push_back(pGo->GetGUID());
            break;
    }
}

void instance_sunken_temple::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_ZOLO:
        case NPC_GASHER:
        case NPC_LORO:
        case NPC_HUKKU:
        case NPC_ZULLOR:
        case NPC_MIJAN:
            ++m_uiProtectorsRemaining;
            break;
        case NPC_JAMMALAN:
            m_uiJammalanGUID = pCreature->GetGUID();
            break;
        case NPC_ATALARION:
            m_uiAtalarionGUID = pCreature->GetGUID();
            break;
    }
}

void instance_sunken_temple::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ATALARION: SetData(TYPE_ATALARION, DONE); break;
        case NPC_JAMMALAN:  SetData(TYPE_JAMMALAN, DONE);  break;
        // Jammalain mini-bosses
        case NPC_ZOLO:
        case NPC_GASHER:
        case NPC_LORO:
        case NPC_HUKKU:
        case NPC_ZULLOR:
        case NPC_MIJAN:
            SetData(TYPE_PROTECTORS, DONE);
            break;
    }
}

void instance_sunken_temple::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ATALARION:
            if (uiData == SPECIAL)
                DoSpawnAtalarionIfCan();
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_PROTECTORS:
            if (uiData == DONE)
            {
                --m_uiProtectorsRemaining;
                if (!m_uiProtectorsRemaining)
                {
                    m_auiEncounter[1] = uiData;
                    DoUseDoorOrButton(m_uiJammalanBarrierGUID);
                    // Intro yell
                    if (Creature* pJammalan = instance->GetCreature(m_uiJammalanGUID))
                        DoScriptText(SAY_JAMMALAN_INTRO, pJammalan);
                }
            }
            break;
        case TYPE_JAMMALAN:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_MALFURION:
            m_auiEncounter[3] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];
        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_sunken_temple::DoSpawnAtalarionIfCan()
{
    // return if already summoned
    if (m_uiAtalarionGUID)
        return;

    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    pPlayer->SummonCreature(NPC_ATALARION, aSunkenTempleLocation[0].m_fX, aSunkenTempleLocation[0].m_fY, aSunkenTempleLocation[0].m_fZ, aSunkenTempleLocation[0].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);

    // Spawn the idol of Hakkar
    DoRespawnGameObject(m_uiIdolOfHakkarGUID, 30 * MINUTE);

    // Spawn the big green lights
    for (GUIDList::const_iterator itr = m_luiBigLightGUIDs.begin(); itr != m_luiBigLightGUIDs.end(); ++itr)
        DoRespawnGameObject(*itr, 30*MINUTE*IN_MILLISECONDS);
}

bool instance_sunken_temple::ProcessStatueEvent(uint32 uiEventId)
{
    bool bEventStatus = false;

    // Check if the statues are activated correctly
    // Increase the counter when the correct statue is activated
    for (uint8 i = 0; i < MAX_STATUES; ++i)
    {
        if (uiEventId == m_aAtalaiStatueEvents[i] && m_uiStatueCounter == i)
        {
            // Right Statue activated
            ++m_uiStatueCounter;
            bEventStatus = true;
            break;
        }
    }

    if (!bEventStatus)
        return false;

    // Check if all statues are active
    if (m_uiStatueCounter == MAX_STATUES)
        SetData(TYPE_ATALARION, SPECIAL);

    return true;
}

void instance_sunken_temple::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        // Here a bit custom, to have proper mechanics for the statue events
        if (m_auiEncounter[i] != DONE)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_sunken_temple::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_ATALARION:
            return m_auiEncounter[0];
        case TYPE_PROTECTORS:
            return m_auiEncounter[1];
        case TYPE_JAMMALAN:
            return m_auiEncounter[2];
        case TYPE_MALFURION:
            return m_auiEncounter[3];
    }
    return 0;
}

InstanceData* GetInstanceData_instance_sunken_temple(Map* pMap)
{
    return new instance_sunken_temple(pMap);
}

void AddSC_instance_sunken_temple()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_sunken_temple";
    pNewScript->GetInstanceData = &GetInstanceData_instance_sunken_temple;
    pNewScript->RegisterSelf();
}
