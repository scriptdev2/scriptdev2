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

bool GossipHello_guard_orgrimmar(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_WINDRIDER     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_ZEPPLINMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 9, GOSSIP_TEXT_OFFICERS      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
    player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_orgrimmar(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Bank
    {
        player->SEND_POI(1631.51, -4375.33, 6, 6, 0, "Bank of Orgrimmar");
        player->SEND_GOSSIP_MENU(2554,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//wind rider
    {
        player->SEND_POI(1676.6, -4332.72, 6, 6, 0, "The Sky Tower");
        player->SEND_GOSSIP_MENU(2555,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//guild master
    {
        player->SEND_POI(1576.93, -4294.75, 6, 6, 0, "Horde Embassy");
        player->SEND_GOSSIP_MENU(2556,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Inn
    {
        player->SEND_POI(1644.51, -4447.27, 6, 6, 0, "Orgrimmar Inn");
        player->SEND_GOSSIP_MENU(2557,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//mailbox
    {
        player->SEND_POI(1622.53, -4388.79, 6, 6, 0, "Orgrimmar Mailbox");
        player->SEND_GOSSIP_MENU(2558,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//auction house
    {
        player->SEND_POI(1679.21, -4450.1, 6, 6, 0, "Orgrimmar Auction House");
        player->SEND_GOSSIP_MENU(3075,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//zeppelin
    {
        player->SEND_POI(1337.36, -4632.7, 6, 6, 0, "Orgrimmar Zeppelin Tower");
        player->SEND_GOSSIP_MENU(3173,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//weapon master
    {
        player->SEND_POI(2092.56, -4823.95, 6, 6, 0, "Sayoc & Hanashi");
        player->SEND_GOSSIP_MENU(4519,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//stable master
    {
        player->SEND_POI(2133.12, -4663.93, 6, 6, 0, "Xon'cha");
        player->SEND_GOSSIP_MENU(5974,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//officers lounge
    {
        player->SEND_POI(1633.56, -4249.37, 6, 6, 0, "Hall of Legends");
        player->SEND_GOSSIP_MENU(7046,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//battlemaster
    {
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY       , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN         , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH        , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
        player->SEND_GOSSIP_MENU(7521,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//class trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER     , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE       , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST     , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN     , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE      , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK    , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR    , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->SEND_GOSSIP_MENU(2599,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 13)//profession trainer
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
        player->SEND_GOSSIP_MENU(2594,_Creature->GetGUID());
    }

}

void SendBattleMasterMenu_guard_orgrimmar(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//AV
    {
        player->SEND_POI(1983.92, -4794.2, 6, 6, 0, "Hall of the Brave");
        player->SEND_GOSSIP_MENU(7484,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//AB
    {
        player->SEND_POI(1983.92, -4794.2, 6, 6, 0, "Hall of the Brave");
        player->SEND_GOSSIP_MENU(7644,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//WSG
    {
        player->SEND_POI(1983.92, -4794.2, 6, 6, 0, "Hall of the Brave");
        player->SEND_GOSSIP_MENU(7520,_Creature->GetGUID());
    }
}

void SendClassTrainerMenu_guard_orgrimmar(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Hunter
    {
        player->SEND_POI(2114.84, -4625.31, 6, 6, 0, "Orgrimmar Hunter's Hall");
        player->SEND_GOSSIP_MENU(2559,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Mage
    {
        player->SEND_POI(1451.26, -4223.33, 6, 6, 0, "Darkbriar Lodge");
        player->SEND_GOSSIP_MENU(2560,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Priest
    {
        player->SEND_POI(1442.21, -4183.24, 6, 6, 0, "Spirit Lodge");
        player->SEND_GOSSIP_MENU(2561,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Shaman
    {
        player->SEND_POI(1925.34, -4181.89, 6, 6, 0, "Thrall's Fortress");
        player->SEND_GOSSIP_MENU(2562,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Rogue
    {
        player->SEND_POI(1773.39, -4278.97, 6, 6, 0, "Shadowswift Brotherhood");
        player->SEND_GOSSIP_MENU(2563,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Warlock
    {
        player->SEND_POI(1849.57, -4359.68, 6, 6, 0, "Darkfire Enclave");
        player->SEND_GOSSIP_MENU(2564,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Warrior
    {
        player->SEND_POI(1983.92, -4794.2, 6, 6, 0, "Hall of the Brave");
        player->SEND_GOSSIP_MENU(2565,_Creature->GetGUID());
    }
}

void SendProfTrainerMenu_guard_orgrimmar(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(1955.17, -4475.79, 6, 6, 0, "Yelmak's Alchemy and Potions");
        player->SEND_GOSSIP_MENU(2497,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Blacksmithing
    {
        player->SEND_POI(2054.34, -4831.85, 6, 6, 0, "The Burning Anvil");
        player->SEND_GOSSIP_MENU(2499,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Cooking
    {
        player->SEND_POI(1780.96, -4481.31, 6, 6, 0, "Borstan's Firepit");
        player->SEND_GOSSIP_MENU(2500,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Enchanting
    {
        player->SEND_POI(1917.5, -4434.95, 6, 6, 0, "Godan's Runeworks");
        player->SEND_GOSSIP_MENU(2501,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Engineering
    {
        player->SEND_POI(2038.45, -4744.75, 6, 6, 0, "Nogg's Machine Shop");
        player->SEND_GOSSIP_MENU(2653,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//First Aid
    {
        player->SEND_POI(1485.21, -4160.91, 6, 6, 0, "Survival of the Fittest");
        player->SEND_GOSSIP_MENU(2502,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Fishing
    {
        player->SEND_POI(1994.15, -4655.7, 6, 6, 0, "Lumak's Fishing");
        player->SEND_GOSSIP_MENU(2503,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Herbalism
    {
        player->SEND_POI(1898.61, -4454.93, 6, 6, 0, "Jandi's Arboretum");
        player->SEND_GOSSIP_MENU(2504,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Leatherworking
    {
        player->SEND_POI(1852.82, -4562.31, 6, 6, 0, "Kodohide Leatherworkers");
        player->SEND_GOSSIP_MENU(2513,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Mining
    {
        player->SEND_POI(2029.79, -4704, 6, 6, 0, "Red Canyon Mining");
        player->SEND_GOSSIP_MENU(2515,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Skinning
    {
        player->SEND_POI(1852.82, -4562.31, 6, 6, 0, "Kodohide Leatherworkers");
        player->SEND_GOSSIP_MENU(2516,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Tailoring
    {
        player->SEND_POI(1802.66, -4560.66, 6, 6, 0, "Magar's Cloth Goods");
        player->SEND_GOSSIP_MENU(2518,_Creature->GetGUID());
    }
}

bool GossipSelect_guard_orgrimmar(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_orgrimmar(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_orgrimmar(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_orgrimmar(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_BATTLEINFO)
        SendBattleMasterMenu_guard_orgrimmar(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU
 *******************************************************/

CreatureAI* GetAI_guard_orgrimmar(Creature *_Creature)
{
    return new guardAI (_Creature);
}

void AddSC_guard_orgrimmar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_orgrimmar";
    newscript->pGossipHello          = &GossipHello_guard_orgrimmar;
    newscript->pGossipSelect         = &GossipSelect_guard_orgrimmar;
    newscript->GetAI = GetAI_guard_orgrimmar;

    m_scripts[nrscripts++] = newscript;
}
