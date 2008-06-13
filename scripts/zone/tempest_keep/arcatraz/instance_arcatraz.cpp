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
SD%Complete: 60
SDComment: Harbringer Skyriss not implemented
SDCategory: Tempest Keep, Arcatraz
EndScriptData */

#include "def_arcatraz.h"
#include "GameObject.h"

#define ENCOUNTERS 4

#define CONTAINMENT_CORE_SECURITY_FIELD_ALPHA 184318 //door opened when Wrath-Scryer Soccothrates dies
#define CONTAINMENT_CORE_SECURITY_FIELD_BETA  184319 //door opened when Dalliah the Doomsayer dies

/* Arcatraz encounters:
1 - Zereketh the Unbound event
2 - Dalliah the Doomsayer event
3 - Wrath-Scryer Soccothrates event
4 - Harbinger Skyriss event
*/

struct MANGOS_DLL_DECL instance_arcatraz : public ScriptedInstance
{
    instance_arcatraz(Map *Map) : ScriptedInstance(Map) {Initialize();};

    bool Encounter[ENCOUNTERS];

    GameObject *Containment_Core_Security_Field_Alpha;
    GameObject *Containment_Core_Security_Field_Beta;

    void Initialize()
    {
        Containment_Core_Security_Field_Alpha = NULL;
        Containment_Core_Security_Field_Beta  = NULL;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounter[i] = false;
    }

    bool IsEncounterInProgress() const 
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounter[i]) return true;

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

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case TYPE_ZEREKETH:
                Encounter[0] = (data) ? true : false;
                break;

            case TYPE_DALLIAH:
            {
                if(data == DONE)
                {
                    Encounter[1] = DONE;
                    if(Containment_Core_Security_Field_Beta)
                        Containment_Core_Security_Field_Beta->UseDoorOrButton();
                }
                else
                    Encounter[1] = NOT_STARTED;
                break;
            }

            case TYPE_SOCCOTHRATES:
            {
                if(data == DONE)
                {
                    Encounter[2] = DONE;
                    if(Containment_Core_Security_Field_Alpha)
                        Containment_Core_Security_Field_Alpha->UseDoorOrButton();
                }
                else
                    Encounter[2] = NOT_STARTED;
                break;
            }

            case TYPE_HARBINGERSKYRISS:
                Encounter[3] = (data) ? true : false;
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
