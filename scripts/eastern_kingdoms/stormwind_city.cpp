/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Stormwind_City
SD%Complete: 100
SDComment: Quest support: 1640, 1447, 4185, 6402.
SDCategory: Stormwind City
EndScriptData */

/* ContentData
npc_bartleby
npc_dashel_stonefist
npc_lady_katrana_prestor
npc_squire_rowe
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

/*######
## npc_bartleby
######*/

enum
{
    FACTION_ENEMY       = 168,
    QUEST_BEAT          = 1640
};

struct MANGOS_DLL_DECL npc_bartlebyAI : public ScriptedAI
{
    npc_bartlebyAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiNormalFaction = pCreature->getFaction();
        Reset();
    }

    uint32 m_uiNormalFaction;

    void Reset()
    {
        if (m_creature->getFaction() != m_uiNormalFaction)
            m_creature->setFaction(m_uiNormalFaction);
    }

    void AttackedBy(Unit* pAttacker)
    {
        if (m_creature->getVictim())
            return;

        if (m_creature->IsFriendlyTo(pAttacker))
            return;

        AttackStart(pAttacker);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth() || ((m_creature->GetHealth() - uiDamage)*100 / m_creature->GetMaxHealth() < 15))
        {
            uiDamage = 0;

            if (pDoneBy->GetTypeId() == TYPEID_PLAYER)
                ((Player*)pDoneBy)->AreaExploredOrEventHappens(QUEST_BEAT);

            EnterEvadeMode();
        }
    }
};

bool QuestAccept_npc_bartleby(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_BEAT)
    {
        pCreature->setFaction(FACTION_ENEMY);
        pCreature->AI()->AttackStart(pPlayer);
    }
    return true;
}

CreatureAI* GetAI_npc_bartleby(Creature* pCreature)
{
    return new npc_bartlebyAI(pCreature);
}

/*######
## npc_dashel_stonefist
######*/

enum
{
    QUEST_MISSING_DIPLO_PT8     = 1447,
    FACTION_HOSTILE             = 168
};

struct MANGOS_DLL_DECL npc_dashel_stonefistAI : public ScriptedAI
{
    npc_dashel_stonefistAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiNormalFaction = pCreature->getFaction();
        Reset();
    }

    uint32 m_uiNormalFaction;

    void Reset()
    {
        if (m_creature->getFaction() != m_uiNormalFaction)
            m_creature->setFaction(m_uiNormalFaction);
    }

    void AttackedBy(Unit* pAttacker)
    {
        if (m_creature->getVictim())
            return;

        if (m_creature->IsFriendlyTo(pAttacker))
            return;

        AttackStart(pAttacker);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth() || ((m_creature->GetHealth() - uiDamage)*100 / m_creature->GetMaxHealth() < 15))
        {
            uiDamage = 0;

            if (pDoneBy->GetTypeId() == TYPEID_PLAYER)
                ((Player*)pDoneBy)->AreaExploredOrEventHappens(QUEST_MISSING_DIPLO_PT8);

            EnterEvadeMode();
        }
    }
};

bool QuestAccept_npc_dashel_stonefist(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_MISSING_DIPLO_PT8)
    {
        pCreature->setFaction(FACTION_HOSTILE);
        pCreature->AI()->AttackStart(pPlayer);
    }
    return true;
}

CreatureAI* GetAI_npc_dashel_stonefist(Creature* pCreature)
{
    return new npc_dashel_stonefistAI(pCreature);
}

/*######
## npc_lady_katrana_prestor
######*/

#define GOSSIP_ITEM_KAT_1 "Pardon the intrusion, Lady Prestor, but Highlord Bolvar suggested that I seek your advice."
#define GOSSIP_ITEM_KAT_2 "My apologies, Lady Prestor."
#define GOSSIP_ITEM_KAT_3 "Begging your pardon, Lady Prestor. That was not my intent."
#define GOSSIP_ITEM_KAT_4 "Thank you for your time, Lady Prestor."

