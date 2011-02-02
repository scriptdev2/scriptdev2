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
SDName: Terokkar_Forest
SD%Complete: 80
SDComment: Quest support: 9889, 10009, 10873, 10896, 10446/10447, 10852, 10887, 10922, 11096, 11093, 10051, 10052. Skettis->Ogri'la Flight
SDCategory: Terokkar Forest
EndScriptData */

/* ContentData
mob_unkor_the_ruthless
mob_infested_root_walker
mob_rotting_forest_rager
mob_netherweb_victim
npc_akuno
npc_floon
npc_hungry_nether_ray
npc_letoll
npc_mana_bomb_exp_trigger
go_mana_bomb
npc_skyguard_handler_deesak
npc_slim
go_veil_skith_cage
npc_captive_child
npc_isla_starmane
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"
#include "pet_ai.h"

/*######
## mob_unkor_the_ruthless
######*/

enum
{
    SAY_SUBMIT                  = -1000194,

    FACTION_HOSTILE             = 45,
    FACTION_FRIENDLY            = 35,
    QUEST_DONT_KILL_THE_FAT_ONE = 9889,

    SPELL_PULVERIZE             = 2676,
    // SPELL_QUID9889           = 32174,                    // TODO Make use of this quest-credit spell
};

struct MANGOS_DLL_DECL mob_unkor_the_ruthlessAI : public ScriptedAI
{
    mob_unkor_the_ruthlessAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool m_bCanDoQuest;
    uint32 m_uiUnfriendlyTimer;
    uint32 m_uiPulverizeTimer;

    void Reset()
    {
        m_bCanDoQuest = false;
        m_uiUnfriendlyTimer = 0;
        m_uiPulverizeTimer = 3000;
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->setFaction(FACTION_HOSTILE);
    }

    void DoNice()
    {
        DoScriptText(SAY_SUBMIT, m_creature);
        m_creature->setFaction(FACTION_FRIENDLY);
        m_creature->SetStandState(UNIT_STAND_STATE_SIT);
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
        m_uiUnfriendlyTimer = 60000;
    }

