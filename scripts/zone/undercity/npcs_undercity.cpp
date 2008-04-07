/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Npcs_Undercity
SD%Complete: 95
SDComment: Parqual Fintallas questions/'answers' might have more to it, need more info
SDCategory: Undercity
EndScriptData */

#include "sc_gossip.h"

/*######
## npc_parqual_fintallas
######*/

bool GossipHello_npc_parqual_fintallas(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(6628) == QUEST_STATUS_INCOMPLETE && player->HasAura(6767,0) == false )
    {
        player->ADD_GOSSIP_ITEM( 0, "Gul'dan", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->ADD_GOSSIP_ITEM( 0, "Kel'Thuzad", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->ADD_GOSSIP_ITEM( 0, "Ner'zhul", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

        player->PlayerTalkClass->SendGossipMenu(5822, _Creature->GetGUID());
    }
    else
        player->PlayerTalkClass->SendGossipMenu(5821, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_parqual_fintallas(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->PlayerTalkClass->CloseGossip();
        player->CastSpell(player,6767,false);               //mark of shame
    }
    if (action == GOSSIP_ACTION_INFO_DEF+2)
    {
        player->PlayerTalkClass->CloseGossip();
        player->CompleteQuest(6628);
    }
    return true;
}

/*######
## AddSC
######*/

void AddSC_npcs_undercity()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_parqual_fintallas";
    newscript->pGossipHello = &GossipHello_npc_parqual_fintallas;
    newscript->pGossipSelect = &GossipSelect_npc_parqual_fintallas;
    m_scripts[nrscripts++] = newscript;
}
