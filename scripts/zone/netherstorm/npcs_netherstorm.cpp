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
SDName: npcs_netherstorm
SD%Complete: 99
SDComment: misc npcs netherstorm, true text missing for gossip items. Script Veronia disabled due to core limitation.
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## npc_protectorate_nether_drake
######*/

#define GOSSIP_ITEM1_PRO "Fly me to Ultris"

bool GossipHello_npc_protectorate_nether_drake(Player *player, Creature *_Creature)
{
        //On Nethery Wings
    if (player->GetQuestStatus(10438) == QUEST_STATUS_INCOMPLETE && player->HasItemCount(29778,1) )
    {
        player->ADD_GOSSIP_ITEM(2, GOSSIP_ITEM1_PRO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    }

    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_protectorate_nether_drake(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->PlayerTalkClass->CloseGossip();

        std::vector<uint32> nodes;

        nodes.resize(2);
        nodes[0] = 152;                                     //from drake
        nodes[1] = 153;                                     //end at drake
        player->ActivateTaxiPathTo(nodes);                  //TaxiPath 627
    }
    return true;
}

/*######
## npc_veronia
######*/

/*
UPDATE `creature_template` SET `ScriptName` = 'npc_veronia' WHERE `entry` = 20162;
*/

/*#define GOSSIP_ITEM1_VER "Fly me to Manaforge Coruu"

bool GossipHello_npc_veronia(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

        //Behind Enemy Lines
    if (player->GetQuestStatus(10652) != QUEST_STATUS_NONE && !player->GetQuestRewardStatus(10652))
    {
        player->ADD_GOSSIP_ITEM(2, GOSSIP_ITEM1_VER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    }

    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_veronia(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->PlayerTalkClass->CloseGossip();

        std::vector<uint32> nodes;

        nodes.resize(2);
        nodes[0] = 145;                                     //from area 52
        nodes[1] = 146;                                     //end at manaforge coruu
        player->ActivateTaxiPathTo(nodes);                  //TaxiPath 606
    }
    return true;
}*/

/*######
## 
######*/

void AddSC_npcs_netherstorm()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_protectorate_nether_drake";
    newscript->pGossipHello =   &GossipHello_npc_protectorate_nether_drake;
    newscript->pGossipSelect =  &GossipSelect_npc_protectorate_nether_drake;
    m_scripts[nrscripts++] = newscript;

    /*newscript = new Script;
    newscript->Name="npc_veronia";
    newscript->pGossipHello =   &GossipHello_npc_veronia;
    newscript->pGossipSelect =  &GossipSelect_npc_veronia;
    m_scripts[nrscripts++] = newscript;*/
}
