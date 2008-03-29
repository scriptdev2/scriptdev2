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
SDName: Npc_Captains_Blackanvil & Skullsplitter
SD%Complete: 100
SDComment: Script for quests 8731 & 8507
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"
#include "../../../../../game/QuestDef.h"

// **** This script is still under Developement ****

#define GOSSIP_ITEM        "I need you to sign my Field Duty Papers"

const char GossipTexts[3][75] = 
{
    {"Nice to meet you my Good $C           "},
    {"Say hello to Windcaller Kaldon for me."},
    {"Have a good day my friend $N.         "}
};
bool GossipSelect_npcs_captains_blackanvil_and_skullsplit(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if( action == GOSSIP_ACTION_INFO_DEF + 1 )
    {
        if( player->HasItemCount(23024,1) )
        {
            ItemPosCountVec dest;
            uint8 msg = player->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, 20810, 1 );
            if( msg == EQUIP_ERR_OK )
            {
                Item * item = player->StoreNewItem( dest, 20810, 1 );
                if( item )
                {
                    player->DestroyItemCount(23024,1,true);
                    player->UpdatePvP(true,true);
                    player->PlayerTalkClass->CloseGossip();
                    _Creature->Say(GossipTexts[rand()%3], LANG_UNIVERSAL, player->GetGUID());
                }
                else
                    player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW,NULL,NULL);
            }
            else
                player->SendEquipError(EQUIP_ERR_INVENTORY_FULL,NULL,NULL);
        }
        else if( player->HasItemCount(21143,1) )
        {
            player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND,NULL,NULL);
            _Creature->Say("Prepare your Unsigned Field Duty Papers", LANG_UNIVERSAL, player->GetGUID());
        }
        else
            player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND,NULL,NULL);
    }
    return true;
}
bool GossipHello_npcs_captains_blackanvil_and_skullsplit(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(8731) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(8507) == QUEST_STATUS_INCOMPLETE )
    {
        player->ADD_GOSSIP_ITEM( 2, GOSSIP_ITEM , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(7827,_Creature->GetGUID());
    }
    else
    {
        _Creature->prepareGossipMenu( player,0 );
        _Creature->sendPreparedGossip( player );
    }
    return true;
}
void AddSC_npcs_captains_blackanvil_and_skullsplit()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npcs_captains_blackanvil_and_skullsplit";
    newscript->pGossipHello =  &GossipHello_npcs_captains_blackanvil_and_skullsplit;
    newscript->pGossipSelect = &GossipSelect_npcs_captains_blackanvil_and_skullsplit;
    m_scripts[nrscripts++] = newscript;
}
