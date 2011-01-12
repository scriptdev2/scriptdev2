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
SDName: Instance_Molten_Core
SD%Complete: 25
SDComment: Majordomos and Ragnaros Event missing
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"

instance_molten_core::instance_molten_core(Map* pMap) : ScriptedInstance(pMap),
    m_uiGarrGUID(0),
    m_uiSulfuronGUID(0),
    m_uiMajordomoGUID(0),
    m_uiRagnarosGUID(0),
    m_uiRuneKoroGUID(0),
    m_uiRuneZethGUID(0),
    m_uiRuneMazjGUID(0),
    m_uiRuneTheriGUID(0),
    m_uiRuneBlazGUID(0),
    m_uiRuneKressGUID(0),
    m_uiRuneMohnGUID(0),
    m_uiFirelordCacheGUID(0)
{
    Initialize();
}

void instance_molten_core::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_molten_core::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_molten_core::OnPlayerEnter(Player* pPlayer)
{
    // Summon Majordomo if can
    DoSpawnMajordomoIfCan(true);
}

void instance_molten_core::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        // Bosses
        case NPC_GARR:      m_uiGarrGUID = pCreature->GetGUID();        break;
        case NPC_SULFURON:  m_uiSulfuronGUID = pCreature->GetGUID();    break;
        case NPC_MAJORDOMO: m_uiMajordomoGUID = pCreature->GetGUID();   break;
        case NPC_RAGNAROS:  m_uiRagnarosGUID = pCreature->GetGUID();    break;

        // Push adds to lists in order to handle respawn
        case NPC_FLAMEWAKER_PROTECTOR:  m_luiProtectorGUIDs.push_back(pCreature->GetGUID());    break;
        case NPC_FLAMEWAKER:            m_luiFlamewakerGUIDs.push_back(pCreature->GetGUID());   break;
        case NPC_FIRESWORN:             m_luiFireswornGUIDs.push_back(pCreature->GetGUID());    break;
        case NPC_FLAMEWAKER_PRIEST:     m_luiPriestGUIDs.push_back(pCreature->GetGUID());       break;
        case NPC_CORE_RAGER:            m_luiRagerGUIDs.push_back(pCreature->GetGUID());        break;
    }
}

void instance_molten_core::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        // Runes
        case GO_RUNE_KRESS: m_uiRuneKressGUID = pGo->GetGUID(); break;      // Magmadar
        case GO_RUNE_MOHN:  m_uiRuneMohnGUID = pGo->GetGUID();  break;      // Gehennas
        case GO_RUNE_BLAZ:  m_uiRuneBlazGUID = pGo->GetGUID();  break;      // Garr
        case GO_RUNE_MAZJ:  m_uiRuneMazjGUID = pGo->GetGUID();  break;      // Shazzrah
        case GO_RUNE_ZETH:  m_uiRuneZethGUID = pGo->GetGUID();  break;      // Geddon
        case GO_RUNE_THERI: m_uiRuneTheriGUID = pGo->GetGUID(); break;      // Golemagg
        case GO_RUNE_KORO:  m_uiRuneKoroGUID = pGo->GetGUID();  break;      // Sulfuron

        // Majordomo event chest
        case GO_CACHE_OF_THE_FIRE_LORD: m_uiFirelordCacheGUID = pGo->GetGUID(); break;
    }
}

void instance_molten_core::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_LUCIFRON:
            m_auiEncounter[uiType] = uiData;
            if (uiData == FAIL)
                DoHandleAdds(m_luiProtectorGUIDs);
            break;
        case TYPE_MAGMADAR:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiRuneKressGUID);
            break;
        case TYPE_GEHENNAS:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiRuneMohnGUID);
                m_luiFlamewakerGUIDs.clear();
            }
            if (uiData == FAIL)
                DoHandleAdds(m_luiFlamewakerGUIDs);
            break;
        case TYPE_GARR:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiRuneBlazGUID);
                m_luiFireswornGUIDs.clear();
            }
            if (uiData == FAIL)
                DoHandleAdds(m_luiFireswornGUIDs);
            break;
        case TYPE_SHAZZRAH:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiRuneMazjGUID);
            break;
        case TYPE_GEDDON:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiRuneZethGUID);
            break;
        case TYPE_GOLEMAGG:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiRuneTheriGUID);
                DoHandleAdds(m_luiRagerGUIDs, false);
                m_luiRagerGUIDs.clear();
            }
            if (uiData == FAIL)
                DoHandleAdds(m_luiRagerGUIDs);
            break;
        case TYPE_SULFURON:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiRuneKoroGUID);
                m_luiPriestGUIDs.clear();
            }
            if (uiData == FAIL)
                DoHandleAdds(m_luiPriestGUIDs);
            break;
        case TYPE_MAJORDOMO:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(m_uiFirelordCacheGUID);
            break;
        case TYPE_RAGNAROS:
            m_auiEncounter[uiType] = uiData;
            break;
    }

    // Check if Majordomo can be summoned
    if (uiData == DONE)
        DoSpawnMajordomoIfCan(false);

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
            << m_auiEncounter[9];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_molten_core::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