    void DamageTaken(Unit* pDealer, uint32 &uiDamage)
    {
        if ((m_creature->GetHealth() - uiDamage)*100 / m_creature->GetMaxHealth() >= 30)
            return;

        if (Player* pPlayer = pDealer->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            if (Group* pGroup = pPlayer->GetGroup())
            {
                for(GroupReference* itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
                {
                    Player* pGroupie = itr->getSource();
                    if (pGroupie &&
                        pGroupie->GetQuestStatus(QUEST_DONT_KILL_THE_FAT_ONE) == QUEST_STATUS_INCOMPLETE &&
                        pGroupie->GetReqKillOrCastCurrentCount(QUEST_DONT_KILL_THE_FAT_ONE, 18260) == 10)
                    {
                        pGroupie->AreaExploredOrEventHappens(QUEST_DONT_KILL_THE_FAT_ONE);
                        if (!m_bCanDoQuest)
                            m_bCanDoQuest = true;
                    }
                }
            }
            else if (pPlayer->GetQuestStatus(QUEST_DONT_KILL_THE_FAT_ONE) == QUEST_STATUS_INCOMPLETE &&
                pPlayer->GetReqKillOrCastCurrentCount(QUEST_DONT_KILL_THE_FAT_ONE, 18260) == 10)
            {
                pPlayer->AreaExploredOrEventHappens(QUEST_DONT_KILL_THE_FAT_ONE);
                m_bCanDoQuest = true;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bCanDoQuest)
        {
            if (!m_uiUnfriendlyTimer)
            {
                //DoCastSpellIfCan(m_creature,SPELL_QUID9889);        //not using spell for now
                DoNice();
            }
            else
            {
                if (m_uiUnfriendlyTimer <= uiDiff)
                    EnterEvadeMode();
                else
                    m_uiUnfriendlyTimer -= uiDiff;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPulverizeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_PULVERIZE);
            m_uiPulverizeTimer = 9000;
        }
        else
            m_uiPulverizeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_unkor_the_ruthless(Creature* pCreature)
{
    return new mob_unkor_the_ruthlessAI(pCreature);
}

/*######
## mob_infested_root_walker
######*/

enum
{
    SPELL_SUMMON_WOOD_MITES     = 39130,
};

struct MANGOS_DLL_DECL mob_infested_root_walkerAI : public ScriptedAI
{
    mob_infested_root_walkerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() { }

    void DamageTaken(Unit* pDealer, uint32 &uiDamage)
    {
        if (m_creature->GetHealth() <= uiDamage)
            if (pDealer->IsControlledByPlayer())
                if (urand(0, 3))
                    //Summon Wood Mites
                    DoCastSpellIfCan(m_creature, SPELL_SUMMON_WOOD_MITES, CAST_TRIGGERED);
    }
};

CreatureAI* GetAI_mob_infested_root_walker(Creature* pCreature)
{
    return new mob_infested_root_walkerAI(pCreature);
}

/*######
## mob_rotting_forest_rager
######*/

enum
{
    SPELL_SUMMON_LOTS_OF_WOOD_MIGHTS    = 39134,
};

struct MANGOS_DLL_DECL mob_rotting_forest_ragerAI : public ScriptedAI
{
    mob_rotting_forest_ragerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() { }

    void DamageTaken(Unit* pDealer, uint32 &uiDamage)
    {
        if (m_creature->GetHealth() <= uiDamage)
            if (pDealer->IsControlledByPlayer())
                if (urand(0, 3))
                    //Summon Lots of Wood Mights
                    DoCastSpellIfCan(m_creature, SPELL_SUMMON_LOTS_OF_WOOD_MIGHTS, CAST_TRIGGERED);
    }
};
CreatureAI* GetAI_mob_rotting_forest_rager(Creature* pCreature)
{
    return new mob_rotting_forest_ragerAI(pCreature);
}

/*######
## mob_netherweb_victim
######*/

enum
{
    NPC_FREED_WARRIOR       = 22459,
    QUEST_TAKEN_IN_NIGHT    = 10873
    //SPELL_FREE_WEBBED       = 38950
};

const uint32 netherwebVictims[6] =
{
    18470, 16805, 21242, 18452, 22482, 21285
};
struct MANGOS_DLL_DECL mob_netherweb_victimAI : public ScriptedAI
{
    mob_netherweb_victimAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();
    }

    void Reset() { }
    void MoveInLineOfSight(Unit* pWho) { }

    void JustDied(Unit* pKiller)
    {
        if (Player* pPlayer = pKiller->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            if (pPlayer->GetQuestStatus(QUEST_TAKEN_IN_NIGHT) == QUEST_STATUS_INCOMPLETE)
            {
                if (!urand(0, 3))
                {
                    m_creature->SummonCreature(NPC_FREED_WARRIOR, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                    pPlayer->KilledMonsterCredit(NPC_FREED_WARRIOR, m_creature->GetGUID());
                }
                else
                    m_creature->SummonCreature(netherwebVictims[urand(0, 5)], 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
            }
        }
    }
};

CreatureAI* GetAI_mob_netherweb_victim(Creature* pCreature)
{
    return new mob_netherweb_victimAI(pCreature);
}

/*#####
## npc_akuno
#####*/

enum
{
    SAY_AKU_START           = -1000477,
    SAY_AKU_AMBUSH_A        = -1000478,
    SAY_AKU_AMBUSH_B        = -1000479,
    SAY_AKU_AMBUSH_B_REPLY  = -1000480,
    SAY_AKU_COMPLETE        = -1000481,

    SPELL_CHAIN_LIGHTNING   = 39945,

    QUEST_ESCAPING_TOMB     = 10887,
    NPC_CABAL_SKIRMISHER    = 21661
};

static float m_afAmbushB1[]= {-2895.525879f, 5336.431641f, -11.800f};
static float m_afAmbushB2[]= {-2890.604980f, 5331.938965f, -11.282f};

struct MANGOS_DLL_DECL npc_akunoAI : public npc_escortAI
{
    npc_akunoAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    uint32 m_uiChainLightningTimer;

    void Reset()
    {
        m_uiChainLightningTimer = 1000;
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 5:
                DoScriptText(SAY_AKU_AMBUSH_A, m_creature);
                m_creature->SummonCreature(NPC_CABAL_SKIRMISHER, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                break;
            case 14:
                DoScriptText(SAY_AKU_AMBUSH_B, m_creature);

                if (Creature* pTemp = m_creature->SummonCreature(NPC_CABAL_SKIRMISHER, m_afAmbushB1[0], m_afAmbushB1[1], m_afAmbushB1[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000))
                    DoScriptText(SAY_AKU_AMBUSH_B_REPLY, pTemp);

                m_creature->SummonCreature(NPC_CABAL_SKIRMISHER, m_afAmbushB2[0], m_afAmbushB2[1], m_afAmbushB2[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                break;
            case 15:
                SetRun();
                break;
            case 18:
                DoScriptText(SAY_AKU_COMPLETE, m_creature);

                if (Player* pPlayer = GetPlayerForEscort())
                    pPlayer->GroupEventHappens(QUEST_ESCAPING_TOMB, m_creature);

                break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(m_creature);
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiChainLightningTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHAIN_LIGHTNING);
            m_uiChainLightningTimer = urand(7000, 14000);
        }
        else
            m_uiChainLightningTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

bool QuestAccept_npc_akuno(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_ESCAPING_TOMB)
    {
        if (npc_akunoAI* pEscortAI = dynamic_cast<npc_akunoAI*>(pCreature->AI()))
        {
            pCreature->SetStandState(UNIT_STAND_STATE_STAND);
            pCreature->setFaction(FACTION_ESCORT_N_NEUTRAL_ACTIVE);

            DoScriptText(SAY_AKU_START, pCreature);
            pEscortAI->Start(false, pPlayer->GetGUID(), pQuest);
        }
    }
    return true;
}

CreatureAI* GetAI_npc_akuno(Creature* pCreature)
{
    return new npc_akunoAI(pCreature);
}

/*######
## npc_floon -- TODO move to EventAI and WorldDB (gossip)
######*/

enum
{
    SAY_FLOON_ATTACK        = -1000195,

    SPELL_SILENCE           = 6726,
    SPELL_FROSTBOLT         = 9672,
    SPELL_FROST_NOVA        = 11831,

    FACTION_HOSTILE_FL      = 1738,
    QUEST_CRACK_SKULLS      = 10009
};

#define GOSSIP_FLOON1       "You owe Sim'salabim money. Hand them over or die!"
#define GOSSIP_FLOON2       "Hand over the money or die...again!"

struct MANGOS_DLL_DECL npc_floonAI : public ScriptedAI
{
    npc_floonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiNormFaction = pCreature->getFaction();
        Reset();
    }

    uint32 m_uiNormFaction;
    uint32 m_uiSilence_Timer;
    uint32 m_uiFrostbolt_Timer;
    uint32 m_uiFrostNova_Timer;

    void Reset()
    {
        m_uiSilence_Timer = 2000;
        m_uiFrostbolt_Timer = 4000;
        m_uiFrostNova_Timer = 9000;

        if (m_creature->getFaction() != m_uiNormFaction)
            m_creature->setFaction(m_uiNormFaction);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSilence_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SILENCE);
            m_uiSilence_Timer = 30000;
        }else m_uiSilence_Timer -= uiDiff;

        if (m_uiFrostNova_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,SPELL_FROST_NOVA);
            m_uiFrostNova_Timer = 20000;
        }else m_uiFrostNova_Timer -= uiDiff;

        if (m_uiFrostbolt_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROSTBOLT);
            m_uiFrostbolt_Timer = 5000;
        }else m_uiFrostbolt_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_floon(Creature* pCreature)
{
    return new npc_floonAI(pCreature);
}

bool GossipHello_npc_floon(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_CRACK_SKULLS) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_FLOON1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(9442, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_floon(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_FLOON2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(9443, pCreature->GetGUID());
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->setFaction(FACTION_HOSTILE_FL);
        DoScriptText(SAY_FLOON_ATTACK, pCreature, pPlayer);
        pCreature->AI()->AttackStart(pPlayer);
    }
    return true;
}

/*######
## npc_skyguard_handler_deesak -- TODO move to WorldDB (gossip)
######*/

#define GOSSIP_SKYGUARD "Fly me to Ogri'la please"

bool GossipHello_npc_skyguard_handler_deesak(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetReputationRank(1031) >= REP_HONORED)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SKYGUARD, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_skyguard_handler_deesak(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer,41279,true);               //TaxiPath 705 (Taxi - Skettis to Skyguard Outpost)
    }
    return true;
}

/*######
## npc_hungry_nether_ray
######*/

enum
{
    EMOTE_FEED                  = -1000628,
    NPC_BLACK_WARP_CHASER       = 23219,
    SPELL_FEED_CREDIT           = 41427,                    // credit for quest 11093
};

struct MANGOS_DLL_DECL npc_hungry_nether_rayAI : public ScriptedPetAI
{
    npc_hungry_nether_rayAI(Creature* pCreature) : ScriptedPetAI(pCreature) { Reset(); }

    void Reset() { }

    void OwnerKilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_UNIT && pVictim->GetEntry() == NPC_BLACK_WARP_CHASER)
        {
            // Distance expected?
            if (m_creature->IsWithinDistInMap(pVictim, 10.0f))
            {
                DoScriptText(EMOTE_FEED, m_creature);
                m_creature->CastSpell(m_creature, SPELL_FEED_CREDIT, true);
            }
        }
    }
};

CreatureAI* GetAI_npc_hungry_nether_ray(Creature* pCreature)
{
    return new npc_hungry_nether_rayAI(pCreature);
}

/*######
## npc_letoll
######*/

enum
{
    SAY_LE_START                    = -1000511,
    SAY_LE_KEEP_SAFE                = -1000512,
    SAY_LE_NORTH                    = -1000513,
    SAY_LE_ARRIVE                   = -1000514,
    SAY_LE_BURIED                   = -1000515,
    SAY_LE_ALMOST                   = -1000516,
    SAY_LE_DRUM                     = -1000517,
    SAY_LE_DRUM_REPLY               = -1000518,
    SAY_LE_DISCOVERY                = -1000519,
    SAY_LE_DISCOVERY_REPLY          = -1000520,
    SAY_LE_NO_LEAVE                 = -1000521,
    SAY_LE_NO_LEAVE_REPLY1          = -1000522,
    SAY_LE_NO_LEAVE_REPLY2          = -1000523,
    SAY_LE_NO_LEAVE_REPLY3          = -1000524,
    SAY_LE_NO_LEAVE_REPLY4          = -1000525,
    SAY_LE_SHUT                     = -1000526,
    SAY_LE_REPLY_HEAR               = -1000527,
    SAY_LE_IN_YOUR_FACE             = -1000528,
    SAY_LE_HELP_HIM                 = -1000529,
    EMOTE_LE_PICK_UP                = -1000530,
    SAY_LE_THANKS                   = -1000531,

