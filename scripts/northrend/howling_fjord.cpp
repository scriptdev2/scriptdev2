/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Howling_Fjord
SD%Complete: ?
SDComment: Quest support: 11221, 11483, 11300
SDCategory: Howling Fjord
EndScriptData */

/* ContentData
npc_daegarn
npc_deathstalker_razael
npc_dark_ranger_lyana
npc_mcgoyver
EndContentData */

#include "precompiled.h"

/*######
## npc_daegarn
######*/

enum
{
    QUEST_DEFEAT_AT_RING            = 11300,

    NPC_FIRJUS                      = 24213,
    NPC_JLARBORN                    = 24215,
    NPC_YOROS                       = 24214,
    NPC_OLUF                        = 23931,

    NPC_PRISONER_1                  = 24253,                // looks the same but has different abilities
    NPC_PRISONER_2                  = 24254,
    NPC_PRISONER_3                  = 24255,
};

static float afSummon[] = {838.81f, -4678.06f, -94.182f};
static float afCenter[] = {801.88f, -4721.87f, -96.143f};

// TODO: make prisoners help (unclear if summoned or using npc's from surrounding cages (summon inside small cages?))
struct MANGOS_DLL_DECL npc_daegarnAI : public ScriptedAI
{
    npc_daegarnAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool m_bEventInProgress;
    uint64 m_uiPlayerGUID;

    void Reset()
    {
        m_bEventInProgress = false;
        m_uiPlayerGUID = 0;
    }

    void StartEvent(Player* pPlayer)
    {
        if (m_bEventInProgress)
            return;

        m_uiPlayerGUID = pPlayer->GetGUID();

        SummonGladiator(NPC_FIRJUS);
    }

    void JustSummoned(Creature* pSummon)
    {
        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
        {
            if (pPlayer->isAlive())
            {
                pSummon->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pSummon->GetMotionMaster()->MovePoint(0, afCenter[0], afCenter[1], afCenter[2]);
                return;
            }
        }

        Reset();
    }

    void SummonGladiator(uint32 uiEntry)
    {
        m_creature->SummonCreature(uiEntry, afSummon[0], afSummon[1], afSummon[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30*IN_MILLISECONDS);
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID);

        // could be group, so need additional here.
        if (!pPlayer || !pPlayer->isAlive())
        {
            Reset();
            return;
        }

        if (pSummoned->IsWithinDistInMap(pPlayer, 75.0f))   // ~the radius of the ring
            pSummoned->AI()->AttackStart(pPlayer);
    }

    void SummonedCreatureDespawn(Creature* pSummoned)
    {
        uint32 uiEntry = 0;

        // will eventually reset the event if something goes wrong
        switch(pSummoned->GetEntry())
        {
            case NPC_FIRJUS:    uiEntry = NPC_JLARBORN; break;
            case NPC_JLARBORN:  uiEntry = NPC_YOROS;    break;
            case NPC_YOROS:     uiEntry = NPC_OLUF;     break;
            case NPC_OLUF:      Reset();                return;
        }

        SummonGladiator(uiEntry);
    }
};

bool QuestAccept_npc_daegarn(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_DEFEAT_AT_RING)
    {
        if (npc_daegarnAI* pDaegarnAI = dynamic_cast<npc_daegarnAI*>(pCreature->AI()))
            pDaegarnAI->StartEvent(pPlayer);
    }

    return true;
}

CreatureAI* GetAI_npc_daegarn(Creature* pCreature)
{
    return new npc_daegarnAI(pCreature);
}

/*#######################
## Deathstalker Razael ##
#######################*/

#define GOSSIP_ITEM_DEATHSTALKER_RAZAEL "High Executor Anselm requests your report."

enum
{
    QUEST_REPORTS_FROM_THE_FIELD       = 11221,
    SPELL_RAZAEL_KILL_CREDIT           = 42756,
    GOSSIP_TEXTID_DEATHSTALKER_RAZAEL1 = 11562,
    GOSSIP_TEXTID_DEATHSTALKER_RAZAEL2 = 11564
};

