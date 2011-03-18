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
SDName: instance_dire_maul
SD%Complete: 30
SDComment: Basic Support - Most events and quest-related stuff missing
SDCategory: Dire Maul
EndScriptData */

#include "precompiled.h"
#include "dire_maul.h"

instance_dire_maul::instance_dire_maul(Map* pMap) : ScriptedInstance(pMap),
    // East
    m_bWallDestroyed(false),
    m_uiCrumbleWallGUID(0),
    m_uiCorruptVineGUID(0),
    m_uiConservatoryDoorGUID(0),
    m_uiOldIronbarkGUID(0),

    // West
    m_uiPrinceTortheldrinGUID(0),
    m_uiImmolTharGUID(0),
    m_uiForcefieldGUID(0),
    m_uiPrincesChestAuraGUID(0),
    m_uiTendrisWarpwoodDoorGUID(0),

    // North
    m_uiGordokGUID(0),
    m_uiChoRushGUID(0),
    m_uiMizzleGUID(0)
{
    Initialize();
}

void instance_dire_maul::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiCrystalGeneratorGUID, 0, sizeof(m_auiCrystalGeneratorGUID));

    m_lFelvineShardGUIDs.clear();
    m_luiHighborneSummonerGUIDs.clear();
    m_lGeneratorGuardGUIDs.clear();
}

void instance_dire_maul::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        // West
        case NPC_PRINCE_TORTHELDRIN:
            m_uiPrinceTortheldrinGUID = pCreature->GetGUID();
            if (m_auiEncounter[TYPE_IMMOLTHAR] == DONE)
                pCreature->setFaction(FACTION_HOSTILE);
            break;
        case NPC_ARCANE_ABERRATION:
        case NPC_MANA_REMNANT:
            m_lGeneratorGuardGUIDs.push_back(pCreature->GetGUID());
            break;
        case NPC_IMMOLTHAR:
            m_uiImmolTharGUID = pCreature->GetGUID();
            break;
        case NPC_HIGHBORNE_SUMMONER:
            m_luiHighborneSummonerGUIDs.push_back(pCreature->GetGUID());
            break;

        // North
        case NPC_CHORUSH:
            m_uiChoRushGUID = pCreature->GetGUID();
            break;
        case NPC_KING_GORDOK:
            m_uiGordokGUID = pCreature->GetGUID();
            break;
        case NPC_MIZZLE_THE_CRAFTY:
            m_uiMizzleGUID = pCreature->GetGUID();
            break;
    }
}

void instance_dire_maul::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        // East
        case GO_CRUMBLE_WALL:
            m_uiCrumbleWallGUID = pGo->GetGUID();
            if (m_bWallDestroyed)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CORRUPT_VINE:
            m_uiCorruptVineGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_ALZZIN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_FELVINE_SHARD:
            m_lFelvineShardGUIDs.push_back(pGo->GetGUID());
            break;

        // West
        case GO_CRYSTAL_GENERATOR_1:
            m_auiCrystalGeneratorGUID[0] = pGo->GetGUID();
            if (m_auiEncounter[TYPE_PYLON_1] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CRYSTAL_GENERATOR_2:
            m_auiCrystalGeneratorGUID[1] = pGo->GetGUID();
            if (m_auiEncounter[TYPE_PYLON_2] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CRYSTAL_GENERATOR_3:
            m_auiCrystalGeneratorGUID[2] = pGo->GetGUID();
            if (m_auiEncounter[TYPE_PYLON_3] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CRYSTAL_GENERATOR_4:
            m_auiCrystalGeneratorGUID[3] = pGo->GetGUID();
            if (m_auiEncounter[TYPE_PYLON_4] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CRYSTAL_GENERATOR_5:
            m_auiCrystalGeneratorGUID[4] = pGo->GetGUID();
            if (m_auiEncounter[TYPE_PYLON_5] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_FORCEFIELD:
            m_uiForcefieldGUID = pGo->GetGUID();
            if (CheckAllGeneratorsDestroyed())
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PRINCES_CHEST_AURA:
            m_uiPrincesChestAuraGUID = pGo->GetGUID();
            break;
    }
}

void instance_dire_maul::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        // East
        case TYPE_ALZZIN:                                   // This Encounter is expected to be handled within Acid (reason handling at 50% hp)
            if (uiData == DONE)
            {
                if (!m_bWallDestroyed)
                {
                    DoUseDoorOrButton(m_uiCrumbleWallGUID);
                    m_bWallDestroyed = true;
                }

                DoUseDoorOrButton(m_uiCorruptVineGUID);

                if (!m_lFelvineShardGUIDs.empty())
                {
                    for(std::list<uint64>::iterator i = m_lFelvineShardGUIDs.begin(); i != m_lFelvineShardGUIDs.end(); ++i)
                        DoRespawnGameObject(*i);
                }
            }
            else if (uiData == SPECIAL && !m_bWallDestroyed)
            {
                DoUseDoorOrButton(m_uiCrumbleWallGUID);
                m_bWallDestroyed = true;
            }
            m_auiEncounter[uiType] = uiData;
            break;

        // West
        case TYPE_IMMOLTHAR:
            if (uiData == DONE)
            {
                if (Creature* pPrince = instance->GetCreature(m_uiPrinceTortheldrinGUID))
                {
                    DoScriptText(SAY_FREE_IMMOLTHAR, pPrince);
                    pPrince->setFaction(FACTION_HOSTILE);
                    // Despawn Chest-Aura
                    if (GameObject* pChestAura = instance->GetGameObject(m_uiPrincesChestAuraGUID))
                        pChestAura->Use(pPrince);
                }
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_PRINCE:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_PYLON_1:
        case TYPE_PYLON_2:
        case TYPE_PYLON_3:
        case TYPE_PYLON_4:
        case TYPE_PYLON_5:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_auiCrystalGeneratorGUID[uiType - TYPE_PYLON_1]);
                if (CheckAllGeneratorsDestroyed())
                    ProcessForceFieldOpening();
            }
            break;

        // North
        case TYPE_KING_GORDOK:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                // Apply Aura to players in the map
                Map::PlayerList const& players = instance->GetPlayers();
                for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* pPlayer = itr->getSource())
                        pPlayer->CastSpell(pPlayer, SPELL_KING_OF_GORDOK, true);
                }
            }
            break;
    }

    if (uiData >= DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream    << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                      << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                      << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_dire_maul::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

uint64 instance_dire_maul::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_CHORUSH:       return m_uiChoRushGUID;
        case NPC_KING_GORDOK:   return m_uiGordokGUID;
        default:
            return 0;
    }
}

