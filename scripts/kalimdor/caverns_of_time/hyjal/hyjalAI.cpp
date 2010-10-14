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
SDName: HyjalAI
SD%Complete: 90
SDComment:
SDCategory: Caverns of Time, Mount Hyjal
EndScriptData */

#include "precompiled.h"
#include "hyjalAI.h"

struct sHyjalLocation
{
    eBaseArea m_pBaseArea;
    float m_fX, m_fY, m_fZ;
};

// Locations for summoning waves
sHyjalLocation m_aHyjalSpawnLoc[]=
{
    {BASE_ALLY,  4979.010f, -1709.134f, 1339.674f},
    {BASE_ALLY,  4969.123f, -1705.904f, 1341.363f},
    {BASE_ALLY,  4970.260f, -1698.546f, 1341.200f},
    {BASE_ALLY,  4975.262f, -1698.239f, 1341.427f},
    {BASE_HORDE, 5557.582f, -2587.159f, 1481.644f},
    {BASE_HORDE, 5545.901f, -2582.246f, 1479.256f},
    {BASE_HORDE, 5565.642f, -2565.666f, 1481.635f},
    {BASE_HORDE, 5547.218f, -2574.589f, 1479.194f}
};

// used to inform the wave where to move
sHyjalLocation m_aHyjalWaveMoveTo[]=
{
    {BASE_ALLY,  5018.654f, -1752.074f, 1322.203f},
    {BASE_HORDE, 5504.569f, -2688.489f, 1479.991f}
};

struct sHyjalYells
{
    uint32   uiCreatureEntry;
    YellType m_pYellType;                                   // Used to determine the type of yell (attack, rally, etc)
    int32    m_iTextId;                                     // The text id to be yelled
};

sHyjalYells m_aHyjalYell[]=
{
    {NPC_JAINA,  ATTACKED, -1534000},
    {NPC_JAINA,  ATTACKED, -1534001},
    {NPC_JAINA,  INCOMING, -1534002},
    {NPC_JAINA,  BEGIN,    -1534003},
    {NPC_JAINA,  RALLY,    -1534004},
    {NPC_JAINA,  RALLY,    -1534005},
    {NPC_JAINA,  FAILURE,  -1534006},
    {NPC_JAINA,  SUCCESS,  -1534007},
    {NPC_JAINA,  DEATH,    -1534008},

    {NPC_THRALL, ATTACKED, -1534009},
    {NPC_THRALL, ATTACKED, -1534010},
    {NPC_THRALL, INCOMING, -1534011},
    {NPC_THRALL, BEGIN,    -1534012},
    {NPC_THRALL, RALLY,    -1534013},
    {NPC_THRALL, RALLY,    -1534014},
    {NPC_THRALL, FAILURE,  -1534015},
    {NPC_THRALL, SUCCESS,  -1534016},
    {NPC_THRALL, DEATH,    -1534017}
};

void hyjalAI::Reset()
{
    // GUIDs
    m_uiBossGUID[0] = 0;
    m_uiBossGUID[1] = 0;

    // Timers
    m_uiNextWaveTimer = 10000;
    m_uiWaveMoveTimer = 15000;
    m_uiCheckTimer = 0;
    m_uiRetreatTimer = 25000;

    // Misc
    m_uiWaveCount = 0;
    m_uiEnemyCount = 0;

    // Set base area based on creature entry
    switch(m_creature->GetEntry())
    {
        case NPC_JAINA:
            m_uiBase = BASE_ALLY;
            DoCastSpellIfCan(m_creature, SPELL_BRILLIANCE_AURA, CAST_TRIGGERED);
            break;
        case NPC_THRALL:
            m_uiBase = BASE_HORDE;
            break;
    }

    // Bools
    m_bIsEventInProgress = false;
    m_bIsSummoningWaves = false;

    m_bIsRetreating = false;
    m_bDebugMode = false;

    // Flags
    m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

    // Reset World States
    m_pInstance->DoUpdateWorldState(WORLD_STATE_WAVES, 0);
    m_pInstance->DoUpdateWorldState(WORLD_STATE_ENEMY, 0);
    m_pInstance->DoUpdateWorldState(WORLD_STATE_ENEMYCOUNT, 0);

    if (!m_pInstance)
        return;

    m_bIsFirstBossDead = m_uiBase ? m_pInstance->GetData(TYPE_KAZROGAL) : m_pInstance->GetData(TYPE_WINTERCHILL);
    m_bIsSecondBossDead = m_uiBase ? m_pInstance->GetData(TYPE_AZGALOR) : m_pInstance->GetData(TYPE_ANETHERON);

    // Reset Instance Data for trash count
    m_pInstance->SetData(DATA_RESET_TRASH_COUNT, 0);
}

