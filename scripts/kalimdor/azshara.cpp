/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Azshara
SD%Complete: 90
SDComment: Quest support: 2744, 3141, 9364, 10994
SDCategory: Azshara
EndScriptData */

/* ContentData
npc_rizzle_sprysprocket
npc_depth_charge
go_southfury_moonstone
mobs_spitelashes
npc_loramus_thalipedes
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

/*#####
## npc_rizzle_sprysprocket
#####*/

enum
{
    SAY_START                   = -1000351,
    EMOTE_START                 = -1000352,
    SAY_WHISPER_CHILL           = -1000353,
    SAY_GRENADE_FAIL            = -1000354,
    SAY_END                     = -1000355,

    QUEST_MOONSTONE             = 10994,
    NPC_RIZZLE                  = 23002,
    NPC_DEPTH_CHARGE            = 23025,

    SPELL_SUMMON_RIZZLE         = 39866,
    SPELL_BLACKJACK             = 39865,                    //stuns player
    SPELL_ESCAPE                = 39871,                    //teleports to water
    SPELL_SWIM_SPEED            = 40596,

    SPELL_FROST_TRAP            = 39902,                    //not used?

    SPELL_PERIODIC_GRENADE      = 40553,                    //cannot tell who are supposed to have this aura
    SPELL_FROST_GRENADE         = 40525,                    //triggered by periodic grenade

    SPELL_SUMMON_DEPTH_CHARGE   = 39907,                    //summons the bomb creature
    SPELL_TRAP                  = 39899,                    //knockback

    SPELL_PERIODIC_CHECK        = 39888,
    SPELL_SURRENDER             = 39889,                    //should be triggered by periodic check, if player comes in certain distance with quest incomplete

    SPELL_GIVE_MOONSTONE        = 39886
};

#define GOSSIP_ITEM_MOONSTONE   "Hand over the Southfury moonstone and I'll let you go."

struct MANGOS_DLL_DECL npc_rizzle_sprysprocketAI : public npc_escortAI
{
    npc_rizzle_sprysprocketAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        pCreature->SetActiveObjectState(true);
        m_bIsIntro = true;
        m_uiIntroPhase = 0;
        m_uiIntroTimer = 0;
        m_uiDepthChargeTimer = 10000;
        Reset();
    }

    bool m_bIsIntro;
    uint8 m_uiIntroPhase;
    uint32 m_uiIntroTimer;
    uint32 m_uiDepthChargeTimer;

    void MoveInLineOfSight(Unit* pUnit)
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING) && pUnit->GetTypeId() == TYPEID_PLAYER)
        {
            if (!HasEscortState(STATE_ESCORT_PAUSED) && m_creature->IsWithinDistInMap(pUnit, INTERACTION_DISTANCE) && m_creature->IsWithinLOSInMap(pUnit))
            {
                if (((Player*)pUnit)->GetQuestStatus(QUEST_MOONSTONE) == QUEST_STATUS_INCOMPLETE)
                    m_creature->CastSpell(m_creature, SPELL_SURRENDER, true);
            }
        }

        npc_escortAI::MoveInLineOfSight(pUnit);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 0:
                m_creature->CastSpell(m_creature,SPELL_PERIODIC_CHECK,true);
                break;
        }
    }

    void Reset() { }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_SURRENDER)
        {
            SetEscortPaused(true);
            DoScriptText(SAY_END, m_creature);
            m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
    }

    //this may be wrong (and doesn't work)
    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pTarget->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_FROST_GRENADE)
            DoScriptText(SAY_WHISPER_CHILL, m_creature, pTarget);
    }

    //this may be wrong
    void JustSummoned(Creature* pSummoned)
    {
        //pSummoned->CastSpell(pSummoned,SPELL_PERIODIC_GRENADE,false,0,0,m_creature->GetGUID());
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (m_bIsIntro)
        {
            if (m_uiIntroTimer < uiDiff)
                m_uiIntroTimer = 1500;
            else
            {
                m_uiIntroTimer -= uiDiff;
                return;
            }

            switch(m_uiIntroPhase)
            {
                case 0:
                    DoScriptText(SAY_START, m_creature);
                    DoScriptText(EMOTE_START, m_creature);
                    break;
                case 1:
                    //teleports to water _before_ we Start()
                    m_creature->CastSpell(m_creature, SPELL_ESCAPE, false);
                    break;
                case 2:
                    m_creature->CastSpell(m_creature, SPELL_SWIM_SPEED, false);
                    m_bIsIntro = false;
                    Start(true);
                    break;
            }

            ++m_uiIntroPhase;
            return;
        }

        if (m_uiDepthChargeTimer < uiDiff)
        {
            if (!HasEscortState(STATE_ESCORT_PAUSED))
                m_creature->CastSpell(m_creature, SPELL_SUMMON_DEPTH_CHARGE, false);

            m_uiDepthChargeTimer = urand(10000, 15000);
        }
        else
            m_uiDepthChargeTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_rizzle_sprysprocket(Creature* pCreature)
{
    return new npc_rizzle_sprysprocketAI(pCreature);
}

bool GossipHello_npc_rizzle_sprysprocket(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_MOONSTONE) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_MOONSTONE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_rizzle_sprysprocket(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_GIVE_MOONSTONE, false);
    }

    return true;
}

