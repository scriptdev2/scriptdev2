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
SDName: npcs_blasted_lands
SD%Complete: 98
SDComment: npcs blasted lands, mostly quest related. Fallen hero of horde is as close as it gets without first hand knowledge.
SDCategory: Blasted Lands
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## npc_fallen_hero_of_horde
######*/

#define GOSSIP_ITEM_FALLEN "Continue..."

bool GossipHello_npc_fallen_hero_of_horde(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(2784) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "Why are you here?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    if (player->GetQuestStatus(2801) == QUEST_STATUS_INCOMPLETE && player->GetTeam() == HORDE)
        player->ADD_GOSSIP_ITEM( 0, "Continue story...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    if (player->GetQuestStatus(2801) == QUEST_STATUS_INCOMPLETE && player->GetTeam() == ALLIANCE)
        player->ADD_GOSSIP_ITEM( 0, "Why are you here?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_fallen_hero_of_horde(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->SEND_GOSSIP_MENU(1392, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+11:
            player->SEND_GOSSIP_MENU(1411, _Creature->GetGUID());
            if (player->GetQuestStatus(2784) == QUEST_STATUS_INCOMPLETE)
                player->CompleteQuest(2784);
            if (player->GetTeam() == ALLIANCE)
            {
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(1411, _Creature->GetGUID());
            }
            break;

        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
            player->SEND_GOSSIP_MENU(1451, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+21:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
            player->SEND_GOSSIP_MENU(1452, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+22:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23);
            player->SEND_GOSSIP_MENU(1453, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+23:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24);
            player->SEND_GOSSIP_MENU(1454, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+24:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25);
            player->SEND_GOSSIP_MENU(1455, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+25:
            player->SEND_GOSSIP_MENU(1456, _Creature->GetGUID());
            player->CompleteQuest(2801);
            break;
    }
    return true;
}

void AddSC_npcs_blasted_lands()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_fallen_hero_of_horde";
    newscript->pGossipHello =  &GossipHello_npc_fallen_hero_of_horde;
    newscript->pGossipSelect = &GossipSelect_npc_fallen_hero_of_horde;
    m_scripts[nrscripts++] = newscript;
}
