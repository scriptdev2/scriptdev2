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
SDName: Instance_Zulaman
SD%Complete: 50
SDComment: Support for Quests and Mini-Events still TODO
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"

instance_zulaman::instance_zulaman(Map* pMap) : ScriptedInstance(pMap),
    m_uiEventTimer(MINUTE*IN_MILLISECONDS),
    m_uiGongCount(0),

    m_uiAkilzonGUID(0),
    m_uiNalorakkGUID(0),
    m_uiJanalaiGUID(0),
    m_uiHalazziGUID(0),
    m_uiSpiritLynxGUID(0),
    m_uiZuljinGUID(0),
    m_uiMalacrassGUID(0),
    m_uiHarrisonGUID(0),

    m_uiStrangeGongGUID(0),
    m_uiMassiveGateGUID(0),
    m_uiWindDoorGUID(0),
    m_uiLynxTempleEntranceGUID(0),
    m_uiLynxTempleExitGUID(0),
    m_uiMalacrassEntranceGUID(0),
    m_uiWoodenDoorGUID(0),
    m_uiFireDoorGUID(0),

    m_uiEggsRemainingCount_Left(20),
    m_uiEggsRemainingCount_Right(20)
{
    Initialize();
}

void instance_zulaman::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiRandVendor, 0, sizeof(m_auiRandVendor));
}

bool instance_zulaman::IsEncounterInProgress() const
{
    // Skip Time-Event and Time-Event timer
    for (uint8 i = 1; i < MAX_ENCOUNTER - 1; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_zulaman::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_AKILZON:     m_uiAkilzonGUID     = pCreature->GetGUID(); break;
        case NPC_NALORAKK:    m_uiNalorakkGUID    = pCreature->GetGUID(); break;
        case NPC_JANALAI:     m_uiJanalaiGUID     = pCreature->GetGUID(); break;
        case NPC_HALAZZI:     m_uiHalazziGUID     = pCreature->GetGUID(); break;
        case NPC_MALACRASS:   m_uiMalacrassGUID   = pCreature->GetGUID(); break;
        case NPC_ZULJIN:      m_uiZuljinGUID      = pCreature->GetGUID(); break;
        case NPC_HARRISON:    m_uiHarrisonGUID    = pCreature->GetGUID(); break;
        case NPC_SPIRIT_LYNX: m_uiSpiritLynxGUID  = pCreature->GetGUID(); break;

        case NPC_TANZAR:      m_aEventNpcInfo[INDEX_NALORAKK].npGuid = pCreature->GetObjectGuid(); break;
        case NPC_KRAZ:        m_aEventNpcInfo[INDEX_JANALAI].npGuid =  pCreature->GetObjectGuid(); break;
        case NPC_ASHLI:       m_aEventNpcInfo[INDEX_HALAZZI].npGuid =  pCreature->GetObjectGuid(); break;
        case NPC_HARKOR:      m_aEventNpcInfo[INDEX_AKILZON].npGuid =  pCreature->GetObjectGuid(); break;

        case NPC_EGG:
            if (m_auiEncounter[3] != DONE)
                m_lEggsGUIDList.push_back(pCreature->GetGUID());
            break;
    }
}

void instance_zulaman::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_STRANGE_GONG:
            m_uiStrangeGongGUID = pGo->GetGUID();
            break;
        case GO_MASSIVE_GATE:
            m_uiMassiveGateGUID = pGo->GetGUID();
            if (m_auiEncounter[0] == DONE || m_auiEncounter[0] == FAIL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_WIND_DOOR:
            m_uiWindDoorGUID = pGo->GetGUID();
            break;
        case GO_LYNX_TEMPLE_ENTRANCE:
            m_uiLynxTempleEntranceGUID = pGo->GetGUID();
            break;
        case GO_LYNX_TEMPLE_EXIT:
            m_uiLynxTempleExitGUID = pGo->GetGUID();
            if (m_auiEncounter[4] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HEXLORD_ENTRANCE:
            m_uiMalacrassEntranceGUID = pGo->GetGUID();
            if (GetKilledPreBosses() == 4)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_WOODEN_DOOR:
            m_uiWoodenDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[5] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_FIRE_DOOR:
            m_uiFireDoorGUID = pGo->GetGUID();
            break;
    }
}

