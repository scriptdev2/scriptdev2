/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: npcs_blackrock_depths
SD%Complete: 95
SDComment: npcs inside blackrock depths, mostly quest related, includes creation of thorium brotherhood contract
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## npc_kharan_mighthammer
######*/

bool GossipHello_npc_kharan_mighthammer(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(4342) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "All is not lost, Kharan!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_kharan_mighthammer(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "Continue...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(2476, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, "So what happened?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(2477, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM( 0, "Continue...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->SEND_GOSSIP_MENU(2478, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM( 0, "So you suspect that someone on the inside was involved? That they were tipped off?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(2479, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->ADD_GOSSIP_ITEM( 0, "Continue with your story please.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->SEND_GOSSIP_MENU(2480, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            player->ADD_GOSSIP_ITEM( 0, "Indeed.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->SEND_GOSSIP_MENU(2481, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            player->ADD_GOSSIP_ITEM( 0, "The door is open, Kharan. You are a free man.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->SEND_GOSSIP_MENU(2482, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+8:
            player->PlayerTalkClass->CloseGossip();
            player->CompleteQuest(4342);
            break;
    }
    return true;
}

/*######
## npc_lokhtos_darkbargainer
######*/

#define ITEM_THRORIUM_BROTHERHOOD_CONTRACT               18628
#define ITEM_SULFURON_INGOT                              17203
#define QUEST_A_BINDING_CONTRACT                         7604
#define SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT_DND    23059

bool GossipHello_npc_lokhtos_darkbargainer(Player *player, Creature *_Creature)
{
    if (player->GetQuestRewardStatus(QUEST_A_BINDING_CONTRACT) != 1 &&
            !player->HasItemCount(ITEM_THRORIUM_BROTHERHOOD_CONTRACT, 1) &&
            player->GetBankItemCount(ITEM_THRORIUM_BROTHERHOOD_CONTRACT) < 1 &&
            player->HasItemCount(ITEM_SULFURON_INGOT, 1))
    {
        player->ADD_GOSSIP_ITEM(7, "Get Thorium Brotherhood Contract",
            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(384,_Creature->GetGUID());
        return true;
    }
    return false;
}

bool GossipSelect_npc_lokhtos_darkbargainer(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
        player->CastSpell(player, SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT_DND, false);
    }

    return true;
}

/*######
## 
######*/

void AddSC_npcs_blackrock_depths()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_kharan_mighthammer";
    newscript->pGossipHello = &GossipHello_npc_kharan_mighthammer;
    newscript->pGossipSelect = &GossipSelect_npc_kharan_mighthammer;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_lokhtos_darkbargainer";
    newscript->pGossipHello          = &GossipHello_npc_lokhtos_darkbargainer;
    newscript->pGossipSelect         = &GossipSelect_npc_lokhtos_darkbargainer;
    m_scripts[nrscripts++] = newscript;
}
