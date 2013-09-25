/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: instance_ulduar
SD%Complete:
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_PRE_LEVIATHAN_1                     = -1603239,
    SAY_PRE_LEVIATHAN_2                     = -1603240,
    SAY_PRE_LEVIATHAN_3                     = -1603241,
};

static const DialogueEntry aUlduarDialogue[] =
{
    {SAY_PRE_LEVIATHAN_1,       NPC_BRONZEBEARD_RADIO,      7000},
    {SAY_PRE_LEVIATHAN_2,       NPC_BRONZEBEARD_RADIO,      5000},
    {SAY_PRE_LEVIATHAN_3,       NPC_BRONZEBEARD_RADIO,      2000},
    {NPC_LEVIATHAN,             0,                          0},
    {0, 0, 0}
};

struct sSpawnLocation
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static sSpawnLocation m_aKeepersSpawnLocs[] =
{
    {2036.892f, 25.621f, 411.358f, 3.83f},                  // Freya
    {1939.215f, 42.677f, 411.355f, 5.31f},                  // Mimiron
    {1939.195f, -90.662f, 411.357f, 1.06f},                 // Hodir
    {2036.674f, -73.814f, 411.355f, 2.51f},                 // Thorim
};

instance_ulduar::instance_ulduar(Map* pMap) : ScriptedInstance(pMap), DialogueHelper(aUlduarDialogue),
    m_uiShatterAchievTimer(0),
    m_uiGauntletStatus(0),
    m_uiActiveTowers(0)
{
    Initialize();
}

void instance_ulduar::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiHardBoss, 0, sizeof(m_auiHardBoss));
    memset(&m_auiUlduarKeepers, 0, sizeof(m_auiUlduarKeepers));
    memset(&m_auiUlduarTowers, 0, sizeof(m_auiUlduarTowers));

    InitializeDialogueHelper(this);

    for (uint8 i = 0; i < MAX_SPECIAL_ACHIEV_CRITS; ++i)
        m_abAchievCriteria[i] = false;
}

void instance_ulduar::OnPlayerEnter(Player* pPlayer)
{
    // spawn Flame Leviathan if necessary
    if (GetData(TYPE_LEVIATHAN) == SPECIAL || GetData(TYPE_LEVIATHAN) == FAIL)
    {
        if (!GetSingleCreatureFromStorage(NPC_LEVIATHAN, true))
        {
            pPlayer->SummonCreature(NPC_LEVIATHAN, afLeviathanMovePos[0], afLeviathanMovePos[1], afLeviathanMovePos[2], afLeviathanMovePos[3], TEMPSUMMON_DEAD_DESPAWN, 0, true);
            DoCallLeviathanHelp();
        }
    }

    // spawn Brann at the archivum if necessary
    if (GetData(TYPE_ASSEMBLY) == DONE)
    {
        if (!GetSingleCreatureFromStorage(NPC_BRANN_ARCHIVUM, true))
        {
            pPlayer->SummonCreature(NPC_BRANN_ARCHIVUM, afBrannArchivumSpawnPos[0], afBrannArchivumSpawnPos[1], afBrannArchivumSpawnPos[2], afBrannArchivumSpawnPos[3], TEMPSUMMON_DEAD_DESPAWN, 0, true);
            pPlayer->SummonCreature(instance->IsRegularDifficulty() ? NPC_PROSPECTOR_DOREN : NPC_PROSPECTOR_DOREN_H, afProspectorSpawnPos[0], afProspectorSpawnPos[1], afProspectorSpawnPos[2], afProspectorSpawnPos[3], TEMPSUMMON_DEAD_DESPAWN, 0, true);
        }
    }
}

bool instance_ulduar::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }
    return false;
}

void instance_ulduar::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_LEVIATHAN:
        case NPC_EXPLORER_DELLORAH:
        case NPC_BRANN_BRONZEBEARD:
        case NPC_ORBITAL_SUPPORT:
        case NPC_RAZORSCALE:
        case NPC_EXPEDITION_COMMANDER:
        case NPC_XT002:
        case NPC_HEART_DECONSTRUCTOR:

        case NPC_STEELBREAKER:
        case NPC_MOLGEIM:
        case NPC_BRUNDIR:
        case NPC_KOLOGARN:
        case NPC_RIGHT_ARM:
        case NPC_LEFT_ARM:
        case NPC_AURIAYA:
        case NPC_FERAL_DEFENDER:
        case NPC_BRANN_ARCHIVUM:

        case NPC_LEVIATHAN_MK:
        case NPC_RUNIC_COLOSSUS:
        case NPC_RUNE_GIANT:
        case NPC_JORMUNGAR_BEHEMOTH:
        case NPC_ELDER_BRIGHTLEAF:
        case NPC_ELDER_IRONBRACH:
        case NPC_ELDER_STONEBARK:
        case NPC_VEZAX:
        case NPC_SARONITE_ANIMUS:
        case NPC_YOGGSARON:
        case NPC_SARA:
        case NPC_YOGG_BRAIN:
        case NPC_ALGALON:
            break;

        case NPC_MIMIRON:
            if (m_auiEncounter[TYPE_MIMIRON] == DONE)
                SpawnFriendlyKeeper(NPC_MIMIRON_IMAGE);
            break;
        case NPC_HODIR:
            if (m_auiEncounter[TYPE_HODIR] == DONE)
                SpawnFriendlyKeeper(NPC_HODIR_IMAGE);
            break;
        case NPC_THORIM:
            if (m_auiEncounter[TYPE_THORIM] == DONE)
                SpawnFriendlyKeeper(NPC_THORIM_IMAGE);
            break;
        case NPC_FREYA:
            if (m_auiEncounter[TYPE_FREYA] == DONE)
                SpawnFriendlyKeeper(NPC_FREYA_IMAGE);
            break;

        case NPC_ULDUAR_COLOSSUS:
            if (pCreature->GetPositionX() > 300.0f)
                m_sColossusGuidSet.insert(pCreature->GetObjectGuid());
            return;
        case NPC_EXPEDITION_DEFENDER:
            m_lDefendersGuids.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_EXPEDITION_ENGINEER:
            m_lEngineersGuids.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_EXPEDITION_TRAPPER:
            m_lTrappersGuids.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_RAZORSCALE_CONTROLLER:
            // sort the controllers which are assigned to harpoons and allow the central one into the mail guid store
            if (pCreature->GetPositionY() > -145.0f)
            {
                m_lHarpoonDummyGuids.push_back(pCreature->GetObjectGuid());
                return;
            }
            break;
        case NPC_XT_TOY_PILE:
            m_vToyPileGuidVector.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_RUBBLE_STALKER:
            if (pCreature->GetPositionY() > -10.0f)
                m_rightKoloStalkerGuid = pCreature->GetObjectGuid();
            else
                m_leftKoloStalkerGuid = pCreature->GetObjectGuid();
            return;

        default:
            return;
    }
    m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
}

