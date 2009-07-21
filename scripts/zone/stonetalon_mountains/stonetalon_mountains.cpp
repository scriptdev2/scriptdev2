/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Stonetalon_Mountains
SD%Complete: 95
SDComment: Quest support: 6627 (Braug Dimspirits questions/'answers' might have more to it, need more info),6523
SDCategory: Stonetalon Mountains
EndScriptData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*######
## npc_braug_dimspirit
######*/

bool GossipHello_npc_braug_dimspirit(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(6627) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Ysera", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Neltharion", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nozdormu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Alexstrasza", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Malygos", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->SEND_GOSSIP_MENU(5820, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(5819, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_braug_dimspirit(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->CastSpell(pPlayer,6766,false);

    }
    if (action == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->AreaExploredOrEventHappens(6627);
    }
    return true;
}

/*######
## npc_kaya
######*/

enum
{
    FACTION_ESCORTEE_H          = 775,

    NPC_GRIMTOTEM_RUFFIAN       = 11910,
    NPC_GRIMTOTEM_BRUTE         = 11912,
    NPC_GRIMTOTEM_SORCERER      = 11913,

    SAY_START                   = -1000357,
    SAY_AMBUSH                  = -1000358,
    SAY_END                     = -1000359,

    QUEST_PROTECT_KAYA          = 6523
};

struct MANGOS_DLL_DECL npc_kayaAI : public npc_escortAI
{
    npc_kayaAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        uiNormFaction = pCreature->getFaction();
        Reset();
    }

    uint32 uiNormFaction;

    void Reset()
    {
        if (!IsBeingEscorted)
            m_creature->setFaction(uiNormFaction);
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(m_creature);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            //Ambush
            case 16:
                //note about event here:
                //apparently NPC say _after_ the ambush is over, and is most likely a bug at you-know-where.
                //we simplify this, and make say when the ambush actually start.
                DoScriptText(SAY_AMBUSH, m_creature);
                m_creature->SummonCreature(NPC_GRIMTOTEM_RUFFIAN, -50.75, -500.77, -46.13, 0.4, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                m_creature->SummonCreature(NPC_GRIMTOTEM_BRUTE, -40.05, -510.89,- 46.05, 1.7, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                m_creature->SummonCreature(NPC_GRIMTOTEM_SORCERER, -32.21, -499.20, -45.35, 2.8, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                break;
            // Award quest credit
            case 18:
                DoScriptText(SAY_END, m_creature);

                Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID);

                if (pPlayer && pPlayer->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)pPlayer)->GroupEventHappens(QUEST_PROTECT_KAYA, m_creature);
                break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID))
        {
            // If NPC dies, player fails the quest
            if (pPlayer->GetTypeId() == TYPEID_PLAYER && ((Player*)pPlayer)->GetQuestStatus(QUEST_PROTECT_KAYA) == QUEST_STATUS_INCOMPLETE)
                ((Player*)pPlayer)->FailQuest(QUEST_PROTECT_KAYA);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        npc_escortAI::UpdateAI(uiDiff);
    }
};

CreatureAI* GetAI_npc_kaya(Creature* pCreature)
{
    npc_kayaAI* kayaAI = new npc_kayaAI(pCreature);

    kayaAI->FillPointMovementListForCreature();

    return (CreatureAI*)kayaAI;
}

bool QuestAccept_npc_kaya(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    //Casting Spell and Starting the Escort quest is buggy, so this is a hack. Use the spell when it is possible.

    if (pQuest->GetQuestId() == QUEST_PROTECT_KAYA)
    {
        pCreature->setFaction(FACTION_ESCORTEE_H);
        DoScriptText(SAY_START,pCreature);

        if (npc_kayaAI* pEscortAI = dynamic_cast<npc_kayaAI*>(pCreature->AI()))
            pEscortAI->Start(true, false, pPlayer->GetGUID(), pQuest);
    }
    return true;
}

/*######
## AddSC
######*/

void AddSC_stonetalon_mountains()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_braug_dimspirit";
    newscript->pGossipHello = &GossipHello_npc_braug_dimspirit;
    newscript->pGossipSelect = &GossipSelect_npc_braug_dimspirit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_kaya";
    newscript->GetAI = &GetAI_npc_kaya;
    newscript->pQuestAccept = &QuestAccept_npc_kaya;
    newscript->RegisterSelf();
}
