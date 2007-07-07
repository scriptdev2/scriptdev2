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

bool GossipHello_guard_exodar(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_HIPPOGRYPH     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN            , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_exodar(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Auction house
    {
        player->SEND_POI(-4023.6, -11739.3, 6, 6, 0, "Exodar Auction House");//18349
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Bank
    {
        player->SEND_POI(-3923.89, -11544.5, 6, 6, 0, "Exodar Bank");//17773
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Guild master
    {
        player->SEND_POI(-4092.57, -11626.5, 6, 6, 0, "Exodar Guild Master");//16734
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Hippogryph master
    {
        player->SEND_POI(-4060.46, -11787.1, 6, 6, 0, "Exodar Hippogryph Master");//17555
        player->SEND_GOSSIP_MENU(9530, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Inn
    {
        player->SEND_POI(-3741.87, -11695.1, 6, 6, 0, "Exodar Inn");//16739
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Mailbox
    {
        //player->SEND_POI(1, 1, 6, 6, 0, "Exodar Mailbox");//go 
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Stable master
    {
        player->SEND_POI(-4146, -12492.4, 6, 6, 0, "Esbina");//17485
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Weapon trainer
    {
        player->SEND_POI(-4215.68, -11628.9, 6, 6, 0, "Handiir");//16773
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
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        
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

        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }
}

void SendBattleMasterMenu_guard_exodar(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//AV
    {
        player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Alterac Valley Battlemaster");
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//AB
    {
        player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Arathi Basin Battlemaster");
        player->SEND_GOSSIP_MENU(9316,  _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//A
    {
        player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Arena Battlemaster");
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//EOS
    {
        player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Eye Of The Storm Battlemaster");
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//WSG
    {
        player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Warsong Gulch Battlemaster");
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

}

void SendClassTrainerMenu_guard_exodar(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Druid
    {
        player->SEND_POI(-4170.3, -11578.3, 6, 6, 0, "Exodar Druid Trainer");//16721
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Hunter
    {
        player->SEND_POI(-4232.18, -11569.2, 6, 6, 0, "Exodar Hunter Trainer");//17122
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Mage
    {
        player->SEND_POI(-4057.32, -11556.5, 6, 6, 0, "Exodar Mage Trainer");//17513
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }
    
    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Paladin
    {
        player->SEND_POI(-4191.2, -11470.4, 6, 6, 0, "Exodar Paladin Trainer");//17509
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }
    
    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Priest
    {
        player->SEND_POI(-3969.63, -11482.8, 6, 6, 0, "Exodar Priest Trainer");//17510
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }
    
    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Shaman
    {
        player->SEND_POI(-3886.88, -11339.7, 6, 6, 0, "Exodar Shaman Trainer");//17520
        player->SEND_GOSSIP_MENU(9556, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Warrior
    {
        player->SEND_POI(-4189.43, -11653.7, 6, 6, 0, "Exodar Warrior Trainer");//17120
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }
}

void SendProfTrainerMenu_guard_exodar(Player *player, Creature *_Creature, uint32 action)
{
    
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(-4146.6, -12531.8, 6, 6, 0, "Exodar Alchemy Trainer");//17215
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Blacksmithing
    {
        player->SEND_POI(-4699, -12420.5, 6, 6, 0, "Exodar Blacksmithing Trainer");//17245
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Cooking
    {
        player->SEND_POI(-4710, -12393.8, 6, 6, 0, "Exodar Cooking Trainer");//17246
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Enchanting
    {
        player->SEND_POI(-3889.3, -11495, 6, 6, 0, "Exodar Enchanting Trainer");//16742
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Engineering
    {
        player->SEND_POI(-4257.68, -11640.3, 6, 6, 0, "Exodar Engineering Trainer");//16743
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//First Aid
    {
        player->SEND_POI(-1928.4, -11857, 6, 6, 0, "Exodar First Aid Trainer");//17424
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Fishing
    {
        player->SEND_POI(-4266.52, -12985.2, 6, 6, 0, "Exodar Fishing Trainer");//17101
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Jewelcrafting
    {
        player->SEND_POI(-3787.7, -11547.6, 6, 6, 0, "Exodar Jewelcrafting Trainer");//17512
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Herbalism
    {
        player->SEND_POI(-4189.61, -12459.5, 6, 6, 0, "Exodar Herbalism Trainer");//17983
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Leatherworking
    {
        player->SEND_POI(-4140.6, -11776.7, 6, 6, 0, "Exodar Leatherworking Trainer");//16745
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Mining
    {
        player->SEND_POI(-4165.5, -12459.1, 6, 6, 0, "Exodar Mining Trainer");//17488
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Skinning
    {
        player->SEND_POI(-4134.97, -11760.5, 6, 6, 0, "Exodar Skinning Trainer");//16763
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 13)//Tailoring
    {
        player->SEND_POI(-4095.78, -11746.9, 6, 6, 0, "Exodar Tailor");//17487
        player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
    }

}

bool GossipSelect_guard_exodar(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_exodar(player, _Creature, action);


    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_exodar(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_exodar(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_BATTLEINFO)
        SendBattleMasterMenu_guard_exodar(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU
 *******************************************************/

CreatureAI* GetAI_guard_exodar(Creature *_Creature)
{
    return new guardAI (_Creature);
}


void AddSC_guard_exodar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_exodar";
    newscript->pGossipHello          = &GossipHello_guard_exodar;
    newscript->pGossipSelect         = &GossipSelect_guard_exodar;
    newscript->GetAI = GetAI_guard_exodar;

    m_scripts[nrscripts++] = newscript;
}
