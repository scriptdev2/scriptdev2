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
SDName: Npcs_Western_Plaguelands
SD%Complete: 90
SDComment: To obtain Vitreous Focuser (could use more spesifics about gossip items)
SDCategory: Western Plaguelands
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"

bool GossipHello_npcs_dithers_and_arbington(Player *player, Creature *_Creature)
{
    if(_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );
    if (_Creature->isVendor())
        player->ADD_GOSSIP_ITEM( 1, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    if(player->GetQuestRewardStatus(5237) || player->GetQuestRewardStatus(5238))
    {
        player->ADD_GOSSIP_ITEM(0, "What does the Felstone Field Cauldron need?",      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->ADD_GOSSIP_ITEM(0, "What does the Dalson's Tears Cauldron need?",      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        player->ADD_GOSSIP_ITEM(0, "What does the Writhing Haunt Cauldron need?",      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
        player->ADD_GOSSIP_ITEM(0, "What does the Gahrron's Withering Cauldron need?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
        player->SEND_GOSSIP_MENU(3985, _Creature->GetGUID());
    }else
        player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npcs_dithers_and_arbington(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch(action)
    {
    case GOSSIP_ACTION_TRADE:
        player->SEND_VENDORLIST( _Creature->GetGUID() );
        break;
    case GOSSIP_ACTION_INFO_DEF+1:
        player->ADD_GOSSIP_ITEM(0, "Thanks, i need a Vitreous Focuser", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
        player->SEND_GOSSIP_MENU(3980, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+2:
        player->ADD_GOSSIP_ITEM(0, "Thanks, i need a Vitreous Focuser", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
        player->SEND_GOSSIP_MENU(3981, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+3:
        player->ADD_GOSSIP_ITEM(0, "Thanks, i need a Vitreous Focuser", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
        player->SEND_GOSSIP_MENU(3982, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+4:
        player->ADD_GOSSIP_ITEM(0, "Thanks, i need a Vitreous Focuser", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
        player->SEND_GOSSIP_MENU(3983, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+5:
        player->CLOSE_GOSSIP_MENU();
        _Creature->CastSpell(player, 17529, false);
        break;
    }
    return true;
}

void AddSC_npcs_western_plaguelands()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npcs_dithers_and_arbington";
    newscript->pGossipHello = &GossipHello_npcs_dithers_and_arbington;
    newscript->pGossipSelect = &GossipSelect_npcs_dithers_and_arbington;
    m_scripts[nrscripts++] = newscript;
}
