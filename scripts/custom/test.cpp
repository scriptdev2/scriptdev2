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
SDName: Test.cpp
SD%Complete: 100
SDComment: Script used for testing
SDCategory: Custom
EndScriptData */

#include "sc_creature.h"
#include "../npc/npc_escortAI.h"
#include "sc_gossip.h"

void TestCallBack(Creature* c, Player*, uint32)
{
    c->Say("Hmm a nice day for a walk alright", LANG_UNIVERSAL, NULL);
}

void TestCallBack2(Creature* c, Player*, uint32)
{
    c->Say("Watchout!", LANG_UNIVERSAL, NULL);
    Creature* temp = c->SummonCreature(3099, c->GetPositionX()+5, c->GetPositionY()+7, c->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);

    temp->AI()->AttackStart(c);
}

void TestCallBack3(Creature* c, Player*, uint32)
{
    c->Say("ARG HEART ATTACK!", LANG_UNIVERSAL, NULL);
}

CreatureAI* GetAI_test(Creature *_Creature)
{
    npc_escortAI* testAI = new npc_escortAI(_Creature);

    testAI->AddWaypoint(0, 1231, -4419, 23);
    testAI->AddWaypoint(1, 1198, -4440, 23, 0, (EscortAICallback)TestCallBack);
    testAI->AddWaypoint(2, 1208, -4392, 23);
    testAI->AddWaypoint(3, 1231, -4419, 23, 5000, (EscortAICallback)TestCallBack2);
    testAI->AddWaypoint(4, 1208, -4392, 23, 2000, (EscortAICallback)TestCallBack3);

    return (CreatureAI*)testAI;
}

bool GossipHello_npc_test(Player *player, Creature *_Creature)
{
    player->TalkedToCreature(_Creature->GetEntry(),_Creature->GetGUID());
    _Creature->prepareGossipMenu(player,0);

    player->PlayerTalkClass->GetGossipMenu()->AddMenuItem(0, "Click to Test Escort", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1,"",0);

    _Creature->sendPreparedGossip( player );
    return true;
}
 
bool GossipSelect_npc_test(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->CLOSE_GOSSIP_MENU();
        ((npc_escortAI*)(_Creature->AI()))->Start(false, true);

        return true;    // prevent mangos core handling
    }
    return false;       // the player didn't select "trick or treat" or cheated, normal core handling
}

void AddSC_test()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="test";
    newscript->GetAI = GetAI_test;
    newscript->pGossipHello          = &GossipHello_npc_test;
    newscript->pGossipSelect         = &GossipSelect_npc_test;
    m_scripts[nrscripts++] = newscript;
}
