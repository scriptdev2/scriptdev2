/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: instance_oculus
SD%Complete: 30
SDComment: Only basic support
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

instance_oculus::instance_oculus(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_oculus::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_oculus::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_VAROS:
        case NPC_UROM:
        case NPC_EREGOS:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_oculus::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_CACHE_EREGOS:
        case GO_CACHE_EREGOS_H:
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
    }
}

void instance_oculus::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_DRAKOS:
            m_auiEncounter[TYPE_DRAKOS] = uiData;
            break;
        case TYPE_VAROS:
            m_auiEncounter[TYPE_VAROS] = uiData;
            break;
        case TYPE_UROM:
            m_auiEncounter[TYPE_UROM] = uiData;
            break;
        case TYPE_EREGOS:
            m_auiEncounter[TYPE_EREGOS] = uiData;
            if (uiData == DONE)
                DoToggleGameObjectFlags(instance->IsRegularDifficulty() ? GO_CACHE_EREGOS : GO_CACHE_EREGOS_H, GO_FLAG_NO_INTERACT, false);
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[TYPE_DRAKOS] << " " << m_auiEncounter[TYPE_VAROS] << " " << m_auiEncounter[TYPE_UROM] << " " << m_auiEncounter[TYPE_EREGOS];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_oculus::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_oculus::OnCreatureEnterCombat(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_DRAKOS: SetData(TYPE_DRAKOS, IN_PROGRESS); break;
        case NPC_VAROS:  SetData(TYPE_VAROS,  IN_PROGRESS); break;
    }
}

void instance_oculus::OnCreatureEvade(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_DRAKOS: SetData(TYPE_DRAKOS, FAIL); break;
        case NPC_VAROS:  SetData(TYPE_VAROS,  FAIL); break;
    }
}

void instance_oculus::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_DRAKOS: SetData(TYPE_DRAKOS, DONE); break;
        case NPC_VAROS:  SetData(TYPE_VAROS,  DONE); break;
    }
}

void instance_oculus::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[TYPE_DRAKOS] >> m_auiEncounter[TYPE_VAROS] >> m_auiEncounter[TYPE_UROM] >> m_auiEncounter[TYPE_EREGOS];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_oculus(Map* pMap)
{
    return new instance_oculus(pMap);
}

void AddSC_instance_oculus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_oculus";
    pNewScript->GetInstanceData = &GetInstanceData_instance_oculus;
    pNewScript->RegisterSelf();
}