bool GossipHello_npc_lady_katrana_prestor(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(4185) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KAT_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(2693, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_lady_katrana_prestor(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KAT_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(2694, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KAT_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(2695, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KAT_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(2696, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(4185);
            break;
    }
    return true;
}

/*######
## npc_squire_rowe
######*/

enum
{
    SAY_SIGNAL_SENT             = -1000822,
    SAY_DISMOUNT                = -1000823,
    SAY_WELCOME                 = -1000824,

    GOSSIP_ITEM_WINDSOR         = -3000106,

    GOSSIP_TEXT_ID_DEFAULT      = 9063,
    GOSSIP_TEXT_ID_PROGRESS     = 9064,
    GOSSIP_TEXT_ID_START        = 9065,

    NPC_WINDSOR                 = 12580,
    NPC_WINDSOR_MOUNT           = 12581,

    QUEST_STORMWIND_RENDEZVOUS  = 6402,
    QUEST_THE_GREAT_MASQUERADE  = 6403,
};

static const DialogueEntry aIntroDialogue[] =
{
    {NPC_WINDSOR,                0,           3000},        // wait
    {NPC_WINDSOR_MOUNT,          0,           1000},        // summon horse
    {SAY_DISMOUNT,               NPC_WINDSOR, 2000},
    {QUEST_STORMWIND_RENDEZVOUS, 0,           2000},        // face player
    {QUEST_THE_GREAT_MASQUERADE, 0,           0},           // say intro to player
    {0, 0, 0},
};

static const float aWindsorSpawnLoc[3] = {-9145.68f, 373.79f, 90.64f};
static const float aWindsorMoveLoc[3] = {-9050.39f, 443.55f, 93.05f};

struct MANGOS_DLL_DECL npc_squire_roweAI : public npc_escortAI, private DialogueHelper
{
    npc_squire_roweAI(Creature* m_creature) : npc_escortAI(m_creature),
        DialogueHelper(aIntroDialogue)
    {
        m_bIsEventInProgress = false;
        Reset();
    }

    bool m_bIsEventInProgress;

    ObjectGuid m_windsorGuid;
    ObjectGuid m_horseGuid;

    void Reset() { }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_WINDSOR)
        {
            pSummoned->SetWalk(false);
            pSummoned->GetMotionMaster()->MovePoint(1, aWindsorMoveLoc[0], aWindsorMoveLoc[1], aWindsorMoveLoc[2]);

            m_windsorGuid = pSummoned->GetObjectGuid();
            m_bIsEventInProgress = true;
        }
        else if (pSummoned->GetEntry() == NPC_WINDSOR_MOUNT)
            m_horseGuid = pSummoned->GetObjectGuid();
    }

    void SummonedCreatureDespawn(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_WINDSOR)
        {
            m_windsorGuid.Clear();
            m_bIsEventInProgress = false;
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE || !uiPointId || pSummoned->GetEntry() != NPC_WINDSOR)
            return;

        if (uiPointId)
            StartNextDialogueText(NPC_WINDSOR);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 2:
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                break;
            case 3:
                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                m_creature->SummonCreature(NPC_WINDSOR, aWindsorSpawnLoc[0], aWindsorSpawnLoc[1], aWindsorSpawnLoc[2], 0, TEMPSUMMON_DEAD_DESPAWN, 0);
                break;
            case 6:
                DoScriptText(SAY_SIGNAL_SENT, m_creature);
                m_creature->SetFacingTo(2.15f);
                SetEscortPaused(true);
                break;
        }
    }

    void JustDidDialogueStep(int32 iEntry)
    {
        switch(iEntry)
        {
            case NPC_WINDSOR_MOUNT:
            {
                if (Creature* pWindsor = m_creature->GetMap()->GetCreature(m_windsorGuid))
                {
                    pWindsor->Unmount();
                    m_creature->SummonCreature(NPC_WINDSOR_MOUNT, pWindsor->GetPositionX() - 1.0f, pWindsor->GetPositionY() + 1.0f, pWindsor->GetPositionZ(), pWindsor->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 30000);
                }
                break;
            }
            case SAY_DISMOUNT:
            {
                if (Creature* pHorse = m_creature->GetMap()->GetCreature(m_horseGuid))
                {
                    pHorse->SetWalk(false);
                    pHorse->GetMotionMaster()->MovePoint(1, aWindsorSpawnLoc[0], aWindsorSpawnLoc[1], aWindsorSpawnLoc[2]);
                }
                break;
            }
            case QUEST_STORMWIND_RENDEZVOUS:
            {
                Creature* pWindsor = m_creature->GetMap()->GetCreature(m_windsorGuid);
                Player* pPlayer = GetPlayerForEscort();
                if (!pWindsor || !pPlayer)
                    break;

                pWindsor->SetFacingToObject(pPlayer);
                break;
            }
            case QUEST_THE_GREAT_MASQUERADE:
            {
                Creature* pWindsor = m_creature->GetMap()->GetCreature(m_windsorGuid);
                Player* pPlayer = GetPlayerForEscort();
                if (!pWindsor || !pPlayer)
                    break;

                DoScriptText(SAY_WELCOME, pWindsor, pPlayer);
                // Allow players to finish quest and also finish the escort
                pWindsor->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                SetEscortPaused(false);
                break;
            }
        }
    }

    Creature* GetSpeakerByEntry(uint32 uiEntry)
    {
        if (uiEntry == NPC_WINDSOR)
            return m_creature->GetMap()->GetCreature(m_windsorGuid);

        return NULL;
    }

    // Check if the event is already running
    bool IsStormwindQuestActive() { return m_bIsEventInProgress; }

    void UpdateEscortAI(const uint32 uiDiff) { DialogueUpdate(uiDiff); }
};

