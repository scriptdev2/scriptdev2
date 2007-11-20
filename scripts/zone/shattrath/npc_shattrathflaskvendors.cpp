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
SDName: Npc_Shattrath_Flask_Vendors
SD%Complete: 100
SDComment: Provides flasks to people exalted with 3 factions
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"
#include "../../../../../game/QuestDef.h"

/*
##################################################
Shattrath City Flask Vendors:
Haldor the Compulsive 
Arcanist Xorith
Both sell special flasks for use in Outlands 25man raids only,
purchasable for one Mark of Illidari each
Purchase requires exalted reputation with Scryers/Aldor, Cenarion Expedition and The Sha'tar
################################################## 
*/

bool GossipHello_npc_shattrathflaskvendors(Player *player, Creature *_Creature)
{

    if(_Creature->GetEntry() == 23484)
    {
        // Aldor vendor

        if( (player->GetReputationRank(932) == REP_EXALTED) && (player->GetReputationRank(935) == REP_EXALTED) && (player->GetReputationRank(942) == REP_EXALTED) )
        {
            player->ADD_GOSSIP_ITEM( 1, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        }
        else
        {
            player->SEND_GOSSIP_MENU(11083, _Creature->GetGUID());        
            player->PlayerTalkClass->SendGossipMenu(11083,_Creature->GetGUID());
        }        
    }

    if(_Creature->GetEntry() == 23483)
    {
        // Scryers vendor

        if( (player->GetReputationRank(934) == REP_EXALTED) && (player->GetReputationRank(935) == REP_EXALTED) && (player->GetReputationRank(942) == REP_EXALTED) )
        {
            player->ADD_GOSSIP_ITEM( 1, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            player->PlayerTalkClass->SendGossipMenu(11084 ,_Creature->GetGUID());
        }
        else
        {
            player->SEND_GOSSIP_MENU(11084, _Creature->GetGUID());
        }        
    }

    return true;

}

bool GossipSelect_npc_shattrathflaskvendors(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_TRADE)
    {
        player->SEND_VENDORLIST( _Creature->GetGUID() );
    }
    return true;
}

void AddSC_npc_shattrathflaskvendors()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_shattrathflaskvendors";
    newscript->pGossipHello =  &GossipHello_npc_shattrathflaskvendors;
    newscript->pGossipSelect = &GossipSelect_npc_shattrathflaskvendors;
    m_scripts[nrscripts++] = newscript;

}
