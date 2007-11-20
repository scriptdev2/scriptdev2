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
SDName: Npc_Lokhtos_Darkbargainer
SD%Complete: 100
SDComment: Gives Thorium Brotherhood Contract
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"

#define ITEM_THRORIUM_BROTHERHOOD_CONTRACT               18628
#define ITEM_SULFURON_INGOT                              17203
#define QUEST_A_BINDING_CONTRACT                         7604
#define SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT_DND    23059

bool GossipHello_npc_lokhtos_darkbargainer(Player *player, Creature *_Creature)
{
    if (player->GetQuestRewardStatus(QUEST_A_BINDING_CONTRACT) != 1 &&
            !player->HasItemCount(ITEM_THRORIUM_BROTHERHOOD_CONTRACT, 1) &&
            player->GetBankItemCount(ITEM_THRORIUM_BROTHERHOOD_CONTRACT) < 1 &&
            player->HasItemCount(ITEM_SULFURON_INGOT, 1))
    {
        player->ADD_GOSSIP_ITEM(7, "Get Thorium Brotherhood Contract",
            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(384,_Creature->GetGUID());
        return true;
    }
    return false;
}

bool GossipSelect_npc_lokhtos_darkbargainer(Player *player, Creature *_Creature,
    uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
        player->CastSpell(player, SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT_DND, false);
    }

    return true;
}

void AddSC_npc_lokhtos_darkbargainer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_lokhtos_darkbargainer";
    newscript->pGossipHello          = &GossipHello_npc_lokhtos_darkbargainer;
    newscript->pGossipSelect         = &GossipSelect_npc_lokhtos_darkbargainer;
    m_scripts[nrscripts++] = newscript;
}
