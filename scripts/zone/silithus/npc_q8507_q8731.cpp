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

#include "../../sc_defines.h"

// **** This script is still under Developement ****

#define GOSSIP_ITEM        "I need you to sign my Field Duty Papers"

const char GossipTexts[4][75] = 
{
    {"Be aware on the road of return, i saw some Ironforge Legion."},
    {"Nice to meet you my Good $C"},
    {"OK bye, say hello to Windcaller Kaldon for me."},
    {"Have a good day my friend $N."}
};
bool GossipSelect_npc_q8507_q8731(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if( action == GOSSIP_ACTION_INFO_DEF + 1 )
    {
        player->PlayerTalkClass->CloseGossip();
        
        if( player->HasItemCount(23024,1) )
        {
            uint16 dest;
            uint8 msg = player->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, 20810, 1, false );
            if( msg == EQUIP_ERR_OK )
            {
                Item * item = player->StoreNewItem( dest, 20810, 1, true);
                if( item )
                {
                    player->DestroyItemCount(23024,1,true);
                    player->UpdatePvP(true,true);
                    _Creature->Say(GossipTexts[rand()%4], LANG_UNIVERSAL, player->GetGUID());
                }
                else
                   _Creature->Say("I can't add you the item. Ask GM quest: Field Duty", LANG_UNIVERSAL, player->GetGUID());  
            }
        }
        else if( player->HasItemCount(21143,1) )
            _Creature->Say("You must prepare first your Field Duty Papper", LANG_UNIVERSAL, player->GetGUID());
        else
            _Creature->Say("You do not have your Field Duty Papper", LANG_UNIVERSAL, player->GetGUID());  
    }
    return true;
}
bool GossipHello_npc_q8507_q8731(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(8731) == QUEST_STATUS_INCOMPLETE || 
        player->GetQuestStatus(8507) == QUEST_STATUS_INCOMPLETE )
    {
        player->ADD_GOSSIP_ITEM( 2, GOSSIP_ITEM , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(3543,_Creature->GetGUID());
    }
    else
    {
        _Creature->prepareGossipMenu( player,0 );
        _Creature->sendPreparedGossip( player );
    }
    return true;
}
void AddSC_npc_q8507_q8731()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_q8507_q8731";
    newscript->pGossipHello = &GossipHello_npc_q8507_q8731;
    newscript->pGossipSelect = &GossipSelect_npc_q8507_q8731;
    m_scripts[nrscripts++] = newscript;
}