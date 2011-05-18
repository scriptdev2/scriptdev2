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
SDName: Instance_The_Eye
SD%Complete: 100
SDComment:
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "the_eye.h"

instance_the_eye::instance_the_eye(Map* pMap) : ScriptedInstance(pMap),
    m_uiThaladredGUID(0),
    m_uiSanguinarGUID(0),
    m_uiCapernianGUID(0),
    m_uiTelonicusGUID(0),
    m_uiKaelthasGUID(0),
    m_uiAstromancerGUID(0),

    m_uiKaelthasEventPhase(0)
{
    Initialize();
}

void instance_the_eye::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_the_eye::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    if (PHASE_1_ADVISOR <= m_uiKaelthasEventPhase  && m_uiKaelthasEventPhase <= PHASE_5_GRAVITY)
        return true;

    return false;
}

void instance_the_eye::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_THALADRED:   m_uiThaladredGUID = pCreature->GetGUID();   break;
        case NPC_TELONICUS:   m_uiTelonicusGUID = pCreature->GetGUID();   break;
        case NPC_CAPERNIAN:   m_uiCapernianGUID = pCreature->GetGUID();   break;
        case NPC_SANGUINAR:   m_uiSanguinarGUID = pCreature->GetGUID();   break;
        case NPC_KAELTHAS:    m_uiKaelthasGUID = pCreature->GetGUID();    break;
        case NPC_ASTROMANCER: m_uiAstromancerGUID = pCreature->GetGUID(); break;
    }
}

void instance_the_eye::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_ALAR:
        case TYPE_SOLARIAN:
        case TYPE_VOIDREAVER:
            m_auiEncounter[uiType] = uiData;
            break;

        case TYPE_KAELTHAS_PHASE:
            m_uiKaelthasEventPhase = uiData;
            break;
    }
}

uint32 instance_the_eye::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_ALAR:
        case TYPE_SOLARIAN:
        case TYPE_VOIDREAVER:
            return m_auiEncounter[uiType];
        case TYPE_KAELTHAS_PHASE:
            return m_uiKaelthasEventPhase;

        default:
            return 0;
    }
}

uint64 instance_the_eye::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_THALADRED:   return m_uiThaladredGUID;
        case NPC_SANGUINAR:   return m_uiSanguinarGUID;
        case NPC_CAPERNIAN:   return m_uiCapernianGUID;
        case NPC_TELONICUS:   return m_uiTelonicusGUID;
        case NPC_KAELTHAS:    return m_uiKaelthasGUID;
        case NPC_ASTROMANCER: return m_uiAstromancerGUID;

        default:
            return 0;
    }
}

InstanceData* GetInstanceData_instance_the_eye(Map* pMap)
{
    return new instance_the_eye(pMap);
}

void AddSC_instance_the_eye()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_the_eye";
    pNewScript->GetInstanceData = &GetInstanceData_instance_the_eye;
    pNewScript->RegisterSelf();
}
