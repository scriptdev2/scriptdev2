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

bool GossipHello_guard_bluffwatcher(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK         , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_WINDRIDER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->SEND_GOSSIP_MENU(3543,_Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Bank
    {
        player->SEND_POI(-1257.8, 24.14, 6, 6, 0, "Thunder Bluff Bank");
        player->SEND_GOSSIP_MENU(1292,_Creature->GetGUID());
   }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Wind master
    {
        player->SEND_POI(-1196.43, 28.26, 6, 6, 0, "Wind Rider Roost");
        player->SEND_GOSSIP_MENU(1293,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Guild master
    {
        player->SEND_POI(-1296.5, 127.57, 6, 6, 0, "Thunder Bluff Civic Information");
        player->SEND_GOSSIP_MENU(1291,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Inn
    {
        player->SEND_POI(-1296, 39.7, 6, 6, 0, "Thunder Bluff Inn");
        player->SEND_GOSSIP_MENU(3153,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Mailbox
    {
        player->SEND_POI(-1263.59, 44.36, 6, 6, 0, "Thunder Bluff Mailbox");
        player->SEND_GOSSIP_MENU(3154,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Auction House
    {
        player->SEND_POI(1381.77, -4371.16, 6, 6, 0, GOSSIP_TEXT_AUCTIONHOUSE);
        player->SEND_GOSSIP_MENU(3155,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Weapon master
    {
        player->SEND_POI(-1282.31, 89.56, 6, 6, 0, "Ansekhwa");
        player->SEND_GOSSIP_MENU(4520,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Stable master
    {
        player->SEND_POI(-1270.19, 48.84, 6, 6, 0, "Bulrug");
        player->SEND_GOSSIP_MENU(5977,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//battlemaster
    {
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY       , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN         , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH        , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
        player->SEND_GOSSIP_MENU(7527,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Class trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                 , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->SEND_GOSSIP_MENU(3542,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Profession trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING               , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
        player->SEND_GOSSIP_MENU(3541,_Creature->GetGUID());
    }
}

void SendBattleMasterMenu_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//AV
    {
        player->SEND_POI(-1387.82, -97.55, 6, 6, 0, "Taim Ragetotem");
        player->SEND_GOSSIP_MENU(7522,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//AB
    {
        player->SEND_POI(-997, 214.12, 6, 6, 0, "Martin Lindsey");
        player->SEND_GOSSIP_MENU(7648,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//WSG
    {
        player->SEND_POI(-1384.94, -75.91, 6, 6, 0, "Kergul Bloodaxe");
        player->SEND_GOSSIP_MENU(7523,_Creature->GetGUID());
    }
}
//
void SendClassTrainerMenu_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Druid
    {
        player->SEND_POI(-1054.47, -285, 6, 6, 0, "Hall of Elders");
        player->SEND_GOSSIP_MENU(1294,_Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Hunter
    {
        player->SEND_POI(-1416.32, -114.28, 6, 6, 0, "Hunter's Hall");
        player->SEND_GOSSIP_MENU(1295,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Mage
    {
       player->SEND_POI(-1061.2, 195.5, 6, 6, 0, "Pools of Vision");
       player->SEND_GOSSIP_MENU(1296,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Priest
    {
        player->SEND_POI(-1061.2, 195.5, 6, 6, 0, "Pools of Vision");
        player->SEND_GOSSIP_MENU(1297,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Shaman
    {
        player->SEND_POI(-989.54, 278.25, 6, 6, 0, "Hall of Spirits");
        player->SEND_GOSSIP_MENU(1298,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Warrior
    {
        player->SEND_POI(-1416.32, -114.28, 6, 6, 0, "Hunter's Hall");
        player->SEND_GOSSIP_MENU(1299,_Creature->GetGUID());
    }
}

void SendProfTrainerMenu_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(-1085.56, 27.29, 6, 6, 0, "Bena's Alchemy");
        player->SEND_GOSSIP_MENU(1332,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Blacksmithing
    {
        player->SEND_POI(-1239.75, 104.88, 6, 6, 0, "Karn's Smithy");
        player->SEND_GOSSIP_MENU(1333,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Cooking
    {
        player->SEND_POI(-1214.5, -21.23, 6, 6, 0, "Aska's Kitchen");
        player->SEND_GOSSIP_MENU(1334,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Enchanting
    {
        player->SEND_POI(-1112.65, 48.26, 6, 6, 0, "Dawnstrider Enchanters");
        player->SEND_GOSSIP_MENU(1335,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//First Aid
    {
        player->SEND_POI(-996.58, 200.5, 6, 6, 0, "Spiritual Healing");
        player->SEND_GOSSIP_MENU(1336,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Fishing
    {
        player->SEND_POI(-1169.35, -68.87, 6, 6, 0, "Mountaintop Bait & Tackle");
        player->SEND_GOSSIP_MENU(1337,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Herbalism
    {
        player->SEND_POI(-1137.7, -1.51, 6, 6, 0, "Holistic Herbalism");
        player->SEND_GOSSIP_MENU(1338,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Leatherworking
    {
        player->SEND_POI(-1156.22, 66.86, 6, 6, 0, "Thunder Bluff Armorers");
        player->SEND_GOSSIP_MENU(1339,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Mining
    {
        player->SEND_POI(-1249.17, 155, 6, 6, 0, "Stonehoof Geology");
        player->SEND_GOSSIP_MENU(1340,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Skinning
    {
        player->SEND_POI(-1148.56, 51.18, 6, 6, 0, "Mooranta");
        player->SEND_GOSSIP_MENU(1343,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Tailoring
    {
        player->SEND_POI(-1156.22, 66.86, 6, 6, 0, "Thunder Bluff Armorers");
        player->SEND_GOSSIP_MENU(1341,_Creature->GetGUID());
    }
}

bool GossipSelect_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_bluffwatcher(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_bluffwatcher(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_bluffwatcher(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_BATTLEINFO)
        SendBattleMasterMenu_guard_bluffwatcher(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU
 *******************************************************/

CreatureAI* GetAI_guard_bluffwatcher(Creature *_Creature)
{
    return new guardAI (_Creature);
}

void AddSC_guard_bluffwatcher()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_bluffwatcher";
    newscript->pGossipHello          = &GossipHello_guard_bluffwatcher;
    newscript->pGossipSelect         = &GossipSelect_guard_bluffwatcher;
    newscript->GetAI = GetAI_guard_bluffwatcher;

    m_scripts[nrscripts++] = newscript;
}