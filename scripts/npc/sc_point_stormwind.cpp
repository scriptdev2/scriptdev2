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

bool GossipHello_pointguard_stormwind(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, "Directions to Alliance Cities", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, "Directions to Horde Cities", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->SEND_GOSSIP_MENU(933,_Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_pointguard_stormwind(Player *player, Creature *_Creature, uint32 action)
{

	//Alliance
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->ADD_GOSSIP_ITEM( 0, "Stormwind", GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 0, "Ironforge", GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 0, "Darnassus", GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->SEND_GOSSIP_MENU(933,_Creature->GetGUID());
    }

	//Horde
	if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
        player->ADD_GOSSIP_ITEM( 0, "Orgrimmar", GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 0, "Undercity", GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 0, "Thunderbluff", GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->SEND_GOSSIP_MENU(933,_Creature->GetGUID());
    }
}

void SendClassTrainerMenu_pointguard_stormwind(Player *player, Creature *_Creature, uint32 action)
{
	//stormwind
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->SEND_POI(-8886.2, 574.577, 6, 6, 0, "Stormwind City");
        player->SEND_GOSSIP_MENU(1337001,_Creature->GetGUID());
    }

	//ironforge
	 if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
        player->SEND_POI(-4981.25, -881.542, 6, 6, 0, "Ironforge");
        player->SEND_GOSSIP_MENU(1337001,_Creature->GetGUID());
    }

	//darnassus
	 if (action == GOSSIP_ACTION_INFO_DEF + 3)
    {
        player->SEND_POI(9948.55, 2413.59, 6, 6, 0, "Darnassus");
        player->SEND_GOSSIP_MENU(1337001,_Creature->GetGUID());
    }

	//orgrimmar
	 if (action == GOSSIP_ACTION_INFO_DEF + 4)
    {
        player->SEND_POI(1502.71, -4415.42, 6, 6, 0, "Orgrimmar");
        player->SEND_GOSSIP_MENU(1337001,_Creature->GetGUID());
    }

	//undercity
	 if (action == GOSSIP_ACTION_INFO_DEF + 5)
    {
        player->SEND_POI(1832.44, 236.426, 6, 6, 0, "The Undercity");
        player->SEND_GOSSIP_MENU(1337001,_Creature->GetGUID());
    }

	//thunder bluff
	 if (action == GOSSIP_ACTION_INFO_DEF + 6)
    {
        player->SEND_POI(-1285.42, 176.523, 6, 6, 0, "Thunder Bluff");
        player->SEND_GOSSIP_MENU(1337001,_Creature->GetGUID());
    }
}

bool GossipSelect_pointguard_stormwind(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_pointguard_stormwind(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_pointguard_stormwind(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU
 *******************************************************/

void AddSC_pointguard_stormwind()
{
    UnitScript *newscript;

    newscript = new UnitScript;
    newscript->Name="pointguard_stormwind";
    newscript->pGossipHello          = &GossipHello_pointguard_stormwind;
    newscript->pGossipSelect         = &GossipSelect_pointguard_stormwind;

    m_scripts[nrscripts++] = newscript;
}
