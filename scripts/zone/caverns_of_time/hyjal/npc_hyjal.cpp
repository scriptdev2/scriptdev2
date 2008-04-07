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

#include "hyjalAI.h"
#include "def_hyjal.h"

#define GOSSIP_ITEM_BEGIN_ALLY  "We are ready to defend the Alliance base."
#define GOSSIP_ITEM_ANETHERON   "The defenses are holding up; we can continue."
#define GOSSIP_ITEM_RETREAT     "We can't keep this up. Let's retreat!"

#define GOSSIP_ITEM_BEGIN_HORDE "We're here to help! The Alliance are overrun."
#define GOSSIP_ITEM_AZGALOR     "We're okay so far. Let's do this!"

#define ALLIANCE 0
#define HORDE    1

#define HORDEBASE_X         5464.5522
#define HORDEBASE_Y         -2731.5644
#define HORDEBASE_Z         1485.7075

#define NIGHTELFBASE_X      5186.07
#define NIGHTELFBASE_Y      -3383.49
#define NIGHTELFBASE_Z      1638.28

CreatureAI* GetAI_npc_jaina_proudmoore(Creature *_Creature)
{
    hyjalAI* ai = new hyjalAI(_Creature);

    ai->Reset();
    ai->SetFaction(ALLIANCE);
    
    return ai;
}

bool GossipHello_npc_jaina_proudmoore(Player *player, Creature *_Creature)
{
    if((!((hyjalAI*)_Creature->AI())->EventBegun) && (!((hyjalAI*)_Creature->AI())->FirstBossDead))
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_BEGIN_ALLY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    else
    {
        if((!((hyjalAI*)_Creature->AI())->SecondBossDead))
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_ANETHERON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        else
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_RETREAT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    }
    player->PlayerTalkClass->SendGossipMenu(907, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_jaina_proudmoore(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    player->PlayerTalkClass->GetGossipMenu();
    switch(action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            ((hyjalAI*)_Creature->AI())->StartEvent(player);
            ((hyjalAI*)_Creature->AI())->SetInstanceData(DATA_RAGEWINTERCHILLEVENT, ENCOUNTER_IN_PROGRESS);
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            ((hyjalAI*)_Creature->AI())->StartEvent(player);
            ((hyjalAI*)_Creature->AI())->SetInstanceData(DATA_RAGEWINTERCHILLEVENT, ENCOUNTER_COMPLETE);
            ((hyjalAI*)_Creature->AI())->SetInstanceData(DATA_ANETHERONEVENT, ENCOUNTER_IN_PROGRESS);
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            ((hyjalAI*)_Creature->AI())->SetInstanceData(DATA_ANETHERONEVENT, ENCOUNTER_COMPLETE);
            ((hyjalAI*)_Creature->AI())->TeleportRaid(player, HORDEBASE_X, HORDEBASE_Y, HORDEBASE_Z);
            break;
    }

    return true;
}

CreatureAI* GetAI_npc_thrall(Creature *_Creature)
{
    hyjalAI* ai = new hyjalAI(_Creature);

    ai->Reset();
    ai->EnterEvadeMode();
    ai->SetFaction(HORDE);

    return ai;
}

bool GossipHello_npc_thrall(Player *player, Creature *_Creature)
{
    uint32 AnetheronEvent = 0;
    AnetheronEvent = ((hyjalAI*)_Creature->AI())->GetInstanceData(DATA_ANETHERONEVENT);
    if(AnetheronEvent >= 3) // Only let them start the Horde phase if Anetheron is dead.
    {
        if((((hyjalAI*)_Creature->AI())->EventBegun) && (!((hyjalAI*)_Creature->AI())->FirstBossDead))
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_BEGIN_HORDE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        else
        {
            if((!((hyjalAI*)_Creature->AI())->SecondBossDead))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_AZGALOR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            else
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_RETREAT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        }
    }

    player->PlayerTalkClass->SendGossipMenu(907, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_thrall(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    player->PlayerTalkClass->GetGossipMenu();
    switch(action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            ((hyjalAI*)_Creature->AI())->StartEvent(player);
            ((hyjalAI*)_Creature->AI())->SetInstanceData(DATA_KAZROGALEVENT, ENCOUNTER_IN_PROGRESS);
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            ((hyjalAI*)_Creature->AI())->StartEvent(player);
            ((hyjalAI*)_Creature->AI())->SetInstanceData(DATA_KAZROGALEVENT, ENCOUNTER_COMPLETE);
            ((hyjalAI*)_Creature->AI())->SetInstanceData(DATA_AZGALOREVENT, ENCOUNTER_IN_PROGRESS);
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            ((hyjalAI*)_Creature->AI())->SetInstanceData(DATA_AZGALOREVENT, ENCOUNTER_COMPLETE);
            ((hyjalAI*)_Creature->AI())->TeleportRaid(player, NIGHTELFBASE_X, NIGHTELFBASE_Y, NIGHTELFBASE_Z);
            break;
    }

    return true;
}

bool GossipHello_npc_tyrande_whisperwind(Player* player, Creature* _Creature)
{
    player->ADD_GOSSIP_ITEM(1, "Aid us in defending Nordrassil", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
    player->PlayerTalkClass->SendGossipMenu(907, _Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_tyrande_whisperwind(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_TRADE)
        player->SEND_VENDORLIST( _Creature->GetGUID() );

    return true;
}

void AddSC_hyjal()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_jaina_proudmoore";
    newscript->GetAI = GetAI_npc_jaina_proudmoore;
    newscript->pGossipHello = &GossipHello_npc_jaina_proudmoore;
    newscript->pGossipSelect = &GossipSelect_npc_jaina_proudmoore;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "npc_thrall";
    newscript->GetAI = GetAI_npc_thrall;
    newscript->pGossipHello = &GossipHello_npc_thrall;
    newscript->pGossipSelect = &GossipSelect_npc_thrall;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "npc_tyrande_whisperwind";
    newscript->pGossipHello = &GossipHello_npc_tyrande_whisperwind;
    newscript->pGossipSelect = &GossipSelect_npc_tyrande_whisperwind;
    m_scripts[nrscripts++] = newscript;
}
