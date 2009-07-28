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
SDName: HyjalAI
SD%Complete: 90
SDComment:
SDCategory: Caverns of Time, Mount Hyjal
EndScriptData */

#include "precompiled.h"
#include "hyjalAI.h"

// Locations for summoning waves in Alliance base
float AllianceBase[4][3]=
{
    {4979.010, -1709.134, 1339.674},
    {4969.123, -1705.904, 1341.363},
    {4970.260, -1698.546, 1341.200},
    {4975.262, -1698.239, 1341.427}
};

// Locations for summoning waves in Horde base
float HordeBase[4][3]=
{
    {5554.399, -2581.419, 1480.820},
    {5538.996, -2577.742, 1479.790},
    {5565.642, -2565.666, 1481.635},
    {5547.218, -2574.589, 1479.194}
};

// used to inform the wave where to move and attack to
/*float AttackArea[2][3]=
{
    {5042.9189, -1776.2562, 1323.0621},                     // Alliance
    {5510.4815, -2676.7112, 1480.4314}                      // Horde
};*/

struct Yells
{
    uint32   uiCreatureEntry;
    YellType m_pYellType;                                   // Used to determine the type of yell (attack, rally, etc)
    int32    m_iTextId;                                     // The text id to be yelled
};

Yells m_aYell[]=
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
    m_uiCheckTimer = 0;
    m_uiRetreatTimer = 1000;

    // Misc
    m_uiWaveCount = 0;
    m_uiEnemyCount = 0;

    // Set base area based on creature entry
    switch(m_creature->GetEntry())
    {
        case NPC_JAINA:
            m_uiBase = BASE_ALLY;
            DoCast(m_creature, SPELL_BRILLIANCE_AURA, true);
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
    UpdateWorldState(WORLD_STATE_WAVES, 0);
    UpdateWorldState(WORLD_STATE_ENEMY, 0);
    UpdateWorldState(WORLD_STATE_ENEMYCOUNT, 0);

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
    m_creature->LoadCreaturesAddon();

    if (m_creature->isAlive())
        m_creature->GetMotionMaster()->MoveTargetedHome();

    m_creature->SetLootRecipient(NULL);
}

