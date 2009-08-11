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
npc_wilda
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
                    DoCast(m_creature, SPELL_JUST_EATEN);
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

enum
{
    FACTION_DEFAULT                 = 62,
    FACTION_FRIENDLY                = 1840,                 // Not sure if this is correct, it was taken off of Mordenai.

    SPELL_HIT_FORCE_OF_NELTHARAKU   = 38762,
    SPELL_FORCE_OF_NELTHARAKU       = 38775,

    QUEST_FORCE_OF_NELT             = 10854,
    NPC_DRAGONMAW_SUBJUGATOR        = 21718,
    NPC_ESCAPE_DUMMY                = 22317,
    NPC_ENSLAVED_DRAKE_KILL_CREDIT  = 22316
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

        FlyTimer = 10000;
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

            if (Creature* Dragonmaw = GetClosestCreatureWithEntry(m_creature, NPC_DRAGONMAW_SUBJUGATOR, 50.0f))
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

            m_creature->ForcedDespawn();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
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
                            pPlayer->KilledMonsterCredit(NPC_ENSLAVED_DRAKE_KILL_CREDIT, m_creature->GetGUID());

                            /*
                            float x,y,z;
                            m_creature->GetPosition(x,y,z);

                            float dx,dy,dz;
                            m_creature->GetRandomPoint(x, y, z, 20, dx, dy, dz);
                            dz += 20; // so it's in the air, not ground*/

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

            m_creature->RemoveMonsterMoveFlag(MONSTER_MOVE_WALK);
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

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

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

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npcs_flanis_swiftwing_and_kagrosh(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        ItemPosCountVec dest;
        uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 30658, 1, false);
        if (msg == EQUIP_ERR_OK)
        {
            pPlayer->StoreNewItem(dest, 30658, 1, true);
            pPlayer->PlayerTalkClass->ClearMenus();
        }
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
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
    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

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
        switch(rand()%3)
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
        if (rand()%5)
            return;

        //only aggro text if not player
        if (pWho->GetTypeId() != TYPEID_PLAYER)
        {
            //appears to be random
            switch(rand()%4)
            {
                case 0: DoScriptText(SAY_WIL_AGGRO1, m_creature, pWho); break;
                case 1: DoScriptText(SAY_WIL_AGGRO2, m_creature, pWho); break;
            }
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //TODO: add more abilities
        if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 30)
        {
            if (m_uiHealingTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_HEALING_WAVE);
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
}
