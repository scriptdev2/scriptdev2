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
SDName: Ghostlands
SD%Complete: 100
SDComment: Quest support: 9212, 9692. Obtain Budd's Guise of Zul'aman. Vendor Rathis Tomber
SDCategory: Ghostlands
EndScriptData */

/* ContentData
npc_blood_knight_dawnstar
npc_budd_nedreck
npc_ranger_lilatha
npc_rathis_tomber
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*######
## npc_blood_knight_dawnstar
######*/

#define GOSSIP_ITEM_INSIGNIA    "Take Blood Knight Insignia"

bool GossipHello_npc_blood_knight_dawnstar(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(9692) == QUEST_STATUS_INCOMPLETE && !pPlayer->HasItemCount(24226,1,true))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,GOSSIP_ITEM_INSIGNIA,GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_blood_knight_dawnstar(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        ItemPosCountVec dest;
        uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 24226, 1, false);
        if (msg == EQUIP_ERR_OK)
        {
            pPlayer->StoreNewItem(dest, 24226, 1, true);
            pPlayer->PlayerTalkClass->ClearMenus();
        }
    }
    return true;
}

/*######
## npc_budd_nedreck
######*/

#define GOSSIP_ITEM_DISGUISE        "You gave the crew disguises?"

bool GossipHello_npc_budd_nedreck(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(11166) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,GOSSIP_ITEM_DISGUISE,GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_budd_nedreck(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action==GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->CastSpell(pPlayer, 42540, false);
    }
    return true;
}

/*######
## npc_ranger_lilatha
######*/

enum
{
    SAY_START           = -1000140,
    SAY_PROGRESS1       = -1000141,
    SAY_PROGRESS2       = -1000142,
    SAY_PROGRESS3       = -1000143,
    SAY_END1            = -1000144,
    SAY_END2            = -1000145,
    CAPTAIN_ANSWER      = -1000146,

    QUEST_CATACOMBS     = 9212,
    GO_CAGE             = 181152,
    NPC_CAPTAIN_HELIOS  = 16220,
    FACTION_SMOON_E     = 1603,
};

struct MANGOS_DLL_DECL npc_ranger_lilathaAI : public npc_escortAI
{
    npc_ranger_lilathaAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_uiNormFaction = pCreature->getFaction();
        m_uiGoCageGUID = 0;
        m_uiHeliosGUID = 0;
        Reset();
    }

    uint32 m_uiNormFaction;
    uint64 m_uiGoCageGUID;
    uint64 m_uiHeliosGUID;

    void MoveInLineOfSight(Unit* pUnit)
    {
        if (IsBeingEscorted)
        {
            if (!m_uiHeliosGUID && pUnit->GetEntry() == NPC_CAPTAIN_HELIOS)
            {
                if (m_creature->IsWithinDistInMap(pUnit, 30.0f))
                    m_uiHeliosGUID = pUnit->GetGUID();
            }
        }

        npc_escortAI::MoveInLineOfSight(pUnit);
    }

    void WaypointReached(uint32 i)
    {
        Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID);

        if (!pPlayer || pPlayer->GetTypeId() != TYPEID_PLAYER)
            return;

        switch(i)
        {
            case 0:
                if (GameObject* pGoTemp = GetClosestGameObjectWithEntry(m_creature, GO_CAGE, 10.0f))
                {
                    m_uiGoCageGUID = pGoTemp->GetGUID();
                    pGoTemp->SetGoState(GO_STATE_ACTIVE);
                }

                m_creature->SetStandState(UNIT_STAND_STATE_STAND);

                DoScriptText(SAY_START, m_creature, pPlayer);
                break;
            case 1:
                if (GameObject* pGo = m_creature->GetMap()->GetGameObject(m_uiGoCageGUID))
                    pGo->SetGoState(GO_STATE_READY);
                break;
            case 5:
                DoScriptText(SAY_PROGRESS1, m_creature, pPlayer);
                break;
            case 11:
                DoScriptText(SAY_PROGRESS2, m_creature, pPlayer);
                break;
            case 18:
                DoScriptText(SAY_PROGRESS3, m_creature, pPlayer);
                if (Creature* pSum1 = m_creature->SummonCreature(16342, 7627.083984, -7532.538086, 152.128616, 1.082733, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pSum1->AI()->AttackStart(m_creature);
                if (Creature* pSum2 = m_creature->SummonCreature(16343, 7620.432129, -7532.550293, 152.454865, 0.827478, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pSum2->AI()->AttackStart(pPlayer);
                break;
            case 19:
                SetRun();
                break;
            case 25:
                SetRun(false);
                break;
            case 30:
                ((Player*)pPlayer)->GroupEventHappens(QUEST_CATACOMBS,m_creature);
                break;
            case 32:
                DoScriptText(SAY_END1, m_creature, pPlayer);
                break;
            case 33:
                DoScriptText(SAY_END2, m_creature, pPlayer);
                if (Unit* pHelios = Unit::GetUnit(*m_creature, m_uiHeliosGUID))
                    DoScriptText(CAPTAIN_ANSWER, pHelios, m_creature);
                break;
        }
    }

    void Reset()
    {
        if (!IsBeingEscorted)
        {
            m_creature->setFaction(m_uiNormFaction);
            m_uiGoCageGUID = 0;
            m_uiHeliosGUID = 0;
        }
    }

    void JustDied(Unit* killer)
    {
        if (IsBeingEscorted && PlayerGUID)
        {
            if (Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID))
                ((Player*)pPlayer)->FailQuest(QUEST_CATACOMBS);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_ranger_lilathaAI(Creature* pCreature)
{
    npc_ranger_lilathaAI* ranger_lilathaAI = new npc_ranger_lilathaAI(pCreature);

    ranger_lilathaAI->FillPointMovementListForCreature();

    return (CreatureAI*)ranger_lilathaAI;
}

bool QuestAccept_npc_ranger_lilatha(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_CATACOMBS)
    {
        pCreature->setFaction(FACTION_SMOON_E);
        ((npc_escortAI*)(pCreature->AI()))->Start(true, true, false, pPlayer->GetGUID());
    }
    return true;
}

/*######
## npc_rathis_tomber
######*/

bool GossipHello_npc_rathis_tomber(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pCreature->isVendor() && pPlayer->GetQuestRewardStatus(9152))
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        pPlayer->SEND_GOSSIP_MENU(8432, pCreature->GetGUID());
    }else
        pPlayer->SEND_GOSSIP_MENU(8431, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_rathis_tomber(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetGUID());

    return true;
}

void AddSC_ghostlands()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_blood_knight_dawnstar";
    newscript->pGossipHello = &GossipHello_npc_blood_knight_dawnstar;
    newscript->pGossipSelect = &GossipSelect_npc_blood_knight_dawnstar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_budd_nedreck";
    newscript->pGossipHello = &GossipHello_npc_budd_nedreck;
    newscript->pGossipSelect = &GossipSelect_npc_budd_nedreck;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_ranger_lilatha";
    newscript->GetAI = &GetAI_npc_ranger_lilathaAI;
    newscript->pQuestAccept = &QuestAccept_npc_ranger_lilatha;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_rathis_tomber";
    newscript->pGossipHello = &GossipHello_npc_rathis_tomber;
    newscript->pGossipSelect = &GossipSelect_npc_rathis_tomber;
    newscript->RegisterSelf();
}
