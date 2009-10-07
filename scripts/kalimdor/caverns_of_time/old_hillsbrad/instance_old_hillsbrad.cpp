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
SDName: Instance_Old_Hillsbrad
SD%Complete: 75
SDComment: If thrall escort fail, all parts will reset. In future, save sub-parts and continue from last known.
SDCategory: Caverns of Time, Old Hillsbrad Foothills
EndScriptData */

#include "precompiled.h"
#include "old_hillsbrad.h"

enum
{
    MAX_ENCOUNTER               = 6,

    NPC_THRALL                  = 17876,
    NPC_TARETHA                 = 18887,
    NPC_DRAKE                   = 17848,
    NPC_LODGE_QUEST_TRIGGER     = 20155,
    QUEST_ENTRY_DIVERSION       = 10283
};

struct MANGOS_DLL_DECL instance_old_hillsbrad : public ScriptedInstance
{
    instance_old_hillsbrad(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint32 m_uiBarrelCount;
    uint32 m_uiThrallEventCount;

    uint64 m_uiThrallGUID;
    uint64 m_uiTarethaGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiBarrelCount        = 0;
        m_uiThrallEventCount   = 0;
        m_uiThrallGUID         = 0;
        m_uiTarethaGUID        = 0;
    }

    Player* GetPlayerInMap()
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* plr = itr->getSource())
                    return plr;
            }
        }

        debug_log("SD2: Instance Old Hillsbrad: GetPlayerInMap, but PlayerList is empty!");
        return NULL;
    }

    void UpdateQuestCredit()
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* pPlayer = itr->getSource())
                    pPlayer->KilledMonsterCredit(NPC_LODGE_QUEST_TRIGGER, 0);
            }
        }
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_THRALL:
                m_uiThrallGUID = pCreature->GetGUID();
                break;
            case NPC_TARETHA:
                m_uiTarethaGUID = pCreature->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        Player* pPlayer = GetPlayerInMap();

        if (!pPlayer)
        {
            debug_log("SD2: Instance Old Hillsbrad: SetData (Type: %u Data %u) cannot find any pPlayer.", uiType, uiData);
            return;
        }

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

                    m_auiEncounter[0] = IN_PROGRESS;

                    if (m_uiBarrelCount == 5)
                    {
                        UpdateQuestCredit();
                        pPlayer->SummonCreature(NPC_DRAKE,2128.43,71.01,64.42,1.74,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1800000);
                        m_auiEncounter[0] = DONE;
                    }
                }
                break;
            }
            case TYPE_THRALL_EVENT:
            {
                if (uiData == FAIL)
                {
                    if (m_uiThrallEventCount <= 20)
                    {
                        ++m_uiThrallEventCount;
                        m_auiEncounter[1] = NOT_STARTED;

                        debug_log("SD2: Instance Old Hillsbrad: Thrall event failed %u times. Resetting all sub-events.", m_uiThrallEventCount);

                        m_auiEncounter[2] = NOT_STARTED;
                        m_auiEncounter[3] = NOT_STARTED;
                        m_auiEncounter[4] = NOT_STARTED;
                        m_auiEncounter[5] = NOT_STARTED;
                    }
                    else if (m_uiThrallEventCount > 20)
                    {
                        m_auiEncounter[1] = uiData;
                        m_auiEncounter[2] = uiData;
                        m_auiEncounter[3] = uiData;
                        m_auiEncounter[4] = uiData;
                        m_auiEncounter[5] = uiData;
                        debug_log("SD2: Instance Old Hillsbrad: Thrall event failed %u times. Reset instance required.", m_uiThrallEventCount);
                    }
                }
                else
                    m_auiEncounter[1] = uiData;

                debug_log("SD2: Instance Old Hillsbrad: Thrall escort event adjusted to data %u.",uiData);
                break;
            }
            case TYPE_THRALL_PART1:
                m_auiEncounter[2] = uiData;
                debug_log("SD2: Instance Old Hillsbrad: Thrall event part I adjusted to data %u.",uiData);
                break;
            case TYPE_THRALL_PART2:
                m_auiEncounter[3] = uiData;
                debug_log("SD2: Instance Old Hillsbrad: Thrall event part II adjusted to data %u.",uiData);
                break;
            case TYPE_THRALL_PART3:
                m_auiEncounter[4] = uiData;
                debug_log("SD2: Instance Old Hillsbrad: Thrall event part III adjusted to data %u.",uiData);
                break;
            case TYPE_THRALL_PART4:
                m_auiEncounter[5] = uiData;
                debug_log("SD2: Instance Old Hillsbrad: Thrall event part IV adjusted to data %u.",uiData);
                break;
        }
    }

    uint32 GetData(uint32 uiData)
    {
        switch(uiData)
        {
            case TYPE_BARREL_DIVERSION:
                return m_auiEncounter[0];
            case TYPE_THRALL_EVENT:
                return m_auiEncounter[1];
            case TYPE_THRALL_PART1:
                return m_auiEncounter[2];
            case TYPE_THRALL_PART2:
                return m_auiEncounter[3];
            case TYPE_THRALL_PART3:
                return m_auiEncounter[4];
            case TYPE_THRALL_PART4:
                return m_auiEncounter[5];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_THRALL:
                return m_uiThrallGUID;
            case DATA_TARETHA:
                return m_uiTarethaGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_old_hillsbrad(Map* pMap)
{
    return new instance_old_hillsbrad(pMap);
}

void AddSC_instance_old_hillsbrad()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_old_hillsbrad";
    newscript->GetInstanceData = &GetInstanceData_instance_old_hillsbrad;
    newscript->RegisterSelf();
}
