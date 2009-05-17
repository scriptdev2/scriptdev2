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
SDName: Eversong_Woods
SD%Complete: 100
SDComment: Quest support: 8483, 9686
SDCategory: Eversong Woods
EndScriptData */

/* ContentData
npc_kelerun_bloodmourn
go_harbinger_second_trial
npc_prospector_anvilward
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*######
## npc_kelerun_bloodmourn
######*/

enum
{
    NPC_KELERUN                 = 17807,
    GO_SECOND_TRIAL             = 182052,
    QUEST_SECOND_TRIAL          = 9686,
    MAX_CHALLENGER              = 4
};

const uint32 uiChallengerId[4] =
{
    17809,                                                  //Bloodwrath
    17810,                                                  //Lightrend
    17811,                                                  //Swiftblade
    17812                                                   //Sunstriker
};

const int32 uiSayId[4] =
{
    -1000319,
    -1000320,
    -1000321,
    -1000322
};

float fChallengerLoc[4][4]=
{
    {10110.667, -6628.059, 4.100, 2.708},
    {10093.919, -6634.340, 4.098, 1.106},
    {10087.565, -6617.282, 4.098, 5.887},
    {10104.807, -6611.145, 4.101, 4.265}
};

struct MANGOS_DLL_DECL npc_kelerun_bloodmournAI : public ScriptedAI
{
    npc_kelerun_bloodmournAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        uiNpcFlags = pCreature->GetUInt32Value(UNIT_NPC_FLAGS);
        Reset();
    }

    uint32 uiNpcFlags;
    uint64 uiPlayerGUID;
    uint64 uiChallengerGUID[MAX_CHALLENGER];

    uint8 uiChallengerCount;

    uint32 uiTimeOutTimer;
    uint32 uiCheckAliveStateTimer;
    uint32 uiEngageTimer;

    bool bIsEventInProgress;

    void Reset()
    {
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, uiNpcFlags);

        uiPlayerGUID = 0;

        for(uint8 i = 0; i < MAX_CHALLENGER; i++)
            uiChallengerGUID[i] = 0;

        uiChallengerCount = 0;

        uiTimeOutTimer = 60000;
        uiCheckAliveStateTimer = 2500;
        uiEngageTimer = 0;

        bIsEventInProgress = false;
    }

    void StartEvent()
    {
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
        bIsEventInProgress = true;
    }

    bool CanProgressEvent(uint64 uiPlayer)
    {
        if (bIsEventInProgress)
        {
            uiPlayerGUID = uiPlayer;
            DoSpawnChallengers();
            uiEngageTimer = 15000;

            return true;
        }

        return false;
    }

    void DoSpawnChallengers()
    {
        for(uint8 i = 0; i < MAX_CHALLENGER; i++)
        {
            if (Creature* pCreature = m_creature->SummonCreature(uiChallengerId[i],
                fChallengerLoc[i][0], fChallengerLoc[i][1],
                fChallengerLoc[i][2], fChallengerLoc[i][3],
                TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000))
            {
                uiChallengerGUID[i] = pCreature->GetGUID();
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (bIsEventInProgress)
        {
            if (uiTimeOutTimer && uiTimeOutTimer < diff)
            {
                if (!uiPlayerGUID)
                {
                    //player are expected to use GO within a minute, if not, event will fail.
                    Reset();
                    return;
                }

                uiTimeOutTimer = 0;
            }
            else
                uiTimeOutTimer -= diff;

            if (uiCheckAliveStateTimer < diff)
            {
                if (Unit* pChallenger = Unit::GetUnit(*m_creature,uiChallengerGUID[uiChallengerCount]))
                {
                    if (!pChallenger->isAlive())
                    {
                        Player* pPlayer = (Player*)Unit::GetUnit(*m_creature,uiPlayerGUID);

                        if (pPlayer && !pPlayer->isAlive())
                        {
                            Reset();
                            return;
                        }

                        ++uiChallengerCount;

                        //count starts at 0
                        if (uiChallengerCount == MAX_CHALLENGER)
                        {
                            if (pPlayer && pPlayer->isAlive())
                                pPlayer->GroupEventHappens(QUEST_SECOND_TRIAL,m_creature);

                            Reset();
                            return;
                        }
                        else
                            uiEngageTimer = 15000;
                    }
                }
                uiCheckAliveStateTimer = 2500;
            }
            else
                uiCheckAliveStateTimer -= diff;

            if (uiEngageTimer && uiEngageTimer < diff)
            {
                Unit* pPlayer = Unit::GetUnit(*m_creature,uiPlayerGUID);

                if (pPlayer && pPlayer->isAlive())
                {
                    if (Creature* pCreature = (Creature*)Unit::GetUnit(*m_creature,uiChallengerGUID[uiChallengerCount]))
                    {
                        DoScriptText(uiSayId[uiChallengerCount], m_creature, pPlayer);
                        pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pCreature->AI()->AttackStart(pPlayer);
                    }
                }

                uiEngageTimer = 0;
            }
            else
                uiEngageTimer -= diff;
        }
    }
};

CreatureAI* GetAI_npc_kelerun_bloodmourn(Creature* pCreature)
{
    return new npc_kelerun_bloodmournAI(pCreature);
}

//easiest way is to expect database to respawn GO at quest accept (quest_start_script)
bool QuestAccept_npc_kelerun_bloodmourn(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_SECOND_TRIAL)
        ((npc_kelerun_bloodmournAI*)(pCreature->AI()))->StartEvent();

    return true;
}

