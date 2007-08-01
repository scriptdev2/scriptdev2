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

//script spesific action
#define GOSSIP_ACTION_TAVERN                  101
#define GOSSIP_ACTION_GEMMERCHANT             102
#define GOSSIP_ACTION_MANALOOM                103

//script spesific sender
#define GOSSIP_SENDER_SEC_GEMMERCHANT         101

//script spesific gossip text
#define GOSSIP_TEXT_TAVERN                    "Worlds End Tavern"
#define GOSSIP_TEXT_BANKSCYERS                "Scyers bank"
#define GOSSIP_TEXT_BANKALDOR                 "Aldor Bank"
#define GOSSIP_TEXT_INNSCYERS                 "Scyers Inn"
#define GOSSIP_TEXT_INNALDOR                  "Aldor Inn"
#define GOSSIP_TEXT_MAILBOXSCYERS             "Scyers Mailbox"
#define GOSSIP_TEXT_MAILBOXALDOR              "Aldor Mailbox"
#define GOSSIP_TEXT_STABLESCYERS              "Scyers Stable Master"
#define GOSSIP_TEXT_STABLEALDOR               "Aldor Stable Master"
#define GOSSIP_TEXT_BATTLEMASTERALLIANCE      "Alliance Battlemasters"
#define GOSSIP_TEXT_BATTLEMASTERHORDE         "Horde And Arena Battlemasters"
#define GOSSIP_TEXT_MANALOOM                  "Mana Loom"
#define GOSSIP_TEXT_ALCHEMYLAB                "Alchemy Lab"
#define GOSSIP_TEXT_GEMMERCHANT               "Gem Merchant"
#define GOSSIP_TEXT_GEMSCYERS                 "Scyers Gem Merchant"
#define GOSSIP_TEXT_GEMALDOR                  "Aldor Gem Merchant"

bool GossipHello_guard_shattrath(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_TAVERN                       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK                         , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN                          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_FLIGHTMASTER                 , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX                      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER                 , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER                 , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER                  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_MANALOOM                     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_ALCHEMYLAB                   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GEMMERCHANT                  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    
    player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());

    return true;
}