void instance_ulduar::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
            // -----------------    Doors & Other   -----------------
            // The siege
        case GO_SHIELD_WALL:
            break;
        case GO_LIGHTNING_DOOR:
            if (m_auiEncounter[TYPE_LEVIATHAN] == SPECIAL || m_auiEncounter[TYPE_LEVIATHAN] == FAIL)
                pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_LEVIATHAN_GATE:
            if (m_auiEncounter[TYPE_LEVIATHAN] != NOT_STARTED)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_XT002_GATE:
            pGo->SetGoState(GO_STATE_READY);
            if (m_auiEncounter[TYPE_XT002] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            if (m_auiEncounter[TYPE_LEVIATHAN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HODIR_CRYSTAL:
            if (m_auiUlduarTowers[0] == FAIL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_THORIM_CRYSTAL:
            if (m_auiUlduarTowers[1] == FAIL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_FREYA_CRYSTAL:
            if (m_auiUlduarTowers[2] == FAIL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_MIMIRON_CRYSTAL:
            if (m_auiUlduarTowers[3] == FAIL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

            // Archivum
        case GO_IRON_ENTRANCE_DOOR:
            break;
        case GO_ARCHIVUM_DOOR:
            if (m_auiEncounter[TYPE_ASSEMBLY])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
            // Celestial Planetarium
        case GO_CELESTIAL_ACCES:
        case GO_CELESTIAL_DOOR:
        case GO_UNIVERSE_FLOOR_CELESTIAL:
        case GO_AZEROTH_GLOBE:
            break;
            // Shattered Hallway
        case GO_KOLOGARN_BRIDGE:
            if (m_auiEncounter[TYPE_KOLOGARN] == DONE)
                pGo->SetGoState(GO_STATE_READY);
            break;

            // -----------------    The Keepers    -----------------
            // Hodir
        case GO_HODIR_EXIT:
            if (m_auiEncounter[TYPE_HODIR])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HODIR_ICE_WALL:
            if (m_auiEncounter[TYPE_HODIR])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HODIR_ENTER:
            break;
            // Mimiron
        case G0_MIMIRON_BUTTON:
            if (m_auiEncounter[TYPE_MIMIRON] == NOT_STARTED)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;
        case GO_MIMIRON_DOOR_1:
        case GO_MIMIRON_DOOR_2:
        case GO_MIMIRON_DOOR_3:
        case GO_MIMIRON_ELEVATOR:
        case GO_MIMIRON_TEL1:
        case GO_MIMIRON_TEL2:
        case GO_MIMIRON_TEL3:
        case GO_MIMIRON_TEL4:
        case GO_MIMIRON_TEL5:
        case GO_MIMIRON_TEL6:
        case GO_MIMIRON_TEL7:
        case GO_MIMIRON_TEL8:
        case GO_MIMIRON_TEL9:
            // Thorim
        case GO_DARK_IRON_PORTCULIS:
        case GO_RUNED_STONE_DOOR:
        case GO_THORIM_STONE_DOOR:
        case GO_LIGHTNING_FIELD:
            break;
        case GO_DOOR_LEVER:
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;

            // Prison
        case GO_ANCIENT_GATE:
            DoOpenMadnessDoorIfCan();
            break;
        case GO_VEZAX_GATE:
            pGo->SetGoState(GO_STATE_READY);
            if (m_auiEncounter[TYPE_VEZAX])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_YOGG_GATE:
        case GO_BRAIN_DOOR1:
        case GO_BRAIN_DOOR2:
        case GO_BRAIN_DOOR3:
            break;

            // -----------------    Chests    -----------------
            // Kologarn
        case GO_CACHE_OF_LIVING_STONE_10:
        case GO_CACHE_OF_LIVING_STONE_25:

            // Hodir
        case GO_CACHE_OF_WINTER_10:
        case GO_CACHE_OF_WINTER_25:
        case GO_CACHE_OF_RARE_WINTER_10:
        case GO_CACHE_OF_RARE_WINTER_25:

            // Thorim
        case GO_CACHE_OF_STORMS_10:
        case GO_CACHE_OF_STORMS_25:
        case GO_CACHE_OF_STORMS_10_H:
        case GO_CACHE_OF_STORMS_25_H:

            // Mimiron
        case GO_CACHE_OF_INOV_10:
        case GO_CACHE_OF_INOV_25:
        case GO_CACHE_OF_INOV_10_H:
        case GO_CACHE_OF_INOV_25_H:

            // Alagon
        case GO_GIFT_OF_OBSERVER_10:
        case GO_GIFT_OF_OBSERVER_25:
            break;

        case GO_BROKEN_HARPOON:
            m_vBrokenHarpoonsGuids.push_back(pGo->GetObjectGuid());
            return;
        case GO_HARPOON_GUN_1:
        case GO_HARPOON_GUN_2:
        case GO_HARPOON_GUN_3:
        case GO_HARPOON_GUN_4:
            m_lRepairedHarpoonsGuids.push_back(pGo->GetObjectGuid());
            return;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

// Used in order to unlock the door to Vezax
void instance_ulduar::DoOpenMadnessDoorIfCan()
{
    if (m_auiEncounter[TYPE_MIMIRON] == DONE && m_auiEncounter[TYPE_HODIR] == DONE && m_auiEncounter[TYPE_THORIM] == DONE && m_auiEncounter[TYPE_FREYA] == DONE)
    {
        if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_ANCIENT_GATE))
            pDoor->SetGoState(GO_STATE_ACTIVE);
    }
}

void instance_ulduar::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_LEVIATHAN:
            m_auiEncounter[uiType] = uiData;
            if (uiData != SPECIAL)
                DoUseDoorOrButton(GO_SHIELD_WALL);
            if (uiData == IN_PROGRESS)
            {
                // make sure that the Lightning door is closed when engaged in combat
                if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_LIGHTNING_DOOR))
                {
                    if (pDoor->GetGoState() != GO_STATE_READY)
                        DoUseDoorOrButton(GO_LIGHTNING_DOOR);
                }

                SetSpecialAchievementCriteria(TYPE_ACHIEV_SHUTOUT, true);
            }
            else if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_XT002_GATE);
                DoUseDoorOrButton(GO_LIGHTNING_DOOR);
            }
            else if (uiData == FAIL)
                DoCallLeviathanHelp();
            break;
        case TYPE_IGNIS:
            m_auiEncounter[uiType] = uiData;
            if (uiData == IN_PROGRESS)
            {
                DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_START_IGNIS_ID);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_SHATTERED, false);
            }
            break;
        case TYPE_RAZORSCALE:
            if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEV_QUICK_SHAVE, true);
            else if (uiData == FAIL)
            {
                // reset the commander
                if (Creature* pCommander = GetSingleCreatureFromStorage(NPC_EXPEDITION_COMMANDER))
                    pCommander->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                // reset all creatures
                for (GuidList::const_iterator itr = m_lDefendersGuids.begin(); itr != m_lDefendersGuids.end(); ++itr)
                {
                    if (Creature* pDefender = instance->GetCreature(*itr))
                    {
                        if (!pDefender->isAlive())
                            pDefender->Respawn();
                        else
                            pDefender->GetMotionMaster()->MoveTargetedHome();
                    }
                }
                for (GuidList::const_iterator itr = m_lEngineersGuids.begin(); itr != m_lEngineersGuids.end(); ++itr)
                {
                    if (Creature* pEngineer = instance->GetCreature(*itr))
                    {
                        if (!pEngineer->isAlive())
                            pEngineer->Respawn();
                        else
                            pEngineer->GetMotionMaster()->MoveTargetedHome();
                    }
                }
                for (GuidList::const_iterator itr = m_lTrappersGuids.begin(); itr != m_lTrappersGuids.end(); ++itr)
                {
                    if (Creature* pTrapper = instance->GetCreature(*itr))
                    {
                        if (!pTrapper->isAlive())
                            pTrapper->Respawn();
                        else
                            pTrapper->GetMotionMaster()->MoveTargetedHome();
                    }
                }
                for (GuidList::const_iterator itr = m_lHarpoonDummyGuids.begin(); itr != m_lHarpoonDummyGuids.end(); ++itr)
                {
                    if (Creature* pHarpoon = instance->GetCreature(*itr))
                        pHarpoon->InterruptNonMeleeSpells(false);
                }

                // reset Harpoons: respawn the broken ones and despawn the repaired ones
                for (GuidVector::const_iterator itr = m_vBrokenHarpoonsGuids.begin(); itr != m_vBrokenHarpoonsGuids.end(); ++itr)
                {
                    if (GameObject* pHarpoon = instance->GetGameObject(*itr))
                    {
                        if (!pHarpoon->isSpawned())
                            pHarpoon->Respawn();
                    }
                }
                for (GuidList::const_iterator itr = m_lRepairedHarpoonsGuids.begin(); itr != m_lRepairedHarpoonsGuids.end(); ++itr)
                {
                    if (GameObject* pHarpoon = instance->GetGameObject(*itr))
                    {
                        if (pHarpoon->isSpawned())
                            pHarpoon->SetLootState(GO_JUST_DEACTIVATED);
                    }
                }
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_XT002:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_XT002_GATE);
            if (uiData == IN_PROGRESS)
            {
                DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_START_XT002_ID);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_NERF_ENG, true);
            }
            break;
        case TYPE_ASSEMBLY:
            // Don't set the same encounter data twice
            if (uiData == m_auiEncounter[uiType])
                return;
            m_auiEncounter[uiType] = uiData;
            // don't continue for encounter = special
            if (uiData == SPECIAL)
                return;
            DoUseDoorOrButton(GO_IRON_ENTRANCE_DOOR);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ARCHIVUM_DOOR);

                if (Player* pPlayer = GetPlayerInMap())
                {
                    pPlayer->SummonCreature(NPC_BRANN_ARCHIVUM, afBrannArchivumSpawnPos[0], afBrannArchivumSpawnPos[1], afBrannArchivumSpawnPos[2], afBrannArchivumSpawnPos[3], TEMPSUMMON_DEAD_DESPAWN, 0, true);
                    pPlayer->SummonCreature(instance->IsRegularDifficulty() ? NPC_PROSPECTOR_DOREN : NPC_PROSPECTOR_DOREN_H, afProspectorSpawnPos[0], afProspectorSpawnPos[1], afProspectorSpawnPos[2], afProspectorSpawnPos[3], TEMPSUMMON_DEAD_DESPAWN, 0, true);
                }
            }
            else if (uiData == IN_PROGRESS)
            {
                SetSpecialAchievementCriteria(TYPE_ACHIEV_BRUNDIR, true);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_MOLGEIM, true);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_STEELBREAKER, true);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_STUNNED, true);
            }
            break;
        case TYPE_KOLOGARN:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoToggleGameObjectFlags(instance->IsRegularDifficulty() ? GO_CACHE_OF_LIVING_STONE_10 : GO_CACHE_OF_LIVING_STONE_25, GO_FLAG_NO_INTERACT, false);
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_LIVING_STONE_10 : GO_CACHE_OF_LIVING_STONE_25, 30 * MINUTE);
                DoUseDoorOrButton(GO_KOLOGARN_BRIDGE);
            }
            else if (uiData == IN_PROGRESS)
            {
                SetSpecialAchievementCriteria(TYPE_ACHIEV_RUBBLE, false);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_DISARMED, false);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_LOOKS_KILL, true);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_OPEN_ARMS, true);
            }
            break;
        case TYPE_AURIAYA:
            m_auiEncounter[uiType] = uiData;
            if (uiData == IN_PROGRESS)
            {
                SetSpecialAchievementCriteria(TYPE_ACHIEV_CAT_LADY, true);
                SetSpecialAchievementCriteria(TYPE_ACHIEV_NINE_LIVES, false);
            }
            break;
            // Keepers
        case TYPE_MIMIRON:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_MIMIRON_DOOR_1);
            DoUseDoorOrButton(GO_MIMIRON_DOOR_2);
            DoUseDoorOrButton(GO_MIMIRON_DOOR_3);
            if (uiData == DONE)
            {
                if (GetData(TYPE_MIMIRON_HARD) != DONE)
                    DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_INOV_10 : GO_CACHE_OF_INOV_25, 30 * MINUTE);
                SpawnFriendlyKeeper(NPC_MIMIRON_IMAGE);
            }
            break;
        case TYPE_HODIR:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_HODIR_ENTER);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_HODIR_ICE_WALL);
                DoUseDoorOrButton(GO_HODIR_EXIT);
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_WINTER_10 : GO_CACHE_OF_WINTER_25, 30 * MINUTE);
                SpawnFriendlyKeeper(NPC_HODIR_IMAGE);
            }
            break;
        case TYPE_THORIM:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_LIGHTNING_FIELD);
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_DARK_IRON_PORTCULIS);
            if (uiData == DONE)
            {
                if (GetData(TYPE_THORIM_HARD) != DONE)
                    DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_STORMS_10 : GO_CACHE_OF_STORMS_25, 30 * MINUTE);
                SpawnFriendlyKeeper(NPC_THORIM_IMAGE);
            }
            break;
        case TYPE_FREYA:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                SpawnFriendlyKeeper(NPC_FREYA_IMAGE);
            break;
            // Prison
        case TYPE_VEZAX:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(GO_VEZAX_GATE);
            break;
        case TYPE_YOGGSARON:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_YOGG_GATE);
            break;

            // Celestial Planetarium
        case TYPE_ALGALON:
            m_auiEncounter[uiType] = uiData;
            // TODO: need to find the proper way to use these
            DoUseDoorOrButton(GO_CELESTIAL_DOOR);
            DoUseDoorOrButton(GO_UNIVERSE_FLOOR_CELESTIAL);
            if (uiData == DONE)
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_GIFT_OF_OBSERVER_10 : GO_GIFT_OF_OBSERVER_25, 30 * MINUTE);
            break;

            // Hard modes
        case TYPE_LEVIATHAN_HARD:
            m_auiHardBoss[0] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_XT002_HARD:
            m_auiHardBoss[1] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_HODIR_HARD:
            m_auiHardBoss[2] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_RARE_WINTER_10 : GO_CACHE_OF_RARE_WINTER_25, 30 * MINUTE);
            break;
        case TYPE_THORIM_HARD:
            m_auiHardBoss[3] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_STORMS_10_H : GO_CACHE_OF_STORMS_25_H, 30 * MINUTE);
            break;
        case TYPE_MIMIRON_HARD:
            m_auiHardBoss[4] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CACHE_OF_INOV_10_H : GO_CACHE_OF_INOV_25_H, 30 * MINUTE);
            break;
        case TYPE_VEZAX_HARD:
            m_auiHardBoss[5] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_YOGGSARON_HARD:
            m_auiHardBoss[6] = uiData;                      // TODO: add extra loot
            break;

            // Ulduar keepers
        case TYPE_KEEPER_HODIR:
            m_auiUlduarKeepers[0] = uiData;
            break;
        case TYPE_KEEPER_THORIM:
            m_auiUlduarKeepers[1] = uiData;
            break;
        case TYPE_KEEPER_FREYA:
            m_auiUlduarKeepers[2] = uiData;
            break;
        case TYPE_KEEPER_MIMIRON:
            m_auiUlduarKeepers[3] = uiData;
            break;

            // Ulduar towers
        case TYPE_TOWER_HODIR:
            if (m_auiUlduarTowers[0] == uiData)
                return;
            if (uiData == FAIL)
                DoUseDoorOrButton(GO_HODIR_CRYSTAL);
            m_auiUlduarTowers[0] = uiData;
            break;
        case TYPE_TOWER_THORIM:
            if (m_auiUlduarTowers[1] == uiData)
                return;
            if (uiData == FAIL)
                DoUseDoorOrButton(GO_THORIM_CRYSTAL);
            m_auiUlduarTowers[1] = uiData;
            break;
        case TYPE_TOWER_FREYA:
            if (m_auiUlduarTowers[2] == uiData)
                return;
            if (uiData == FAIL)
                DoUseDoorOrButton(GO_FREYA_CRYSTAL);
            m_auiUlduarTowers[2] = uiData;
            break;
        case TYPE_TOWER_MIMIRON:
            if (m_auiUlduarTowers[3] == uiData)
                return;
            if (uiData == FAIL)
                DoUseDoorOrButton(GO_MIMIRON_CRYSTAL);
            m_auiUlduarTowers[3] = uiData;
            break;

            // Other types - not saved
        case TYPE_LEVIATHAN_GAUNTLET:
            m_uiGauntletStatus = uiData;
            return;
        case TYPE_LEVIATHAN_TOWERS:
            m_uiActiveTowers = uiData;
            return;
    }

    DoOpenMadnessDoorIfCan();

    if (uiData == DONE || uiData == FAIL || uiData == SPECIAL)
    {
        OUT_SAVE_INST_DATA;

        // Save all encounters, hard bosses, keepers and teleporters
        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                   << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                   << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
                   << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11] << " "
                   << m_auiEncounter[12] << " " << m_auiEncounter[13] << " " << m_auiHardBoss[0] << " "
                   << m_auiHardBoss[1] << " " << m_auiHardBoss[2] << " " << m_auiHardBoss[2] << " "
                   << m_auiHardBoss[4] << " " << m_auiHardBoss[5] << " " << m_auiHardBoss[6] << " "
                   << m_auiUlduarKeepers[0] << " " << m_auiUlduarKeepers[1] << " " << m_auiUlduarKeepers[2] << " "
                   << m_auiUlduarKeepers[3] << " " << m_auiUlduarTowers[0] << " " << m_auiUlduarTowers[1] << " "
                   << m_auiUlduarTowers[2] << " " << m_auiUlduarTowers[3];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

