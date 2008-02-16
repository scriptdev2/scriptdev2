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
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Black Temple
EndScriptData */

#include "../../sc_defines.h"

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

    uint64 GetData64(char *identifier)
    {
        if(identifier == "HighWarlordNajentus" && Najentus)
            return Najentus;
        else if(identifier == "Akama" && Akama)
            return Akama;
        else if(identifier == "Akama(Shade)" && Akama_Shade)
            return Akama_Shade;
        else if(identifier == "ShadeOfAkama" && ShadeOfAkama)
            return ShadeOfAkama;
        else if(identifier == "Supremus" && Supremus)
            return Supremus;
        else if(identifier == "IllidanStormrage" && IllidanStormrage)
            return IllidanStormrage;
        else if(identifier == "GathiosTheShatterer" && GathiosTheShatterer)
            return GathiosTheShatterer;
        else if(identifier == "HighNethermancerZerevor" && HighNethermancerZerevor)
            return HighNethermancerZerevor;
        else if(identifier == "LadyMalande" && LadyMalande)
            return LadyMalande;
        else if(identifier == "VerasDarkshadow" && VerasDarkshadow)
            return VerasDarkshadow;

        return NULL;
    }

    void SetData(char *type, uint32 data)
    {
        if(type == "HighWarlordNajentusEvent")
            Encounters[0] = data;
        else if(type == "SupremusEvent")
            Encounters[1] = data;
        else if(type == "ShadeOfAkamaEvent")
            Encounters[2] = data;
        else if(type == "TeronGorefiendEvent")
            Encounters[3] = data;
        else if(type == "GurtoggBloodboilEvent")
            Encounters[4] = data;
        else if(type == "ReliquaryOfSoulsEvent")
            Encounters[5] = data;
        else if(type == "MotherShahrazEvent")
            Encounters[6] = data;
        else if(type == "IllidariCouncilEvent")
            Encounters[7] = data;
        else if(type == "IllidanStormrageEvent")
            Encounters[8] = data;
    }

    uint32 GetData(char *type)
    {
        if(type == "HighWarlordNajentusEvent")
            return Encounters[0];
        else if(type == "SupremusEvent")
            return Encounters[1];
        else if(type == "ShadeOfAkamaEvent")
            return Encounters[2];
        else if(type == "TeronGorefiendEvent")
            return Encounters[3];
        else if(type == "GurtoggBloodboilEvent")
            return Encounters[4];
        else if(type == "ReliquaryOfSoulsEvent")
            return Encounters[5];
        else if(type == "MotherShahrazEvent")
            return Encounters[6];
        else if(type == "IllidariCouncilEvent")
            return Encounters[7];
        else if(type == "IllidanStormrageEvent")
            return Encounters[8];

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
