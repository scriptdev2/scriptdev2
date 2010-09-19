/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: instance_forge_of_souls
SD%Complete: 90%
SDComment: TODO: Movement of the extro-event is missing, implementation unclear!
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

instance_forge_of_souls::instance_forge_of_souls(Map* pMap) : ScriptedInstance(pMap),
    m_bCriteriaPhantomBlastFailed(false),
    m_uiTeam(0),
    m_uiBronjahmGUID(0),
    m_uiDevourerOrSoulsGUID(0)
{
    Initialize();
}

void instance_forge_of_souls::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_forge_of_souls::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_BRONJAHM:                  m_uiBronjahmGUID = pCreature->GetGUID(); break;
        case NPC_DEVOURER_OF_SOULS:         m_uiDevourerOrSoulsGUID = pCreature->GetGUID(); break;
        case NPC_CORRUPTED_SOUL_FRAGMENT:   m_luiSoulFragmentAliveGUIDs.push_back(pCreature->GetGUID()); break;
    }
}

void instance_forge_of_souls::OnPlayerEnter(Player* pPlayer)
{
    if (!m_uiTeam)                                          // very first player to enter
    {
        m_uiTeam = pPlayer->GetTeam();
        ProcessEventNpcs(pPlayer, false);
    }
}

void instance_forge_of_souls::ProcessEventNpcs(Player* pPlayer, bool bChanged)
{
    if (!pPlayer)
        return;

    if (m_auiEncounter[0] != DONE || m_auiEncounter[1] != DONE)
    {
        // Spawn Begin Mobs
        for (uint8 i = 0; i < sizeof(aEventBeginLocations)/sizeof(sIntoEventNpcSpawnLocations); i++)
        {
            if (Creature* pSummon = pPlayer->SummonCreature(m_uiTeam == HORDE ? aEventBeginLocations[i].uiEntryHorde : aEventBeginLocations[i].uiEntryAlliance,
                                                            aEventBeginLocations[i].fSpawnX, aEventBeginLocations[i].fSpawnY, aEventBeginLocations[i].fSpawnZ, aEventBeginLocations[i].fSpawnO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS))
                m_lEventMobGUIDs.push_back(pSummon->GetGUID());
        }
    }
    else
    {
        // if bChanged, despawn Begin Mobs, spawn End Mobs at Spawn, else spawn EndMobs at End
        if (bChanged)
        {
            for (std::list<uint64>::const_iterator itr = m_lEventMobGUIDs.begin(); itr != m_lEventMobGUIDs.end(); itr++)
            {
                if (Creature* pSummoned = instance->GetCreature(*itr))
                    pSummoned->ForcedDespawn();
            }

            for (uint8 i = 0; i < sizeof(aEventEndLocations)/sizeof(sExtroEventNpcLocations); i++)
            {
                pPlayer->SummonCreature(m_uiTeam == HORDE ? aEventEndLocations[i].uiEntryHorde : aEventEndLocations[i].uiEntryAlliance,
                                        aEventEndLocations[i].fSpawnX, aEventEndLocations[i].fSpawnY, aEventEndLocations[i].fSpawnZ, aEventEndLocations[i].fStartO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS);

                // TODO: Let the NPCs Move along their paths
            }
        }
        else
        {   // Summon at end, without event
            for (uint8 i = 0; i < sizeof(aEventEndLocations)/sizeof(sExtroEventNpcLocations); i++)
            {
                pPlayer->SummonCreature(m_uiTeam == HORDE ? aEventEndLocations[i].uiEntryHorde : aEventEndLocations[i].uiEntryAlliance,
                                        aEventEndLocations[i].fEndX, aEventEndLocations[i].fEndY, aEventEndLocations[i].fEndZ, aEventEndLocations[i].fEndO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS);
            }
        }
    }
}

Player* instance_forge_of_souls::GetPlayer()
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
    return NULL;
}

bool instance_forge_of_souls::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_SOUL_POWER:
            return m_luiSoulFragmentAliveGUIDs.size() >= 4;
        case ACHIEV_CRIT_PHANTOM_BLAST:
            return !m_bCriteriaPhantomBlastFailed;
        default:
            return 0;
    }
}

void instance_forge_of_souls::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_BRONJAHM:
            m_auiEncounter[0] = uiData;

            // Despawn remaining adds and clear list
            for (std::list<uint64>::const_iterator itr = m_luiSoulFragmentAliveGUIDs.begin(); itr != m_luiSoulFragmentAliveGUIDs.end(); itr++)
            {
                if (Creature* pFragment = instance->GetCreature(*itr))
                    pFragment->ForcedDespawn();
            }
            m_luiSoulFragmentAliveGUIDs.clear();
            break;
        case TYPE_DECOURER_OF_SOULS:
            m_auiEncounter[1] = uiData;
            if (uiData == DONE)
                ProcessEventNpcs(GetPlayer(), true);
            break;
        case TYPE_ACHIEV_PHANTOM_BLAST:
            m_bCriteriaPhantomBlastFailed = (uiData == FAIL);
            return;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_forge_of_souls::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_forge_of_souls::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_BRONJAHM:
            return m_auiEncounter[0];
        case TYPE_DECOURER_OF_SOULS:
            return m_auiEncounter[1];
        default:
            return 0;
    }
}

uint64 instance_forge_of_souls::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_BRONJAHM:
            return m_uiBronjahmGUID;
        case NPC_DEVOURER_OF_SOULS:
            return m_uiDevourerOrSoulsGUID;
        default:
            return 0;
    }
}

void instance_forge_of_souls::SetData64(uint32 uiType, uint64 uiData)
{
    if (uiType == DATA_SOULFRAGMENT_REMOVE)
        m_luiSoulFragmentAliveGUIDs.remove(uiData);
}

InstanceData* GetInstanceData_instance_forge_of_souls(Map* pMap)
{
    return new instance_forge_of_souls(pMap);
}

void AddSC_instance_forge_of_souls()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_forge_of_souls";
    pNewScript->GetInstanceData = &GetInstanceData_instance_forge_of_souls;
    pNewScript->RegisterSelf();
}
