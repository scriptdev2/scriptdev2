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
SDName: Npcs_Shadowfang_Keep
SD%Complete: 75
SDComment: Missing prisoners movement to door, open door after reaching door, move back to location (or despawn to indicate they are freed?)
SDCategory: Shadowfang Keep
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"
#include "def_shadowfang_keep.h"

/*######
## npc_shadowfang_prisoner
######*/

struct MANGOS_DLL_DECL npc_shadowfang_prisonerAI : public ScriptedAI
{
    npc_shadowfang_prisonerAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset() {}
    void Aggro(Unit* who) {}
};

CreatureAI* GetAI_npc_shadowfang_prisoner(Creature *_creature)
{
    return new npc_shadowfang_prisonerAI(_creature);
}

bool GossipHello_npc_shadowfang_prisoner(Player *player, Creature *_Creature)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)_Creature->GetInstanceData());

    if( pInstance && (pInstance->GetData(TYPE_RETHILGORE) >= DONE) && (pInstance->GetData(TYPE_FREE_NPC) == NOT_STARTED) )
        player->ADD_GOSSIP_ITEM( 0, "Thanks", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_shadowfang_prisoner(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->CLOSE_GOSSIP_MENU();
            ((npc_shadowfang_prisonerAI*)_Creature->AI())->pInstance->SetData(TYPE_FREE_NPC, DONE);
    }
    return true;
}

/*######
## AddSC
######*/

void AddSC_npcs_shadowfang_keep()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_shadowfang_prisoner";
    newscript->pGossipHello =  &GossipHello_npc_shadowfang_prisoner;
    newscript->pGossipSelect = &GossipSelect_npc_shadowfang_prisoner;
    newscript->GetAI = GetAI_npc_shadowfang_prisoner;
    m_scripts[nrscripts++] = newscript;
}
