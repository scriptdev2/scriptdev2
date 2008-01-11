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
SDName: Npc_Great_Bear_Spirit
SD%Complete: 100
SDComment: For Quest 5929, 5930
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"

// **** This script is still under Developement ****

#define GOSSIP_ITEM1 "What do you represent, spirit?"
#define GOSSIP_ITEM2 "I seek to understand the importance of strength of the body."
#define GOSSIP_ITEM3 "I seek to understand the importance of strength of the heart."
#define GOSSIP_ITEM4 "I have heard your words, Great Bear Spirit, and I understand. I now seek your blessings to fully learn the way of the Claw."

/*################
## Start default menu
################*/

bool GossipHello_npc_great_bear_spirit(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(5929) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(5930) == QUEST_STATUS_INCOMPLETE)//ally or horde quest
    {
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        player->SEND_GOSSIP_MENU(4719, _Creature->GetGUID());
    }
    else
        player->SEND_GOSSIP_MENU(4718, _Creature->GetGUID());

    return true;
}

/*################
## Menues
################*/

bool GossipSelect_npc_great_bear_spirit(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(4721, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(4733, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(4734, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            player->SEND_GOSSIP_MENU(4735, _Creature->GetGUID());
            if (player->GetQuestStatus(5929)==QUEST_STATUS_INCOMPLETE)
                player->CompleteQuest(5929);
            if (player->GetQuestStatus(5930)==QUEST_STATUS_INCOMPLETE)
                player->CompleteQuest(5930);
            break;
    }
    return true;
}

/*########
## End
########*/

void AddSC_npc_great_bear_spirit()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_great_bear_spirit";
    newscript->pGossipHello =  &GossipHello_npc_great_bear_spirit;
    newscript->pGossipSelect = &GossipSelect_npc_great_bear_spirit;
    m_scripts[nrscripts++] = newscript;
}
