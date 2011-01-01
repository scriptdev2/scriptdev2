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
SDName: Instance_Zulaman
SD%Complete: 25
SDComment:
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"

struct MANGOS_DLL_DECL instance_zulaman : public ScriptedInstance
{
    instance_zulaman(Map* pMap) : ScriptedInstance(pMap) {Initialize();}

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint32 m_auiRandVendor[MAX_VENDOR];
    std::string strInstData;

    uint32 m_uiEventTimer;
    uint32 m_uiEventMinuteStep;

    uint32 m_uiGongCount;

    uint64 m_uiAkilzonGUID;
    uint64 m_uiNalorakkGUID;
    uint64 m_uiJanalaiGUID;
    uint64 m_uiHalazziGUID;
    uint64 m_uiSpiritLynxGUID;
    uint64 m_uiZuljinGUID;
    uint64 m_uiMalacrassGUID;
    uint64 m_uiHarrisonGUID;

    uint64 m_uiStrangeGongGUID;
    uint64 m_uiMassiveGateGUID;
    uint64 m_uiMalacrassEntranceGUID;

    std::list<uint64> m_lEggsGUIDList;
    uint32 m_uiEggsRemainingCount_Left;
    uint32 m_uiEggsRemainingCount_Right;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
        memset(&m_auiRandVendor, 0, sizeof(m_auiRandVendor));

        m_uiEventTimer = MINUTE*IN_MILLISECONDS;
        m_uiEventMinuteStep = MINUTE/3;

        m_uiGongCount = 0;

        m_uiAkilzonGUID = 0;
        m_uiNalorakkGUID = 0;
        m_uiJanalaiGUID = 0;
        m_uiHalazziGUID = 0;
        m_uiSpiritLynxGUID = 0;
        m_uiZuljinGUID = 0;
        m_uiMalacrassGUID = 0;
        m_uiHarrisonGUID = 0;

        m_uiStrangeGongGUID = 0;
        m_uiMassiveGateGUID = 0;
        m_uiMalacrassEntranceGUID = 0;

        m_lEggsGUIDList.clear();
        m_uiEggsRemainingCount_Left = 20;
        m_uiEggsRemainingCount_Right = 20;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 23574: m_uiAkilzonGUID     = pCreature->GetGUID(); break;
            case 23576: m_uiNalorakkGUID    = pCreature->GetGUID(); break;
            case 23578: m_uiJanalaiGUID     = pCreature->GetGUID(); break;
            case 23577: m_uiHalazziGUID     = pCreature->GetGUID(); break;
            case 23863: m_uiZuljinGUID      = pCreature->GetGUID(); break;
            case 24239: m_uiMalacrassGUID   = pCreature->GetGUID(); break;
            case 24358: m_uiHarrisonGUID    = pCreature->GetGUID(); break;
            case NPC_SPIRIT_LYNX: m_uiSpiritLynxGUID  = pCreature->GetGUID(); break;
            case NPC_EGG:
                if (m_auiEncounter[3] != DONE)
                    m_lEggsGUIDList.push_back(pCreature->GetGUID());
                break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case 187359:
                m_uiStrangeGongGUID = pGo->GetGUID();
                break;
            case 186728:
                m_uiMassiveGateGUID = pGo->GetGUID();
                if (m_auiEncounter[0] == IN_PROGRESS || m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case 186305:
                m_uiMalacrassEntranceGUID = pGo->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
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
                }
                if (uiData == IN_PROGRESS)
                {
                    DoUseDoorOrButton(m_uiMassiveGateGUID);
                    DoUpdateWorldState(WORLD_STATE_COUNTER,m_uiEventMinuteStep);
                    DoUpdateWorldState(WORLD_STATE_ID,1);
                    m_auiEncounter[0] = uiData;
                }
                break;
            case TYPE_AKILZON:
                if (uiData == DONE)
                {
                    if (m_auiEncounter[0] == IN_PROGRESS)
                    {
                        m_uiEventMinuteStep += MINUTE/6;    //add 10 minutes
                        DoUpdateWorldState(WORLD_STATE_COUNTER,m_uiEventMinuteStep);
                    }
                }
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_NALORAKK:
                if (uiData == DONE)
                {
                    if (m_auiEncounter[0] == IN_PROGRESS)
                    {
                        m_uiEventMinuteStep += MINUTE/4;    //add 15 minutes
                        DoUpdateWorldState(WORLD_STATE_COUNTER,m_uiEventMinuteStep);
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
                        for(std::list<uint64>::iterator itr = m_lEggsGUIDList.begin(); itr != m_lEggsGUIDList.end(); ++itr)
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
                    m_lEggsGUIDList.clear();

                m_auiEncounter[3] = uiData;
                break;
            case TYPE_HALAZZI:
                m_auiEncounter[4] = uiData;
                break;
            case TYPE_ZULJIN:
                m_auiEncounter[5] = uiData;
                break;
            case TYPE_MALACRASS:
                m_auiEncounter[6] = uiData;
                break;

            case DATA_J_EGGS_RIGHT:
                --m_uiEggsRemainingCount_Right;
                break;
            case DATA_J_EGGS_LEFT:
                --m_uiEggsRemainingCount_Left;
                break;

            case TYPE_RAND_VENDOR_1:
                m_auiRandVendor[0] = uiData;
                break;
            case TYPE_RAND_VENDOR_2:
                m_auiRandVendor[1] = uiData;
                break;
            default:
                error_log("SD2: Instance Zulaman: ERROR SetData = %u for type %u does not exist/not implemented.",uiType,uiData);
                break;
        }

        if (m_auiEncounter[1] == DONE && m_auiEncounter[2] == DONE && m_auiEncounter[3] == DONE &&
            m_auiEncounter[4] == DONE && m_auiEncounter[5] != IN_PROGRESS)
            DoUseDoorOrButton(m_uiMalacrassEntranceGUID);

        if (uiData == DONE || (uiType == TYPE_EVENT_RUN && uiData == IN_PROGRESS))
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                << m_auiEncounter[6];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strInstData.c_str();
    }

