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
SDName: Netherstorm
SD%Complete: 80
SDComment: Quest support: 10438, 10652 (special flight paths), 10299, 10321, 10322, 10323, 10329, 10330, 10337, 10338, 10365(Shutting Down Manaforge), 10198, 10191, 10924
SDCategory: Netherstorm
EndScriptData */

/* ContentData
npc_manaforge_control_console
go_manaforge_control_console
npc_commander_dawnforge
npc_protectorate_nether_drake
npc_veronia
npc_bessy
npc_maxx_a_million
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"
#include "pet_ai.h"

/*######
## npc_manaforge_control_console
######*/
enum
{
    EMOTE_START                  = -1000211,
    EMOTE_60                     = -1000212,
    EMOTE_30                     = -1000213,
    EMOTE_10                     = -1000214,
    EMOTE_COMPLETE               = -1000215,
    EMOTE_ABORT                  = -1000216,

    NPC_BNAAR_C_CONSOLE          = 20209,
    NPC_CORUU_C_CONSOLE          = 20417,
    NPC_DURO_C_CONSOLE           = 20418,
    NPC_ARA_C_CONSOLE            = 20440,

    NPC_SUNFURY_TECH             = 20218,
    NPC_SUNFURY_PROT             = 20436,

    NPC_ARA_TECH                 = 20438,
    NPC_ARA_ENGI                 = 20439,
    NPC_ARA_GORKLONN             = 20460,

    QUEST_SHUTDOWN_BNAAR_ALDOR   = 10299,
    QUEST_SHUTDOWN_BNAAR_SCRYERS = 10329,
    QUEST_SHUTDOWN_CORUU_ALDOR   = 10321,
    QUEST_SHUTDOWN_CORUU_SCRYERS = 10330,
    QUEST_SHUTDOWN_DURO_ALDOR    = 10322,
    QUEST_SHUTDOWN_DURO_SCRYERS  = 10338,
    QUEST_SHUTDOWN_ARA_ALDOR     = 10323,
    QUEST_SHUTDOWN_ARA_SCRYERS   = 10365,

    ITEM_BNAAR_ACESS_CRYSTAL     = 29366,
    ITEM_CORUU_ACESS_CRYSTAL     = 29396,
    ITEM_DURO_ACESS_CRYSTAL      = 29397,
    ITEM_ARA_ACESS_CRYSTAL       = 29411,

    SPELL_DISABLE_VISUAL         = 35031,
    SPELL_INTERRUPT_1            = 35016,                   // ACID mobs should cast this
    SPELL_INTERRUPT_2            = 35176,                   // ACID mobs should cast this (Manaforge Ara-version)
};

struct MANGOS_DLL_DECL npc_manaforge_control_consoleAI : public ScriptedAI
{
    npc_manaforge_control_consoleAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 m_uiPlayerGUID;
    uint64 m_uiConsoleGUID;
    uint32 m_uiEventTimer;
    uint32 m_uiWaveTimer;
    uint32 m_uiPhase;
    bool   m_bWave;

    void Reset()
    {
        m_uiPlayerGUID = 0;
        m_uiConsoleGUID = 0;
        m_uiEventTimer = 3000;
        m_uiWaveTimer = 0;
        m_uiPhase = 1;
        m_bWave = false;
    }

    /*void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        //we have no way of telling the creature was hit by spell -> got aura applied after 10-12 seconds
        //then no way for the mobs to actually stop the shutdown as intended.
        if (spell->Id == SPELL_INTERRUPT_1)
            ...
    }*/

    void JustDied(Unit* pKiller)
    {
        DoScriptText(EMOTE_ABORT, m_creature);

        if (m_uiPlayerGUID)
        {
            Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID);

            if (pPlayer)
            {
                switch(m_creature->GetEntry())
                {
                    case NPC_BNAAR_C_CONSOLE:
                        pPlayer->FailQuest(QUEST_SHUTDOWN_BNAAR_ALDOR);
                        pPlayer->FailQuest(QUEST_SHUTDOWN_BNAAR_SCRYERS);
                        break;
                    case NPC_CORUU_C_CONSOLE:
                        pPlayer->FailQuest(QUEST_SHUTDOWN_CORUU_ALDOR);
                        pPlayer->FailQuest(QUEST_SHUTDOWN_CORUU_SCRYERS);
                        break;
                    case NPC_DURO_C_CONSOLE:
                        pPlayer->FailQuest(QUEST_SHUTDOWN_DURO_ALDOR);
                        pPlayer->FailQuest(QUEST_SHUTDOWN_DURO_SCRYERS);
                        break;
                    case NPC_ARA_C_CONSOLE:
                        pPlayer->FailQuest(QUEST_SHUTDOWN_ARA_ALDOR);
                        pPlayer->FailQuest(QUEST_SHUTDOWN_ARA_SCRYERS);
                        break;
                }
            }
        }

