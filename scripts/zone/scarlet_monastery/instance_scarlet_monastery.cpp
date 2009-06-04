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
SDName: Instance_Scarlet_Monastery
SD%Complete: 50
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "precompiled.h"
#include "def_scarlet_monastery.h"

#define ENCOUNTERS 1

struct MANGOS_DLL_DECL instance_scarlet_monastery : public ScriptedInstance
{
    instance_scarlet_monastery(Map *map) : ScriptedInstance(map) {Initialize();};

    uint64 MograineGUID;
    uint64 WhitemaneGUID;
    uint64 VorrelGUID;
    uint64 DoorHighInquisitorGUID;

    uint32 Encounter[ENCOUNTERS];

    void Initialize()
    {
        MograineGUID = 0;
        WhitemaneGUID = 0;
        VorrelGUID = 0;
        DoorHighInquisitorGUID = 0;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounter[i] = NOT_STARTED;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 3976: MograineGUID = pCreature->GetGUID(); break;
            case 3977: WhitemaneGUID = pCreature->GetGUID(); break;
            case 3981: VorrelGUID = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject *go)
    {
        if (go->GetEntry() == 104600)
            DoorHighInquisitorGUID = go->GetGUID();
    }

    uint64 GetData64(uint32 data)
    {
        switch(data)
        {
            case DATA_MOGRAINE:
                return MograineGUID;
            case DATA_WHITEMANE:
                return WhitemaneGUID;
            case DATA_VORREL:
                return VorrelGUID;
            case DATA_DOOR_WHITEMANE:
                return DoorHighInquisitorGUID;
        }

        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        if (type == TYPE_MOGRAINE_AND_WHITE_EVENT)
        {
            if (data == IN_PROGRESS)
                DoUseDoorOrButton(DoorHighInquisitorGUID);
            if (data == FAIL)
                DoUseDoorOrButton(DoorHighInquisitorGUID);

            Encounter[0] = data;
        }
    }

    uint32 GetData(uint32 data)
    {
        if (data == TYPE_MOGRAINE_AND_WHITE_EVENT)
            return Encounter[0];

        return 0;
    }
};

InstanceData* GetInstanceData_instance_scarlet_monastery(Map* map)
{
    return new instance_scarlet_monastery(map);
}

void AddSC_instance_scarlet_monastery()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_scarlet_monastery";
    newscript->GetInstanceData = &GetInstanceData_instance_scarlet_monastery;
    newscript->RegisterSelf();
}
