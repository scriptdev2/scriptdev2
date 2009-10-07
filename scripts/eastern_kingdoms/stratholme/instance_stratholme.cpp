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
SDName: Instance_Stratholme
SD%Complete: 50
SDComment: In progress. Undead side 75% implemented. Save/load not implemented.
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"
#include "stratholme.h"

enum
{
    MAX_ENCOUNTER               = 6,

    GO_SERVICE_ENTRANCE         = 175368,
    GO_GAUNTLET_GATE1           = 175357,
    GO_ZIGGURAT1                = 175380,                   //baroness
    GO_ZIGGURAT2                = 175379,                   //nerub'enkan
    GO_ZIGGURAT3                = 175381,                   //maleki
    GO_ZIGGURAT4                = 175405,                   //rammstein
    GO_ZIGGURAT5                = 175796,                   //baron
    GO_PORT_GAUNTLET            = 175374,                   //port from gauntlet to slaugther
    GO_PORT_SLAUGTHER           = 175373,                   //port at slaugther
    GO_PORT_ELDERS              = 175377,                   //port at elders square

    NPC_CRYSTAL                 = 10415,                    //three ziggurat crystals
    NPC_BARON                   = 10440,
    NPC_YSIDA_TRIGGER           = 16100,

    NPC_RAMSTEIN                = 10439,
    NPC_ABOM_BILE               = 10416,
    NPC_ABOM_VENOM              = 10417,
    NPC_BLACK_GUARD             = 10394,
    NPC_YSIDA                   = 16031
};

