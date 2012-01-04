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
SDName: world_map_scripts
SD%Complete: 0
SDComment: Quest support: 11538
SDCategory: World Map Scripts
EndScriptData */

#include "precompiled.h"

/* *********************************************************
 *                  EASTERN KINGDOMS
 */
struct MANGOS_DLL_DECL world_map_eastern_kingdoms : public ScriptedMap
{
    world_map_eastern_kingdoms(Map* pMap) : ScriptedMap(pMap) {}

    void SetData(uint32 uiType, uint32 uiData) {}
};

InstanceData* GetInstanceData_world_map_eastern_kingdoms(Map* pMap)
{
    return new world_map_eastern_kingdoms(pMap);
}

/* *********************************************************
 *                     KALIMDOR
 */
struct MANGOS_DLL_DECL world_map_kalimdor : public ScriptedMap
{
    world_map_kalimdor(Map* pMap) : ScriptedMap(pMap) {}

    void SetData(uint32 uiType, uint32 uiData) {}
};

InstanceData* GetInstanceData_world_map_kalimdor(Map* pMap)
{
    return new world_map_kalimdor(pMap);
}

/* *********************************************************
 *                     OUTLAND
 */

enum
{
    NPC_EMISSARY_OF_HATE        = 25003,
    NPC_IRESPEAKER              = 24999,
    NPC_UNLEASHED_HELLION       = 25002,
};

static const float aSpawnLocations[1][4] =
{
    {12583.019531f, -6916.194336f, 4.601806f, 6.182446f}    // Emissary of Hate, guesswork
};

struct MANGOS_DLL_DECL world_map_outland : public ScriptedMap
{
    world_map_outland(Map* pMap) : ScriptedMap(pMap) { Initialize(); }

    uint8 m_uiEmissaryOfHate_KilledAddCount;

    void Initialize()
    {
        m_uiEmissaryOfHate_KilledAddCount = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        if (pCreature->GetEntry() == NPC_EMISSARY_OF_HATE)
            m_mNpcEntryGuidStore[NPC_EMISSARY_OF_HATE] = pCreature->GetObjectGuid();
    }

    void OnCreatureDeath(Creature* pCreature)
    {
        switch (pCreature->GetEntry())
        {
            case NPC_IRESPEAKER:
            case NPC_UNLEASHED_HELLION:
                if (!GetSingleCreatureFromStorage(NPC_EMISSARY_OF_HATE, true))
                {
                    ++m_uiEmissaryOfHate_KilledAddCount;
                    if (m_uiEmissaryOfHate_KilledAddCount == 6)
                    {
                        pCreature->SummonCreature(NPC_EMISSARY_OF_HATE, aSpawnLocations[0][0], aSpawnLocations[0][1], aSpawnLocations[0][2], aSpawnLocations[0][3], TEMPSUMMON_DEAD_DESPAWN, 0);
                        m_uiEmissaryOfHate_KilledAddCount = 0;
                    }
                }
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData) {}
};

InstanceData* GetInstanceData_world_map_outland(Map* pMap)
{
    return new world_map_outland(pMap);
}

/* *********************************************************
 *                     NORTHREND
 */
struct MANGOS_DLL_DECL world_map_northrend : public ScriptedMap
{
    world_map_northrend(Map* pMap) : ScriptedMap(pMap) {}

    void SetData(uint32 uiType, uint32 uiData) {}
};

InstanceData* GetInstanceData_world_map_northrend(Map* pMap)
{
    return new world_map_northrend(pMap);
}

void AddSC_world_map_scripts()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "world_map_eastern_kingdoms";
    pNewScript->GetInstanceData = &GetInstanceData_world_map_eastern_kingdoms;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "world_map_kalimdor";
    pNewScript->GetInstanceData = &GetInstanceData_world_map_kalimdor;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "world_map_outland";
    pNewScript->GetInstanceData = &GetInstanceData_world_map_outland;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "world_map_northrend";
    pNewScript->GetInstanceData = &GetInstanceData_world_map_northrend;
    pNewScript->RegisterSelf();
}
