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
SDName: npcs_searing_gorge
SD%Complete: 90
SDComment: Provides quest and portal to Molten Core + misc other quest. More accurate info on Kalaran needed.
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"
#include "../../../../../game/QuestDef.h"

/*######
## npc_kalaran_windblade
######*/

bool GossipHello_npc_kalaran_windblade(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(3441) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "Tell me what drives this vengance?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_kalaran_windblade(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, "Please continue", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(1954, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "I will avenge your loss", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(1955, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->PlayerTalkClass->CloseGossip();
            player->CompleteQuest(3441);
            break;
    }
    return true;
}

/*######
## npc_lothos_riftwalker
######*/

struct MANGOS_DLL_DECL npc_lothos_riftwalkerAI : public ScriptedAI
{
    npc_lothos_riftwalkerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    bool InCombat;

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }
};


CreatureAI* GetAI_npc_lothos_riftwalker(Creature *_Creature)
{
    return new npc_lothos_riftwalkerAI (_Creature);
}

bool GossipHello_npc_lothos_riftwalker(Player *player, Creature *_Creature)
{
    if (player->GetQuestRewardStatus(7487)==1 || player->GetQuestRewardStatus(7848)==1)  {
        player->ADD_GOSSIP_ITEM(2, "Teleport me to the Molten Core", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(384,_Creature->GetGUID());
    }else{
        uint64 guid = _Creature->GetGUID();
        if (_Creature->isQuestGiver()) {
            player->PrepareQuestMenu( guid );
            player->SendPreparedQuest( guid );
        }
    }
    return true;
}

bool GossipSelect_npc_lothos_riftwalker(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(409, 1096, -467, -104.6, 3.64);
    }

    return true;
}

/*######
## 
######*/

void AddSC_npcs_searing_gorge()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_kalaran_windblade";
    newscript->pGossipHello =  &GossipHello_npc_kalaran_windblade;
    newscript->pGossipSelect = &GossipSelect_npc_kalaran_windblade;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_lothos_riftwalker";
    newscript->pGossipHello          = &GossipHello_npc_lothos_riftwalker;
    newscript->pGossipSelect         = &GossipSelect_npc_lothos_riftwalker;
    newscript->GetAI = GetAI_npc_lothos_riftwalker;
    m_scripts[nrscripts++] = newscript;
}
