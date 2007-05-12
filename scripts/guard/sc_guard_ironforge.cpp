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

#include "../sc_defines.h"
#include "sc_guard_ai.h"

bool GossipHello_guard_ironforge(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_IRONFORGE_BANK    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_DEEPRUNTRAM       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_GRYPHON           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN               , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);

    player->SEND_GOSSIP_MENU(2760, _Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_ironforge(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Auction House
    {
        player->SEND_POI(-4957.39, -911.6, 6, 6, 0, "Ironforge Auction House");
        player->SEND_GOSSIP_MENU(3014, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Bank
    {
        player->SEND_POI(-4891.91, -991.47, 6, 6, 0, "The Vault");
        player->SEND_GOSSIP_MENU(2761, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Tram
    {
        player->SEND_POI(-4835.27, -1294.69, 6, 6, 0, "Deeprun Tram");
        player->SEND_GOSSIP_MENU(3814, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Gryphon Master
    {
        player->SEND_POI(-4821.52, -1152.3, 6, 6, 0, "Ironforge Gryphon Master");
        player->SEND_GOSSIP_MENU(2762, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Guild Master
    {
        player->SEND_POI(-5021, -996.45, 6, 6, 0, "Ironforge Visitor's Center");
        player->SEND_GOSSIP_MENU(2764, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Inn
    {
        player->SEND_POI(-4850.47, -872.57, 6, 6, 0, "Stonefire Tavern");
        player->SEND_GOSSIP_MENU(2768, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Mailbox
    {
        player->SEND_POI(-4845.7, -880.55, 6, 6, 0, "Ironforge Mailbox");
        player->SEND_GOSSIP_MENU(2769, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Stable Master
    {
        player->SEND_POI(-5010.2, -1262, 6, 6, 0, "Ulbrek Firehand");
        player->SEND_GOSSIP_MENU(5986, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Weapons Trainer
    {
        player->SEND_POI(-5040, -1201.88, 6, 6, 0, "Bixi and Buliwyf");
        player->SEND_GOSSIP_MENU(4518, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Battlemaster
    {
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH    , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
        player->SEND_GOSSIP_MENU(7529, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Class Trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER           , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST           , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE            , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->SEND_GOSSIP_MENU(2766, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Profession Trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING    , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING   , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
        player->SEND_GOSSIP_MENU(2793, _Creature->GetGUID());
    }
}

void SendBattleMasterMenu_guard_ironforge(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//AV
    {
        player->SEND_POI(-5047.87, -1263.77, 6, 6, 0, "Glordrum Steelbeard");
        player->SEND_GOSSIP_MENU(7483, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//AB
    {
        player->SEND_POI(-5038.37, -1266.39, 6, 6, 0, "Donal Osgood");
        player->SEND_GOSSIP_MENU(7649, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//WSG
    {
        player->SEND_POI(-5037.24, -1274.82, 6, 6, 0, "Lylandris");
        player->SEND_GOSSIP_MENU(7528, _Creature->GetGUID());
    }
}

void SendClassTrainerMenu_guard_ironforge(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Hunter
    {
        player->SEND_POI(-5023, -1253.68, 6, 6, 0, "Hall of Arms");
        player->SEND_GOSSIP_MENU(2770, _Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Mage
    {
        player->SEND_POI(-4627, -926.45, 6, 6, 0, "Hall of Mysteries");
        player->SEND_GOSSIP_MENU(2771, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Paladin
    {
        player->SEND_POI(-4627.02, -926.45, 6, 6, 0, "Hall of Mysteries");
        player->SEND_GOSSIP_MENU(2773, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Priest
    {
        player->SEND_POI(-4627, -926.45, 6, 6, 0, "Hall of Mysteries");
        player->SEND_GOSSIP_MENU(2772, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Rogue
    {
        player->SEND_POI(-4647.83, -1124, 6, 6, 0, "Ironforge Rogue Trainer");
        player->SEND_GOSSIP_MENU(2774, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Warlock
    {
        player->SEND_POI(-4605, -1110.45, 6, 6, 0, "Ironforge Warlock Trainer");
        player->SEND_GOSSIP_MENU(2775, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Warrior
    {
        player->SEND_POI(-5023.08, -1253.68, 6, 6, 0, "Hall of Arms");
        player->SEND_GOSSIP_MENU(2776, _Creature->GetGUID());
    }
}

void SendProfTrainerMenu_guard_ironforge(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(-4858.5, -1241.83, 6, 6, 0, "Berryfizz's Potions and Mixed Drinks");
        player->SEND_GOSSIP_MENU(2794, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Blacksmithing
    {
        player->SEND_POI(-4796.97, -1110.17, 6, 6, 0, "The Great Forge");
        player->SEND_GOSSIP_MENU(2795, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Cooking
    {
        player->SEND_POI(-4767.83, -1184.59, 6, 6, 0, "The Bronze Kettle");
        player->SEND_GOSSIP_MENU(2796, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Enchanting
    {
        player->SEND_POI(-4803.72, -1196.53, 6, 6, 0, "Thistlefuzz Arcanery");
        player->SEND_GOSSIP_MENU(2797, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Engineering
    {
        player->SEND_POI(-4799.56, -1250.23, 6, 6, 0, "Springspindle's Gadgets");
        player->SEND_GOSSIP_MENU(2798, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//First Aid
    {
        player->SEND_POI(-4881.6, -1153.13, 6, 6, 0, "Ironforge Physician");
        player->SEND_GOSSIP_MENU(2799, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Fishing
    {
        player->SEND_POI(-4597.91, -1091.93, 6, 6, 0, "Traveling Fisherman");
        player->SEND_GOSSIP_MENU(2800, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Herbalism
    {
        player->SEND_POI(-4876.9, -1151.92, 6, 6, 0, "Ironforge Physician");
        player->SEND_GOSSIP_MENU(2801, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Leatherworking
    {
        player->SEND_POI(-4745, -1027.57, 6, 6, 0, "Finespindle's Leather Goods");
        player->SEND_GOSSIP_MENU(2802, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Minning
    {
        player->SEND_POI(-4705.06, -1116.43, 6, 6, 0, "Deepmountain Mining Guild");
        player->SEND_GOSSIP_MENU(2804, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Skinning
    {
        player->SEND_POI(-4745, -1027.57, 6, 6, 0, "Finespindle's Leather Goods");
        player->SEND_GOSSIP_MENU(2805, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Tailoring
    {
        player->SEND_POI(-4719.60, -1056.96, 6, 6, 0, "Stonebrow's Clothier");
        player->SEND_GOSSIP_MENU(2807, _Creature->GetGUID());
    }
}

bool GossipSelect_guard_ironforge(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_ironforge(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_ironforge(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_ironforge(player, _Creature, action);
    
    if (sender == GOSSIP_SENDER_SEC_BATTLEINFO)
        SendBattleMasterMenu_guard_ironforge(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU
 *******************************************************/

CreatureAI* GetAI_guard_ironforge(Creature *_Creature)
{
    return new guardAI (_Creature);
}

void AddSC_guard_ironforge()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_ironforge";
    newscript->pGossipHello          = &GossipHello_guard_ironforge;
    newscript->pGossipSelect         = &GossipSelect_guard_ironforge;
    newscript->GetAI = GetAI_guard_ironforge;

    m_scripts[nrscripts++] = newscript;
}