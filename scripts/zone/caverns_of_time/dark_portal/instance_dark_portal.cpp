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
SDName: Instance_Dark_Portal
SD%Complete: 30
SDComment: Quest support: 9836, 10297. Currently in progress.
SDCategory: Caverns of Time, The Dark Portal
EndScriptData */

#include "precompiled.h"
#include "def_dark_portal.h"

#define ENCOUNTERS              2

#define C_MEDIVH                15608
#define C_TIME_RIFT             17838

#define SPELL_RIFT_CHANNEL      31387

float PortalLocation[4][4]=
{
    {-2041.06, 7042.08, 29.99, 1.30},
    {-1968.18, 7042.11, 21.93, 2.12},
    {-1885.82, 7107.36, 22.32, 3.07},
    {-1928.11, 7175.95, 22.11, 3.44}
};

struct Wave
{
    uint32 PortalBoss;                                      //protector of current portal
    uint32 PortalMob[4];                                    //spawns for portal waves (in order)
};

static Wave RiftWaves[]=
{
    {RIFT_BOSS, C_ASSAS, C_WHELP, C_CHRON, 0},
    {C_DEJA, C_ASSAS, C_WHELP, C_CHRON, 0},
    {RIFT_BOSS, C_EXECU, C_CHRON, C_WHELP, C_ASSAS},
    {C_TEMPO, C_EXECU, C_CHRON, C_WHELP, C_ASSAS},
    {RIFT_BOSS, C_EXECU, C_VANQU, C_CHRON, C_ASSAS},
    {C_AEONUS, 0, 0, 0, 0}
};

struct MANGOS_DLL_DECL instance_dark_portal : public ScriptedInstance
{
    instance_dark_portal(Map *map) : ScriptedInstance(map) {Initialize();};

    uint32 Encounter[ENCOUNTERS];

    uint32 mRiftPortalCount;
    uint32 mShieldPercent;
    uint8 mRiftWaveCount;
    uint8 mRiftWaveId;

    uint32 TimeRift_Timer;
    bool IsRiftBoss;

    uint64 MedivhGUID;
    uint64 TimeRiftGUID[4];
    uint8 CurrentRiftId;

    void Initialize()
    {
        MedivhGUID          = 0;
        Clear();
    }

    void Clear()
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounter[i] = NOT_STARTED;

        mRiftPortalCount    = 0;
        mShieldPercent      = 100;
        mRiftWaveCount      = 0;
        mRiftWaveId         = 0;

        CurrentRiftId = 0;

        TimeRift_Timer      = 30000;
        IsRiftBoss          = true;

        for(uint8 i = 0; i < 4; i++)
            TimeRiftGUID[i] = 0;
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

