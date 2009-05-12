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
SDName: Mulgore
SD%Complete: 100
SDComment: Quest support: 11129. Skorn Whitecloud: Just a story if not rewarded for quest
SDCategory: Mulgore
EndScriptData */

/* ContentData
npc_kyle_the_frenzied
npc_skorn_whitecloud
EndContentData */

#include "precompiled.h"

/*######
# npc_kyle_the_frenzied
######*/

enum
{
    EMOTE_SEE_LUNCH         = -1000340,
    EMOTE_EAT_LUNCH         = -1000341,
    EMOTE_DANCE             = -1000342,

    SPELL_LUNCH             = 42222,
    NPC_KYLE_FRENZIED       = 23616,
    NPC_KYLE_FRIENDLY       = 23622,
    POINT_ID                = 1
};

struct MANGOS_DLL_DECL npc_kyle_the_frenziedAI : public ScriptedAI
{
    npc_kyle_the_frenziedAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    bool bEvent;
    uint64 uiPlayerGUID;
    uint32 uiEventTimer;
    uint8 uiEventPhase;

    void Reset()
    {
        bEvent = false;
        uiPlayerGUID = 0;
        uiEventTimer = 5000;
        uiEventPhase = 1;

        if (m_creature->GetEntry() == NPC_KYLE_FRIENDLY)
            m_creature->UpdateEntry(NPC_KYLE_FRENZIED);
    }

    void SpellHit(Unit* pCaster, SpellEntry const* pSpell)
    {
        if (!m_creature->isInCombat() && !bEvent && pSpell->Id == SPELL_LUNCH)
        {
            if (pCaster->GetTypeId() == TYPEID_PLAYER)
                uiPlayerGUID = pCaster->GetGUID();

            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
            {
                m_creature->GetMotionMaster()->MovementExpired();
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->StopMoving();
            }

            bEvent = true;
            DoScriptText(EMOTE_SEE_LUNCH, m_creature);
            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_CREATURE_SPECIAL);
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE || !bEvent)
            return;

        if (uiPointId == POINT_ID)
            uiEventTimer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (bEvent)
        {
            if (!uiEventTimer)
                return;

            if (uiEventTimer < diff)
            {
                switch(uiEventPhase)
                {
                    case 1:
                        uiEventTimer = 0;

                        if (Unit* pUnit = Unit::GetUnit(*m_creature,uiPlayerGUID))
                        {
                            if (GameObject* pGo = pUnit->GetGameObject(SPELL_LUNCH))
                                m_creature->GetMotionMaster()->MovePoint(POINT_ID, pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ());
                        }
                        break;
                    case 2:
                        uiEventTimer = 5000;
                        DoScriptText(EMOTE_EAT_LUNCH, m_creature);
                        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_USESTANDING);
                        break;
                    case 3:
                        uiEventTimer = 5000;

                        if (Unit* pUnit = Unit::GetUnit(*m_creature,uiPlayerGUID))
                            ((Player*)pUnit)->TalkedToCreature(m_creature->GetEntry(), m_creature->GetGUID());

                        m_creature->UpdateEntry(NPC_KYLE_FRIENDLY);
                        break;
                    case 4:
                        uiEventTimer = 30000;
                        DoScriptText(EMOTE_DANCE, m_creature);
                        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DANCESPECIAL);
                        break;
                    case 5:
                        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                        Reset();
                        m_creature->GetMotionMaster()->Clear();
                        break;
                }

                if (uiEventPhase != 5)
                    ++uiEventPhase;
            }
            else
                uiEventTimer -= diff;
        }
    }
};

CreatureAI* GetAI_npc_kyle_the_frenzied(Creature* pCreature)
{
    return new npc_kyle_the_frenziedAI(pCreature);
}

/*######
# npc_skorn_whitecloud
######*/

bool GossipHello_npc_skorn_whitecloud(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (!pPlayer->GetQuestRewardStatus(770))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tell me a story, Skorn.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(522, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_skorn_whitecloud(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF)
        pPlayer->SEND_GOSSIP_MENU(523, pCreature->GetGUID());

    return true;
}

void AddSC_mulgore()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_kyle_the_frenzied";
    newscript->GetAI = &GetAI_npc_kyle_the_frenzied;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_skorn_whitecloud";
    newscript->pGossipHello = &GossipHello_npc_skorn_whitecloud;
    newscript->pGossipSelect = &GossipSelect_npc_skorn_whitecloud;
    newscript->RegisterSelf();
}
