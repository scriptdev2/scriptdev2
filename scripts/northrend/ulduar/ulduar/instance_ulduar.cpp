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
SDName: instance_ulduar
SD%Complete: 
SDComment: 
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

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

instance_ulduar::instance_ulduar(Map* pMap) : ScriptedInstance(pMap),
    // Creatures
    m_uiLeviathanGUID(0),
    m_uiIgnisGUID(0),
    m_uiRazorscaleGUID(0),
    m_uiCommanderGUID(0),
    m_uiXT002GUID(0),
    m_uiBrundirGUID(0),
    m_uiMolgeimGUID(0),
    m_uiSteelbreakerGUID(0),
    m_uiKologarnGUID(0),
    m_uiAuriayaGUID(0),
    m_uiMimironGUID(0),
    m_uiHodirGUID(0),
    m_uiThorimGUID(0),
    m_uiFreyaGUID(0),
    m_uiVezaxGUID(0),
    m_uiYoggSaronGUID(0),
    m_uiAlgalonGUID(0),
    m_uiRightArmGUID(0),
    m_uiLeftArmGUID(0),
    m_uiFeralDefenderGUID(0),
    m_uiElderBrightleafGUID(0),
    m_uiElderStonebarkGUID(0),
    m_uiElderIronbrachGUID(0),
    m_uiSaroniteAnimusGUID(0),
    m_uiRunicColossusGUID(0),
    m_uiRuneGiantGUID(0),
    m_uiJormungarGUID(0),
    m_uiLeviathanMkGUID(0),
    m_uiSaraGUID(0),
    m_uiYoggBrainGUID(0),

    // Chests
    m_uiKologarnLootGUID(0),
    m_uiHodirLootGUID(0),
    m_uiHodirRareLootGUID(0),
    m_uiThorimLootGUID(0),
    m_uiThorimRareLootGUID(0),
    m_uiMimironLootGUID(0),
    m_uiMimironHardLootGUID(0),
    m_uiAlagonLootGUID(0),

    // Doors
    // The siege
    m_uiShieldWallGUID(0),
    m_uiLeviathanGateGUID(0),
    m_uiXT002GateGUID(0),
    m_uiBrokenHarpoonGUID(0),
    // Archivum
    m_uiIronCouncilDoorGUID(0),
    m_uiArchivumDoorGUID(0),
    m_uiArchivumConsoleGUID(0),
    m_uiUniverseFloorArchivumGUID(0),
    // Celestial planetarium
    m_uiCelestialDoorGUID(0),
    m_uiCelestialConsoleGUID(0),
    m_uiUniverseFloorCelestialGUID(0),
    m_uiAzerothGlobeGUID(0),
    // Kologarn
    m_uiShatteredHallsDoorGUID(0),
    m_uiKologarnBridgeGUID(0),
    // Hodir
    m_uiHodirEnterDoorGUID(0),
    m_uiHodirWallGUID(0),
    m_uiHodirExitDoorGUID(0),
    // Mimiron
    m_uiMimironButtonGUID(0),
    m_uiMimironDoor1GUID(0),
    m_uiMimironDoor2GUID(0),
    m_uiMimironDoor3GUID(0),
    m_uiMimironElevatorGUID(0),
    // Thorim
    m_uiArenaEnterDoorGUID(0),
    m_uiArenaExitDoorGUID(0),
    m_uiHallwayDoorGUID(0),
    m_uiThorimEnterDoorGUID(0),
    m_uiThorimLeverGUID(0),
    // Prison
    m_uiAncientGateGUID(0),
    m_uiVezaxGateGUID(0),
    m_uiYoggGateGUID(0),
    m_uiBrainDoor1GUID(0),
    m_uiBrainDoor2GUID(0),
    m_uiBrainDoor3GUID(0)
{
    Initialize();
}

