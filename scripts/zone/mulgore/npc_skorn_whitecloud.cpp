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
SDName: Npc_Skorn_Whitecloud
SD%Complete: 100
SDComment: Just a story if not rewarded for quest
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

bool GossipHello_skorn_whitecloud(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (!player->GetQuestRewardStatus(770) == 1)
        player->ADD_GOSSIP_ITEM( 0, "Tell me a story, Skorn.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF );

    player->SEND_GOSSIP_MENU(522,_Creature->GetGUID());     // Skorn Whitecloud message

    return true;
}

bool GossipSelect_skorn_whitecloud(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF)
        player->SEND_GOSSIP_MENU(523,_Creature->GetGUID());

    return true;
}

void AddSC_skorn_whitecloud()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="skorn_whitecloud";
    newscript->pGossipHello          = &GossipHello_skorn_whitecloud;
    newscript->pGossipSelect         = &GossipSelect_skorn_whitecloud;
    m_scripts[nrscripts++] = newscript;
}