bool GossipHello_npc_deathstalker_razael(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_REPORTS_FROM_THE_FIELD) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEATHSTALKER_RAZAEL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEATHSTALKER_RAZAEL1, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_deathstalker_razael(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEATHSTALKER_RAZAEL2, pCreature->GetGUID());
            pCreature->CastSpell(pPlayer, SPELL_RAZAEL_KILL_CREDIT, true);
            break;
    }

    return true;
}

/*#####################
## Dark Ranger Lyana ##
#####################*/

#define GOSSIP_ITEM_DARK_RANGER_LYANA "High Executor Anselm requests your report."

enum
{
    GOSSIP_TEXTID_DARK_RANGER_LYANA1    = 11586,
    GOSSIP_TEXTID_DARK_RANGER_LYANA2    = 11588,
    SPELL_DARK_RANGER_LYANA_KILL_CREDIT = 42799
};

bool GossipHello_npc_dark_ranger_lyana(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_REPORTS_FROM_THE_FIELD) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DARK_RANGER_LYANA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DARK_RANGER_LYANA1, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_dark_ranger_lyana(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DARK_RANGER_LYANA2, pCreature->GetGUID());
            pCreature->CastSpell(pPlayer, SPELL_DARK_RANGER_LYANA_KILL_CREDIT, true);
            break;
    }

    return true;
}

/*############
## McGoyver ##
############*/

#define GOSSIP_ITEM_MCGOYVER1 "Walt sent me to pick up some dark iron ingots."
#define GOSSIP_ITEM_MCGOYVER2 "Yarp."

enum
{
    QUEST_WE_CAN_REBUILD_IT                = 11483,
    GOSSIP_TEXTID_MCGOYVER                 = 12193,
    ITEM_DARK_IRON_INGOTS                  = 34135,
    SPELL_MCGOYVER_TAXI_EXPLORERSLEAGUE    = 44280,
    SPELL_MCGOYVER_CREATE_DARK_IRON_INGOTS = 44512
};

bool GossipHello_npc_mcgoyver(Player* pPlayer, Creature* pCreature)
{
    switch(pPlayer->GetQuestStatus(QUEST_WE_CAN_REBUILD_IT))
    {
        case QUEST_STATUS_INCOMPLETE:
            if (!pPlayer->HasItemCount(ITEM_DARK_IRON_INGOTS, 1, true))
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_MCGOYVER1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            }
            else
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_MCGOYVER2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_MCGOYVER, pCreature->GetGUID());
            }
            break;
        case QUEST_STATUS_COMPLETE:
            if (!pPlayer->GetQuestRewardStatus(QUEST_WE_CAN_REBUILD_IT))
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_MCGOYVER2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_MCGOYVER, pCreature->GetGUID());
                break;
            }
        default:
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    }

    return true;
}

bool GossipSelect_npc_mcgoyver(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pCreature->CastSpell(pPlayer, SPELL_MCGOYVER_CREATE_DARK_IRON_INGOTS, true);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_MCGOYVER2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_MCGOYVER, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer, SPELL_MCGOYVER_TAXI_EXPLORERSLEAGUE, true);
            break;
    }

    return true;
}

void AddSC_howling_fjord()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_daegarn";
    newscript->GetAI = &GetAI_npc_daegarn;
    newscript->pQuestAccept = &QuestAccept_npc_daegarn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_deathstalker_razael";
    newscript->pGossipHello = &GossipHello_npc_deathstalker_razael;
    newscript->pGossipSelect = &GossipSelect_npc_deathstalker_razael;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_dark_ranger_lyana";
    newscript->pGossipHello = &GossipHello_npc_dark_ranger_lyana;
    newscript->pGossipSelect = &GossipSelect_npc_dark_ranger_lyana;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_mcgoyver";
    newscript->pGossipHello = &GossipHello_npc_mcgoyver;
    newscript->pGossipSelect = &GossipSelect_npc_mcgoyver;
    newscript->RegisterSelf();
}
