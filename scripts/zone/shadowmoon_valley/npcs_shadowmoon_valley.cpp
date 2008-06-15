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
SDName: Npcs_Shadowmoon_Valley
SD%Complete: 100
SDComment: Quest support: 10519, 10583, 10601, 10814. Vendor Drake Dealer Hurlunk. Teleporter TO Invasion Point: Cataclysm
SDCategory: Shadowmoon Valley
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

/*######
## npc_drake_dealer_hurlunk
######*/

bool GossipHello_npc_drake_dealer_hurlunk(Player *player, Creature *_Creature)
{
    if (_Creature->isVendor() && player->GetReputationRank(1015) == REP_EXALTED)
        player->ADD_GOSSIP_ITEM( 1, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_drake_dealer_hurlunk(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_TRADE)
        player->SEND_VENDORLIST( _Creature->GetGUID() );

    return true;
}

/*######
## npc_invis_legion_teleporter
######*/

#define SPELL_TELE_A_TO   37387
#define SPELL_TELE_H_TO   37389

struct MANGOS_DLL_DECL npc_invis_legion_teleporterAI : public ScriptedAI
{
    npc_invis_legion_teleporterAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint64 PlayerGuid;
    uint32 TeleTimer;

    void Reset()
    {
        PlayerGuid=0;
        TeleTimer = 5000;
    }

    void Aggro(Unit* who)
    {
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || who->GetTypeId() != TYPEID_PLAYER)
            return;

        if(who->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance(who)<4)
        {
            if (who->isAlive() || !who->isInCombat())
                PlayerGuid = who->GetGUID();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(TeleTimer < diff)
        {
            if(PlayerGuid)
            {
                Player* player = ((Player*)Unit::GetUnit((*m_creature), PlayerGuid));

                if(m_creature->GetDistance(player)<3)
                {
                    if(player->GetTeam()== ALLIANCE && player->GetQuestRewardStatus(10589))
                        player->CastSpell(player,SPELL_TELE_A_TO,false);
                    if(player->GetTeam()== HORDE && player->GetQuestRewardStatus(10604))
                        player->CastSpell(player,SPELL_TELE_H_TO,false);
                }
                PlayerGuid=0;
            }
            TeleTimer = 5000;
        }else TeleTimer -= diff;
    }
};
CreatureAI* GetAI_npc_invis_legion_teleporter(Creature *_Creature)
{
    return new npc_invis_legion_teleporterAI (_Creature);
}

/*######
## npc_flanis_swiftwing_and_kagrosh
######*/

bool GossipHello_npcs_flanis_swiftwing_and_kagrosh(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(10583) == QUEST_STATUS_INCOMPLETE && !player->HasItemCount(30658,1,true))
        player->ADD_GOSSIP_ITEM( 0, "Take Flanis's Pack", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    if (player->GetQuestStatus(10601) == QUEST_STATUS_INCOMPLETE && !player->HasItemCount(30659,1,true))
        player->ADD_GOSSIP_ITEM( 0, "Take Kagrosh's Pack", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npcs_flanis_swiftwing_and_kagrosh(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
	    ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, 30658, 1, false);
        if( msg == EQUIP_ERR_OK )
        {
            player->StoreNewItem( dest, 30658, 1, true);
            player->PlayerTalkClass->ClearMenus();
        }
    }
    if (action == GOSSIP_ACTION_INFO_DEF+2)
    {
	    ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, 30659, 1, false);
        if( msg == EQUIP_ERR_OK )
        {
            player->StoreNewItem( dest, 30659, 1, true);
            player->PlayerTalkClass->ClearMenus();
        }
    }
    return true;
}

/*######
## npc_neltharaku
######*/

bool GossipHello_npc_neltharaku(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(10814) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "I am listening, dragon", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(10613, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_neltharaku(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "But you are dragons! How could orcs do this to you?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(10614, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, "Your mate?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->SEND_GOSSIP_MENU(10615, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM( 0, "I have battled many beasts, dragon. I will help you.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->SEND_GOSSIP_MENU(10616, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->CLOSE_GOSSIP_MENU();
            player->AreaExploredOrEventHappens(10814);
            break;
    }
    return true;
}

/*######
## npc_oronok
######*/

#define GOSSIP_ORONOK1 "I am ready to hear your story, Oronok."
#define GOSSIP_ORONOK2 "How do I find the cipher?"
#define GOSSIP_ORONOK3 "How do you know all of this?"
#define GOSSIP_ORONOK4 "Yet what? What is it, Oronok?"
#define GOSSIP_ORONOK5 "Continue, please."
#define GOSSIP_ORONOK6 "So what of the cipher now? And your boys?"
#define GOSSIP_ORONOK7 "I will find your boys and the cipher, Oronok."

bool GossipHello_npc_oronok_tornheart(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );
    if (_Creature->isVendor())
        player->ADD_GOSSIP_ITEM( 1, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
    
    if (player->GetQuestStatus(10519) == QUEST_STATUS_INCOMPLETE)
    {
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ORONOK1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        player->SEND_GOSSIP_MENU(10312, _Creature->GetGUID());
    }else
        player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_oronok_tornheart(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_TRADE:
            player->SEND_VENDORLIST( _Creature->GetGUID() );
            break;
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ORONOK2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(10313, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ORONOK3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(10314, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ORONOK4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->SEND_GOSSIP_MENU(10315, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ORONOK5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->SEND_GOSSIP_MENU(10316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ORONOK6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU(10317, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ORONOK7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            player->SEND_GOSSIP_MENU(10318, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            player->CLOSE_GOSSIP_MENU();
            player->AreaExploredOrEventHappens(10519);
            break;
    }
    return true;
}

/*####
# npc_karynaku
####*/

bool QuestAccept_npc_karynaku(Player* player, Creature* creature, Quest const* quest)
{
    if(quest->GetQuestId() == 10870) // Ally of the Netherwing
    {
        std::vector<uint32> nodes;

        nodes.resize(2);
        nodes[0] = 161; // From Karynaku
        nodes[1] = 162; // To Mordenai
        error_log("SD2: Player %s started quest 10870 which has disabled taxi node, need to be fixed in core", player->GetName());
//        player->ActivateTaxiPathTo(nodes, 20811);
    }

    return true;
}

void AddSC_npcs_shadowmoon_valley()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_drake_dealer_hurlunk";
    newscript->pGossipHello =  &GossipHello_npc_drake_dealer_hurlunk;
    newscript->pGossipSelect = &GossipSelect_npc_drake_dealer_hurlunk;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_invis_legion_teleporter";
    newscript->GetAI = GetAI_npc_invis_legion_teleporter;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npcs_flanis_swiftwing_and_kagrosh";
    newscript->pGossipHello =  &GossipHello_npcs_flanis_swiftwing_and_kagrosh;
    newscript->pGossipSelect = &GossipSelect_npcs_flanis_swiftwing_and_kagrosh;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_neltharaku";
    newscript->pGossipHello =  &GossipHello_npc_neltharaku;
    newscript->pGossipSelect = &GossipSelect_npc_neltharaku;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "npc_karynaku";
    newscript->pQuestAccept = &QuestAccept_npc_karynaku;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_oronok_tornheart";
    newscript->pGossipHello =  &GossipHello_npc_oronok_tornheart;
    newscript->pGossipSelect = &GossipSelect_npc_oronok_tornheart;
    m_scripts[nrscripts++] = newscript;
}