void instance_zulaman::SetData(uint32 uiType, uint32 uiData)
{
    debug_log("SD2: Instance Zulaman: SetData received for type %u with data %u",uiType,uiData);

    switch(uiType)
    {
        case TYPE_EVENT_RUN:
            if (uiData == SPECIAL)
            {
                ++m_uiGongCount;
                if (m_uiGongCount == 5)
                    m_auiEncounter[0] = uiData;
                return;
            }
            if (uiData == IN_PROGRESS)
            {
                DoTimeRunSay(RUN_START);
                DoUseDoorOrButton(m_uiMassiveGateGUID);
                if (m_auiEncounter[7])
                    SetData(TYPE_RUN_EVENT_TIME, m_auiEncounter[7]);
                else
                    SetData(TYPE_RUN_EVENT_TIME, 20);   // 20 Minutes as default time
                DoUpdateWorldState(WORLD_STATE_ID, 1);
            }
            if (uiData == FAIL)
            {
                DoTimeRunSay(RUN_FAIL);
                DoUpdateWorldState(WORLD_STATE_ID, 0);
                // Kill remaining Event NPCs
                for (uint8 i = 0; i < MAX_CHESTS; ++i)
                {
                    // Not yet rescued, so too late
                    if (!m_aEventNpcInfo[i].uiSavePosition)
                    {
                        if (Creature* pCreature = instance->GetCreature(m_aEventNpcInfo[i].npGuid))
                            pCreature->ForcedDespawn();
                    }
                }
            }
            if (uiData == DONE)
            {
                DoTimeRunSay(RUN_DONE);
                DoUpdateWorldState(WORLD_STATE_ID, 0);
            }
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_AKILZON:
            DoUseDoorOrButton(m_uiWindDoorGUID);
            if (uiData == DONE)
            {
                if (m_auiEncounter[0] == IN_PROGRESS)
                {
                    m_auiEncounter[7] += 10;            // Add 10 minutes
                    SetData(TYPE_RUN_EVENT_TIME, m_auiEncounter[7]);
                    DoChestEvent(INDEX_AKILZON);
                }
            }
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_NALORAKK:
            if (uiData == DONE)
            {
                if (m_auiEncounter[0] == IN_PROGRESS)
                {
                    m_auiEncounter[7] += 15;            // Add 15 minutes
                    SetData(TYPE_RUN_EVENT_TIME, m_auiEncounter[7]);
                    DoChestEvent(INDEX_NALORAKK);
                }
            }
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_JANALAI:
            if (uiData == NOT_STARTED)
            {
                m_uiEggsRemainingCount_Left = 20;
                m_uiEggsRemainingCount_Right = 20;

                if (!m_lEggsGUIDList.empty())
                {
                    for(GUIDList::const_iterator itr = m_lEggsGUIDList.begin(); itr != m_lEggsGUIDList.end(); ++itr)
                    {
                        if (Creature* pEgg = instance->GetCreature(*itr))
                        {
                            if (!pEgg->isAlive())
                                pEgg->Respawn();
                        }
                    }
                }
            }
            if (uiData == DONE)
            {
                m_lEggsGUIDList.clear();

                if (m_auiEncounter[0] == IN_PROGRESS)
                    DoChestEvent(INDEX_JANALAI);
            }
            m_auiEncounter[3] = uiData;
            break;
        case TYPE_HALAZZI:
            DoUseDoorOrButton(m_uiLynxTempleEntranceGUID);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiLynxTempleExitGUID);
                if (m_auiEncounter[0] == IN_PROGRESS)
                    DoChestEvent(INDEX_HALAZZI);
            }
            m_auiEncounter[4] = uiData;
            break;
        case TYPE_MALACRASS:
            DoUseDoorOrButton(m_uiMalacrassEntranceGUID);
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiWoodenDoorGUID);
            m_auiEncounter[5] = uiData;
            break;
        case TYPE_ZULJIN:
            DoUseDoorOrButton(m_uiFireDoorGUID);
            m_auiEncounter[6] = uiData;
            break;
        case TYPE_J_EGGS_RIGHT:
            --m_uiEggsRemainingCount_Right;
            break;
        case TYPE_J_EGGS_LEFT:
            --m_uiEggsRemainingCount_Left;
            break;
        case TYPE_RAND_VENDOR_1:
            m_auiRandVendor[0] = uiData;
            break;
        case TYPE_RAND_VENDOR_2:
            m_auiRandVendor[1] = uiData;
            break;

        case TYPE_RUN_EVENT_TIME:
            m_auiEncounter[7] = uiData;
            DoUpdateWorldState(WORLD_STATE_COUNTER, m_auiEncounter[7]);
            break;

        default:
            error_log("SD2: Instance Zulaman: ERROR SetData = %u for type %u does not exist/not implemented.",uiType,uiData);
            return;
    }

    if (uiData == DONE && GetKilledPreBosses() == 4 && (uiType == TYPE_AKILZON || uiType == TYPE_NALORAKK || uiType == TYPE_JANALAI || uiType == TYPE_HALAZZI))
    {
        DoUseDoorOrButton(m_uiMalacrassEntranceGUID);
        if (m_auiEncounter[0] == IN_PROGRESS)
            SetData(TYPE_EVENT_RUN, DONE);
    }

    if (uiData == DONE || uiType == TYPE_RUN_EVENT_TIME || uiType == TYPE_EVENT_RUN)
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

