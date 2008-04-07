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
SDName: Instance_Karazhan
SD%Complete: 0
SDComment: 
EndScriptData */

#include "sc_creature.h"
#include "def_karazhan.h"
#include "GameObject.h"

#define ENCOUNTERS      11

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAILED        = 2,
    DONE          = 3
};

/*
0  - Attumen + Midnight (optional)
1  - Moroes
2  - Maiden of Virtue (optional)
3  - Hyakiss the Lurker /  Rokad the Ravager  / Shadikith the Glider 
4  - Opera Event
5  - Curator
6  - Shade of Aran (optional)
7  - Terestian Illhoof (optional)
8  - Netherspite (optional)
9  - Chess Event
10 - Prince Malchezzar
11 - Netherbane
*/
struct MANGOS_DLL_DECL instance_karazhan : public ScriptedInstance
{
    instance_karazhan(Map* map) : ScriptedInstance(map)
    {
        Initialize();
    }

    uint32 Encounters[ENCOUNTERS];

    uint32 OperaEvent; // 0 - OZ, 1 - RAJ, 2 - HOOD
    uint32 BarnesIntro; // 0 - Not Complete, 1 - In Progress, 2 - Complete

    GameObject* Curtain;
    GameObject* Door;

    void Initialize()
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = NOT_STARTED;

        OperaEvent = rand()%3; // This never gets altered.
        BarnesIntro = 0;

        Curtain = NULL;
        Door = NULL;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i] == IN_PROGRESS) return true;
        return false;
    }

    uint32 GetData(uint32 identifier)
    {
        switch(identifier)
        {
            case DATA_ATTUMEN_EVENT:
                return Encounters[0];

            case DATA_MOROES_EVENT:
                return Encounters[1];

            case DATA_MAIDENOFVIRTUE_EVENT:
                return Encounters[2];

            case DATA_OPTIONAL_BOSS_EVENT:
                return Encounters[3];

            case DATA_OPERA_EVENT:
                return Encounters[4];
                
            case DATA_CURATOR_EVENT:
                return Encounters[5];

            case DATA_SHADEOFARAN_EVENT:
                return Encounters[6];

            case DATA_TERESTIAN_EVENT:
                return Encounters[7];

            case DATA_CHESS_EVENT:
                return Encounters[8];

            case DATA_MALCHEZZAR_EVENT:
                return Encounters[9];

            case DATA_NETHERBANE_EVENT:
                return Encounters[10];

            case DATA_OPERA_PERFORMANCE:
                return OperaEvent;

            case DATA_BARNES_INTRO:
                return BarnesIntro;
        }

        return 0;
    }

    void SetData(uint32 identifier, uint32 data)
    {
        switch(identifier)
        {                
            case DATA_ATTUMEN_EVENT:
                Encounters[0] = data;
                break;

            case DATA_MOROES_EVENT:
                 Encounters[1] = data;
                 break;

            case DATA_MAIDENOFVIRTUE_EVENT:
                 Encounters[2] = data;
                 break;

            case DATA_OPTIONAL_BOSS_EVENT:
                 Encounters[3] = data;
                 break;

            case DATA_OPERA_EVENT:
                 Encounters[4] = data;
                 break;
                
            case DATA_CURATOR_EVENT:
                 Encounters[5] = data;
                 break;

            case DATA_SHADEOFARAN_EVENT:
                 Encounters[6] = data;
                 break;

            case DATA_TERESTIAN_EVENT:
                 Encounters[7] = data;
                 break;

            case DATA_CHESS_EVENT:
                 Encounters[8] = data;
                 break;

            case DATA_MALCHEZZAR_EVENT:
                 Encounters[9] = data;
                 break;

            case DATA_NETHERBANE_EVENT:
                 Encounters[10] = data;
                 break;

            case DATA_BARNES_INTRO:
                BarnesIntro = data;
                CheckStatus();
                break;

        }
    }

    void CheckStatus()
    {
        switch(BarnesIntro)
        {
            case 1: // In Progress
                SetGOState(Door, 1);
                break;
            case 2: // Complete
                SetGOState(Curtain, 1);
                break;
        }
    }

    void SetGOState(GameObject* go, uint32 state)
    {
        if(go)
            go->SetUInt32Value(GAMEOBJECT_STATE, state);
    }

    void OnObjectCreate(GameObject* go)
    {
        switch(go->GetEntry())
        {
            case 183932:
                Curtain = go;
                break;
            case 184278:
                Door = go;
                break;
        }
    }
};

InstanceData* GetInstanceData_instance_karazhan(Map* map)
{
    return new instance_karazhan(map);
}

void AddSC_instance_karazhan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_karazhan";
    newscript->GetInstanceData = GetInstanceData_instance_karazhan;
    m_scripts[nrscripts++] = newscript;
}
