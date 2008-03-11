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
SDName: Npcs_Nagrand
SD%Complete: 90
SDComment: Quest support: 9991, 10107, 10108, 10044, 10172, 10646, 10085. TextId's unknown for altruis_the_sufferer and greatmother_geyah (npc_text)
SDCategory: Nagrand
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## npc_altruis_the_sufferer
######*/

bool GossipHello_npc_altruis_the_sufferer(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    //gossip before obtaining Survey the Land
    if ( player->GetQuestStatus(9991) == QUEST_STATUS_NONE )
        player->ADD_GOSSIP_ITEM( 0, "I see twisted steel and smell sundered earth.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+10);

    //gossip when Survey the Land is incomplete (technically, after the flight)
    if (player->GetQuestStatus(9991) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "Well...?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+20);

    if (player->GetQuestStatus(10646) == QUEST_STATUS_INCOMPLETE) //www.wowwiki.com/Varedis
        player->ADD_GOSSIP_ITEM( 0, "[PH] Story about Illidan's Pupil", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+30);

    player->SEND_GOSSIP_MENU(9419, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_altruis_the_sufferer(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+10:
            player->ADD_GOSSIP_ITEM( 0, "Legion?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->SEND_GOSSIP_MENU(9420, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+11:
            player->ADD_GOSSIP_ITEM( 0, "And now?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(9421, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+12:
            player->ADD_GOSSIP_ITEM( 0, "How do you see them now?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
            player->SEND_GOSSIP_MENU(9422, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+13:
            player->ADD_GOSSIP_ITEM( 0, "Forge camps?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
            player->SEND_GOSSIP_MENU(9423, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+14:
            player->SEND_GOSSIP_MENU(9424, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF+20:
            player->ADD_GOSSIP_ITEM( 0, "Ok.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
            player->SEND_GOSSIP_MENU(9427, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+21:
            player->PlayerTalkClass->CloseGossip();
            player->CompleteQuest(9991);
            break;

        case GOSSIP_ACTION_INFO_DEF+30:
            player->ADD_GOSSIP_ITEM( 0, "[PH] Story done", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
            player->SEND_GOSSIP_MENU(384, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+31:
            player->PlayerTalkClass->CloseGossip();
            player->CompleteQuest(10646);
            break;
    }
    return true;
}

bool QuestAccept_npc_altruis_the_sufferer(Player *player, Creature *creature, Quest const *quest )
{
    if ( !player->GetQuestRewardStatus(9991) )              //Survey the Land, q-id 9991
    {
        player->PlayerTalkClass->CloseGossip();

        std::vector<uint32> nodes;

        nodes.resize(2);
        nodes[0] = 113;                                     //from
        nodes[1] = 114;                                     //end at
        player->ActivateTaxiPathTo(nodes);                  //TaxiPath 532
    }
    return true;
}

/*######
## npc_greatmother_geyah
######*/

//all the textId's for the below is unknown, but i do believe the gossip item texts are proper.
bool GossipHello_npc_greatmother_geyah(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(10044) == QUEST_STATUS_INCOMPLETE)
    {
        player->ADD_GOSSIP_ITEM( 0, "Hello, Greatmother. Garrosh told me that you wanted to speak with me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(),_Creature->GetGUID());
    }
    else if (player->GetQuestStatus(10172) == QUEST_STATUS_INCOMPLETE)
    {
        player->ADD_GOSSIP_ITEM( 0, "Garrosh is beyond redemption, Greatmother. I fear that in helping the Mag'har, I have convinced Garrosh that he is unfit to lead.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
        player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(),_Creature->GetGUID());
    }
    else

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(),_Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_greatmother_geyah(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->ADD_GOSSIP_ITEM( 0, "You raised all of the orcs here, Greatmother?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            player->ADD_GOSSIP_ITEM( 0, "Do you believe that?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            player->ADD_GOSSIP_ITEM( 0, "What can be done? I have tried many different things. I have done my best to help the people of Nagrand. Each time I have approached Garrosh, he has dismissed me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:
            player->ADD_GOSSIP_ITEM( 0, "Left? How can you choose to leave?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:
            player->ADD_GOSSIP_ITEM( 0, "What is this duty?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:
            player->ADD_GOSSIP_ITEM( 0, "Is there anything I can do for you, Greatmother?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:
            player->CompleteQuest(10044);
            player->CLOSE_GOSSIP_MENU();
            break;

        case GOSSIP_ACTION_INFO_DEF + 10:
            player->ADD_GOSSIP_ITEM( 0, "I have done all that I could, Greatmother. I thank you for your kind words.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:
            player->ADD_GOSSIP_ITEM( 0, "Greatmother, you are the mother of Durotan?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:
            player->ADD_GOSSIP_ITEM( 0, "Greatmother, I never had the honor. Durotan died long before my time, but his heroics are known to all on my world. The orcs of Azeroth reside in a place known as Durotar, named after your son. And ... (You take a moment to breathe and think through what you are about to tell the Greatmother.)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 13:
            player->ADD_GOSSIP_ITEM( 0, "It is my Warchief, Greatmother. The leader of my people. From my world. He ... He is the son of Durotan. He is your grandchild.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 14:
            player->ADD_GOSSIP_ITEM( 0, "I will return to Azeroth at once, Greatmother.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 15:
            player->CompleteQuest(10172);
            player->CLOSE_GOSSIP_MENU();
            break;
    }
    return true;
}

/*######
## npc_lantresor_of_the_blade
######*/

bool GossipHello_npc_lantresor_of_the_blade(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(10107) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10108) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "I have killed many of your ogres, Lantresor. I have no fear.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->PlayerTalkClass->SendGossipMenu(9361, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_lantresor_of_the_blade(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, "Should I know? You look like an orc to me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(9362, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "And the other half?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(9363, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, "I have heard of your kind, but I never thought to see the day when I would meet a half-breed.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(9364, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM( 0, "My apologies. I did not mean to offend. I am here on behalf of my people.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->SEND_GOSSIP_MENU(9365, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM( 0, "My people ask that you pull back your Boulderfist ogres and cease all attacks on our territories. In return, we will also pull back our forces.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(9366, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->ADD_GOSSIP_ITEM( 0, "We will fight you until the end, then, Lantresor. We will not stand idly by as you pillage our towns and kill our people.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->SEND_GOSSIP_MENU(9367, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            player->ADD_GOSSIP_ITEM( 0, "What do I need to do?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->SEND_GOSSIP_MENU(9368, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            player->SEND_GOSSIP_MENU(9369, _Creature->GetGUID());
            if (player->GetQuestStatus(10107) == QUEST_STATUS_INCOMPLETE)
                player->CompleteQuest(10107);
            if (player->GetQuestStatus(10108) == QUEST_STATUS_INCOMPLETE)
                player->CompleteQuest(10108);
            break;
    }
    return true;
}
/*######
## npc_creditmarker_visist_with_ancestors (Quest 10085)
######*/

/*
update creature_template set scriptname = 'npc_creditmarker_visit_with_ancestors' where entry in (18840,18841,18842,18843);
*/

struct MANGOS_DLL_DECL npc_creditmarker_visit_with_ancestorsAI : public ScriptedAI
{
  npc_creditmarker_visit_with_ancestorsAI(Creature* c) : ScriptedAI(c) { Reset(); }

  void Reset()
  {
  }

  void MoveInLineOfSight(Unit *who)
  {
      if(!who)
          return;
      
      if(who->GetTypeId() == TYPEID_PLAYER)
      {
          if(((Player*)who)->GetQuestStatus(10085) == QUEST_STATUS_INCOMPLETE)
          {
              uint32 creditMarkerId = m_creature->GetEntry();
              if((creditMarkerId >= 18840) && (creditMarkerId <= 18843))
              {
                  // 18840: Sunspring, 18841: Laughing, 18842: Garadar, 18843: Bleeding
                  if(!((Player*)who)->GetReqKillOrCastCurrentCount(10085, creditMarkerId))
                      ((Player*)who)->KilledMonster(creditMarkerId, m_creature->GetGUID());
              }
          }
      }
  }
};

CreatureAI* GetAI_npc_creditmarker_visit_with_ancestors(Creature *_Creature)
{
    return new npc_creditmarker_visit_with_ancestorsAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_npcs_nagrand()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_altruis_the_sufferer";
    newscript->pGossipHello =  &GossipHello_npc_altruis_the_sufferer;
    newscript->pGossipSelect = &GossipSelect_npc_altruis_the_sufferer;
    newscript->pQuestAccept =  &QuestAccept_npc_altruis_the_sufferer;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_greatmother_geyah";
    newscript->pGossipHello =  &GossipHello_npc_greatmother_geyah;
    newscript->pGossipSelect = &GossipSelect_npc_greatmother_geyah;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_lantresor_of_the_blade";
    newscript->pGossipHello =  &GossipHello_npc_lantresor_of_the_blade;
    newscript->pGossipSelect = &GossipSelect_npc_lantresor_of_the_blade;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="npc_creditmarker_visit_with_ancestors";
    newscript->GetAI = GetAI_npc_creditmarker_visit_with_ancestors;
    m_scripts[nrscripts++] = newscript;
}
