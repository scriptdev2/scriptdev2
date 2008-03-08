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
SDName: Instance_arcatraz
SD%Complete: 0
SDComment: VERIFY SCRIPT
EndScriptData */

#include "../../../sc_defines.h"
#include "def_arcatraz.h"
#include "../../../../../../game/GameObject.h"

#define ENCOUNTERS 4

#define CONTAINMENT_CORE_SECURITY_FIELD_ALPHA 184318 //door opened when Wrath-Scryer Soccothrates dies
#define CONTAINMENT_CORE_SECURITY_FIELD_BETA 184319 //door opened when Dalliah the Doomsayer dies

/* Arcatraz encounters:
1 - Zereketh the Unbound event
2 - Dalliah the Doomsayer event
3 - Wrath-Scryer Soccothrates event
4 - Harbinger Skyriss event
*/

struct MANGOS_DLL_DECL instance_arcatraz : public ScriptedInstance
{
    instance_arcatraz(Map *Map) : ScriptedInstance(Map) {Initialize();};

    bool Encounters[ENCOUNTERS];

    GameObject *Containment_Core_Security_Field_Alpha;
    GameObject *Containment_Core_Security_Field_Beta;

    void Initialize()
    {
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
            case CONTAINMENT_CORE_SECURITY_FIELD_ALPHA:
                Containment_Core_Security_Field_Alpha = go;
                break;

            case CONTAINMENT_CORE_SECURITY_FIELD_BETA:
                Containment_Core_Security_Field_Beta = go;
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
            case DATA_ZEREKETHTHEUNBOUNDEVENT:
                Encounters[0] = (data) ? true : false;
                break;

            case DATA_WRATH_SCRYERSOCCOTHRATESEVENT:
                if(data == 2)
                {
                    Encounters[1] = false;
                    if(Containment_Core_Security_Field_Alpha)
                        OpenDoor(Containment_Core_Security_Field_Alpha);
                }
                Encounters[1] = (data) ? true : false;
                break;

            case DATA_DALLIAHTHEDOOMSAYEREVENT:
                if(data == 2)
                {
                    Encounters[2] = false;
                    if(Containment_Core_Security_Field_Beta)
                        OpenDoor(Containment_Core_Security_Field_Beta);
                }
                Encounters[2] = (data) ? true : false;
                break;

            case DATA_HARBINGERSKYRISSEVENT:
                Encounters[3] = (data) ? true : false;
                break;
        }
    }
};

InstanceData* GetInstanceData_instance_arcatraz(Map* map)
{
    return new instance_arcatraz(map);
}

void AddSC_instance_arcatraz()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_arcatraz";
    newscript->GetInstanceData = GetInstanceData_instance_arcatraz;
    m_scripts[nrscripts++] = newscript;
}
