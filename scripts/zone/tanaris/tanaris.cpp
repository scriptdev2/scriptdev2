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
SDName: Tanaris
SD%Complete: 80
SDComment: Quest support: 2954, 4005, 10279(Special flight path). Noggenfogger vendor
SDCategory: Tanaris
EndScriptData */

/* ContentData
mob_aquementas
npc_marin_noggenfogger
npc_steward_of_time
npc_stone_watcher_of_norgannon
EndContentData */

#include "sc_creature.h"
#include "sc_gossip.h"
#include "WorldPacket.h"

/*######
## mob_aquementas
######*/

#define AQUE_AGGRO_YELL "Who dares awaken Aquementas?"

struct MANGOS_DLL_DECL mob_aquementasAI : public ScriptedAI
{
    mob_aquementasAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 SendItemTimer;
    uint32 SwitchFactionTimer;
    bool isFriendly;

    void Reset()
    {
        SendItemTimer = 25000;
        SwitchFactionTimer = 10000;
        m_creature->setFaction(35);
        isFriendly = true;
    }

    void SendItem(Unit* receiver)
    {
        if (((Player*)receiver)->HasItemCount(11169,1,false) && 
            ((Player*)receiver)->HasItemCount(11172,11,false) && 
            ((Player*)receiver)->HasItemCount(11173,1,false) &&
            !((Player*)receiver)->HasItemCount(11522,1,true))
        {
            ItemPosCountVec dest;
            uint8 msg = ((Player*)receiver)->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, 11522, 1, false);
            if( msg == EQUIP_ERR_OK )
                ((Player*)receiver)->StoreNewItem( dest, 11522, 1, true);
        }
    }

    void Aggro(Unit* who)
    {
        DoYell(AQUE_AGGRO_YELL,LANG_UNIVERSAL,who);
    }

    void UpdateAI(const uint32 diff)
    {
        if(isFriendly)
        {
            if (SwitchFactionTimer < diff)
            {
                m_creature->setFaction(91);
                isFriendly = false;
            }else SwitchFactionTimer -= diff;
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!isFriendly)
        {
            if (SendItemTimer < diff)
            {
                if(m_creature->getVictim()->GetTypeId() == TYPEID_PLAYER)
                    SendItem(m_creature->getVictim());

                SendItemTimer = 5000;
            }else SendItemTimer -= diff;
        }

        //TODO: add spells
        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mob_aquementas(Creature *_Creature)
{
    return new mob_aquementasAI (_Creature);
}

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

#define GOSSIP_FLIGHT "Please take me to the master's lair."

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
## npc_stone_watcher_of_norgannon
######*/

bool GossipHello_npc_stone_watcher_of_norgannon(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(2954) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "What function do you serve?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->SEND_GOSSIP_MENU(1674, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_stone_watcher_of_norgannon(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, "What are the Plates of Uldum?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(1675, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "Where are the Plates of Uldum?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(1676, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, "Excuse me? We've been \"reschedueled for visitations\"? What does that mean?!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->SEND_GOSSIP_MENU(1677, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM( 0, "So, what's inside Uldum?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->SEND_GOSSIP_MENU(1678, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM( 0, "I will return when i have the Plates of Uldum.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU(1679, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->CLOSE_GOSSIP_MENU();
            player->AreaExploredOrEventHappens(2954);
            break;
    }
    return true;
}

/*######
## AddSC
######*/

void AddSC_tanaris()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_aquementas";
    newscript->GetAI = GetAI_mob_aquementas;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_marin_noggenfogger";
    newscript->pGossipHello =  &GossipHello_npc_marin_noggenfogger;
    newscript->pGossipSelect = &GossipSelect_npc_marin_noggenfogger;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_steward_of_time";
    newscript->GetAI = GetAI_npc_steward_of_time;
    newscript->pGossipHello =  &GossipHello_npc_steward_of_time;
    newscript->pGossipSelect = &GossipSelect_npc_steward_of_time;
    newscript->pQuestAccept =  &QuestAccept_npc_steward_of_time;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_stone_watcher_of_norgannon";
    newscript->pGossipHello =  &GossipHello_npc_stone_watcher_of_norgannon;
    newscript->pGossipSelect = &GossipSelect_npc_stone_watcher_of_norgannon;
    m_scripts[nrscripts++] = newscript;
}