void instance_zulaman::Load(const char* chrIn)
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

    // Skip m_auiEncounter[7], to start the time event properly if needed
    for (uint8 i = 0; i < MAX_ENCOUNTER - 1; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    // Restart TYPE_EVENT_RUN if was already started
    if (m_auiEncounter[7] != 0 && m_auiEncounter[0] != DONE && m_auiEncounter[0] != FAIL)
        SetData(TYPE_EVENT_RUN, IN_PROGRESS);

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_zulaman::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_EVENT_RUN:     return m_auiEncounter[0];
        case TYPE_AKILZON:       return m_auiEncounter[1];
        case TYPE_NALORAKK:      return m_auiEncounter[2];
        case TYPE_JANALAI:       return m_auiEncounter[3];
        case TYPE_HALAZZI:       return m_auiEncounter[4];
        case TYPE_ZULJIN:        return m_auiEncounter[5];
        case TYPE_MALACRASS:     return m_auiEncounter[6];
        case TYPE_J_EGGS_LEFT:   return m_uiEggsRemainingCount_Left;
        case TYPE_J_EGGS_RIGHT:  return m_uiEggsRemainingCount_Right;
        case TYPE_RAND_VENDOR_1: return m_auiRandVendor[0];
        case TYPE_RAND_VENDOR_2: return m_auiRandVendor[1];

        default:
            return 0;
    }
}

uint64 instance_zulaman::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_AKILZON:         return m_uiAkilzonGUID;
        case NPC_NALORAKK:        return m_uiNalorakkGUID;
        case NPC_JANALAI:         return m_uiJanalaiGUID;
        case NPC_HALAZZI:         return m_uiHalazziGUID;
        case NPC_SPIRIT_LYNX:     return m_uiSpiritLynxGUID;
        case NPC_ZULJIN:          return m_uiZuljinGUID;
        case NPC_MALACRASS:       return m_uiMalacrassGUID;
        case NPC_HARRISON:        return m_uiHarrisonGUID;

        case GO_STRANGE_GONG:     return m_uiStrangeGongGUID;
        case GO_MASSIVE_GATE:     return m_uiMassiveGateGUID;
        case GO_HEXLORD_ENTRANCE: return m_uiMalacrassEntranceGUID;

        default:
            return 0;
    }
}

