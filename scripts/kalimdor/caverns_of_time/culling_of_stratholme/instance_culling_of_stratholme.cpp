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
SDName: Instance_culling_of_stratholme
SD%Complete: 80%
SDComment:
SDCategory: Culling of Stratholme
EndScriptData */

#include "precompiled.h"
#include "culling_of_stratholme.h"

enum
{
    MAX_ARTHAS_SPAWN_POS = 5,
    SAY_CHROMIE_HURRY    = -1000000                         // TODO
};

struct sSpawnLocation
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static sSpawnLocation m_aArthasSpawnLocs[] =                // need tuning
{
    {1969.73f, 1287.12f, 145.48f, 3.14f},
    {2049.43f, 1287.43f, 142.75f, 0.06f},
    {2365.54f, 1194.85f, 131.98f, 0.47f},
    {2534.46f, 1125.99f, 130.75f, 0.27f},
    {2363.77f, 1406.31f, 128.64f, 3.23f}
};

static sSpawnLocation m_aChromieSpawnLocs[] =               // need tuning, escpecially EndPositions!
{
    {1814.46f, 1283.97f, 142.30f, 4.32f},                   // near bridge
    {2311.0f, 1502.4f, 127.9f, 0.0f},                       // End
    {1811.52f, 1285.92f, 142.37f, 4.47f},                   // Hourglass, near bridge
    {2186.42f, 1323.77f, 129.91f, 0.0f},                    // Hourglass, End
};

instance_culling_of_stratholme::instance_culling_of_stratholme(Map* pMap) : ScriptedInstance(pMap),
    m_uiGrainCrateCount(0),
    m_uiRemoveCrateStateTimer(0),
    m_uiArthasRespawnTimer(0),

    m_uiChromieInnGUID(0),
    m_uiChromieEntranceGUID(0),
    m_uiChromieEndGUID(0),
    m_uiHourglassGUID(0),
    m_uiArthasGUID(0),
    m_uiMeathookGUID(0),
    m_uiSalrammGUID(0),
    m_uiEpochGUID(0),
    m_uiCorrupterGUID(0),
    m_uiLordaeronCrierGUID(0),

    m_uiBelfastGUID(0),
    m_uiForrestenGUID(0),
    m_uiSiabiGUID(0),
    m_uiJamesGUID(0),
    m_uiCorricksGUID(0),
    m_uiStoutmantleGUID(0),

    m_uiOwensGUID(0),
    m_uiMoriganGUID(0),
    m_uiAndersonGUID(0),
    m_uiMooreGUID(0),
    m_uiBattsonGUID(0),

    m_uiOReillyGUID(0),

    m_uiDoorBookcaseGUID(0),
    m_uiDarkRunedChestGUID(0)
{
    Initialize();
}