        if (m_uiConsoleGUID)
        {
            if (GameObject* pGo = m_creature->GetMap()->GetGameObject(m_uiConsoleGUID))
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
        }
    }

    void DoWaveSpawnForCreature(Creature* pCreature)
    {
        Creature* pAdd = NULL;

        switch(pCreature->GetEntry())
        {
            case NPC_BNAAR_C_CONSOLE:
                if (urand(0, 1))
                {
                    if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2933.68f, 4162.55f, 164.00f, 1.60f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        pAdd->GetMotionMaster()->MovePoint(0, 2927.36f, 4212.97f, 164.00f);
                }
                else
                {
                    if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2927.36f, 4212.97f, 164.00f, 4.94f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        pAdd->GetMotionMaster()->MovePoint(0, 2933.68f, 4162.55f, 164.00f);
                }
                m_uiWaveTimer = 30000;
                break;
            case NPC_CORUU_C_CONSOLE:
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2445.21f, 2765.26f, 134.49f, 3.93f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2424.21f, 2740.15f, 133.81f);
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2429.86f, 2731.85f, 134.53f, 1.31f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2435.37f, 2766.04f, 133.81f);
                m_uiWaveTimer = 20000;
                break;
            case NPC_DURO_C_CONSOLE:
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2986.80f, 2205.36f, 165.37f, 3.74f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2985.15f, 2197.32f, 164.79f);
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2952.91f, 2191.20f, 165.32f, 0.22f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2060.01f, 2185.27f, 164.67f);
                m_uiWaveTimer = 15000;
                break;
            case NPC_ARA_C_CONSOLE:
                if (urand(0, 1))
                {
                    if (pAdd = m_creature->SummonCreature(NPC_ARA_TECH, 4035.11f, 4038.97f, 194.27f, 2.57f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        pAdd->GetMotionMaster()->MovePoint(0, 4003.42f, 4040.19f, 193.49f);
                    if (pAdd = m_creature->SummonCreature(NPC_ARA_TECH, 4033.66f, 4036.79f, 194.28f, 2.57f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        pAdd->GetMotionMaster()->MovePoint(0, 4003.42f, 4040.19f, 193.49f);
                    if (pAdd = m_creature->SummonCreature(NPC_ARA_TECH, 4037.13f, 4037.30f, 194.23f, 2.57f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        pAdd->GetMotionMaster()->MovePoint(0, 4003.42f, 4040.19f, 193.49f);
                }
                else
                {
                    if (pAdd = m_creature->SummonCreature(NPC_ARA_TECH, 3099.59f, 4049.30f, 194.22f, 0.05f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        pAdd->GetMotionMaster()->MovePoint(0, 4028.01f, 4035.17f, 193.59f);
                    if (pAdd = m_creature->SummonCreature(NPC_ARA_TECH, 3999.72f, 4046.75f, 194.22f, 0.05f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        pAdd->GetMotionMaster()->MovePoint(0, 4028.01f, 4035.17f, 193.59f);
                    if (pAdd = m_creature->SummonCreature(NPC_ARA_TECH, 3996.81f, 4048.26f, 194.22f, 0.05f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        pAdd->GetMotionMaster()->MovePoint(0, 4028.01f, 4035.17f, 193.59f);
                }
                m_uiWaveTimer = 15000;
                break;
        }
    }

    void DoFinalSpawnForCreature(Creature* pCreature)
    {
        Creature* pAdd = NULL;

        switch(pCreature->GetEntry())
        {
            case NPC_BNAAR_C_CONSOLE:
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2946.52f, 4201.42f, 163.47f, 3.54f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2927.49f, 4192.81f, 163.00f);
                break;
            case NPC_CORUU_C_CONSOLE:
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2453.88f, 2737.85f, 133.27f, 2.59f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2433.96f, 2751.53f, 133.85f);
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2441.62f, 2735.32f, 134.49f, 1.97f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2433.96f, 2751.53f, 133.85f);
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2450.73f, 2754.50f, 134.49f, 3.29f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2433.96f, 2751.53f, 133.85f);
                break;
            case NPC_DURO_C_CONSOLE:
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2956.18f, 2202.85f, 165.32f, 5.45f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2972.27f, 2193.22f, 164.48f);
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_TECH, 2975.30f, 2211.50f, 165.32f, 4.55f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2972.27f, 2193.22f, 164.48f);
                if (pAdd = m_creature->SummonCreature(NPC_SUNFURY_PROT, 2965.02f, 2217.45f, 164.16f, 4.96f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 2972.27f, 2193.22f, 164.48f);
                break;
            case NPC_ARA_C_CONSOLE:
                if (pAdd = m_creature->SummonCreature(NPC_ARA_ENGI, 3994.51f, 4020.46f, 192.18f, 0.91f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 4008.35f, 4035.04f, 192.70f);
                if (pAdd = m_creature->SummonCreature(NPC_ARA_GORKLONN, 4021.56f, 4059.35f, 193.59f, 4.44f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                    pAdd->GetMotionMaster()->MovePoint(0, 4016.62f, 4039.89f, 193.46f);
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiEventTimer < uiDiff)
        {
            if (!m_uiPlayerGUID)
                return;

            Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID);

            if (!pPlayer)
                return;

            switch(m_uiPhase)
            {
                case 1:
                    DoScriptText(EMOTE_START, m_creature, pPlayer);
                    m_uiEventTimer = 60000;
                    m_bWave = true;
                    ++m_uiPhase;
                    break;
                case 2:
                    DoScriptText(EMOTE_60, m_creature, pPlayer);
                    m_uiEventTimer = 30000;
                    ++m_uiPhase;
                    break;
                case 3:
                    DoScriptText(EMOTE_30, m_creature, pPlayer);
                    m_uiEventTimer = 20000;
                    DoFinalSpawnForCreature(m_creature);
                    ++m_uiPhase;
                    break;
                case 4:
                    DoScriptText(EMOTE_10, m_creature, pPlayer);
                    m_uiEventTimer = 10000;
                    m_bWave = false;
                    ++m_uiPhase;
                    break;
                case 5:
                    DoScriptText(EMOTE_COMPLETE, m_creature, pPlayer);
                    pPlayer->KilledMonsterCredit(m_creature->GetEntry(), m_creature->GetGUID());
                    DoCastSpellIfCan(m_creature, SPELL_DISABLE_VISUAL);
                    if (m_uiConsoleGUID)
                    {
                        if (GameObject* pGo = m_creature->GetMap()->GetGameObject(m_uiConsoleGUID))
                            pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
                    }
                    ++m_uiPhase;
                    break;
            }
        }
        else
            m_uiEventTimer -= uiDiff;

        if (m_bWave)
        {
            if (m_uiWaveTimer < uiDiff)
            {
                DoWaveSpawnForCreature(m_creature);
            }
            else
                m_uiWaveTimer -= uiDiff;
        }
    }
};
CreatureAI* GetAI_npc_manaforge_control_console(Creature* pCreature)
{
    return new npc_manaforge_control_consoleAI(pCreature);
}

/*######
## go_manaforge_control_console
######*/

// TODO: clean up this workaround when mangos adds support to do it properly (with gossip selections instead of instant summon)
bool GOUse_go_manaforge_control_console(Player* pPlayer, GameObject* pGo)
{
    if (pGo->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER)
    {
        pPlayer->PrepareQuestMenu(pGo->GetGUID());
        pPlayer->SendPreparedQuest(pGo->GetGUID());
    }

    Creature* pManaforge = NULL;

    switch(pGo->GetAreaId())
    {
        case 3726:                                          // b'naar
            if ((pPlayer->GetQuestStatus(QUEST_SHUTDOWN_BNAAR_ALDOR) == QUEST_STATUS_INCOMPLETE
                || pPlayer->GetQuestStatus(QUEST_SHUTDOWN_BNAAR_SCRYERS) == QUEST_STATUS_INCOMPLETE)
                && pPlayer->HasItemCount(ITEM_BNAAR_ACESS_CRYSTAL, 1))
                pManaforge = pPlayer->SummonCreature(NPC_BNAAR_C_CONSOLE, 2918.95f, 4189.98f, 161.88f, 0.34f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 125000);
            break;
        case 3730:                                          // coruu
            if ((pPlayer->GetQuestStatus(QUEST_SHUTDOWN_CORUU_ALDOR) == QUEST_STATUS_INCOMPLETE
                || pPlayer->GetQuestStatus(QUEST_SHUTDOWN_CORUU_SCRYERS) == QUEST_STATUS_INCOMPLETE)
                && pPlayer->HasItemCount(ITEM_CORUU_ACESS_CRYSTAL, 1))
                pManaforge = pPlayer->SummonCreature(NPC_CORUU_C_CONSOLE, 2426.77f, 2750.38f, 133.24f, 2.14f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 125000);
            break;
        case 3734:                                          // duro
            if ((pPlayer->GetQuestStatus(QUEST_SHUTDOWN_DURO_ALDOR) == QUEST_STATUS_INCOMPLETE
                || pPlayer->GetQuestStatus(QUEST_SHUTDOWN_DURO_SCRYERS) == QUEST_STATUS_INCOMPLETE)
                && pPlayer->HasItemCount(ITEM_DURO_ACESS_CRYSTAL, 1))
                pManaforge = pPlayer->SummonCreature(NPC_DURO_C_CONSOLE, 2976.48f, 2183.29f, 163.20f, 1.85f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 125000);
            break;
        case 3722:                                          // ara
            if ((pPlayer->GetQuestStatus(QUEST_SHUTDOWN_ARA_ALDOR) == QUEST_STATUS_INCOMPLETE
                || pPlayer->GetQuestStatus(QUEST_SHUTDOWN_ARA_SCRYERS) == QUEST_STATUS_INCOMPLETE)
                && pPlayer->HasItemCount(ITEM_ARA_ACESS_CRYSTAL, 1))
                pManaforge = pPlayer->SummonCreature(NPC_ARA_C_CONSOLE, 4013.71f, 4028.76f, 192.10f, 1.25f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 125000);
            break;
    }

    if (pManaforge)
    {
        if (npc_manaforge_control_consoleAI* pManaforgeAI = dynamic_cast<npc_manaforge_control_consoleAI*>(pManaforge->AI()))
        {
            pManaforgeAI->m_uiPlayerGUID = pPlayer->GetGUID();
            pManaforgeAI->m_uiConsoleGUID = pGo->GetGUID();
        }

        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
    }
    return true;
}

/*######
## npc_commander_dawnforge
######*/

// The Speech of Dawnforge, Ardonis & Pathaleon
enum
{
    SAY_COMMANDER_DAWNFORGE_1              = -1000128,
    SAY_ARCANIST_ARDONIS_1                 = -1000129,
    SAY_COMMANDER_DAWNFORGE_2              = -1000130,
    SAY_PATHALEON_THE_CALCULATOR_IMAGE_1   = -1000131,
    SAY_COMMANDER_DAWNFORGE_3              = -1000132,
    SAY_PATHALEON_THE_CALCULATOR_IMAGE_2   = -1000133,
    SAY_PATHALEON_THE_CALCULATOR_IMAGE_2_1 = -1000134,
    SAY_PATHALEON_THE_CALCULATOR_IMAGE_2_2 = -1000135,
    SAY_COMMANDER_DAWNFORGE_4              = -1000136,
    SAY_ARCANIST_ARDONIS_2                 = -1000136,
    SAY_COMMANDER_DAWNFORGE_5              = -1000137,

    QUEST_INFO_GATHERING                   = 10198,
    SPELL_SUNFURY_DISGUISE                 = 34603,

    NPC_ARCANIST_ARDONIS                   = 19830,
    NPC_COMMANDER_DAWNFORGE                = 19831,
    NPC_PATHALEON_THE_CALCULATOR_IMAGE     = 21504
};

struct MANGOS_DLL_DECL npc_commander_dawnforgeAI : public ScriptedAI
{
    npc_commander_dawnforgeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset (); }

    uint64 m_uiPlayerGUID;
    uint64 m_uiArdonisGUID;
    uint64 m_uiPathaleonGUID;

    uint32 m_uiPhase;
    uint32 m_uiPhaseSubphase;
    uint32 m_uiPhaseTimer;
    bool   m_bIsEvent;

    void Reset()
    {
        m_uiPlayerGUID = 0;
        m_uiArdonisGUID = 0;
        m_uiPathaleonGUID = 0;

        m_uiPhase = 1;
        m_uiPhaseSubphase = 0;
        m_uiPhaseTimer = 4000;
        m_bIsEvent = false;
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_PATHALEON_THE_CALCULATOR_IMAGE)
            m_uiPathaleonGUID = pSummoned->GetGUID();
    }

    void TurnToPathaleonsImage()
    {
        Creature* pArdonis = m_creature->GetMap()->GetCreature(m_uiArdonisGUID);
        Creature* pPathaleon = m_creature->GetMap()->GetCreature(m_uiPathaleonGUID);
        Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID);

        if (!pArdonis || !pPathaleon || !pPlayer)
            return;

        m_creature->SetFacingToObject(pPathaleon);
        pArdonis->SetFacingToObject(pPathaleon);

        // the boss is there kneel before him
        m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
        pArdonis->SetStandState(UNIT_STAND_STATE_KNEEL);
    }

    void TurnToEachOther()
    {
        if (Creature* pArdonis = m_creature->GetMap()->GetCreature(m_uiArdonisGUID))
        {
            Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID);

            if (!pPlayer)
                return;

            m_creature->SetFacingToObject(pArdonis);
            pArdonis->SetFacingToObject(m_creature);

            // get up
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
            pArdonis->SetStandState(UNIT_STAND_STATE_STAND);
        }
    }

    bool CanStartEvent(Player* pPlayer)
    {
        if (!m_bIsEvent)
        {
            Creature* pArdonis = GetClosestCreatureWithEntry(m_creature, NPC_ARCANIST_ARDONIS, 10.0f);

            if (!pArdonis)
                return false;

            m_uiArdonisGUID = pArdonis->GetGUID();
            m_uiPlayerGUID = pPlayer->GetGUID();

            m_bIsEvent = true;

            TurnToEachOther();
            return true;
        }

        debug_log("SD2: npc_commander_dawnforge event already in progress, need to wait.");
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Is event even running?
        if (!m_bIsEvent)
            return;

        // Phase timing
        if (m_uiPhaseTimer >= uiDiff)
        {
            m_uiPhaseTimer -= uiDiff;
            return;
        }

        Creature* pArdonis = m_creature->GetMap()->GetCreature(m_uiArdonisGUID);
        Creature* pPathaleon = m_creature->GetMap()->GetCreature(m_uiPathaleonGUID);
        Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID);

        if (!pArdonis || !pPlayer)
        {
            Reset();
            return;
        }

        if (m_uiPhase > 4 && !pPathaleon)
        {
            Reset();
            return;
        }

        switch (m_uiPhase)
        {
            case 1:
                DoScriptText(SAY_COMMANDER_DAWNFORGE_1, m_creature);
                ++m_uiPhase;
                m_uiPhaseTimer = 16000;
                break;
            case 2:
                DoScriptText(SAY_ARCANIST_ARDONIS_1, pArdonis);
                ++m_uiPhase;
                m_uiPhaseTimer = 16000;
                break;
            case 3:
                DoScriptText(SAY_COMMANDER_DAWNFORGE_2, m_creature);
                ++m_uiPhase;
                m_uiPhaseTimer = 16000;
                break;
            case 4:
                // spawn pathaleon's image
                m_creature->SummonCreature(NPC_PATHALEON_THE_CALCULATOR_IMAGE, 2325.851563f, 2799.534668f, 133.084229f, 6.038996f, TEMPSUMMON_TIMED_DESPAWN, 90000);
                ++m_uiPhase;
                m_uiPhaseTimer = 500;
                break;
            case 5:
                DoScriptText(SAY_PATHALEON_THE_CALCULATOR_IMAGE_1, pPathaleon);
                ++m_uiPhase;
                m_uiPhaseTimer = 6000;
                break;
            case 6:
                switch(m_uiPhaseSubphase)
                {
                    case 0:
                        TurnToPathaleonsImage();
                        ++m_uiPhaseSubphase;
                        m_uiPhaseTimer = 8000;
                        break;
                    case 1:
                        DoScriptText(SAY_COMMANDER_DAWNFORGE_3, m_creature);
                        m_uiPhaseSubphase = 0;
                        ++m_uiPhase;
                        m_uiPhaseTimer = 8000;
                        break;
                }
                break;
            case 7:
                switch(m_uiPhaseSubphase)
                {
                    case 0:
                        DoScriptText(SAY_PATHALEON_THE_CALCULATOR_IMAGE_2, pPathaleon);
                        ++m_uiPhaseSubphase;
                        m_uiPhaseTimer = 12000;
                        break;
                    case 1:
                        DoScriptText(SAY_PATHALEON_THE_CALCULATOR_IMAGE_2_1, pPathaleon);
                        ++m_uiPhaseSubphase;
                        m_uiPhaseTimer = 16000;
                        break;
                    case 2:
                        DoScriptText(SAY_PATHALEON_THE_CALCULATOR_IMAGE_2_2, pPathaleon);
                        m_uiPhaseSubphase = 0;
                        ++m_uiPhase;
                        m_uiPhaseTimer = 10000;
                        break;
                }
                break;
            case 8:
                DoScriptText(SAY_COMMANDER_DAWNFORGE_4, m_creature);
                DoScriptText(SAY_ARCANIST_ARDONIS_2, pArdonis);
                ++m_uiPhase;
                m_uiPhaseTimer = 4000;
                break;
            case 9:
                TurnToEachOther();
                // hide pathaleon, unit will despawn shortly
                pPathaleon->SetVisibility(VISIBILITY_OFF);
                m_uiPhaseSubphase = 0;
                ++m_uiPhase;
                m_uiPhaseTimer = 3000;
                break;
            case 10:
                DoScriptText(SAY_COMMANDER_DAWNFORGE_5, m_creature);
                pPlayer->AreaExploredOrEventHappens(QUEST_INFO_GATHERING);
                Reset();
                break;
        }
    }
};

CreatureAI* GetAI_npc_commander_dawnforge(Creature* pCreature)
{
    return new npc_commander_dawnforgeAI(pCreature);
}

bool AreaTrigger_at_commander_dawnforge(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    // if player lost aura or not have at all, we should not try start event.
    if (!pPlayer->HasAura(SPELL_SUNFURY_DISGUISE, EFFECT_INDEX_0))
        return false;

    if (pPlayer->isAlive() && pPlayer->GetQuestStatus(QUEST_INFO_GATHERING) == QUEST_STATUS_INCOMPLETE)
    {
        Creature* pDawnforge = GetClosestCreatureWithEntry(pPlayer, NPC_COMMANDER_DAWNFORGE, 30.0f);

        if (!pDawnforge)
            return false;

        if (npc_commander_dawnforgeAI* pDawnforgeAI = dynamic_cast<npc_commander_dawnforgeAI*>(pDawnforge->AI()))
        {
            pDawnforgeAI->CanStartEvent(pPlayer);
            return true;
        }


    }
    return false;
}

/*######
## npc_protectorate_nether_drake
######*/

enum
{
    QUEST_NETHER_WINGS          = 10438,
    ITEM_PH_DISRUPTOR           = 29778,
    TAXI_PATH_ID                = 627                       //(possibly 627+628(152->153->154->155))
};

#define GOSSIP_ITEM_FLY_ULTRIS  "Fly me to Ultris"

bool GossipHello_npc_protectorate_nether_drake(Player* pPlayer, Creature* pCreature)
{
    // On Nethery Wings
    if (pPlayer->GetQuestStatus(QUEST_NETHER_WINGS) == QUEST_STATUS_INCOMPLETE && pPlayer->HasItemCount(ITEM_PH_DISRUPTOR, 1))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FLY_ULTRIS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_protectorate_nether_drake(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID);
    }
    return true;
}