    QUEST_DIGGING_BONES             = 10922,

    NPC_RESEARCHER                  = 22464,
    NPC_BONE_SIFTER                 = 22466,

    MAX_RESEARCHER                  = 4
};

//Some details still missing from here, and will also have issues if followers evade for any reason.
struct MANGOS_DLL_DECL npc_letollAI : public npc_escortAI
{
    npc_letollAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_uiEventTimer = 5000;
        m_uiEventCount = 0;
        Reset();
    }

    std::list<Creature*> m_lResearchersList;

    uint32 m_uiEventTimer;
    uint32 m_uiEventCount;

    void Reset() {}

    //will make them follow, but will only work until they enter combat with any unit
    void SetFormation()
    {
        uint32 uiCount = 0;

        for(std::list<Creature*>::iterator itr = m_lResearchersList.begin(); itr != m_lResearchersList.end(); ++itr)
        {
            float fAngle = uiCount < MAX_RESEARCHER ? M_PI/MAX_RESEARCHER - (uiCount*2*M_PI/MAX_RESEARCHER) : 0.0f;

            if ((*itr)->isAlive() && !(*itr)->isInCombat())
                (*itr)->GetMotionMaster()->MoveFollow(m_creature, 2.5f, fAngle);

            ++uiCount;
        }
    }

