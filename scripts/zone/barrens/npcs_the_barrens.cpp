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
SDName: Npcs_The_Barrens
SD%Complete: 100
SDComment: Quest support: 4921, 6981
SDCategory: Barrens
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

/*######
## npc_beaten_corpse
######*/

bool GossipHello_npc_beaten_corpse(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(4921) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(4921) == QUEST_STATUS_COMPLETE)
        player->ADD_GOSSIP_ITEM(0,"Examine corpse in detail...",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(3557,_Creature->GetGUID());
    return true; 
}

bool GossipSelect_npc_beaten_corpse(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if(action == GOSSIP_ACTION_INFO_DEF +1)
    {
        player->SEND_GOSSIP_MENU(3558,_Creature->GetGUID());
        player->KilledMonster( 10668,_Creature->GetGUID() );
    }
    return true;
}

/*######
## npc_sputtervalve
######*/

bool GossipHello_npc_sputtervalve(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if( player->GetQuestStatus(6981) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM(0,"Can you tell me about this shard?",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(),_Creature->GetGUID());
    return true; 
}

bool GossipSelect_npc_sputtervalve(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if(action == GOSSIP_ACTION_INFO_DEF)
    {
        player->SEND_GOSSIP_MENU(2013,_Creature->GetGUID());
        player->AreaExploredOrEventHappens(6981);
    }
    return true;
}

void AddSC_npcs_the_barrens()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_beaten_corpse";
    newscript->pGossipHello = &GossipHello_npc_beaten_corpse;
    newscript->pGossipSelect = &GossipSelect_npc_beaten_corpse;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_sputtervalve";
    newscript->pGossipHello = &GossipHello_npc_sputtervalve;
    newscript->pGossipSelect = &GossipSelect_npc_sputtervalve;
    m_scripts[nrscripts++] = newscript;
}
