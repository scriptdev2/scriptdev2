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


#include "../../sc_defines.h"

#define GOSSIP_ITEM     "Free at last"

struct MANGOS_DLL_DECL npc_darrowshire_spiritAI : public ScriptedAI
{
    npc_darrowshire_spiritAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }
}; 

CreatureAI* GetAI_npc_darrowshire_spirit(Creature *_Creature)
{
    return new npc_darrowshire_spiritAI (_Creature);
}

//when player selects from gossip menu
void SendDefaultMenu_npc_darrowshire_spirit(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        //update quest status (add 1+ to mobcount) This require SpecialFlag to work (QUEST_SPECIAL_FLAGS_KILL_OR_CAST = 8)
        player->KilledMonster(_Creature->GetEntry(), _Creature->GetGUID());
        //close menu
        player->PlayerTalkClass->CloseGossip();
        //prevent further interaction
        _Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
}

//when player interacts with creature
bool GossipSelect_npc_darrowshire_spirit(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_npc_darrowshire_spirit(player, _Creature, action);

    return true;
}

//when gossip menu opens
bool GossipHello_npc_darrowshire_spirit(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    //should probably have it's own entry in DB npc_text
    player->PlayerTalkClass->SendGossipMenu(1228,_Creature->GetGUID());

    return true;
}

void AddSC_npc_darrowshire_spirit()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_darrowshire_spirit";
    newscript->GetAI = GetAI_npc_darrowshire_spirit;
    newscript->pGossipHello = &GossipHello_npc_darrowshire_spirit;
    newscript->pGossipSelect = &GossipSelect_npc_darrowshire_spirit;

    m_scripts[nrscripts++] = newscript;
}