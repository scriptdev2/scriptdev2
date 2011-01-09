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
SDName: Instance_Old_Hillsbrad
SD%Complete: 75
SDComment: If thrall escort fail, all parts will reset. In future, save sub-parts and continue from last known.
SDCategory: Caverns of Time, Old Hillsbrad Foothills
EndScriptData */

#include "precompiled.h"
#include "old_hillsbrad.h"

instance_old_hillsbrad::instance_old_hillsbrad(Map* pMap) : ScriptedInstance(pMap),
    m_uiBarrelCount(0),
    m_uiThrallEventCount(0),
    m_uiThrallGUID(0),
    m_uiTarethaGUID(0),
    m_uiScarlocGUID(0),
    m_uiEpochGUID(0)
{
    Initialize();
}

void instance_old_hillsbrad::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_old_hillsbrad::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_THRALL:
            m_uiThrallGUID = pCreature->GetGUID();
            break;
        case NPC_TARETHA:
            m_uiTarethaGUID = pCreature->GetGUID();
            break;
        case NPC_EPOCH:
            m_uiEpochGUID = pCreature->GetGUID();
            break;
    }
}

void instance_old_hillsbrad::OnCreatureDeath(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_EPOCH)
    {
        // notify thrall so he can continue
        if (Creature* pThrall = instance->GetCreature(m_uiThrallGUID))
            pThrall->AI()->KilledUnit(pCreature);
    }
}

void instance_old_hillsbrad::HandleThrallRelocation()
{
    if (Creature* pThrall = instance->GetCreature(m_uiThrallGUID))
    {
        debug_log("SD2: Instance Old Hillsbrad: Thrall relocation");

        if (m_auiEncounter[TYPE_THRALL_PART4] == IN_PROGRESS)
        {
            // boss failed, reloc to inn
            pThrall->GetMap()->CreatureRelocation(pThrall, 2660.57f, 659.173f, 61.9370f, 0.0f);
            m_auiEncounter[TYPE_THRALL_PART4] = NOT_STARTED;
        }
        else if (m_auiEncounter[TYPE_THRALL_PART3] == IN_PROGRESS)
        {
            // barn to inn failed, reloc to inn
            pThrall->GetMap()->CreatureRelocation(pThrall, 2660.57f, 659.173f, 61.9370f, 0.0f);
            m_auiEncounter[TYPE_THRALL_PART3] = DONE;
        }
        else if (m_auiEncounter[TYPE_THRALL_PART2] == IN_PROGRESS)
        {
            // keep to barn failed, reloc to barn
            pThrall->GetMap()->CreatureRelocation(pThrall, 2486.91f, 626.356f, 58.0761f, 0.0f);
            m_auiEncounter[TYPE_THRALL_PART2] = DONE;
        }
        else if (m_auiEncounter[TYPE_THRALL_PART1] == IN_PROGRESS)
        {
            // didn't reach very far, back to the basement using default
            m_auiEncounter[TYPE_THRALL_PART1] = NOT_STARTED;
        }
    }
}