    void Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);

        std::istringstream loadStream(chrIn);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
            >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6];

        //not changing m_uiEncounter[0], TYPE_EVENT_RUN must not reset to NOT_STARTED
        for(uint8 i = 1; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_EVENT_RUN:
                return m_auiEncounter[0];
            case TYPE_AKILZON:
                return m_auiEncounter[1];
            case TYPE_NALORAKK:
                return m_auiEncounter[2];
            case TYPE_JANALAI:
                return m_auiEncounter[3];
            case TYPE_HALAZZI:
                return m_auiEncounter[4];
            case TYPE_ZULJIN:
                return m_auiEncounter[5];
            case TYPE_MALACRASS:
                return m_auiEncounter[6];

            case DATA_J_EGGS_LEFT:
                return m_uiEggsRemainingCount_Left;
            case DATA_J_EGGS_RIGHT:
                return m_uiEggsRemainingCount_Right;

            case TYPE_RAND_VENDOR_1:
                return m_auiRandVendor[0];
            case TYPE_RAND_VENDOR_2:
                return m_auiRandVendor[1];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_AKILZON:
                return m_uiAkilzonGUID;
            case DATA_NALORAKK:
                return m_uiNalorakkGUID;
            case DATA_JANALAI:
                return m_uiJanalaiGUID;
            case DATA_HALAZZI:
                return m_uiHalazziGUID;
            case DATA_SPIRIT_LYNX:
                return m_uiSpiritLynxGUID;
            case DATA_ZULJIN:
                return m_uiZuljinGUID;
            case DATA_MALACRASS:
                return m_uiMalacrassGUID;
            case DATA_HARRISON:
                return m_uiHarrisonGUID;
            case DATA_GO_GONG:
                return m_uiStrangeGongGUID;
            case DATA_GO_ENTRANCE:
                return m_uiMassiveGateGUID;
            case DATA_GO_MALACRASS_GATE:
                return m_uiMalacrassEntranceGUID;
        }
        return 0;
    }

    void Update(uint32 uiDiff)
    {
        if (GetData(TYPE_EVENT_RUN) == IN_PROGRESS)
        {
            if (m_uiEventTimer <= uiDiff)
            {
                if (m_uiEventMinuteStep == 0)
                {
                    debug_log("SD2: Instance Zulaman: event time reach end, event failed.");
                    m_auiEncounter[0] = FAIL;
                    return;
                }

                --m_uiEventMinuteStep;
                DoUpdateWorldState(WORLD_STATE_COUNTER, m_uiEventMinuteStep);
                debug_log("SD2: Instance Zulaman: minute decrease to %u.",m_uiEventMinuteStep);

                m_uiEventTimer = MINUTE*IN_MILLISECONDS;
            }
            else
                m_uiEventTimer -= uiDiff;
        }
    }
};

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