struct MANGOS_DLL_DECL instance_stratholme : public ScriptedInstance
{
    instance_stratholme(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    bool IsSilverHandDead[5];

    uint32 m_uiBaronRun_Timer;
    uint32 m_uiSlaugtherSquare_Timer;

    uint64 m_uiServiceEntranceGUID;
    uint64 m_uiGauntletGate1GUID;
    uint64 m_uiZiggurat1GUID;
    uint64 m_uiZiggurat2GUID;
    uint64 m_uiZiggurat3GUID;
    uint64 m_uiZiggurat4GUID;
    uint64 m_uiZiggurat5GUID;
    uint64 m_uiPortGauntletGUID;
    uint64 m_uiPortSlaugtherGUID;
    uint64 m_uiPortElderGUID;

    uint64 m_uiBaronGUID;
    uint64 m_uiYsidaTriggerGUID;
    std::set<uint64> crystalsGUID;
    std::set<uint64> abomnationGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        for(uint8 i = 0; i < 5; ++i)
            IsSilverHandDead[i] = false;

        m_uiBaronRun_Timer = 0;
        m_uiSlaugtherSquare_Timer = 0;

        m_uiServiceEntranceGUID = 0;
        m_uiGauntletGate1GUID = 0;
        m_uiZiggurat1GUID = 0;
        m_uiZiggurat2GUID = 0;
        m_uiZiggurat3GUID = 0;
        m_uiZiggurat4GUID = 0;
        m_uiZiggurat5GUID = 0;
        m_uiPortGauntletGUID = 0;
        m_uiPortSlaugtherGUID = 0;
        m_uiPortElderGUID = 0;

        m_uiBaronGUID = 0;
        m_uiYsidaTriggerGUID = 0;

        crystalsGUID.clear();
        abomnationGUID.clear();
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; i++)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;
        return false;
    }

    bool StartSlaugtherSquare()
    {
        //change to DONE when crystals implemented
        if (m_auiEncounter[1] == IN_PROGRESS && m_auiEncounter[2] == IN_PROGRESS && m_auiEncounter[3] == IN_PROGRESS)
        {
            UpdateGoState(m_uiPortGauntletGUID,0,false);
            UpdateGoState(m_uiPortSlaugtherGUID,0,false);
            return true;
        }

        debug_log("SD2: Instance Stratholme: Cannot open slaugther square yet.");
        return false;
    }

    //if withRestoreTime true, then newState will be ignored and GO should be restored to original state after 10 seconds
    void UpdateGoState(uint64 goGuid, uint32 newState, bool withRestoreTime)
    {
        if (!goGuid)
            return;

        if (GameObject* pGo = instance->GetGameObject(goGuid))
        {
            if (withRestoreTime)
                pGo->UseDoorOrButton(10);
            else
                pGo->SetGoState(GOState(newState));
        }
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_BARON:           m_uiBaronGUID = pCreature->GetGUID(); break;
            case NPC_YSIDA_TRIGGER:   m_uiYsidaTriggerGUID = pCreature->GetGUID(); break;
            case NPC_CRYSTAL:         crystalsGUID.insert(pCreature->GetGUID()); break;
            case NPC_ABOM_BILE:
            case NPC_ABOM_VENOM:      abomnationGUID.insert(pCreature->GetGUID()); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_SERVICE_ENTRANCE:   m_uiServiceEntranceGUID = pGo->GetGUID(); break;
            case GO_GAUNTLET_GATE1:
                //weird, but unless flag is set, client will not respond as expected. DB bug?
                pGo->SetFlag(GAMEOBJECT_FLAGS,GO_FLAG_LOCKED);
                m_uiGauntletGate1GUID = pGo->GetGUID();
                break;
            case GO_ZIGGURAT1:          m_uiZiggurat1GUID = pGo->GetGUID(); break;
            case GO_ZIGGURAT2:          m_uiZiggurat2GUID = pGo->GetGUID(); break;
            case GO_ZIGGURAT3:          m_uiZiggurat3GUID = pGo->GetGUID(); break;
            case GO_ZIGGURAT4:          m_uiZiggurat4GUID = pGo->GetGUID(); break;
            case GO_ZIGGURAT5:          m_uiZiggurat5GUID = pGo->GetGUID(); break;
            case GO_PORT_GAUNTLET:      m_uiPortGauntletGUID = pGo->GetGUID(); break;
            case GO_PORT_SLAUGTHER:     m_uiPortSlaugtherGUID = pGo->GetGUID(); break;
            case GO_PORT_ELDERS:        m_uiPortElderGUID = pGo->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_BARON_RUN:
                switch(uiData)
                {
                    case IN_PROGRESS:
                        if (m_auiEncounter[0] == IN_PROGRESS || m_auiEncounter[0] == FAIL)
                            break;
                        m_uiBaronRun_Timer = 2700000;
                        debug_log("SD2: Instance Stratholme: Baron run in progress.");
                        break;
                    case FAIL:
                        //may add code to remove aura from players, but in theory the time should be up already and removed.
                        break;
                    case DONE:
                        if (Creature* pYsidaT = instance->GetCreature(m_uiYsidaTriggerGUID))
                            pYsidaT->SummonCreature(NPC_YSIDA,
                            pYsidaT->GetPositionX(),pYsidaT->GetPositionY(),pYsidaT->GetPositionZ(),pYsidaT->GetOrientation(),
                            TEMPSUMMON_TIMED_DESPAWN,1800000);

                        m_uiBaronRun_Timer = 0;
                        break;
                }
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_BARONESS:
                m_auiEncounter[1] = uiData;
                if (uiData == IN_PROGRESS)
                    UpdateGoState(m_uiZiggurat1GUID,GO_STATE_ACTIVE,false);
                if (uiData == IN_PROGRESS)                  //change to DONE when crystals implemented
                    StartSlaugtherSquare();
                break;
            case TYPE_NERUB:
                m_auiEncounter[2] = uiData;
                if (uiData == IN_PROGRESS)
                    UpdateGoState(m_uiZiggurat2GUID,GO_STATE_ACTIVE,false);
                if (uiData == IN_PROGRESS)                  //change to DONE when crystals implemented
                    StartSlaugtherSquare();
                break;
            case TYPE_PALLID:
                m_auiEncounter[3] = uiData;
                if (uiData == IN_PROGRESS)
                    UpdateGoState(m_uiZiggurat3GUID,GO_STATE_ACTIVE,false);
                if (uiData == IN_PROGRESS)                  //change to DONE when crystals implemented
                    StartSlaugtherSquare();
                break;
            case TYPE_RAMSTEIN:
                if (uiData == IN_PROGRESS)
                {
                    if (m_auiEncounter[4] != IN_PROGRESS)
                        UpdateGoState(m_uiPortGauntletGUID,GO_STATE_READY,false);

                    uint32 uiCount = abomnationGUID.size();
                    for(std::set<uint64>::iterator i = abomnationGUID.begin(); i != abomnationGUID.end(); ++i)
                    {
                        if (Creature* pAbom = instance->GetCreature(*i))
                        {
                            if (!pAbom->isAlive())
                                --uiCount;
                        }
                    }

                    if (!uiCount)
                    {
                        //a bit itchy, it should close the door after 10 secs, but it doesn't. skipping it for now.
                        //UpdateGoState(ziggurat4GUID,0,true);

                        if (Creature* pBaron = instance->GetCreature(m_uiBaronGUID))
                            pBaron->SummonCreature(NPC_RAMSTEIN,4032.84,-3390.24,119.73,4.71,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1800000);

                        debug_log("SD2: Instance Stratholme: Ramstein spawned.");
                    }
                    else
                        debug_log("SD2: Instance Stratholme: %u Abomnation left to kill.", uiCount);
                }
                if (uiData == DONE)
                {
                    m_uiSlaugtherSquare_Timer = 300000;
                    debug_log("SD2: Instance Stratholme: Slaugther event will continue in 5 minutes.");
                }
                m_auiEncounter[4] = uiData;
                break;
            case TYPE_BARON:
                if (uiData == IN_PROGRESS)
                {
                    if (GetData(TYPE_BARON_RUN) == IN_PROGRESS)
                    {
                        Map::PlayerList const& players = instance->GetPlayers();

                        if (!players.isEmpty())
                        {
                            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            {
                                if (Player* pPlayer = itr->getSource())
                                {
                                    if (pPlayer->HasAura(SPELL_BARON_ULTIMATUM))
                                        pPlayer->RemoveAurasDueToSpell(SPELL_BARON_ULTIMATUM);

                                    if (pPlayer->GetQuestStatus(QUEST_DEAD_MAN_PLEA) == QUEST_STATUS_INCOMPLETE)
                                        pPlayer->AreaExploredOrEventHappens(QUEST_DEAD_MAN_PLEA);
                                }
                            }
                        }

                        SetData(TYPE_BARON_RUN,DONE);
                    }
                }
                m_auiEncounter[5] = uiData;
                break;

            case TYPE_SH_AELMAR:
                IsSilverHandDead[0] = (uiData) ? true : false;
                break;
            case TYPE_SH_CATHELA:
                IsSilverHandDead[1] = (uiData) ? true : false;
                break;
            case TYPE_SH_GREGOR:
                IsSilverHandDead[2] = (uiData) ? true : false;
                break;
            case TYPE_SH_NEMAS:
                IsSilverHandDead[3] = (uiData) ? true : false;
                break;
            case TYPE_SH_VICAR:
                IsSilverHandDead[4] = (uiData) ? true : false;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_SH_QUEST:
                if (IsSilverHandDead[0] && IsSilverHandDead[1] && IsSilverHandDead[2] && IsSilverHandDead[3] && IsSilverHandDead[4])
                    return 1;
                return 0;
            case TYPE_BARON_RUN:
                return m_auiEncounter[0];
            case TYPE_BARONESS:
                return m_auiEncounter[1];
            case TYPE_NERUB:
                return m_auiEncounter[2];
            case TYPE_PALLID:
                return m_auiEncounter[3];
            case TYPE_RAMSTEIN:
                return m_auiEncounter[4];
            case TYPE_BARON:
                return m_auiEncounter[5];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_BARON:
                return m_uiBaronGUID;
            case DATA_YSIDA_TRIGGER:
                return m_uiYsidaTriggerGUID;
        }
        return 0;
    }

    void Update(uint32 uiDiff)
    {
        if (m_uiBaronRun_Timer)
        {
            if (m_uiBaronRun_Timer <= uiDiff)
            {
                if (GetData(TYPE_BARON_RUN) != DONE)
                    SetData(TYPE_BARON_RUN, FAIL);

                m_uiBaronRun_Timer = 0;
                debug_log("SD2: Instance Stratholme: Baron run event reached end. Event has state %u.",GetData(TYPE_BARON_RUN));
            }
            else
                m_uiBaronRun_Timer -= uiDiff;
        }

        if (m_uiSlaugtherSquare_Timer)
        {
            if (m_uiSlaugtherSquare_Timer <= uiDiff)
            {
                if (Creature* pBaron = instance->GetCreature(m_uiBaronGUID))
                {
                    for(uint8 i = 0; i < 4; ++i)
                        pBaron->SummonCreature(NPC_BLACK_GUARD,4032.84,-3390.24,119.73,4.71,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1800000);

                    UpdateGoState(m_uiZiggurat4GUID,GO_STATE_ACTIVE,false);
                    UpdateGoState(m_uiZiggurat5GUID,GO_STATE_ACTIVE,false);

                    debug_log("SD2: Instance Stratholme: Black guard sentries spawned. Opening gates to baron.");
                }
                m_uiSlaugtherSquare_Timer = 0;
            }
            else
                m_uiSlaugtherSquare_Timer -= uiDiff;
        }
    }
};

InstanceData* GetInstanceData_instance_stratholme(Map* pMap)
{
    return new instance_stratholme(pMap);
}

void AddSC_instance_stratholme()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_stratholme";
    newscript->GetInstanceData = &GetInstanceData_instance_stratholme;
    newscript->RegisterSelf();
}
