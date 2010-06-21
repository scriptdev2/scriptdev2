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
SDName: Blackrock_Depths
SD%Complete: 50
SDComment: Quest support: 4001, 4342, 7604. Vendor Lokhtos Darkbargainer.
SDCategory: Blackrock Depths
EndScriptData */

/* ContentData
go_shadowforge_brazier
at_ring_of_law
npc_grimstone
mob_phalanx
npc_kharan_mighthammer
npc_lokhtos_darkbargainer
EndContentData */

#include "precompiled.h"
#include "blackrock_depths.h"
#include "escort_ai.h"

/*######
## go_shadowforge_brazier
######*/

bool GOHello_go_shadowforge_brazier(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_LYCEUM) == IN_PROGRESS)
            pInstance->SetData(TYPE_LYCEUM, DONE);
        else
            pInstance->SetData(TYPE_LYCEUM, IN_PROGRESS);
    }
    return false;
}

/*######
## npc_grimstone
######*/

enum
{
    NPC_GRIMSTONE       = 10096,
    NPC_THELDREN        = 16059,

    //4 or 6 in total? 1+2+1 / 2+2+2 / 3+3. Depending on this, code should be changed.
    MAX_MOB_AMOUNT      = 4
};

static uint32 RingMob[]=
{
    8925,                                                   // Dredge Worm
    8926,                                                   // Deep Stinger
    8927,                                                   // Dark Screecher
    8928,                                                   // Burrowing Thundersnout
    8933,                                                   // Cave Creeper
    8932,                                                   // Borer Beetle
};

static uint32 RingBoss[]=
{
    9027,                                                   // Gorosh
    9028,                                                   // Grizzle
    9029,                                                   // Eviscerator
    9030,                                                   // Ok'thor
    9031,                                                   // Anub'shiah
    9032,                                                   // Hedrum
};

bool AreaTrigger_at_ring_of_law(Player* pPlayer, AreaTriggerEntry* pAt)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_RING_OF_LAW) == IN_PROGRESS || pInstance->GetData(TYPE_RING_OF_LAW) == DONE)
            return false;

        pInstance->SetData(TYPE_RING_OF_LAW, IN_PROGRESS);
        pPlayer->SummonCreature(NPC_GRIMSTONE, 625.559f, -205.618f, -52.735f, 2.609f, TEMPSUMMON_DEAD_DESPAWN, 0);

        return false;
    }
    return false;
}

/*######
## npc_grimstone
######*/

//TODO: implement quest part of event (different end boss)
struct MANGOS_DLL_DECL npc_grimstoneAI : public npc_escortAI
{
    npc_grimstoneAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_uiMobSpawnId = urand(0, 5);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint8 m_uiEventPhase;
    uint32 m_uiEventTimer;

    uint8 m_uiMobSpawnId;
    uint8 m_uiMobCount;
    uint32 m_uiMobDeathTimer;

    uint64 m_auiRingMobGUID[MAX_MOB_AMOUNT];
    uint64 m_uiRingBossGUID;

    bool m_bCanWalk;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        memset(&m_auiRingMobGUID, 0, sizeof(m_auiRingMobGUID));

        m_uiEventTimer    = 1000;
        m_uiEventPhase    = 0;
        m_uiMobCount      = 0;
        m_uiMobDeathTimer = 0;
        m_uiRingBossGUID  = 0;

