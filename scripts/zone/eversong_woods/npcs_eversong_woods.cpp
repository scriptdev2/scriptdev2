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
SDName: npc_eversong_wood
SD%Complete: 100
SDComment: Script for npc Prospector Anvilward
SDCategory: Eversong Woods
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"
#include "../../npc/npc_escortAI.h"

#define QUEST_THE_DWARVEN_SPY 8483

struct MANGOS_DLL_DECL npc_Prospector_Anvilward : public npc_escortAI
{
public:
    // CreatureAI functions
    npc_Prospector_Anvilward(Creature *c) : npc_escortAI(c) {Reset();}	

    // Pure Virtual Functions
    void WaypointReached(uint32 i)
    {
        switch (i)
        {
        case 0:
            m_creature->Say("Very well. Let's see what you have to show me, $N.", LANG_UNIVERSAL, PlayerGUID);
            break;

        case 6:
            {
                m_creature->Say("What manner of trick is this, $R? If you seek to ambush me, I warn you I will not go down quietly!", LANG_UNIVERSAL, PlayerGUID);
                //Change Faction
                m_creature->setFaction(24);

                Unit* player = Unit::GetUnit(*m_creature, PlayerGUID);
                if (player)
                {
                    m_creature->AI()->AttackStart(player);
                }
            }
            break;
        }
    }

    void Aggro(Unit*)
    {

    }

    void Reset()
    {
        //Default npc faction
        m_creature->setFaction(35);		
    }

    void JustDied(Unit* killer)
    {	
        //Default npc faction
        m_creature->setFaction(35);
    }

    void UpdateAI(const uint32 diff)
    {	//Must update npc_escortAI
        npc_escortAI::UpdateAI(diff);

        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            //Make sure our attack is ready and we arn't currently casting
            if( m_creature->isAttackReady())
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }

    }
};

CreatureAI* GetAI_Prospector_Anvilward(Creature *_Creature)
{
    npc_Prospector_Anvilward* AI = new npc_Prospector_Anvilward (_Creature);

    AI->AddWaypoint(0, 9294, -6681, 22.2);
    AI->AddWaypoint(1, 9297, -6669, 22.2);
    AI->AddWaypoint(2, 9308, -6659, 22.2);
    AI->AddWaypoint(3, 9306, -6651, 25.5);
    AI->AddWaypoint(4, 9298, -6648, 28.5);
    AI->AddWaypoint(5, 9290, -6652, 31.2,2500);
    AI->AddWaypoint(6, 9288, -6657, 31.2);
    AI->AddWaypoint(7, 9288, -6657, 31.2);

    return (CreatureAI*)AI;
}


bool GossipHello_npc_Prospector_Anvilward(Player *player, Creature *_Creature)
{

    if (player->GetQuestStatus(QUEST_THE_DWARVEN_SPY) == QUEST_STATUS_INCOMPLETE)
    {		
        player->PlayerTalkClass->GetGossipMenu()->AddMenuItem
            (0, "I need a moment of your time, sir.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1,"",0);		
        player->PlayerTalkClass->SendGossipMenu(8239, _Creature->GetGUID());

        return true;
    }	


    return false;
}

bool GossipSelect_npc_Prospector_Anvilward(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (action)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        player->ADD_GOSSIP_ITEM
            ( 0, "Why... yes, of course. I've something to show you right inside this building, Mr. Anvilward.",
            GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF + 2);
        player->SEND_GOSSIP_MENU(8240, _Creature->GetGUID());
        return true;
        break;
    case GOSSIP_ACTION_INFO_DEF+2:
        player->CLOSE_GOSSIP_MENU();
        //atack,defend,walk
        ((npc_escortAI*)(_Creature->AI()))->Start(true, true, false, player->GetGUID());
        return true;	
        break;
    }
    return false;
}

void AddSC_npcs_eversong_woods()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name= "Prospector_Anvilward";
    newscript->GetAI = GetAI_Prospector_Anvilward;
    newscript->pGossipHello		  = &GossipHello_npc_Prospector_Anvilward;
    newscript->pGossipSelect		 = &GossipSelect_npc_Prospector_Anvilward;
    m_scripts[nrscripts++] = newscript;
}
