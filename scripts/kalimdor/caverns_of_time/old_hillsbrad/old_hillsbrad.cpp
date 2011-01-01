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
SDName: Old_Hillsbrad
SD%Complete: 60
SDComment: Quest support: 10283, 10284. All friendly NPC's. Thrall waypoints fairly complete, missing many details, but possible to complete escort.
SDCategory: Caverns of Time, Old Hillsbrad Foothills
EndScriptData */

/* ContentData
npc_brazen
npc_erozion
npc_thrall_old_hillsbrad
npc_taretha
EndContentData */

#include "precompiled.h"
#include "old_hillsbrad.h"
#include "escort_ai.h"

struct MANGOS_DLL_DECL npc_tarethaAI : public npc_escortAI
{
    npc_tarethaAI(Creature* pCreature);

    instance_old_hillsbrad* m_pInstance;
    uint64 m_uiErozionGUID;
    uint32 m_uiErozionEventTimer;
    uint32 m_uiErozionPhase;

    void Reset() {}
    void JustSummoned(Creature* pSummoned);
    void WaypointReached(uint32 uiPoint);
    void UpdateEscortAI(const uint32 uiDiff);
};

/*######
## npc_brazen
######*/

enum
{
    GOSSIP_ID_UNKNOWN_TEXT          = -1000000,
    GOSSIP_ITEM_READY               = -3560000,

    TEXT_ID_HAS_BOMBS               = 9780,
    ITEM_ENTRY_BOMBS                = 25853,

    TAXI_PATH_ID                    = 534
};

bool GossipHello_npc_brazen(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_READY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_brazen(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if (!pPlayer->HasItemCount(ITEM_ENTRY_BOMBS, 1))
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_HAS_BOMBS, pCreature->GetGUID());
        else
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID);
        }
    }
    return true;
}

/*######
## npc_erozion
######*/

enum
{
    GOSSIP_ITEM_NEED_BOMBS          = -3560001,
    TEXT_ID_DEFAULT                 = 9778,
    TEXT_ID_GOT_ITEM                = 9515,
};

