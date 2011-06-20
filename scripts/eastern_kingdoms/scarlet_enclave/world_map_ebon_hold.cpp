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
SDName: world_map_ebon_hold
SD%Complete: 0
SDComment:
SDCategory: Ebon Hold
EndScriptData */

#include "precompiled.h"
#include "world_map_ebon_hold.h"

world_map_ebon_hold::world_map_ebon_hold(Map* pMap) : ScriptedInstance(pMap),
    m_uiBattleEncounter(0)
{
    Initialize();
}

void world_map_ebon_hold::Initialize() {}


InstanceData* GetInstance_world_map_ebon_hold(Map* pMap)
{
    return new world_map_ebon_hold(pMap);
}

void AddSC_world_map_ebon_hold()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "world_map_ebon_hold";
    pNewScript->GetInstanceData = &GetInstance_world_map_ebon_hold;
    pNewScript->RegisterSelf();
}