void instance_culling_of_stratholme::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_culling_of_stratholme::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_CHROMIE_INN:                   m_uiChromieInnGUID = pCreature->GetGUID();      break;
        case NPC_CHROMIE_ENTRANCE:              m_uiChromieEntranceGUID = pCreature->GetGUID(); break;
        case NPC_CHROMIE_END:                   m_uiChromieEndGUID = pCreature->GetGUID();      break;
        case NPC_HOURGLASS:                     m_uiHourglassGUID = pCreature->GetGUID();       break;
        case NPC_ARTHAS:                        m_uiArthasGUID = pCreature->GetGUID();          break;
        case NPC_MEATHOOK:                      m_uiMeathookGUID = pCreature->GetGUID();        break;
        case NPC_SALRAMM_THE_FLESHCRAFTER:      m_uiSalrammGUID = pCreature->GetGUID();         break;
        case NPC_CHRONO_LORD_EPOCH:             m_uiEpochGUID = pCreature->GetGUID();           break;
        case NPC_MALGANIS:                      m_uiMalganisGUID = pCreature->GetGUID();        break;
        case NPC_MICHAEL_BELFAST:               m_uiBelfastGUID = pCreature->GetGUID();         break;
        case NPC_HEARTHSINGER_FORRESTEN:        m_uiForrestenGUID = pCreature->GetGUID();       break;
        case NPC_FRAS_SIABI:                    m_uiSiabiGUID = pCreature->GetGUID();           break;
        case NPC_FOOTMAN_JAMES:                 m_uiJamesGUID = pCreature->GetGUID();           break;
        case NPC_MAL_CORRICKS:                  m_uiCorricksGUID = pCreature->GetGUID();        break;
        case NPC_GRYAN_STOUTMANTLE:             m_uiStoutmantleGUID = pCreature->GetGUID();     break;
        case NPC_ROGER_OWENS:                   m_uiOwensGUID = pCreature->GetGUID();           break;
        case NPC_SERGEANT_MORIGAN:              m_uiMoriganGUID = pCreature->GetGUID();         break;
        case NPC_JENA_ANDERSON:                 m_uiAndersonGUID = pCreature->GetGUID();        break;
        case NPC_MALCOM_MOORE:                  m_uiMooreGUID = pCreature->GetGUID();           break;
        case NPC_BARTLEBY_BATTSON:              m_uiBattsonGUID = pCreature->GetGUID();         break;
        case NPC_PATRICIA_O_REILLY:             m_uiOReillyGUID = pCreature->GetGUID();         break;
        case NPC_LORDAERON_CRIER:               m_uiLordaeronCrierGUID = pCreature->GetGUID();  break;
        case NPC_INFINITE_CORRUPTER:            m_uiCorrupterGUID = pCreature->GetGUID();       break;

        case NPC_CRATES_BUNNY:                  m_luiCratesBunnyGUIDs.push_back(pCreature->GetGUID()); break;
        case NPC_LORDAERON_FOOTMAN:             m_luiFootmanGUIDs.push_back(pCreature->GetGUID());     break;

        case NPC_STRATHOLME_CITIZEN:
        case NPC_STRATHOLME_RESIDENT:
            if (m_auiEncounter[TYPE_ARTHAS_INTRO_EVENT] == DONE)
                pCreature->UpdateEntry(NPC_ZOMBIE);
            else
                m_luiResidentGUIDs.push_back(pCreature->GetGUID());
            break;
        case NPC_AGIATED_STRATHOLME_CITIZEN:    m_lAgiatedCitizenGUIDList.push_back(pCreature->GetGUID());  break;
        case NPC_AGIATED_STRATHOLME_RESIDENT:   m_lAgiatedResidentGUIDList.push_back(pCreature->GetGUID()); break;
    }
}

void instance_culling_of_stratholme::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_BOOKCASE:
            m_uiDoorBookcaseGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_EPOCH_EVENT] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DARK_RUNED_CHEST:
        case GO_DARK_RUNED_CHEST_H:
            m_uiDarkRunedChestGUID = pGo->GetGUID();
            break;
    }
}

void instance_culling_of_stratholme::UpdateQuestCredit()
{
    Map::PlayerList const& players = instance->GetPlayers();
    if (!players.isEmpty())
    {
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                pPlayer->KilledMonsterCredit(NPC_CRATES_BUNNY);
        }
    }
}

void instance_culling_of_stratholme::DoChromieHurrySpeech()
{
    if (Creature* pChromie = instance->GetCreature(m_uiChromieEntranceGUID))
    {
        Map::PlayerList const& players = instance->GetPlayers();
        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* pPlayer = itr->getSource())
                    DoScriptText(SAY_CHROMIE_HURRY, pChromie, pPlayer);
            }
        }
    }
}

