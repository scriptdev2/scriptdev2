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

bool GossipHello_guard_undercity(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_BATHANDLER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_ZEPPLINMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);

    player->SEND_GOSSIP_MENU(3543,_Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU section
 *******************************************************/

void SendDefaultMenu_guard_undercity(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Bank
    {
        player->SEND_POI(1595.64, 232.45, 6, 6, 0, "Undercity Bank");
        player->SEND_GOSSIP_MENU(3514,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Bat handler
    {
        player->SEND_POI(1565.9, 271.43, 6, 6, 0, "Undercity Bat Handler");
        player->SEND_GOSSIP_MENU(3515,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Guild master
    {
        player->SEND_POI(1594.17, 205.57, 6, 6, 0, "Undercity Guild Master");
        player->SEND_GOSSIP_MENU(3516,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Inn
    {
        player->SEND_POI(1639.43, 220.99, 6, 6, 0, "Undercity Inn");
        player->SEND_GOSSIP_MENU(3517,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Mailbox
    {
        player->SEND_POI(1632.68, 219.4, 6, 6, 0, "Undercity Mailbox");
        player->SEND_GOSSIP_MENU(3518,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Auction House
    {
        player->SEND_POI(1647.9, 258.49, 6, 6, 0, "Undercity Auction House");
        player->SEND_GOSSIP_MENU(3519,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Zeppelin
    {
        player->SEND_POI(2059, 274.86, 6, 6, 0, "Undercity Zeppelin");
        player->SEND_GOSSIP_MENU(3520,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Weapon Master
    {
        player->SEND_POI(1670.31, 324.66, 6, 6, 0, "Archibald");
        player->SEND_GOSSIP_MENU(4521,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Stable master
    {
        player->SEND_POI(1634.18, 226.76, 6, 6, 0, "Anya Maulray");
        player->SEND_GOSSIP_MENU(5979,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Battlemaster
    {
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH    , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);

        player->SEND_GOSSIP_MENU(7527,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Class trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE     , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST       , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE     , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK     , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR      , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);

        player->SEND_GOSSIP_MENU(3542,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Profession trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING               , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);

        player->SEND_GOSSIP_MENU(3541,_Creature->GetGUID());
    }
}

void SendBattleMasterMenu_guard_undercity(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//AV
    {
        player->SEND_POI(1329, 333.92, 6, 6, 0, "Grizzle Halfmane");
        player->SEND_GOSSIP_MENU(7525,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//AB
    {
        player->SEND_POI(1283.3, 287.16, 6, 6, 0, "Sir Malory Wheeler");
        player->SEND_GOSSIP_MENU(7646,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//WSG
    {
        player->SEND_POI(1265, 351.18, 6, 6, 0, "Kurden Bloodclaw");
        player->SEND_GOSSIP_MENU(7526,_Creature->GetGUID());
    }
}

void SendClassTrainerMenu_guard_undercity(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Mage
    {
        player->SEND_POI(1781, 53, 6, 6, 0, "Undercity Mage Trainers");
        player->SEND_GOSSIP_MENU(3513,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Priest
    {
        player->SEND_POI(1758.33, 401.5, 6, 6, 0, "Undercity Priest Trainers");
        player->SEND_GOSSIP_MENU(3521,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Rogue
    {
        player->SEND_POI(1418.56, 65, 6, 6, 0, "Undercity Rogue Trainers");
        player->SEND_GOSSIP_MENU(3524,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Warlock
    {
        player->SEND_POI(1780.92, 53.16, 6, 6, 0, "Undercity Warlock Trainers");
        player->SEND_GOSSIP_MENU(3526,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Warrior
    {
        player->SEND_POI(1775.59, 418.19, 6, 6, 0, "Undercity Warrior Trainers");
        player->SEND_GOSSIP_MENU(3527,_Creature->GetGUID());
    }
}

void SendProfTrainerMenu_guard_undercity(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(1419.82, 417.19, 6, 6, 0, "The Apothecarium");
        player->SEND_GOSSIP_MENU(3528,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Blacksmithing
    {
        player->SEND_POI(1696, 285, 6, 6, 0, "Undercity Blacksmithing Trainer");
        player->SEND_GOSSIP_MENU(3529,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Cooking
    {
        player->SEND_POI(1596.34, 274.68, 6, 6, 0, "Undercity Cooking Trainer");
        player->SEND_GOSSIP_MENU(3530,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Enchanting
    {
        player->SEND_POI(1488.54, 280.19, 6, 6, 0, "Undercity Enchanting Trainer");
        player->SEND_GOSSIP_MENU(3531,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Engineering
    {
        player->SEND_POI(1408.58, 143.43, 6, 6, 0, "Undercity Engineering Trainer");
        player->SEND_GOSSIP_MENU(3532,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//First Aid
    {
        player->SEND_POI(1519.65, 167.19, 6, 6, 0, "Undercity First Aid Trainer");
        player->SEND_GOSSIP_MENU(3533,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Fishing
    {
        player->SEND_POI(1679.9, 89, 6, 6, 0, "Undercity Fishing Trainer");
        player->SEND_GOSSIP_MENU(3534,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Herbalism
    {
        player->SEND_POI(1558, 349.36, 6, 6, 0, "Undercity Herbalism Trainer");
        player->SEND_GOSSIP_MENU(3535,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Leatherworking
    {
        player->SEND_POI(1498.76, 196.43, 6, 6, 0, "Undercity Leatherworking Trainer");
        player->SEND_GOSSIP_MENU(3536,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Mining
    {
        player->SEND_POI(1642.88, 335.58, 6, 6, 0, "Undercity Mining Trainer");
        player->SEND_GOSSIP_MENU(3537,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Skinning
    {
        player->SEND_POI(1498.6, 196.46, 6, 6, 0, "Undercity Skinning Trainer");
        player->SEND_GOSSIP_MENU(3538,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Tailoring
    {
        player->SEND_POI(1689.55, 193, 6, 6, 0, "Undercity Tailoring Trainer");
        player->SEND_GOSSIP_MENU(3539,_Creature->GetGUID());
    }
}

bool GossipSelect_guard_undercity(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_undercity(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_undercity(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_undercity(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_BATTLEINFO)
        SendBattleMasterMenu_guard_undercity(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU section
 *******************************************************/

CreatureAI* GetAI_guard_undercity(Creature *_Creature)
{
    return new guardAI (_Creature);
}

void AddSC_guard_undercity()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_undercity";
    newscript->pGossipHello          = &GossipHello_guard_undercity;
    newscript->pGossipSelect         = &GossipSelect_guard_undercity;
    newscript->GetAI = GetAI_guard_undercity;

    m_scripts[nrscripts++] = newscript;
}
