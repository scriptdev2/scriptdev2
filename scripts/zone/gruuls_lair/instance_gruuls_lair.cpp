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
SDName: Instance_Gruuls_Lair
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"
#include "def_gruuls_lair.h"

#define ENCOUNTERS 2

/* Gruuls Lair encounters:
1 - High King Maulgar event
2 - Gruul event
*/

struct MANGOS_DLL_DECL instance_gruuls_lair : public ScriptedInstance
{
    instance_gruuls_lair(Map *Map) : ScriptedInstance(Map) {Initialize();};

    bool Encounters[ENCOUNTERS];

    uint64 MaulgarEvent_Tank;
    uint64 KigglerTheCrazed;
    uint64 BlindeyeTheSeer;
    uint64 OlmTheSummoner;
    uint64 KroshFirehand;

    void Initialize()
    {
        MaulgarEvent_Tank = 0;
        KigglerTheCrazed = 0;
        BlindeyeTheSeer = 0;
        OlmTheSummoner = 0;
        KroshFirehand = 0;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = false;
    }

    bool IsEncounterInProgress() const 
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i]) return true;

        return false;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature_entry)
        {
            case 18835:
            KigglerTheCrazed = creature->GetGUID();
            break;

            case 18836:
            BlindeyeTheSeer = creature->GetGUID();
            break;

            case 18834:
            OlmTheSummoner = creature->GetGUID();
            break;

            case 18832:
            KroshFirehand = creature->GetGUID();
            break;
        }
    }

    void SetData64(uint32 type, uint64 data)
    {
        if(type == DATA_MAULGAREVENT_TANK)
            MaulgarEvent_Tank = data;
    }

    uint64 GetData64(uint32 identifier)
    {
        if(identifier  == DATA_MAULGAREVENT_TANK)
            return MaulgarEvent_Tank;
        else if(identifier  == DATA_KIGGLERTHECRAZED)
            return KigglerTheCrazed;
        else if(identifier  == DATA_BLINDEYETHESEER)
            return BlindeyeTheSeer;
        else if(identifier  == DATA_OLMTHESUMMONER)
            return OlmTheSummoner;
        else if(identifier  == DATA_KROSHFIREHAND)
            return KroshFirehand;

        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        if(type == DATA_MAULGAREVENT)
            Encounters[0] = (data) ? true : false;
        else if(type == DATA_GRUULEVENT)
            Encounters[1] = (data) ? true : false;
    }

    uint32 GetData(uint32 type) 
    { 
        if(type == DATA_MAULGAREVENT)
            return Encounters[0];
        else if(type == DATA_GRUULEVENT)
            return Encounters[1];

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
    newscript->Name = "instance_gruuls_lair";
    newscript->GetInstanceData = GetInstanceData_instance_gruuls_lair;
    m_scripts[nrscripts++] = newscript;
}