void instance_culling_of_stratholme::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_GRAIN_EVENT:
            m_auiEncounter[TYPE_GRAIN_EVENT] = uiData;
            if (uiData == SPECIAL)
                DoUpdateWorldState(WORLD_STATE_CRATES, 1);
            else if (uiData == IN_PROGRESS)
            {
                if (m_uiGrainCrateCount >= 5)
                    return;

                ++m_uiGrainCrateCount;
                DoUpdateWorldState(WORLD_STATE_CRATES_COUNT, m_uiGrainCrateCount);

                if (m_uiGrainCrateCount == 5)
                {
                    UpdateQuestCredit();
                    m_uiRemoveCrateStateTimer = 20000;
                    SetData(TYPE_GRAIN_EVENT, DONE);
                }
            }
            break;
        case TYPE_ARTHAS_INTRO_EVENT:
            m_auiEncounter[TYPE_ARTHAS_INTRO_EVENT] = uiData;
            break;
        case TYPE_ARTHAS_ESCORT_EVENT:
            m_auiEncounter[TYPE_ARTHAS_ESCORT_EVENT] = uiData;
            break;
        case TYPE_MEATHOOK_EVENT:
            m_auiEncounter[TYPE_MEATHOOK_EVENT] = uiData;
            if (uiData == DONE)
                SetData(TYPE_SALRAMM_EVENT, IN_PROGRESS);
            break;
        case TYPE_SALRAMM_EVENT:
            m_auiEncounter[TYPE_SALRAMM_EVENT] = uiData;
            if (uiData == DONE)
                DoUpdateWorldState(WORLD_STATE_WAVE, 0);    // Remove WaveCounter
            break;
        case TYPE_EPOCH_EVENT:
            m_auiEncounter[TYPE_EPOCH_EVENT] = uiData;
            break;
        case TYPE_MALGANIS_EVENT:
            m_auiEncounter[TYPE_MALGANIS_EVENT] = uiData;
            if (uiData == DONE)
            {
                DoRespawnGameObject(m_uiDarkRunedChestGUID, 30*MINUTE);
                DoSpawnChromieIfNeeded();
            }
            break;
        case TYPE_INFINITE_CORRUPTER_TIME:
            m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME] = uiData;
            if (!uiData)
            {
                DoUpdateWorldState(WORLD_STATE_TIME, 0);    // Remove Timer
                DoUpdateWorldState(WORLD_STATE_TIME_COUNTER, 0);
            }
            else
                DoUpdateWorldState(WORLD_STATE_TIME_COUNTER, uiData/(MINUTE*IN_MILLISECONDS));
            break;
        case TYPE_INFINITE_CORRUPTER:
            m_auiEncounter[TYPE_INFINITE_CORRUPTER] = uiData;
            switch(uiData)
            {
                case IN_PROGRESS:
                    if (!m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME])
                        SetData(TYPE_INFINITE_CORRUPTER_TIME, MINUTE*25*IN_MILLISECONDS);
                    DoUpdateWorldState(WORLD_STATE_TIME, 1);// Show Timer
                    break;
                case DONE:
                    SetData(TYPE_INFINITE_CORRUPTER_TIME, 0);
                    break;
                case SPECIAL:
                    DoChromieHurrySpeech();
                    break;
                case FAIL:
                    SetData(TYPE_INFINITE_CORRUPTER_TIME, 0);
                    if (Creature* pCorrupter = instance->GetCreature(m_uiCorrupterGUID))
                        if (pCorrupter->isAlive())
                            pCorrupter->ForcedDespawn();
                    break;
            }
            break;
    }

    if (uiData == DONE || (uiType == TYPE_INFINITE_CORRUPTER && uiData == FAIL))
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_culling_of_stratholme::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (i != TYPE_INFINITE_CORRUPTER_TIME)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }
    }

    // If already started counting down time, the event is "in progress"
    if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME])
        m_auiEncounter[TYPE_INFINITE_CORRUPTER] = IN_PROGRESS;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_culling_of_stratholme::OnPlayerEnter(Player* pPlayer)
{
    if (instance->GetPlayersCountExceptGMs() == 0)
    {
        DoSpawnArthasIfNeeded();
        DoSpawnChromieIfNeeded();

        // Show World States if needed, TODO verify if needed and if this is the right way
        if (m_auiEncounter[TYPE_GRAIN_EVENT] == IN_PROGRESS || m_auiEncounter[TYPE_GRAIN_EVENT] == SPECIAL)
            DoUpdateWorldState(WORLD_STATE_CRATES, 1);      // Show Crates Counter
        else
            DoUpdateWorldState(WORLD_STATE_CRATES, 0);      // Remove Crates Counter

        if (m_auiEncounter[TYPE_MEATHOOK_EVENT] == IN_PROGRESS)
            DoUpdateWorldState(WORLD_STATE_WAVE, 1);        // Add WaveCounter
        else if (m_auiEncounter[TYPE_SALRAMM_EVENT] == IN_PROGRESS)
            DoUpdateWorldState(WORLD_STATE_WAVE, 6);        // Add WaveCounter
        else
            DoUpdateWorldState(WORLD_STATE_WAVE, 0);        // Remove WaveCounter

        if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME])
            DoUpdateWorldState(WORLD_STATE_TIME, 1);        // Show Timer
        else
            DoUpdateWorldState(WORLD_STATE_TIME, 0);        // Remove Timer
    }
}

