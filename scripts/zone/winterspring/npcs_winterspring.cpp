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
SDName: Npcs_Winterspring
SD%Complete: 95
SDComment: Quest support: 5126 (Loraxs' tale missing proper gossip items text). Vendor Rivern Frostwind
SDCategory: Winterspring
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## npc_lorax
######*/

bool GossipHello_npc_lorax(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(5126) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "Talk to me", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_lorax(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, "What do you do here?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(3759, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "I can help you", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(3760, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, "What deal?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(3761, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM( 0, "Then what happened?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->SEND_GOSSIP_MENU(3762, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM( 0, "He is not safe, i'll make sure of that.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(3763, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->PlayerTalkClass->CloseGossip();
            player->CompleteQuest(5126);
            break;
    }
    return true;
}

/*######
## npc_rivern_frostwind
######*/

bool GossipHello_npc_rivern_frostwind(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (_Creature->isVendor() && player->GetReputationRank(589) == REP_EXALTED)
        player->ADD_GOSSIP_ITEM( 1, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_rivern_frostwind(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_TRADE)
    {
        player->SEND_VENDORLIST( _Creature->GetGUID() );
    }
    return true;
}

void AddSC_npcs_winterspring()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_lorax";
    newscript->pGossipHello =  &GossipHello_npc_lorax;
    newscript->pGossipSelect = &GossipSelect_npc_lorax;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_rivern_frostwind";
    newscript->pGossipHello =  &GossipHello_npc_rivern_frostwind;
    newscript->pGossipSelect = &GossipSelect_npc_rivern_frostwind;
    m_scripts[nrscripts++] = newscript;
}
