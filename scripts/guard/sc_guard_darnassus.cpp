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

bool GossipHello_guard_darnassus(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_HIPPOGRYPH     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN            , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->SEND_GOSSIP_MENU(3016, _Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_darnassus(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Auction house
    {
        player->SEND_POI(9861.23, 2334.55, 6, 6, 0, "Darnassus Auction House");
        player->SEND_GOSSIP_MENU(3833, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Bank
    {
        player->SEND_POI(9938.45, 2512.35, 6, 6, 0, "Darnassus Bank");
        player->SEND_GOSSIP_MENU(3017, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Wind master
    {
        player->SEND_POI(9945.65, 2618.94, 6, 6, 0, "Rut'theran Village");
        player->SEND_GOSSIP_MENU(3018, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Guild master
    {
        player->SEND_POI(10076.40, 2199.59, 6, 6, 0, "Darnassus Guild Master");
        player->SEND_GOSSIP_MENU(3019, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Inn
    {
        player->SEND_POI(10133.29, 2222.52, 6, 6, 0, "Darnassus Inn");
        player->SEND_GOSSIP_MENU(3020, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Mailbox
    {
        player->SEND_POI(9942.17, 2495.48, 6, 6, 0, "Darnassus Mailbox");
        player->SEND_GOSSIP_MENU(3021, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Stable master
    {
        player->SEND_POI(10167.20, 2522.66, 6, 6, 0, "Alassin");
        player->SEND_GOSSIP_MENU(5980, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Weapon trainer
    {
        player->SEND_POI(9907.11, 2329.70, 6, 6, 0, "Ilyenia Moonfire");
        player->SEND_GOSSIP_MENU(4517, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Battlemaster
    {
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH    , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
        
        player->SEND_GOSSIP_MENU(7519, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Class trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);

        player->SEND_GOSSIP_MENU(4264, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Profession trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);

        player->SEND_GOSSIP_MENU(4273, _Creature->GetGUID());
    }
}

void SendBattleMasterMenu_guard_darnassus(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//AV
    {
        player->SEND_POI(9923.61, 2327.43, 6, 6, 0, "Brogun Stoneshield");
        player->SEND_GOSSIP_MENU(7518, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//AB
    {
        player->SEND_POI(9977.37, 2324.39, 6, 6, 0, "Keras Wolfheart");
        player->SEND_GOSSIP_MENU(7651,  _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//WSG
    {
        player->SEND_POI(9979.84, 2315.79, 6, 6, 0, "Aethalas");
        player->SEND_GOSSIP_MENU(7482, _Creature->GetGUID());
    }
}

void SendClassTrainerMenu_guard_darnassus(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Druid
    {
        player->SEND_POI(10186, 2570.46, 6, 6, 0, "Darnassus Druid Trainer");
        player->SEND_GOSSIP_MENU(3024, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Hunter
    {
        player->SEND_POI(10177.29, 2511.10, 6, 6, 0, "Darnassus Hunter Trainer");
        player->SEND_GOSSIP_MENU(3023, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Priest
    {
        player->SEND_POI(9659.12, 2524.88, 6, 6, 0, "Temple of the Moon");
        player->SEND_GOSSIP_MENU(3025, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Rogue
    {
        player->SEND_POI(10122, 2599.12, 6, 6, 0, "Darnassus Rogue Trainer");
        player->SEND_GOSSIP_MENU(3026, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Warrior
    {
        player->SEND_POI(9951.91, 2280.38, 6, 6, 0, "Warrior's Terrace");
        player->SEND_GOSSIP_MENU(3033, _Creature->GetGUID());
    }
}

void SendProfTrainerMenu_guard_darnassus(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(10075.90, 2356.76, 6, 6, 0, "Darnassus Alchemy Trainer");
        player->SEND_GOSSIP_MENU(3035, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Cooking
    {
        player->SEND_POI(10088.59, 2419.21, 6, 6, 0, "Darnassus Cooking Trainer");
        player->SEND_GOSSIP_MENU(3036, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Enchanting
    {
        player->SEND_POI(10146.09, 2313.42, 6, 6, 0, "Darnassus Enchanting Trainer");
        player->SEND_GOSSIP_MENU(3337, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//First Aid
    {
        player->SEND_POI(10150.09, 2390.43, 6, 6, 0, "Darnassus First Aid Trainer");
        player->SEND_GOSSIP_MENU(3037, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Fishing
    {
        player->SEND_POI(9836.20, 2432.17, 6, 6, 0, "Darnassus Fishing Trainer");
        player->SEND_GOSSIP_MENU(3038, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Herbalism
    {
        player->SEND_POI(9757.17, 2430.16, 6, 6, 0, "Darnassus Herbalism Trainer");
        player->SEND_GOSSIP_MENU(3039, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Leatherworking
    {
        player->SEND_POI(10086.59, 2255.77, 6, 6, 0, "Darnassus Leatherworking Trainer");
        player->SEND_GOSSIP_MENU(3040, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Skinning
    {
        player->SEND_POI(10081.40, 2257.18, 6, 6, 0, "Darnassus Skinning Trainer");
        player->SEND_GOSSIP_MENU(3042, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Tailoring
    {
        player->SEND_POI(10079.70, 2268.19, 6, 6, 0, "Darnassus Tailor");
        player->SEND_GOSSIP_MENU(3044, _Creature->GetGUID());
    }
}

bool GossipSelect_guard_darnassus(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_darnassus(player, _Creature, action);


    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_darnassus(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_darnassus(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_BATTLEINFO)
        SendBattleMasterMenu_guard_darnassus(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU
 *******************************************************/

CreatureAI* GetAI_guard_darnassus(Creature *_Creature)
{
    return new guardAI (_Creature);
}


void AddSC_guard_darnassus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_darnassus";
    newscript->pGossipHello          = &GossipHello_guard_darnassus;
    newscript->pGossipSelect         = &GossipSelect_guard_darnassus;
    newscript->GetAI = GetAI_guard_darnassus;

    m_scripts[nrscripts++] = newscript;
}
