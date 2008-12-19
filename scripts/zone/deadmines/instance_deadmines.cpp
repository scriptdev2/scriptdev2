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
SDName: Instance_Deadmines
SD%Complete: 0
SDComment: Placeholder
SDCategory: Deadmines
EndScriptData */

#include "precompiled.h"
#include "def_deadmines.h"

#define INST_SAY_ALARM1     -1036000
#define INST_SAY_ALARM2     -1036001

#define GO_DOOR_LEVER       101833
#define GO_IRON_CLAD        16397
#define GO_DEFIAS_CANNON    16398
#define C_MR_SMITE          646
#define C_PIRATE            657

#define ENCOUNTERS          1

struct MANGOS_DLL_DECL instance_deadmines : public ScriptedInstance
{
    instance_deadmines(Map *map) : ScriptedInstance(map) {Initialize();};

    uint32 Encounter[ENCOUNTERS];

    uint64 ironCladGUID;
    uint64 cannonGUID;
    uint64 smiteGUID;

    uint32 IronDoor_Timer;
    uint32 Door_Step;

    void Initialize()
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounter[i] = NOT_STARTED;

        ironCladGUID = 0;
        cannonGUID = 0;
        smiteGUID = 0;

        IronDoor_Timer = 0;
        Door_Step = 0;
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

        debug_log("SD2: Instance Deadmines: GetPlayerInMap, but PlayerList is empty!");
        return NULL;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        if (creature->GetEntry() == C_MR_SMITE)
            smiteGUID = creature->GetGUID();
    }

    void OnObjectCreate(GameObject *go)
    {
        if (go->GetEntry() == GO_IRON_CLAD)
            ironCladGUID = go->GetGUID();

        if (go->GetEntry() == GO_DEFIAS_CANNON)
            cannonGUID = go->GetGUID();
    }

    void SetData(uint32 type, uint32 data)
    {
        Player *player = GetPlayerInMap();

        if (!player)
        {
            debug_log("SD2: Instance Deadmines: SetData (Type: %u Data %u) cannot find any player.", type, data);
            return;
        }

        if (type == TYPE_DEFIAS_ENDDOOR)
        {
            if (data == IN_PROGRESS)
            {
                if (GameObject *go = GameObject::GetGameObject(*player,ironCladGUID))
                {
                    go->SetGoState(2);
                    IronDoor_Timer = 3000;
                }
            }
            Encounter[0] = data;
        }
    }

    uint32 GetData(uint32 type)
    {
        if (type = TYPE_DEFIAS_ENDDOOR)
            return Encounter[0];

        return 0;
    }

    uint64 GetData64(uint32 data)
    {
        if (data = DATA_DEFIAS_DOOR)
            return ironCladGUID;

        return 0;
    }

    void Update(uint32 diff)
    {
        if (IronDoor_Timer)
        {
            if (IronDoor_Timer <= diff)
            {
                Player *player = GetPlayerInMap();
                if (!player)
                    return;

                Unit *Smite = Unit::GetUnit(*player,smiteGUID);
                if (!Smite)
                    return;

                Creature *pi1 = NULL;
                Creature *pi2 = NULL;

                switch(Door_Step)
                {
                    case 0:
                        DoScriptText(INST_SAY_ALARM1,Smite);
                        IronDoor_Timer = 2000;
                        ++Door_Step;
                        break;
                    case 1:
                        pi1 = Smite->SummonCreature(C_PIRATE, 93.68,-678.63,7.71,2.09, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000);
                        if (pi1) pi1->GetMotionMaster()->MovePoint(0,100.11,-670.65,7.42);
                        pi2 = Smite->SummonCreature(C_PIRATE,102.63,-685.07,7.42,1.28, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000);
                        if (pi2) pi2->GetMotionMaster()->MovePoint(0,100.11,-670.65,7.42);
                        ++Door_Step;
                        IronDoor_Timer = 10000;
                        break;
                    case 2:
                        DoScriptText(INST_SAY_ALARM2,Smite);
                        Door_Step = 0;
                        IronDoor_Timer = 0;
                        debug_log("SD2: Instance Deadmines: Iron door event reached end.");
                        break;
                }
            }else IronDoor_Timer -= diff;
        }
    }
};

InstanceData* GetInstanceData_instance_deadmines(Map* map)
{
    return new instance_deadmines(map);
}

void AddSC_instance_deadmines()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_deadmines";
    newscript->GetInstanceData = &GetInstanceData_instance_deadmines;
    newscript->RegisterSelf();
}