        debug_log("SD2: Instance Black Portal: GetPlayerInMap, but PlayerList is empty!");
        Clear();
        return NULL;
    }

    void UpdateBMWorldState(uint32 id, uint32 state)
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* player = itr->getSource())
                    player->SendUpdateWorldState(id,state);
            }
        }else debug_log("SD2: Instance Black Portal: UpdateBMWorldState, but PlayerList is empty!");
    }

    void InitWorldState(bool Enable = true)
    {
        UpdateBMWorldState(WORLD_STATE_BM,Enable ? 1 : 0);
        UpdateBMWorldState(WORLD_STATE_BM_SHIELD,100);
        UpdateBMWorldState(WORLD_STATE_BM_RIFT,0);
    }

    bool IsEncounterInProgress()
    {
        if (GetData(TYPE_MEDIVH))
            return true;

        return false;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        if (creature->GetEntry() == C_MEDIVH)
            MedivhGUID = creature->GetGUID();
    }

    //perhaps store rift guids in list instead, make it possible (easier) to make selection return random.
    uint64 SetNextRift()
    {
        ++CurrentRiftId;

        if (CurrentRiftId >= 4)
            CurrentRiftId = 0;

        return TimeRiftGUID[CurrentRiftId];
    }

    uint8 GetRiftWaveId()
    {
        switch(mRiftPortalCount)
        {
            case 6:
                mRiftWaveId = 2;
                return 1;
            case 12:
                mRiftWaveId = 4;
                return 3;
            case 18:
                return 5;
            default:
                return mRiftWaveId;
        }
    }

    void SetData(uint32 type, uint32 data)
    {
        Player *player = GetPlayerInMap();

        if (!player)
        {
            debug_log("SD2: Instance Black Portal: SetData (Type: %u Data %u) cannot find any player.", type, data);
            return;
        }

        switch(type)
        {
            case TYPE_MEDIVH:
                if (data == SPECIAL && Encounter[0] == IN_PROGRESS)
                {
                    --mShieldPercent;
                    UpdateBMWorldState(WORLD_STATE_BM_SHIELD,mShieldPercent);

                    if (!mShieldPercent)
                    {
                        if (Unit *medivh = Unit::GetUnit(*player,MedivhGUID))
                        {
                            if (medivh->isAlive())
                            {
                                medivh->DealDamage(medivh, medivh->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                                Encounter[0] = FAIL;
                                Encounter[1] = NOT_STARTED;
                            }
                        }
                    }
                }
                else
                {
                    if (data == IN_PROGRESS)
                    {
                        debug_log("SD2: Instance Dark Portal: Starting event.");
                        InitWorldState();
                        DoSpawnPortals();
                    }

                    if (data == DONE)
                    {
                        //this may be completed further out in the post-event
                        if (Unit *medivh = Unit::GetUnit(*player,MedivhGUID))
                        {
                            player->GroupEventHappens(QUEST_OPENING_PORTAL,medivh);
                            player->GroupEventHappens(QUEST_MASTER_TOUCH,medivh);
                        }
                    }

                    Encounter[0] = data;
                }
                break;
            case TYPE_RIFT:
                if (data == SPECIAL)
                {
                    if (Unit *rift = Unit::GetUnit(*player,TimeRiftGUID[CurrentRiftId]))
                        rift->SetVisibility(VISIBILITY_OFF);

                    IsRiftBoss = true;
                }
                else
                    Encounter[1] = data;
                break;
        }
    }

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case TYPE_MEDIVH:
                return Encounter[0];
            case TYPE_RIFT:
                return Encounter[1];
        }
        return 0;
    }

    uint64 GetData64(uint32 data)
    {
        if (data == DATA_MEDIVH)
            return MedivhGUID;

        return 0;
    }

    void DoSpawnPortals()
    {
        Player *player = GetPlayerInMap();
        if (!player)
            return;

        if (Unit *medivh = Unit::GetUnit(*player,MedivhGUID))
        {
            SetData(TYPE_RIFT,IN_PROGRESS);

            for(uint8 i = 0; i < 4; i++)
            {
                Unit *temp = medivh->SummonCreature(C_TIME_RIFT,
                    PortalLocation[i][0],PortalLocation[i][1],PortalLocation[i][2],PortalLocation[i][3],
                    TEMPSUMMON_CORPSE_DESPAWN,0);
                if (temp)
                {
                    TimeRiftGUID[i] = temp->GetGUID();
                    temp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    temp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    temp->SetVisibility(VISIBILITY_OFF);
                    debug_log("SD2: Instance Dark Portal: Time Rift %u created.",i);
                }
            }
        }
    }

    void DoSummonAtRift(uint32 creature_entry)
    {
        Player *player = GetPlayerInMap();
        if (!player || !creature_entry)
            return;

        if (Unit *rift = Unit::GetUnit(*player,TimeRiftGUID[CurrentRiftId]))
        {
            float x,y,z;
            rift->GetRandomPoint(rift->GetPositionX(),rift->GetPositionY(),rift->GetPositionZ(),10.0f,x,y,z);

            //normalize Z-level if we can, if rift is not at ground level.
            z = std::max(instance->GetHeight(x, y, MAX_HEIGHT), instance->GetWaterLevel(x, y));

            Unit *Summon = rift->SummonCreature(creature_entry,x,y,z,rift->GetOrientation(),
                IsRiftBoss ? TEMPSUMMON_TIMED_OR_DEAD_DESPAWN : TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                IsRiftBoss ? 300000 : 30000);

            if (Summon && !IsRiftBoss)
            {
                if (Unit *temp = Unit::GetUnit(*player,MedivhGUID))
                    Summon->AddThreat(temp,0.0f);
            }
            else if (Summon && Summon->GetEntry() == C_AEONUS)
            {
                if (Unit *temp = Unit::GetUnit(*player,MedivhGUID))
                    Summon->AddThreat(temp,0.0f);
            }
            else if (Summon)
            {
                rift->CastSpell(Summon,SPELL_RIFT_CHANNEL,false);
                if (Unit *temp = Unit::GetUnit(*player,rift->GetGUID()))
                    Summon->AddThreat(temp,0.0f);
            }
        }
    }

    void DoSelectSummon()
    {
        if (IsRiftBoss)
        {
            debug_log("SD2: Instance Dark Portal: Summoning rift boss (Count %u, Entry %u).",
                mRiftPortalCount,RiftWaves[GetRiftWaveId()].PortalBoss);

            DoSummonAtRift(RiftWaves[GetRiftWaveId()].PortalBoss);
            IsRiftBoss = false;
        }
        else
        {
            uint32 entry = 0;

            if ((mRiftWaveCount > 2 && GetRiftWaveId() < 1) || mRiftWaveCount > 3)
                mRiftWaveCount = 0;

            entry = RiftWaves[GetRiftWaveId()].PortalMob[mRiftWaveCount];
            debug_log("SD2: Instance Dark Portal: no rift boss, summoning wave creature (Count %u, Entry %u).",
                mRiftPortalCount,entry);

            ++mRiftWaveCount;

            if (entry == C_WHELP)
            {
                for(uint8 i = 0; i < 3; i++)
                    DoSummonAtRift(entry);
            }else DoSummonAtRift(entry);
        }
    }

    void Update(uint32 diff)
    {
        if (GetData(TYPE_RIFT) == IN_PROGRESS && TimeRift_Timer)
        {
            if (TimeRift_Timer < diff)
            {
                if (IsRiftBoss)
                {
                    ++mRiftPortalCount;
                    UpdateBMWorldState(WORLD_STATE_BM_RIFT,mRiftPortalCount);

                    Player *p = GetPlayerInMap();
                    if (!p)
                        return;

                    if (Unit *newRift = Unit::GetUnit(*p,SetNextRift()))
                        newRift->SetVisibility(VISIBILITY_ON);
                }

                DoSelectSummon();
                TimeRift_Timer = 15000;
            }else TimeRift_Timer -= diff;
        }
    }
};

InstanceData* GetInstanceData_instance_dark_portal(Map* map)
{
    return new instance_dark_portal(map);
}

void AddSC_instance_dark_portal()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_dark_portal";
    newscript->GetInstanceData = &GetInstanceData_instance_dark_portal;
    newscript->RegisterSelf();
}
