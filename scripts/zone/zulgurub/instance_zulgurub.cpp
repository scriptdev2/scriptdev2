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
SDName: Instance_ZulGurub
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

struct MANGOS_DLL_DECL instance_zulgurub : public ScriptedInstance
{
    instance_zulgurub(Map *Map) : ScriptedInstance(Map) {Initialize();};

    //If all High Priest bosses were killed.
    bool IsBossDied[5];

    void Initialize()
    {
        IsBossDied[0] = false;
        IsBossDied[1] = false;
        IsBossDied[2] = false;
        IsBossDied[3] = false;
        IsBossDied[4] = false;
    }

    bool IsEncounterInProgress() const 
    {
        //not active in Zul'Gurub
        return false;
    }

    uint32 GetData(char *type)
    {
        if(type == "HakkarNormal")
            if(IsBossDied[0] && IsBossDied[1] && IsBossDied[2] && IsBossDied[3] && IsBossDied[4])
                return 1;

        if(type == "JeklikIsDead")
            if(IsBossDied[0])
                return 1;

        if(type == "VenoxisIsDead")
            if(IsBossDied[1])
                return 1;

        if(type == "MarliIsDead")
            if(IsBossDied[2])
                return 1;

        if(type == "ThekalIsDead")
            if(IsBossDied[3])
                return 1;

        if(type == "ThekalIsDead")
            if(IsBossDied[4])
                return 1;



        return 0;
    }

    void SetData(char *type, uint32 data)
    {
        if(type == "Jeklik_Death")
            IsBossDied[0] = true;
        else if(type == "Venoxis_Death")
            IsBossDied[1] = true;
        else if(type == "Marli_Death")
            IsBossDied[2] = true;
        else if(type == "Thekal_Death")
            IsBossDied[3] = true;
        else if(type == "Arlokk_Death")
            IsBossDied[4] = true;
    }
};

InstanceData* GetInstanceData_instance_zulgurub(Map* map)
{
    return new instance_zulgurub(map);
}

void AddSC_instance_zulgurub()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_zulgurub";
    newscript->GetInstanceData = GetInstanceData_instance_zulgurub;
    m_scripts[nrscripts++] = newscript;
}