bool GOHello_go_harbinger_second_trial(Player* pPlayer, GameObject* pGO)
{
    if (pGO->GetGoType() == GAMEOBJECT_TYPE_GOOBER)
    {
        if (Creature* pCreature = GetClosestCreatureWithEntry(pGO, NPC_KELERUN, 30.0f))
        {
            if (((npc_kelerun_bloodmournAI*)(pCreature->AI()))->CanProgressEvent(pPlayer->GetGUID()))
                return false;
        }
    }

    return false;
}

/*######
## npc_prospector_anvilward
######*/

#define SAY_ANVIL1              -1000209
#define SAY_ANVIL2              -1000210

#define QUEST_THE_DWARVEN_SPY   8483

struct MANGOS_DLL_DECL npc_prospector_anvilwardAI : public npc_escortAI
{
    // CreatureAI functions
    npc_prospector_anvilwardAI(Creature* pCreature) : npc_escortAI(pCreature) {Reset();}

    // Pure Virtual Functions
    void WaypointReached(uint32 i)
    {
        Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID);

        if (!pPlayer)
            return;

        switch (i)
        {
            case 0:
                DoScriptText(SAY_ANVIL1, m_creature, pPlayer);
                break;
            case 5:
                DoScriptText(SAY_ANVIL1, m_creature, pPlayer);
                break;
            case 6:
                m_creature->setFaction(24);
                break;
        }
    }

    void Reset()
    {
        //Default npc faction
        m_creature->setFaction(35);
    }

    void JustDied(Unit* killer)
    {
        //Default npc faction
        m_creature->setFaction(35);
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);                       //Must update npc_escortAI

    }
};

CreatureAI* GetAI_npc_prospector_anvilward(Creature* pCreature)
{
    npc_prospector_anvilwardAI* thisAI = new npc_prospector_anvilwardAI(pCreature);

    thisAI->FillPointMovementListForCreature();

    return (CreatureAI*)thisAI;
}

bool GossipHello_npc_prospector_anvilward(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_THE_DWARVEN_SPY) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I need a moment of your time, sir.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(8239, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_prospector_anvilward(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    switch(action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Why... yes, of course. I've something to show you right inside this building, Mr. Anvilward.",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(8240, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            //attack,defend,walk
            ((npc_escortAI*)(pCreature->AI()))->Start(true, true, false, pPlayer->GetGUID());
            break;
    }
    return true;
}

void AddSC_eversong_woods()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name= "npc_kelerun_bloodmourn";
    newscript->GetAI = &GetAI_npc_kelerun_bloodmourn;
    newscript->pQuestAccept = &QuestAccept_npc_kelerun_bloodmourn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_harbinger_second_trial";
    newscript->pGOHello = &GOHello_go_harbinger_second_trial;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name= "npc_prospector_anvilward";
    newscript->GetAI = &GetAI_npc_prospector_anvilward;
    newscript->pGossipHello =  &GossipHello_npc_prospector_anvilward;
    newscript->pGossipSelect = &GossipSelect_npc_prospector_anvilward;
    newscript->RegisterSelf();
}