        m_bCanWalk = false;
    }

    void JustSummoned(Creature* pSummoned)
    {

    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {

    }
    
    void DoGate(uint32 id, uint32 state)
    {
        if (GameObject* pGo = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(id)))
            pGo->SetGoState(GOState(state));

        debug_log("SD2: npc_grimstone, arena gate update state.");
    }

    //TODO: move them to center
    void SummonRingMob()
    {
        if (Creature* pTmp = m_creature->SummonCreature(RingMob[m_uiMobSpawnId], 608.960f, -235.322f, -53.907f, 1.857f, TEMPSUMMON_DEAD_DESPAWN, 0))
            m_auiRingMobGUID[m_uiMobCount] = pTmp->GetGUID();

        ++m_uiMobCount;

        if (m_uiMobCount == MAX_MOB_AMOUNT)
            m_uiMobDeathTimer = 2500;
    }

    //TODO: move them to center
    void SummonRingBoss()
    {
        if (Creature* pTmp = m_creature->SummonCreature(RingBoss[urand(0, 5)], 644.300f, -175.989f, -53.739f, 3.418f, TEMPSUMMON_DEAD_DESPAWN, 0))
            m_uiRingBossGUID = pTmp->GetGUID();

        m_uiMobDeathTimer = 2500;
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 0:
                DoScriptText(-1000000, m_creature);//2
                m_bCanWalk = false;
                m_uiEventTimer = 5000;
                break;
            case 1:
                DoScriptText(-1000000, m_creature);//4
                m_bCanWalk = false;
                m_uiEventTimer = 5000;
                break;
            case 2:
                m_bCanWalk = false;
                break;
            case 3:
                DoScriptText(-1000000, m_creature);//5
                break;
            case 4:
                DoScriptText(-1000000, m_creature);//6
                m_bCanWalk = false;
                m_uiEventTimer = 5000;
                break;
            case 5:
                if (m_pInstance)
                {
                    m_pInstance->SetData(TYPE_RING_OF_LAW, DONE);
                    debug_log("SD2: npc_grimstone: event reached end and set complete.");
                }
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (m_uiMobDeathTimer)
        {
            if (m_uiMobDeathTimer <= uiDiff)
            {
                m_uiMobDeathTimer = 2500;

                if (m_uiRingBossGUID)
                {
                    Creature* pBoss = (Creature*)Unit::GetUnit(*m_creature, m_uiRingBossGUID);
                    if (pBoss && !pBoss->isAlive() && pBoss->isDead())
                    {
                        m_uiRingBossGUID = 0;
                        m_uiEventTimer = 5000;
                        m_uiMobDeathTimer = 0;
                        return;
                    }
                    return;
                }

                for(uint8 i = 0; i < MAX_MOB_AMOUNT; ++i)
                {
                    Creature* pMob = (Creature*)Unit::GetUnit(*m_creature, m_auiRingMobGUID[i]);
                    if (pMob && !pMob->isAlive() && pMob->isDead())
                    {
                        m_auiRingMobGUID[i] = 0;
                        --m_uiMobCount;

                        //seems all are gone, so set timer to continue and discontinue this
                        if (!m_uiMobCount)
                        {
                            m_uiEventTimer = 5000;
                            m_uiMobDeathTimer = 0;
                        }
                    }
                }
            }
            else
                m_uiMobDeathTimer -= uiDiff;
        }

        if (m_uiEventTimer)
        {
            if (m_uiEventTimer <= uiDiff)
            {
                switch(m_uiEventPhase)
                {
                    case 0:
                        DoScriptText(-1000000, m_creature);
                        DoGate(DATA_ARENA4, GO_STATE_READY);
                        Start(false, false);
                        m_bCanWalk = true;
                        m_uiEventTimer = 0;
                        break;
                    case 1:
                        m_bCanWalk = true;
                        m_uiEventTimer = 0;
                        break;
                    case 2:
                        m_uiEventTimer = 2000;
                        break;
                    case 3:
                        DoGate(DATA_ARENA1, GO_STATE_ACTIVE);
                        m_uiEventTimer = 3000;
                        break;
                    case 4:
                        m_bCanWalk = true;
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        SummonRingMob();
                        m_uiEventTimer = 8000;
                        break;
                    case 5:
                        SummonRingMob();
                        SummonRingMob();
                        m_uiEventTimer = 8000;
                        break;
                    case 6:
                        SummonRingMob();
                        m_uiEventTimer = 0;
                        break;
                    case 7:
                        m_creature->SetVisibility(VISIBILITY_ON);
                        DoGate(DATA_ARENA1, GO_STATE_READY);
                        DoScriptText(-1000000, m_creature);
                        m_bCanWalk = true;
                        m_uiEventTimer = 0;
                        break;
                    case 8:
                        DoGate(DATA_ARENA2, GO_STATE_ACTIVE);
                        m_uiEventTimer = 5000;
                        break;
                    case 9:
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        SummonRingBoss();
                        m_uiEventTimer = 0;
                        break;
                    case 10:
                        //if quest, complete
                        DoGate(DATA_ARENA2, GO_STATE_READY);
                        DoGate(DATA_ARENA3, GO_STATE_ACTIVE);
                        DoGate(DATA_ARENA4, GO_STATE_ACTIVE);
                        m_bCanWalk = true;
                        m_uiEventTimer = 0;
                        break;
                }
                ++m_uiEventPhase;
            }
            else
                m_uiEventTimer -= uiDiff;
        }

        if (m_bCanWalk)
            npc_escortAI::UpdateAI(uiDiff);
    }
};

CreatureAI* GetAI_npc_grimstone(Creature* pCreature)
{
    return new npc_grimstoneAI(pCreature);
}

/*######
## mob_phalanx
######*/

enum
{
    SPELL_THUNDERCLAP    = 15588,
    SPELL_FIREBALLVOLLEY = 15285,
    SPELL_MIGHTYBLOW     = 14099
};

struct MANGOS_DLL_DECL mob_phalanxAI : public ScriptedAI
{
    mob_phalanxAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiThunderClapTimer;
    uint32 m_uiFireballVolleyTimer;
    uint32 m_uiMightyBlowTimer;

