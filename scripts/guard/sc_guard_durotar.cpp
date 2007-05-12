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

bool GossipHello_guard_durotar(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK         , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_WINDRIDER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->SEND_GOSSIP_MENU(4037,_Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_durotar(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Bank
        player->SEND_GOSSIP_MENU(4032,_Creature->GetGUID());

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Wind rider
        player->SEND_GOSSIP_MENU(4033,_Creature->GetGUID());

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Inn
    {
        player->SEND_POI(338.7, -4688.87, 6, 6, 0, "Razor Hill Inn");
        player->SEND_GOSSIP_MENU(4034,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Stable master
    {
        player->SEND_POI(330.31, -4710.66, 6, 6, 0, "Shoja'my");
        player->SEND_GOSSIP_MENU(5973,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Class trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                 , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->SEND_GOSSIP_MENU(4035,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Profession trainer
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
        player->SEND_GOSSIP_MENU(4036,_Creature->GetGUID());
    }
}

void SendClassTrainerMenu_guard_durotar(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Hunter
    {
        player->SEND_POI(276, -4706.72, 6, 6, 0, "Thotar");
        player->SEND_GOSSIP_MENU(4013,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Mage
    {
        player->SEND_POI(-839.33, -4935.6, 6, 6, 0, "Un'Thuwa");
        player->SEND_GOSSIP_MENU(4014,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Priest
    {
        player->SEND_POI(296.22, -4828.1, 6, 6, 0, "Tai'jin");
        player->SEND_GOSSIP_MENU(4015,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Rogue
    {
        player->SEND_POI(265.76, -4709, 6, 6, 0, "Kaplak");
        player->SEND_GOSSIP_MENU(4016,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Shaman
    {
        player->SEND_POI(307.79, -4836.97, 6, 6, 0, "Swart");
        player->SEND_GOSSIP_MENU(4017,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Warlock
    {
        player->SEND_POI(355.88, -4836.45, 6, 6, 0, "Dhugru Gorelust");
        player->SEND_GOSSIP_MENU(4018,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Warrior
    {
        player->SEND_POI(312.3, -4824.66, 6, 6, 0, "Tarshaw Jaggedscar");
        player->SEND_GOSSIP_MENU(4019,_Creature->GetGUID());
    }
}

void SendProfTrainerMenu_guard_durotar(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_POI(-800.25, -4894.33, 6, 6, 0, "Miao'zan");
        player->SEND_GOSSIP_MENU(4020,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Blacksmithing
    {
        player->SEND_POI(373.24, -4716.45, 6, 6, 0, "Dwukk");
        player->SEND_GOSSIP_MENU(4021,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Cooking
        player->SEND_GOSSIP_MENU(4022,_Creature->GetGUID());

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Enchanting
        player->SEND_GOSSIP_MENU(4023,_Creature->GetGUID());

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Engineering
    {
        player->SEND_POI(368.95, -4723.95, 6, 6, 0, "Mukdrak");
        player->SEND_GOSSIP_MENU(4024,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//First Aid
    {
        player->SEND_POI(327.17, -4825.62, 6, 6, 0, "Rawrk");
        player->SEND_GOSSIP_MENU(4025,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Fishing
    {
        player->SEND_POI(-1065.48, -4777.43, 6, 6, 0, "Lau'Tiki");
        player->SEND_GOSSIP_MENU(4026,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Herbalism
    {
        player->SEND_POI(-836.25, -4896.89, 6, 6, 0, "Mishiki");
        player->SEND_GOSSIP_MENU(4027,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Leatherworking
        player->SEND_GOSSIP_MENU(4028,_Creature->GetGUID());

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Mining
    {
        player->SEND_POI(366.94, -4705, 6, 6, 0, "Krunn");
        player->SEND_GOSSIP_MENU(4029,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Skinning
        player->SEND_GOSSIP_MENU(4030,_Creature->GetGUID());

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Tailoring
        player->SEND_GOSSIP_MENU(4031,_Creature->GetGUID());

}

bool GossipSelect_guard_durotar(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_durotar(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_durotar(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_durotar(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU
 *******************************************************/

CreatureAI* GetAI_guard_durotar(Creature *_Creature)
{
    return new guardAI (_Creature);
}

void AddSC_guard_durotar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_durotar";
    newscript->pGossipHello          = &GossipHello_guard_durotar;
    newscript->pGossipSelect         = &GossipSelect_guard_durotar;
    newscript->GetAI = GetAI_guard_durotar;

    m_scripts[nrscripts++] = newscript;
}