void hyjalAI::JustReachedHome()
{
    if (m_uiBase == BASE_ALLY)
        DoCast(m_creature, SPELL_BRILLIANCE_AURA, true);

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

void hyjalAI::SummonCreature(uint32 entry, float Base[4][3])
{
    uint32 random = rand()%4;
    float SpawnLoc[3];
    //float AttackLoc[3];

    for(uint8 i = 0; i < 3; ++i)
    {
        SpawnLoc[i] = Base[random][i];

        //not needed, we already make creature attack and then targeted movegen initialized
        //AttackLoc[i] = AttackArea[Faction][i];
    }

    if (Creature* pCreature = m_creature->SummonCreature(entry, SpawnLoc[0], SpawnLoc[1], SpawnLoc[2], 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000))
    {
        // Increment Enemy Count to be used in World States and instance script
        ++m_uiEnemyCount;

        pCreature->SetInCombatWith(m_creature);
        pCreature->AddThreat(m_creature, 0.0f);

        pCreature->SetInCombatWithZone();

        // Check if creature is a boss.
        if (pCreature->isWorldBoss())
        {
            if (!m_bIsFirstBossDead)
                m_uiBossGUID[0] = pCreature->GetGUID();
            else
                m_uiBossGUID[1] = pCreature->GetGUID();

            m_uiCheckTimer = 5000;
        }
    }
}

void hyjalAI::SummonNextWave(Wave wave[18], uint32 Count, float Base[4][3])
{
    // 1 in 4 chance we give a rally yell. Not sure if the chance is offilike.
    if (rand()%4 == 0)
        DoTalk(RALLY);

    if (!m_pInstance)
    {
        error_log(ERROR_INST_DATA);
        return;
    }

    m_uiEnemyCount = m_pInstance->GetData(DATA_TRASH);

    for(uint8 i = 0; i < MAX_WAVE_MOB; ++i)
    {
        if (wave[Count].Mob[i])
            SummonCreature(wave[Count].Mob[i], Base);
    }

    if (!wave[Count].IsBoss)
    {
        uint32 stateValue = Count+1;

        if (m_bIsFirstBossDead)
            stateValue -= MAX_WAVES;                        // Subtract 9 from it to give the proper wave number if we are greater than 8

        UpdateWorldState(WORLD_STATE_WAVES, stateValue);    // Set world state to our current wave number
        UpdateWorldState(WORLD_STATE_ENEMY, 1);             // Enable world state

        m_pInstance->SetData(DATA_TRASH, m_uiEnemyCount);   // Send data for instance script to update count

        if (!m_bDebugMode)
            m_uiNextWaveTimer = wave[Count].WaveTimer;
        else
        {
            m_uiNextWaveTimer = 15000;
            debug_log("SD2: HyjalAI: debug mode is enabled. Next Wave in 15 seconds");
        }
    }
    else
    {
        UpdateWorldState(WORLD_STATE_WAVES, 0);             // Set world state for waves to 0 to disable it.
        UpdateWorldState(WORLD_STATE_ENEMY, 1);
        UpdateWorldState(WORLD_STATE_ENEMYCOUNT, 1);        // Set World State for enemies invading to 1.
        m_bIsSummoningWaves = false;
    }

    m_uiCheckTimer = 5000;
}

void hyjalAI::StartEvent()
{
    DoTalk(BEGIN);

    m_bIsEventInProgress = true;
    m_bIsSummoningWaves = true;

    m_uiNextWaveTimer = 15000;
    m_uiCheckTimer = 5000;

    m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

    UpdateWorldState(WORLD_STATE_WAVES, 0);
    UpdateWorldState(WORLD_STATE_ENEMY, 0);
    UpdateWorldState(WORLD_STATE_ENEMYCOUNT, 0);
}

void hyjalAI::DoTalk(YellType pYellType)
{
    Yells* pYell = NULL;

    uint32 uiMaxCount = sizeof(m_aYell)/sizeof(Yells);
    bool bGetNext = false;

    for (uint32 i = 0; i < uiMaxCount; ++i)
    {
        if (m_aYell[i].uiCreatureEntry == m_creature->GetEntry() && m_aYell[i].m_pYellType == pYellType)
        {
            //this would not be safe unless we knew these had two entries in m_aYell
            if (pYellType == ATTACKED || pYellType== RALLY)
            {
                if (!bGetNext && rand()%2)
                {
                    bGetNext = true;
                    continue;
                }
            }

            pYell = &m_aYell[i];
            break;
        }
    }

    if (pYell)
        DoScriptText(pYell->m_iTextId, m_creature);
}

void hyjalAI::UpdateWorldState(uint32 uiStateId, uint32 uiState)
{
    Map* pMap = m_creature->GetMap();

    if (!pMap->IsDungeon())
        return;

    Map::PlayerList const& players = pMap->GetPlayers();

    if (!players.isEmpty())
    {
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                pPlayer->SendUpdateWorldState(uiStateId, uiState);
        }
    }
    else
        debug_log("SD2: HyjalAI: UpdateWorldState, but PlayerList is empty");
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

    DoCast(m_creature, SPELL_MASS_TELEPORT);

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

    if (m_bIsSummoningWaves)
    {
        if (m_pInstance && m_uiEnemyCount)
        {
            m_uiEnemyCount = m_pInstance->GetData(DATA_TRASH);

            if (!m_uiEnemyCount)
                m_uiNextWaveTimer = 5000;
        }

        if (m_uiNextWaveTimer < uiDiff)
        {
            if (m_uiBase == BASE_HORDE)
                SummonNextWave(HordeWaves, m_uiWaveCount, HordeBase);
            else
                SummonNextWave(AllianceWaves, m_uiWaveCount, AllianceBase);

            ++m_uiWaveCount;
        }
        else
            m_uiNextWaveTimer -= uiDiff;
    }

    if (m_uiCheckTimer < uiDiff)
    {
        for(uint8 i = 0; i < 2; ++i)
        {
            if (m_uiBossGUID[i])
            {
                Unit* pUnit = Unit::GetUnit(*m_creature, m_uiBossGUID[i]);

                if (pUnit && !pUnit->isAlive())
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
                    UpdateWorldState(WORLD_STATE_ENEMY, 0); // Reset world state for enemies to disable it
                }
            }
        }

        m_uiCheckTimer = 5000;
    }
    else
        m_uiCheckTimer -= uiDiff;

    if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
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
                    case TARGETTYPE_RANDOM: pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0); break;
                    case TARGETTYPE_VICTIM: pTarget = m_creature->getVictim(); break;
                }

                if (pTarget)
                {
                    DoCast(pTarget, m_aSpells[i].m_uiSpellId);
                    m_uiSpellTimer[i] = m_aSpells[i].m_uiCooldown;
                }
            }
            else
                m_uiSpellTimer[i] -= uiDiff;
        }
    }

    DoMeleeAttackIfReady();
}