void hyjalAI::EnterEvadeMode()
{
    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop(true);
    m_creature->LoadCreatureAddon();

    if (m_creature->isAlive())
        m_creature->GetMotionMaster()->MoveTargetedHome();

    m_creature->SetLootRecipient(NULL);
}

void hyjalAI::JustReachedHome()
{
    if (m_uiBase == BASE_ALLY)
        DoCastSpellIfCan(m_creature, SPELL_BRILLIANCE_AURA, CAST_TRIGGERED);

    m_bIsFirstBossDead = m_uiBase ? m_pInstance->GetData(TYPE_KAZROGAL) : m_pInstance->GetData(TYPE_WINTERCHILL);
    m_bIsSecondBossDead = m_uiBase ? m_pInstance->GetData(TYPE_AZGALOR) : m_pInstance->GetData(TYPE_ANETHERON);
}

void hyjalAI::Aggro(Unit *who)
{
    for(uint8 i = 0; i < MAX_SPELL; ++i)
        if (m_aSpells[i].m_uiCooldown)
            m_uiSpellTimer[i] = m_aSpells[i].m_uiCooldown;

    DoTalk(ATTACKED);
}

void hyjalAI::SpawnCreatureForWave(uint32 uiMobEntry)
{
    sHyjalLocation* pSpawn = NULL;

    uint32 uiMaxCount = sizeof(m_aHyjalSpawnLoc)/sizeof(sHyjalLocation);
    uint32 uiRandId = urand(1, uiMaxCount/2);               //unsafe, if array becomes uneven.

    uint32 uiJ = 0;

    for (uint32 i = 0; i < uiMaxCount; ++i)
    {
        if (m_aHyjalSpawnLoc[i].m_pBaseArea != m_uiBase)
            continue;

        ++uiJ;

        if (uiJ == uiRandId)
        {
            pSpawn = &m_aHyjalSpawnLoc[i];
            break;
        }
    }

    if (pSpawn)
        m_creature->SummonCreature(uiMobEntry, pSpawn->m_fX, pSpawn->m_fY, pSpawn->m_fZ, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000);
}

void hyjalAI::JustSummoned(Creature* pSummoned)
{
    // not interesting for us
    if (pSummoned->GetEntry() == NPC_WATER_ELEMENTAL || pSummoned->GetEntry() == NPC_DIRE_WOLF)
        return;

    // Increment Enemy Count to be used in World States and instance script
    ++m_uiEnemyCount;

    sHyjalLocation* pMove = NULL;

    uint32 uiMaxCount = sizeof(m_aHyjalWaveMoveTo)/sizeof(sHyjalLocation);

    for (uint32 i = 0; i < uiMaxCount; ++i)
    {
        if (m_aHyjalWaveMoveTo[i].m_pBaseArea != m_uiBase)
            continue;

        pMove = &m_aHyjalWaveMoveTo[i];
        break;
    }

    if (pMove)
    {
        float fX, fY, fZ;
        pSummoned->GetRandomPoint(pMove->m_fX, pMove->m_fY, pMove->m_fZ, 10.0f, fX, fY, fZ);

        pSummoned->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        pSummoned->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
    }

    // Check if creature is a boss.
    if (pSummoned->IsWorldBoss())
    {
        if (!m_bIsFirstBossDead)
            m_uiBossGUID[0] = pSummoned->GetGUID();
        else
            m_uiBossGUID[1] = pSummoned->GetGUID();

        m_uiCheckTimer = 5000;
    }
    else
        lWaveMobGUIDList.push_back(pSummoned->GetGUID());
}

