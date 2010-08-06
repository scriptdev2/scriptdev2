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
    m_uiStoneKeepersFallen(0)
{
    Initialize();
}

void instance_uldaman::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    m_lWardens.clear();
    m_lKeeperList.clear();
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
        case MOB_HALLSHAPER:
        case MOB_CUSTODIAN:
        case MOB_GUARDIAN:
        case MOB_VAULT_WARDER:
            m_lWardens.push_back(pCreature->GetGUID());
            break;
        case NPC_STONE_KEEPER:
            m_lKeeperList.push_back(pCreature->GetGUID());
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
            if (uiData == SPECIAL)
            {
                ++m_uiStoneKeepersFallen;

                if (m_uiStoneKeepersFallen > 3)
                {
                    uiData = DONE;
                    DoUseDoorOrButton(m_uiTempleDoorUpperGUID);
                    DoUseDoorOrButton(m_uiTempleDoorLowerGUID);
                }

                m_auiEncounter[0] = uiData;
            }
            break;

        case TYPE_ARCHAEDAS:
            if (uiData == FAIL)
            {
                for (std::list<uint64>::iterator itr = m_lWardens.begin(); itr != m_lWardens.end(); ++itr)
                {
                    Creature* pWarden = instance->GetCreature(*itr);
                    if (pWarden && !pWarden->isAlive())
                        pWarden->Respawn();
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

Creature* instance_uldaman::GetDwarf()
{
    std::list<Creature*> lTemp;

    for (std::list<uint64>::iterator itr = m_lWardens.begin(); itr != m_lWardens.end(); ++itr)
    {
       Creature* pTemp = instance->GetCreature(*itr);
       if (pTemp && pTemp->isAlive() && pTemp->HasAura(SPELL_STONED) && 
           (pTemp->GetEntry() == MOB_CUSTODIAN || pTemp->GetEntry() == MOB_HALLSHAPER) )
           lTemp.push_back(pTemp);
    }

    if (lTemp.empty())
        return NULL;

    std::list<Creature*>::iterator i = lTemp.begin();
    advance(i, (rand()%lTemp.size()));
    return (*i);
}

// hack
void instance_uldaman::SimulateSpellHit(uint32 uiCreatureEntry, uint32 uiSpellEntry, Unit* pCaster)
{
    for (std::list<uint64>::iterator itr = m_lWardens.begin(); itr != m_lWardens.end(); ++itr)
    {
       Creature* pTemp = instance->GetCreature(*itr);
       const SpellEntry* pSpell = GetSpellStore()->LookupEntry(uiSpellEntry);
       if (pSpell && pTemp && pTemp->isAlive() && pTemp->GetEntry() == uiCreatureEntry &&
           pTemp->HasAura(SPELL_STONED))
           pTemp->AI()->SpellHit(pCaster, pSpell);
    }
}

void instance_uldaman::Update(uint32 uiDiff)
{
    if (m_auiEncounter[0] == IN_PROGRESS)
    {
        if (!m_lKeeperList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lKeeperList.begin(); itr != m_lKeeperList.end(); ++itr)
            {
                Creature* pKeeper = instance->GetCreature(*itr);

                if (pKeeper && pKeeper->isAlive())
                {
                    Unit* pTarget = Unit::GetUnit(*pKeeper, m_uiPlayerGUID);

                    pKeeper->setFaction(FACTION_TITAN_HOSTILE);
                    pKeeper->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pKeeper->RemoveAurasDueToSpell(SPELL_STONED);

                    if (pTarget && pTarget->isAlive() && pKeeper->AI())
                        pKeeper->AI()->AttackStart(pTarget);
                }
            }

            m_auiEncounter[0] = SPECIAL;
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
