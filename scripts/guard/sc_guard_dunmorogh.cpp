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

bool GossipHello_guard_dunmorogh(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK             , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_HIPPOGRYPH       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN              , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);

    player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());

    return true;
}

/*******************************************************
 *				Start of GOSSIP_MENU section
 *******************************************************/

void SendDefaultMenu_guard_dunmorogh(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Bank
    {
        player->SEND_GOSSIP_MENU(4288,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Gryphon master
    {
        player->SEND_GOSSIP_MENU(4289,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Guild master
    {
        player->SEND_GOSSIP_MENU(4290,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Inn
    {
        player->SEND_POI(-5582.66, -525.89, 6, 6, 0, "Thunderbrew Distillery");
        player->SEND_GOSSIP_MENU(4291,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Stable Master
    {
        player->SEND_POI(-5604, -509.58, 6, 6, 0, "Shelby Stoneflint");
        player->SEND_GOSSIP_MENU(5985,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Class trainer
    {
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                 , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);

        player->SEND_GOSSIP_MENU(4292,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Profession trainer
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

        player->SEND_GOSSIP_MENU(4300,_Creature->GetGUID());
    }
}

void SendClassTrainerMenu_guard_dunmorogh(Player *player, Creature *_Creature, uint32 action)
{

    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Hunter
    {
        player->SEND_POI(-5618.29, -454.25, 6, 6, 0, "Grif Wildheart");
        player->SEND_GOSSIP_MENU(4293,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Mage
    {
        player->SEND_POI(-5585.6, -539.99, 6, 6, 0, "Magis Sparkmantle");
        player->SEND_GOSSIP_MENU(4294,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Paladin
    {
        player->SEND_POI(-5585.6, -539.99, 6, 6, 0, "Azar Stronghammer");
        player->SEND_GOSSIP_MENU(4295,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Priest
    {
        player->SEND_POI(-5591.74, -525.61, 6, 6, 0, "Maxan Anvol");
        player->SEND_GOSSIP_MENU(4296,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Rogue
    {
        player->SEND_POI(-5602.75, -542.4, 6, 6, 0, "Hogral Bakkan");
        player->SEND_GOSSIP_MENU(4297,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//Warlock
    {
        player->SEND_POI(-5641.97, -523.76, 6, 6, 0, "Gimrizz Shadowcog");
        player->SEND_GOSSIP_MENU(4298,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Warrior
    {
        player->SEND_POI(-5604.79, -529.38, 6, 6, 0, "Granis Swiftaxe");
        player->SEND_GOSSIP_MENU(4299,_Creature->GetGUID());
    }
}

void SendProfTrainerMenu_guard_dunmorogh(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Alchemy
    {
        player->SEND_GOSSIP_MENU(4301,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)//Blacksmithing
    {
        player->SEND_POI(-5584.72, -428.41, 6, 6, 0, "Tognus Flintfire");
        player->SEND_GOSSIP_MENU(4302,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)//Cooking
    {
        player->SEND_POI(-5596.85, -541.43, 6, 6, 0, "Gremlock Pilsnor");
        player->SEND_GOSSIP_MENU(4303,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)//Enchanting
    {
        player->SEND_GOSSIP_MENU(4304,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 5)//Engineering
    {
        player->SEND_POI(-5531, -666.53, 6, 6, 0, "Bronk Guzzlegear");
        player->SEND_GOSSIP_MENU(4305,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)//First Aid
    {
        player->SEND_POI(-5603.67, -523.57, 6, 6, 0, "Thamner Pol");
        player->SEND_GOSSIP_MENU(4306,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 7)//Fishing
    {
        player->SEND_POI(-5202.39, -51.36, 6, 6, 0, "Paxton Ganter");
        player->SEND_GOSSIP_MENU(4307,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 8)//Herbalism
    {
        player->SEND_GOSSIP_MENU(4308,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 9)//Leatherworking
    {
        player->SEND_GOSSIP_MENU(4310,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 10)//Mining
    {
        player->SEND_POI(-5531, -666.53, 6, 6, 0, "Yarr Hamerstone");
        player->SEND_GOSSIP_MENU(4311,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 11)//Skinning
    {
        player->SEND_GOSSIP_MENU(4312,_Creature->GetGUID());
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 12)//Tailoring
    {
        player->SEND_GOSSIP_MENU(4313,_Creature->GetGUID());
    }
}

bool GossipSelect_guard_dunmorogh(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_guard_dunmorogh(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_CLASSTRAIN)
        SendClassTrainerMenu_guard_dunmorogh(player, _Creature, action);

    if (sender == GOSSIP_SENDER_SEC_PROFTRAIN)
        SendProfTrainerMenu_guard_dunmorogh(player, _Creature, action);

    return true;
}

/*******************************************************
 *				End of GOSSIP_MENU section
 *******************************************************/

CreatureAI* GetAI_guard_dunmorogh(Creature *_Creature)
{
    return new guardAI (_Creature);
}

void AddSC_guard_dunmorogh()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_dunmorogh";
    newscript->pGossipHello          = &GossipHello_guard_dunmorogh;
    newscript->pGossipSelect         = &GossipSelect_guard_dunmorogh;
    newscript->GetAI = GetAI_guard_dunmorogh;

    m_scripts[nrscripts++] = newscript;
}