void instance_ulduar::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiHardBoss, 0, sizeof(m_auiHardBoss));
    memset(&m_auiUlduarKeepers, 0, sizeof(m_auiUlduarKeepers));
    memset(&m_auiUlduarTeleporters, 0, sizeof(m_auiUlduarTeleporters));
    memset(&m_auiMimironTelGUID, 0, sizeof(m_auiMimironTelGUID));
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
            m_uiLeviathanGUID = pCreature->GetGUID();
            break;
        case NPC_IGNIS:
            m_uiIgnisGUID = pCreature->GetGUID();
            break;
        case NPC_RAZORSCALE:
            m_uiRazorscaleGUID = pCreature->GetGUID();
            break;
        case NPC_COMMANDER:
            m_uiCommanderGUID = pCreature->GetGUID();
            break;
        case NPC_XT002:
            m_uiXT002GUID = pCreature->GetGUID();
            break;
        case NPC_STEELBREAKER:
            m_uiSteelbreakerGUID = pCreature->GetGUID();
            break;
        case NPC_MOLGEIM:
            m_uiMolgeimGUID = pCreature->GetGUID();
            break;
        case NPC_BRUNDIR:
            m_uiBrundirGUID = pCreature->GetGUID();
            break;
        case NPC_KOLOGARN:
            m_uiKologarnGUID = pCreature->GetGUID();
            break;
        case NPC_RIGHT_ARM:
            m_uiRightArmGUID = pCreature->GetGUID();
            break;
        case NPC_LEFT_ARM:
            m_uiLeftArmGUID = pCreature->GetGUID();
            break;
        case NPC_AURIAYA:
            m_uiAuriayaGUID = pCreature->GetGUID();
            break;
        case NPC_FERAL_DEFENDER:
            m_uiFeralDefenderGUID = pCreature->GetGUID();
            break;
        case NPC_MIMIRON:
            m_uiMimironGUID = pCreature->GetGUID();
            if(m_auiEncounter[7] == DONE)
                DoSpawnMimiron();
            break;
        case NPC_LEVIATHAN_MK:
            m_uiLeviathanMkGUID = pCreature->GetGUID();
            break;
        case NPC_HODIR:
            m_uiHodirGUID = pCreature->GetGUID();
            if(m_auiEncounter[8] == DONE)
                DoSpawnHodir();
            break;
        case NPC_THORIM:
            m_uiThorimGUID = pCreature->GetGUID();
            if(m_auiEncounter[9] == DONE)
                DoSpawnThorim();
            break;
        case NPC_RUNIC_COLOSSUS:
            m_uiRunicColossusGUID = pCreature->GetGUID();
            break;
        case NPC_RUNE_GIANT:
            m_uiRuneGiantGUID = pCreature->GetGUID();
            break;
        case NPC_JORMUNGAR_BEHEMOTH:
            m_uiJormungarGUID = pCreature->GetGUID();
            break;
        case NPC_FREYA:
            m_uiFreyaGUID = pCreature->GetGUID();
            if(m_auiEncounter[10] == DONE)
                DoSpawnFreya();
            break;
        case NPC_ELDER_BRIGHTLEAF:
            m_uiElderBrightleafGUID = pCreature->GetGUID();
            break;
        case NPC_ELDER_IRONBRACH:
            m_uiElderIronbrachGUID = pCreature->GetGUID();
            break;
        case NPC_ELDER_STONEBARK:
            m_uiElderStonebarkGUID = pCreature->GetGUID();
            break;
        case NPC_VEZAX:
            m_uiVezaxGUID = pCreature->GetGUID();
            break;
        case NPC_SARONITE_ANIMUS:
            m_uiSaroniteAnimusGUID = pCreature->GetGUID();
            break;
        case NPC_YOGGSARON:
            m_uiYoggSaronGUID = pCreature->GetGUID();
            break;
        case NPC_SARA:
            m_uiSaraGUID = pCreature->GetGUID();
            break;
        case NPC_YOGG_BRAIN:
            m_uiYoggBrainGUID = pCreature->GetGUID();
            break;
        case NPC_ALGALON:
            m_uiAlgalonGUID = pCreature->GetGUID();
            break;
     }
}