uint32 instance_culling_of_stratholme::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_GRAIN_EVENT:             return m_auiEncounter[0];
        case TYPE_ARTHAS_INTRO_EVENT:      return m_auiEncounter[1];
        case TYPE_MEATHOOK_EVENT:          return m_auiEncounter[2];
        case TYPE_SALRAMM_EVENT:           return m_auiEncounter[3];
        case TYPE_EPOCH_EVENT:             return m_auiEncounter[4];
        case TYPE_ARTHAS_ESCORT_EVENT:     return m_auiEncounter[5];
        case TYPE_MALGANIS_EVENT:          return m_auiEncounter[6];
        case TYPE_INFINITE_CORRUPTER_TIME: return m_auiEncounter[7];
        case TYPE_INFINITE_CORRUPTER:      return m_auiEncounter[8];
        default: return 0;
    }
}

uint64 instance_culling_of_stratholme::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_CHROMIE_INN:              return m_uiChromieInnGUID;
        case NPC_CHROMIE_ENTRANCE:         return m_uiChromieEntranceGUID;
        case NPC_CHROMIE_END:              return m_uiChromieEndGUID;
        case NPC_HOURGLASS:                return m_uiHourglassGUID;
        case NPC_ARTHAS:                   return m_uiArthasGUID;
        case NPC_MEATHOOK:                 return m_uiMeathookGUID;
        case NPC_SALRAMM_THE_FLESHCRAFTER: return m_uiSalrammGUID;
        case NPC_CHRONO_LORD_EPOCH:        return m_uiEpochGUID;
        case NPC_MALGANIS:                 return m_uiMalganisGUID;
        case NPC_INFINITE_CORRUPTER:       return m_uiCorrupterGUID;
        case NPC_MICHAEL_BELFAST:          return m_uiBelfastGUID;
        case NPC_HEARTHSINGER_FORRESTEN:   return m_uiForrestenGUID;
        case NPC_FRAS_SIABI:               return m_uiSiabiGUID;
        case NPC_FOOTMAN_JAMES:            return m_uiJamesGUID;
        case NPC_MAL_CORRICKS:             return m_uiCorricksGUID;
        case NPC_GRYAN_STOUTMANTLE:        return m_uiStoutmantleGUID;
        case NPC_ROGER_OWENS:              return m_uiOwensGUID;
        case NPC_SERGEANT_MORIGAN:         return m_uiMoriganGUID;
        case NPC_JENA_ANDERSON:            return m_uiAndersonGUID;
        case NPC_MALCOM_MOORE:             return m_uiMooreGUID;
        case NPC_BARTLEBY_BATTSON:         return m_uiBattsonGUID;
        case NPC_PATRICIA_O_REILLY:        return m_uiOReillyGUID;
        case GO_DOOR_BOOKCASE:             return m_uiDoorBookcaseGUID;
        default: return 0;
    }
}

