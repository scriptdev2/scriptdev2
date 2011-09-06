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
SDName: world_map_scripts
SD%Complete: 0
SDComment: Placeholder, Scripts for continents
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

struct MANGOS_DLL_DECL world_map_outland : public ScriptedMap
{
    world_map_outland(Map* pMap) : ScriptedMap(pMap) {}

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