bool instance_ulduar::CheckConditionCriteriaMeet(Player const* pPlayer, uint32 uiInstanceConditionId, WorldObject const* pConditionSource, uint32 conditionSourceType) const
{
    switch (uiInstanceConditionId)
    {
        case INSTANCE_CONDITION_ID_NORMAL_MODE:
        case INSTANCE_CONDITION_ID_HARD_MODE:
        case INSTANCE_CONDITION_ID_HARD_MODE_2:
        case INSTANCE_CONDITION_ID_HARD_MODE_3:
        case INSTANCE_CONDITION_ID_HARD_MODE_4:
        {
            if (!pConditionSource)
                break;

            uint32 uiCondId = 0;
            switch (pConditionSource->GetEntry())
            {
                case NPC_LEVIATHAN:
                    uiCondId = m_uiActiveTowers;
                    break;
                case NPC_XT002:
                    if (GetData(TYPE_XT002_HARD) == DONE)
                        uiCondId = 1;
                    break;
                case NPC_VEZAX:
                    if (GetData(TYPE_VEZAX_HARD) == DONE)
                        uiCondId = 1;
                    break;
            }

            return uiCondId == uiInstanceConditionId;
        }
        case INSTANCE_CONDITION_ID_ULDUAR:
        {
            if (!pConditionSource)
                break;

            // handle vehicle spell clicks - are available only after the gauntlet was started by gossip or when Leviathan is active
            return GetData(TYPE_LEVIATHAN_GAUNTLET) == IN_PROGRESS || GetData(TYPE_LEVIATHAN) == SPECIAL || GetData(TYPE_LEVIATHAN) == FAIL;
        }
    }

    script_error_log("instance_ulduar::CheckConditionCriteriaMeet called with unsupported Id %u. Called with param plr %s, src %s, condition source type %u",
                     uiInstanceConditionId, pPlayer ? pPlayer->GetGuidStr().c_str() : "NULL", pConditionSource ? pConditionSource->GetGuidStr().c_str() : "NULL", conditionSourceType);
    return false;
}

