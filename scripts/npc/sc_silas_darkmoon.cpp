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

bool GossipHello_silas_darkmoon(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 0, "Silas, why is most everything here at the fair free? How do you make a profit?", GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO );
//	player->SEND_GOSSIP_MENU(MUST BE SNIFFED!,_Creature->GetGUID()); //His message must be sniffed. If you have some information, please, contact me via PM.

    return true;
}

bool GossipSelect_silas_darkmoon(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    if (action == GOSSIP_SENDER_INFO)
//        player->SEND_GOSSIP_MENU(MUST BE SNIFFED!,_Creature->GetGUID());//His message must be sniffed. If you have some information, please, contact me via PM.

    return true;

    return true;
}

uint32 NPCDialogStatus_silas_darkmoon(Player *player, Creature *_Creature )
{
    return _Creature->QUEST_DIALOG_STATUS(player, DIALOG_STATUS_CHAT);
}

void AddSC_silas_darkmoon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="silas_darkmoon";
    newscript->pGossipHello          = &GossipHello_silas_darkmoon;
    newscript->pGossipSelect         = &GossipSelect_silas_darkmoon;
    newscript->pNPCDialogStatus      = &NPCDialogStatus_silas_darkmoon;
    m_scripts[nrscripts++] = newscript;
}