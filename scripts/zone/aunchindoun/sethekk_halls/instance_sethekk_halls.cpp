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

/* ScriptData
SDName: Instance - Sethekk Halls
SD%Complete: 50
SDComment: Instance Data for Sethekk Halls instance
SDCategory: Auchindoun, Sethekk Halls
EndScriptData */

#include "def_sethekk_halls.h"
#include "GameObject.h"

#define IKISS_DOOR          177203 //udb spawn

struct MANGOS_DLL_DECL instance_sethekk_halls : public ScriptedInstance
{
    instance_sethekk_halls(Map *Map) : ScriptedInstance(Map) {Initialize();};

    GameObject *IkissDoor;

    void Initialize()
    {
        IkissDoor = NULL;
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry())
        {
            case IKISS_DOOR:
                IkissDoor = go;
                break;
        }
    }

    void OpenDoor(GameObject *go)
    {
        //open the door
        go->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
        go->SetUInt32Value(GAMEOBJECT_STATE, 0);
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case DATA_IKISSDOOREVENT:
                    if(IkissDoor)
                        OpenDoor(IkissDoor);
                    break;
        }
    }
};

InstanceData* GetInstanceData_instance_sethekk_halls(Map* map)
{
    return new instance_sethekk_halls(map);
}

void AddSC_instance_sethekk_halls()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_sethekk_halls";
    newscript->GetInstanceData = GetInstanceData_instance_sethekk_halls;
    m_scripts[nrscripts++] = newscript;
}
