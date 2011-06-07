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
SDName: Instance_Steam_Vault
SD%Complete: 80
SDComment: Instance script and access panel GO
SDCategory: Coilfang Resevoir, The Steamvault
EndScriptData */

#include "precompiled.h"
#include "steam_vault.h"

/* Steam Vaults encounters:
1 - Hydromancer Thespia Event
2 - Mekgineer Steamrigger Event
3 - Warlord Kalithresh Event
*/

bool GOUse_go_main_chambers_access_panel(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    if (pGo->GetEntry() == GO_ACCESS_PANEL_HYDRO && pInstance->GetData(TYPE_HYDROMANCER_THESPIA) == DONE)
        pInstance->SetData(TYPE_HYDROMANCER_THESPIA, SPECIAL);

    if (pGo->GetEntry() == GO_ACCESS_PANEL_MEK && pInstance->GetData(TYPE_MEKGINEER_STEAMRIGGER) == DONE)
        pInstance->SetData(TYPE_MEKGINEER_STEAMRIGGER, SPECIAL);

    return true;
}

instance_steam_vault::instance_steam_vault(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_steam_vault::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_steam_vault::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_STEAMRIGGER:
            m_mNpcEntryGuidStore[NPC_STEAMRIGGER] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_steam_vault::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_MAIN_CHAMBERS_DOOR:
        case GO_ACCESS_PANEL_HYDRO:
        case GO_ACCESS_PANEL_MEK:
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
    }
}

void instance_steam_vault::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_HYDROMANCER_THESPIA:
            if (uiData == SPECIAL)
            {
                DoUseDoorOrButton(GO_ACCESS_PANEL_HYDRO);

                if (GetData(TYPE_MEKGINEER_STEAMRIGGER) == SPECIAL)
                    DoUseDoorOrButton(GO_MAIN_CHAMBERS_DOOR);

                debug_log("SD2: Instance Steamvault: Access panel used.");
            }
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_MEKGINEER_STEAMRIGGER:
            if (uiData == SPECIAL)
            {
                DoUseDoorOrButton(GO_ACCESS_PANEL_MEK);

                if (GetData(TYPE_HYDROMANCER_THESPIA) == SPECIAL)
                    DoUseDoorOrButton(GO_MAIN_CHAMBERS_DOOR);

                    debug_log("SD2: Instance Steamvault: Access panel used.");
            }
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_WARLORD_KALITHRESH:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_DISTILLER:
            m_auiEncounter[3] = uiData;
            break;
    }
}

uint32 instance_steam_vault::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_HYDROMANCER_THESPIA:   return m_auiEncounter[0];
        case TYPE_MEKGINEER_STEAMRIGGER: return m_auiEncounter[1];
        case TYPE_WARLORD_KALITHRESH:    return m_auiEncounter[2];
        case TYPE_DISTILLER:             return m_auiEncounter[3];

        default:
            return 0;
    }
}

InstanceData* GetInstanceData_instance_steam_vault(Map* pMap)
{
    return new instance_steam_vault(pMap);
}

void AddSC_instance_steam_vault()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "go_main_chambers_access_panel";
    pNewScript->pGOUse = &GOUse_go_main_chambers_access_panel;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "instance_steam_vault";
    pNewScript->GetInstanceData = &GetInstanceData_instance_steam_vault;
    pNewScript->RegisterSelf();
}
