/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "../../../sc_defines.h"
#include "def_steam_vault.h"
#include "../../../../../../game/GameObject.h"

#define ENCOUNTERS 3

#define MAIN_CHAMBERS_DOOR       183049 //door opened when hydromancer and mekgineer are died

/* Steam Vaults encounters:
1 - Hydromancer Thespia Event
2 - Mekgineer Steamrigger Event
3 - Warlord Kalithresh Event
*/

struct MANGOS_DLL_DECL instance_steam_vault : public ScriptedInstance
{
    instance_steam_vault(Map *Map) : ScriptedInstance(Map) {Initialize();};

    bool Encounters[ENCOUNTERS];

    bool IsHydromancerDied, IsMekgineerDied;
    GameObject *MainChambersDoor;

    void Initialize()
    {
        IsHydromancerDied = false;
        IsMekgineerDied = false;
        MainChambersDoor = NULL;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = false;
    }

    bool IsEncounterInProgress() const 
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i]) return true;

        return false;
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry())
        {
            case MAIN_CHAMBERS_DOOR:
            MainChambersDoor = go;
            break;
        }
    }

    void OpenDoor(GameObject *go)
    {
        //open the door
        go->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
        go->SetUInt32Value(GAMEOBJECT_STATE, 0);
    }

    void CheckInstanceStatus()
    {
        if(IsHydromancerDied) //ToDo: add IsMekgineerDied check when mekginner script is implemented
        {
            if(MainChambersDoor)
                OpenDoor(MainChambersDoor);
        }
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case DATA_HYDROMANCERTHESPIAEVENT:
                if(data == 2)
                {
                    Encounters[0] = false;
                    IsHydromancerDied = true;
                    CheckInstanceStatus();
                }
                else
                    Encounters[0] = (data) ? true : false;
                break;

            case DATA_MEKGINEERSTEAMRIGGEREVENT:
                if(data == 2)
                {
                    Encounters[1] = false;
                    IsMekgineerDied = true;
                    CheckInstanceStatus();
                }
                else
                    Encounters[1] = (data) ? true : false;
                break;

            case DATA_WARLORDKALITHRESHEVENT:
                Encounters[2] = (data) ? true : false;
                break;
        }
    }
};

InstanceData* GetInstanceData_instance_steam_vault(Map* map)
{
    return new instance_steam_vault(map);
}

void AddSC_instance_steam_vault()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_steam_vault";
    newscript->GetInstanceData = GetInstanceData_instance_steam_vault;
    m_scripts[nrscripts++] = newscript;
}