void instance_ulduar::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
     {
        // -----------------    Doors & Other   -----------------
        // The siege
        case GO_SHIELD_WALL:
            m_uiShieldWallGUID = pGo->GetGUID();
            break;
        case GO_LEVIATHAN_GATE:
            m_uiLeviathanGateGUID = pGo->GetGUID();
            if (m_auiEncounter[0] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_XT002_GATE:
            pGo->SetGoState(GO_STATE_READY);
            if (m_auiEncounter[3] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            if (m_auiEncounter[1] == DONE && m_auiEncounter[2] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiXT002GateGUID = pGo->GetGUID();
            break;
        case GO_BROKEN_HARPOON:
            m_uiBrokenHarpoonGUID = pGo->GetGUID();
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;

        // Archivum
        case GO_IRON_ENTRANCE_DOOR:
            m_uiIronCouncilDoorGUID = pGo->GetGUID();
            break;
        case GO_ARCHIVUM_DOOR:
            m_uiArchivumDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[4])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARCHIVUM_CONSOLE:
            m_uiArchivumConsoleGUID = pGo->GetGUID();
            break;
        case GO_UNIVERSE_FLOOR_ARCHIVUM:
            m_uiUniverseFloorArchivumGUID = pGo->GetGUID();
            break;
        // Celestial Planetarium
        case GO_CELESTIAL_ACCES:
            m_uiCelestialConsoleGUID = pGo->GetGUID();
            break;
        case GO_CELESTIAL_DOOR:
            m_uiCelestialDoorGUID = pGo->GetGUID();
            break;
        case GO_UNIVERSE_FLOOR_CELESTIAL:
            m_uiUniverseFloorCelestialGUID = pGo->GetGUID();
            break;
        case GO_AZEROTH_GLOBE:
            m_uiAzerothGlobeGUID = pGo->GetGUID();
            break;
        // Shattered Hallway
        case GO_KOLOGARN_BRIDGE:
            m_uiKologarnBridgeGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            if (m_auiEncounter[5] == DONE)
                pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_SHATTERED_DOOR:
            m_uiShatteredHallsDoorGUID = pGo->GetGUID();
            break;

        // -----------------    The Keepers    -----------------
        // Hodir
        case GO_HODIR_EXIT:
            m_uiHodirExitDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[8])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HODIR_ICE_WALL:
            m_uiHodirWallGUID = pGo->GetGUID();
            if (m_auiEncounter[8])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HODIR_ENTER:
            m_uiHodirEnterDoorGUID = pGo->GetGUID();
            break;
        // Mimiron
        case G0_MIMIRON_BUTTON:
            m_uiMimironButtonGUID = pGo->GetGUID();
            if (m_auiEncounter[7] == NOT_STARTED)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        case GO_MIMIRON_DOOR_1:
            m_uiMimironDoor1GUID = pGo->GetGUID();
            break;
        case GO_MIMIRON_DOOR_2:
            m_uiMimironDoor2GUID = pGo->GetGUID();
            break;
        case GO_MIMIRON_DOOR_3:
            m_uiMimironDoor3GUID = pGo->GetGUID();
            break;
        case GO_MIMIRON_ELEVATOR:
            m_uiMimironElevatorGUID = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL1:
            m_auiMimironTelGUID[0] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL2:
            m_auiMimironTelGUID[1] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL3:
            m_auiMimironTelGUID[2] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL4:
            m_auiMimironTelGUID[3] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL5:
            m_auiMimironTelGUID[4] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL6:
            m_auiMimironTelGUID[5] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL7:
            m_auiMimironTelGUID[6] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL8:
            m_auiMimironTelGUID[7] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL9:
            m_auiMimironTelGUID[8] = pGo->GetGUID();
            break;
        // Thorim
        case GO_DARK_IRON_PORTCULIS:
            m_uiArenaExitDoorGUID = pGo->GetGUID();
            break;
        case GO_RUNED_STONE_DOOR:
            m_uiHallwayDoorGUID = pGo->GetGUID();
            break;
        case GO_THORIM_STONE_DOOR:
            m_uiThorimEnterDoorGUID = pGo->GetGUID();
            break;
        case GO_LIGHTNING_FIELD:
            m_uiArenaEnterDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_LEVER:
            m_uiThorimLeverGUID = pGo->GetGUID();
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;

        // Prison
        case GO_ANCIENT_GATE:
            m_uiAncientGateGUID = pGo->GetGUID();
            DoOpenMadnessDoorIfCan();
            break;
        case GO_VEZAX_GATE:
            m_uiVezaxGateGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_READY);
            if (m_auiEncounter[11])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_YOGG_GATE:
            m_uiYoggGateGUID = pGo->GetGUID();
            break;
        case GO_BRAIN_DOOR1:
            m_uiBrainDoor1GUID = pGo->GetGUID();
            break;
        case GO_BRAIN_DOOR2:
            m_uiBrainDoor2GUID = pGo->GetGUID();
            break;
        case GO_BRAIN_DOOR3:
            m_uiBrainDoor3GUID = pGo->GetGUID();
            break;

        // -----------------    Chests    -----------------
        // Kologarn
        case GO_CACHE_OF_LIVING_STONE:
        case GO_CACHE_OF_LIVING_STONE_H:
            m_uiKologarnLootGUID = pGo->GetGUID();
            break;

        // Hodir
        case GO_CACHE_OF_WINTER:
        case GO_CACHE_OF_WINTER_H:
            m_uiHodirLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_RARE_WINTER:
        case GO_CACHE_OF_RARE_WINTER_H:
            m_uiHodirRareLootGUID = pGo->GetGUID();
            break;

        // Thorim
        case GO_CACHE_OF_STORMS:
        case GO_CACHE_OF_STORMS_H:
            m_uiThorimLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_RARE_STORMS:
        case GO_CACHE_OF_RARE_STORMS_H:
            m_uiThorimRareLootGUID = pGo->GetGUID();
            break;

        // Mimiron
        case GO_CACHE_OF_INOV:
        case GO_CACHE_OF_INOV_H:
            m_uiMimironLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_INOV_HARD:
        case GO_CACHE_OF_INOV_HARD_H:
            m_uiMimironHardLootGUID = pGo->GetGUID();
            break;

        // Alagon
        case GO_GIFT_OF_OBSERVER:
        case GO_GIFT_OF_OBSERVER_H:
            m_uiAlagonLootGUID = pGo->GetGUID();
            break;
     }
}

