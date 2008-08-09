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
SDName: Instance_Shadow_Labyrinth
SD%Complete: 99
SDComment: VERIFY SCRIPT
SDCategory: Auchindoun, Shadow Labyrinth
EndScriptData */

#include "precompiled.h"
#include "def_shadow_labyrinth.h"
#include "GameObject.h"

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
    uint64 GrandmasterVorpil;

    void Initialize()
    {
        RefectoryDoor = NULL;
        ScreamingHallDoor = NULL;
        GrandmasterVorpil = 0;

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

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature_entry)
        {
            case 18732:
            GrandmasterVorpil = creature->GetGUID();
            break;
        }
    }

    uint64 GetUnitGUID(uint32 identifier)
    {
        if(identifier  == DATA_GRANDMASTERVORPIL)
            return GrandmasterVorpil;

        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case DATA_AMBASSADORHELLMAWEVENT:
                Encounters[0] = (data) ? true : false;
                break;

            case DATA_BLACKHEARTTHEINCITEREVENT:
                if(data == 2)
                {
                    Encounters[1] = false;
                    if(RefectoryDoor)
                        RefectoryDoor->UseDoorOrButton();
                }
                else
                    Encounters[1] = (data) ? true : false;
                break;
            case DATA_GRANDMASTERVORPILEVENT:
                if(data == 2)
                {
                    Encounters[2] = false;
                    if(ScreamingHallDoor)
                        ScreamingHallDoor->UseDoorOrButton();
                }
                else
                    Encounters[2] = (data) ? true : false;
                break;
            case DATA_MURMUREVENT:
                Encounters[3] = (data) ? true : false;
                break;
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
