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
SDName: npc_mount_vendor
SD%Complete: 80
SDComment: Regular mount vendors all over the world. Display gossip if player doesn't meet the requirements to buy
SDCategory: NPCs
EndScriptData */

#include "../sc_defines.h"
#include "../../../../game/Player.h"
#include "../../../../game/QuestDef.h"
#include "../../../../game/GossipDef.h"

/*######
## npc_mount_vendor
######*/

#define GOSSIP_ITEM_GOODS "I'd like to browse your goods."

bool GossipHello_npc_mount_vendor(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    bool canBuy;
    canBuy = false;
    uint32 vendor = _Creature->GetEntry();
    uint8 race = player->getRace();

    switch (vendor)
    {
        case 384: //Katie Hunter
            if (player->GetReputationRank(72) != REP_EXALTED && race != RACE_HUMAN)
                player->SEND_GOSSIP_MENU(5855, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 1261: //Veron Amberstill
            if (player->GetReputationRank(47) != REP_EXALTED && race != RACE_DWARF)
                player->SEND_GOSSIP_MENU(5856, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 1460: //Unger Statforth
            if (player->GetReputationRank(72) != REP_EXALTED && race != RACE_HUMAN)
                player->SEND_GOSSIP_MENU(5855, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 2357: //Merideth Carlson
            if (player->GetReputationRank(72) != REP_EXALTED && race != RACE_HUMAN)
                player->SEND_GOSSIP_MENU(5855, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 3362: //Ogunaro Wolfrunner
            if (player->GetReputationRank(76) != REP_EXALTED && race != RACE_ORC)
                player->SEND_GOSSIP_MENU(5841, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 3685: //Harb Clawhoof
            if (player->GetReputationRank(81) != REP_EXALTED && race != RACE_TAUREN)
                player->SEND_GOSSIP_MENU(5843, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 4730: //Lelanai
            if (player->GetReputationRank(69) != REP_EXALTED && race != RACE_NIGHTELF)
                player->SEND_GOSSIP_MENU(5844, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 4731: //Zachariah Post
            if (player->GetReputationRank(68) != REP_EXALTED && race != RACE_UNDEAD_PLAYER)
                player->SEND_GOSSIP_MENU(5840, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 4885: //Gregor MacVince
            if (player->GetReputationRank(72) != REP_EXALTED && race != RACE_HUMAN)
                player->SEND_GOSSIP_MENU(5855, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 7952: //Zjolnir
            if (player->GetReputationRank(530) != REP_EXALTED && race != RACE_TROLL)
                player->SEND_GOSSIP_MENU(5842, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 7955: //Milli Featherwhistle
            if (player->GetReputationRank(54) != REP_EXALTED && race != RACE_GNOME)
                player->SEND_GOSSIP_MENU(5857, _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 16264: //Winaestra
            if (player->GetReputationRank(911) != REP_EXALTED && race != RACE_BLOODELF)
                //player->SEND_GOSSIP_MENU(5844, _Creature->GetGUID());//unknown id
                player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());
            else 
                canBuy = true;
            break;
        case 17584: //Torallius the Pack Handler
            if (player->GetReputationRank(930) != REP_EXALTED && race != RACE_DRAENEI)
                //player->SEND_GOSSIP_MENU(5844, _Creature->GetGUID());//unknown id
                player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());
            else 
                canBuy = true;
            break;
    }

    if (canBuy)
    {
        if (_Creature->isVendor())
            player->ADD_GOSSIP_ITEM( 1, GOSSIP_ITEM_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
    }

    return true;
}

bool GossipSelect_npc_mount_vendor(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_TRADE)
    {
        player->SEND_VENDORLIST( _Creature->GetGUID() );
    }
    return true;
}

void AddSC_npc_mount_vendor()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_mount_vendor";
    newscript->pGossipHello =  &GossipHello_npc_mount_vendor;
    newscript->pGossipSelect = &GossipSelect_npc_mount_vendor;
    m_scripts[nrscripts++] = newscript;
}
