/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Instance_Zulaman
SD%Complete: 60
SDComment:
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "def_zulaman.h"

#define ENCOUNTERS     7
#define RAND_VENDOR    2

struct MANGOS_DLL_DECL instance_zulaman : public ScriptedInstance
{
    instance_zulaman(Map *map) : ScriptedInstance(map) {Initialize();};

    uint64 akilzonGUID;
    uint64 nalorakkGUID;
    uint64 janalaiGUID;
    uint64 halazziGUID;
    uint64 zuljinGUID;
    uint64 malacrassGUID;

    uint64 StrangeGongGUID;
    uint64 HexlordEntranceGUID;

    uint32 janalai_eggs_l;
    uint32 janalai_eggs_r;

    uint32 Encounters[ENCOUNTERS];
    uint32 RandVendor[RAND_VENDOR];

    void Initialize()
    {
        akilzonGUID = 0;
        nalorakkGUID = 0;
        janalaiGUID = 0;
        halazziGUID = 0;
        zuljinGUID = 0;
        malacrassGUID = 0;

        StrangeGongGUID = 0;
        HexlordEntranceGUID = 0;

        janalai_eggs_l = 20;
        janalai_eggs_r = 20;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = NOT_STARTED;

        for(uint8 i = 0; i < RAND_VENDOR; i++)
            RandVendor[i] = NOT_STARTED;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature->GetEntry())
        {
            case 23574: akilzonGUID     = creature->GetGUID(); break;
            case 23576: nalorakkGUID    = creature->GetGUID(); break;
            case 23578: janalaiGUID     = creature->GetGUID(); break;
            case 23577: halazziGUID     = creature->GetGUID(); break;
            case 23863: zuljinGUID      = creature->GetGUID(); break;
            case 24239: malacrassGUID   = creature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry())
        {
            case 187359: StrangeGongGUID        = go->GetGUID(); break;
            case 186305: HexlordEntranceGUID    = go->GetGUID(); break;
        }
    }

    uint64 GetData64(uint32 data)
    {
        switch(data)
        {
            case DATA_AKILZON:
                return akilzonGUID;
            case DATA_NALORAKK:
                return nalorakkGUID;
            case DATA_JANALAI:
                return janalaiGUID;
            case DATA_HALAZZI:
                return halazziGUID;
            case DATA_ZULJIN:
                return zuljinGUID;
            case DATA_MALACRASS:
                return malacrassGUID;
            case DATA_GO_GONG:
                return StrangeGongGUID;
            case DATA_GO_HEXLORD_GATE:
                return HexlordEntranceGUID;
        }
        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case TYPE_JANALAI:
                if (data == NOT_STARTED)
                {
                    janalai_eggs_l = 20;
                    janalai_eggs_r = 20;
                }
                Encounters[3] = data;
                break;
            case DATA_J_HATCHLEFT:
                janalai_eggs_l -= data;
                break;
            case DATA_J_HATCHRIGHT:
                janalai_eggs_r -= data;
                break;
            case TYPE_RAND_VENDOR_1:
                RandVendor[0] = data;
                break;
            case TYPE_RAND_VENDOR_2:
                RandVendor[1] = data;
                break;
            default:
                error_log("SD2: Zul'Aman: ERROR SetData = %u for type %u does not exist/not implemented.",data,type);
                break;
        }
    }

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case TYPE_EVENT_RUN:
                return Encounters[0];
            case TYPE_AKILZON:
                return Encounters[1];
            case TYPE_NALORAKK:
                return Encounters[2];
            case TYPE_JANALAI:
                return Encounters[3];
            case TYPE_HALAZZI:
                return Encounters[4];
            case TYPE_ZULJIN:
                return Encounters[5];
            case TYPE_MALACRASS:
                return Encounters[6];

            case DATA_J_EGGSLEFT:
                return janalai_eggs_l;
            case DATA_J_EGGSRIGHT:
                return janalai_eggs_r;

            case TYPE_RAND_VENDOR_1:
                return RandVendor[0];
            case TYPE_RAND_VENDOR_2:
                return RandVendor[1];
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_zulaman(Map* map)
{
    return new instance_zulaman(map);
}

void AddSC_instance_zulaman()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_zulaman";
    newscript->GetInstanceData = &GetInstanceData_instance_zulaman;
    newscript->RegisterSelf();
}
