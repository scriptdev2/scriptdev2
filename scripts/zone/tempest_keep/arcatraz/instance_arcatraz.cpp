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
SDName: Instance_arcatraz
SD%Complete: 60
SDComment: Harbringer Skyriss not implemented
SDCategory: Tempest Keep, Arcatraz
EndScriptData */

#include "def_arcatraz.h"
#include "GameObject.h"

#define ENCOUNTERS 9

#define CONTAINMENT_CORE_SECURITY_FIELD_ALPHA 184318        //door opened when Wrath-Scryer Soccothrates dies
#define CONTAINMENT_CORE_SECURITY_FIELD_BETA  184319        //door opened when Dalliah the Doomsayer dies
#define POD_ALPHA   183961                                  //pod first boss wave
#define POD_BETA    183963                                  //pod second boss wave
#define POD_DELTA   183964                                  //pod third boss wave
#define POD_GAMMA   183962                                  //pod fourth boss wave
#define POD_OMEGA   183965                                  //pod fifth boss wave

/* Arcatraz encounters:
1 - Zereketh the Unbound event
2 - Dalliah the Doomsayer event
3 - Wrath-Scryer Soccothrates event
4 - Harbinger Skyriss event, 5 sub-events
*/

struct MANGOS_DLL_DECL instance_arcatraz : public ScriptedInstance
{
    instance_arcatraz(Map *Map) : ScriptedInstance(Map) {Initialize();};

    bool Encounter[ENCOUNTERS];

    GameObject *Containment_Core_Security_Field_Alpha;
    GameObject *Containment_Core_Security_Field_Beta;
    GameObject *Pod_Alpha;
    GameObject *Pod_Gamma;
    GameObject *Pod_Beta;
    GameObject *Pod_Delta;
    GameObject *Pod_Omega;

    void Initialize()
    {
        Containment_Core_Security_Field_Alpha = NULL;
        Containment_Core_Security_Field_Beta  = NULL;
        Pod_Alpha = NULL;
        Pod_Beta  = NULL;
        Pod_Delta = NULL;
        Pod_Gamma = NULL;
        Pod_Omega = NULL;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounter[i] = false;
    }

    bool IsEncounterInProgress() const 
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounter[i]) return true;

        return false;
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry())
        {
            case CONTAINMENT_CORE_SECURITY_FIELD_ALPHA:
                Containment_Core_Security_Field_Alpha = go;
                break;
            case CONTAINMENT_CORE_SECURITY_FIELD_BETA:
                Containment_Core_Security_Field_Beta = go;
                break;
            case POD_ALPHA:
                Pod_Alpha = go;
                break;
            case POD_BETA:
                Pod_Beta = go;
                break;
            case POD_DELTA:
                Pod_Delta = go;
                break;
            case POD_GAMMA:
                Pod_Gamma = go;
                break;
            case POD_OMEGA:
                Pod_Omega = go;
                break;
        }
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case TYPE_ZEREKETH:
                Encounter[0] = (data) ? true : false;
                break;

            case TYPE_DALLIAH:
            {
                if(data == DONE)
                {
                    Encounter[1] = DONE;
                    if(Containment_Core_Security_Field_Beta)
                        Containment_Core_Security_Field_Beta->UseDoorOrButton();
                }
                else
                    Encounter[1] = NOT_STARTED;
                break;
            }

            case TYPE_SOCCOTHRATES:
            {
                if(data == DONE)
                {
                    Encounter[2] = DONE;
                    if(Containment_Core_Security_Field_Alpha)
                        Containment_Core_Security_Field_Alpha->UseDoorOrButton();
                }
                else
                    Encounter[2] = NOT_STARTED;
                break;
            }

            case TYPE_HARBINGERSKYRISS:
                Encounter[3] = (data) ? true : false;
                break;

            case TYPE_WARDEN_1:
            {
                switch(data)
                {
                    case IN_PROGRESS:
                        Encounter[4] = IN_PROGRESS;
                        if(Pod_Alpha)
                            Pod_Alpha->UseDoorOrButton();
                        break;
                    case DONE:
                        Encounter[4] = DONE;
                        break;
                    default:
                        Encounter[4] = (data) ? FAIL : NOT_STARTED;
                        break;
                }
            }
            case TYPE_WARDEN_2:
            {
                switch(data)
                {
                    case IN_PROGRESS:
                        Encounter[5] = IN_PROGRESS;
                        if(Pod_Beta)
                            Pod_Beta->UseDoorOrButton();
                        break;
                    case DONE:
                        Encounter[5] = DONE;
                        break;
                    default:
                        Encounter[5] = (data) ? FAIL : NOT_STARTED;
                        break;
                }
            }
            case TYPE_WARDEN_3:
            {
                switch(data)
                {
                    case IN_PROGRESS:
                        Encounter[6] = IN_PROGRESS;
                        if(Pod_Delta)
                            Pod_Delta->UseDoorOrButton();
                        break;
                    case DONE:
                        Encounter[6] = DONE;
                        break;
                    default:
                        Encounter[6] = (data) ? FAIL : NOT_STARTED;
                        break;
                }
            }
            case TYPE_WARDEN_4:
            {
                switch(data)
                {
                    case IN_PROGRESS:
                        Encounter[7] = IN_PROGRESS;
                        if(Pod_Gamma)
                            Pod_Gamma->UseDoorOrButton();
                        break;
                    case DONE:
                        Encounter[7] = DONE;
                        break;
                    default:
                        Encounter[7] = (data) ? FAIL : NOT_STARTED;
                        break;
                }
            }
            case TYPE_WARDEN_5:
            {
                switch(data)
                {
                    case IN_PROGRESS:
                        Encounter[8] = IN_PROGRESS;
                        if(Pod_Omega)
                            Pod_Omega->UseDoorOrButton();
                        break;
                    case DONE:
                        Encounter[8] = DONE;
                        break;
                    default:
                        Encounter[8] = (data) ? FAIL : NOT_STARTED;
                        break;
                }
            }

        }
    }
};

InstanceData* GetInstanceData_instance_arcatraz(Map* map)
{
    return new instance_arcatraz(map);
}

void AddSC_instance_arcatraz()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_arcatraz";
    newscript->GetInstanceData = GetInstanceData_instance_arcatraz;
    m_scripts[nrscripts++] = newscript;
}