// Handle Majordomo summon here
void instance_molten_core::DoSpawnMajordomoIfCan(bool bByPlayerEnter)
{
    // If already spawned return
    if (m_uiMajordomoGUID)
        return;

    // If both Majordomo and Ragnaros events are finished, return
    if (m_auiEncounter[TYPE_MAJORDOMO] == DONE && m_auiEncounter[TYPE_RAGNAROS] == DONE)
        return;

    // Check if all rune bosses are done
    for(uint8 i = TYPE_MAGMADAR; i < TYPE_MAJORDOMO; i++)
    {
        if (m_auiEncounter[i] != DONE)
            return;
    }

    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    // Summon Majordomo
    // If Majordomo encounter isn't done, summon at encounter place, else near Ragnaros
    uint8 uiSummonPos = m_auiEncounter[TYPE_MAJORDOMO] == DONE ? 1 : 0;
    if (Creature* pMajordomo = pPlayer->SummonCreature(m_aMajordomoLocations[uiSummonPos].m_uiEntry, m_aMajordomoLocations[uiSummonPos].m_fX, m_aMajordomoLocations[uiSummonPos].m_fY, m_aMajordomoLocations[uiSummonPos].m_fZ, m_aMajordomoLocations[uiSummonPos].m_fO, TEMPSUMMON_MANUAL_DESPAWN, 2*HOUR*IN_MILLISECONDS))
    {
        if (uiSummonPos)                                    // Majordomo encounter already done, set faction
        {
            pMajordomo->setFaction(FACTION_MAJORDOMO_FRIENDLY);
            pMajordomo->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            pMajordomo->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
        else                                                // Else yell and summon adds
        {
            if (!bByPlayerEnter)
                DoScriptText(SAY_MAJORDOMO_SPAWN, pMajordomo);

            for (uint8 i = 0; i < MAX_MAJORDOMO_ADDS; ++i)
                pMajordomo->SummonCreature(m_aBosspawnLocs[i].m_uiEntry, m_aBosspawnLocs[i].m_fX, m_aBosspawnLocs[i].m_fY, m_aBosspawnLocs[i].m_fZ, m_aBosspawnLocs[i].m_fO, TEMPSUMMON_MANUAL_DESPAWN, DAY*IN_MILLISECONDS);
        }
    }
}

void instance_molten_core::DoHandleAdds(GUIDList &luiAddsGUIDs, bool bRespawn /*=true*/)
{
    if (luiAddsGUIDs.empty())
        return;

    for (GUIDList::const_iterator itr = luiAddsGUIDs.begin(); itr != luiAddsGUIDs.end(); ++itr)
    {
        if (Creature* pAdd = instance->GetCreature(*itr))
        {
            // Respawn dead mobs (or corpses)
            if (bRespawn && !pAdd->isAlive())
                pAdd->Respawn();
            // Kill adds
            else if (!bRespawn)
                pAdd->DealDamage(pAdd, pAdd->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }
}

uint64 instance_molten_core::GetData64(uint32 uiData)
{
    switch (uiData)
    {
        case NPC_GARR:      return m_uiGarrGUID;
        case NPC_SULFURON:  return m_uiSulfuronGUID;
        case NPC_MAJORDOMO: return m_uiMajordomoGUID;
        case NPC_RAGNAROS:  return m_uiRagnarosGUID;

        default:
            return 0;
    }
}

void instance_molten_core::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);

    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
    >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
    >> m_auiEncounter[8] >> m_auiEncounter[9];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstance_instance_molten_core(Map* pMap)
{
    return new instance_molten_core(pMap);
}

void AddSC_instance_molten_core()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_molten_core";
    pNewScript->GetInstanceData = &GetInstance_instance_molten_core;
    pNewScript->RegisterSelf();
}
