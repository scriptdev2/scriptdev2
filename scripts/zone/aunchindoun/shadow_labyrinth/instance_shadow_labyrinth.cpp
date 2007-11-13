/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
#include "../../../../../../game/GameObject.h"

#define ENCOUNTERS 4

#define REFECTORY_DOOR          183296 //door opened when blackheart the inciter dies
#define SCREAMING_HALL_DOOR     183295 //door opened when grandmaster vorpil dies

/* Shadow Labyrinth encounters:
1 - Ambassador Hellmaw event
2 - Blackheart the Inciter event
3 - Grandmaster Vorpil event
4 - Murmur event
*/

struct MANGOS_DLL_DECL instance_shadow_labyrinth : public ScriptedInstance
{
    instance_shadow_labyrinth(Map *Map) : ScriptedInstance(Map) {Initialize();};

    bool Encounters[ENCOUNTERS];

    GameObject *RefectoryDoor;
    GameObject *ScreamingHallDoor;

    void Initialize()
    {
        RefectoryDoor = NULL;
        ScreamingHallDoor = NULL;

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
            case REFECTORY_DOOR:
            RefectoryDoor = go;
            break;

            case SCREAMING_HALL_DOOR:
            ScreamingHallDoor = go;
            break;
        }
    }

    void OpenDoor(GameObject *go)
    {
        //open the door
        go->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
        go->SetUInt32Value(GAMEOBJECT_STATE, 0);
    }

    void SetData(char *type, uint32 data)
    {
        if(type == "AmbassadorHellmawEvent")
            Encounters[0] = (data) ? true : false;
        else if(type == "BlackheartTheInciterEvent")
            Encounters[1] = (data) ? true : false;
        else if(type == "GrandmasterVorpilEvent")
            Encounters[2] = (data) ? true : false;
        else if(type == "MurmurEvent")
            Encounters[3] = (data) ? true : false;
        else if(type == "BlackheartTheInciter_Death")
        {
            if(RefectoryDoor)
            OpenDoor(RefectoryDoor);
        }
        else if(type == "GrandmasterVorpil_Death")
        {
            if(ScreamingHallDoor)
                OpenDoor(ScreamingHallDoor);
        }
    }
};

InstanceData* GetInstanceData_instance_shadow_labyrinth(Map* map)
{
    return new instance_shadow_labyrinth(map);
}

void AddSC_instance_shadow_labyrinth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_shadow_labyrinth";
    newscript->GetInstanceData = GetInstanceData_instance_shadow_labyrinth;
    m_scripts[nrscripts++] = newscript;
}