    void Reset()
    {
        m_uiThunderClapTimer    = 12000;
        m_uiFireballVolleyTimer = 0;
        m_uiMightyBlowTimer     = 15000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // ThunderClap
        if (m_uiThunderClapTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_THUNDERCLAP);
            m_uiThunderClapTimer = 10000;
        }
        else
            m_uiThunderClapTimer -= uiDiff;

        // FireballVolley
        if (m_creature->GetHealthPercent() < 51.0f)
        {
            if (m_uiFireballVolleyTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIREBALLVOLLEY);
                m_uiFireballVolleyTimer = 15000;
            }
            else
                m_uiFireballVolleyTimer -= uiDiff;
        }

        // MightyBlow
        if (m_uiMightyBlowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTYBLOW);
            m_uiMightyBlowTimer = 10000;
        }
        else
            m_uiMightyBlowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_phalanx(Creature* pCreature)
{
    return new mob_phalanxAI(pCreature);
}

/*######
## npc_kharan_mighthammer
######*/
enum
{
    QUEST_WHAT_IS_GOING_ON = 4001,
    QUEST_KHARANS_TALE     = 4342
};

#define GOSSIP_ITEM_KHARAN_1    "I need to know where the princess are, Kharan!"
#define GOSSIP_ITEM_KHARAN_2    "All is not lost, Kharan!"

#define GOSSIP_ITEM_KHARAN_3    "Gor'shak is my friend, you can trust me."
#define GOSSIP_ITEM_KHARAN_4    "Not enough, you need to tell me more."
#define GOSSIP_ITEM_KHARAN_5    "So what happened?"
#define GOSSIP_ITEM_KHARAN_6    "Continue..."
#define GOSSIP_ITEM_KHARAN_7    "So you suspect that someone on the inside was involved? That they were tipped off?"
#define GOSSIP_ITEM_KHARAN_8    "Continue with your story please."
#define GOSSIP_ITEM_KHARAN_9    "Indeed."
#define GOSSIP_ITEM_KHARAN_10   "The door is open, Kharan. You are a free man."

bool GossipHello_npc_kharan_mighthammer(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_WHAT_IS_GOING_ON) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    if (pPlayer->GetQuestStatus(QUEST_KHARANS_TALE) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

    if (pPlayer->GetTeam() == HORDE)
        pPlayer->SEND_GOSSIP_MENU(2473, pCreature->GetGUID());
    else
        pPlayer->SEND_GOSSIP_MENU(2474, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_kharan_mighthammer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(2475, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(2476, pCreature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->SEND_GOSSIP_MENU(2477, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(2478, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            pPlayer->SEND_GOSSIP_MENU(2479, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_8, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
            pPlayer->SEND_GOSSIP_MENU(2480, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_9, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+8);
            pPlayer->SEND_GOSSIP_MENU(2481, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+8:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KHARAN_10, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+9);
            pPlayer->SEND_GOSSIP_MENU(2482, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+9:
            pPlayer->CLOSE_GOSSIP_MENU();
            if (pPlayer->GetTeam() == HORDE)
                pPlayer->AreaExploredOrEventHappens(QUEST_WHAT_IS_GOING_ON);
            else
                pPlayer->AreaExploredOrEventHappens(QUEST_KHARANS_TALE);
            break;
    }
    return true;
}

/*######
## npc_lokhtos_darkbargainer
######*/

enum
{
    FACTION_THORIUM_BROTHERHOOD               = 59,
    
    ITEM_THRORIUM_BROTHERHOOD_CONTRACT        = 18628,
    ITEM_SULFURON_INGOT                       = 17203,
    
    QUEST_A_BINDING_CONTRACT                  = 7604,
    
    SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT = 23059
};

#define GOSSIP_ITEM_SHOW_ACCESS     "Show me what I have access to, Lothos."
#define GOSSIP_ITEM_GET_CONTRACT    "Get Thorium Brotherhood Contract"

bool GossipHello_npc_lokhtos_darkbargainer(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pCreature->isVendor() && pPlayer->GetReputationRank(FACTION_THORIUM_BROTHERHOOD) >= REP_FRIENDLY)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_ITEM_SHOW_ACCESS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    if (!pPlayer->GetQuestRewardStatus(QUEST_A_BINDING_CONTRACT) &&
        !pPlayer->HasItemCount(ITEM_THRORIUM_BROTHERHOOD_CONTRACT, 1, true) &&
        pPlayer->HasItemCount(ITEM_SULFURON_INGOT, 1))
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GET_CONTRACT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    }

    if (pPlayer->GetReputationRank(FACTION_THORIUM_BROTHERHOOD) < REP_FRIENDLY)
        pPlayer->SEND_GOSSIP_MENU(3673, pCreature->GetGUID());
    else
        pPlayer->SEND_GOSSIP_MENU(3677, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_lokhtos_darkbargainer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT, false);
    }

    if (uiAction == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetGUID());

    return true;
}

/*######
## npc_rocknot
######*/

enum
{
    SAY_GOT_BEER       = -1230000,

    SPELL_DRUNKEN_RAGE = 14872,

    QUEST_ALE          = 4295
};

struct MANGOS_DLL_DECL npc_rocknotAI : public npc_escortAI
{
    npc_rocknotAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiBreakKegTimer;
    uint32 m_uiBreakDoorTimer;

    void Reset()
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            return;

        m_uiBreakKegTimer  = 0;
        m_uiBreakDoorTimer = 0;
    }

    void DoGo(uint32 id, uint32 state)
    {
        if (GameObject* pGo = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(id)))
            pGo->SetGoState(GOState(state));
    }

    void WaypointReached(uint32 uiPointId)
    {
        if (!m_pInstance)
            return;

        switch(uiPointId)
        {
            case 1:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KICK);
                break;
            case 2:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ATTACKUNARMED);
                break;
            case 3:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ATTACKUNARMED);
                break;
            case 4:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KICK);
                break;
            case 5:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KICK);
                m_uiBreakKegTimer = 2000;
                break;
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (m_uiBreakKegTimer)
        {
            if (m_uiBreakKegTimer <= uiDiff)
            {
                DoGo(DATA_GO_BAR_KEG,0);
                m_uiBreakKegTimer = 0;
                m_uiBreakDoorTimer = 1000;
            }
            else
                m_uiBreakKegTimer -= uiDiff;
        }

        if (m_uiBreakDoorTimer)
        {
            if (m_uiBreakDoorTimer <= uiDiff)
            {
                DoGo(DATA_GO_BAR_DOOR, 2);
                DoGo(DATA_GO_BAR_KEG_TRAP, 0);              //doesn't work very well, leaving code here for future
                                                            //spell by trap has effect61, this indicate the bar go hostile

                if (Unit* pTmp = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_PHALANX)))
                    pTmp->setFaction(14);

                // for later, this event(s) has alot more to it.
                // optionally, DONE can trigger bar to go hostile.
                m_pInstance->SetData(TYPE_BAR, DONE);

                m_uiBreakDoorTimer = 0;
            }
            else
                m_uiBreakDoorTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_rocknot(Creature* pCreature)
{
    return new npc_rocknotAI(pCreature);
}

