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
SDName: Npcs_Isle_of_Queldanas
SD%Complete: 100
SDComment: Quest support: 11532, 11533, 11542, 11543
SDCategory: Isle Of Quel'Danas
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

/*######
## npc_ayren_cloudbreaker
######*/

bool GossipHello_npc_ayren_cloudbreaker(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(11532) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(11533) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM(0,"Dead Scar bombing run",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);

    if( player->GetQuestStatus(11542) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(11543) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM(0,"Ship bombing run",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(),_Creature->GetGUID());
    return true; 
}

bool GossipSelect_npc_ayren_cloudbreaker(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->CLOSE_GOSSIP_MENU();

        std::vector<uint32> nodes;

        nodes.resize(2);
        nodes[0] = 209;                                     //from ayren
        nodes[1] = 210;                                     //end at ayren
        player->ActivateTaxiPathTo(nodes,25044);            //TaxiPath 779
    }
    if (action == GOSSIP_ACTION_INFO_DEF+2)
    {
        player->CLOSE_GOSSIP_MENU();

        std::vector<uint32> nodes;

        nodes.resize(2);
        nodes[0] = 211;                                     //from ayren
        nodes[1] = 212;                                     //end at unrestrained dragonhawk
        player->ActivateTaxiPathTo(nodes,25044);            //TaxiPath 784
    }
    return true;
}

/*######
## npc_unrestrained_dragonhawk
######*/

bool GossipHello_npc_unrestrained_dragonhawk(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(11542) == QUEST_STATUS_COMPLETE || player->GetQuestStatus(11543) == QUEST_STATUS_COMPLETE )
        player->ADD_GOSSIP_ITEM(0,"Take me back.",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(),_Creature->GetGUID());
    return true; 
}

bool GossipSelect_npc_unrestrained_dragonhawk(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->CLOSE_GOSSIP_MENU();

        std::vector<uint32> nodes;

        nodes.resize(2);
        nodes[0] = 212;                                     //from unrestrained dragonhawk
        nodes[1] = 211;                                     //end at ayren
        player->ActivateTaxiPathTo(nodes,25044);            //TaxiPath 788
    }
    return true;
}

void AddSC_npcs_isle_of_queldanas()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_ayren_cloudbreaker";
    newscript->pGossipHello = &GossipHello_npc_ayren_cloudbreaker;
    newscript->pGossipSelect = &GossipSelect_npc_ayren_cloudbreaker;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_unrestrained_dragonhawk";
    newscript->pGossipHello = &GossipHello_npc_unrestrained_dragonhawk;
    newscript->pGossipSelect = &GossipSelect_npc_unrestrained_dragonhawk;
    m_scripts[nrscripts++] = newscript;
}