/*******************************************************
 *                Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                                 //Tavern
            player->SEND_POI(-1759.5, 5165, 6, 6, 0, "Worlds End Tavern");
            player->SEND_GOSSIP_MENU(10394, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 2:                                 //Bank
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BANKSCYERS           , GOSSIP_SENDER_SEC_BANK, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BANKALDOR            , GOSSIP_SENDER_SEC_BANK, GOSSIP_ACTION_INFO_DEF + 2);
        
            player->SEND_GOSSIP_MENU(10379, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 3:                                 //Inn
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_INNSCYERS            , GOSSIP_SENDER_SEC_INN, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_INNALDOR             , GOSSIP_SENDER_SEC_INN, GOSSIP_ACTION_INFO_DEF + 2);
        
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 4:                                 //Flight master
            player->SEND_POI(-1832, 5299, 6, 6, 0, "Shattrath Flight Master");//18940
            player->SEND_GOSSIP_MENU(10385, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 5:                                 //Mailbox
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_MAILBOXSCYERS        , GOSSIP_SENDER_SEC_MAILBOX, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_MAILBOXALDOR         , GOSSIP_SENDER_SEC_MAILBOX, GOSSIP_ACTION_INFO_DEF + 2);
        
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 6:                                 //Stable master
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_STABLESCYERS         , GOSSIP_SENDER_SEC_STABLEMASTER, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_STABLEALDOR          , GOSSIP_SENDER_SEC_STABLEMASTER, GOSSIP_ACTION_INFO_DEF + 2);
        
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 7:                                 //Battlemaster
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTERALLIANCE , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTERHORDE    , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
        
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 8:                                 //Profession master
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_JEWELCRAFTING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);

            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 9:                                 //Mana Loom
            player->SEND_POI(-2070, 5265.5, 6, 6, 0, "Mana Loom");       //GO in the Lower City
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 10:                                //Alchemy Lab
            player->SEND_POI(-1648.5, 5540, 6, 6, 0, "Alchemy Lab");     //GO in the Lower City, arokkar tower
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 11:                                //Gem Merchant
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_GEMSCYERS            , GOSSIP_SENDER_SEC_GEMMERCHANT, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_GEMALDOR             , GOSSIP_SENDER_SEC_GEMMERCHANT, GOSSIP_ACTION_INFO_DEF + 2);

            player->SEND_GOSSIP_MENU(10697, _Creature->GetGUID());
            break;
    }
}

void SendBankMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-1997.7, 5363, 6, 6, 0, "Scyers Bank");     //19246, 19338
            player->SEND_GOSSIP_MENU(10379, _Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1730.5, 5496, 6, 6, 0, "Aldor Bank");      //19318, 19034
            player->SEND_GOSSIP_MENU(10380,  _Creature->GetGUID());
    }
}

void SendInnMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-2178, 5405, 6, 6, 0, "Scyers Inn");        //19232
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1895, 5767, 6, 6, 0, "Aldor Inn");         //19046
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
    }
}

void SendMailboxMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-2028.5, 5347.5, 6, 6, 0, "Scyers Mailbox");//in bank
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1701, 5510.5, 6, 6, 0, "Aldor Mailbox");   //in bank
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
    }
}

void SendStableMasterMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-2170, 5404, 6, 6, 0, "Scyers Stable Master");//21517
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1888.5, 5761, 6, 6, 0, "Aldor Stable Master");//21518
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
    }
}

void SendBattleMasterMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)                            //battlemaster, ally
    {
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY        , GOSSIP_SENDER_SUB1_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN          , GOSSIP_SENDER_SUB1_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH          , GOSSIP_SENDER_SUB1_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_EYEOFTHESTORM        , GOSSIP_SENDER_SUB1_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 4);

            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)                            //battlemaster, horde and arena
    {
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY        , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN          , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH          , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_EYEOFTHESTORM        , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARENA                , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 5);

            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
    }
}

void SendBattleMasterSub1Menu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->SEND_POI(-1774, 5251, 6, 6, 0, "Alterac Valley Battle Master");//20271
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 2:
            player->SEND_POI(-1774, 5251, 6, 6, 0, "Arathi Basin Battle Master");//20273
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 3:
            player->SEND_POI(-1774, 5251, 6, 6, 0, "Warsong Gulch Battle Master");//20272
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 4:
            player->SEND_POI(-1774, 5251, 6, 6, 0, "Eye of The Storm Battle Master");//20362
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;
    }
}

void SendBattleMasterSub2Menu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Alterac Valley Battle Master");//20276
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 2:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Arathi Basin Battle Master");//20274
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 3:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Warsong Gulch Battle Master");//20269
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 4:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Eye of The Storm Battle Master");//20384
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 5:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Arena Battle Master");//21235
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                                 //Alchemy
            player->SEND_POI(-1648.5, 5534, 6, 6, 0, "Shattrath Alchemy Trainer");//19052
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 2:                                 //Blacksmithing
            player->SEND_POI(-1847, 5222, 6, 6, 0, "Shattrath Blacksmithing Trainers");//20125,20124
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 3:                                 //Cooking
            player->SEND_POI(-2069, 5309.5, 6, 6, 0, "Shattrath Cooking Trainer");//19185
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 4:                                 //Enchanting
            player->SEND_POI(-2263.5, 5563.5, 6, 6, 0, "Shattrath Enchanting Trainers");//19252,19250,19249,19248
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 5:                                 //First Aid
            player->SEND_POI(-1591, 5265.5, 6, 6, 0, "Shattrath First Aid Trainer");//19184
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 6:                                 //Jewelcrafting
            player->SEND_POI(-1654, 5667.5, 6, 6, 0, "Shattrath Jewelcrafting Trainer");//19063
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 7:                                 //Leatherworking
            player->SEND_POI(-2060.5, 5256.5, 6, 6, 0, "Shattrath Leatherworking Trainer");//19187
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF + 8:                                 //Skinning
            player->SEND_POI(-2048, 5300, 6, 6, 0, "Shattrath Skinning Trainer");//19180
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
    }
}

void SendGemMerchantMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-2193, 5424.5, 6, 6, 0, "Scyers Gem Merchant");//19330
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1645, 5669.5, 6, 6, 0, "Aldor Gem Merchant");//19065
            player->SEND_GOSSIP_MENU(10698,  _Creature->GetGUID());
    }
}

bool GossipSelect_guard_shattrath(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:              SendDefaultMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BANK:          SendBankMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_INN:           SendInnMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_STABLEMASTER:  SendStableMasterMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_GEMMERCHANT:   SendGemMerchantMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_MAILBOX:       SendMailboxMenu_guard_shattrath(player, _Creature, action);    
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:     SendProfTrainerMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:    SendBattleMasterMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SUB1_BATTLEINFO:   SendBattleMasterSub1Menu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SUB2_BATTLEINFO:   SendBattleMasterSub2Menu_guard_shattrath(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 *                End of GOSSIP_MENU
 *******************************************************/

CreatureAI* GetAI_guard_shattrath(Creature *_Creature)
{
    return new guardAI (_Creature);
}

void AddSC_guard_shattrath()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_shattrath";
    newscript->pGossipHello          = &GossipHello_guard_shattrath;
    newscript->pGossipSelect         = &GossipSelect_guard_shattrath;
    newscript->GetAI = GetAI_guard_shattrath;

    m_scripts[nrscripts++] = newscript;
}