Player* instance_ulduar::GetPlayerInMap()
{
    Map::PlayerList const& players = instance->GetPlayers();

    if (!players.isEmpty())
    {
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* plr = itr->getSource())
                return plr;
        }
    }
    return NULL;
}

// Used in order to unlock the door to Vezax
void instance_ulduar::DoOpenMadnessDoorIfCan()
{
    if (m_auiEncounter[7] == DONE && m_auiEncounter[8] == DONE && m_auiEncounter[9] == DONE && m_auiEncounter[10] == DONE)
    {
        if(GameObject* pDoor = instance->GetGameObject(m_uiAncientGateGUID))
            pDoor->SetGoState(GO_STATE_ACTIVE);
    }
}

void instance_ulduar::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_LEVIATHAN:
            m_auiEncounter[0] = uiData;
            DoUseDoorOrButton(m_uiShieldWallGUID);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiXT002GateGUID);
                DoUseDoorOrButton(m_uiLeviathanGateGUID);
            }
            break;
        case TYPE_IGNIS:
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_RAZORSCALE:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_XT002:
            m_auiEncounter[3] = uiData;
            DoUseDoorOrButton(m_uiXT002GateGUID);
            break;
        case TYPE_ASSEMBLY:
            m_auiEncounter[4] = uiData;
            DoUseDoorOrButton(m_uiIronCouncilDoorGUID);
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiArchivumDoorGUID);
            break;
        case TYPE_KOLOGARN:
            m_auiEncounter[5] = uiData;
            DoUseDoorOrButton(m_uiShatteredHallsDoorGUID);
            if (uiData == DONE)
            {
                DoRespawnGameObject(m_uiKologarnLootGUID, 30*MINUTE);
                if(GameObject* pBridge = instance->GetGameObject(m_uiKologarnBridgeGUID))
                    pBridge->SetGoState(GO_STATE_READY);
            }
            break;
        case TYPE_AURIAYA:
            m_auiEncounter[6] = uiData;
            break;
        // Keepers
        case TYPE_MIMIRON:
            m_auiEncounter[7] = uiData;
            DoUseDoorOrButton(m_uiMimironDoor1GUID);
            DoUseDoorOrButton(m_uiMimironDoor2GUID);
            DoUseDoorOrButton(m_uiMimironDoor3GUID);
            if (uiData == DONE)
            {
                if (m_auiHardBoss[3] != DONE)
                    DoRespawnGameObject(m_uiMimironLootGUID, 30*MINUTE);
                DoSpawnMimiron();
                DoOpenMadnessDoorIfCan();
            }
            break;
        case TYPE_HODIR:
            m_auiEncounter[8] = uiData;
            DoUseDoorOrButton(m_uiHodirEnterDoorGUID);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiHodirWallGUID);
                DoUseDoorOrButton(m_uiHodirExitDoorGUID);
                DoRespawnGameObject(m_uiHodirLootGUID, 30*MINUTE);
                DoSpawnHodir();
                DoOpenMadnessDoorIfCan();
            }
            break;
        case TYPE_THORIM:
            m_auiEncounter[9] = uiData;
            DoUseDoorOrButton(m_uiArenaEnterDoorGUID);
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(m_uiArenaExitDoorGUID);
            if (uiData == DONE)
            {
                if (m_auiHardBoss[5] != DONE)
                    DoRespawnGameObject(m_uiThorimLootGUID, 30*MINUTE);
                DoSpawnThorim();
                DoOpenMadnessDoorIfCan();
            }
            break;
        case TYPE_FREYA:
            m_auiEncounter[10] = uiData;
            if (uiData == DONE)
            {
                DoSpawnFreya();
                DoOpenMadnessDoorIfCan();
            }
            break;
        // Prison
        case TYPE_VEZAX:
            m_auiEncounter[11] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiVezaxGateGUID);
            break;
        case TYPE_YOGGSARON:
            m_auiEncounter[12] = uiData;
            DoUseDoorOrButton(m_uiYoggGateGUID);
            break;

        // Celestial Planetarium
        case TYPE_ALGALON:
            m_auiEncounter[13] = uiData;
            //TODO: need to find the proper way to use these
            DoUseDoorOrButton(m_uiCelestialDoorGUID);
            DoUseDoorOrButton(m_uiUniverseFloorCelestialGUID);
            if (uiData == DONE)
                DoRespawnGameObject(m_uiAlagonLootGUID, 30*MINUTE);
            break;

        // Hard modes
        case TYPE_LEVIATHAN_HARD:
            m_auiHardBoss[0] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_XT002_HARD:
            m_auiHardBoss[1] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_HODIR_HARD:
            m_auiHardBoss[4] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(m_uiHodirRareLootGUID, 30*MINUTE);
            break;
        case TYPE_ASSEMBLY_HARD:
            m_auiHardBoss[2] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_FREYA_HARD:
            m_auiHardBoss[6] = uiData;                      // Hard mode loot in in script
            break;
        case TYPE_THORIM_HARD:
            m_auiHardBoss[5] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(m_uiThorimRareLootGUID, 30*MINUTE);
            break;
        case TYPE_MIMIRON_HARD:
            m_auiHardBoss[3] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(m_uiMimironHardLootGUID, 30*MINUTE);
            break;
        case TYPE_VEZAX_HARD:
            m_auiHardBoss[7] = uiData;                      // TODO: add extra loot
            break;
        case TYPE_YOGGSARON_HARD:
            m_auiHardBoss[8] = uiData;                      // TODO: add extra loot
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

        // Teleporters
        case TYPE_LEVIATHAN_TP:
            m_auiUlduarTeleporters[0] = uiData;
            break;
        case TYPE_XT002_TP:
            m_auiUlduarTeleporters[1] = uiData;
            break;
        case TYPE_MIMIRON_TP:
            m_auiUlduarTeleporters[2] = uiData;
            break;
     }

     if (uiData == DONE || uiData == FAIL)
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
             << m_auiHardBoss[7] << " " << m_auiHardBoss[8] << " " << m_auiUlduarKeepers[0] << " "
             << m_auiUlduarKeepers[1] << " " << m_auiUlduarKeepers[2] << " " << m_auiUlduarKeepers[3];

         strInstData = saveStream.str();

         SaveToDB();
         OUT_SAVE_INST_DATA_COMPLETE;
     }
}

