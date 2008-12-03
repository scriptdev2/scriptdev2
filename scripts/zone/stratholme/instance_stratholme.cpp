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
SDName: Instance_Stratholme
SD%Complete: 50
SDComment: in progress.
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"
#include "def_stratholme.h"

#define GO_SERVICE_ENTRANCE     175368
#define GO_GAUNTLET_GATE1       175357
#define GO_ZIGGURAT1            175380                      //baroness
#define GO_ZIGGURAT2            175379                      //nerub'enkan
#define GO_ZIGGURAT3            175381                      //maleki
#define GO_ZIGGURAT4            175405                      //rammstein
#define GO_ZIGGURAT5            175796                      //baron
#define GO_PORT_GAUNTLET        175374                      //port from gauntlet to slaugther
#define GO_PORT_SLAUGTHER       175373                      //port at slaugther
#define GO_PORT_ELDERS          175377                      //port at elders square

#define C_CRYSTAL               10415                       //three ziggurat crystals
#define C_BARON                 10440
#define C_YSIDA_TRIGGER         16100

#define C_RAMSTEIN              10439
#define C_ABOM_BILE             10416
#define C_ABOM_VENOM            10417

#define ENCOUNTERS              6

struct MANGOS_DLL_DECL instance_stratholme : public ScriptedInstance
{
    instance_stratholme(Map *map) : ScriptedInstance(map) {Initialize();};

    uint32 Encounter[ENCOUNTERS];
    bool IsSilverHandDead[5];

    uint32 BaronRun_Timer;

    uint64 serviceEntranceGUID;
    uint64 gauntletGate1GUID;
    uint64 ziggurat1GUID;
    uint64 ziggurat2GUID;
    uint64 ziggurat3GUID;
    uint64 ziggurat4GUID;
    uint64 ziggurat5GUID;
    uint64 portGauntletGUID;
    uint64 portSlaugtherGUID;
    uint64 portElderGUID;

    uint64 baronGUID;
    uint64 ysidaTriggerGUID;
    std::set<uint64> crystalsGUID;
    std::set<uint64> abomnationGUID;

    void Initialize()
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounter[i] = NOT_STARTED;

        for(uint8 i = 0; i < 5; i++)
            IsSilverHandDead[5] = false;

        BaronRun_Timer = 0;

        serviceEntranceGUID = 0;
        gauntletGate1GUID = 0;
        ziggurat1GUID = 0;
        ziggurat2GUID = 0;
        ziggurat3GUID = 0;
        ziggurat4GUID = 0;
        ziggurat5GUID = 0;
        portGauntletGUID = 0;
        portSlaugtherGUID = 0;
        portElderGUID = 0;

        baronGUID = 0;
        ysidaTriggerGUID = 0;
        crystalsGUID.clear();
        abomnationGUID.clear();
    }

    Player* GetPlayerInMap()
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* plr = itr->getSource())
                    return plr;
            }
        }

        debug_log("SD2: Instance Stratholme: GetPlayerInMap, but PlayerList is empty!");
        return NULL;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature->GetEntry())
        {
            case C_BARON:           baronGUID = creature->GetGUID(); break;
            case C_YSIDA_TRIGGER:   ysidaTriggerGUID = creature->GetGUID(); break;
            case C_CRYSTAL:         crystalsGUID.insert(creature->GetGUID()); break;
            case C_ABOM_BILE:
            case C_ABOM_VENOM:      abomnationGUID.insert(creature->GetGUID()); break;
        }
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry())
        {
            case GO_SERVICE_ENTRANCE:   serviceEntranceGUID = go->GetGUID(); break;
            case GO_GAUNTLET_GATE1:     gauntletGate1GUID = go->GetGUID(); break;
            case GO_ZIGGURAT1:          ziggurat1GUID = go->GetGUID(); break;
            case GO_ZIGGURAT2:          ziggurat2GUID = go->GetGUID(); break;
            case GO_ZIGGURAT3:          ziggurat3GUID = go->GetGUID(); break;
            case GO_ZIGGURAT4:          ziggurat4GUID = go->GetGUID(); break;
            case GO_ZIGGURAT5:          ziggurat5GUID = go->GetGUID(); break;
            case GO_PORT_GAUNTLET:      portGauntletGUID = go->GetGUID(); break;
            case GO_PORT_SLAUGTHER:     portSlaugtherGUID = go->GetGUID(); break;
            case GO_PORT_ELDERS:        portElderGUID = go->GetGUID(); break;
        }
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case TYPE_BARON_RUN:
                Encounter[0] = data;
                break;
            case TYPE_BARONESS:
                Encounter[1] = data;
                break;
            case TYPE_NERUB:
                Encounter[2] = data;
                break;
            case TYPE_PALLID:
                Encounter[3] = data;
                break;
            case TYPE_RAMSTEIN:
                Encounter[4] = data;
                break;
            case TYPE_BARON:
                Encounter[5] = data;
                break;

            case TYPE_SH_AELMAR:
                IsSilverHandDead[0] = (data) ? true : false;
                break;
            case TYPE_SH_CATHELA:
                IsSilverHandDead[1] = (data) ? true : false;
                break;
            case TYPE_SH_GREGOR:
                IsSilverHandDead[2] = (data) ? true : false;
                break;
            case TYPE_SH_NEMAS:
                IsSilverHandDead[3] = (data) ? true : false;
                break;
            case TYPE_SH_VICAR:
                IsSilverHandDead[4] = (data) ? true : false;
                break;
        }
    }

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case TYPE_SH_QUEST:
                if (IsSilverHandDead[0] && IsSilverHandDead[1] && IsSilverHandDead[2] && IsSilverHandDead[3] && IsSilverHandDead[4])
                    return 1;
                return 0;
            case TYPE_BARON_RUN:
                return Encounter[0];
            case TYPE_BARONESS:
                return Encounter[1];
            case TYPE_NERUB:
                return Encounter[2];
            case TYPE_PALLID:
                return Encounter[3];
            case TYPE_RAMSTEIN:
                return Encounter[4];
            case TYPE_BARON:
                return Encounter[5];
        }
        return 0;
    }

    uint64 GetData64(uint32 type)
    {
        switch(type)
        {
            case DATA_BARON:
                return baronGUID;
            case DATA_YSIDA_TRIGGER:
                return ysidaTriggerGUID;
        }
        return 0;
    }

    void Update(uint32 diff)
    {
        if (BaronRun_Timer)
        {
            if (BaronRun_Timer <= diff)
            {
                if (GetData(TYPE_BARON_RUN) != DONE)
                    SetData(TYPE_BARON_RUN, FAIL);
                BaronRun_Timer = 0;
            }else BaronRun_Timer -= diff;
        }
    }
};

InstanceData* GetInstanceData_instance_stratholme(Map* map)
{
    return new instance_stratholme(map);
}

void AddSC_instance_stratholme()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_stratholme";
    newscript->GetInstanceData = &GetInstanceData_instance_stratholme;
    newscript->RegisterSelf();
}
