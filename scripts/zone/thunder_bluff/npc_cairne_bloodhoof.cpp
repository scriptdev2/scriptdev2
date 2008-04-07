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
SDName: npc_caine_bloodhoof
SD%Complete: 100
SDComment: for quest 925
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

bool GossipHello_npc_cairne_bloodhoof(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(925) == QUEST_STATUS_INCOMPLETE )
    {
        player->ADD_GOSSIP_ITEM( 0, "Give me hoofprint.", GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO );
        player->SEND_GOSSIP_MENU( 7013, _Creature->GetGUID() );
    }
    else
        if( _Creature->isQuestGiver() ) 
        {
            player->PrepareQuestMenu( _Creature->GetGUID() );
            player->SendPreparedQuest( _Creature->GetGUID() );
        }
        return true;
}
bool GossipSelect_npc_cairne_bloodhoof(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if( action == GOSSIP_SENDER_INFO )
    {
        player->CastSpell( player, 23123, false);
        player->SEND_GOSSIP_MENU( 7014, _Creature->GetGUID() );
    }
    return true;
}
void AddSC_npc_cairne_bloodhoof()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_cairne_bloodhoof";
    newscript->pGossipHello = &GossipHello_npc_cairne_bloodhoof;
    newscript->pGossipSelect = &GossipSelect_npc_cairne_bloodhoof;
    m_scripts[nrscripts++] = newscript;
}