bool ChooseReward_npc_rocknot(Player* pPlayer, Creature* pCreature, const Quest* pQuest, uint32 item)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());

    if (!pInstance)
        return true;

    if (pInstance->GetData(TYPE_BAR) == DONE || pInstance->GetData(TYPE_BAR) == SPECIAL)
        return true;

    if (pQuest->GetQuestId() == QUEST_ALE)
    {
        if (pInstance->GetData(TYPE_BAR) != IN_PROGRESS)
            pInstance->SetData(TYPE_BAR,IN_PROGRESS);

        pInstance->SetData(TYPE_BAR,SPECIAL);

        // keep track of amount in instance script, returns SPECIAL if amount ok and event in progress
        if (pInstance->GetData(TYPE_BAR) == SPECIAL)
        {
            DoScriptText(SAY_GOT_BEER, pCreature);
            pCreature->CastSpell(pCreature, SPELL_DRUNKEN_RAGE, false);

            if (npc_rocknotAI* pEscortAI = dynamic_cast<npc_rocknotAI*>(pCreature->AI()))
                pEscortAI->Start(false, false, 0, NULL, true);
        }
    }

    return true;
}

void AddSC_blackrock_depths()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "go_shadowforge_brazier";
    pNewScript->pGOHello = &GOHello_go_shadowforge_brazier;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_ring_of_law";
    pNewScript->pAreaTrigger = &AreaTrigger_at_ring_of_law;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_grimstone";
    pNewScript->GetAI = &GetAI_npc_grimstone;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_phalanx";
    pNewScript->GetAI = &GetAI_mob_phalanx;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_kharan_mighthammer";
    pNewScript->pGossipHello =  &GossipHello_npc_kharan_mighthammer;
    pNewScript->pGossipSelect = &GossipSelect_npc_kharan_mighthammer;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_lokhtos_darkbargainer";
    pNewScript->pGossipHello =  &GossipHello_npc_lokhtos_darkbargainer;
    pNewScript->pGossipSelect = &GossipSelect_npc_lokhtos_darkbargainer;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_rocknot";
    pNewScript->GetAI = &GetAI_npc_rocknot;
    pNewScript->pChooseReward = &ChooseReward_npc_rocknot;
    pNewScript->RegisterSelf();
}
