/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: trial_of_the_crusader
SD%Complete: 0
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum
{
    NPC_GORMOK                  = 34796,
    NPC_JARAXXUS                = 34780,

    GOSSIP_ITEM_START_EVENT1    = -3649000
};

/*######
## npc_barrett_ramsey
######*/

struct MANGOS_DLL_DECL npc_barrett_ramseyAI : public ScriptedAI
{
    npc_barrett_ramseyAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void StartEvent(Player* pPlayer)
    {
        // code starting the event here
    }
};

bool GossipHello_npc_barrett_ramsey(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START_EVENT1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_barrett_ramsey(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        if (npc_barrett_ramseyAI* pBarrettAI = dynamic_cast<npc_barrett_ramseyAI*>(pCreature->AI()))
            pBarrettAI->StartEvent(pPlayer);
    }

    return true;
}

CreatureAI* GetAI_npc_barrett_ramsey(Creature* pCreature)
{
    return new npc_barrett_ramseyAI(pCreature);
}

void AddSC_trial_of_the_crusader()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_barrett_ramsey";
    pNewScript->GetAI = &GetAI_npc_barrett_ramsey;
    pNewScript->pGossipHello = &GossipHello_npc_barrett_ramsey;
    pNewScript->pGossipSelect = &GossipSelect_npc_barrett_ramsey;
    pNewScript->RegisterSelf();
}