uint64 instance_ulduar::GetData64(uint32 uiData)
{
    switch (uiData)
    {
        // Siege
        case NPC_LEVIATHAN:
            return m_uiLeviathanGUID;
        case NPC_IGNIS:
            return m_uiIgnisGUID;
        case NPC_RAZORSCALE:
            return m_uiRazorscaleGUID;
        case NPC_COMMANDER:
            return m_uiCommanderGUID;
        case NPC_XT002:
            return m_uiXT002GUID;
        // Antechamber
        case NPC_STEELBREAKER:
            return m_uiSteelbreakerGUID;
        case NPC_MOLGEIM:
            return m_uiMolgeimGUID;
        case NPC_BRUNDIR:
            return m_uiBrundirGUID;
        case NPC_KOLOGARN:
            return m_uiKologarnGUID;
        case NPC_LEFT_ARM:
            return m_uiLeftArmGUID;
        case NPC_RIGHT_ARM:
            return m_uiRightArmGUID;
        case NPC_AURIAYA:
            return m_uiAuriayaGUID;
        // Keepers
        case NPC_MIMIRON:
            return m_uiMimironGUID;
        case NPC_LEVIATHAN_MK:
            return m_uiLeviathanMkGUID;
        case NPC_HODIR:
            return m_uiMimironGUID;
        case NPC_THORIM:
            return m_uiThorimGUID;
        case NPC_RUNE_GIANT:
            return m_uiRuneGiantGUID;
        case NPC_RUNIC_COLOSSUS:
            return m_uiRunicColossusGUID;
        case NPC_JORMUNGAR_BEHEMOTH:
            return m_uiJormungarGUID;
        case NPC_FREYA:
            return m_uiFreyaGUID;
        case NPC_ELDER_BRIGHTLEAF:
            return m_uiElderBrightleafGUID;
        case NPC_ELDER_IRONBRACH:
            return m_uiElderIronbrachGUID;
        case NPC_ELDER_STONEBARK:
            return m_uiElderStonebarkGUID;
        case NPC_VEZAX:
            return m_uiVezaxGUID;
        case NPC_YOGGSARON:
            return m_uiYoggSaronGUID;
        case NPC_SARA:
            return m_uiSaraGUID;
        case NPC_YOGG_BRAIN:
            return m_uiYoggBrainGUID;
        case NPC_ALGALON:
            return m_uiAlgalonGUID;

        // Mimiron hard mode button
        case G0_MIMIRON_BUTTON:
            return m_uiMimironButtonGUID;
        // Celestial door
        case GO_CELESTIAL_DOOR:
            return m_uiCelestialDoorGUID;
    }

    return 0;
}

