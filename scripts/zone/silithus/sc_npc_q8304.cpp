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

#define GOSSIP_ITEM        "You know something about a girl named Natalia?"

const char GossipTexts[4][75] = 
{
    {"I last see her going north of here, she was looking sick."},
    {"I remember seeing her, but haven't see where she going, sorry."},
    {"Yeah i see her, she was into bad shape."},
    {"Of course, i see her going north of her, she was talking alone."}
};
bool GossipSelect_npc_q8304(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Search Natalia
    {
        player->PlayerTalkClass->CloseGossip();

        _Creature->Say(GossipTexts[rand()%4], LANG_UNIVERSAL, player->GetGUID());
        player->Player::KilledMonster( _Creature->GetEntry(), _Creature->GetGUID() );
    }
    return true;
}
bool GossipHello_npc_q8304(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(8304) == QUEST_STATUS_INCOMPLETE && !player->GetReqKillOrCastCurrentCount(8304, _Creature->GetEntry()) )
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
void AddSC_npc_q8304()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_q8304";
    newscript->pGossipHello = &GossipHello_npc_q8304;
    newscript->pGossipSelect = &GossipSelect_npc_q8304;
    m_scripts[nrscripts++] = newscript;
}