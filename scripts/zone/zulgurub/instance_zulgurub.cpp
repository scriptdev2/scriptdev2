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
SDName: Instance_ZulGurub
SD%Complete: 80
SDComment: Missing reset function after killing a boss for Ohgan, Thekal.
EndScriptData */

#include "../../sc_defines.h"

struct MANGOS_DLL_DECL instance_zulgurub : public ScriptedInstance
{
    instance_zulgurub(Map *Map) : ScriptedInstance(Map) {Initialize();};

    //If all High Priest bosses were killed. Lorkhan, Zath and Ohgan are added too.
    bool IsBossDied[9];

    //Storing Lorkhan, Zath and Thekal because we need to cast on them later. Jindo is needed for healfunction too.
    uint64 LorKhanGUID;
    uint64 ZathGUID;
    uint64 ThekalGUID;
    uint64 JindoGUID;

    void OnCreatureCreate (Creature *creature, uint32 creature_entry)
    {
    switch (creature_entry) {
        case 11347:
        LorKhanGUID = creature->GetGUID();
        break;

        case 11348:
        ZathGUID = creature->GetGUID();
        break;

        case 14509:
        ThekalGUID = creature->GetGUID();
        break;

        case 11380:
        JindoGUID = creature->GetGUID();
        break;
    }
    } 

    void Initialize()
    {
        IsBossDied[0] = false;
        IsBossDied[1] = false;
        IsBossDied[2] = false;
        IsBossDied[3] = false;
        IsBossDied[4] = false;
        IsBossDied[5] = false;
        IsBossDied[6] = false;

        IsBossDied[7] = false;
        IsBossDied[8] = false;
    }

    bool IsEncounterInProgress() const 
    {
        //not active in Zul'Gurub
        return false;
    }

    uint32 GetData(char *type)
    {

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

        if(type == "ArlokkIsDead")
            if(IsBossDied[4])
                return 1;

        if(type == "LorKhanIsDead")
            if(IsBossDied[5])
                return 1;

        if(type == "ZathIsDead")
            if(IsBossDied[6])
                return 1;

        if(type == "ThekalIsFakeDead")
            if(IsBossDied[7])
                return 1;

        if(type == "OhganIsDead")
            if(IsBossDied[8])
                return 1;

        //Boss is not dead. Resetting function for some bosses after killing them but whiping at the complete encounter.

        if(type == "ThekalIsAlive")
            if(IsBossDied[3])
                return 0;

        if(type == "LorKhanIsAlive")
            if(IsBossDied[5])
                return 0;

        if(type == "ZathIsAlive")
            if(IsBossDied[6])
                return 0;

        return 0;
    }

    uint64 GetData64 (char *identifier) {
        if (identifier == "LorKhan")
            return LorKhanGUID;
        if (identifier == "Zath")
            return ZathGUID;
        if (identifier == "Thekal")
            return ThekalGUID;
        if (identifier == "Jindo")
            return JindoGUID;
        return 0;
    } // end GetData64

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
        else if(type == "LorKhan_Death")
            IsBossDied[5] = true;
        else if(type == "Zath_Death")
            IsBossDied[6] = true;
        else if(type == "ThekalFake_Death")
        IsBossDied[7] = true;
        else if(type == "Ohgan_Death")
            IsBossDied[8] = true;
        else if(type == "LorKhan_Alive")
            IsBossDied[5] = false;
        else if(type == "Zath_Alive")
            IsBossDied[6] = false;
        else if(type == "Thekal_Alive")
            IsBossDied[7] = false;
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
