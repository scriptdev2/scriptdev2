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

bool GossipHello_travelmaster(Player *player, Creature *_Creature)
{
    // Info message
    switch (_Creature->GetEntry())
    {
//Zeppelins
    //Durotar
        case 9564: //Frezza to Tirisfal
            player->SEND_GOSSIP_MENU(2642,_Creature->GetGUID());
            break;

        case 12136: //Snurk to Grom`gol
            player->SEND_GOSSIP_MENU(4693,_Creature->GetGUID());
            break;
    //Grom`gol
        case 3149: //Nez`raz to Durotar
            player->SEND_GOSSIP_MENU(3133,_Creature->GetGUID());
            break;

        case 12137: //Squibby Overspeck to Tirisfal
            player->SEND_GOSSIP_MENU(4694,_Creature->GetGUID());
            break;

    //Tirisfal
        case 9566: //Zapetta to Durotar
            player->SEND_GOSSIP_MENU(2644,_Creature->GetGUID());
            break;

        case 3150: //Hin Denburg to Grom`gol
            player->SEND_GOSSIP_MENU(2753,_Creature->GetGUID());
            break;
//Ships
        case 9559: //Booty Bay
            player->SEND_GOSSIP_MENU(2641,_Creature->GetGUID());
            break;

        case 9558: //Ratchet
            player->SEND_GOSSIP_MENU(2640,_Creature->GetGUID());
            break;

//not defined
        default :
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
            break;
    }
    return true;
}

uint32 NPCDialogStatus_travelmaster(Player *player, Creature *_Creature )
{
    return 128;
}

void AddSC_travelmaster()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="travelmaster";
    newscript->pGossipHello          = &GossipHello_travelmaster;
    newscript->pNPCDialogStatus      = &NPCDialogStatus_travelmaster;
    m_scripts[nrscripts++] = newscript;
}