bool GossipHello_npc_erozion(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (pInstance && pInstance->GetData(TYPE_BARREL_DIVERSION) != DONE && !pPlayer->HasItemCount(ITEM_ENTRY_BOMBS,1))
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NEED_BOMBS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    // Need info, should have option to teleport or not
    /*if (!pPlayer->GetQuestRewardStatus(QUEST_ENTRY_RETURN) && pPlayer->GetQuestStatus(QUEST_ENTRY_RETURN) == QUEST_STATUS_COMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[PH] Teleport please, i'm tired.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);*/

    pPlayer->SEND_GOSSIP_MENU(TEXT_ID_DEFAULT, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_erozion(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if (Item* pItem = pPlayer->StoreNewItemInInventorySlot(ITEM_ENTRY_BOMBS, 1))
            pPlayer->SendNewItem(pItem, 1, true, false);

        pPlayer->SEND_GOSSIP_MENU(TEXT_ID_GOT_ITEM, pCreature->GetGUID());
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
        pPlayer->CLOSE_GOSSIP_MENU();

    return true;
}

/*######
## npc_thrall_old_hillsbrad
######*/

enum
{
    // Thrall texts
    SAY_TH_START_EVENT_PART1        = -1560023,
    SAY_TH_ARMORY                   = -1560024,
    SAY_TH_SKARLOC_MEET             = -1560025,
    SAY_TH_SKARLOC_TAUNT            = -1560026,
    SAY_TH_START_EVENT_PART2        = -1560027,
    SAY_TH_MOUNTS_UP                = -1560028,
    SAY_TH_CHURCH_END               = -1560029,
    SAY_TH_MEET_TARETHA             = -1560030,

    SAY_EPOCH_ENTER1                = -1560013,
    SAY_EPOCH_ENTER2                = -1560014,
    SAY_EPOCH_ENTER3                = -1560015,

    SAY_TH_EPOCH_WONDER             = -1560031,
    SAY_TH_EPOCH_KILL_TARETHA       = -1560032,

    SAY_TH_RANDOM_LOW_HP1           = -1560034,
    SAY_TH_RANDOM_LOW_HP2           = -1560035,

    SAY_TH_RANDOM_DIE1              = -1560036,
    SAY_TH_RANDOM_DIE2              = -1560037,

    SAY_TH_RANDOM_AGGRO1            = -1560038,
    SAY_TH_RANDOM_AGGRO2            = -1560039,
    SAY_TH_RANDOM_AGGRO3            = -1560040,
    SAY_TH_RANDOM_AGGRO4            = -1560041,

    SAY_TH_RANDOM_KILL1             = -1560042,
    SAY_TH_RANDOM_KILL2             = -1560043,
    SAY_TH_RANDOM_KILL3             = -1560044,

    SAY_TH_KILL_ARMORER             = -1560050,

    SAY_TH_LEAVE_COMBAT1            = -1560045,
    SAY_TH_LEAVE_COMBAT2            = -1560046,
    SAY_TH_LEAVE_COMBAT3            = -1560047,

    // Taretha texts
    SAY_TA_ESCAPED                  = -1560049,

    // end event texts
    SAY_TA_FREE                     = -1560048,
    SAY_TR_GLAD_SAFE                = -1560054,
    SAY_TA_NEVER_MET                = -1560055,
    SAY_TR_THEN_WHO                 = -1560056,
    SAY_PRE_WIPE                    = -1560057,
    SAY_WIPE_MEMORY                 = -1560051,
    SAY_AFTER_WIPE                  = -1560058,
    SAY_ABOUT_TARETHA               = -1560052,
    SAY_TH_EVENT_COMPLETE           = -1560033,
    SAY_TA_FAREWELL                 = -1560053,

    // Misc for Thrall
    SPELL_STRIKE                    = 14516,
    SPELL_SHIELD_BLOCK              = 12169,
    SPELL_SUMMON_EROZION_IMAGE      = 33954,                // if thrall dies during escort?

    EQUIP_ID_WEAPON                 = 927,
    EQUIP_ID_SHIELD                 = 20913,
    MODEL_THRALL_UNEQUIPPED         = 17292,
    MODEL_THRALL_EQUIPPED           = 18165,

    // misc creature entries
    NPC_ARMORER                     = 18764,
    NPC_SCARLOC                     = 17862,

    NPC_RIFLE                       = 17820,
    NPC_WARDEN                      = 17833,
    NPC_VETERAN                     = 17860,
    NPC_WATCHMAN                    = 17814,
    NPC_SENTRY                      = 17815,

    NPC_BARN_GUARDSMAN              = 18092,
    NPC_BARN_PROTECTOR              = 18093,
    NPC_BARN_LOOKOUT                = 18094,

    NPC_CHURCH_GUARDSMAN            = 23175,
    NPC_CHURCH_PROTECTOR            = 23179,
    NPC_CHURCH_LOOKOUT              = 23177,

    NPC_INN_GUARDSMAN               = 23176,
    NPC_INN_PROTECTOR               = 23180,
    NPC_INN_LOOKOUT                 = 23178,

    NPC_SKARLOC_MOUNT               = 18798,
    MODEL_SKARLOC_MOUNT             = 18223,
    NPC_EROZION                     = 18723,
    NPC_THRALL_QUEST_TRIGGER        = 20156,

    // gossip
    TEXT_ID_START                   = 9568,
    TEXT_ID_SKARLOC1                = 9614,                 // I'm glad Taretha is alive. We now must find a way to free her...
    GOSSIP_ITEM_SKARLOC1            = -3560002,             // "Taretha cannot see you, Thrall."
    TEXT_ID_SKARLOC2                = 9579,                 // What do you mean by this? Is Taretha in danger?
    GOSSIP_ITEM_SKARLOC2            = -3560003,             // "The situation is rather complicated, Thrall. It would be best for you to head into the mountains now, before more of Blackmoore's men show up. We'll make sure Taretha is safe."
    TEXT_ID_SKARLOC3                = 9580,

    TEXT_ID_TARREN                  = 9597,                 // tarren mill is beyond these trees
    GOSSIP_ITEM_TARREN              = -3560004,             // "We're ready, Thrall."

    TEXT_ID_COMPLETE                = 9578,                 // Thank you friends, I owe my freedom to you. Where is Taretha? I hoped to see her
};

const float SPEED_WALK              = 0.5f;
const float SPEED_RUN               = 1.0f;
const float SPEED_MOUNT             = 1.6f;

struct MANGOS_DLL_DECL npc_thrall_old_hillsbradAI : public npc_escortAI
{
    npc_thrall_old_hillsbradAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (instance_old_hillsbrad*)pCreature->GetInstanceData();
        m_bHadMount = false;
        pCreature->SetActiveObjectState(true);              // required for proper relocation
        Reset();
    }

    instance_old_hillsbrad* m_pInstance;

    uint64 m_uiScarlocMountGUID;

    bool m_bIsLowHp;
    bool m_bHadMount;

    void Reset()
    {
        m_uiScarlocMountGUID = 0;

        m_bIsLowHp = false;

        if (m_bHadMount)
            DoMount();

        if (!HasEscortState(STATE_ESCORT_ESCORTING))
        {
            DoUnmount();
            m_bHadMount = false;
            SetEquipmentSlots(true);
            m_creature->SetDisplayId(MODEL_THRALL_UNEQUIPPED);
        }
    }

    void EnterEvadeMode()
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
        {
            switch(urand(0, 2))
            {
                case 0: DoScriptText(SAY_TH_LEAVE_COMBAT1, m_creature); break;
                case 1: DoScriptText(SAY_TH_LEAVE_COMBAT2, m_creature); break;
                case 2: DoScriptText(SAY_TH_LEAVE_COMBAT3, m_creature); break;
            }
        }

        npc_escortAI::EnterEvadeMode();
    }

    void WaypointReached(uint32 uiPoint)
    {
        if (!m_pInstance)
            return;

        switch(uiPoint)
        {
            case 8:
                SetRun(false);
                m_creature->SummonCreature(NPC_ARMORER, 2181.87f, 112.46f, 89.45f, 0.26f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                break;
            case 9:
                DoScriptText(SAY_TH_ARMORY, m_creature);
                SetEquipmentSlots(false, EQUIP_ID_WEAPON, EQUIP_ID_SHIELD, EQUIP_NO_CHANGE);
                break;
            case 10:
                m_creature->SetDisplayId(MODEL_THRALL_EQUIPPED);
                break;
            case 11:
                SetRun();
                break;
            case 15:
                m_creature->SummonCreature(NPC_RIFLE, 2200.28f, 137.37f, 87.93f, 5.07f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_WARDEN, 2197.44f, 131.83f, 87.93f, 0.78f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_VETERAN, 2203.62f, 135.40f, 87.93f, 3.70f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_VETERAN, 2200.75f, 130.13f, 87.93f, 1.48f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                break;
            case 21:
                m_creature->SummonCreature(NPC_RIFLE, 2135.80f, 154.01f, 67.45f, 4.98f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_WARDEN, 2144.36f, 151.87f, 67.74f, 4.46f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_VETERAN, 2142.12f, 154.41f, 67.12f, 4.56f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_VETERAN, 2138.08f, 155.38f, 67.24f, 4.60f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                break;
            case 25:
                m_creature->SummonCreature(NPC_RIFLE, 2102.98f, 192.17f, 65.24f, 6.02f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_WARDEN, 2108.48f, 198.75f, 65.18f, 5.15f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_VETERAN, 2106.11f, 197.29f, 65.18f, 5.63f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_VETERAN, 2104.18f, 194.82f, 65.18f, 5.75f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                break;
            case 29:
                DoScriptText(SAY_TH_SKARLOC_MEET, m_creature);
                m_creature->SummonCreature(NPC_SCARLOC, 2036.48f, 271.22f, 63.43f, 5.27f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                break;
            case 30:
                SetEscortPaused(true);
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                SetRun(false);
                break;
            case 31:
                DoScriptText(SAY_TH_MOUNTS_UP, m_creature);
                DoMount();
                SetRun();
                break;
            case 37:
                // possibly regular patrollers? If so, remove this and let database handle them
                m_creature->SummonCreature(NPC_WATCHMAN, 2124.26f, 522.16f, 56.87f, 3.99f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_WATCHMAN, 2121.69f, 525.37f, 57.11f, 4.01f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_SENTRY, 2124.65f, 524.55f, 56.63f, 3.98f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                break;
            case 59:
                m_creature->SummonCreature(NPC_SKARLOC_MOUNT, 2488.64f, 625.77f, 58.26f, 4.71f, TEMPSUMMON_TIMED_DESPAWN, 10000);
                DoUnmount();
                m_bHadMount = false;
                SetRun(false);
                break;
            case 60:
                m_creature->HandleEmote(EMOTE_ONESHOT_EXCLAMATION);
                // make horsie run off
                SetEscortPaused(true);
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                m_pInstance->SetData(TYPE_THRALL_PART2, DONE);
                SetRun();
                break;
            case 64:
                SetRun(false);
                break;
            case 68:
                m_creature->SummonCreature(NPC_BARN_PROTECTOR, 2500.22f, 692.60f, 55.50f, 2.84f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_BARN_LOOKOUT, 2500.13f, 696.55f, 55.51f, 3.38f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_BARN_GUARDSMAN, 2500.55f, 693.64f, 55.50f, 3.14f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_BARN_GUARDSMAN, 2500.94f, 695.81f, 55.50f, 3.14f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                break;
            case 71:
                SetRun();
                break;
            case 81:
                SetRun(false);
                break;
            case 83:
                m_creature->SummonCreature(NPC_CHURCH_PROTECTOR, 2627.33f, 646.82f, 56.03f, 4.28f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000);
                m_creature->SummonCreature(NPC_CHURCH_LOOKOUT, 2624.14f, 648.03f, 56.03f, 4.50f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000);
                m_creature->SummonCreature(NPC_CHURCH_GUARDSMAN, 2625.32f, 649.60f, 56.03f, 4.38f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000);
                m_creature->SummonCreature(NPC_CHURCH_GUARDSMAN, 2627.22f, 649.00f, 56.03f, 4.34f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000);
                break;
            case 84:
                DoScriptText(SAY_TH_CHURCH_END, m_creature);
                SetRun();
                break;
            case 91:
                SetRun(false);
                break;
            case 93:
                m_creature->SummonCreature(NPC_INN_PROTECTOR, 2652.71f, 660.31f, 61.93f, 1.67f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_INN_LOOKOUT, 2648.96f, 662.59f, 61.93f, 0.79f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_INN_GUARDSMAN, 2657.36f, 662.34f, 61.93f, 2.68f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                m_creature->SummonCreature(NPC_INN_GUARDSMAN, 2656.39f, 659.77f, 61.93f, 2.61f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                break;
            case 94:
                if (Creature* pTaretha = m_pInstance->GetTaretha())
                    DoScriptText(SAY_TA_ESCAPED, pTaretha, m_creature);

                break;
            case 95:
                DoScriptText(SAY_TH_MEET_TARETHA, m_creature);
                m_pInstance->SetData(TYPE_THRALL_PART3, DONE);
                SetEscortPaused(true);
                break;
            case 96:
                DoScriptText(SAY_TH_EPOCH_WONDER, m_creature);
                break;
            case 97:
                DoScriptText(SAY_TH_EPOCH_KILL_TARETHA, m_creature);
                SetRun();
                break;
            case 104:
                if (Creature* pEpoch = m_pInstance->GetEpoch())
                    DoScriptText(SAY_EPOCH_ENTER3, pEpoch);

                break;
            case 105:                                       // outside inn, meeting the dragon
                SetEscortPaused(true);

                if (Creature* pEpoch = m_pInstance->GetEpoch())
                    m_creature->SetFacingToObject(pEpoch);

                break;
            case 106:                                       // epoch is dead, proceeding with cheering
            {
                // trigger taretha to run down outside
                if (Creature* pTaretha = m_pInstance->GetTaretha())
                {
                    if (Player* pPlayer = GetPlayerForEscort())
                    {
                        if (npc_tarethaAI* pTarethaAI = dynamic_cast<npc_tarethaAI*>(pTaretha->AI()))
                            pTarethaAI->Start(true, pPlayer->GetGUID());
                    }
                }

                // kill credit creature for quest
                Map::PlayerList const& lPlayerList = m_pInstance->instance->GetPlayers();

                if (!lPlayerList.isEmpty())
                {
                    for(Map::PlayerList::const_iterator itr = lPlayerList.begin(); itr != lPlayerList.end(); ++itr)
                    {
                        if (Player* pPlayer = itr->getSource())
                            pPlayer->KilledMonsterCredit(NPC_THRALL_QUEST_TRIGGER, m_creature->GetGUID());
                    }
                }

                // a lot will happen here, thrall and taretha talk, erozion appear at spot to explain
                // handled by taretha script
                SetEscortPaused(true);
                break;
            }
            case 107:
                m_creature->SetActiveObjectState(false);
                break;
        }
    }

    void WaypointStart(uint32 uiPointId)
    {
        if (!m_pInstance)
            return;

        if (uiPointId == 97)
        {
            if (Creature* pEpoch = m_pInstance->GetEpoch())
                DoScriptText(SAY_EPOCH_ENTER2, pEpoch);
        }
    }

    void StartWP()
    {
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        SetEscortPaused(false);
    }

    void DoMount()
    {
        m_creature->Mount(MODEL_SKARLOC_MOUNT);
        m_creature->SetSpeedRate(MOVE_RUN, SPEED_MOUNT);
    }

    void DoUnmount()
    {
        m_creature->Unmount();
        m_creature->SetSpeedRate(MOVE_RUN, SPEED_RUN);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_TH_RANDOM_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_TH_RANDOM_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_TH_RANDOM_AGGRO3, m_creature); break;
            case 3: DoScriptText(SAY_TH_RANDOM_AGGRO4, m_creature); break;
        }

        if (m_creature->IsMounted())
        {
            DoUnmount();
            m_bHadMount = true;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            // TODO: make Scarloc start into event instead, and not start attack directly
            case NPC_BARN_GUARDSMAN:
            case NPC_BARN_PROTECTOR:
            case NPC_BARN_LOOKOUT:
                break;
            case NPC_SKARLOC_MOUNT:
                m_uiScarlocMountGUID = pSummoned->GetGUID();
                break;
            default:
                pSummoned->AI()->AttackStart(m_creature);
                break;
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_TH_RANDOM_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_TH_RANDOM_KILL2, m_creature); break;
            case 2: DoScriptText(SAY_TH_RANDOM_KILL3, m_creature); break;
        }

        // Death called from instance script (or if he has the killing blow of course)
        // Thrall should normally always be the one killing, but no support for this yet.
        if (pVictim->GetEntry() == NPC_EPOCH)
            SetEscortPaused(false);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(urand(0, 1) ? SAY_TH_RANDOM_DIE1 : SAY_TH_RANDOM_DIE2, m_creature);
    }

    void CorpseRemoved(uint32 &uiRespawnDelay)
    {
        uiRespawnDelay = 0;

        // if we're done, just set some high so he never really respawn
        if (m_pInstance && m_pInstance->GetData(TYPE_THRALL_EVENT) == DONE)
            uiRespawnDelay = 4 * HOUR;
    }

    void JustRespawned()
    {
        if (!m_pInstance)
            return;

        Reset();

        if (m_pInstance->GetData(TYPE_THRALL_EVENT) == IN_PROGRESS)
        {
            SetEscortPaused(true);

            m_bHadMount = false;
            DoUnmount();

            m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

            // check current states before fail and set spesific for the part
            if (m_pInstance->GetData(TYPE_THRALL_PART1) == IN_PROGRESS)
            {
                SetCurrentWaypoint(1);                      // basement

                SetEquipmentSlots(true);
                m_creature->SetDisplayId(MODEL_THRALL_UNEQUIPPED);

                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
            else if (m_pInstance->GetData(TYPE_THRALL_PART2) == IN_PROGRESS)
            {
                SetCurrentWaypoint(61);                     // barn
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
            else if (m_pInstance->GetData(TYPE_THRALL_PART3) == IN_PROGRESS || m_pInstance->GetData(TYPE_THRALL_PART4) == IN_PROGRESS)
                SetCurrentWaypoint(96);                     // inn

            // fail, and relocation handled in instance script
            m_pInstance->SetData(TYPE_THRALL_EVENT, FAIL);
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // TODO: add his abilities'n-crap here

        if (!m_bIsLowHp && m_creature->GetHealthPercent() < 20.0f)
        {
            DoScriptText(urand(0, 1) ? SAY_TH_RANDOM_LOW_HP1 : SAY_TH_RANDOM_LOW_HP2, m_creature);
            m_bIsLowHp = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_thrall_old_hillsbrad(Creature* pCreature)
{
    return new npc_thrall_old_hillsbradAI(pCreature);
}

bool GossipHello_npc_thrall_old_hillsbrad(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
    {
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());
        pPlayer->SendPreparedQuest(pCreature->GetGUID());
    }

    if (instance_old_hillsbrad* pInstance = (instance_old_hillsbrad*)pCreature->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_BARREL_DIVERSION) == DONE && !pInstance->GetData(TYPE_THRALL_EVENT))
        {
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ID_UNKNOWN_TEXT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_START, pCreature->GetGUID());
        }

        if (pInstance->GetData(TYPE_THRALL_PART1) == DONE && !pInstance->GetData(TYPE_THRALL_PART2))
        {
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_SKARLOC1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_SKARLOC1, pCreature->GetGUID());
        }

        if (pInstance->GetData(TYPE_THRALL_PART2) == DONE && !pInstance->GetData(TYPE_THRALL_PART3))
        {
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TARREN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_TARREN, pCreature->GetGUID());
        }
    }
    return true;
}

bool GossipSelect_npc_thrall_old_hillsbrad(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    instance_old_hillsbrad* pInstance = (instance_old_hillsbrad*)pCreature->GetInstanceData();

    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
        {
            pPlayer->CLOSE_GOSSIP_MENU();

            pInstance->SetData(TYPE_THRALL_EVENT, IN_PROGRESS);
            pInstance->SetData(TYPE_THRALL_PART1, IN_PROGRESS);

            DoScriptText(SAY_TH_START_EVENT_PART1, pCreature);

            if (npc_thrall_old_hillsbradAI* pThrallAI = dynamic_cast<npc_thrall_old_hillsbradAI*>(pCreature->AI()))
                pThrallAI->Start(true, pPlayer->GetGUID());

            break;
        }
        case GOSSIP_ACTION_INFO_DEF+2:
        {
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_SKARLOC2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+20);
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_SKARLOC2, pCreature->GetGUID());
            break;
        }
        case GOSSIP_ACTION_INFO_DEF+20:
        {
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_SKARLOC3, pCreature->GetGUID());

            pCreature->SummonCreature(NPC_SKARLOC_MOUNT, 2038.81f, 270.26f, 63.20f, 5.41f, TEMPSUMMON_TIMED_DESPAWN,12000);
            pInstance->SetData(TYPE_THRALL_PART2, IN_PROGRESS);

            DoScriptText(SAY_TH_START_EVENT_PART2, pCreature);

            if (npc_thrall_old_hillsbradAI* pThrallAI = dynamic_cast<npc_thrall_old_hillsbradAI*>(pCreature->AI()))
                pThrallAI->StartWP();

            break;
        }
        case GOSSIP_ACTION_INFO_DEF+3:
        {
            pPlayer->CLOSE_GOSSIP_MENU();

            pInstance->SetData(TYPE_THRALL_PART3, IN_PROGRESS);

            if (npc_thrall_old_hillsbradAI* pThrallAI = dynamic_cast<npc_thrall_old_hillsbradAI*>(pCreature->AI()))
                pThrallAI->StartWP();

            break;
        }
    }
    return true;
}

/*######
## npc_taretha
######*/

enum
{
    TEXT_ID_EPOCH1          = 9610,                         // Thank you for helping Thrall escape, friends. Now I only hope
    GOSSIP_ITEM_EPOCH1      = -3560005,                     // "Strange wizard?"
    TEXT_ID_EPOCH2          = 9613,                         // Yes, friends. This man was no wizard of
    GOSSIP_ITEM_EPOCH2      = -3560006,                     // "We'll get you out. Taretha. Don't worry. I doubt the wizard would wander too far away."
};

npc_tarethaAI::npc_tarethaAI(Creature* pCreature) : npc_escortAI(pCreature)
{
    m_pInstance = (instance_old_hillsbrad*)pCreature->GetInstanceData();
    m_uiErozionGUID = 0;
    m_uiErozionEventTimer = 5000;
    m_uiErozionPhase = 0;
    Reset();
}

void npc_tarethaAI::JustSummoned(Creature* pSummoned)
{
    if (pSummoned->GetEntry() == NPC_EROZION)
        m_uiErozionGUID = pSummoned->GetGUID();
    else
        DoScriptText(SAY_EPOCH_ENTER1, pSummoned);
}

void npc_tarethaAI::WaypointReached(uint32 uiPoint)
{
    switch(uiPoint)
    {
        case 6:
            DoScriptText(SAY_TA_FREE, m_creature);
            break;
        case 7:
            m_creature->HandleEmote(EMOTE_ONESHOT_CHEER);
            m_creature->SummonCreature(NPC_EROZION, 2646.47f, 680.416f, 55.38f, 4.16f, TEMPSUMMON_TIMED_DESPAWN, 120000);
            SetEscortPaused(true);
            SetRun(false);
            break;
    }
}

void npc_tarethaAI::UpdateEscortAI(const uint32 uiDiff)
{
    if (!HasEscortState(STATE_ESCORT_PAUSED))
        return;

    if (m_uiErozionEventTimer < uiDiff)
    {
        ++m_uiErozionPhase;
        m_uiErozionEventTimer = 5000;

        switch(m_uiErozionPhase)
        {
            case 1:
                if (Creature* pThrall = m_pInstance->GetThrall())
                {
                    pThrall->SetFacingToObject(m_creature);
                    DoScriptText(SAY_TR_GLAD_SAFE, pThrall);
                }
                break;
            case 2:
                DoScriptText(SAY_TA_NEVER_MET, m_creature);
                break;
            case 3:
                if (Creature* pThrall = m_pInstance->GetThrall())
                    DoScriptText(SAY_TR_THEN_WHO, pThrall);
                break;
            case 4:
                if (Creature* pErozion = m_creature->GetMap()->GetCreature(m_uiErozionGUID))
                    DoScriptText(SAY_PRE_WIPE, pErozion);
                break;
            case 5:
                //if (Creature* pErozion = m_creature->GetMap()->GetCreature(m_uiErozionGUID))
                    //pErozion->AI()->DoCastSpellIfCan();
                break;
            case 6:
                if (Creature* pErozion = m_creature->GetMap()->GetCreature(m_uiErozionGUID))
                    DoScriptText(SAY_WIPE_MEMORY, pErozion);
                break;
            case 7:
                if (Creature* pErozion = m_creature->GetMap()->GetCreature(m_uiErozionGUID))
                    DoScriptText(SAY_ABOUT_TARETHA, pErozion);
                break;
            case 8:
                if (Creature* pErozion = m_creature->GetMap()->GetCreature(m_uiErozionGUID))
                    DoScriptText(SAY_AFTER_WIPE, pErozion);
                break;
            case 9:
                if (Creature* pThrall = m_pInstance->GetThrall())
                    DoScriptText(SAY_TH_EVENT_COMPLETE, pThrall);
                break;
            case 10:
                DoScriptText(SAY_TA_FAREWELL, m_creature);
                SetEscortPaused(false);

                if (Creature* pThrall = m_pInstance->GetThrall())
                {
                    if (npc_thrall_old_hillsbradAI* pThrallAI = dynamic_cast<npc_thrall_old_hillsbradAI*>(pThrall->AI()))
                        pThrallAI->SetEscortPaused(false);
                }

                break;
        }
    }
    else
        m_uiErozionEventTimer -= uiDiff;
}

CreatureAI* GetAI_npc_taretha(Creature* pCreature)
{
    return new npc_tarethaAI(pCreature);
}

bool GossipHello_npc_taretha(Player* pPlayer, Creature* pCreature)
{
    instance_old_hillsbrad* pInstance = (instance_old_hillsbrad*)pCreature->GetInstanceData();

    if (pInstance && pInstance->GetData(TYPE_THRALL_PART3) == DONE && pInstance->GetData(TYPE_THRALL_PART4) == NOT_STARTED)
    {
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_EPOCH1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(TEXT_ID_EPOCH1, pCreature->GetGUID());
    }

    return true;
}

bool GossipSelect_npc_taretha(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    instance_old_hillsbrad* pInstance = (instance_old_hillsbrad*)pCreature->GetInstanceData();

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_EPOCH2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        pPlayer->SEND_GOSSIP_MENU(TEXT_ID_EPOCH2, pCreature->GetGUID());
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();

        if (pInstance && pInstance->GetData(TYPE_THRALL_EVENT) == IN_PROGRESS)
        {
            pInstance->SetData(TYPE_THRALL_PART4, IN_PROGRESS);
            pCreature->SummonCreature(NPC_EPOCH, 2639.13f, 698.55f, 65.43f, 4.59f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000);

            if (Creature* pThrall = pInstance->GetThrall())
            {
                if (npc_thrall_old_hillsbradAI* pThrallAI = dynamic_cast<npc_thrall_old_hillsbradAI*>(pThrall->AI()))
                    pThrallAI->StartWP();
            }
        }
    }

    return true;
}

/*######
## AddSC
######*/

void AddSC_old_hillsbrad()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_brazen";
    pNewScript->pGossipHello = &GossipHello_npc_brazen;
    pNewScript->pGossipSelect = &GossipSelect_npc_brazen;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_erozion";
    pNewScript->pGossipHello = &GossipHello_npc_erozion;
    pNewScript->pGossipSelect = &GossipSelect_npc_erozion;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_thrall_old_hillsbrad";
    pNewScript->pGossipHello = &GossipHello_npc_thrall_old_hillsbrad;
    pNewScript->pGossipSelect = &GossipSelect_npc_thrall_old_hillsbrad;
    pNewScript->GetAI = &GetAI_npc_thrall_old_hillsbrad;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_taretha";
    pNewScript->pGossipHello = &GossipHello_npc_taretha;
    pNewScript->pGossipSelect = &GossipSelect_npc_taretha;
    pNewScript->GetAI = &GetAI_npc_taretha;
    pNewScript->RegisterSelf();
}
