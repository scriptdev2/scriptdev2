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

bool GossipHello_guard_silvermoon(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN            , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_WINDRIDER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_silvermoon(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Auction house
    {
        player->SEND_POI(9672.73, -7528.61, 6, 6, 0, "Silvermoon Auction House");//16628
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Bank
    {
        player->SEND_POI(9808.4, -7488.16, 6, 6, 0, "Silvermoon Bank");//16616
        player->SEND_GOSSIP_MENU(9322, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Guild master
    {
        player->SEND_POI(9474.97, -7345.21, 6, 6, 0, "Silvermoon Guild Master");//16568
        player->SEND_GOSSIP_MENU(9324, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Inn
    {
        player->SEND_POI(9683.82, -7365.19, 6, 6, 0, "Silvermoon Inn");//16618
        player->SEND_GOSSIP_MENU(9325, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Mailbox
    {
        player->SEND_POI(9658.33, -7492.17, 6, 6, 0, "Silvermoon Mailbox");//go 181980
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Stable master
    {
        player->SEND_POI(9904.95, -7404.31, 6, 6, 0, "Shalenn");//16656
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Weapon trainer
    {
        player->SEND_POI(9841.17, -7505.13, 6, 6, 0, "Ileda");//16621
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Wind master
    {
        player->SEND_POI(9378.45, -7163.94, 6, 6, 0, "Silvermoon Wind Master");//16192
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Battlemaster
    {
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARENA           , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_EYEOFTHESTORM   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 4);        
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 5);
        
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Class trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                 , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);

        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Profession trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_JEWELCRAFTING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING               , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 13);

        player->SEND_GOSSIP_MENU(9338, _Creature->GetGUID());
    }
}

void SendBattleMasterMenu_guard_silvermoon(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//AV
    {
        player->SEND_POI(9850.49, -7572.26, 6, 6, 0, "Gurak");//16695
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//AB
    {
        player->SEND_POI(9857.18, -7564.36, 6, 6, 0, "Karen Wentworth");//16694
        player->SEND_GOSSIP_MENU(9316,  _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//A
    {
        player->SEND_POI(9850.6, -7559.25, 6, 6, 0, "Bipp Glizzitor");//19923
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//EOS
    {
        player->SEND_POI(9857.18, -7564.36, 6, 6, 0, "Karen Wentworth");//16694
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//WSG
    {
        player->SEND_POI(9845.45, -7562.58, 6, 6, 0, "Krukk");//16696
        player->SEND_GOSSIP_MENU(1, _Creature->GetGUID());
    }

}

void SendClassTrainerMenu_guard_silvermoon(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Druid
    {
        player->SEND_POI(9700.55, -7262.57, 6, 6, 0, "Silvermoon Druid Trainer");//16655
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }
    
    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Hunter
    {
        player->SEND_POI(9927.48, -7426.14, 6, 6, 0, "Silvermoon Hunter Trainer");//16674
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }
    
    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Mage
    {
        player->SEND_POI(9995.07, -7118.17, 6, 6, 0, "Silvermoon Mage Trainer");//16653
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Paladin
    {
        player->SEND_POI(9850.22, -7516.93, 6, 6, 0, "Silvermoon Paladin Trainer");//16681
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Priest
    {
        player->SEND_POI(9926.79, -7066.66, 6, 6, 0, "Silvermoon Priest Trainer");//16658
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Rogue
    {
        player->SEND_POI(9739.88, -7374.33, 6, 6, 0, "Silvermoon Rogue Trainer");//16684
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Warlock
    {
        player->SEND_POI(9787.57, -7284.63, 6, 6, 0, "Silvermoon Warlock Trainer");//16648
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }
}

void SendProfTrainerMenu_guard_silvermoon(Player *player, Creature *_Creature, uint32 action)
{
    
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(9998.09, -7214.36, 6, 6, 0, "Silvermoon Alchemy Trainer");//16642 isn't spawned, so use 16641-Melaris
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Blacksmithing
    {
        player->SEND_POI(9841.43, -7361.53, 6, 6, 0, "Silvermoon Blacksmithing Trainer");//16669
        player->SEND_GOSSIP_MENU(9340, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Cooking
    {
        player->SEND_POI(9577.26, -7243.6, 6, 6, 0, "Silvermoon Cooking Trainer");//16676
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Enchanting
    {
        player->SEND_POI(9962.57, -7246.18, 6, 6, 0, "Silvermoon Enchanting Trainer");//16633
        player->SEND_GOSSIP_MENU(9341, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Engineering
    {
        player->SEND_POI(9820.18, -7329.56, 6, 6, 0, "Silvermoon Engineering Trainer");//16667
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//First Aid
    {
        player->SEND_POI(9579.8, -7343.71, 6, 6, 0, "Silvermoon First Aid Trainer");//16662
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Fishing
    {
        player->SEND_POI(9607.73, -7324.3, 6, 6, 0, "Silvermoon Fishing Trainer");//16780
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Jewelcrafting
    {
        player->SEND_POI(9553.54, -7506.43, 6, 6, 0, "Silvermoon Jewelcrafting Trainer");//16702-Telia isn't spawned, so use 16703-Amin
        player->SEND_GOSSIP_MENU(9346, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Herbalism
    {
        player->SEND_POI(10004.4, -7216.86, 6, 6, 0, "Silvermoon Herbalism Trainer");//16644 and 22023 :)
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Leatherworking
    {
        player->SEND_POI(9503.72, -7430.16, 6, 6, 0, "Silvermoon Leatherworking Trainer");//16688
        player->SEND_GOSSIP_MENU(9347, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Mining
    {
        player->SEND_POI(9805.1, -7355.56, 6, 6, 0, "Silvermoon Mining Trainer");//16663
        player->SEND_GOSSIP_MENU(9348, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Skinning
    {
        player->SEND_POI(9513.37, -7429.4, 6, 6, 0, "Silvermoon Skinning Trainer");//16692
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 13)//Tailoring
    {
        player->SEND_POI(9750.55, -7095.28, 6, 6, 0, "Silvermoon Tailor");//16640
        player->SEND_GOSSIP_MENU(9350, _Creature->GetGUID());
    }

}

bool GossipSelect_guard_silvermoon(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_silvermoon(player, _Creature, action);


    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_silvermoon(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_silvermoon(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_BATTLEINFO)
        SendBattleMasterMenu_guard_silvermoon(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU
 *******************************************************/

CreatureAI* GetAI_guard_silvermoon(Creature *_Creature)
{
    return new guardAI (_Creature);
}


void AddSC_guard_silvermoon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_silvermoon";
    newscript->pGossipHello          = &GossipHello_guard_silvermoon;
    newscript->pGossipSelect         = &GossipSelect_guard_silvermoon;
    newscript->GetAI = GetAI_guard_silvermoon;

    m_scripts[nrscripts++] = newscript;
}
