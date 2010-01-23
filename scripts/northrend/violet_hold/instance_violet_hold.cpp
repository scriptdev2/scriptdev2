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
SDName: Instance_Violet_Hold
SD%Complete: 25
SDComment: "experimental" use of header/source object
SDCategory: Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

instance_violet_hold::instance_violet_hold(Map* pMap) : ScriptedInstance(pMap),
    m_uiSinclariGUID(0),
    m_uiErekemGUID(0),
    m_uiMoraggGUID(0),
    m_uiIchoronGUID(0),
    m_uiXevozzGUID(0),
    m_uiLavanthorGUID(0),
    m_uiZuramatGUID(0),

    m_uiCellErekemGuard_LGUID(0),
    m_uiCellErekemGuard_RGUID(0),
    m_uiIntroCrystalGUID(0),

    m_uiWorldState(0),
    m_uiWorldStateSealCount(100),
    m_uiWorldStatePortalCount(0)
{
    Initialize();
}

void instance_violet_hold::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_violet_hold::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_SINCLARI: m_uiSinclariGUID = pCreature->GetGUID(); break;
        case NPC_DOOR_SEAL: m_uiDoorSealGUID = pCreature->GetGUID(); break;
        case NPC_EREKEM: m_uiErekemGUID = pCreature->GetGUID(); break;
        case NPC_MORAGG: m_uiMoraggGUID = pCreature->GetGUID(); break;
        case NPC_ICHORON: m_uiIchoronGUID = pCreature->GetGUID(); break;
        case NPC_XEVOZZ: m_uiXevozzGUID = pCreature->GetGUID(); break;
        case NPC_LAVANTHOR: m_uiLavanthorGUID = pCreature->GetGUID(); break;
        case NPC_ZURAMAT: m_uiZuramatGUID = pCreature->GetGUID(); break;
    }
}

void instance_violet_hold::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_CELL_LAVANTHOR:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_LAVANTHOR, pGo->GetGUID()));
            break;
        case GO_CELL_MORAGG:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_MORAGG, pGo->GetGUID()));
            break;
        case GO_CELL_ZURAMAT:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_ZURAMAT, pGo->GetGUID()));
            break;
        case GO_CELL_XEVOZZ:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_XEVOZZ, pGo->GetGUID()));
            break;
        case GO_CELL_ICHORON:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_ICHORON, pGo->GetGUID()));
            break;
        case GO_CELL_EREKEM:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_EREKEM, pGo->GetGUID()));
            break;
        case GO_CELL_EREKEM_GUARD_L:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_EREKEM, pGo->GetGUID()));
            break;
        case GO_CELL_EREKEM_GUARD_R:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_EREKEM, pGo->GetGUID()));
            break;

        case GO_INTRO_CRYSTAL:
            m_uiIntroCrystalGUID = pGo->GetGUID();
            break;
        case GO_PRISON_SEAL_DOOR:
            m_uiDoorSealGUID = pGo->GetGUID();
            break;
    }
}

void instance_violet_hold::SetCellForBoss(uint32 uiBossEntry)
{
    BossToCellMap::const_iterator itrCellLower = m_mBossToCellMap.lower_bound(uiBossEntry);
    BossToCellMap::const_iterator itrCellUpper = m_mBossToCellMap.upper_bound(uiBossEntry);

    if (itrCellLower == itrCellUpper)
        return;

    for(BossToCellMap::const_iterator itr = itrCellLower; itr != itrCellUpper; ++itr)
        DoUseDoorOrButton(itr->second);
}

void instance_violet_hold::SetWorldState(bool bEnable)
{
    if (bEnable)
        m_uiWorldState = 1;
    else
        m_uiWorldState = 0;

    DoUpdateWorldState(WORLD_STATE_ID, m_uiWorldState);
    DoUpdateWorldState(WORLD_STATE_SEAL, m_uiWorldStateSealCount);
    DoUpdateWorldState(WORLD_STATE_PORTALS, m_uiWorldStatePortalCount);
}

void instance_violet_hold::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
       case TYPE_MAIN:
            m_auiEncounter[0] = uiData;
            break;
       case TYPE_SEAL:
            m_auiEncounter[1] = uiData;
            break;
    }
}

InstanceData* GetInstanceData_instance_violet_hold(Map* pMap)
{
    return new instance_violet_hold(pMap);
}

void AddSC_instance_violet_hold()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_violet_hold";
    newscript->GetInstanceData = GetInstanceData_instance_violet_hold;
    newscript->RegisterSelf();
}