struct MANGOS_DLL_DECL npc_depth_chargeAI : public ScriptedAI
{
    npc_depth_chargeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void MoveInLineOfSight(Unit* pUnit)
    {
        if (pUnit->GetTypeId() != TYPEID_PLAYER)
            return;

        if (m_creature->IsWithinDistInMap(pUnit, INTERACTION_DISTANCE) && m_creature->IsWithinLOSInMap(pUnit))
            m_creature->CastSpell(pUnit, SPELL_TRAP, false);
    }

    void Reset() { }
};

CreatureAI* GetAI_npc_depth_charge(Creature* pCreature)
{
    return new npc_depth_chargeAI(pCreature);
}

/*######
## go_southfury_moonstone
######*/

bool GOUse_go_southfury_moonstone(Player* pPlayer, GameObject* pGo)
{
    //implicitTarget=48 not implemented as of writing this code, and manual summon may be just ok for our purpose
    //pPlayer->CastSpell(pPlayer,SPELL_SUMMON_RIZZLE,false);

    if (Creature* pCreature = pPlayer->SummonCreature(NPC_RIZZLE, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0))
        pCreature->CastSpell(pPlayer, SPELL_BLACKJACK, false);

    return false;
}

/*######
## mobs_spitelashes
######*/

struct MANGOS_DLL_DECL mobs_spitelashesAI : public ScriptedAI
{
    mobs_spitelashesAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 morphtimer;
    bool spellhit;

    void Reset()
    {
        morphtimer = 0;
        spellhit = false;
    }

    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
    {
        if (!spellhit &&
            Hitter->GetTypeId() == TYPEID_PLAYER &&
            ((Player*)Hitter)->GetQuestStatus(9364) == QUEST_STATUS_INCOMPLETE &&
            (Spellkind->Id==118 || Spellkind->Id== 12824 || Spellkind->Id== 12825 || Spellkind->Id== 12826))
        {
            spellhit=true;
            DoCastSpellIfCan(m_creature,29124);                       //become a sheep
        }
    }

    void UpdateAI(const uint32 diff)
    {
        // we mustn't remove the creature in the same round in which we cast the summon spell, otherwise there will be no summons
        if (spellhit && morphtimer>=5000)
        {
            m_creature->ForcedDespawn();
            return;
        }

        // walk 5 seconds before summoning
        if (spellhit && morphtimer<5000)
        {
            morphtimer+=diff;
            if (morphtimer>=5000)
            {
                DoCastSpellIfCan(m_creature,28406);                   //summon copies
                DoCastSpellIfCan(m_creature,6924);                    //visual explosion
            }
        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //TODO: add abilities for the different creatures
        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mobs_spitelashes(Creature* pCreature)
{
    return new mobs_spitelashesAI(pCreature);
}

/*######
## npc_loramus_thalipedes
######*/

bool GossipHello_npc_loramus_thalipedes(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(2744) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Can you help me?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    if (pPlayer->GetQuestStatus(3141) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tell me your story", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_loramus_thalipedes(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(2744);
            break;

        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Please continue", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
            pPlayer->SEND_GOSSIP_MENU(1813, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+21:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I do not understand", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
            pPlayer->SEND_GOSSIP_MENU(1814, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+22:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Indeed", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23);
            pPlayer->SEND_GOSSIP_MENU(1815, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+23:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I will do this with or your help, Loramus", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24);
            pPlayer->SEND_GOSSIP_MENU(1816, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+24:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Yes", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25);
            pPlayer->SEND_GOSSIP_MENU(1817, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+25:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(3141);
            break;
    }
    return true;
}

void AddSC_azshara()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_rizzle_sprysprocket";
    newscript->GetAI = &GetAI_npc_rizzle_sprysprocket;
    newscript->pGossipHello = &GossipHello_npc_rizzle_sprysprocket;
    newscript->pGossipSelect = &GossipSelect_npc_rizzle_sprysprocket;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_depth_charge";
    newscript->GetAI = &GetAI_npc_depth_charge;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_southfury_moonstone";
    newscript->pGOUse = &GOUse_go_southfury_moonstone;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mobs_spitelashes";
    newscript->GetAI = &GetAI_mobs_spitelashes;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_loramus_thalipedes";
    newscript->pGossipHello =  &GossipHello_npc_loramus_thalipedes;
    newscript->pGossipSelect = &GossipSelect_npc_loramus_thalipedes;
    newscript->RegisterSelf();
}
