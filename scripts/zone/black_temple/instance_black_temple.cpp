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
SDName: Instance_Black_Temple
SD%Complete: 100
SDComment: Instance Data Scripts and functions to acquire mobs and set encounter status for use in various Black Temple Scripts
SDCategory: Black Temple
EndScriptData */

#include "../../sc_defines.h"
#include "def_black_temple.h"

#define ENCOUNTERS     8

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAILED        = 2,
    DONE          = 3
};

/* Black Temple encounters:
0 - High Warlord Naj'entus event
1 - Supremus Event
2 - Shade of Akama Event
3 - Teron Gorefiend Event
4 - Gurtogg Bloodboil Event
5 - Reliquary Of Souls Event
6 - Mother Shahraz Event
7 - Illidari Council Event
8 - Illidan Stormrage Event
*/
 
struct MANGOS_DLL_DECL instance_black_temple : public ScriptedInstance
{
    instance_black_temple(Map *Map) : ScriptedInstance(Map) {Initialize();};

    uint64 Najentus;
    uint64 Akama; // This is the Akama that starts the Illidan encounter.
    uint64 Akama_Shade; // This is the Akama that starts the Shade of Akama encounter.
    uint64 ShadeOfAkama;
    uint64 Supremus;
    uint64 LadyMalande;
    uint64 GathiosTheShatterer;
    uint64 HighNethermancerZerevor;
    uint64 VerasDarkshadow;
    uint64 IllidanStormrage;

    uint32 Encounters[ENCOUNTERS];

    void Initialize()
    {
        Najentus = 0;
        Akama = 0;
        Akama_Shade = 0;
        ShadeOfAkama = 0;
        Supremus = 0;
        LadyMalande = 0;
        GathiosTheShatterer = 0;
        HighNethermancerZerevor = 0;
        VerasDarkshadow = 0;
        IllidanStormrage = 0;
        
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
            case 22887:
                Najentus = creature->GetGUID();
                break;

            case 22990:
                Akama = creature->GetGUID();
                break;

            case 23890:
                Akama_Shade = creature->GetGUID();
                break;

            case 22841:
                ShadeOfAkama = creature->GetGUID();
                break;

            case 22898:
                Supremus = creature->GetGUID();
                break;

            case 22917:
                IllidanStormrage = creature->GetGUID();
                break;

            case 22949:
                GathiosTheShatterer = creature->GetGUID();
                break;

            case 22950:
                HighNethermancerZerevor = creature->GetGUID();
                break;

            case 22951:
                LadyMalande = creature->GetGUID();
                break;

            case 22952:
                VerasDarkshadow = creature->GetGUID();
                break;
        }
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case DATA_HIGHWARLORDNAJENTUS:
                return Najentus;

             case DATA_AKAMA:
                return Akama;

             case DATA_AKAMA_SHADE:
                return Akama_Shade;

             case DATA_SHADEOFAKAMA:
                return ShadeOfAkama;

             case DATA_SUPREMUS:
                return Supremus;

             case DATA_ILLIDANSTORMRAGE:
                return IllidanStormrage;

             case DATA_GATHIOSTHESHATTERER:
                return GathiosTheShatterer;

             case DATA_HIGHNETHERMANCERZEREVOR:
                return HighNethermancerZerevor;

             case DATA_LADYMALANDE:
                return LadyMalande;

             case DATA_VERASDARKSHADOW:
                return VerasDarkshadow;
        }

        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case DATA_HIGHWARLORDNAJENTUSEVENT:
                Encounters[0] = data;
                break;

            case DATA_SUPREMUSEVENT:
                Encounters[1] = data;
                break;

            case DATA_SHADEOFAKAMAEVENT:
                Encounters[2] = data;
                break;

            case DATA_TERONGOREFIENDEVENT:
                Encounters[3] = data;
                break;

            case DATA_GURTOGGBLOODBOILEVENT:
                Encounters[4] = data;
                break;

            case DATA_RELIQUARYOFSOULSEVENT:
                Encounters[5] = data;
                break;

            case DATA_MOTHERSHAHRAZEVENT:
                Encounters[6] = data;
                break;

            case DATA_ILLIDARICOUNCILEVENT:
                Encounters[7] = data;
                break;

            case DATA_ILLIDANSTORMRAGEEVENT:
                Encounters[8] = data;
                break;
        }
    }

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case DATA_HIGHWARLORDNAJENTUSEVENT:
                return Encounters[0];

             case DATA_SUPREMUSEVENT:
                return Encounters[1];

             case DATA_SHADEOFAKAMAEVENT:
                return Encounters[2];

             case DATA_TERONGOREFIENDEVENT:
                return Encounters[3];

             case DATA_GURTOGGBLOODBOILEVENT:
                return Encounters[4];

             case DATA_RELIQUARYOFSOULSEVENT:
                return Encounters[5];

             case DATA_MOTHERSHAHRAZEVENT:
                return Encounters[6];

             case DATA_ILLIDARICOUNCILEVENT:
                return Encounters[7];

             case DATA_ILLIDANSTORMRAGEEVENT:
                return Encounters[8];
        }

        return 0;
    }
};

InstanceData* GetInstanceData_instance_black_temple(Map* map)
{
    return new instance_black_temple(map);
}

void AddSC_instance_black_temple()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_black_temple";
    newscript->GetInstanceData = GetInstanceData_instance_black_temple;
    m_scripts[nrscripts++] = newscript;
}