void instance_old_hillsbrad::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_BARREL_DIVERSION:
        {
            if (uiData == IN_PROGRESS)
            {
                if (m_uiBarrelCount >= 5)
                    return;

                ++m_uiBarrelCount;
                DoUpdateWorldState(WORLD_STATE_OH, m_uiBarrelCount);

                debug_log("SD2: Instance Old Hillsbrad: go_barrel_old_hillsbrad count %u", m_uiBarrelCount);

                m_auiEncounter[TYPE_BARREL_DIVERSION] = IN_PROGRESS;

                if (m_uiBarrelCount == 5)
                {
                    UpdateLodgeQuestCredit();

                    if (Player* pPlayer = GetPlayerInMap())
                        pPlayer->SummonCreature(NPC_DRAKE, 2128.43f, 71.01f, 64.42f, 1.74f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000);
                    else
                        debug_log("SD2: Instance Old Hillsbrad: SetData (Type: %u Data %u) cannot find any pPlayer.", uiType, uiData);

                    m_auiEncounter[TYPE_BARREL_DIVERSION] = DONE;
                }
            }
            break;
        }
        case TYPE_THRALL_EVENT:
        {
            // nothing to do if already done and thrall respawn
            if (m_auiEncounter[TYPE_THRALL_EVENT] == DONE)
                return;

            if (uiData == FAIL)
            {
                if (m_uiThrallEventCount <= 20)
                {
                    ++m_uiThrallEventCount;
                    debug_log("SD2: Instance Old Hillsbrad: Thrall event failed %u times.", m_uiThrallEventCount);

                    HandleThrallRelocation();
                }
                else if (m_uiThrallEventCount > 20)
                {
                    m_auiEncounter[TYPE_THRALL_EVENT] = uiData;
                    m_auiEncounter[TYPE_THRALL_PART1] = uiData;
                    m_auiEncounter[TYPE_THRALL_PART2] = uiData;
                    m_auiEncounter[TYPE_THRALL_PART3] = uiData;
                    m_auiEncounter[TYPE_THRALL_PART4] = uiData;
                    debug_log("SD2: Instance Old Hillsbrad: Thrall event failed %u times. Reset instance required.", m_uiThrallEventCount);
                }
            }
            else
                m_auiEncounter[TYPE_THRALL_EVENT] = uiData;

            debug_log("SD2: Instance Old Hillsbrad: Thrall escort event adjusted to data %u.",uiData);
            break;
        }
        case TYPE_THRALL_PART1:
            m_auiEncounter[TYPE_THRALL_PART1] = uiData;
            debug_log("SD2: Instance Old Hillsbrad: Thrall event part I adjusted to data %u.",uiData);
            break;
        case TYPE_THRALL_PART2:
            m_auiEncounter[TYPE_THRALL_PART2] = uiData;
            debug_log("SD2: Instance Old Hillsbrad: Thrall event part II adjusted to data %u.",uiData);
            break;
        case TYPE_THRALL_PART3:
            m_auiEncounter[TYPE_THRALL_PART3] = uiData;
            debug_log("SD2: Instance Old Hillsbrad: Thrall event part III adjusted to data %u.",uiData);
            break;
        case TYPE_THRALL_PART4:
            m_auiEncounter[TYPE_THRALL_PART4] = uiData;
            debug_log("SD2: Instance Old Hillsbrad: Thrall event part IV adjusted to data %u.",uiData);
            break;
    }
}

uint32 instance_old_hillsbrad::GetData(uint32 uiData)
{
    switch(uiData)
    {
        case TYPE_BARREL_DIVERSION:
            return m_auiEncounter[TYPE_BARREL_DIVERSION];
        case TYPE_THRALL_EVENT:
            return m_auiEncounter[TYPE_THRALL_EVENT];
        case TYPE_THRALL_PART1:
            return m_auiEncounter[TYPE_THRALL_PART1];
        case TYPE_THRALL_PART2:
            return m_auiEncounter[TYPE_THRALL_PART2];
        case TYPE_THRALL_PART3:
            return m_auiEncounter[TYPE_THRALL_PART3];
        case TYPE_THRALL_PART4:
            return m_auiEncounter[TYPE_THRALL_PART4];
        default:
            return 0;
    }
}

uint64 instance_old_hillsbrad::GetData64(uint32 uiData)
{
    return 0;
}

void instance_old_hillsbrad::UpdateLodgeQuestCredit()
{
    Map::PlayerList const& players = instance->GetPlayers();

    if (!players.isEmpty())
    {
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                pPlayer->KilledMonsterCredit(NPC_LODGE_QUEST_TRIGGER);
        }
    }
}

InstanceData* GetInstanceData_instance_old_hillsbrad(Map* pMap)
{
    return new instance_old_hillsbrad(pMap);
}

void AddSC_instance_old_hillsbrad()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_old_hillsbrad";
    pNewScript->GetInstanceData = &GetInstanceData_instance_old_hillsbrad;
    pNewScript->RegisterSelf();
}