CreatureAI* GetAI_npc_squire_rowe(Creature* pCreature)
{
    return new npc_squire_roweAI(pCreature);
}

bool GossipHello_npc_squire_rowe(Player* pPlayer, Creature* pCreature)
{
    // Allow gossip if quest 6402 is completed but not yet rewarded or 6402 is rewarded but 6403 isn't yet completed
    if ((pPlayer->GetQuestStatus(QUEST_STORMWIND_RENDEZVOUS) == QUEST_STATUS_COMPLETE && !pPlayer->GetQuestRewardStatus(QUEST_STORMWIND_RENDEZVOUS)) ||
        (pPlayer->GetQuestRewardStatus(QUEST_STORMWIND_RENDEZVOUS) && pPlayer->GetQuestStatus(QUEST_THE_GREAT_MASQUERADE) != QUEST_STATUS_COMPLETE))
    {
        bool bIsEventInProgress = true;

        // Check if event is already in progress
        if (npc_squire_roweAI* pRoweAI = dynamic_cast<npc_squire_roweAI*>(pCreature->AI()))
            bIsEventInProgress = pRoweAI->IsStormwindQuestActive();

        // If event is already in progress, then inform the player to wait
        if (bIsEventInProgress)
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_PROGRESS, pCreature->GetObjectGuid());
        else
        {
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_WINDSOR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_START, pCreature->GetObjectGuid());
        }
    }
    else
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_DEFAULT, pCreature->GetObjectGuid());

    return true;
 }

bool GossipSelect_npc_squire_rowe(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if (npc_squire_roweAI* pRoweAI = dynamic_cast<npc_squire_roweAI*>(pCreature->AI()))
            pRoweAI->Start(true, pPlayer, 0, true, false);

        pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

void AddSC_stormwind_city()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_bartleby";
    pNewScript->GetAI = &GetAI_npc_bartleby;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_bartleby;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_dashel_stonefist";
    pNewScript->GetAI = &GetAI_npc_dashel_stonefist;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_dashel_stonefist;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_lady_katrana_prestor";
    pNewScript->pGossipHello = &GossipHello_npc_lady_katrana_prestor;
    pNewScript->pGossipSelect = &GossipSelect_npc_lady_katrana_prestor;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_squire_rowe";
    pNewScript->GetAI = &GetAI_npc_squire_rowe;
    pNewScript->pGossipHello =  &GossipHello_npc_squire_rowe;
    pNewScript->pGossipSelect = &GossipSelect_npc_squire_rowe;
    pNewScript->RegisterSelf();
}
