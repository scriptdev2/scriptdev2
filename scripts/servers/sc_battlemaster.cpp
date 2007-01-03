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

bool GossipHello_battlemaster(Player *player, Creature *_Creature)
{
    uint64 guid = _Creature->GetGUID();
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( guid );
    player->SendPreparedQuest( guid );

                                                            // If creature has BattleMaster flag - addind gossip text
    player->ADD_GOSSIP_ITEM( 2, "I want to go to the battlegroud.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_BATTLE);
                                                            // Info message.
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

    return true;
}

bool GossipSelect_battlemaster(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    /*if (opt == co.GOSSIP_ACTION_BATTLE)
          player.SendBattleFieldsList(self)*/

    return false;
}

uint32 NPCDialogStatus_battlemaster(Player *player, Creature *_Creature )
{
    return _Creature->QUEST_DIALOG_STATUS(player, DIALOG_STATUS_CHAT);
}

void AddSC_battlemaster()                                   // Please write comments for under functions.
{
    UnitScript *newscript;

    newscript = new UnitScript;
    newscript->Name="battlemaster";
    newscript->pGossipHello          = &GossipHello_battlemaster;
    newscript->pGossipSelect         = &GossipSelect_battlemaster;
    newscript->pNPCDialogStatus      = &NPCDialogStatus_battlemaster;
    m_scripts[nrscripts++] = newscript;
}