uint8 instance_culling_of_stratholme::GetInstancePosition()
{
    if (m_auiEncounter[TYPE_MALGANIS_EVENT] == DONE)
        return POS_INSTANCE_FINISHED;
    else if (m_auiEncounter[TYPE_ARTHAS_ESCORT_EVENT] == DONE)
        return POS_ARTHAS_MALGANIS;
    else if (m_auiEncounter[TYPE_EPOCH_EVENT] == DONE)
        return POS_ARTHAS_ESCORTING;
    else if (m_auiEncounter[TYPE_SALRAMM_EVENT] == DONE)
        return POS_ARTHAS_TOWNHALL;
    else if (m_auiEncounter[TYPE_MEATHOOK_EVENT] == DONE)
        return POS_ARTHAS_WAVES;
    else if (m_auiEncounter[TYPE_ARTHAS_INTRO_EVENT] == DONE)
        return POS_ARTHAS_WAVES;
    else if (m_auiEncounter[TYPE_GRAIN_EVENT] == DONE)
        return POS_ARTHAS_INTRO;
    else
        return 0;
}

static bool sortFromEastToWest(Creature* pFirst, Creature* pSecond)
{
    return pFirst && pSecond && pFirst->GetPositionY() < pSecond->GetPositionY();
}

static bool sortFromSouthToNorth(Creature* pFirst, Creature* pSecond)
{
    return pFirst && pSecond && pFirst->GetPositionX() < pSecond->GetPositionX();
}

void instance_culling_of_stratholme::GetCratesBunnyOrderedList(std::list<Creature*> &lList)
{
    std::list<Creature*> lCratesBunnyList;
    for (std::list<uint64>::const_iterator itr = m_luiCratesBunnyGUIDs.begin(); itr != m_luiCratesBunnyGUIDs.end(); itr++)
    {
        if (Creature* pBunny = instance->GetCreature(*itr))
            lCratesBunnyList.push_back(pBunny);
    }
    if (lCratesBunnyList.empty())
        return;

    lCratesBunnyList.sort(sortFromEastToWest);
    lList = lCratesBunnyList;
}

Creature* instance_culling_of_stratholme::GetStratIntroFootman()
{
    std::list<Creature*> lFootmanList;
    for (std::list<uint64>::const_iterator itr = m_luiFootmanGUIDs.begin(); itr != m_luiFootmanGUIDs.end(); itr++)
    {
        if (Creature* pFootman = instance->GetCreature(*itr))
            lFootmanList.push_back(pFootman);
    }

    if (lFootmanList.empty())
        return NULL;
    else
    {
        lFootmanList.sort(sortFromSouthToNorth);
        return *lFootmanList.begin();
    }
}

void instance_culling_of_stratholme::GetResidentOrderedList(std::list<Creature*> &lList)
{
    std::list<Creature*> lResidentList;
    for (std::list<uint64>::const_iterator itr = m_luiResidentGUIDs.begin(); itr != m_luiResidentGUIDs.end(); itr++)
    {
        if (Creature* pResident = instance->GetCreature(*itr))
            lResidentList.push_back(pResident);
    }
    if (lResidentList.empty())
        return;

    lResidentList.sort(sortFromSouthToNorth);
    lList = lResidentList;
}

void instance_culling_of_stratholme::ArthasJustDied()
{
    m_uiArthasRespawnTimer = 10000;                         // TODO, could be instant
}

