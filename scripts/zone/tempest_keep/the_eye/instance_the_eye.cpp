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
SDName: Instance_The_Eye
SD%Complete: 100
SDComment: 
EndScriptData */
 
#include "../../../sc_defines.h"

#define ENCOUNTERS 5

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
    uint64 Kaelthas;

    uint8 KaelthasEventPhase;

    bool Encounters[ENCOUNTERS];

    void Initialize()
    {
        ThaladredTheDarkener = 0;
        LordSanguinar = 0;
        GrandAstromancerCapernian = 0;
        MasterEngineerTelonicus = 0;
        Kaelthas = 0;

        KaelthasEventPhase = 0;

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

            case 19622:
            Kaelthas = creature->GetGUID();
            break;
        }
    }

    uint64 GetData64(char *identifier)
    {
        if(identifier == "ThaladredTheDarkener")
            return ThaladredTheDarkener;
        else if(identifier == "LordSanguinar")
            return LordSanguinar;
        else if(identifier == "GrandAstromancerCapernian")
            return GrandAstromancerCapernian;
        else if(identifier == "MasterEngineerTelonicus")
            return MasterEngineerTelonicus;
        else if(identifier == "Kaelthas")
            return Kaelthas;

        return NULL;
    }

    void SetData(char *type, uint32 data)
    {
        if(type == "AlArEvent")
            Encounters[0] = (data) ? true : false;
        else if(type == "SolarianEvent")
            Encounters[1] = (data) ? true : false;
        else if(type == "VoidReaverEvent")
            Encounters[2] = (data) ? true : false;

        //Kael'thas
        else if(type == "KaelThasEvent")
        {
            KaelthasEventPhase = data;
            Encounters[3] = (data) ? true : false;
        }
    }

    uint32 GetData(char *type)
    {
        if(type == "AlArEvent")
            return Encounters[0];
        else if(type == "SolarianEvent")
            return Encounters[1];
        else if(type == "VoidReaverEvent")
            return Encounters[2];

        //Kael'thas
        else if(type == "KaelThasEvent")
            return KaelthasEventPhase;

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
    newscript->Name = "instance_the_eye";
    newscript->GetInstanceData = GetInstanceData_instance_the_eye;
    m_scripts[nrscripts++] = newscript;
}
