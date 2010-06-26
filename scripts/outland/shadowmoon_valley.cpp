/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDComment: Quest support: 10519, 10583, 10601, 10781, 10814, 10804, 10854, 11082. Vendor Drake Dealer Hurlunk.
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
npc_wilda
mob_torloth
npc_lord_illidan_stormrage
go_crystal_prison
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

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
                    DoCastSpellIfCan(m_creature, SPELL_JUST_EATEN);
                    DoScriptText(SAY_JUST_EATEN, m_creature);

                    if (Player* pPlr = (Player*)Unit::GetUnit((*m_creature), uiPlayerGUID))
                        pPlr->KilledMonsterCredit(NPC_EVENT_PINGER, m_creature->GetGUID());

                    Reset();
                    m_creature->GetMotionMaster()->Clear();
                }
            }
            else
                EatTimer -= diff;

            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (CastTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_NETHER_BREATH);
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

enum
{
    FACTION_DEFAULT                 = 62,
    FACTION_FRIENDLY                = 1840,                 // Not sure if this is correct, it was taken off of Mordenai.

    SPELL_HIT_FORCE_OF_NELTHARAKU   = 38762,
    SPELL_FORCE_OF_NELTHARAKU       = 38775,

    QUEST_FORCE_OF_NELT             = 10854,
    NPC_DRAGONMAW_SUBJUGATOR        = 21718,
    NPC_ESCAPE_DUMMY                = 21348
};

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

        FlyTimer = 2500;
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_HIT_FORCE_OF_NELTHARAKU && !Tapped)
        {
            if (Player* pPlayer = pCaster->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                Tapped = true;
                PlayerGUID = pPlayer->GetGUID();

                m_creature->setFaction(FACTION_FRIENDLY);

                if (Creature* pDragonmaw = GetClosestCreatureWithEntry(m_creature, NPC_DRAGONMAW_SUBJUGATOR, 50.0f))
                    AttackStart(pDragonmaw);
            }
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == 1)
            m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (Tapped)
            {
                if (FlyTimer <= diff)
                {
                    Tapped = false;

                    if (Player* pPlayer = (Player*)Unit::GetUnit(*m_creature, PlayerGUID))
                    {
                        if (pPlayer->GetQuestStatus(QUEST_FORCE_OF_NELT) == QUEST_STATUS_INCOMPLETE)
                        {
                            DoCastSpellIfCan(pPlayer, SPELL_FORCE_OF_NELTHARAKU, CAST_TRIGGERED);
                            PlayerGUID = 0;

                            float dx, dy, dz;

                            if (Creature* EscapeDummy = GetClosestCreatureWithEntry(m_creature, NPC_ESCAPE_DUMMY, 30.0f))
                                EscapeDummy->GetPosition(dx, dy, dz);
                            else
                            {
                                m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 20, dx, dy, dz);
                                dz += 25;
                            }

                            m_creature->GetMotionMaster()->MovePoint(1, dx, dy, dz);
                        }
                    }
                }
                else
                    FlyTimer -= diff;
            }
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

enum
{
    SPELL_SERVING_MUTTON            = 40468,
    NPC_DRAGONMAW_KILL_CREDIT       = 23209,
    QUEST_SLOW_DEATH                = 11020,
    POINT_DEST                      = 1
};

struct MANGOS_DLL_DECL mob_dragonmaw_peonAI : public ScriptedAI
{
    mob_dragonmaw_peonAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint64 m_uiPlayerGUID;
    bool m_bIsTapped;
    uint32 m_uiPoisonTimer;