    Creature* GetAvailableResearcher(uint8 uiListNum)
    {
        if (!m_lResearchersList.empty())
        {
            uint8 uiNum = 1;

            for(std::list<Creature*>::iterator itr = m_lResearchersList.begin(); itr != m_lResearchersList.end(); ++itr)
            {
                if (uiListNum && uiListNum != uiNum)
                {
                    ++uiNum;
                    continue;
                }

                if ((*itr)->isAlive() && (*itr)->IsWithinDistInMap(m_creature, 20.0f))
                    return (*itr);
            }
        }

        return NULL;
    }

    void JustStartedEscort()
    {
        m_uiEventTimer = 5000;
        m_uiEventCount = 0;

        m_lResearchersList.clear();

        GetCreatureListWithEntryInGrid(m_lResearchersList, m_creature, NPC_RESEARCHER, 25.0f);

        if (!m_lResearchersList.empty())
            SetFormation();
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 0:
                if (Player* pPlayer = GetPlayerForEscort())
                    DoScriptText(SAY_LE_KEEP_SAFE, m_creature, pPlayer);
                break;
            case 1:
                DoScriptText(SAY_LE_NORTH, m_creature);
                break;
            case 10:
                DoScriptText(SAY_LE_ARRIVE, m_creature);
                break;
            case 12:
                DoScriptText(SAY_LE_BURIED, m_creature);
                SetEscortPaused(true);
                break;
            case 13:
                SetRun();
                break;
        }
    }

    void Aggro(Unit* pWho)
    {
        if (pWho->isInCombat() && pWho->GetTypeId() == TYPEID_UNIT && pWho->GetEntry() == NPC_BONE_SIFTER)
            DoScriptText(SAY_LE_HELP_HIM, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (pPlayer && pPlayer->isAlive())
            pSummoned->AI()->AttackStart(pPlayer);
        else
            pSummoned->AI()->AttackStart(m_creature);
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (HasEscortState(STATE_ESCORT_PAUSED))
            {
                if (m_uiEventTimer < uiDiff)
                {
                    m_uiEventTimer = 7000;

                    switch(m_uiEventCount)
                    {
                        case 0:
                            DoScriptText(SAY_LE_ALMOST, m_creature);
                            break;
                        case 1:
                            DoScriptText(SAY_LE_DRUM, m_creature);
                            break;
                        case 2:
                            if (Creature* pResearcher = GetAvailableResearcher(0))
                                DoScriptText(SAY_LE_DRUM_REPLY, pResearcher);
                            break;
                        case 3:
                            DoScriptText(SAY_LE_DISCOVERY, m_creature);
                            break;
                        case 4:
                            if (Creature* pResearcher = GetAvailableResearcher(0))
                                DoScriptText(SAY_LE_DISCOVERY_REPLY, pResearcher);
                            break;
                        case 5:
                            DoScriptText(SAY_LE_NO_LEAVE, m_creature);
                            break;
                        case 6:
                            if (Creature* pResearcher = GetAvailableResearcher(1))
                                DoScriptText(SAY_LE_NO_LEAVE_REPLY1, pResearcher);
                            break;
                        case 7:
                            if (Creature* pResearcher = GetAvailableResearcher(2))
                                DoScriptText(SAY_LE_NO_LEAVE_REPLY2, pResearcher);
                            break;
                        case 8:
                            if (Creature* pResearcher = GetAvailableResearcher(3))
                                DoScriptText(SAY_LE_NO_LEAVE_REPLY3, pResearcher);
                            break;
                        case 9:
                            if (Creature* pResearcher = GetAvailableResearcher(4))
                                DoScriptText(SAY_LE_NO_LEAVE_REPLY4, pResearcher);
                            break;
                        case 10:
                            DoScriptText(SAY_LE_SHUT, m_creature);
                            break;
                        case 11:
                            if (Creature* pResearcher = GetAvailableResearcher(0))
                                DoScriptText(SAY_LE_REPLY_HEAR, pResearcher);
                            break;
                        case 12:
                            DoScriptText(SAY_LE_IN_YOUR_FACE, m_creature);
                            m_creature->SummonCreature(NPC_BONE_SIFTER, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                            break;
                        case 13:
                            DoScriptText(EMOTE_LE_PICK_UP, m_creature);

                            if (Player* pPlayer = GetPlayerForEscort())
                            {
                                DoScriptText(SAY_LE_THANKS, m_creature, pPlayer);
                                pPlayer->GroupEventHappens(QUEST_DIGGING_BONES, m_creature);
                            }

                            SetEscortPaused(false);
                            break;
                    }

                    ++m_uiEventCount;
                }
                else
                    m_uiEventTimer -= uiDiff;
            }

            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_letoll(Creature* pCreature)
{
    return new npc_letollAI(pCreature);
}

bool QuestAccept_npc_letoll(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_DIGGING_BONES)
    {
        if (npc_letollAI* pEscortAI = dynamic_cast<npc_letollAI*>(pCreature->AI()))
        {
            DoScriptText(SAY_LE_START, pCreature);
            pCreature->setFaction(FACTION_ESCORT_N_NEUTRAL_PASSIVE);

            pEscortAI->Start(false, pPlayer->GetGUID(), pQuest, true);
        }
    }

    return true;
}

/*######
## npc_mana_bomb_exp_trigger
######*/

enum
{
    SAY_COUNT_1                 = -1000472,
    SAY_COUNT_2                 = -1000473,
    SAY_COUNT_3                 = -1000474,
    SAY_COUNT_4                 = -1000475,
    SAY_COUNT_5                 = -1000476,

    SPELL_MANA_BOMB_LIGHTNING   = 37843,
    SPELL_MANA_BOMB_EXPL        = 35513,

    NPC_MANA_BOMB_EXPL_TRIGGER  = 20767,
    NPC_MANA_BOMB_KILL_TRIGGER  = 21039
};

struct MANGOS_DLL_DECL npc_mana_bomb_exp_triggerAI : public ScriptedAI
{
    npc_mana_bomb_exp_triggerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    GameObject* pManaBomb;

    bool m_bIsActivated;
    uint32 m_uiEventTimer;
    uint32 m_uiEventCounter;

    void Reset()
    {
        pManaBomb = NULL;
        m_bIsActivated = false;
        m_uiEventTimer = 1000;
        m_uiEventCounter = 0;
    }

    void DoTrigger(Player* pPlayer, GameObject* pGo)
    {
        if (m_bIsActivated)
            return;

        m_bIsActivated = true;

        pPlayer->KilledMonsterCredit(NPC_MANA_BOMB_KILL_TRIGGER);

        pManaBomb = pGo;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsActivated)
            return;

        if (m_uiEventTimer < uiDiff)
        {
            m_uiEventTimer = 1000;

            if (m_uiEventCounter < 10)
                m_creature->CastSpell(m_creature, SPELL_MANA_BOMB_LIGHTNING, false);

            switch(m_uiEventCounter)
            {
                case 5:
                    if (pManaBomb)
                        pManaBomb->SetGoState(GO_STATE_ACTIVE);

                    DoScriptText(SAY_COUNT_1, m_creature);
                    break;
                case 6:
                    DoScriptText(SAY_COUNT_2, m_creature);
                    break;
                case 7:
                    DoScriptText(SAY_COUNT_3, m_creature);
                    break;
                case 8:
                    DoScriptText(SAY_COUNT_4, m_creature);
                    break;
                case 9:
                    DoScriptText(SAY_COUNT_5, m_creature);
                    break;
                case 10:
                    m_creature->CastSpell(m_creature, SPELL_MANA_BOMB_EXPL, false);
                    break;
                case 30:
                    if (pManaBomb)
                        pManaBomb->SetGoState(GO_STATE_READY);

                    Reset();
                    break;
            }

            ++m_uiEventCounter;
        }
        else
            m_uiEventTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_mana_bomb_exp_trigger(Creature* pCreature)
{
    return new npc_mana_bomb_exp_triggerAI(pCreature);
}

/*######
## go_mana_bomb
######*/

bool GOUse_go_mana_bomb(Player* pPlayer, GameObject* pGo)
{
    if (Creature* pCreature = GetClosestCreatureWithEntry(pGo, NPC_MANA_BOMB_EXPL_TRIGGER, INTERACTION_DISTANCE))
    {
        if (npc_mana_bomb_exp_triggerAI* pBombAI = dynamic_cast<npc_mana_bomb_exp_triggerAI*>(pCreature->AI()))
            pBombAI->DoTrigger(pPlayer, pGo);
    }

    return true;
}

/*######
## npc_slim
######*/

enum
{
    FACTION_CONSORTIUM  = 933
};

bool GossipHello_npc_slim(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isVendor() && pPlayer->GetReputationRank(FACTION_CONSORTIUM) >= REP_FRIENDLY)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        pPlayer->SEND_GOSSIP_MENU(9896, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(9895, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_slim(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetGUID());

    return true;
}

/*#####
## go_veil_skith_cage & npc_captive_child
#####*/

enum
{
    QUEST_MISSING_FRIENDS     = 10852,
    NPC_CAPTIVE_CHILD         = 22314,
    SAY_THANKS_1              = -1000590,
    SAY_THANKS_2              = -1000591,
    SAY_THANKS_3              = -1000592,
    SAY_THANKS_4              = -1000593
};

bool GOUse_go_veil_skith_cage(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestStatus(QUEST_MISSING_FRIENDS) == QUEST_STATUS_INCOMPLETE)
    {
        std::list<Creature*> lChildrenList;
        GetCreatureListWithEntryInGrid(lChildrenList, pGo, NPC_CAPTIVE_CHILD, INTERACTION_DISTANCE);
        for(std::list<Creature*>::const_iterator itr = lChildrenList.begin(); itr != lChildrenList.end(); ++itr)
        {
            pPlayer->KilledMonsterCredit(NPC_CAPTIVE_CHILD, (*itr)->GetGUID());
            switch(urand(0,3))
            {
                case 0: DoScriptText(SAY_THANKS_1, *itr); break;
                case 1: DoScriptText(SAY_THANKS_2, *itr); break;
                case 2: DoScriptText(SAY_THANKS_3, *itr); break;
                case 3: DoScriptText(SAY_THANKS_4, *itr); break;
            }

            (*itr)->GetMotionMaster()->Clear();
            (*itr)->GetMotionMaster()->MovePoint(0, -2648.049f, 5274.573f, 1.691529f);
        }
    }
    return false;
};

struct MANGOS_DLL_DECL npc_captive_child : public ScriptedAI
{
    npc_captive_child(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() {}

    void WaypointReached(uint32 uiPointId)
    {
        // we only have one waypoint
        m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_npc_captive_child(Creature* pCreature)
{
    return new npc_captive_child(pCreature);
}

/*#####
## npc_isla_starmane
##
##  TODO: Verify SpellIDs, Research Timers, Finish Text?
#####*/

enum
{
    QUEST_ESCAPE_FROM_FIREWING_POINT_A  = 10051,
    QUEST_ESCAPE_FROM_FIREWING_POINT_H  = 10052,

    SAY_ISLA_PERIODIC_1                 = -1000629,
    SAY_ISLA_PERIODIC_2                 = -1000630,
    SAY_ISLA_PERIODIC_3                 = -1000631,
    SAY_ISLA_START                      = -1000632,
    SAY_ISLA_WAITING                    = -1000633,
    SAY_ISLA_LEAVE_BUILDING             = -1000634,

    GO_CAGE                             = 182794,

    SPELL_ENTANGLING_ROOTS              = 33844,            // these spell IDs seem to deal not enough dmg, but are linked
    SPELL_MOONFIRE                      = 15798,
    SPELL_WRATH                         = 9739,
    SPELL_TRAVELFORM                    = 32447             // guesswork
};

struct MANGOS_DLL_DECL npc_isla_starmaneAI : public npc_escortAI
{
    npc_isla_starmaneAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiPeriodicTalkTimer;
    uint32 m_uiEntanglingRootsTimer;
    uint32 m_uiMoonfireTimer;
    uint32 m_uiWrathTimer;

    void Reset()
    {
        m_uiPeriodicTalkTimer = urand(20000, 40000);
        m_uiEntanglingRootsTimer = 100;
        m_uiMoonfireTimer = 1600;
        m_uiWrathTimer = 2000;

        if (!HasEscortState(STATE_ESCORT_ESCORTING))
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
    }

    void JustStartedEscort()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
        DoScriptText(SAY_ISLA_START, m_creature);
        if (GameObject* pCage = GetClosestGameObjectWithEntry(m_creature, GO_CAGE, 2*INTERACTION_DISTANCE))
            pCage->Use(m_creature);
    }

    void WaypointStart(uint32 uiPointId)
    {
        switch (uiPointId)
        {
            case 7:  DoScriptText(SAY_ISLA_LEAVE_BUILDING, m_creature); break;
            case 68: DoCastSpellIfCan(m_creature, SPELL_TRAVELFORM);    break;
        }
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 6:
                DoScriptText(SAY_ISLA_WAITING, m_creature);
                break;
            case 61:
                if (Player* pPlayer = GetPlayerForEscort())
                    pPlayer->GroupEventHappens(pPlayer->GetTeam() == ALLIANCE ? QUEST_ESCAPE_FROM_FIREWING_POINT_A : QUEST_ESCAPE_FROM_FIREWING_POINT_H, m_creature);
                break;
            case 67:
                if (Player* pPlayer = GetPlayerForEscort())
                    m_creature->SetFacingToObject(pPlayer);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                break;
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!HasEscortState(STATE_ESCORT_ESCORTING))
        {
            if (m_uiPeriodicTalkTimer < uiDiff)
            {
                m_uiPeriodicTalkTimer = urand(30000, 60000);
                switch (urand(0, 2))
                {
                    case 0: DoScriptText(SAY_ISLA_PERIODIC_1, m_creature); break;
                    case 1: DoScriptText(SAY_ISLA_PERIODIC_2, m_creature); break;
                    case 2: DoScriptText(SAY_ISLA_PERIODIC_3, m_creature); break;
                }
            }
            else
                m_uiPeriodicTalkTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEntanglingRootsTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ENTANGLING_ROOTS) == CAST_OK)
                m_uiEntanglingRootsTimer = urand(8000, 16000);
        }
        else
            m_uiEntanglingRootsTimer -= uiDiff;

        if (m_uiMoonfireTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MOONFIRE) == CAST_OK)
                m_uiMoonfireTimer = urand(6000, 12000);
        }
        else
            m_uiMoonfireTimer -= uiDiff;

        if (m_uiWrathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_WRATH) == CAST_OK)
                m_uiWrathTimer = 2000;
        }
        else
            m_uiWrathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

