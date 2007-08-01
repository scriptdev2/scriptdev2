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
#include "guard_ai.h"

bool GossipHello_guard_stormwind(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE	  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);  //added
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_STORMWIND_BANK    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_DEEPRUNTRAM		  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);  //added
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN               , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_GRYPHON           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);    
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);  //added
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);  //added
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_OFFICERS          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11); //added
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);

    player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_stormwind(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Auction House
    {
        player->SEND_POI(-8811.46, 667.46, 6, 6, 0, "Stormwind Auction House");
        player->SEND_GOSSIP_MENU(3834,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Bank
    {
        player->SEND_POI(-8916.87, 622.87, 6, 6, 0, "Stormwind Bank");
        player->SEND_GOSSIP_MENU(764,_Creature->GetGUID());
    }


    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Deeprun tram
    {
        player->SEND_POI(-8378.88, 554.23, 6, 6, 0, "The Deeprun Tram");
      player->SEND_GOSSIP_MENU(3813,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Inn
    {
        player->SEND_POI(-8869.0, 675.4, 6, 6, 0, "The Gilded Rose");
        player->SEND_GOSSIP_MENU(3860,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Gryphon Master
    {
        player->SEND_POI(-8837.0, 493.5, 6, 6, 0, "Stormwind Gryphon Master");
        player->SEND_GOSSIP_MENU(879,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Guild Master
    {
        player->SEND_POI(-8894.0, 611.2, 6, 6, 0, "Stormwind Vistor`s Center");
        player->SEND_GOSSIP_MENU(882,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Mailbox
    {
        player->SEND_POI(-8876.48, 649.18, 6, 6, 0, "Stormwind Mailbox");
        player->SEND_GOSSIP_MENU(3861,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Stable Master
    {
        player->SEND_POI(-8433.0, 554.7, 6, 6, 0, "Jenova Stoneshield");
        player->SEND_GOSSIP_MENU(5984,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Weapon Trainer

    {
        player->SEND_POI(-8797.0, 612.8, 6, 6, 0, "Woo Ping");
        player->SEND_GOSSIP_MENU(4516,_Creature->GetGUID());
    }


    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Officers Lounge
    {
        player->SEND_POI(-8759.92, 399.69, 6, 6, 0, "Champions` Hall");
        player->SEND_GOSSIP_MENU(7047,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Battlemasters
    {
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY        , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN          , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH          , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);

        player->SEND_GOSSIP_MENU(7499,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Class trainers
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                 , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 9);

        player->SEND_GOSSIP_MENU(4264,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 13)//Profession trainers
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING               , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);

        player->SEND_GOSSIP_MENU(4273,_Creature->GetGUID());
    }
}
void SendBattleMasterMenu_guard_stormwind(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//AV
    {
        player->SEND_POI(-8443.88, 335.99, 6, 6, 0, "Thelman Slatefist");
        player->SEND_GOSSIP_MENU(7500, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//AB
    {
        player->SEND_POI(-8443.88, 335.99, 6, 6, 0, "Lady Hoteshem");
        player->SEND_GOSSIP_MENU(7650, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//WSG
    {
        player->SEND_POI(-8443.88, 335.99, 6, 6, 0, "Elfarran");
        player->SEND_GOSSIP_MENU(7501, _Creature->GetGUID());
    }
}
void SendClassTrainerMenu_guard_stormwind(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Mage
    {
        player->SEND_POI(-9012.0, 867.6, 6, 6, 0, "Wizard`s Sanctum");
        player->SEND_GOSSIP_MENU(899,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Rogue
    {
        player->SEND_POI(-8753.0, 367.8, 6, 6, 0, "Stormwind - Rogue House");
        player->SEND_GOSSIP_MENU(900,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Warrior
    {
        player->SEND_POI(-8624.54, 402.61, 6, 6, 0, "Pig and Whistle Tavern");
        player->SEND_GOSSIP_MENU(901,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Druid
    {
        player->SEND_POI(-8751.0, 1124.5, 6, 6, 0, "The Park");
        player->SEND_GOSSIP_MENU(902,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Priest
    {
        player->SEND_POI(-8512.0, 862.4, 6, 6, 0, "Catedral Of Light");
        player->SEND_GOSSIP_MENU(903,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Paladin
    {
        player->SEND_POI(-8577.0, 881.7, 6, 6, 0, "Catedral Of Light");
        player->SEND_GOSSIP_MENU(904,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Hunter
    {
        player->SEND_POI(-8413.0, 541.5, 6, 6, 0, "Hunter Lodge");
        player->SEND_GOSSIP_MENU(905,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Warlock
    {
        player->SEND_POI(-8948.91, 998.35, 6, 6, 0, "The Slaughtered Lamb");
        player->SEND_GOSSIP_MENU(906,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Shaman
    {
        player->SEND_POI(-9033, 550, 6, 6, 0, "Valley Of Heroes");
        player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());//incorrect id
    }
}

void SendProfTrainerMenu_guard_stormwind(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(-8988.0, 759.60, 6, 6, 0, "Alchemy Needs");
        player->SEND_GOSSIP_MENU(919,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Blacksmithing
    {
        player->SEND_POI(-8424.0, 616.9, 6, 6, 0, "Therum Deepforge");
        player->SEND_GOSSIP_MENU(920,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Cooking
    {
        player->SEND_POI(-8611.0, 364.6, 6, 6, 0, "Pig and Whistle Tavern");
        player->SEND_GOSSIP_MENU(921,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Enchanting
    {
        player->SEND_POI(-8858.0, 803.7, 6, 6, 0, "Lucan Cordell");
        player->SEND_GOSSIP_MENU(941,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Engineering
    {
        player->SEND_POI(-8347.0, 644.1, 6, 6, 0, "Lilliam Sparkspindle");
        player->SEND_GOSSIP_MENU(922,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//First Aid
    {
        player->SEND_POI(-8513.0, 801.8, 6, 6, 0, "Shaina Fuller");
        player->SEND_GOSSIP_MENU(923,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Fishing
    {
        player->SEND_POI(-8803.0, 767.5, 6, 6, 0, "Arnold Leland");
        player->SEND_GOSSIP_MENU(940,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Herbalism
    {
        player->SEND_POI(-8967.0, 779.5, 6, 6, 0, "Alchemy Needs");
        player->SEND_GOSSIP_MENU(924,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Leatherworking
    {
        player->SEND_POI(-8726.0, 477.4, 6, 6, 0, "The Protective Hide");
        player->SEND_GOSSIP_MENU(925,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Mining
    {
        player->SEND_POI(-8434.0, 692.8, 6, 6, 0, "Gelman Stonehand");
        player->SEND_GOSSIP_MENU(927,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Skinning
    {
        player->SEND_POI(-8716.0, 469.4, 6, 6, 0, "The Protective Hide");
        player->SEND_GOSSIP_MENU(928,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Tailoring
    {
        player->SEND_POI(-8938.0, 800.7, 6, 6, 0, "Duncan`s Textiles");
        player->SEND_GOSSIP_MENU(929,_Creature->GetGUID());
    }
}

bool GossipSelect_guard_stormwind(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_stormwind(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_stormwind(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_stormwind(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_BATTLEINFO)
        SendBattleMasterMenu_guard_stormwind(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU
 *******************************************************/

CreatureAI* GetAI_guard_stormwind(Creature *_Creature)
{
    return new guardAI (_Creature);
}

void AddSC_guard_stormwind()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_stormwind";
    newscript->pGossipHello          = &GossipHello_guard_stormwind;
    newscript->pGossipSelect         = &GossipSelect_guard_stormwind;
    newscript->GetAI = GetAI_guard_stormwind;

    m_scripts[nrscripts++] = newscript;
}
