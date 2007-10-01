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

#define ENCOUNTERS     2

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAILED        = 2,
    DONE          = 3
};

struct MANGOS_DLL_DECL instance_gruuls_lair : public InstanceData
{
    instance_gruuls_lair(Map *Map) : InstanceData(Map) {Initialize();};

    uint32 Encounters[ENCOUNTERS];

    Creature *KigglerTheCrazed;
    Creature *BlindeyeTheSeer;
    Creature *OlmTheSummoner;
    Creature *KroshFirehand;

    virtual void Initialize()
    {
        KigglerTheCrazed = NULL;
        BlindeyeTheSeer = NULL;
        OlmTheSummoner = NULL;
        KroshFirehand = NULL;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = NOT_STARTED;
    }

    virtual bool IsEncounterInProgress() const 
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i] == IN_PROGRESS) return true; 

        return false;
    }

    virtual void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature_entry)
        {
            case 18835:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            creature->setFaction(35);
            KigglerTheCrazed = creature;
            break;

            case 18836:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            creature->setFaction(35);
            BlindeyeTheSeer = creature;
            break;

            case 18834:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            creature->setFaction(35);
            OlmTheSummoner = creature;
            break;

            case 18832:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            creature->setFaction(35);
            KroshFirehand = creature;
            break;
        }
    }

    virtual Creature* GetUnit(char *identifier)
    {
        if(identifier == "KigglerTheCrazed")
            return KigglerTheCrazed;
        else if(identifier == "BlindeyeTheSeer")
            return BlindeyeTheSeer;
        else if(identifier == "OlmTheSummoner")
            return OlmTheSummoner;
        else if(identifier == "KroshFirehand")
            return KroshFirehand;

        return NULL; 
    }

    virtual void SetData(char *type, uint32 data)
    {
        if(type == "HighKingMaulgarEvent")
            Encounters[0] = data;
        else if(type == "GruulEvent")
            Encounters[1] = data;
    }

    virtual uint32 GetData(char *type)
    {
        if(type == "HighKingMaulgarEvent")
            return Encounters[0];
        else if(type == "GruulEvent")
            return Encounters[1];

        return 0;
    }

    virtual GameObject* GetGO(char *identifier) { return NULL; }
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
