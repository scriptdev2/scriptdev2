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
SDComment: Quest support: 731, 2078
SDCategory: Darkshore
EndScriptData */

/* ContentData
npc_prospector_remtravel
npc_threshwackonator
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*####
# npc_prospector_remtravel
####*/

enum
{
    SAY_REM_START               = -1000327,
    SAY_REM_AGGRO               = -1000339,
    SAY_REM_RAMP1_1             = -1000328,
    SAY_REM_RAMP1_2             = -1000329,
    SAY_REM_BOOK                = -1000330,
    SAY_REM_TENT1_1             = -1000331,
    SAY_REM_TENT1_2             = -1000332,
    SAY_REM_MOSS                = -1000333,
    EMOTE_REM_MOSS              = -1000334,
    SAY_REM_MOSS_PROGRESS       = -1000335,
    SAY_REM_PROGRESS            = -1000336,
    SAY_REM_REMEMBER            = -1000337,
    EMOTE_REM_END               = -1000338,

    FACTION_ESCORTEE            = 10,
    QUEST_ABSENT_MINDED_PT2     = 731,
    NPC_GRAVEL_SCOUT            = 2158,
    NPC_GRAVEL_BONE             = 2159,
    NPC_GRAVEL_GEO              = 2160
};

struct MANGOS_DLL_DECL npc_prospector_remtravelAI : public npc_escortAI
{
    npc_prospector_remtravelAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    void WaypointReached(uint32 i)
    {
        Unit* pUnit = Unit::GetUnit(*m_creature, PlayerGUID);

        if (!pUnit || pUnit->GetTypeId() != TYPEID_PLAYER)
            return;

        switch(i)
        {
            case 0:
                DoScriptText(SAY_REM_START, m_creature, pUnit);
                break;
            case 5:
                DoScriptText(SAY_REM_RAMP1_1, m_creature, pUnit);
                break;
            case 6:
                DoSpawnCreature(NPC_GRAVEL_SCOUT, -10.0f, 5.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                DoSpawnCreature(NPC_GRAVEL_BONE, -10.0f, 7.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                break;
            case 9:
                DoScriptText(SAY_REM_RAMP1_2, m_creature, pUnit);
                break;
            case 14:
                //depend quest rewarded?
                DoScriptText(SAY_REM_BOOK, m_creature, pUnit);
                break;
            case 15:
                DoScriptText(SAY_REM_TENT1_1, m_creature, pUnit);
                break;
            case 16:
                DoSpawnCreature(NPC_GRAVEL_SCOUT, -10.0f, 5.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                DoSpawnCreature(NPC_GRAVEL_BONE, -10.0f, 7.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                break;
            case 17:
                DoScriptText(SAY_REM_TENT1_2, m_creature, pUnit);
                break;
            case 26:
                DoScriptText(SAY_REM_MOSS, m_creature, pUnit);
                break;
            case 27:
                DoScriptText(EMOTE_REM_MOSS, m_creature, pUnit);
                break;
            case 28:
                DoScriptText(SAY_REM_MOSS_PROGRESS, m_creature, pUnit);
                break;
            case 29:
                DoSpawnCreature(NPC_GRAVEL_SCOUT, -15.0f, 3.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                DoSpawnCreature(NPC_GRAVEL_BONE, -15.0f, 5.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                DoSpawnCreature(NPC_GRAVEL_GEO, -15.0f, 7.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                break;
            case 31:
                DoScriptText(SAY_REM_PROGRESS, m_creature, pUnit);
                break;
            case 41:
                DoScriptText(SAY_REM_REMEMBER, m_creature, pUnit);
                break;
            case 42:
                DoScriptText(EMOTE_REM_END, m_creature, pUnit);
                ((Player*)pUnit)->GroupEventHappens(QUEST_ABSENT_MINDED_PT2,m_creature);
                break;
        }
    }

    void Reset() { }

    void Aggro(Unit* who)
    {
        if (rand()%2)
            DoScriptText(SAY_REM_AGGRO, m_creature, who);
    }

    void JustSummoned(Creature* pSummoned)
    {
        //unsure if it should be any
        //pSummoned->AI()->AttackStart(m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_prospector_remtravel(Creature* pCreature)
{
    npc_prospector_remtravelAI* tempAI = new npc_prospector_remtravelAI(pCreature);

    tempAI->FillPointMovementListForCreature();

    return (CreatureAI*)tempAI;
}

bool QuestAccept_npc_prospector_remtravel(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_ABSENT_MINDED_PT2)
    {
        ((npc_escortAI*)(pCreature->AI()))->Start(false, true, false, pPlayer->GetGUID());
        pCreature->setFaction(FACTION_ESCORTEE);
    }

    return true;
}

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
    npc_threshwackonatorAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        uiFaction = pCreature->getFaction();
        uiNpcFlags = pCreature->GetUInt32Value(UNIT_NPC_FLAGS);
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

        DoScriptText(EMOTE_START,m_creature);
    }

    void DoAtEnd()
    {
        m_creature->setFaction(FACTION_HOSTILE);

        if (Unit* pHolder = Unit::GetUnit(*m_creature,uiPlayerGUID))
            m_creature->AI()->AttackStart(pHolder);

        uiPlayerGUID = 0;
    }

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

CreatureAI* GetAI_npc_threshwackonator(Creature* pCreature)
{
    return new npc_threshwackonatorAI(pCreature);
}

bool GossipHello_npc_threshwackonator(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_GYROMAST_REV) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INSERT_KEY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

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
    newscript->Name = "npc_prospector_remtravel";
    newscript->GetAI = &GetAI_npc_prospector_remtravel;
    newscript->pQuestAccept = &QuestAccept_npc_prospector_remtravel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_threshwackonator";
    newscript->GetAI = &GetAI_npc_threshwackonator;
    newscript->pGossipHello = &GossipHello_npc_threshwackonator;
    newscript->pGossipSelect = &GossipSelect_npc_threshwackonator;
    newscript->RegisterSelf();
}