uint32 instance_ulduar::GetData(uint32 uiType) const
{
    switch (uiType)
    {
        case TYPE_LEVIATHAN:
            return m_auiEncounter[0];
        case TYPE_IGNIS:
            return m_auiEncounter[1];
        case TYPE_RAZORSCALE:
            return m_auiEncounter[2];
        case TYPE_XT002:
            return m_auiEncounter[3];
        case TYPE_ASSEMBLY:
            return m_auiEncounter[4];
        case TYPE_KOLOGARN:
            return m_auiEncounter[5];
        case TYPE_AURIAYA:
            return m_auiEncounter[6];
        case TYPE_MIMIRON:
            return m_auiEncounter[7];
        case TYPE_HODIR:
            return m_auiEncounter[8];
        case TYPE_THORIM:
            return m_auiEncounter[9];
        case TYPE_FREYA:
            return m_auiEncounter[10];
        case TYPE_VEZAX:
            return m_auiEncounter[11];
        case TYPE_YOGGSARON:
            return m_auiEncounter[12];
        case TYPE_ALGALON:
            return m_auiEncounter[13];

            // Hard modes
        case TYPE_LEVIATHAN_HARD:
            return m_auiHardBoss[0];
        case TYPE_XT002_HARD:
            return m_auiHardBoss[1];
        case TYPE_HODIR_HARD:
            return m_auiHardBoss[2];
        case TYPE_THORIM_HARD:
            return m_auiHardBoss[3];
        case TYPE_MIMIRON_HARD:
            return m_auiHardBoss[4];
        case TYPE_VEZAX_HARD:
            return m_auiHardBoss[5];
        case TYPE_YOGGSARON_HARD:
            return m_auiHardBoss[6];

            // Ulduar Keepers
        case TYPE_KEEPER_HODIR:
            return m_auiUlduarKeepers[0];
        case TYPE_KEEPER_THORIM:
            return m_auiUlduarKeepers[1];
        case TYPE_KEEPER_FREYA:
            return m_auiUlduarKeepers[2];
        case TYPE_KEEPER_MIMIRON:
            return m_auiUlduarKeepers[3];

            // Ulduar Towers
        case TYPE_TOWER_HODIR:
            return m_auiUlduarTowers[0];
        case TYPE_TOWER_THORIM:
            return m_auiUlduarTowers[1];
        case TYPE_TOWER_FREYA:
            return m_auiUlduarTowers[2];
        case TYPE_TOWER_MIMIRON:
            return m_auiUlduarTowers[3];

        case TYPE_LEVIATHAN_GAUNTLET:
            return m_uiGauntletStatus;
    }

    return 0;
}