void instance_dire_maul::OnCreatureEnterCombat(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        // West
        // - Handling of guards of generators
        case NPC_ARCANE_ABERRATION:
        case NPC_MANA_REMNANT:
            if (!m_lGeneratorGuardGUIDs.empty())
            {
                for (uint8 i = 0; i < MAX_GENERATORS; i++)
                {
                    GameObject* pGenerator = instance->GetGameObject(m_auiCrystalGeneratorGUID[i]);
                    // Skip non-existing or finished generators
                    if (!pGenerator || GetData(TYPE_PYLON_1 + i) == DONE)
                        continue;

                    // Sort all remaining (alive) NPCs to unfinished generators
                    for (std::list<uint64>::iterator itr = m_lGeneratorGuardGUIDs.begin(); itr != m_lGeneratorGuardGUIDs.end();)
                    {
                        Creature* pGuard = instance->GetCreature(*itr);
                        if (!pGuard || pGuard->isDead())    // Remove invalid guids and dead guards
                        {
                            m_lGeneratorGuardGUIDs.erase(itr++);
                            continue;
                        }

                        if (pGuard->IsWithinDistInMap(pGenerator, 20.0f))
                        {
                            m_sSortedGeneratorGuards[i].insert(pGuard->GetGUIDLow());
                            m_lGeneratorGuardGUIDs.erase(itr++);
                        }
                        else
                            ++itr;
                    }
                }
            }
            break;
        // - Set InstData for ImmolThar
        case NPC_IMMOLTHAR:
            SetData(TYPE_IMMOLTHAR, IN_PROGRESS);
            break;
    }
}

void instance_dire_maul::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        // West
        // - Handling of guards of generators
        case NPC_ARCANE_ABERRATION:
        case NPC_MANA_REMNANT:
            for (uint8 i = 0; i < MAX_GENERATORS; i++)
            {
                // Skip already activated generators
                if (GetData(TYPE_PYLON_1 + i) == DONE)
                    continue;

                // Only process generator where the npc is sorted in
                if (m_sSortedGeneratorGuards[i].find(pCreature->GetGUIDLow()) != m_sSortedGeneratorGuards[i].end())
                {
                    m_sSortedGeneratorGuards[i].erase(pCreature->GetGUIDLow());
                    if (m_sSortedGeneratorGuards[i].empty())
                        SetData(TYPE_PYLON_1 + i, DONE);

                    break;
                }
            }
            break;
        // - Set InstData for ImmolThar
        case NPC_IMMOLTHAR:
            SetData(TYPE_IMMOLTHAR, DONE);
            break;

        // North
        // - Handling of Ogre Boss (Assume boss can be handled in Acid)
        case NPC_KING_GORDOK:
            SetData(TYPE_KING_GORDOK, DONE);
            break;
    }
}

void instance_dire_maul::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >>   m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >>
                    m_auiEncounter[3] >> m_auiEncounter[4] >> m_auiEncounter[5] >>
                    m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8];

    if (m_auiEncounter[TYPE_ALZZIN] >= DONE)
       m_bWallDestroyed = true;

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

bool instance_dire_maul::CheckAllGeneratorsDestroyed()
{
    if (m_auiEncounter[TYPE_PYLON_1] != DONE || m_auiEncounter[TYPE_PYLON_2] != DONE || m_auiEncounter[TYPE_PYLON_3] != DONE || m_auiEncounter[TYPE_PYLON_4] != DONE || m_auiEncounter[TYPE_PYLON_5] != DONE)
        return false;

    return true;
}

void instance_dire_maul::ProcessForceFieldOpening()
{
    // 'Open' the force field
    DoUseDoorOrButton(m_uiForcefieldGUID);

    // Let the summoners attack Immol'Thar
    Creature* pImmolThar = instance->GetCreature(m_uiImmolTharGUID);
    if (!pImmolThar || pImmolThar->isDead())
        return;

    bool bHasYelled = false;
    for (std::list<uint64>::const_iterator itr = m_luiHighborneSummonerGUIDs.begin(); itr != m_luiHighborneSummonerGUIDs.end(); ++itr)
    {
        Creature* pSummoner = instance->GetCreature(*itr);

        if (!bHasYelled && pSummoner)
        {
            DoScriptText(SAY_KILL_IMMOLTHAR, pSummoner);
            bHasYelled = true;
        }

        if (!pSummoner || pSummoner->isDead())
            continue;

        pSummoner->AI()->AttackStart(pImmolThar);
    }
    m_luiHighborneSummonerGUIDs.clear();
}

InstanceData* GetInstanceData_instance_dire_maul(Map* pMap)
{
    return new instance_dire_maul(pMap);
}

void AddSC_instance_dire_maul()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_dire_maul";
    pNewScript->GetInstanceData = &GetInstanceData_instance_dire_maul;
    pNewScript->RegisterSelf();
}