    void Reset()
    {
        m_uiPlayerGUID = 0;
        m_bIsTapped = false;
        m_uiPoisonTimer = 0;
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (!pCaster)
            return;

        if (pCaster->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_SERVING_MUTTON && !m_bIsTapped)
        {
            m_uiPlayerGUID = pCaster->GetGUID();

            m_bIsTapped = true;

            float fX, fY, fZ;
            pCaster->GetClosePoint(fX, fY, fZ, m_creature->GetObjectSize());

            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->GetMotionMaster()->MovePoint(POINT_DEST, fX, fY, fZ);
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == POINT_DEST)
        {
            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_EAT);
            m_uiPoisonTimer = 15000;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiPoisonTimer)
        {
            if (m_uiPoisonTimer <= uiDiff)
            {
                if (Player* pPlayer = (Player*)Unit::GetUnit(*m_creature, m_uiPlayerGUID))
                {
                    if (pPlayer->GetQuestStatus(QUEST_SLOW_DEATH) == QUEST_STATUS_INCOMPLETE)
                        pPlayer->KilledMonsterCredit(NPC_DRAGONMAW_KILL_CREDIT, m_creature->GetGUID());
                }

                m_uiPoisonTimer = 0;
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            else
                m_uiPoisonTimer -= uiDiff;
        }
    }
};

/*######
## npc_drake_dealer_hurlunk
######*/

