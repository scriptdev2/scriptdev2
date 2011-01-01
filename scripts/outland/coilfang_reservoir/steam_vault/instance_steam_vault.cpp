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
SDName: Instance_Steam_Vault
SD%Complete: 80
SDComment: Instance script and access panel GO
SDCategory: Coilfang Resevoir, The Steamvault
EndScriptData */

#include "precompiled.h"
#include "steam_vault.h"

#define MAX_ENCOUNTER 4

#define MAIN_CHAMBERS_DOOR      183049
#define ACCESS_PANEL_HYDRO      184125
#define ACCESS_PANEL_MEK        184126

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

    if (pGo->GetEntry() == ACCESS_PANEL_HYDRO && pInstance->GetData(TYPE_HYDROMANCER_THESPIA) == DONE)
        pInstance->SetData(TYPE_HYDROMANCER_THESPIA,SPECIAL);

    if (pGo->GetEntry() == ACCESS_PANEL_MEK && pInstance->GetData(TYPE_MEKGINEER_STEAMRIGGER) == DONE)
        pInstance->SetData(TYPE_MEKGINEER_STEAMRIGGER,SPECIAL);

    return true;
}

struct MANGOS_DLL_DECL instance_steam_vault : public ScriptedInstance
{
    instance_steam_vault(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiThespiaGUID;
    uint64 m_uiMekgineerGUID;
    uint64 m_uiKalithreshGUID;

    uint64 m_uiMainChambersDoor;
    uint64 m_uiAccessPanelHydro;
    uint64 m_uiAccessPanelMek;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiThespiaGUID = 0;
        m_uiMekgineerGUID = 0;
        m_uiKalithreshGUID = 0;
        m_uiMainChambersDoor = 0;
        m_uiAccessPanelHydro = 0;
        m_uiAccessPanelMek = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 17797: m_uiThespiaGUID = pCreature->GetGUID(); break;
            case 17796: m_uiMekgineerGUID = pCreature->GetGUID(); break;
            case 17798: m_uiKalithreshGUID = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case MAIN_CHAMBERS_DOOR: m_uiMainChambersDoor = pGo->GetGUID(); break;
            case ACCESS_PANEL_HYDRO: m_uiAccessPanelHydro = pGo->GetGUID(); break;
            case ACCESS_PANEL_MEK:   m_uiAccessPanelMek = pGo->GetGUID(); break;
        }
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case TYPE_HYDROMANCER_THESPIA:
                if (data == SPECIAL)
                {
                    if (GameObject* pGo = instance->GetGameObject(m_uiAccessPanelHydro))
                        pGo->SetGoState(GO_STATE_ACTIVE);

                    if (GetData(TYPE_MEKGINEER_STEAMRIGGER) == SPECIAL)
                    {
                        if (GameObject* pGo = instance->GetGameObject(m_uiMainChambersDoor))
                            pGo->SetGoState(GO_STATE_ACTIVE);
                    }

                    debug_log("SD2: Instance Steamvault: Access panel used.");
                }
                m_auiEncounter[0] = data;
                break;
            case TYPE_MEKGINEER_STEAMRIGGER:
                if (data == SPECIAL)
                {
                    if (GameObject* pGo = instance->GetGameObject(m_uiAccessPanelMek))
                        pGo->SetGoState(GO_STATE_ACTIVE);

                    if (GetData(TYPE_HYDROMANCER_THESPIA) == SPECIAL)
                    {
                        if (GameObject* pGo = instance->GetGameObject(m_uiMainChambersDoor))
                            pGo->SetGoState(GO_STATE_ACTIVE);
                    }

                    debug_log("SD2: Instance Steamvault: Access panel used.");
                }
                m_auiEncounter[1] = data;
                break;
            case TYPE_WARLORD_KALITHRESH:
                m_auiEncounter[2] = data;
                break;
            case TYPE_DISTILLER:
                m_auiEncounter[3] = data;
                break;
        }
    }

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case TYPE_HYDROMANCER_THESPIA:
                return m_auiEncounter[0];
            case TYPE_MEKGINEER_STEAMRIGGER:
                return m_auiEncounter[1];
            case TYPE_WARLORD_KALITHRESH:
                return m_auiEncounter[2];
            case TYPE_DISTILLER:
                return m_auiEncounter[3];
        }
        return 0;
    }

    uint64 GetData64(uint32 data)
    {
        switch(data)
        {
            case DATA_THESPIA:
                return m_uiThespiaGUID;
            case DATA_MEKGINEERSTEAMRIGGER:
                return m_uiMekgineerGUID;
            case DATA_KALITRESH:
                return m_uiKalithreshGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_steam_vault(Map* pMap)
{
    return new instance_steam_vault(pMap);
}

void AddSC_instance_steam_vault()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_main_chambers_access_panel";
    newscript->pGOUse = &GOUse_go_main_chambers_access_panel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "instance_steam_vault";
    newscript->GetInstanceData = &GetInstanceData_instance_steam_vault;
    newscript->RegisterSelf();
}
