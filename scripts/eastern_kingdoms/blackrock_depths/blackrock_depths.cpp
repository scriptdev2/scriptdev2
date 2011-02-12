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
SDName: Blackrock_Depths
SD%Complete: 80
SDComment: Quest support: 4001, 4342, 7604, 9015. Vendor Lokhtos Darkbargainer.
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

bool GOUse_go_shadowforge_brazier(Player* pPlayer, GameObject* pGo)
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

/* Notes about this event:
 * Visual: Npc Grimstone should use some visual spell when appear/ disappear / opening/ closing doors
 * Texts: The texts and their positions need confirmation
 * Event timer might also need adjustment
 * Quest-Event: This needs to be clearified - there is some suggestion, that Theldren&Adds also might come as first wave.
 */

enum
{
    SAY_START_1                     = -1230004,
    SAY_START_2                     = -1230005,
    SAY_OPEN_EAST_GATE              = -1230006,
    SAY_SUMMON_BOSS_1               = -1230007,
    SAY_SUMMON_BOSS_2               = -1230008,
    SAY_OPEN_NORTH_GATE             = -1230009,

    NPC_GRIMSTONE                   = 10096,
    DATA_BANNER_BEFORE_EVENT        = 5,

    //4 or 6 in total? 1+2+1 / 2+2+2 / 3+3. Depending on this, code should be changed.
    MAX_MOB_AMOUNT                  = 4,
    MAX_THELDREN_ADDS               = 4,
    MAX_POSSIBLE_THELDREN_ADDS      = 8,

    SPELL_SUMMON_THELRIN_DND        = 27517,
    /* Other spells used by Grimstone
    SPELL_ASHCROMBES_TELEPORT_A     = 15742
    SPELL_ASHCROMBES_TELEPORT_B     = 6422,
    SPELL_ARENA_FLASH_A             = 15737,
    SPELL_ARENA_FLASH_B             = 15739,
    */

    QUEST_THE_CHALLENGE             = 9015,
    NPC_THELDREN_QUEST_CREDIT       = 16166,
};

enum SpawnPosition
{
    POS_EAST                        = 0,
    POS_NORTH                       = 1,
    POS_GRIMSTONE                   = 2,
};

static const float aSpawnPositions[3][4] =
{
    {608.960f, -235.322f, -53.907f, 1.857f},                // Ring mob spawn position
    {644.300f, -175.989f, -53.739f, 3.418f},                // Ring boss spawn position
    {625.559f, -205.618f, -52.735f, 2.609f}                 // Grimstone spawn position
};

static const uint32 aGladiator[MAX_POSSIBLE_THELDREN_ADDS] = {NPC_LEFTY, NPC_ROTFANG, NPC_SNOKH, NPC_MALGEN, NPC_KORV, NPC_REZZNIK, NPC_VAJASHNI, NPC_VOLIDA};
static const uint32 aRingMob[] = {NPC_WORM, NPC_STINGER, NPC_SCREECHER, NPC_THUNDERSNOUT, NPC_CREEPER, NPC_BEETLE};
static const uint32 aRingBoss[] = {NPC_GOROSH, NPC_GRIZZLE, NPC_EVISCERATOR, NPC_OKTHOR, NPC_ANUBSHIAH, NPC_HEDRUM};

enum Phases
{
    PHASE_MOBS                      = 0,
    PHASE_BOSS                      = 2,
    PHASE_GLADIATORS                = 3,
};

bool AreaTrigger_at_ring_of_law(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (instance_blackrock_depths* pInstance = (instance_blackrock_depths*)pPlayer->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_RING_OF_LAW) == IN_PROGRESS || pInstance->GetData(TYPE_RING_OF_LAW) == DONE || pInstance->GetData(TYPE_RING_OF_LAW) == SPECIAL)
            return false;

        if (pPlayer->isGameMaster())
            return false;

        pInstance->SetData(TYPE_RING_OF_LAW, pInstance->GetData(TYPE_RING_OF_LAW) == DATA_BANNER_BEFORE_EVENT ? SPECIAL : IN_PROGRESS);

        pPlayer->SummonCreature(NPC_GRIMSTONE, aSpawnPositions[POS_GRIMSTONE][0], aSpawnPositions[POS_GRIMSTONE][1], aSpawnPositions[POS_GRIMSTONE][2], aSpawnPositions[POS_GRIMSTONE][3], TEMPSUMMON_DEAD_DESPAWN, 0);
        pInstance->SetArenaCenterCoords(pAt->x, pAt->y, pAt->z);

        return false;
    }
    return false;
}

/*######
## npc_grimstone
######*/

struct MANGOS_DLL_DECL npc_grimstoneAI : public npc_escortAI
{
    npc_grimstoneAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (instance_blackrock_depths*)pCreature->GetInstanceData();
        m_uiMobSpawnId = urand(0, 5);
        // Select MAX_THELDREN_ADDS(4) random adds for Theldren encounter
        uint8 uiCount = 0;
        for (uint8 i = 0; i < MAX_POSSIBLE_THELDREN_ADDS && uiCount < MAX_THELDREN_ADDS; ++i)
        {
            if (urand(0, 1) || i >= MAX_POSSIBLE_THELDREN_ADDS - MAX_THELDREN_ADDS + uiCount)
            {
                m_uiGladiatorId[uiCount] = aGladiator[i];
                ++uiCount;
            }
        }

