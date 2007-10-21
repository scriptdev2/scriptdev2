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

#define ENCOUNTERS     5

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAILED        = 2,
    DONE          = 3
};

/* The Eye encounters:
0 - Kael'thas event
1 - Al' ar event
2 - Solarian Event
3 - Void Reaver event
*/
 
struct MANGOS_DLL_DECL instance_the_eye : public ScriptedInstance
{
    instance_the_eye(Map *Map) : ScriptedInstance(Map) {Initialize();};

    uint64 ThaladredTheDarkener;
    uint64 LordSanguinar;
    uint64 GrandAstromancerCapernian;
    uint64 MasterEngineerTelonicus;

    uint32 Encounters[ENCOUNTERS];

    void Initialize()
    {
        ThaladredTheDarkener = 0;
        LordSanguinar = 0;
        GrandAstromancerCapernian = 0;
        MasterEngineerTelonicus = 0;

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
            case 20064:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            creature->setFaction(35);
            ThaladredTheDarkener = creature->GetGUID();
            break;

            case 20063:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            creature->setFaction(35);
            MasterEngineerTelonicus = creature->GetGUID();
            break;

            case 20062:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            creature->setFaction(35);
            GrandAstromancerCapernian = creature->GetGUID();
            break;

            case 20060:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            creature->setFaction(35);
            LordSanguinar = creature->GetGUID();
            break;
        }
    }

    uint64 GetUnitGUID(char *identifier)
    {
        if(identifier == "ThaladredTheDarkener" && ThaladredTheDarkener)
            return ThaladredTheDarkener;
        else if(identifier == "LordSanguinar" && LordSanguinar)
            return LordSanguinar;
        else if(identifier == "GrandAstromancerCapernian" && GrandAstromancerCapernian)
            return GrandAstromancerCapernian;
        else if(identifier == "MasterEngineerTelonicus" && MasterEngineerTelonicus)
            return MasterEngineerTelonicus;

        return NULL;
    }

    void SetData(char *type, uint32 data)
    {
        if(type == "KaelThasEvent")
            Encounters[0] = data;
        else if(type == "AlArEvent")
            Encounters[1] = data;
        else if(type == "SolarianEvent")
            Encounters[2] = data;
        else if(type == "VoidReaverEvent")
            Encounters[3] = data;
    }

    uint32 GetData(char *type)
    {
        if(type == "KaelThasEvent")
            return Encounters[0];
        else if(type == "AlArEvent")
            return Encounters[1];
        else if(type == "SolarianEvent")
            return Encounters[2];
        else if(type == "VoidReaverEvent")
            return Encounters[3];

        return 0;
    }
};

InstanceData* GetInstanceData_instance_the_eye(Map* map)
{
    return new instance_the_eye(map);
}

void AddSC_instance_the_eye()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "raid_the_eye";
    newscript->GetInstanceData = GetInstanceData_instance_the_eye;
    m_scripts[nrscripts++] = newscript;
}
