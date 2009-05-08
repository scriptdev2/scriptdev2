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
SDName: Teldrassil
SD%Complete: 100
SDComment: Quest support: 938
SDCategory: Teldrassil
EndScriptData */

/* ContentData
npc_mist
EndContentData */

#include "precompiled.h"

/*####
# npc_mist
####*/

enum
{
    SAY_AT_HOME             = -1000323,
    EMOTE_AT_HOME           = -1000324,
    QUEST_MIST              = 938,
    NPC_ARYNIA              = 3519,
};

struct MANGOS_DLL_DECL npc_mistAI : public ScriptedAI
{
    npc_mistAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        uiNpcFlags = pCreature->GetUInt32Value(UNIT_NPC_FLAGS);
        uiPlayerGUID = 0;
        Reset();
    }

    uint64 uiPlayerGUID;
    uint32 uiNpcFlags;
    uint32 uiCheckPlayerTimer;

    void Reset()
    {
        uiCheckPlayerTimer = 2500;

        if (!uiPlayerGUID)
            m_creature->SetUInt32Value(UNIT_NPC_FLAGS, uiNpcFlags);
    }

    void MoveInLineOfSight(Unit *pWho)
    {
        if (pWho->GetEntry() == NPC_ARYNIA)
        {
            if (m_creature->IsWithinDistInMap(pWho, 10.0f))
            {
                DoScriptText(SAY_AT_HOME, pWho);
                DoComplete();
            }
        }
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
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

        Reset();
    }

    void DoStart(uint64 uiPlayer)
    {
        uiPlayerGUID = uiPlayer;
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);

        if (Unit* pUnit = Unit::GetUnit(*m_creature,uiPlayer))
            m_creature->GetMotionMaster()->MoveFollow(pUnit, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
    }

    void DoComplete()
    {
        DoScriptText(EMOTE_AT_HOME, m_creature);

        if (Unit* pUnit = Unit::GetUnit(*m_creature,uiPlayerGUID))
        {
            if (pUnit->GetTypeId() == TYPEID_PLAYER)
            {
                if (((Player*)pUnit)->GetQuestStatus(QUEST_MIST) == QUEST_STATUS_INCOMPLETE)
                {
                    uint16 uiQuestLogSlot = ((Player*)pUnit)->FindQuestSlot(QUEST_MIST);

                    if (uiQuestLogSlot < MAX_QUEST_LOG_SIZE)
                    {
                        if (((Player*)pUnit)->GetQuestSlotState(uiQuestLogSlot) != QUEST_STATE_FAIL)
                            ((Player*)pUnit)->AreaExploredOrEventHappens(QUEST_MIST);
                    }
                }
            }
        }

        uiPlayerGUID = 0;
        EnterEvadeMode();
    }

    void JustDied(Unit* pKiller)
    {
        if (Unit* pUnit = Unit::GetUnit(*m_creature,uiPlayerGUID))
            ((Player*)pUnit)->FailTimedQuest(QUEST_MIST);

        uiPlayerGUID = 0;
        m_creature->GetMotionMaster()->MovementExpired();
    }

    void UpdateAI(const uint32 diff)
    {
        if (uiPlayerGUID)
        {
            if (!m_creature->isInCombat())
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

CreatureAI* GetAI_npc_mist(Creature* pCreature)
{
    return new npc_mistAI(pCreature);
}

bool QuestAccept_npc_mist(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_MIST)
        ((npc_mistAI*)(pCreature->AI()))->DoStart(pPlayer->GetGUID());

    return true;
}

void AddSC_teldrassil()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_mist";
    newscript->GetAI = &GetAI_npc_mist;
    newscript->pQuestAccept = &QuestAccept_npc_mist;
    newscript->RegisterSelf();
}