uint8 instance_zulaman::GetKilledPreBosses()
{
    return (GetData(TYPE_AKILZON) == DONE ? 1 : 0) + (GetData(TYPE_NALORAKK) == DONE ? 1 : 0) + (GetData(TYPE_JANALAI) == DONE ? 1 : 0) + (GetData(TYPE_HALAZZI) == DONE ? 1 : 0);
}

void instance_zulaman::DoTimeRunSay(RunEventSteps uiData)
{
    // TODO - This yell must be made independend of load state of Malacrass
    Creature* pHexlord = instance->GetCreature(m_uiMalacrassGUID);
    if (!pHexlord)
        return;

    switch (uiData)
    {
        case RUN_START:     DoScriptText(SAY_INST_BEGIN, pHexlord); break;
        case RUN_FAIL:      DoScriptText(urand(0, 1) ? SAY_INST_SACRIF1 : SAY_INST_SACRIF2, pHexlord); break;
        case RUN_DONE:      DoScriptText(SAY_INST_COMPLETE, pHexlord); break;
        case RUN_PROGRESS:
            // This function is on progress called before the data is set to the array
            switch (GetKilledPreBosses() + 1)
            {
                case 1:     DoScriptText(SAY_INST_PROGRESS_1, pHexlord); break;
                case 2:     DoScriptText(SAY_INST_PROGRESS_2, pHexlord); break;
                case 3:     DoScriptText(SAY_INST_PROGRESS_3, pHexlord); break;
            }
            break;
        case RUN_FAIL_SOON:
            switch (GetKilledPreBosses())
            {
                case 0:     DoScriptText(SAY_INST_WARN_1, pHexlord); break;
                case 1:     DoScriptText(SAY_INST_WARN_2, pHexlord); break;
                case 2:     DoScriptText(SAY_INST_WARN_3, pHexlord); break;
                case 3:     DoScriptText(SAY_INST_WARN_4, pHexlord); break;
            }
            break;
    }
}

void instance_zulaman::DoChestEvent(BossToChestIndex uiIndex)
{
    // Store Order of this kill
    m_aEventNpcInfo[uiIndex].uiSavePosition = GetKilledPreBosses() + 1;

    // Do Yell
    DoTimeRunSay(RUN_PROGRESS);

    // related NPC:     m_aEventNpcInfo[uiIndex].npGuid
    // related Chest:   m_aEventNpcInfo[uiIndex]        // Not yet stored, because likely unneeded
}

void instance_zulaman::Update(uint32 uiDiff)
{
    if (GetData(TYPE_EVENT_RUN) == IN_PROGRESS)
    {
        if (m_uiEventTimer <= uiDiff)
        {
            if (m_auiEncounter[7] == 5)                 // TODO, verify 5min for warning texts
                DoTimeRunSay(RUN_FAIL_SOON);

            if (m_auiEncounter[7] == 0)
            {
                debug_log("SD2: Instance Zulaman: event time reach end, event failed.");
                SetData(TYPE_EVENT_RUN, FAIL);
                return;
            }

            --m_auiEncounter[7];
            SetData(TYPE_RUN_EVENT_TIME, m_auiEncounter[7]);
            debug_log("SD2: Instance Zulaman: minute decrease to %u.", m_auiEncounter[7]);

            m_uiEventTimer = MINUTE*IN_MILLISECONDS;
        }
        else
            m_uiEventTimer -= uiDiff;
    }
}

InstanceData* GetInstanceData_instance_zulaman(Map* pMap)
{
    return new instance_zulaman(pMap);
}

void AddSC_instance_zulaman()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_zulaman";
    pNewScript->GetInstanceData = &GetInstanceData_instance_zulaman;
    pNewScript->RegisterSelf();
}