// Spawn the friendly keepers in the central chamber
void instance_ulduar::SpawnFriendlyKeeper(uint32 uiWho)
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    switch (uiWho)
    {
        case NPC_MIMIRON_IMAGE: pPlayer->SummonCreature(NPC_MIMIRON_IMAGE, m_aKeepersSpawnLocs[1].m_fX, m_aKeepersSpawnLocs[1].m_fY, m_aKeepersSpawnLocs[1].m_fZ, m_aKeepersSpawnLocs[1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000); break;
        case NPC_HODIR_IMAGE:   pPlayer->SummonCreature(NPC_HODIR_IMAGE,   m_aKeepersSpawnLocs[2].m_fX, m_aKeepersSpawnLocs[2].m_fY, m_aKeepersSpawnLocs[2].m_fZ, m_aKeepersSpawnLocs[2].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000); break;
        case NPC_THORIM_IMAGE:  pPlayer->SummonCreature(NPC_THORIM_IMAGE,  m_aKeepersSpawnLocs[3].m_fX, m_aKeepersSpawnLocs[3].m_fY, m_aKeepersSpawnLocs[3].m_fZ, m_aKeepersSpawnLocs[3].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000); break;
        case NPC_FREYA_IMAGE:   pPlayer->SummonCreature(NPC_FREYA_IMAGE,   m_aKeepersSpawnLocs[0].m_fX, m_aKeepersSpawnLocs[0].m_fY, m_aKeepersSpawnLocs[0].m_fZ, m_aKeepersSpawnLocs[0].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000); break;
    }
}

