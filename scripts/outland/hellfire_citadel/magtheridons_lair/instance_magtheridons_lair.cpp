/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Instance_Magtheridons_Lair
SD%Complete: 100
SDComment:
SDCategory: Hellfire Citadel, Magtheridon's lair
EndScriptData */

#include "precompiled.h"
#include "magtheridons_lair.h"

instance_magtheridons_lair::instance_magtheridons_lair(Map* pMap) : ScriptedInstance(pMap) { Initialize(); }

void instance_magtheridons_lair::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

    m_uiCageTimer = 0;
    m_uiRespawnTimer = 0;
}

bool instance_magtheridons_lair::IsEncounterInProgress() const
{
    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;

    return false;
}

void instance_magtheridons_lair::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_MAGTHERIDON:
            m_mNpcEntryGuidStore[NPC_MAGTHERIDON] = pCreature->GetObjectGuid();
            break;
        case NPC_CHANNELER:
            m_sChannelerGuid.insert(pCreature->GetObjectGuid());
            break;
    }
}

void instance_magtheridons_lair::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_MANTICRON_CUBE:
            pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;
        case GO_DOODAD_HF_MAG_DOOR01:                       // event door
            m_mGoEntryGuidStore[GO_DOODAD_HF_MAG_DOOR01] = pGo->GetObjectGuid();
            break;
        case GO_DOODAD_HF_RAID_FX01:                        // hall
        case GO_MAGTHERIDON_COLUMN_003:                     // six columns
        case GO_MAGTHERIDON_COLUMN_002:
        case GO_MAGTHERIDON_COLUMN_004:
        case GO_MAGTHERIDON_COLUMN_005:
        case GO_MAGTHERIDON_COLUMN_000:
        case GO_MAGTHERIDON_COLUMN_001:
            m_sColumnGuid.insert(pGo->GetObjectGuid());
            break;
    }
}

void instance_magtheridons_lair::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_MAGTHERIDON_EVENT:
            m_auiEncounter[0] = uiData;
            if (uiData == NOT_STARTED)
                m_uiRespawnTimer = 10000;
            if (uiData != IN_PROGRESS)
            {
                if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_DOODAD_HF_MAG_DOOR01))
                    pDoor->SetGoState(GO_STATE_ACTIVE);
            }
            break;
        case TYPE_CHANNELER_EVENT:
            switch(uiData)
            {
                case NOT_STARTED:                           // Reset all channelers once one is reset.
                    if (m_auiEncounter[1] != NOT_STARTED)
                    {
                        m_auiEncounter[1] = NOT_STARTED;

                        if (m_sChannelerGuid.empty())
                        {
                            debug_log("SD2: Instance Magtheridon: Channeler GUID list are empty.");
                            break;
                        }

                        for(GUIDSet::const_iterator itr = m_sChannelerGuid.begin(); itr != m_sChannelerGuid.end(); ++itr)
                        {
                            if (Creature* pChanneler = instance->GetCreature(*itr))
                            {
                                if (pChanneler->isAlive())
                                    pChanneler->AI()->EnterEvadeMode();
                                else
                                    pChanneler->Respawn();
                            }
                        }

                        m_uiCageTimer = 0;

                        if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_DOODAD_HF_MAG_DOOR01))
                            pDoor->SetGoState(GO_STATE_ACTIVE);
                    }
                    break;
                case IN_PROGRESS:                           // Event start.
                    if (m_auiEncounter[1] != IN_PROGRESS)
                    {
                        m_auiEncounter[1] = IN_PROGRESS;

                        // Let all five channelers aggro.
                        for(GUIDSet::const_iterator itr = m_sChannelerGuid.begin(); itr != m_sChannelerGuid.end(); ++itr)
                        {
                            Creature* pChanneler = instance->GetCreature(*itr);

                            if (pChanneler && pChanneler->isAlive())
                                AttackNearestTarget(pChanneler);
                        }

                        // Magtheridon breaks free after two minutes.
                        Creature* pMagtheridon = GetSingleCreatureFromStorage(NPC_MAGTHERIDON);

                        if (pMagtheridon && pMagtheridon->isAlive())
                            m_uiCageTimer = 120000;

                        if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_DOODAD_HF_MAG_DOOR01))
                            pDoor->SetGoState(GO_STATE_READY);
                    }
                    break;
                case DONE:                                  // Add buff and check if all channelers are dead.
                    for(GUIDSet::iterator itr = m_sChannelerGuid.begin(); itr != m_sChannelerGuid.end(); ++itr)
                    {
                        Creature* pChanneler = instance->GetCreature(*itr);

                        if (pChanneler && pChanneler->isAlive())
                        {
                            //Channeler->InterruptNonMeleeSpells(false);
                            //Channeler->CastSpell(Channeler, SPELL_SOUL_TRANSFER, false);
                            uiData = IN_PROGRESS;
                            break;
                        }
                    }
                    break;
            }
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_HALL_COLLAPSE:
            // IN_PROGRESS - collapse / NOT_STARTED - reset
            for(GUIDSet::const_iterator itr = m_sColumnGuid.begin(); itr != m_sColumnGuid.end(); ++itr)
                DoUseDoorOrButton(*itr);
            break;
    }
}

uint32 instance_magtheridons_lair::GetData(uint32 uiType)
{
    if (uiType == TYPE_MAGTHERIDON_EVENT)
        return m_auiEncounter[0];
    if (uiType == TYPE_CHANNELER_EVENT)
        return m_auiEncounter[1];

    return 0;
}

void instance_magtheridons_lair::AttackNearestTarget(Creature* pCreature)
{
    float minRange = VISIBLE_RANGE;
    float range;
    Player* target = NULL;

    Map::PlayerList const& players = instance->GetPlayers();
    for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        if (Player* i_pl = itr->getSource())
        {
            if (i_pl->isTargetableForAttack())
            {
                range = i_pl->GetDistance(pCreature);
                if (range < minRange)
                {
                    minRange = range;
                    target = i_pl;
                }
            }
        }
    }

    if (!target)
    {
        debug_log("SD2: Instance Magtheridon: AttackNearestTarget failed. No player.");
        return;
    }
    pCreature->AI()->AttackStart(target);
}

void instance_magtheridons_lair::Update(uint32 uiDiff)
{
    if (m_uiCageTimer)
    {
        if (m_uiCageTimer <= uiDiff)
        {
            SetData(TYPE_MAGTHERIDON_EVENT, IN_PROGRESS);
            m_uiCageTimer = 0;
        }
        else
            m_uiCageTimer -= uiDiff;
    }

    if (m_uiRespawnTimer)
    {
        if (m_uiRespawnTimer <= uiDiff)
        {
            for(GUIDSet::const_iterator itr = m_sChannelerGuid.begin(); itr != m_sChannelerGuid.end(); ++itr)
            {
                if (Creature* pChanneler = instance->GetCreature(*itr))
                {
                    if (pChanneler->isAlive())
                        pChanneler->AI()->EnterEvadeMode();
                    else
                        pChanneler->Respawn();
                }
            }

            m_uiRespawnTimer = 0;
        }
        else
            m_uiRespawnTimer -= uiDiff;
    }
}

InstanceData* GetInstanceData_instance_magtheridons_lair(Map* pMap)
{
    return new instance_magtheridons_lair(pMap);
}

void AddSC_instance_magtheridons_lair()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_magtheridons_lair";
    pNewScript->GetInstanceData = &GetInstanceData_instance_magtheridons_lair;
    pNewScript->RegisterSelf();
}