bool QuestAccept_npc_isla_starmane(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_ESCAPE_FROM_FIREWING_POINT_A || pQuest->GetQuestId() == QUEST_ESCAPE_FROM_FIREWING_POINT_H)
    {
        if (npc_isla_starmaneAI* pEscortAI = dynamic_cast<npc_isla_starmaneAI*>(pCreature->AI()))
        {
            pCreature->setFaction(pPlayer->GetTeam() == ALLIANCE ? FACTION_ESCORT_A_NEUTRAL_ACTIVE : FACTION_ESCORT_H_NEUTRAL_ACTIVE);
            pEscortAI->Start(false, pPlayer->GetGUID(), pQuest);
        }
    }
    return true;
}

CreatureAI* GetAI_npc_isla_starmane(Creature* pCreature)
{
    return new npc_isla_starmaneAI(pCreature);
}

void AddSC_terokkar_forest()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mob_unkor_the_ruthless";
    pNewScript->GetAI = &GetAI_mob_unkor_the_ruthless;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_infested_root_walker";
    pNewScript->GetAI = &GetAI_mob_infested_root_walker;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_rotting_forest_rager";
    pNewScript->GetAI = &GetAI_mob_rotting_forest_rager;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_netherweb_victim";
    pNewScript->GetAI = &GetAI_mob_netherweb_victim;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_akuno";
    pNewScript->GetAI = &GetAI_npc_akuno;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_akuno;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_floon";
    pNewScript->GetAI = &GetAI_npc_floon;
    pNewScript->pGossipHello =  &GossipHello_npc_floon;
    pNewScript->pGossipSelect = &GossipSelect_npc_floon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_hungry_nether_ray";
    pNewScript->GetAI = &GetAI_npc_hungry_nether_ray;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_letoll";
    pNewScript->GetAI = &GetAI_npc_letoll;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_letoll;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_mana_bomb_exp_trigger";
    pNewScript->GetAI = &GetAI_npc_mana_bomb_exp_trigger;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_mana_bomb";
    pNewScript->pGOUse = &GOUse_go_mana_bomb;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_skyguard_handler_deesak";
    pNewScript->pGossipHello =  &GossipHello_npc_skyguard_handler_deesak;
    pNewScript->pGossipSelect = &GossipSelect_npc_skyguard_handler_deesak;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_slim";
    pNewScript->pGossipHello =  &GossipHello_npc_slim;
    pNewScript->pGossipSelect = &GossipSelect_npc_slim;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_veil_skith_cage";
    pNewScript->pGOUse = &GOUse_go_veil_skith_cage;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_captive_child";
    pNewScript->GetAI = &GetAI_npc_captive_child;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_isla_starmane";
    pNewScript->GetAI = &GetAI_npc_isla_starmane;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_isla_starmane;
    pNewScript->RegisterSelf();
}