/*######
## npc_veronia
######*/

enum
{
    QUEST_BEHIND_ENEMY_LINES = 10652,
    SPELL_STEALTH_FLIGHT     = 34905
};

#define GOSSIP_ITEM_FLY_CORUU  "Fly me to Manaforge Coruu please"

bool GossipHello_npc_veronia(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    // Behind Enemy Lines
    if (pPlayer->GetQuestStatus(QUEST_BEHIND_ENEMY_LINES) && !pPlayer->GetQuestRewardStatus(QUEST_BEHIND_ENEMY_LINES))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FLY_CORUU, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_veronia(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_STEALTH_FLIGHT, true);//TaxiPath 606
    }
    return true;
}

/*######
## npc_bessy
######*/

enum
{
    QUEST_COWS_COME_HOME = 10337,

    NPC_THADELL          = 20464,
    NPC_TORMENTED_SOUL   = 20512,
    NPC_SEVERED_SPIRIT   = 19881
};

struct MANGOS_DLL_DECL npc_bessyAI : public npc_escortAI
{
    npc_bessyAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 3:
                m_creature->SummonCreature(NPC_TORMENTED_SOUL, 2449.67f, 2183.11f, 96.85f, 6.20f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(NPC_TORMENTED_SOUL, 2449.53f, 2184.43f, 96.36f, 6.27f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(NPC_TORMENTED_SOUL, 2449.85f, 2186.34f, 97.57f, 6.08f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                break;
            case 7:
                m_creature->SummonCreature(NPC_SEVERED_SPIRIT, 2309.64f, 2186.24f, 92.25f, 6.06f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(NPC_SEVERED_SPIRIT, 2309.25f, 2183.46f, 91.75f, 6.22f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                break;
            case 12:
                if (Player* pPlayer = GetPlayerForEscort())
                    pPlayer->GroupEventHappens(QUEST_COWS_COME_HOME, m_creature);
                break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(m_creature);
    }

    void Reset() {}
};

bool QuestAccept_npc_bessy(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_COWS_COME_HOME)
    {
        pCreature->setFaction(FACTION_ESCORT_N_NEUTRAL_PASSIVE);
        pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (npc_bessyAI* pBessyAI = dynamic_cast<npc_bessyAI*>(pCreature->AI()))
            pBessyAI->Start(true, pPlayer->GetGUID(), pQuest);
    }
    return true;
}

CreatureAI* GetAI_npc_bessy(Creature* pCreature)
{
     return new npc_bessyAI(pCreature);
}

/*######
## npc_maxx_a_million
######*/

enum
{
    QUEST_MARK_V_IS_ALIVE       = 10191,
    NPC_BOT_SPECIALIST_ALLEY    = 19578,
    GO_DRAENEI_MACHINE          = 183771,

    SAY_START                   = -1000621,
    SAY_ALLEY_FAREWELL          = -1000622,
    SAY_CONTINUE                = -1000623,
    SAY_ALLEY_FINISH            = -1000624
};

struct MANGOS_DLL_DECL npc_maxx_a_million_escortAI : public npc_escortAI
{
    npc_maxx_a_million_escortAI(Creature* pCreature) : npc_escortAI(pCreature) {Reset();}

    uint8 m_uiSubEvent;
    uint32 m_uiSubEventTimer;
    uint64 m_uiAlleyGUID;
    uint64 m_uiLastDraeneiMachineGUID;

    void Reset()
    {
        if (!HasEscortState(STATE_ESCORT_ESCORTING))
        {
            m_uiSubEvent = 0;
            m_uiSubEventTimer = 0;
            m_uiAlleyGUID = 0;
            m_uiLastDraeneiMachineGUID = 0;

            // Reset fields, that were changed on escort-start
            m_creature->HandleEmote(EMOTE_STATE_STUN);
            // Faction is reset with npc_escortAI::JustRespawned();

            // Unclear how these flags are set/removed in relation to the faction change at start of escort.
            // Workaround here, so that the flags are removed during escort (and while not in evade mode)
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE + UNIT_FLAG_PASSIVE);
        }
        else
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
    }

    void WaypointReached(uint32 uiPoint)
    {
        switch (uiPoint)
        {
            case 1:
                // turn 90 degrees , towards doorway.
                m_creature->SetFacingTo(m_creature->GetOrientation() + (M_PI_F/2));
                DoScriptText(SAY_START, m_creature);
                m_uiSubEventTimer = 3000;
                m_uiSubEvent = 1;
                break;
            case 7:
            case 17:
            case 29:
                if (GameObject* pMachine = GetClosestGameObjectWithEntry(m_creature, GO_DRAENEI_MACHINE, INTERACTION_DISTANCE))
                {
                    m_creature->SetFacingToObject(pMachine);
                    m_uiLastDraeneiMachineGUID = pMachine->GetGUID();
                    m_uiSubEvent = 2;
                    m_uiSubEventTimer = 1000;
                }
                else
                    m_uiLastDraeneiMachineGUID = 0;

                break;
            case 36:
                if (Player* pPlayer = GetPlayerForEscort())
                    pPlayer->GroupEventHappens(QUEST_MARK_V_IS_ALIVE, m_creature);

                if (Creature* pAlley = m_creature->GetMap()->GetCreature(m_uiAlleyGUID))
                    DoScriptText(SAY_ALLEY_FINISH, pAlley);

                break;
        }
    }

    void WaypointStart(uint32 uiPoint)
    {
        switch (uiPoint)
        {
            case 8:
            case 18:
            case 30:
                DoScriptText(SAY_CONTINUE, m_creature);
                break;
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() ||  !m_creature->getVictim())
        {
            if (m_uiSubEventTimer)
            {
                if (m_uiSubEventTimer <= uiDiff)
                {
                    switch (m_uiSubEvent)
                    {
                        case 1:                             // Wait time before Say
                            if (Creature* pAlley = GetClosestCreatureWithEntry(m_creature, NPC_BOT_SPECIALIST_ALLEY, INTERACTION_DISTANCE*2))
                            {
                                m_uiAlleyGUID = pAlley->GetGUID();
                                DoScriptText(SAY_ALLEY_FAREWELL, pAlley);
                            }
                            m_uiSubEventTimer = 0;
                            m_uiSubEvent = 0;
                            break;
                        case 2:                             // Short wait time after reached WP at machine
                            m_creature->HandleEmote(EMOTE_ONESHOT_ATTACKUNARMED);
                            m_uiSubEventTimer = 2000;
                            m_uiSubEvent = 3;
                            break;
                        case 3:                             // Despawn machine after 2s
                            if (GameObject* pMachine = m_creature->GetMap()->GetGameObject(m_uiLastDraeneiMachineGUID))
                                pMachine->Use(m_creature);

                            m_uiLastDraeneiMachineGUID = 0;
                            m_uiSubEventTimer = 0;
                            m_uiSubEvent = 0;
                            break;
                        default:
                            m_uiSubEventTimer = 0;
                            break;
                    }
                }
                else
                    m_uiSubEventTimer -= uiDiff;
            }
        }
        else
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_maxx_a_million(Creature* pCreature)
{
    return new npc_maxx_a_million_escortAI(pCreature);
}

bool QuestAccept_npc_maxx_a_million(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_MARK_V_IS_ALIVE)
    {
        if (npc_maxx_a_million_escortAI* pEscortAI = dynamic_cast<npc_maxx_a_million_escortAI*>(pCreature->AI()))
        {
            // Set Faction to Escort Faction
            pCreature->setFaction(FACTION_ESCORT_N_NEUTRAL_PASSIVE);
            // Set emote-state to 0 (is EMOTE_STATE_STUN by default)
            pCreature->HandleEmote(EMOTE_ONESHOT_NONE);
            // Remove unit_flags (see comment in JustReachedHome)
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE + UNIT_FLAG_PASSIVE);

            pEscortAI->Start(false, pPlayer->GetGUID(), pQuest, true);
        }
    }
    return true;
}

/*######
## npc_zeppit
######*/

enum
{
    EMOTE_GATHER_BLOOD          = -1000625,
    NPC_WARP_CHASER             = 18884,
    SPELL_GATHER_WARP_BLOOD     = 39244,                    // for quest 10924
};

struct MANGOS_DLL_DECL npc_zeppitAI : public ScriptedPetAI
{
    npc_zeppitAI(Creature* pCreature) : ScriptedPetAI(pCreature) { Reset(); }

    void Reset() { }

    void OwnerKilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_UNIT && pVictim->GetEntry() == NPC_WARP_CHASER)
        {
            // Distance not known, be assumed to be ~10 yards, possibly a bit less.
            if (m_creature->IsWithinDistInMap(pVictim, 10.0f))
            {
                DoScriptText(EMOTE_GATHER_BLOOD, m_creature);
                m_creature->CastSpell(m_creature, SPELL_GATHER_WARP_BLOOD, false);
            }
        }
    }
};

