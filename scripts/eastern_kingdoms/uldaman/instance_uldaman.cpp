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
SDName: Instance_Uldaman
SD%Complete: 60
SDComment:
SDCategory: Uldaman
EndScriptData
*/

#include "precompiled.h"
#include "uldaman.h"

instance_uldaman::instance_uldaman(Map* pMap) : ScriptedInstance(pMap),
    m_uiTempleDoorUpperGUID(0),
    m_uiTempleDoorLowerGUID(0),
    m_uiAncientVaultGUID(0),
    m_uiPlayerGUID(0),
    m_uiStoneKeepersFallen(0),
    m_uiKeeperCooldown(5000)
{
    Initialize();
}

void instance_uldaman::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    m_lWardens.clear();
    m_mKeeperMap.clear();
}

void instance_uldaman::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_TEMPLE_DOOR_UPPER:
            if (m_auiEncounter[0] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiTempleDoorUpperGUID = pGo->GetGUID();
            break;
        case GO_TEMPLE_DOOR_LOWER:
            if (m_auiEncounter[0] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiTempleDoorLowerGUID = pGo->GetGUID();
            break;
        case GO_ANCIENT_VAULT:
            if (m_auiEncounter[1] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiAncientVaultGUID = pGo->GetGUID();
            break;
        default:
            break;
    }
}

void instance_uldaman::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_HALLSHAPER:
        case NPC_CUSTODIAN:
        case NPC_GUARDIAN:
        case NPC_VAULT_WARDER:
            m_lWardens.push_back(pCreature->GetGUID());
            pCreature->CastSpell(pCreature, SPELL_STONED, true);
            pCreature->SetNoCallAssistance(true);           // no assistance
            break;
        case NPC_STONE_KEEPER:
            m_mKeeperMap[pCreature->GetGUID()] = pCreature->isAlive();
            pCreature->CastSpell(pCreature, SPELL_STONED, true);
            pCreature->SetNoCallAssistance(true);           // no assistance
            break;
        default:
            break;
    }
}

void instance_uldaman::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ALTAR_EVENT:
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiTempleDoorUpperGUID);
                DoUseDoorOrButton(m_uiTempleDoorLowerGUID);

                m_auiEncounter[0] = uiData;
            }
            break;

        case TYPE_ARCHAEDAS:
            if (uiData == FAIL)
            {
                for (std::list<uint64>::iterator itr = m_lWardens.begin(); itr != m_lWardens.end(); ++itr)
                {
                    if (Creature* pWarden = instance->GetCreature(*itr))
                    {
                        pWarden->SetDeathState(JUST_DIED);
                        pWarden->Respawn();
                        pWarden->SetNoCallAssistance(true);
                    }
                }
            }
            else if (uiData == DONE)
            {
                for (std::list<uint64>::iterator itr = m_lWardens.begin(); itr != m_lWardens.end(); ++itr)
                {
                    Creature* pWarden = instance->GetCreature(*itr);
                    if (pWarden && pWarden->isAlive())
                        pWarden->ForcedDespawn();
                }
                DoUseDoorOrButton(m_uiAncientVaultGUID);
            }
            m_auiEncounter[1] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1];

        strInstData = saveStream.str();
        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_uldaman::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_uldaman::SetData64(uint32 uiData, uint64 uiGuid)
{
    switch(uiData)
    {
        case DATA_EVENT_STARTER:
            m_uiPlayerGUID = uiGuid;
        break;
    }
}

uint32 instance_uldaman::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_ARCHAEDAS:
            return m_auiEncounter[1];
    }
    return 0;
}

uint64 instance_uldaman::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case DATA_EVENT_STARTER:
            return m_uiPlayerGUID;
    }
    return 0;
}

void instance_uldaman::StartEvent(uint32 uiEventId, Player* pPlayer)
{
    m_uiPlayerGUID = pPlayer->GetGUID();

    if (uiEventId == EVENT_ID_ALTAR_KEEPER)
    {
        if (m_auiEncounter[0] == NOT_STARTED)
            m_auiEncounter[0] = IN_PROGRESS;
    }
    else if (m_auiEncounter[1] == NOT_STARTED || m_auiEncounter[1] == FAIL)
        m_auiEncounter[1] = SPECIAL;
}

