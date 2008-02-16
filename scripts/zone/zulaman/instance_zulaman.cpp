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

#include "../../sc_defines.h"

#define ENCOUNTERS     1

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAILED        = 2,
    DONE          = 3
};



struct MANGOS_DLL_DECL instance_zulaman : public ScriptedInstance
{
    instance_zulaman(Map *Map) : ScriptedInstance(Map) {Initialize();};

    uint64 janalai;
    uint32 janalai_eggs_l;
    uint32 janalai_eggs_r;

    uint32 Encounters[ENCOUNTERS];

    void Initialize()
    {
        janalai = 0;
        janalai_eggs_l = 20;
        janalai_eggs_r = 20;


        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = NOT_STARTED;
    }

    bool IsEncounterInProgress() const 
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature_entry)
        {
        case 23578:
            janalai = creature->GetGUID();
            break;


        }
    }

    uint64 GetData64(char *identifier)
    {
        if(identifier == "Janalai" && janalai)
            return janalai;


        return NULL;
    }

    void SetData(char *type, uint32 data)
    {
        if(type == "JanalaiEvent")
        {
            if(data == 0)
            {
                janalai_eggs_l = 20;
                janalai_eggs_r = 20;
            };
            Encounters[0] = data;
        }
        else if(type == "J_Hatchleft")
        {
            janalai_eggs_l -= data;
        }else if(type == "J_Hatchright")
        {
            janalai_eggs_r -= data;
        };

    }

    uint32 GetData(char *type)
    {
        if(type == "JanalaiEvent")
            return Encounters[0];
        else if(type == "J_Eggsleft")
            return janalai_eggs_l;
        else if(type == "J_Eggsright")
            return janalai_eggs_r;

        return 0;
    }
};

InstanceData* GetInstanceData_instance_zulaman(Map* map)
{
    return new instance_zulaman(map);
}

void AddSC_instance_zulaman()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_zulaman";
    newscript->GetInstanceData = GetInstanceData_instance_zulaman;
    m_scripts[nrscripts++] = newscript;
}
