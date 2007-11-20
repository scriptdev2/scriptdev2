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

/* ScriptData
SDName: Instance_Magtheridons_Lair
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../../sc_defines.h"

struct MANGOS_DLL_DECL instance_magtheridons_lair : public ScriptedInstance
{
    instance_magtheridons_lair(Map *Map) : ScriptedInstance(Map) {Initialize();};

    bool EncounterInProgress;
    uint64 Magtheridon;
    uint64 EventStarter;

    void Initialize()
    {
        Magtheridon = 0;
        EncounterInProgress = false;
    }

    bool IsEncounterInProgress() const 
    {
        return EncounterInProgress;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry) 
    {
        if (creature_entry == 17257)
            Magtheridon = creature->GetGUID();
    }

    uint64 GetData64(char *identifier)
    {
        if(identifier == "Magtheridon")
            return Magtheridon;

        if(identifier == "Event_Starter")
            return EventStarter;

        return 0;
    }

    void SetData64(char *identifier, uint64 guid)
    {
        if(identifier == "Magtheridon")
            Magtheridon = guid;

        if(identifier == "Event_Starter")
            EventStarter = guid;
    }

    void SetData(char *type, uint32 data)
    {
        if(type == "Magtheridon_Event_Started")
            EncounterInProgress = true;

        if(type == "Magtheridon_Event_Ended")
            EncounterInProgress = false;
    }

    uint32 GetData(char *type) 
    { 
        if(type == "Magtheridon_Event_Status")
            return EncounterInProgress;

        return 0;
    }
};

InstanceData* GetInstanceData_instance_magtheridons_lair(Map* map)
{
    return new instance_magtheridons_lair(map);
}

void AddSC_instance_magtheridons_lair()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_magtheridons_lair";
    newscript->GetInstanceData = GetInstanceData_instance_magtheridons_lair;
    m_scripts[nrscripts++] = newscript;
}