void instance_ulduar::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_SANCTUM_SENTRY:
            if (GetData(TYPE_AURIAYA) == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEV_CAT_LADY, false);
            break;
        case NPC_ULDUAR_COLOSSUS:
            {
                if (m_sColossusGuidSet.find(pCreature->GetObjectGuid()) != m_sColossusGuidSet.end())
                    m_sColossusGuidSet.erase(pCreature->GetObjectGuid());

                // start pre Leviathan event
                if (m_sColossusGuidSet.empty())
                {
                    StartNextDialogueText(SAY_PRE_LEVIATHAN_1);
                    SetData(TYPE_LEVIATHAN, SPECIAL);
                    SetData(TYPE_LEVIATHAN_GAUNTLET, DONE);
                    pCreature->SummonCreature(NPC_LEVIATHAN, afLeviathanSpawnPos[0], afLeviathanSpawnPos[1], afLeviathanSpawnPos[2], afLeviathanSpawnPos[3], TEMPSUMMON_DEAD_DESPAWN, 0, true);
                }
            }
            break;
    }
}

void instance_ulduar::Load(const char* strIn)
{
    if (!strIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(strIn);

    std::istringstream loadStream(strIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
               >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8]
               >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11] >> m_auiEncounter[12] >> m_auiEncounter[13]
               >> m_auiHardBoss[0] >> m_auiHardBoss[1] >> m_auiHardBoss[2] >> m_auiHardBoss[3] >> m_auiHardBoss[4] >> m_auiHardBoss[5] >> m_auiHardBoss[6]
               >> m_auiUlduarKeepers[0] >> m_auiUlduarKeepers[1] >> m_auiUlduarKeepers[2] >> m_auiUlduarKeepers[3] >> m_auiUlduarTowers[0]
               >> m_auiUlduarTowers[1] >> m_auiUlduarTowers[2] >> m_auiUlduarTowers[3];

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_ulduar::SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet)
{
    if (uiType < MAX_SPECIAL_ACHIEV_CRITS)
        m_abAchievCriteria[uiType] = bIsMet;
}

