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
SDName: npcs_eastern_plaguelands
SD%Complete: 90
SDComment: npcs eastern plaguelands, mostly quest related.
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## npc_darrowshire_spirit
######*/

#define GOSSIP_ITEM     "Free me"

struct MANGOS_DLL_DECL npc_darrowshire_spiritAI : public ScriptedAI
{
    npc_darrowshire_spiritAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
    }
    
    void AttackStart(Unit *who)
    {
        return;
    }

    void MoveInLineOfSight(Unit *who)
    {
        return;
    }
}; 
CreatureAI* GetAI_npc_darrowshire_spirit(Creature *_Creature)
{
    return new npc_darrowshire_spiritAI (_Creature);
}
//to open gossip
bool GossipHello_npc_darrowshire_spirit(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    player->SEND_GOSSIP_MENU(3873,_Creature->GetGUID());

    return true;
}
//after player select gossip item
void SendDefaultMenu_npc_darrowshire_spirit(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        //update quest status, requires SpecialFlag = 8
        player->KilledMonster(_Creature->GetEntry(), _Creature->GetGUID());

        player->SEND_GOSSIP_MENU(3873,_Creature->GetGUID());

        //prevent further interaction
        _Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
}
//what action to take when player selects gossip item
bool GossipSelect_npc_darrowshire_spirit(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_npc_darrowshire_spirit(player, _Creature, action);

    return true;
}

/*######
## npc_tirion_fordring
######*/

bool GossipHello_npc_tirion_fordring(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(5742) == QUEST_STATUS_INCOMPLETE && player->getStandState() == PLAYER_STATE_SIT )
        player->ADD_GOSSIP_ITEM( 0, "I am ready to hear your tale, Tirion.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_tirion_fordring(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "Thank you, Tirion.  What of your identity?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(4493, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, "That is terrible.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(4494, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM( 0, "I will, Tirion.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->SEND_GOSSIP_MENU(4495, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->PlayerTalkClass->CloseGossip();
            player->CompleteQuest(5742);
            break;
    }
    return true;
}

void AddSC_npcs_eastern_plaguelands()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_darrowshire_spirit";
    newscript->GetAI = GetAI_npc_darrowshire_spirit;
    newscript->pGossipHello = &GossipHello_npc_darrowshire_spirit;
    newscript->pGossipSelect = &GossipSelect_npc_darrowshire_spirit;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_tirion_fordring";
    newscript->pGossipHello =  &GossipHello_npc_tirion_fordring;
    newscript->pGossipSelect = &GossipSelect_npc_tirion_fordring;
    m_scripts[nrscripts++] = newscript;
}
