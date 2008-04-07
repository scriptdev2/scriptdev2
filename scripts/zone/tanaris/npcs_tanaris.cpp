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
SDName: Npcs_Tanaris
SD%Complete: 100
SDComment: Quest support: 10279 (Special flight path). Noggenfogger vendor
SDCategory: Tanaris
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"
#include "WorldPacket.h"

/*######
## npc_marin_noggenfogger
######*/

bool GossipHello_npc_marin_noggenfogger(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (_Creature->isVendor() && player->GetQuestRewardStatus(2662))
        player->ADD_GOSSIP_ITEM( 1, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_marin_noggenfogger(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_TRADE)
    {
        player->SEND_VENDORLIST( _Creature->GetGUID() );
    }
    return true;
}

/*######
## npc_steward_of_time
######*/

#define GOSSIP_FLIGHT "Give me a flight. <Real gossip missing>"

bool GossipHello_npc_steward_of_time(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(10279) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10279) == QUEST_STATUS_COMPLETE)
    {
        player->ADD_GOSSIP_ITEM(0, GOSSIP_FLIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(9978,_Creature->GetGUID());
    }
    else
    {
        _Creature->prepareGossipMenu( player,0 );
        _Creature->sendPreparedGossip( player );
    }
    return true;
}

bool QuestAccept_npc_steward_of_time(Player *player, Creature *creature, Quest const *quest )
{
    if(quest->GetQuestId() == 10279) //Quest: To The Master's Lair
    {
        std::vector<uint32> nodes;

        nodes.resize(2);// Creature No.20142
        nodes[0] = 144; // Quest - Caverns of Time (Intro Flight Path) (Start)
        nodes[1] = 143; // Quest - Caverns of Time (Intro Flight Path) (End)
        player->ActivateTaxiPathTo(nodes);
    }
    return false;
}

void SendDefaultMenu_npc_steward_of_time(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        std::vector<uint32> nodes;

        nodes.resize(2);// Creature No.20142
        nodes[0] = 144; // Quest - Caverns of Time (Intro Flight Path) (Start)
        nodes[1] = 143; // Quest - Caverns of Time (Intro Flight Path) (End)
        player->ActivateTaxiPathTo(nodes);
    }
}

bool GossipSelect_npc_steward_of_time(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_npc_steward_of_time(player, _Creature, action);

    return true;
}

struct MANGOS_DLL_DECL npc_steward_of_timeAI : public ScriptedAI
{
    npc_steward_of_timeAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void Aggro(Unit* who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        WorldPacket data;
        data.Initialize(SMSG_MOVE_SET_HOVER, 8+4);
        data.append(m_creature->GetPackGUID());
        data << uint32(0);
        m_creature->SendMessageToSet(&data,true);
    }
}; 
CreatureAI* GetAI_npc_steward_of_time(Creature *_Creature)
{
    return new npc_steward_of_timeAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_npcs_tanaris()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_marin_noggenfogger";
    newscript->pGossipHello =  &GossipHello_npc_marin_noggenfogger;
    newscript->pGossipSelect = &GossipSelect_npc_marin_noggenfogger;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_steward_of_time";
    newscript->GetAI = GetAI_npc_steward_of_time;
    newscript->pGossipHello          = &GossipHello_npc_steward_of_time;
    newscript->pGossipSelect         = &GossipSelect_npc_steward_of_time;
    newscript->pQuestAccept          = &QuestAccept_npc_steward_of_time;
    m_scripts[nrscripts++] = newscript;
}