void hyjalAI::SummonNextWave()
{
    // 1 in 4 chance we give a rally yell. Not sure if the chance is offilike.
    if (!urand(0, 3))
        DoTalk(RALLY);

    if (!m_pInstance)
    {
        error_log(ERROR_INST_DATA);
        return;
    }

    sHyjalWave* pWaveData = m_uiBase ? &m_aHyjalWavesHorde[m_uiWaveCount] : &m_aHyjalWavesAlliance[m_uiWaveCount];

    if (!pWaveData)
    {
        error_log("SD2: hyjalAI not able to obtain wavedata for SummonNextWave.");
        return;
    }

    m_uiEnemyCount = m_pInstance->GetData(DATA_TRASH);

    for(uint8 i = 0; i < MAX_WAVE_MOB; ++i)
    {
        if (pWaveData->m_auiMobEntry[i])
            SpawnCreatureForWave(pWaveData->m_auiMobEntry[i]);
    }

    if (!pWaveData->m_bIsBoss)
    {
        uint32 stateValue = m_uiWaveCount+1;

        if (m_bIsFirstBossDead)
            stateValue -= MAX_WAVES;                        // Subtract 9 from it to give the proper wave number if we are greater than 8

        // Set world state to our current wave number
        m_pInstance->DoUpdateWorldState(WORLD_STATE_WAVES, stateValue);
        // Enable world state
        m_pInstance->DoUpdateWorldState(WORLD_STATE_ENEMY, 1);

        m_pInstance->SetData(DATA_TRASH, m_uiEnemyCount);   // Send data for instance script to update count

        if (!m_bDebugMode)
            m_uiNextWaveTimer = pWaveData->m_uiWaveTimer;
        else
        {
            m_uiNextWaveTimer = 15000;
            debug_log("SD2: HyjalAI: debug mode is enabled. Next Wave in 15 seconds");
        }
    }
    else
    {
        // Set world state for waves to 0 to disable it.
        m_pInstance->DoUpdateWorldState(WORLD_STATE_WAVES, 0);
        m_pInstance->DoUpdateWorldState(WORLD_STATE_ENEMY, 1);

        // Set World State for enemies invading to 1.
        m_pInstance->DoUpdateWorldState(WORLD_STATE_ENEMYCOUNT, 1);

        m_bIsSummoningWaves = false;
    }

    m_uiWaveMoveTimer = 15000;
    m_uiCheckTimer = 5000;
    ++m_uiWaveCount;
}

void hyjalAI::StartEvent()
{
    if (!m_pInstance)
        return;

    DoTalk(BEGIN);

    m_bIsEventInProgress = true;
    m_bIsSummoningWaves = true;

    m_uiNextWaveTimer = 10000;
    m_uiCheckTimer = 5000;

    m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

    m_pInstance->DoUpdateWorldState(WORLD_STATE_WAVES, 0);
    m_pInstance->DoUpdateWorldState(WORLD_STATE_ENEMY, 0);
    m_pInstance->DoUpdateWorldState(WORLD_STATE_ENEMYCOUNT, 0);
}

void hyjalAI::DoTalk(YellType pYellType)
{
    sHyjalYells* pYell = NULL;

    uint32 uiMaxCount = sizeof(m_aHyjalYell)/sizeof(sHyjalYells);
    bool bGetNext = false;

    for (uint32 i = 0; i < uiMaxCount; ++i)
    {
        if (m_aHyjalYell[i].uiCreatureEntry == m_creature->GetEntry() && m_aHyjalYell[i].m_pYellType == pYellType)
        {
            //this would not be safe unless we knew these had two entries in m_aYell
            if (pYellType == ATTACKED || pYellType== RALLY)
            {
                if (!bGetNext && urand(0, 1))
                {
                    bGetNext = true;
                    continue;
                }
            }

            pYell = &m_aHyjalYell[i];
            break;
        }
    }

    if (pYell)
        DoScriptText(pYell->m_iTextId, m_creature);
}

