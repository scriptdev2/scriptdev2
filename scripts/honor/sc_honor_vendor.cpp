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

bool GossipHello_Honor_Vendor(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
   
	return true;
}

bool GossipSelect_Honor_Vendor(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if(action == GOSSIP_ACTION_TRADE)
    {
        player->SEND_VENDORLIST( _Creature->GetGUID() );
    }

    return true;
}

bool GossipSelectWithCode_Honor_Vendor( Player *player, Creature *_Creature, uint32 sender, uint32 action, char* sCode )
{
    return false;
}

uint32 NPCDialogStatus_Honor_Vendor(Player *player, Creature *_Creature )
{
    return _Creature->QUEST_DIALOG_STATUS(player, DIALOG_STATUS_CHAT);
}

void AddSC_Honor_Vendor()
{
    UnitScript *newscript;

    newscript = new UnitScript;
    newscript->Name="honor_vendor";
    newscript->pGossipHello          = &GossipHello_Honor_Vendor;
    newscript->pGossipSelect         = &GossipSelect_Honor_Vendor;
    newscript->pGossipSelectWithCode = &GossipSelectWithCode_Honor_Vendor;
    newscript->pNPCDialogStatus      = &NPCDialogStatus_Honor_Vendor;
    m_scripts[nrscripts++] = newscript;
}