void instance_uldaman::DoResetKeeperEvent()
{
    m_auiEncounter[0] = NOT_STARTED;
    m_uiStoneKeepersFallen = 0;

    for (std::map<uint64, bool>::iterator itr = m_mKeeperMap.begin(); itr != m_mKeeperMap.end(); ++itr)
    {
        if (Creature* pKeeper = instance->GetCreature(itr->first))
        {
            pKeeper->SetDeathState(JUST_DIED);
            pKeeper->Respawn();
            pKeeper->CastSpell(pKeeper, SPELL_STONED, true);
            pKeeper->SetNoCallAssistance(true);
            itr->second = true;
        }
    }
}

Creature* instance_uldaman::GetClosestDwarfNotInCombat(Creature* pSearcher, uint32 uiPhase)
{
    std::list<Creature*> lTemp;

    for (std::list<uint64>::iterator itr = m_lWardens.begin(); itr != m_lWardens.end(); ++itr)
    {
        Creature* pTemp = instance->GetCreature(*itr);

        if (pTemp && pTemp->isAlive() && !pTemp->getVictim())
        {
            switch(uiPhase)
            {
                case PHASE_ARCHA_1:
                    if (pTemp->GetEntry() != NPC_CUSTODIAN && pTemp->GetEntry() != NPC_HALLSHAPER)
                        continue;
                    break;
                case PHASE_ARCHA_2:
                    if (pTemp->GetEntry() != NPC_GUARDIAN)
                        continue;
                    break;
                case PHASE_ARCHA_3:
                    if (pTemp->GetEntry() != NPC_VAULT_WARDER)
                        continue;
                    break;
            }

            lTemp.push_back(pTemp);
        }
    }

    if (lTemp.empty())
        return NULL;

    lTemp.sort(ObjectDistanceOrder(pSearcher));
    return lTemp.front();
}

void instance_uldaman::Update(uint32 uiDiff)
{
    if (m_auiEncounter[0] == IN_PROGRESS)
    {
        if (m_uiKeeperCooldown >= uiDiff)
            m_uiKeeperCooldown -= uiDiff;
        else
        {
            m_uiKeeperCooldown = 5000;

            if (!m_mKeeperMap.empty())
            {
                for(std::map<uint64, bool>::iterator itr = m_mKeeperMap.begin(); itr != m_mKeeperMap.end(); ++itr)
                {
                    // died earlier
                    if (!itr->second)
                        continue;

                    if (Creature* pKeeper = instance->GetCreature(itr->first))
                    {
                        if (pKeeper->isAlive() && !pKeeper->getVictim())
                        {
                            if (Player* pPlayer = pKeeper->GetMap()->GetPlayer(m_uiPlayerGUID))
                            {
                                // we should use group instead, event starter can be dead while group is still fighting
                                if (pPlayer->isAlive() && !pPlayer->isInCombat())
                                {
                                    pKeeper->RemoveAurasDueToSpell(SPELL_STONED);
                                    pKeeper->SetInCombatWith(pPlayer);
                                    pKeeper->AddThreat(pPlayer);
                                }
                                else
                                {
                                    if (!pPlayer->isAlive())
                                        DoResetKeeperEvent();
                                }
                            }

                            break;
                        }
                        else if (!pKeeper->isAlive())
                        {
                            itr->second = pKeeper->isAlive();
                            ++m_uiStoneKeepersFallen;
                        }
                    }
                }

                if (m_uiStoneKeepersFallen == m_mKeeperMap.size())
                    SetData(TYPE_ALTAR_EVENT, DONE);
            }
        }
    }
}

InstanceData* GetInstanceData_instance_uldaman(Map* pMap)
{
    return new instance_uldaman(pMap);
}

bool ProcessEventId_event_spell_altar_boss_aggro(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (instance_uldaman* pInstance = (instance_uldaman*)((Player*)pSource)->GetInstanceData())
        {
            pInstance->StartEvent(uiEventId, (Player*)pSource);
            return true;
        }
    }
    return false;
}

void AddSC_instance_uldaman()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_uldaman";
    pNewScript->GetInstanceData = &GetInstanceData_instance_uldaman;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_spell_altar_boss_aggro";
    pNewScript->pProcessEventId = &ProcessEventId_event_spell_altar_boss_aggro;
    pNewScript->RegisterSelf();
}