void hyjalAI::SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
{
    //TODO: this spell should cause misc mobs to despawn
    //if (pSpell->Id == SPELL_MASS_TELEPORT && pTarget->GetTypeId() != TYPEID_PLAYER)
    //{
        //despawn;
    //}
}

void hyjalAI::Retreat()
{
    //this will trigger ancient gem respawn
    if (m_pInstance)
        m_pInstance->SetData(TYPE_RETREAT, SPECIAL);

    DoCastSpellIfCan(m_creature, SPELL_MASS_TELEPORT);

    m_bIsRetreating = true;
}

void hyjalAI::JustDied(Unit* pKiller)
{
    DoTalk(DEATH);

    //TODO: in case they die during boss encounter, then what? despawn boss?
}

void hyjalAI::UpdateAI(const uint32 uiDiff)
{
    if (!m_bIsEventInProgress)
        return;

    if (m_bIsSummoningWaves && m_pInstance)
    {
        if (m_uiWaveMoveTimer < uiDiff)
        {
            // Skip the master timer, and start next wave in 5. Clear the list, it should not be any here now.
            if (!m_pInstance->GetData(DATA_TRASH))
            {
                lWaveMobGUIDList.clear();
                m_uiNextWaveTimer = 5000;
            }

            if (!lWaveMobGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = lWaveMobGUIDList.begin(); itr != lWaveMobGUIDList.end(); ++itr)
                {
                    if (Creature* pTemp = m_pInstance->instance->GetCreature(*itr))
                    {
                        if (!pTemp->isAlive() || pTemp->getVictim())
                            continue;

                        pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        pTemp->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                    }
                }
            }
            m_uiWaveMoveTimer = 10000;
        }
        else
            m_uiWaveMoveTimer -= uiDiff;

        if (m_uiNextWaveTimer < uiDiff)
            SummonNextWave();
        else
            m_uiNextWaveTimer -= uiDiff;
    }

    if (m_uiCheckTimer < uiDiff)
    {
        for(uint8 i = 0; i < 2; ++i)
        {
            if (m_uiBossGUID[i])
            {
                Creature* pBoss = m_creature->GetMap()->GetCreature(m_uiBossGUID[i]);

                if (pBoss && !pBoss->isAlive())
                {
                    if (m_uiBossGUID[i] == m_uiBossGUID[0])
                    {
                        DoTalk(INCOMING);
                        m_bIsFirstBossDead = true;
                    }
                    else if (m_uiBossGUID[i] == m_uiBossGUID[1])
                    {
                        DoTalk(SUCCESS);
                        m_bIsSecondBossDead = true;
                    }

                    m_bIsEventInProgress = false;
                    m_uiCheckTimer = 0;

                    m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                    m_uiBossGUID[i] = 0;

                    // Reset world state for enemies to disable it
                    m_pInstance->DoUpdateWorldState(WORLD_STATE_ENEMY, 0);
                }
            }
        }

        m_uiCheckTimer = 5000;
    }
    else
        m_uiCheckTimer -= uiDiff;

    if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        return;

    for(uint8 i = 0; i < MAX_SPELL; ++i)
    {
        if (m_aSpells[i].m_uiSpellId)
        {
            if (m_uiSpellTimer[i] < uiDiff)
            {
                if (m_creature->IsNonMeleeSpellCasted(false))
                    m_creature->InterruptNonMeleeSpells(false);

                Unit* pTarget = NULL;

                switch(m_aSpells[i].m_pType)
                {
                    case TARGETTYPE_SELF:   pTarget = m_creature; break;
                    case TARGETTYPE_RANDOM: pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0); break;
                    case TARGETTYPE_VICTIM: pTarget = m_creature->getVictim(); break;
                }

                if (pTarget)
                {
                    DoCastSpellIfCan(pTarget, m_aSpells[i].m_uiSpellId);
                    m_uiSpellTimer[i] = m_aSpells[i].m_uiCooldown;
                }
            }
            else
                m_uiSpellTimer[i] -= uiDiff;
        }
    }

    DoMeleeAttackIfReady();
}