void instance_culling_of_stratholme::DoSpawnArthasIfNeeded()
{
    Creature* pArthas = instance->GetCreature(m_uiArthasGUID);
    if (pArthas && pArthas->isAlive())
        return;

    uint8 uiPosition = GetInstancePosition();
    if (uiPosition && uiPosition <= MAX_ARTHAS_SPAWN_POS)
    {
        if (Player* pPlayer = GetPlayerInMap())
            pPlayer->SummonCreature(NPC_ARTHAS, m_aArthasSpawnLocs[uiPosition-1].m_fX, m_aArthasSpawnLocs[uiPosition-1].m_fY, m_aArthasSpawnLocs[uiPosition-1].m_fZ, m_aArthasSpawnLocs[uiPosition-1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }
}

// Atm here only new Chromies are spawned - despawning depends on Mangos featuring such a thing
// The hourglass also is not yet spawned/ relocated.
void instance_culling_of_stratholme::DoSpawnChromieIfNeeded()
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    if (GetInstancePosition() == POS_INSTANCE_FINISHED)
    {
        Creature* pChromie = instance->GetCreature(m_uiChromieEndGUID);
        if (!pChromie)
            pPlayer->SummonCreature(NPC_CHROMIE_END, m_aChromieSpawnLocs[1].m_fX, m_aChromieSpawnLocs[1].m_fY, m_aChromieSpawnLocs[1].m_fZ, m_aChromieSpawnLocs[1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }
    else if (GetInstancePosition() >= POS_ARTHAS_INTRO)
    {
        Creature* pChromie = instance->GetCreature(m_uiChromieEntranceGUID);
        if (!pChromie)
            pPlayer->SummonCreature(NPC_CHROMIE_ENTRANCE, m_aChromieSpawnLocs[0].m_fX, m_aChromieSpawnLocs[0].m_fY, m_aChromieSpawnLocs[0].m_fZ, m_aChromieSpawnLocs[0].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }
}

void instance_culling_of_stratholme::Update(uint32 uiDiff)
{
    // 25min Run - decrease time, update worldstate every ~20s
    // as the time is always saved by m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME], there is no need for an extra timer
    if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME])
    {
        if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME] <= uiDiff)
            SetData(TYPE_INFINITE_CORRUPTER, FAIL);
        else
        {
            m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME] -= uiDiff;
            if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME]/IN_MILLISECONDS % 20 == 0)
                SetData(TYPE_INFINITE_CORRUPTER_TIME, m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME]);
        }

        // This part is needed for a small "hurry up guys" note, TODO, verify 20min
        if (m_auiEncounter[TYPE_INFINITE_CORRUPTER] == IN_PROGRESS && m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME] <= 24*MINUTE*IN_MILLISECONDS)
            SetData(TYPE_INFINITE_CORRUPTER, SPECIAL);
    }

    // Small Timer, to remove Grain-Crate WorldState and Spawn Second Chromie
    if (m_uiRemoveCrateStateTimer)
    {
        if (m_uiRemoveCrateStateTimer <= uiDiff)
        {
            DoUpdateWorldState(WORLD_STATE_CRATES, 0);
            DoSpawnChromieIfNeeded();
            m_uiRemoveCrateStateTimer = 0;
        }
        else
            m_uiRemoveCrateStateTimer -= uiDiff;
    }

    // Respawn Arthas after some time
    if (m_uiArthasRespawnTimer)
    {
        if (m_uiArthasRespawnTimer <= uiDiff)
        {
            DoSpawnArthasIfNeeded();
            m_uiArthasRespawnTimer = 0;
        }
        else
            m_uiArthasRespawnTimer -= uiDiff;
    }
}

InstanceData* GetInstanceData_instance_culling_of_stratholme(Map* pMap)
{
    return new instance_culling_of_stratholme(pMap);
}

void AddSC_instance_culling_of_stratholme()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_culling_of_stratholme";
    pNewScript->GetInstanceData = &GetInstanceData_instance_culling_of_stratholme;
    pNewScript->RegisterSelf();
}
