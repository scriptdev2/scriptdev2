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
SDName: Shadowmoon_Valley
SD%Complete: 100
SDComment: Quest support: 10519, 10583, 10601, 10814, 10804, 10854, 11082. Vendor Drake Dealer Hurlunk.
SDCategory: Shadowmoon Valley
EndScriptData */

/* ContentData
mob_mature_netherwing_drake
mob_enslaved_netherwing_drake
npc_drake_dealer_hurlunk
npcs_flanis_swiftwing_and_kagrosh
npc_murkblood_overseer
npc_neltharaku
npc_karynaku
npc_oronok_tornheart
EndContentData */

#include "precompiled.h"

/*#####
# mob_mature_netherwing_drake
#####*/

enum
{
    SAY_JUST_EATEN              = -1000175,

    SPELL_PLACE_CARCASS         = 38439,
    SPELL_JUST_EATEN            = 38502,
    SPELL_NETHER_BREATH         = 38467,
    POINT_ID                    = 1,

    QUEST_KINDNESS              = 10804,
    NPC_EVENT_PINGER            = 22131
};

struct MANGOS_DLL_DECL mob_mature_netherwing_drakeAI : public ScriptedAI
{
    mob_mature_netherwing_drakeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint64 uiPlayerGUID;

    bool bCanEat;
    bool bIsEating;

    uint32 EatTimer;
    uint32 CastTimer;

    void Reset()
    {
        uiPlayerGUID = 0;

        bCanEat = false;
        bIsEating = false;

        EatTimer = 5000;
        CastTimer = 5000;
    }

