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
SDName: Instance_Mechanar
SD%Complete: 70
SDComment: Elevator needs core support
SDCategory: Mechanar
EndScriptData */

#include "precompiled.h"
#include "mechanar.h"

instance_mechanar::instance_mechanar(Map* pMap) : ScriptedInstance(pMap),
    m_uiBridgeEventTimer(0),
    m_uiBridgeEventPhase(0)
{
    Initialize();
}

void instance_mechanar::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_mechanar::OnPlayerEnter(Player* pPlayer)
{
    // Check encounter states
    if (GetData(TYPE_SEPETHREA) != DONE || GetData(TYPE_PATHALEON) == DONE)
        return;

    // Check if already summoned
    if (GetSingleCreatureFromStorage(NPC_PATHALEON, true))
        return;

    pPlayer->SummonCreature(NPC_PATHALEON, aBridgeLocs[6].m_fX, aBridgeLocs[6].m_fY, aBridgeLocs[6].m_fZ, aBridgeLocs[6].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
}

void instance_mechanar::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_PATHALEON:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
        case NPC_ASTROMAGE:
        case NPC_PHYSICIAN:
        case NPC_CENTURION:
        case NPC_ENGINEER:
        case NPC_NETHERBINDER:
        case NPC_FORGE_DESTROYER:
            if (pCreature->IsTemporarySummon())
                m_sBridgeTrashGuidSet.insert(pCreature->GetObjectGuid());
            break;
    }
}

void instance_mechanar::OnObjectCreate(GameObject* pGo)
{
    if (pGo->GetEntry() == GO_FACTORY_ELEVATOR)
    {
        // ToDo: activate elevator if TYPE_GYRO_KILL && TYPE_IRON_HAND && TYPE_CAPACITUS are DONE
        m_mGoEntryGuidStore[GO_FACTORY_ELEVATOR] = pGo->GetObjectGuid();
    }
}

void instance_mechanar::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_GYRO_KILL:
        case TYPE_IRON_HAND:
        case TYPE_CAPACITUS:
            m_auiEncounter[uiType] = uiData;
            // ToDo: Activate the Elevator when all these 3 are done
            break;
        case TYPE_SEPETHREA:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                m_uiBridgeEventTimer = 10000;
            break;
        case TYPE_PATHALEON:
            m_auiEncounter[uiType] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3]  << " " << m_auiEncounter[4];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_mechanar::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_mechanar::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
    >> m_auiEncounter[4];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_mechanar::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_GYRO_KILL:      SetData(TYPE_GYRO_KILL, DONE); break;
        case NPC_IRON_HAND:      SetData(TYPE_IRON_HAND, DONE); break;
        case NPC_LORD_CAPACITUS: SetData(TYPE_CAPACITUS, DONE); break;

        case NPC_ASTROMAGE:
        case NPC_PHYSICIAN:
        case NPC_CENTURION:
        case NPC_ENGINEER:
        case NPC_NETHERBINDER:
        case NPC_FORGE_DESTROYER:
            if (m_sBridgeTrashGuidSet.find(pCreature->GetObjectGuid()) != m_sBridgeTrashGuidSet.end())
            {
                m_sBridgeTrashGuidSet.erase(pCreature->GetObjectGuid());

                if (m_sBridgeTrashGuidSet.empty())
                {
                    // After the 3rd wave wait 10 seconds
                    if (m_uiBridgeEventPhase == 3)
                        m_uiBridgeEventTimer = 10000;
                    else
                        DoSpawnBridgeWave();
                }
            }
            break;
    }
}

void instance_mechanar::DoSpawnBridgeWave()
{
    if (Player* pPlayer = GetPlayerInMap(true, false))
    {
        // For these wave ids summon only 1 npc
        if (m_uiBridgeEventPhase == 6 || m_uiBridgeEventPhase == 4 || m_uiBridgeEventPhase == 1)
        {
            if (Creature* pTemp = pPlayer->SummonCreature(aBridgeEventNpcs[m_uiBridgeEventPhase][0], aBridgeLocs[m_uiBridgeEventPhase].m_fX, aBridgeLocs[m_uiBridgeEventPhase].m_fY, aBridgeLocs[m_uiBridgeEventPhase].m_fZ, aBridgeLocs[m_uiBridgeEventPhase].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0))
            {
                pTemp->CastSpell(pTemp, SPELL_SIMPLE_TELEPORT, false);

                // Yell intro when Pathaleon is summoned
                if (m_uiBridgeEventPhase == 6)
                    DoScriptText(SAY_PATHALEON_INTRO, pTemp);
                // The Forge Destroyers need to attack the player
                else
                    pTemp->AI()->AttackStart(pPlayer);
            }
        }
        else
        {
            float fX, fY, fZ;
            for (uint8 i = 0; i < MAX_BRIDGE_TRASH; ++i)
            {
                if (!aBridgeEventNpcs[m_uiBridgeEventPhase][i])
                    continue;

                // Choose a random point to summon the npcs
                pPlayer->GetRandomPoint(aBridgeLocs[m_uiBridgeEventPhase].m_fX, aBridgeLocs[m_uiBridgeEventPhase].m_fY, aBridgeLocs[m_uiBridgeEventPhase].m_fZ, 5.0f, fX, fY, fZ);
                if (Creature* pTemp = pPlayer->SummonCreature(aBridgeEventNpcs[m_uiBridgeEventPhase][i], fX, fY, fZ, aBridgeLocs[m_uiBridgeEventPhase].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0))
                {
                    pTemp->CastSpell(pTemp, SPELL_SIMPLE_TELEPORT, false);

                    // They start attacking the players, except for the ones from phase 0 and 3
                    if (m_uiBridgeEventPhase != 0 && m_uiBridgeEventPhase != 3)
                        pTemp->AI()->AttackStart(pPlayer);
                }
            }
        }
    }
    ++m_uiBridgeEventPhase;
}

void instance_mechanar::Update(uint32 uiDiff)
{
    if (m_uiBridgeEventTimer)
    {
        if (m_uiBridgeEventTimer <= uiDiff)
        {
            DoSpawnBridgeWave();
            m_uiBridgeEventTimer = 0;
        }
        else
            m_uiBridgeEventTimer -= uiDiff;
    }
}

InstanceData* GetInstanceData_instance_mechanar(Map* pMap)
{
    return new instance_mechanar(pMap);
}

void AddSC_instance_mechanar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_mechanar";
    pNewScript->GetInstanceData = &GetInstanceData_instance_mechanar;
    pNewScript->RegisterSelf();
}