        Reset();
    }

    instance_blackrock_depths* m_pInstance;

    uint8 m_uiEventPhase;
    uint32 m_uiEventTimer;

    uint8 m_uiMobSpawnId;
    uint8 m_uiMobDeadCount;

    Phases m_uiPhase;

    uint32 m_uiGladiatorId[MAX_THELDREN_ADDS];

    std::list<uint64> m_lSummonedGUIDList;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        m_uiEventTimer    = 1000;
        m_uiEventPhase    = 0;
        m_uiMobDeadCount  = 0;

        m_uiPhase = PHASE_MOBS;
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (!m_pInstance)
            return;

        // Ring mob or boss summoned
        float fX, fY, fZ;
        float fcX, fcY, fcZ;
        m_pInstance->GetArenaCenterCoords(fX, fY, fZ);
        m_creature->GetRandomPoint(fX, fY, fZ, 10.0f, fcX, fcY, fcZ);
        pSummoned->GetMotionMaster()->MovePoint(1, fcX, fcY, fcZ);

        m_lSummonedGUIDList.push_back(pSummoned->GetGUID());
    }

    void DoChallengeQuestCredit()
    {
        Map::PlayerList const &PlayerList = m_creature->GetMap()->GetPlayers();

        for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            Player* pPlayer = itr->getSource();
            if (pPlayer && pPlayer->GetQuestStatus(QUEST_THE_CHALLENGE) == QUEST_STATUS_INCOMPLETE)
                pPlayer->KilledMonsterCredit(NPC_THELDREN_QUEST_CREDIT);
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        ++m_uiMobDeadCount;

        switch (m_uiPhase)
        {
            case PHASE_MOBS:                                // Ring mob killed
                if (m_uiMobDeadCount == MAX_MOB_AMOUNT)
                {
                    m_uiEventTimer = 5000;
                    m_uiMobDeadCount = 0;
                }
                break;
            case PHASE_BOSS:                                // Ring boss killed
                // One Boss
                if (m_uiMobDeadCount == 1)
                {
                    m_uiEventTimer = 5000;
                    m_uiMobDeadCount = 0;
                }
                break;
            case PHASE_GLADIATORS:                          // Theldren and his band killed
                // Adds + Theldren
                if (m_uiMobDeadCount == MAX_THELDREN_ADDS + 1)
                {
                    m_uiEventTimer = 5000;
                    m_uiMobDeadCount = 0;
                    DoChallengeQuestCredit();
                }
                break;
        }
    }

    void SummonRingMob(uint32 uiEntry, SpawnPosition uiPosition)
    {
        float fX, fY, fZ;
        m_creature->GetRandomPoint(aSpawnPositions[uiPosition][0], aSpawnPositions[uiPosition][1], aSpawnPositions[uiPosition][2], 2.0f, fX, fY, fZ);
        m_creature->SummonCreature(uiEntry, fX, fY, fZ, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 0:                                         // Middle reached first time
                DoScriptText(urand(0, 1) ? SAY_START_1 : SAY_START_2, m_creature);
                SetEscortPaused(true);
                m_uiEventTimer = 5000;
                break;
            case 1:                                         // Reached wall again
                DoScriptText(SAY_OPEN_EAST_GATE, m_creature);
                SetEscortPaused(true);
                m_uiEventTimer = 5000;
                break;
            case 2:                                         // walking along the wall, while door opened
                SetEscortPaused(true);
                break;
            case 3:                                         // Middle reached second time
                DoScriptText(urand(0, 1) ? SAY_SUMMON_BOSS_1 : SAY_SUMMON_BOSS_2, m_creature);
                break;
            case 4:                                         // Reached North Gate
                DoScriptText(SAY_OPEN_NORTH_GATE, m_creature);
                SetEscortPaused(true);
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

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (m_pInstance->GetData(TYPE_RING_OF_LAW) == FAIL)
        {
            // Reset Doors
            if (m_uiEventPhase >= 9)                        // North Gate is opened
            {
                m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_2));
                m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_4));
            }
            else if (m_uiEventPhase >= 4)                   // East Gate is opened
            {
                m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_1));
                m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_4));
            }

            // Despawn Summoned Mobs
            for (std::list<uint64>::const_iterator itr = m_lSummonedGUIDList.begin(); itr != m_lSummonedGUIDList.end(); ++itr)
                if (Creature* pSummoned = m_creature->GetMap()->GetCreature(*itr))
                    pSummoned->ForcedDespawn();
            m_lSummonedGUIDList.clear();

            // Despawn NPC
            m_creature->ForcedDespawn();
            return;
        }

        if (m_uiEventTimer)
        {
            if (m_uiEventTimer <= uiDiff)
            {
                switch(m_uiEventPhase)
                {
                    case 0:
                        // Shortly after spawn, start walking
                        //DoScriptText(-1000000, m_creature); // no more text on spawn
                        m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_4));
                        Start(false);
                        SetEscortPaused(false);
                        m_uiEventTimer = 0;
                        break;
                    case 1:
                        // Start walking towards wall
                        SetEscortPaused(false);
                        m_uiEventTimer = 0;
                        break;
                    case 2:
                        m_uiEventTimer = 2000;
                        break;
                    case 3:
                        // Open East Gate
                        m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_1));
                        m_uiEventTimer = 3000;
                        break;
                    case 4:
                        SetEscortPaused(false);
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        // Summon Ring Mob(s)
                        SummonRingMob(aRingMob[m_uiMobSpawnId], POS_EAST);
                        m_uiEventTimer = 8000;
                        break;
                    case 5:
                        // Summon Ring Mob(s)
                        SummonRingMob(aRingMob[m_uiMobSpawnId], POS_EAST);
                        SummonRingMob(aRingMob[m_uiMobSpawnId], POS_EAST);
                        m_uiEventTimer = 8000;
                        break;
                    case 6:
                        // Summon Ring Mob(s)
                        SummonRingMob(aRingMob[m_uiMobSpawnId], POS_EAST);
                        m_uiEventTimer = 0;
                        break;
                    case 7:
                        // Summoned Mobs are dead, continue event
                        m_creature->SetVisibility(VISIBILITY_ON);
                        m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_1));
                        //DoScriptText(-1000000, m_creature); // after killed the mobs, no say here
                        SetEscortPaused(false);
                        m_uiEventTimer = 0;
                        break;
                    case 8:
                        // Open North Gate
                        m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_2));
                        m_uiEventTimer = 5000;
                        break;
                    case 9:
                        // Summon Boss
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        // If banner summoned after start, then summon Thelden after the creatures are dead
                        if (m_pInstance->GetData(TYPE_RING_OF_LAW) == SPECIAL && m_uiPhase == PHASE_MOBS)
                        {
                            m_uiPhase = PHASE_GLADIATORS;
                            SummonRingMob(NPC_THELDREN, POS_NORTH);
                            for (uint8 i = 0; i < MAX_THELDREN_ADDS; ++i)
                                SummonRingMob(m_uiGladiatorId[i], POS_NORTH);
                        }
                        else
                        {
                            m_uiPhase = PHASE_BOSS;
                            SummonRingMob(aRingBoss[urand(0, 5)], POS_NORTH);
                        }
                        m_uiEventTimer = 0;
                        break;
                    case 10:
                        // Boss dead
                        m_lSummonedGUIDList.clear();
                        m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_2));
                        m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_3));
                        m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(GO_ARENA_4));
                        SetEscortPaused(false);
                        m_uiEventTimer = 0;
                        break;
                }
                ++m_uiEventPhase;
            }
            else
                m_uiEventTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_grimstone(Creature* pCreature)
{
    return new npc_grimstoneAI(pCreature);
}