    void SpellHit(Unit* pCaster, SpellEntry const* pSpell)
    {
        if (bCanEat || bIsEating)
            return;

        if (pCaster->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_PLACE_CARCASS && !m_creature->HasAura(SPELL_JUST_EATEN))
        {
            uiPlayerGUID = pCaster->GetGUID();
            bCanEat = true;
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == POINT_ID)
        {
            bIsEating = true;
            EatTimer = 7000;
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ATTACKUNARMED);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (bCanEat || bIsEating)
        {
            if (EatTimer < diff)
            {
                if (bCanEat && !bIsEating)
                {
                    if (Unit* pUnit = Unit::GetUnit(*m_creature, uiPlayerGUID))
                    {
                        if (GameObject* pGo = pUnit->GetGameObject(SPELL_PLACE_CARCASS))
                        {
                            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
                                m_creature->GetMotionMaster()->MovementExpired();

                            m_creature->GetMotionMaster()->MoveIdle();
                            m_creature->StopMoving();

                            m_creature->GetMotionMaster()->MovePoint(POINT_ID, pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ());
                        }
                    }
                    bCanEat = false;
                }
                else if (bIsEating)
                {
                    DoCast(m_creature, SPELL_JUST_EATEN);
                    DoScriptText(SAY_JUST_EATEN, m_creature);

                    if (Player* pPlr = (Player*)Unit::GetUnit((*m_creature), uiPlayerGUID))
                        pPlr->KilledMonster(NPC_EVENT_PINGER, m_creature->GetGUID());

                    Reset();
                    m_creature->GetMotionMaster()->Clear();
                }
            }
            else
                EatTimer -= diff;

            return;
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (CastTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_NETHER_BREATH);
            CastTimer = 5000;
        }else CastTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_mature_netherwing_drake(Creature* pCreature)
{
    return new mob_mature_netherwing_drakeAI(pCreature);
}

/*###
# mob_enslaved_netherwing_drake
####*/

#define FACTION_DEFAULT     62
#define FACTION_FRIENDLY    1840                            // Not sure if this is correct, it was taken off of Mordenai.

#define SPELL_HIT_FORCE_OF_NELTHARAKU   38762
#define SPELL_FORCE_OF_NELTHARAKU       38775

#define CREATURE_DRAGONMAW_SUBJUGATOR   21718
#define CREATURE_ESCAPE_DUMMY           22317

struct MANGOS_DLL_DECL mob_enslaved_netherwing_drakeAI : public ScriptedAI
{
    mob_enslaved_netherwing_drakeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        PlayerGUID = 0;
        Tapped = false;
        Reset();
    }

    uint64 PlayerGUID;
    uint32 FlyTimer;
    bool Tapped;

    void Reset()
    {
        if (!Tapped)
            m_creature->setFaction(FACTION_DEFAULT);

        FlyTimer = 10000;
        m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
        m_creature->SetVisibility(VISIBILITY_ON);
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (!caster)
            return;

        if (caster->GetTypeId() == TYPEID_PLAYER && spell->Id == SPELL_HIT_FORCE_OF_NELTHARAKU && !Tapped)
        {
            Tapped = true;
            PlayerGUID = caster->GetGUID();

            m_creature->setFaction(FACTION_FRIENDLY);
            DoCast(caster, SPELL_FORCE_OF_NELTHARAKU, true);

            if (Creature* Dragonmaw = GetClosestCreatureWithEntry(m_creature, CREATURE_DRAGONMAW_SUBJUGATOR, 50.0f))
            {
                m_creature->AddThreat(Dragonmaw, 100000.0f);
                AttackStart(Dragonmaw);
            }

            HostilReference* ref = m_creature->getThreatManager().getOnlineContainer().getReferenceByTarget(caster);
            if (ref)
                ref->removeReference();
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == 1)
        {
            if (PlayerGUID)
            {
                Unit* plr = Unit::GetUnit((*m_creature), PlayerGUID);
                if (plr)
                    DoCast(plr, SPELL_FORCE_OF_NELTHARAKU, true);

                PlayerGUID = 0;
            }
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            m_creature->RemoveCorpse();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
        {
            if (Tapped)
                if (FlyTimer < diff)
            {
                Tapped = false;
                if (PlayerGUID)
                {
                    Player* plr = ((Player*)Unit::GetUnit((*m_creature), PlayerGUID));
                    if (plr && plr->GetQuestStatus(10854) == QUEST_STATUS_INCOMPLETE)
                    {
                        plr->KilledMonster(22316, m_creature->GetGUID());
                        /*
                        float x,y,z;
                        m_creature->GetPosition(x,y,z);

                        float dx,dy,dz;
                        m_creature->GetRandomPoint(x, y, z, 20, dx, dy, dz);
                        dz += 20; // so it's in the air, not ground*/

                        float dx, dy, dz;

                        if (Creature* EscapeDummy = GetClosestCreatureWithEntry(m_creature, CREATURE_ESCAPE_DUMMY, 30.0f))
                            EscapeDummy->GetPosition(dx, dy, dz);
                        else
                        {
                            m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 20, dx, dy, dz);
                            dz += 25;
                        }

                        m_creature->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
                        m_creature->GetMotionMaster()->MovePoint(1, dx, dy, dz);
                    }
                }
            }else FlyTimer -= diff;
            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_enslaved_netherwing_drake(Creature* pCreature)
{
    return new mob_enslaved_netherwing_drakeAI(pCreature);
}

/*#####
# mob_dragonmaw_peon
#####*/

struct MANGOS_DLL_DECL mob_dragonmaw_peonAI : public ScriptedAI
{
    mob_dragonmaw_peonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint64 PlayerGUID;
    bool Tapped;
    uint32 PoisonTimer;

    void Reset()
    {
        PlayerGUID = 0;
        Tapped = false;
        PoisonTimer = 0;
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (!caster)
            return;

        if (caster->GetTypeId() == TYPEID_PLAYER && spell->Id == 40468 && !Tapped)
        {
            PlayerGUID = caster->GetGUID();

            Tapped = true;
            float x, y, z;
            caster->GetClosePoint(x, y, z, m_creature->GetObjectSize());

            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
            m_creature->GetMotionMaster()->MovePoint(1, x, y, z);
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id)
        {
            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_EAT);
            PoisonTimer = 15000;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (PoisonTimer)
            if (PoisonTimer <= diff)
        {
            if (PlayerGUID)
            {
                Player* plr = ((Player*)Unit::GetUnit((*m_creature), PlayerGUID));
                if (plr && plr->GetQuestStatus(11020) == QUEST_STATUS_INCOMPLETE)
                    plr->KilledMonster(23209, m_creature->GetGUID());
            }
            PoisonTimer = 0;
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else PoisonTimer -= diff;
    }
};

/*######
## npc_drake_dealer_hurlunk
######*/

bool GossipHello_npc_drake_dealer_hurlunk(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isVendor() && pPlayer->GetReputationRank(1015) == REP_EXALTED)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_drake_dealer_hurlunk(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetGUID());

    return true;
}

/*######
## npc_flanis_swiftwing_and_kagrosh
######*/

bool GossipHello_npcs_flanis_swiftwing_and_kagrosh(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(10583) == QUEST_STATUS_INCOMPLETE && !pPlayer->HasItemCount(30658,1,true))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Take Flanis's Pack", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    if (pPlayer->GetQuestStatus(10601) == QUEST_STATUS_INCOMPLETE && !pPlayer->HasItemCount(30659,1,true))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Take Kagrosh's Pack", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npcs_flanis_swiftwing_and_kagrosh(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        ItemPosCountVec dest;
        uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 30658, 1, false);
        if (msg == EQUIP_ERR_OK)
        {
            pPlayer->StoreNewItem(dest, 30658, 1, true);
            pPlayer->PlayerTalkClass->ClearMenus();
        }
    }
    if (action == GOSSIP_ACTION_INFO_DEF+2)
    {
        ItemPosCountVec dest;
        uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 30659, 1, false);
        if (msg == EQUIP_ERR_OK)
        {
            pPlayer->StoreNewItem(dest, 30659, 1, true);
            pPlayer->PlayerTalkClass->ClearMenus();
        }
    }
    return true;
}