bool instance_ulduar::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* /*pSource*/, Unit const* /*pTarget*/, uint32 /*uiMiscValue1 = 0*/) const
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_SARONITE_N:
        case ACHIEV_CRIT_SARONITE_H:
            return GetData(TYPE_VEZAX_HARD) == DONE;
        case ACHIEV_CRIT_CAT_LADY_N:
        case ACHIEV_CRIT_CAT_LADY_H:
            return m_abAchievCriteria[TYPE_ACHIEV_CAT_LADY];
        case ACHIEV_CRIT_NINE_LIVES_N:
        case ACHIEV_CRIT_NINE_LIVES_H:
            return m_abAchievCriteria[TYPE_ACHIEV_NINE_LIVES];
        case ACHIEV_CRIT_BRUNDIR_N:
        case ACHIEV_CRIT_BRUNDIR_H:
            if (GetData(TYPE_ASSEMBLY) == SPECIAL)
                return m_abAchievCriteria[TYPE_ACHIEV_BRUNDIR];
        case ACHIEV_CRIT_MOLGEIM_N:
        case ACHIEV_CRIT_MOLGEIM_H:
            if (GetData(TYPE_ASSEMBLY) == SPECIAL)
                return m_abAchievCriteria[TYPE_ACHIEV_MOLGEIM];
        case ACHIEV_CRIT_STEELBREAKER_N:
        case ACHIEV_CRIT_STEELBREAKER_H:
            if (GetData(TYPE_ASSEMBLY) == SPECIAL)
                return m_abAchievCriteria[TYPE_ACHIEV_STEELBREAKER];
        case ACHIEV_CRIT_STUNNED_BRUND_N:
        case ACHIEV_CRIT_STUNNED_STEEL_N:
        case ACHIEV_CRIT_STUNNED_MOLG_N:
        case ACHIEV_CRIT_STUNNED_BRUND_H:
        case ACHIEV_CRIT_STUNNED_STEEL_H:
        case ACHIEV_CRIT_STUNNED_MOLG_H:
            if (GetData(TYPE_ASSEMBLY) == SPECIAL)
                return m_abAchievCriteria[TYPE_ACHIEV_STUNNED];
        case ACHIEV_CRIT_SHATTERED_N:
        case ACHIEV_CRIT_SHATTERED_H:
            return m_abAchievCriteria[TYPE_ACHIEV_SHATTERED];
        case ACHIEV_CRIT_HEARTBREAKER_N:
        case ACHIEV_CRIT_HEARTBREAKER_H:
            return GetData(TYPE_XT002_HARD) == DONE;
        case ACHIEV_CRIT_QUICK_SHAVE_N:
        case ACHIEV_CRIT_QUICK_SHAVE_H:
            return m_abAchievCriteria[TYPE_ACHIEV_QUICK_SHAVE];
        case ACHIEV_CRIT_SHUTOUT_N:
        case ACHIEV_CRIT_SHUTOUT_H:
            return m_abAchievCriteria[TYPE_ACHIEV_SHUTOUT];
        case ACHIEV_CRIT_ORB_BOMB_N:
        case ACHIEV_CRIT_ORB_BOMB_H:
            return m_uiActiveTowers >= 1;
        case ACHIEV_CRIT_ORB_DEV_N:
        case ACHIEV_CRIT_ORB_DEV_H:
            return m_uiActiveTowers >= 2;
        case ACHIEV_CRIT_ORB_NUKED_N:
        case ACHIEV_CRIT_ORB_NUKED_H:
            return m_uiActiveTowers >= 3;
        case ACHIEV_CRIT_ORBITUARY_N:
        case ACHIEV_CRIT_ORBITUARY_H:
            return m_uiActiveTowers == 4;
        case ACHIEV_CRIT_NERF_ENG_N:
        case ACHIEV_CRIT_NERF_ENG_H:
            return m_abAchievCriteria[TYPE_ACHIEV_NERF_ENG];
        case ACHIEV_CRIT_RUBBLE_ROLL_N:
        case ACHIEV_CRIT_RUBBLE_ROLL_H:
            return m_abAchievCriteria[TYPE_ACHIEV_RUBBLE];
        case ACHIEV_CRIT_LOOKS_KILL_N:
        case ACHIEV_CRIT_LOOKS_KILL_H:
            return m_abAchievCriteria[TYPE_ACHIEV_LOOKS_KILL];
        case ACHIEV_CRIT_OPEN_ARMS_N:
        case ACHIEV_CRIT_OPEN_ARMS_H:
            return m_abAchievCriteria[TYPE_ACHIEV_OPEN_ARMS];
        case ACHIEV_CRIT_DISARMED_N:
        case ACHIEV_CRIT_DISARMED_H:
            return m_abAchievCriteria[TYPE_ACHIEV_DISARMED];

        default:
            return false;
    }
}

