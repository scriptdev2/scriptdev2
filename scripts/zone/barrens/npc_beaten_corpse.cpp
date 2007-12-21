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
SDName: npc_beaten_corpse
SD%Complete: 100
SDComment: For Quest 4921
SDCategory: Barrens
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"

struct MANGOS_DLL_DECL beatencorpseAI : public ScriptedAI
{
    beatencorpseAI(Creature *c): ScriptedAI(c)
    {
        EnterEvadeMode();
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,PLAYER_STATE_DEAD);
        DoGoHome();
    }

    void UpdateAI(const uint32 diff)
    {
        if(m_creature->GetUInt32Value(UNIT_FIELD_BYTES_1) != PLAYER_STATE_DEAD)
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,PLAYER_STATE_DEAD);
    }
};

bool GossipHello_npc_beaten_corpse(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(4921) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(4921) == QUEST_STATUS_COMPLETE)
        player->ADD_GOSSIP_ITEM(0,"Examine corpse in detail...",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
    player->SEND_GOSSIP_MENU(3557,_Creature->GetGUID());
    return true; 

}

bool GossipSelect_npc_beaten_corpse(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if(action == GOSSIP_ACTION_INFO_DEF +1)
    {
        player->SEND_GOSSIP_MENU(3558,_Creature->GetGUID());
        player->KilledMonster( 10668,_Creature->GetGUID() );
    }
    return true;
}

CreatureAI* GetAI_npc_beaten_corpse(Creature *_creature)
{
    return new beatencorpseAI(_creature);
}

void AddSC_npc_beaten_corpse()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_beaten_corpse";
    newscript->GetAI=GetAI_npc_beaten_corpse;
    newscript->pGossipHello = &GossipHello_npc_beaten_corpse;
    newscript->pGossipSelect = &GossipSelect_npc_beaten_corpse;
    m_scripts[nrscripts++] = newscript;
}