// TODO: implement all hard mode loot here!
bool instance_ulduar::CheckConditionCriteriaMeet(Player const* pSource, uint32 uiMapId, uint32 uiInstanceConditionId)
{
    if (uiMapId != instance->GetId())
        return false;

    switch (uiInstanceConditionId)
     {
        case TYPE_XT002_HARD:
            break;
     }
    return false;
}

uint32 instance_ulduar::GetData(uint32 uiType)
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
        case TYPE_ASSEMBLY_HARD:
            return m_auiHardBoss[2];
        case TYPE_MIMIRON_HARD:
            return m_auiHardBoss[3];
        case TYPE_HODIR_HARD:
            return m_auiHardBoss[4];
        case TYPE_THORIM_HARD:
            return m_auiHardBoss[5];
        case TYPE_FREYA_HARD:
            return m_auiHardBoss[6];
        case TYPE_VEZAX_HARD:
            return m_auiHardBoss[7];
        case TYPE_YOGGSARON_HARD:
            return m_auiHardBoss[8];

        // Ulduar Keepers
        case TYPE_KEEPER_HODIR:
            return m_auiUlduarKeepers[0];
        case TYPE_KEEPER_THORIM:
            return m_auiUlduarKeepers[1];
        case TYPE_KEEPER_FREYA:
            return m_auiUlduarKeepers[2];
        case TYPE_KEEPER_MIMIRON:
            return m_auiUlduarKeepers[3];

        // Teleporters
        case TYPE_LEVIATHAN_TP:
            return m_auiUlduarTeleporters[0];
        case TYPE_XT002_TP:
            return m_auiUlduarTeleporters[1];
        case TYPE_MIMIRON_TP:
            return m_auiUlduarTeleporters[2];
     }

    return 0;
}