// function which will handle the Flame Leviathan backup spawns
void instance_ulduar::DoCallLeviathanHelp()
{
    Creature* pLeviathan = GetSingleCreatureFromStorage(NPC_LEVIATHAN);
    if (!pLeviathan)
        return;

    for (uint8 i = 0; i < countof(afReinforcementsNormal); ++i)
        pLeviathan->SummonCreature(afReinforcementsNormal[i].uiEntry, afReinforcementsNormal[i].fX, afReinforcementsNormal[i].fY, afReinforcementsNormal[i].fZ, afReinforcementsNormal[i].fO, TEMPSUMMON_DEAD_DESPAWN, 0, true);

    if (!instance->IsRegularDifficulty())
    {
        for (uint8 i = 0; i < countof(afReinforcementsHeroic); ++i)
            pLeviathan->SummonCreature(afReinforcementsHeroic[i].uiEntry, afReinforcementsHeroic[i].fX, afReinforcementsHeroic[i].fY, afReinforcementsHeroic[i].fZ, afReinforcementsHeroic[i].fO, TEMPSUMMON_DEAD_DESPAWN, 0, true);
    }
}

void instance_ulduar::DoProcessShatteredEvent()
{
    // If timer is already running set achiev criteria to true, else start the timer
    if (m_uiShatterAchievTimer)
        SetSpecialAchievementCriteria(TYPE_ACHIEV_SHATTERED, true);
    else
        m_uiShatterAchievTimer = 5000;
}

void instance_ulduar::JustDidDialogueStep(int32 iEntry)
{
    switch (iEntry)
    {
        case SAY_PRE_LEVIATHAN_1:
        case SAY_PRE_LEVIATHAN_2:
        case SAY_PRE_LEVIATHAN_3:
            DoOrSimulateScriptTextForThisInstance(iEntry, NPC_BRONZEBEARD_RADIO);
            break;
        case NPC_LEVIATHAN:
            // move the leviathan in the arena
            if (Creature* pLeviathan = GetSingleCreatureFromStorage(NPC_LEVIATHAN))
            {
                // the boss has increased speed for this move; handled as custom
                float fSpeedRate = pLeviathan->GetSpeedRate(MOVE_RUN);
                pLeviathan->SetWalk(false);
                pLeviathan->SetSpeedRate(MOVE_RUN, 5);
                pLeviathan->GetMotionMaster()->MovePoint(1, afLeviathanMovePos[0], afLeviathanMovePos[1], afLeviathanMovePos[2]);
                pLeviathan->SetSpeedRate(MOVE_RUN, fSpeedRate);

                // modify respawn / home position to the center of arena
                pLeviathan->SetRespawnCoord(afLeviathanMovePos[0], afLeviathanMovePos[1], afLeviathanMovePos[2], afLeviathanMovePos[3]);
            }

            // Note: starting 4.x this gate is a GO 33 and it's destroyed at this point
            DoUseDoorOrButton(GO_LEVIATHAN_GATE);
            break;
    }
}

void instance_ulduar::Update(uint32 uiDiff)
{
    DialogueUpdate(uiDiff);

    if (GetData(TYPE_IGNIS) == IN_PROGRESS)
    {
        if (m_uiShatterAchievTimer)
        {
            // Just set the timer to 0 when it expires
            if (m_uiShatterAchievTimer <= uiDiff)
                m_uiShatterAchievTimer = 0;
            else
                m_uiShatterAchievTimer -= uiDiff;
        }
    }
}

InstanceData* GetInstanceData_instance_ulduar(Map* pMap)
{
    return new instance_ulduar(pMap);
}

bool ProcessEventId_event_ulduar(uint32 uiEventId, Object* pSource, Object* /*pTarget*/, bool /*bIsStart*/)
{
    if (uiEventId == EVENT_ID_SPELL_SHATTER)
    {
        if (pSource->GetTypeId() == TYPEID_UNIT)
        {
            if (instance_ulduar* pInstance = (instance_ulduar*)((Creature*)pSource)->GetInstanceData())
            {
                pInstance->DoProcessShatteredEvent();
                return true;
            }
        }
    }
    else if (uiEventId == EVENT_ID_SHUTDOWN)
    {
        if (pSource->GetTypeId() == TYPEID_UNIT)
        {
            if (instance_ulduar* pInstance = (instance_ulduar*)((Creature*)pSource)->GetInstanceData())
            {
                pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_SHUTOUT, false);
                return true;
            }
        }
    }
    else if (uiEventId == EVENT_ID_SCRAP_REPAIR)
    {
        if (pSource->GetTypeId() == TYPEID_UNIT)
        {
            if (instance_ulduar* pInstance = (instance_ulduar*)((Creature*)pSource)->GetInstanceData())
            {
                pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_NERF_ENG, false);
                return true;
            }
        }
    }

    return false;
}

void AddSC_instance_ulduar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_ulduar";
    pNewScript->GetInstanceData = &GetInstanceData_instance_ulduar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_ulduar";
    pNewScript->pProcessEventId = &ProcessEventId_event_ulduar;
    pNewScript->RegisterSelf();
}