CreatureAI* GetAI_npc_zeppit(Creature* pCreature)
{
    return new npc_zeppitAI(pCreature);
}

void AddSC_netherstorm()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "go_manaforge_control_console";
    pNewScript->pGOUse = &GOUse_go_manaforge_control_console;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_manaforge_control_console";
    pNewScript->GetAI = &GetAI_npc_manaforge_control_console;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_commander_dawnforge";
    pNewScript->GetAI = GetAI_npc_commander_dawnforge;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_commander_dawnforge";
    pNewScript->pAreaTrigger = &AreaTrigger_at_commander_dawnforge;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_protectorate_nether_drake";
    pNewScript->pGossipHello = &GossipHello_npc_protectorate_nether_drake;
    pNewScript->pGossipSelect = &GossipSelect_npc_protectorate_nether_drake;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_veronia";
    pNewScript->pGossipHello = &GossipHello_npc_veronia;
    pNewScript->pGossipSelect = &GossipSelect_npc_veronia;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_bessy";
    pNewScript->GetAI = &GetAI_npc_bessy;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_bessy;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_maxx_a_million";
    pNewScript->GetAI = &GetAI_npc_maxx_a_million;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_maxx_a_million;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_zeppit";
    pNewScript->GetAI = &GetAI_npc_zeppit;
    pNewScript->RegisterSelf();
}