bool EffectDummyCreature_spell_banner_of_provocation(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    if (uiSpellId == SPELL_SUMMON_THELRIN_DND && uiEffIndex != EFFECT_INDEX_0)
    {
        instance_blackrock_depths* pInstance = (instance_blackrock_depths*)pCreatureTarget->GetInstanceData();
        if (pInstance && pInstance->GetData(TYPE_RING_OF_LAW) != DONE && pInstance->GetData(TYPE_RING_OF_LAW) != SPECIAL)
            pInstance->SetData(TYPE_RING_OF_LAW, pInstance->GetData(TYPE_RING_OF_LAW) == IN_PROGRESS ? SPECIAL : DATA_BANNER_BEFORE_EVENT);

        return true;
    }
    return false;
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
                m_creature->HandleEmote(EMOTE_ONESHOT_KICK);
                break;
            case 2:
                m_creature->HandleEmote(EMOTE_ONESHOT_ATTACKUNARMED);
                break;
            case 3:
                m_creature->HandleEmote(EMOTE_ONESHOT_ATTACKUNARMED);
                break;
            case 4:
                m_creature->HandleEmote(EMOTE_ONESHOT_KICK);
                break;
            case 5:
                m_creature->HandleEmote(EMOTE_ONESHOT_KICK);
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
                DoGo(GO_BAR_KEG_SHOT, 0);
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
                DoGo(GO_BAR_DOOR, 2);
                DoGo(GO_BAR_KEG_TRAP, 0);                   //doesn't work very well, leaving code here for future
                                                            //spell by trap has effect61, this indicate the bar go hostile

                if (Creature* pTmp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_PHALANX)))
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

bool QuestRewarded_npc_rocknot(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

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
                pEscortAI->Start(false, 0, NULL, true);
        }
    }

    return true;
}

void AddSC_blackrock_depths()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "go_shadowforge_brazier";
    pNewScript->pGOUse = &GOUse_go_shadowforge_brazier;
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
    pNewScript->Name = "npc_theldren_trigger";
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_spell_banner_of_provocation;
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
    pNewScript->pQuestRewardedNPC = &QuestRewarded_npc_rocknot;
    pNewScript->RegisterSelf();
}