/*######
## npc_murkblood_overseer
######*/

#define QUEST_11082     11082

bool GossipHello_npc_murkblood_overseer(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_11082) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I am here for you, overseer.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(10940, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_murkblood_overseer(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "How dare you question an overseer of the Dragonmaw!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                                                            //correct id not known
            pPlayer->SEND_GOSSIP_MENU(10940, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Who speaks of me? What are you talking about, broken?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                                                            //correct id not known
            pPlayer->SEND_GOSSIP_MENU(10940, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Continue please.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                                                            //correct id not known
            pPlayer->SEND_GOSSIP_MENU(10940, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Who are these bidders?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
                                                            //correct id not known
            pPlayer->SEND_GOSSIP_MENU(10940, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Well... yes.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
                                                            //correct id not known
            pPlayer->SEND_GOSSIP_MENU(10940, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
                                                            //correct id not known
            pPlayer->SEND_GOSSIP_MENU(10940, pCreature->GetGUID());
            pCreature->CastSpell(pPlayer,41121,false);
            pPlayer->AreaExploredOrEventHappens(QUEST_11082);
            break;
    }
    return true;
}

/*######
## npc_neltharaku
######*/

bool GossipHello_npc_neltharaku(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(10814) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I am listening, dragon", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(10613, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_neltharaku(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "But you are dragons! How could orcs do this to you?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(10614, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Your mate?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(10615, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I have battled many beasts, dragon. I will help you.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->SEND_GOSSIP_MENU(10616, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(10814);
            break;
    }
    return true;
}

/*######
## npc_oronok
######*/

#define GOSSIP_ORONOK1 "I am ready to hear your story, Oronok."
#define GOSSIP_ORONOK2 "How do I find the cipher?"
#define GOSSIP_ORONOK3 "How do you know all of this?"
#define GOSSIP_ORONOK4 "Yet what? What is it, Oronok?"
#define GOSSIP_ORONOK5 "Continue, please."
#define GOSSIP_ORONOK6 "So what of the cipher now? And your boys?"
#define GOSSIP_ORONOK7 "I will find your boys and the cipher, Oronok."

bool GossipHello_npc_oronok_tornheart(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());
    if (pCreature->isVendor())
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    if (pPlayer->GetQuestStatus(10519) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ORONOK1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        pPlayer->SEND_GOSSIP_MENU(10312, pCreature->GetGUID());
    }else
    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_oronok_tornheart(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_TRADE:
            pPlayer->SEND_VENDORLIST(pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ORONOK2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(10313, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ORONOK3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(10314, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ORONOK4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(10315, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ORONOK5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->SEND_GOSSIP_MENU(10316, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ORONOK6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(10317, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ORONOK7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            pPlayer->SEND_GOSSIP_MENU(10318, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(10519);
            break;
    }
    return true;
}

/*####
# npc_karynaku
####*/

enum
{
    QUEST_ALLY_OF_NETHER    = 10870,
    TAXI_PATH_ID            = 649
};

bool QuestAccept_npc_karynaku(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_ALLY_OF_NETHER)
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID);

    return true;
}

void AddSC_shadowmoon_valley()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_mature_netherwing_drake";
    newscript->GetAI = &GetAI_mob_mature_netherwing_drake;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_enslaved_netherwing_drake";
    newscript->GetAI = &GetAI_mob_enslaved_netherwing_drake;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_drake_dealer_hurlunk";
    newscript->pGossipHello =  &GossipHello_npc_drake_dealer_hurlunk;
    newscript->pGossipSelect = &GossipSelect_npc_drake_dealer_hurlunk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npcs_flanis_swiftwing_and_kagrosh";
    newscript->pGossipHello =  &GossipHello_npcs_flanis_swiftwing_and_kagrosh;
    newscript->pGossipSelect = &GossipSelect_npcs_flanis_swiftwing_and_kagrosh;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_murkblood_overseer";
    newscript->pGossipHello =  &GossipHello_npc_murkblood_overseer;
    newscript->pGossipSelect = &GossipSelect_npc_murkblood_overseer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_neltharaku";
    newscript->pGossipHello =  &GossipHello_npc_neltharaku;
    newscript->pGossipSelect = &GossipSelect_npc_neltharaku;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_karynaku";
    newscript->pQuestAccept = &QuestAccept_npc_karynaku;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_oronok_tornheart";
    newscript->pGossipHello =  &GossipHello_npc_oronok_tornheart;
    newscript->pGossipSelect = &GossipSelect_npc_oronok_tornheart;
    newscript->RegisterSelf();
}
