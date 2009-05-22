/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
#include "def_zulaman.h"

struct MANGOS_DLL_DECL instance_zulaman : public ScriptedInstance
{
    instance_zulaman(Map* pMap) : ScriptedInstance(pMap) {Initialize();}

    std::string strInstData;
    uint32 m_uiEventTimer;
    uint32 m_uiEventMinuteStep;

    uint32 m_uiGongCount;

    uint64 m_uiAkilzonGUID;
    uint64 m_uiNalorakkGUID;
    uint64 m_uiJanalaiGUID;
    uint64 m_uiHalazziGUID;
    uint64 m_uiZuljinGUID;
    uint64 m_uiMalacrassGUID;
    uint64 m_uiHarrisonGUID;

    uint64 m_uiStrangeGongGUID;
    uint64 m_uiMassiveGateGUID;
    uint64 m_uiMalacrassEntranceGUID;

    uint32 m_uiJanalaiEggCntL;
    uint32 m_uiJanalaiEggCntR;

    uint32 m_uiEncounter[ENCOUNTERS];
    uint32 m_uiRandVendor[RAND_VENDOR];

    void Initialize()
    {
        m_uiEventTimer = MINUTE*IN_MILISECONDS;
        m_uiEventMinuteStep = MINUTE/3;

        m_uiGongCount = 0;

        m_uiAkilzonGUID = 0;
        m_uiNalorakkGUID = 0;
        m_uiJanalaiGUID = 0;
        m_uiHalazziGUID = 0;
        m_uiZuljinGUID = 0;
        m_uiMalacrassGUID = 0;
        m_uiHarrisonGUID = 0;

        m_uiStrangeGongGUID = 0;
        m_uiMassiveGateGUID = 0;
        m_uiMalacrassEntranceGUID = 0;

        m_uiJanalaiEggCntL = 20;
        m_uiJanalaiEggCntR = 20;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            m_uiEncounter[i] = NOT_STARTED;

        for(uint8 i = 0; i < RAND_VENDOR; i++)
            m_uiRandVendor[i] = NOT_STARTED;
    }

    void UpdateInstanceWorldState(uint32 uiId, uint32 uiState)
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* pPlayer = itr->getSource())
                {
                    pPlayer->SendUpdateWorldState(uiId, uiState);
                    debug_log("SD2: Instance Zulaman: UpdateInstanceWorldState for id %u with state %u",uiId,uiState);
                }
            }
        }
        else
            debug_log("SD2: Instance Zulaman: UpdateInstanceWorldState, but PlayerList is empty.");
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
                        m_uiEncounter[0] = uiData;
                }
                if (uiData == IN_PROGRESS)
                {
                    DoUseDoorOrButton(m_uiMassiveGateGUID);
                    UpdateInstanceWorldState(WORLD_STATE_COUNTER,m_uiEventMinuteStep);
                    UpdateInstanceWorldState(WORLD_STATE_ID,1);
                    m_uiEncounter[0] = uiData;
                }
                break;
            case TYPE_AKILZON:
                if (uiData == DONE)
                {
                    if (m_uiEncounter[0] == IN_PROGRESS)
                    {
                        m_uiEventMinuteStep += MINUTE/6;    //add 10 minutes
                        UpdateInstanceWorldState(WORLD_STATE_COUNTER,m_uiEventMinuteStep);
                    }
                }
                m_uiEncounter[1] = uiData;
                break;
            case TYPE_NALORAKK:
                if (uiData == DONE)
                {
                    if (m_uiEncounter[0] == IN_PROGRESS)
                    {
                        m_uiEventMinuteStep += MINUTE/4;    //add 15 minutes
                        UpdateInstanceWorldState(WORLD_STATE_COUNTER,m_uiEventMinuteStep);
                    }
                }
                m_uiEncounter[2] = uiData;
                break;
            case TYPE_JANALAI:
                if (uiData == NOT_STARTED)
                {
                    m_uiJanalaiEggCntL = 20;
                    m_uiJanalaiEggCntR = 20;
                }
                m_uiEncounter[3] = uiData;
                break;
            case TYPE_HALAZZI:
                m_uiEncounter[4] = uiData;
                break;
            case TYPE_ZULJIN:
                m_uiEncounter[5] = uiData;
                break;
            case TYPE_MALACRASS:
                m_uiEncounter[6] = uiData;
                break;
            case DATA_J_HATCHLEFT:
                m_uiJanalaiEggCntL -= uiData;
                break;
            case DATA_J_HATCHRIGHT:
                m_uiJanalaiEggCntR -= uiData;
                break;
            case TYPE_RAND_VENDOR_1:
                m_uiRandVendor[0] = uiData;
                break;
            case TYPE_RAND_VENDOR_2:
                m_uiRandVendor[1] = uiData;
                break;
            default:
                error_log("SD2: Instance Zulaman: ERROR SetData = %u for type %u does not exist/not implemented.",uiType,uiData);
                break;
        }

        if (uiData == DONE || (uiType == TYPE_EVENT_RUN && uiData == IN_PROGRESS))
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_uiEncounter[0] << " " << m_uiEncounter[1] << " " << m_uiEncounter[2] << " "
                << m_uiEncounter[3] << " " << m_uiEncounter[4] << " " << m_uiEncounter[5] << " " << m_uiEncounter[6];

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
        loadStream >> m_uiEncounter[0] >> m_uiEncounter[1] >> m_uiEncounter[2] >> m_uiEncounter[3]
            >> m_uiEncounter[4] >> m_uiEncounter[5] >> m_uiEncounter[6];

        //not changing m_uiEncounter[0], TYPE_EVENT_RUN must not reset to NOT_STARTED
        for(uint8 i = 1; i < ENCOUNTERS; ++i)
        {
            if (m_uiEncounter[i] == IN_PROGRESS)
                m_uiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_EVENT_RUN:
                return m_uiEncounter[0];
            case TYPE_AKILZON:
                return m_uiEncounter[1];
            case TYPE_NALORAKK:
                return m_uiEncounter[2];
            case TYPE_JANALAI:
                return m_uiEncounter[3];
            case TYPE_HALAZZI:
                return m_uiEncounter[4];
            case TYPE_ZULJIN:
                return m_uiEncounter[5];
            case TYPE_MALACRASS:
                return m_uiEncounter[6];

            case DATA_J_EGGSLEFT:
                return m_uiJanalaiEggCntL;
            case DATA_J_EGGSRIGHT:
                return m_uiJanalaiEggCntR;

            case TYPE_RAND_VENDOR_1:
                return m_uiRandVendor[0];
            case TYPE_RAND_VENDOR_2:
                return m_uiRandVendor[1];
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
                    m_uiEncounter[0] = FAIL;
                    return;
                }

                --m_uiEventMinuteStep;
                UpdateInstanceWorldState(WORLD_STATE_COUNTER, m_uiEventMinuteStep);
                debug_log("SD2: Instance Zulaman: minute decrease to %u.",m_uiEventMinuteStep);

                m_uiEventTimer = MINUTE*IN_MILISECONDS;
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
