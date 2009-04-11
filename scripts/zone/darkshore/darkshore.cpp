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
SDName: Darkshore
SD%Complete: 100
SDComment: Quest support: 2078
SDCategory: Darkshore
EndScriptData */

/* ContentData
npc_threshwackonator
EndContentData */

#include "precompiled.h"

/*####
# npc_threshwackonator
####*/

enum
{
    EMOTE_START             = -1000325,
    SAY_AT_CLOSE            = -1000326,
    QUEST_GYROMAST_REV      = 2078,
    NPC_GELKAK              = 6667,
    FACTION_HOSTILE         = 14
};

#define GOSSIP_ITEM_INSERT_KEY  "[PH] Insert key"

struct MANGOS_DLL_DECL npc_threshwackonatorAI : public ScriptedAI
{
    npc_threshwackonatorAI(Creature *c) : ScriptedAI(c)
    {
        uiFaction = c->getFaction();
        uiNpcFlags = c->GetUInt32Value(UNIT_NPC_FLAGS);
        uiPlayerGUID = 0;
        Reset();
    }

    uint64 uiPlayerGUID;
    uint32 uiFaction;
    uint32 uiNpcFlags;
    uint32 uiCheckPlayerTimer;

    void Reset()
    {
        uiCheckPlayerTimer = 2500;

        if (!uiPlayerGUID)
        {
            m_creature->setFaction(uiFaction);
            m_creature->SetUInt32Value(UNIT_NPC_FLAGS, uiNpcFlags);
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->GetEntry() == NPC_GELKAK)
        {
            if (uiPlayerGUID && m_creature->IsWithinDistInMap(pWho, 10.0f))
            {
                DoScriptText(SAY_AT_CLOSE, pWho);
                DoAtEnd();
            }
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void EnterEvadeMode()
    {
        m_creature->InterruptNonMeleeSpells(true);
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        m_creature->LoadCreaturesAddon();

        if (m_creature->isAlive())
        {
            if (Unit* pUnit = Unit::GetUnit(*m_creature,uiPlayerGUID))
                m_creature->GetMotionMaster()->MoveFollow(pUnit, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
            else
            {
                m_creature->GetMotionMaster()->MovementExpired();
                m_creature->GetMotionMaster()->MoveTargetedHome();
            }
        }

        m_creature->SetLootRecipient(NULL);

        InCombat = false;
        Reset();
    }

    void DoStart(uint64 uiPlayer)
    {
        uiPlayerGUID = uiPlayer;
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);

        if (Unit* pUnit = Unit::GetUnit(*m_creature,uiPlayer))
            m_creature->GetMotionMaster()->MoveFollow(pUnit, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);

        DoScriptText(EMOTE_START,m_creature);
    }

    void DoAtEnd()
    {
        m_creature->setFaction(FACTION_HOSTILE);

        if (Unit* pHolder = Unit::GetUnit(*m_creature,uiPlayerGUID))
            m_creature->AI()->AttackStart(pHolder);

        uiPlayerGUID = 0;
    }

    void Aggro(Unit* who) { }

    void JustDied(Unit* pKiller)
    {
        if (uiPlayerGUID)
        {
            uiPlayerGUID = 0;
            m_creature->GetMotionMaster()->MovementExpired();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (uiPlayerGUID)
        {
            if (!InCombat)
            {
                if (uiCheckPlayerTimer < diff)
                {
                    uiCheckPlayerTimer = 5000;

                    Unit* pUnit = Unit::GetUnit(*m_creature,uiPlayerGUID);

                    if (pUnit && !pUnit->isAlive())
                    {
                        uiPlayerGUID = 0;
                        EnterEvadeMode();
                    }
                }
                else
                    uiCheckPlayerTimer -= diff;
            }
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_threshwackonator(Creature* pCreature)
{
    return new npc_threshwackonatorAI(pCreature);
}

bool GossipHello_npc_threshwackonator(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_GYROMAST_REV) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_INSERT_KEY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_threshwackonator(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        ((npc_threshwackonatorAI*)(pCreature->AI()))->DoStart(pPlayer->GetGUID());
        pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

void AddSC_darkshore()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_threshwackonator";
    newscript->GetAI = &GetAI_npc_threshwackonator;
    newscript->pGossipHello = &GossipHello_npc_threshwackonator;
    newscript->pGossipSelect = &GossipSelect_npc_threshwackonator;
    newscript->RegisterSelf();
}