bool GossipHello_npc_drake_dealer_hurlunk(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isVendor() && pPlayer->GetReputationRank(1015) == REP_EXALTED)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_drake_dealer_hurlunk(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_TRADE)
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

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npcs_flanis_swiftwing_and_kagrosh(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if (Item* pItem = pPlayer->StoreNewItemInInventorySlot(30658, 1))
            pPlayer->SendNewItem(pItem, 1, true, false);

        pPlayer->CLOSE_GOSSIP_MENU();
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if (Item* pItem = pPlayer->StoreNewItemInInventorySlot(30659, 1))
            pPlayer->SendNewItem(pItem, 1, true, false);

        pPlayer->CLOSE_GOSSIP_MENU();
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

bool GossipSelect_npc_murkblood_overseer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
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

bool GossipSelect_npc_neltharaku(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
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
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_oronok_tornheart(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
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

/*######
# npc_wilda
######*/

enum
{
    SAY_WIL_START               = -1000381,
    SAY_WIL_AGGRO1              = -1000382,
    SAY_WIL_AGGRO2              = -1000383,
    SAY_WIL_PROGRESS1           = -1000384,
    SAY_WIL_PROGRESS2           = -1000385,
    SAY_WIL_FIND_EXIT           = -1000386,
    SAY_WIL_PROGRESS4           = -1000387,
    SAY_WIL_PROGRESS5           = -1000388,
    SAY_WIL_JUST_AHEAD          = -1000389,
    SAY_WIL_END                 = -1000390,

    SPELL_CHAIN_LIGHTNING       = 16006,
    SPELL_EARTHBING_TOTEM       = 15786,
    SPELL_FROST_SHOCK           = 12548,
    SPELL_HEALING_WAVE          = 12491,

    QUEST_ESCAPE_COILSCAR       = 10451,
    NPC_COILSKAR_ASSASSIN       = 21044,
    FACTION_EARTHEN             = 1726                      //guessed
};

//this script needs verification
struct MANGOS_DLL_DECL npc_wildaAI : public npc_escortAI
{
    npc_wildaAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    uint32 m_uiHealingTimer;

    void Reset()
    {
        m_uiHealingTimer = 0;
    }

    void WaypointReached(uint32 uiPointId)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        switch(uiPointId)
        {
            case 13:
                DoScriptText(SAY_WIL_PROGRESS1, m_creature, pPlayer);
                DoSpawnAssassin();
                break;
            case 14:
                DoSpawnAssassin();
                break;
            case 15:
                DoScriptText(SAY_WIL_FIND_EXIT, m_creature, pPlayer);
                break;
            case 19:
                DoRandomSay();
                break;
            case 20:
                DoSpawnAssassin();
                break;
            case 26:
                DoRandomSay();
                break;
            case 27:
                DoSpawnAssassin();
                break;
            case 33:
                DoRandomSay();
                break;
            case 34:
                DoSpawnAssassin();
                break;
            case 37:
                DoRandomSay();
                break;
            case 38:
                DoSpawnAssassin();
                break;
            case 39:
                DoScriptText(SAY_WIL_JUST_AHEAD, m_creature, pPlayer);
                break;
            case 43:
                DoRandomSay();
                break;
            case 44:
                DoSpawnAssassin();
                break;
            case 50:
                DoScriptText(SAY_WIL_END, m_creature, pPlayer);
                pPlayer->GroupEventHappens(QUEST_ESCAPE_COILSCAR, m_creature);
                break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_COILSKAR_ASSASSIN)
            pSummoned->AI()->AttackStart(m_creature);
    }

    //this is very unclear, random say without no real relevance to script/event
    void DoRandomSay()
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_WIL_PROGRESS2, m_creature); break;
            case 1: DoScriptText(SAY_WIL_PROGRESS4, m_creature); break;
            case 2: DoScriptText(SAY_WIL_PROGRESS5, m_creature); break;
        }
    }

    void DoSpawnAssassin()
    {
        //unknown where they actually appear
        float fX, fY, fZ;
        m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 15.0f, fX, fY, fZ);

        m_creature->SummonCreature(NPC_COILSKAR_ASSASSIN, fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
    }

    void Aggro(Unit* pWho)
    {
        //don't always use
        if (urand(0, 4))
            return;

        //only aggro text if not player
        if (pWho->GetTypeId() != TYPEID_PLAYER)
        {
            //appears to be random
            switch(urand(0, 3))
            {
                case 0: DoScriptText(SAY_WIL_AGGRO1, m_creature, pWho); break;
                case 1: DoScriptText(SAY_WIL_AGGRO2, m_creature, pWho); break;
            }
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //TODO: add more abilities
        if (m_creature->GetHealthPercent() <= 30.0f)
        {
            if (m_uiHealingTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_HEALING_WAVE);
                m_uiHealingTimer = 15000;
            }
            else
                m_uiHealingTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_wilda(Creature* pCreature)
{
    return new npc_wildaAI(pCreature);
}

bool QuestAccept_npc_wilda(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_ESCAPE_COILSCAR)
    {
        DoScriptText(SAY_WIL_START, pCreature, pPlayer);
        pCreature->setFaction(FACTION_EARTHEN);

        if (npc_wildaAI* pEscortAI = dynamic_cast<npc_wildaAI*>(pCreature->AI()))
            pEscortAI->Start(false, false, pPlayer->GetGUID(), pQuest);
    }
    return true;
}

/*#####
# Quest: Battle of the Crimson Watch
#####*/

enum
{
    QUEST_BATTLE_OF_THE_CRIMSON_WATCH = 10781,

    EVENT_COOLDOWN                    = 30000,

    SAY_TORLOTH_DIALOGUE1             = -1000532,
    SAY_TORLOTH_DIALOGUE2             = -1000533, 
    SAY_TORLOTH_DIALOGUE3             = -1000534,
    SAY_ILLIDAN_DIALOGUE              = -1000535,
    SAY_ILLIDAN_SUMMON1               = -1000536,
    SAY_ILLIDAN_SUMMON2               = -1000537,
    SAY_ILLIDAN_SUMMON3               = -1000538,
    SAY_ILLIDAN_SUMMON4               = -1000539,
    SAY_EVENT_COMPLETED               = -1000540,

    MODEL_ID_FELGUARD                 = 18654,
    MODEL_ID_DREADLORD                = 19991,

    NPC_ILLIDARI_SOLDIER              = 22075,
    NPC_ILLIDARI_MIND_BREAKER         = 22074,
    NPC_ILLIDARI_HIGHLORD             = 19797,
    NPC_TORLOTH_THE_MAGNIFICENT       = 22076,
    NPC_LORD_ILLIDAN                  = 22083
};

enum CinematicCreature
{
    LORD_ILLIDAN = 1,
    TORLOTH      = 0
};

const float EVENT_AREA_RADIUS = 65.0;

struct TorlothCinematic
{
    int32  iTextId;
    uint32 uiCreature;
    uint32 uiTimer;
};

static TorlothCinematic TorlothAnim[]=
{
    {SAY_TORLOTH_DIALOGUE1, TORLOTH, 2000},
    {SAY_ILLIDAN_DIALOGUE, LORD_ILLIDAN, 7000},
    {SAY_TORLOTH_DIALOGUE2, TORLOTH, 3000},
    {0, TORLOTH, 2000},                                  // Torloth stand
    {SAY_TORLOTH_DIALOGUE3, TORLOTH, 1000}, 
    {0, TORLOTH, 3000},
    {0, TORLOTH, 0}
};

struct Location
{
    float fLocX;
    float fLocY;
    float fLocZ;
    float fOrient;
};

static Location SpawnLocation[]=
{
    {-4615.8556f, 1342.2532f, 139.9f, 1.612f},              // Illidari Soldier
    {-4598.9365f, 1377.3182f, 139.9f, 3.917f},              // Illidari Soldier
    {-4598.4697f, 1360.8999f, 139.9f, 2.427f},              // Illidari Soldier
    {-4589.3599f, 1369.1061f, 139.9f, 3.165f},              // Illidari Soldier
    {-4608.3477f, 1386.0076f, 139.9f, 4.108f},              // Illidari Soldier
    {-4633.1889f, 1359.8033f, 139.9f, 0.949f},              // Illidari Soldier
    {-4623.5791f, 1351.4574f, 139.9f, 0.971f},              // Illidari Soldier
    {-4607.2988f, 1351.6099f, 139.9f, 2.416f},              // Illidari Soldier
    {-4633.7764f, 1376.0417f, 139.9f, 5.608f},              // Illidari Soldier
    {-4600.2461f, 1369.1240f, 139.9f, 3.056f},              // Illidari Mind Breaker
    {-4631.7808f, 1367.9459f, 139.9f, 0.020f},              // Illidari Mind Breaker
    {-4600.2461f, 1369.1240f, 139.9f, 3.056f},              // Illidari Highlord
    {-4631.7808f, 1367.9459f, 139.9f, 0.020f},              // Illidari Highlord
    {-4615.5586f, 1353.0031f, 139.9f, 1.540f},              // Illidari Highlord
    {-4616.4736f, 1384.2170f, 139.9f, 4.971f},              // Illidari Highlord
    {-4627.1240f, 1378.8752f, 139.9f, 2.544f}               // Torloth The Magnificent
};

struct WaveData
{
    uint8  uiSpawnCount;
    uint8  uiUsedSpawnPoint;
    uint32 uiCreatureId; 
    uint32 uiSpawnTimer;
    uint32 uiYellTimer;
    int32  iTextId;
};

static WaveData WavesInfo[]=
{
    // Illidari Soldier
    {9, 0, NPC_ILLIDARI_SOLDIER, 10000, 7000, SAY_ILLIDAN_SUMMON1},
    // Illidari Mind Breaker
    {2, 9, NPC_ILLIDARI_MIND_BREAKER, 10000, 7000, SAY_ILLIDAN_SUMMON2},
    // Illidari Highlord
    {4, 11, NPC_ILLIDARI_HIGHLORD, 10000, 7000, SAY_ILLIDAN_SUMMON3},
    // Torloth The Magnificent
    {1, 15, NPC_TORLOTH_THE_MAGNIFICENT, 10000, 7000, SAY_ILLIDAN_SUMMON4}
};

/*######
# mob_torloth
#####*/

enum
{
    SPELL_CLEAVE           = 15284,
    SPELL_SHADOWFURY       = 39082,
    SPELL_SPELL_REFLECTION = 33961
};

struct MANGOS_DLL_DECL mob_torlothAI : public ScriptedAI
{
    mob_torlothAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 m_uiLordIllidanGUID;
    uint64 m_uiPlayerGUID;

    uint32 m_uiCleaveTimer;
    uint32 m_uiShadowfuryTimer;
    uint32 m_uiSpellReflectionTimer;
    uint8  m_uiAnimationCount;
    uint32 m_uiAnimationTimer;

    void Reset()
    {
        m_uiLordIllidanGUID = 0;
        m_uiPlayerGUID = 0;

        m_uiAnimationCount = 0;
        m_uiAnimationTimer = 4000;
        m_uiCleaveTimer = 10000;
        m_uiShadowfuryTimer = 18000;
        m_uiSpellReflectionTimer = 25000;

        // make him not attackable for the time of animation
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
    }

    void EnterEvadeMode()
    {
        m_creature->ForcedDespawn();
    }

    void HandleAnimation()
    {
        Creature* pCreature = m_creature;

        if (TorlothAnim[m_uiAnimationCount].uiCreature == LORD_ILLIDAN)
        {
            pCreature = ((Creature*)Unit::GetUnit(*m_creature, m_uiLordIllidanGUID));

            if (!pCreature)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }

        if (TorlothAnim[m_uiAnimationCount].iTextId)
            DoScriptText(TorlothAnim[m_uiAnimationCount].iTextId, pCreature);

        m_uiAnimationTimer = TorlothAnim[m_uiAnimationCount].uiTimer;

        switch(m_uiAnimationCount)
        {
            case 0:
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                break; 
            case 3:
                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                break;
            case 5:
                if (Unit* pTarget = Unit::GetUnit((*m_creature), m_uiPlayerGUID))
                {
                    m_creature->AddThreat(pTarget);
                    m_creature->SetFacingToObject(pTarget);
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                }
                 break;
            case 6:
            {
                if (Unit* pTarget = Unit::GetUnit((*m_creature), m_uiPlayerGUID))
                {
                    SetCombatMovement(true);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    float fLocX, fLocY, fLocZ;
                    pTarget->GetPosition(fLocX, fLocY, fLocZ);
                    m_creature->GetMotionMaster()->MovePoint(0, fLocX, fLocY, fLocZ);
                }
                break;
            }
        }

        ++m_uiAnimationCount;
    }

    void JustDied(Unit* pKiller)
    {
        if (Player* pPlayer = pKiller->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            pPlayer->GroupEventHappens(QUEST_BATTLE_OF_THE_CRIMSON_WATCH, m_creature);
        
            if (Creature* pLordIllidan = ((Creature*)Unit::GetUnit(*m_creature, m_uiLordIllidanGUID)))
            {
                DoScriptText(SAY_EVENT_COMPLETED, pLordIllidan, pPlayer);
                pLordIllidan->AI()->EnterEvadeMode();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAnimationCount < 7)
        {
            if (m_uiAnimationTimer < uiDiff)
                HandleAnimation();
            else
                m_uiAnimationTimer -= uiDiff;
        }
        else
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (m_uiCleaveTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = 15000;
            }
            else
                m_uiCleaveTimer -= uiDiff;

            if (m_uiShadowfuryTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWFURY);
                m_uiShadowfuryTimer = 20000;
            }
            else
                m_uiShadowfuryTimer -= uiDiff;

            if (m_uiSpellReflectionTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_SPELL_REFLECTION);
                m_uiSpellReflectionTimer = 30000;
            }
            else
                m_uiSpellReflectionTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_mob_torloth(Creature* pCreature)
{
    return new mob_torlothAI(pCreature);
}

/*#####
# npc_lord_illidan_stormrage
#####*/

struct MANGOS_DLL_DECL npc_lord_illidan_stormrageAI : public Scripted_NoMovementAI
{
    npc_lord_illidan_stormrageAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) {Reset();}

    uint64 m_uiPlayerGUID;
    uint32 m_uiWaveTimer;
    uint32 m_uiAnnounceTimer;
    uint32 m_uiCheckTimer;
    uint8  m_uiMobCount;
    uint8  m_uiWaveCount;

    bool m_bEventStarted;
    bool m_bEventFailed;
    bool m_bWaveAnnounced;

    void Reset()
    {
        m_uiPlayerGUID = 0;

        m_uiWaveTimer = 10000;
        m_uiAnnounceTimer = 7000;
        m_uiCheckTimer = 2000;

        m_uiMobCount = 0;
        m_uiWaveCount = 0;

        m_bEventStarted = false;
        m_bEventFailed = false;
        m_bWaveAnnounced = false;
    }

    void StartEvent(Player* pPlayer)
    {
        m_bEventStarted = true;
        m_uiPlayerGUID = pPlayer->GetGUID();
    }

    void SummonWave()
    {
        uint8 uiCount = WavesInfo[m_uiWaveCount].uiSpawnCount;
        uint8 uiLocIndex = WavesInfo[m_uiWaveCount].uiUsedSpawnPoint;
        uint8 uiFelguardCount = 0;
        uint8 uiDreadlordCount = 0;

        for(uint8 i = 0; i < uiCount; ++i)
        {
            float fLocX, fLocY, fLocZ, fOrient;
            fLocX = SpawnLocation[uiLocIndex + i].fLocX;
            fLocY = SpawnLocation[uiLocIndex + i].fLocY;
            fLocZ = SpawnLocation[uiLocIndex + i].fLocZ;
            fOrient = SpawnLocation[uiLocIndex + i].fOrient;

            if (Creature* pSpawn = m_creature->SummonCreature(WavesInfo[m_uiWaveCount].uiCreatureId, fLocX, fLocY, fLocZ, fOrient, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000))
            {

                if (m_uiWaveCount)                          // only in first wave
                    continue;

                if (!urand(0,2) && uiFelguardCount < 2)
                {
                    pSpawn->SetDisplayId(MODEL_ID_FELGUARD);
                    ++uiFelguardCount;
                }
                else if (uiDreadlordCount < 3)
                {
                    pSpawn->SetDisplayId(MODEL_ID_DREADLORD);
                    ++uiDreadlordCount;
                }
                else if (uiFelguardCount < 2)
                {
                    pSpawn->SetDisplayId(MODEL_ID_FELGUARD);
                    ++uiFelguardCount;
                }
            }
        }

        ++m_uiWaveCount;
        m_uiWaveTimer = WavesInfo[m_uiWaveCount].uiSpawnTimer;
        m_uiAnnounceTimer = WavesInfo[m_uiWaveCount].uiYellTimer;
    }

    void JustSummoned(Creature* pSummoned)
    {
        // increment mob count
        ++m_uiMobCount;

        if (!m_uiPlayerGUID)
            return;

        if (pSummoned->GetEntry() == NPC_TORLOTH_THE_MAGNIFICENT)
        {
            if (mob_torlothAI* pTorlothAI = dynamic_cast<mob_torlothAI*>(pSummoned->AI()))
            {
                pTorlothAI->m_uiLordIllidanGUID = m_creature->GetGUID();
                pTorlothAI->m_uiPlayerGUID = m_uiPlayerGUID;
            }
        }
        else
        {
            if (Unit* pTarget = Unit::GetUnit((*m_creature), m_uiPlayerGUID))
            {
                float fLocX, fLocY, fLocZ;
                pTarget->GetPosition(fLocX, fLocY, fLocZ);
                pSummoned->GetMotionMaster()->MovePoint(0, fLocX, fLocY, fLocZ);
            }
        }
    }

    void SummonedCreatureDespawn(Creature* pCreature) 
    {
        // decrement mob count
        --m_uiMobCount;

        if (!m_uiMobCount)
            m_bWaveAnnounced = false;
    }

    void CheckEventFail()
    {
        Player* pPlayer = ((Player*)Unit::GetUnit((*m_creature), m_uiPlayerGUID));

        if (!pPlayer)
            return;

        if (Group* pEventGroup = pPlayer->GetGroup())
        {
            uint8 uiDeadMemberCount = 0;
            uint8 uiFailedMemberCount = 0;

            for(GroupReference* pRef = pEventGroup->GetFirstMember(); pRef != NULL; pRef = pRef->next())
            {
                if (Player* pMember = pRef->getSource())
                {
                    if (!pMember->isAlive())
                        ++uiDeadMemberCount;

                    // if we already failed no need to check other things
                    if (pMember->GetQuestStatus(QUEST_BATTLE_OF_THE_CRIMSON_WATCH) == QUEST_STATUS_FAILED)
                    {
                        ++uiFailedMemberCount;
                        continue;
                    }

                    // we left event area fail quest
                    if (!pMember->IsWithinDistInMap(m_creature, EVENT_AREA_RADIUS))
                    {
                        pMember->FailQuest(QUEST_BATTLE_OF_THE_CRIMSON_WATCH);
                        ++uiFailedMemberCount;
                    }
                }
            }

            if (pEventGroup->GetMembersCount() == uiFailedMemberCount)
            {
                m_bEventFailed = true;
                return;
            }

            if (pEventGroup->GetMembersCount() == uiDeadMemberCount)
            {
                for(GroupReference* pRef = pEventGroup->GetFirstMember(); pRef != NULL; pRef = pRef->next())
                {
                    if (Player* pMember = pRef->getSource())
                    {
                        if (pMember->GetQuestStatus(QUEST_BATTLE_OF_THE_CRIMSON_WATCH) == QUEST_STATUS_INCOMPLETE)
                            pMember->FailQuest(QUEST_BATTLE_OF_THE_CRIMSON_WATCH);
                    }
                }

                m_bEventFailed = true;
            }
        }
        else if (pPlayer->isDead() || !pPlayer->IsWithinDistInMap(m_creature, EVENT_AREA_RADIUS))
        {
            pPlayer->FailQuest(QUEST_BATTLE_OF_THE_CRIMSON_WATCH);
            m_bEventFailed = true;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_uiPlayerGUID || !m_bEventStarted)
            return;

        if (!m_uiMobCount && m_uiWaveCount < 4)
        {
            if (!m_bWaveAnnounced && m_uiAnnounceTimer < uiDiff)
            {
                DoScriptText(WavesInfo[m_uiWaveCount].iTextId, m_creature);
                m_bWaveAnnounced = true;
            }
            else
                m_uiAnnounceTimer -= uiDiff;

            if (m_uiWaveTimer < uiDiff)
                SummonWave();
            else
                m_uiWaveTimer -= uiDiff;
        }

        if (m_uiCheckTimer < uiDiff)
        {
            CheckEventFail();
            m_uiCheckTimer = 2000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        if (m_bEventFailed)
            Reset();
    }
};

CreatureAI* GetAI_npc_lord_illidan_stormrage(Creature* (pCreature))
{
    return new npc_lord_illidan_stormrageAI(pCreature);
}

/*#####
# go_crystal_prison : GameObject that begins the event and hands out quest
######*/
bool GOQuestAccept_GO_crystal_prison(Player* pPlayer, GameObject* pGo, Quest const* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_BATTLE_OF_THE_CRIMSON_WATCH )
        if (Creature* pLordIllidan = GetClosestCreatureWithEntry(pPlayer, NPC_LORD_ILLIDAN, 50.0))
            if (npc_lord_illidan_stormrageAI* pIllidanAI = dynamic_cast<npc_lord_illidan_stormrageAI*>(pLordIllidan->AI()))
                if (!pIllidanAI->m_bEventStarted)
                    pIllidanAI->StartEvent(pPlayer);

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

    newscript = new Script;
    newscript->Name = "npc_wilda";
    newscript->GetAI = &GetAI_npc_wilda;
    newscript->pQuestAccept = &QuestAccept_npc_wilda;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_lord_illidan_stormrage";
    newscript->GetAI = &GetAI_npc_lord_illidan_stormrage;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_torloth";
    newscript->GetAI = &GetAI_mob_torloth;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_crystal_prison";
    newscript->pGOQuestAccept = &GOQuestAccept_GO_crystal_prison;
    newscript->RegisterSelf();
}
