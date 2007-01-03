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

bool GossipHello_guard_teldrassil(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, "The Bank"             , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 0, "Rut'theran Ferry"     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, "The Guild Master"     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, "The Inn"              , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 5, "Stable Master"        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 3, "Class Trainer"        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 3, "Profession Trainer"   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);

    player->SEND_GOSSIP_MENU(4316,_Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU section
 *******************************************************/

void SendDefaultMenu_guard_teldrassil(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Bank
        player->SEND_GOSSIP_MENU(4317,_Creature->GetGUID());

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Rut`theran
        player->SEND_GOSSIP_MENU(4318,_Creature->GetGUID());

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Guild master
        player->SEND_GOSSIP_MENU(4319,_Creature->GetGUID());

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Inn
    {
        player->SEND_POI(9821.49, 960.13, 6, 6, 0, "Dolanaar Inn");
        player->SEND_GOSSIP_MENU(4320,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//stable master
    {
        player->SEND_POI(9808.37, 931.1, 6, 6, 0, "Seriadne");
        player->SEND_GOSSIP_MENU(5982,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//class trainer
    {
        player->ADD_GOSSIP_ITEM( 3, "Druid"                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, "Hunter"               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, "Priest"               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, "Rogue"                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, "Warrior"              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);

        player->SEND_GOSSIP_MENU(4264,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//profession trainer
    {
        player->ADD_GOSSIP_ITEM( 3, "Alchemy"              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, "Cooking"              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, "Enchanting"           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, "First Aid"            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, "Fishing"              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, "Herbalism"            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, "Leatherworking"       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->ADD_GOSSIP_ITEM( 3, "Skinning"             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
        player->ADD_GOSSIP_ITEM( 3, "Tailoring"            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
        player->SEND_GOSSIP_MENU(4273,_Creature->GetGUID());
    }
}

void SendClassTrainerMenu_guard_teldrassil(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Druid
    {
        player->SEND_POI(9741.58, 963.7, 6, 6, 0, "Kal");
        player->SEND_GOSSIP_MENU(4323,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Hunter
    {
        player->SEND_POI(9815.12, 926.28, 6, 6, 0, "Dazalar");
        player->SEND_GOSSIP_MENU(4324,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Priest
    {
        player->SEND_POI(9906.16, 986.63, 6, 6, 0, "Laurna Morninglight");
        player->SEND_GOSSIP_MENU(4325,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Rogue
    {
        player->SEND_POI(9789, 942.86, 6, 6, 0, "Jannok Breezesong");
        player->SEND_GOSSIP_MENU(4326,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Warrior
    {
        player->SEND_POI(9821.96, 950.61, 6, 6, 0, "Kyra Windblade");
        player->SEND_GOSSIP_MENU(4327,_Creature->GetGUID());
    }

}

void SendProfTrainerMenu_guard_teldrassil(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(9767.59, 878.81, 6, 6, 0, "Cyndra Kindwhisper");
        player->SEND_GOSSIP_MENU(4329,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Cooking
    {
        player->SEND_POI(9751.19, 906.13, 6, 6, 0, "Zarrin");
        player->SEND_GOSSIP_MENU(4330,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Enchanting
    {
        player->SEND_POI(10677.59, 1946.56, 6, 6, 0, "Alanna Raveneye");
        player->SEND_GOSSIP_MENU(4331,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//First Aid
    {
        player->SEND_POI(9903.12, 999, 6, 6, 0, "Byancie");
        player->SEND_GOSSIP_MENU(4332,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Fishing
    {
        player->SEND_GOSSIP_MENU(4333,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Herbalism
    {
        player->SEND_POI(9773.78, 875.88, 6, 6, 0, "Malorne Bladeleaf");
        player->SEND_GOSSIP_MENU(4334,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Leatherworking
    {
        player->SEND_POI(10152.59, 1681.46, 6, 6, 0, "Nadyia Maneweaver");
        player->SEND_GOSSIP_MENU(4335,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Skinning
    {
        player->SEND_POI(10135.59, 1673.18, 6, 6, 0, "Radnaal Maneweaver");
        player->SEND_GOSSIP_MENU(4336,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Tailoring
        player->SEND_GOSSIP_MENU(4337,_Creature->GetGUID());
}

bool GossipSelect_guard_teldrassil(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_teldrassil(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_teldrassil(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_teldrassil(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU section
 *******************************************************/

void AddSC_guard_teldrassil()
{
    UnitScript *newscript;

    newscript = new UnitScript;
    newscript->Name="guard_teldrassil";
    newscript->pGossipHello          = &GossipHello_guard_teldrassil;
    newscript->pGossipSelect         = &GossipSelect_guard_teldrassil;

	m_scripts[nrscripts++] = newscript;
}
