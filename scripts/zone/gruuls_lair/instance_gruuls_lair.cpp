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

struct MANGOS_DLL_DECL instance_gruuls_lair : public ScriptedInstance
{
    instance_gruuls_lair(Map *Map) : ScriptedInstance(Map) {Initialize();};

    bool MaulgarInProgress;
    bool GruulInProgress;

    uint64 Kiggler;
    uint64 Blindeye;
    uint64 Olm;
    uint64 Krosh;
    uint64 Maulgar;
    uint64 Gruul;

    uint64 Maulgar_Event_Starter;

    void Initialize()
    {
        MaulgarInProgress = false;
        GruulInProgress = false;

        Kiggler = 0;
        Blindeye = 0;
        Olm = 0;
        Krosh = 0;
        Maulgar = 0;
        Gruul = 0;

        Maulgar_Event_Starter = 0;
    }

    bool IsEncounterInProgress() const 
    {
        if (MaulgarInProgress || GruulInProgress)
            return true;

        return false;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature_entry)
        {
            case 18835:
            Kiggler = creature->GetGUID();
            break;

            case 18836:
            Blindeye = creature->GetGUID();
            break;

            case 18834:
            Olm = creature->GetGUID();
            break;

            case 18832:
            Krosh = creature->GetGUID();
            break;

            case 18831:
            Maulgar = creature->GetGUID();
            break;

            case 19044:
            Gruul = creature->GetGUID();
            break;
        }
    }
    uint64 GetUnitGUID(char *identifier)
    {
        if (identifier == "Kiggler")
            return Kiggler;
        else if (identifier == "Blindeye")
            return Blindeye;
        else if (identifier == "Olm")
            return Olm;
        else if (identifier == "Krosh")
            return Krosh;
        else if (identifier == "High")
            return Maulgar;
        else if (identifier == "Maulgar_Event_Starter")
            return Maulgar_Event_Starter;
        else if (identifier == "Gruul")
            return Gruul;

        return 0;
    }

    void SetData(char *type, uint32 data)
    {
        if(type == "Event_Maulgar_Started")
        {
            MaulgarInProgress = true;
            Maulgar_Event_Starter = data;
        }
        else if(type == "Event_Maulgar_Started_data2")
        {
            Maulgar_Event_Starter += (uint64(data) << 32);

        }else if(type == "Event_Gruul_Started")
            GruulInProgress = true;
        else if(type == "Event_Maulgar_Ended")
            MaulgarInProgress = false;
        else if(type == "Event_Gruul_Ended")
            GruulInProgress = false;
    }

    virtual uint32 GetData(char *type) 
    { 
        if(type == "Event_Maulgar_Status")
            return MaulgarInProgress;
        else if(type == "Event_Gruul_Status")
            return GruulInProgress;

        return 0;
    }
};

InstanceData* GetInstanceData_instance_gruuls_lair(Map* map)
{
    return new instance_gruuls_lair(map);
}

void AddSC_instance_gruuls_lair()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "raid_gruuls_lair";
    newscript->GetInstanceData = GetInstanceData_instance_gruuls_lair;
    m_scripts[nrscripts++] = newscript;
}
