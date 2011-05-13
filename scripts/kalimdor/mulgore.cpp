/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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

    bool m_bEvent;
    bool m_bIsMovingToLunch;
    ObjectGuid m_playerGuid;
    uint32 m_uiEventTimer;
    uint8 m_uiEventPhase;

    void Reset()
    {
        m_bEvent = false;
        m_bIsMovingToLunch = false;
        m_playerGuid.Clear();
        m_uiEventTimer = 5000;
        m_uiEventPhase = 0;

        if (m_creature->GetEntry() == NPC_KYLE_FRIENDLY)
            m_creature->UpdateEntry(NPC_KYLE_FRENZIED);
    }

    void SpellHit(Unit* pCaster, SpellEntry const* pSpell)
    {
        if (!m_creature->getVictim() && !m_bEvent && pSpell->Id == SPELL_LUNCH)
        {
            if (pCaster->GetTypeId() == TYPEID_PLAYER)
                m_playerGuid = pCaster->GetObjectGuid();

            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
            {
                m_creature->GetMotionMaster()->MovementExpired();
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->StopMoving();
            }

            m_bEvent = true;
            DoScriptText(EMOTE_SEE_LUNCH, m_creature);
            m_creature->HandleEmote(EMOTE_ONESHOT_CREATURE_SPECIAL);
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE || !m_bEvent)
            return;

        if (uiPointId == POINT_ID)
            m_bIsMovingToLunch = false;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_bEvent)
        {
            if (m_bIsMovingToLunch)
                return;

            if (m_uiEventTimer < diff)
            {
                m_uiEventTimer = 5000;
                ++m_uiEventPhase;

                switch(m_uiEventPhase)
                {
                    case 1:
                        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                        {
                            GameObject* pGo = pPlayer->GetGameObject(SPELL_LUNCH);

                            // Workaround for broken function GetGameObject
                            if (!pGo)
                            {
                                const SpellEntry* pSpell = GetSpellStore()->LookupEntry(SPELL_LUNCH);

                                uint32 uiGameobjectEntry = pSpell->EffectMiscValue[EFFECT_INDEX_1];

                                pGo = GetClosestGameObjectWithEntry(pPlayer, uiGameobjectEntry, 2*INTERACTION_DISTANCE);
                            }

                            if (pGo)
                            {
                                m_bIsMovingToLunch = true;

                                float fX, fY, fZ;
                                pGo->GetContactPoint(m_creature, fX, fY, fZ, CONTACT_DISTANCE);

                                m_creature->GetMotionMaster()->MovePoint(POINT_ID, fX, fY, fZ);
                            }
                        }
                        break;
                    case 2:
                        DoScriptText(EMOTE_EAT_LUNCH, m_creature);
                        m_creature->HandleEmote(EMOTE_STATE_USESTANDING);
                        break;
                    case 3:
                        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                            pPlayer->TalkedToCreature(m_creature->GetEntry(), m_creature->GetObjectGuid());

                        m_creature->UpdateEntry(NPC_KYLE_FRIENDLY);
                        break;
                    case 4:
                        m_uiEventTimer = 30000;
                        DoScriptText(EMOTE_DANCE, m_creature);
                        m_creature->HandleEmote(EMOTE_STATE_DANCESPECIAL);
                        break;
                    case 5:
                        m_creature->HandleEmote(EMOTE_STATE_NONE);
                        Reset();
                        m_creature->GetMotionMaster()->Clear();
                        break;
                }
            }
            else
                m_uiEventTimer -= diff;
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
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (!pPlayer->GetQuestRewardStatus(770))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tell me a story, Skorn.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(522, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_skorn_whitecloud(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
        pPlayer->SEND_GOSSIP_MENU(523, pCreature->GetObjectGuid());

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