// Spawn the friendly keepers in the central chamber
void instance_ulduar::DoSpawnMimiron()
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    // Mimiron
    if(m_auiEncounter[7] == DONE)
        pPlayer->SummonCreature(NPC_MIMIRON_IMAGE, m_aKeepersSpawnLocs[1].m_fX, m_aKeepersSpawnLocs[1].m_fY, m_aKeepersSpawnLocs[1].m_fZ, m_aKeepersSpawnLocs[1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
}

void instance_ulduar::DoSpawnHodir()
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    // Hodir
    if(m_auiEncounter[8] == DONE)
        pPlayer->SummonCreature(NPC_HODIR_IMAGE, m_aKeepersSpawnLocs[2].m_fX, m_aKeepersSpawnLocs[2].m_fY, m_aKeepersSpawnLocs[2].m_fZ, m_aKeepersSpawnLocs[2].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
}

void instance_ulduar::DoSpawnThorim()
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    // Thorim
    if(m_auiEncounter[9] == DONE)
        pPlayer->SummonCreature(NPC_THORIM_IMAGE, m_aKeepersSpawnLocs[3].m_fX, m_aKeepersSpawnLocs[3].m_fY, m_aKeepersSpawnLocs[3].m_fZ, m_aKeepersSpawnLocs[3].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
}

void instance_ulduar::DoSpawnFreya()
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    // Freya
    if(m_auiEncounter[10] == DONE)
        pPlayer->SummonCreature(NPC_FREYA_IMAGE, m_aKeepersSpawnLocs[0].m_fX, m_aKeepersSpawnLocs[0].m_fY, m_aKeepersSpawnLocs[0].m_fZ, m_aKeepersSpawnLocs[0].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
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
    >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
    >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11]
    >> m_auiEncounter[12] >> m_auiEncounter[13] >> m_auiHardBoss[0] >> m_auiHardBoss[1]
    >> m_auiHardBoss[2] >> m_auiHardBoss[3] >> m_auiHardBoss[4] >> m_auiHardBoss[5]
    >> m_auiHardBoss[6] >> m_auiHardBoss[7] >> m_auiHardBoss[8] >> m_auiUlduarKeepers[0]
    >> m_auiUlduarKeepers[1] >> m_auiUlduarKeepers[2] >> m_auiUlduarKeepers[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_ulduar(Map* pMap)
{
    return new instance_ulduar(pMap);
}

void AddSC_instance_ulduar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_ulduar";
    pNewScript->GetInstanceData = &GetInstanceData_instance_ulduar;
    pNewScript->RegisterSelf();
}
