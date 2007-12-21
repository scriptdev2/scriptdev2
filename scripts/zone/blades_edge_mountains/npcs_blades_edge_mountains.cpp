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

/* ScriptData
SDName: Npcs_Blades_Edge_Mountains
SD%Complete: 95
SDComment: Daranelle analyze for quest 10556, unknown text id's
SDCategory: Blade's Edge Mountains
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## npc_daranelle
######*/

bool GossipHello_npc_daranelle(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(10556) == QUEST_STATUS_INCOMPLETE && player->HasAura(36904,0) == true )
    {
        player->ADD_GOSSIP_ITEM( 0, "Continue...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        player->PlayerTalkClass->SendGossipMenu(384, _Creature->GetGUID());//unknown id
    }
    else
        player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_daranelle(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->PlayerTalkClass->SendGossipMenu(384, _Creature->GetGUID());//unknown id
        player->KilledMonster(21511, _Creature->GetGUID() );
        player->RemoveAurasDueToSpell(36904);
    }
    return true;
}

/*######
## AddSC
######*/

void AddSC_npcs_blades_edge_mountains()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_daranelle";
    newscript->pGossipHello = &GossipHello_npc_daranelle;
    newscript->pGossipSelect = &GossipSelect_npc_daranelle;
    m_scripts[nrscripts++] = newscript;
}
