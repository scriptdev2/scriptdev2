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
SDName: instance_eye_of_eternity
SD%Complete: 20
SDComment:
SDCategory: Eye of Eternity
EndScriptData */

#include "precompiled.h"
#include "eye_of_eternity.h"

instance_eye_of_eternity::instance_eye_of_eternity(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_eye_of_eternity::Initialize()
{
    m_uiEncounter = NOT_STARTED;
}

bool instance_eye_of_eternity::IsEncounterInProgress() const
{
    return m_uiEncounter == IN_PROGRESS;
}

void instance_eye_of_eternity::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_MALYGOS:
        case NPC_ALEXSTRASZA:
        case NPC_LARGE_TRIGGER:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_eye_of_eternity::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_EXIT_PORTAL:
        case GO_PLATFORM:
        case GO_HEART_OF_MAGIC:
        case GO_HEART_OF_MAGIC_H:
        case GO_ALEXSTRASZAS_GIFT:
        case GO_ALEXSTRASZAS_GIFT_H:
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
    }
}

void instance_eye_of_eternity::SetData(uint32 uiType, uint32 uiData)
{
    if (uiType != TYPE_MALYGOS)
        return;

    m_uiEncounter = uiData;
    if (uiData == IN_PROGRESS)
    {
        // ToDo: Despawn the exit portal

        DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_START_MALYGOS_ID);
    }
    if (uiData == DONE)
    {
        DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_HEART_OF_MAGIC : GO_HEART_OF_MAGIC_H, 30*MINUTE);
        DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_ALEXSTRASZAS_GIFT : GO_ALEXSTRASZAS_GIFT_H, 30*MINUTE);
    }

    // Currently no reason to save anything
}

InstanceData* GetInstanceData_instance_eye_of_eternity(Map* pMap)
{
    return new instance_eye_of_eternity(pMap);
}

void AddSC_instance_eye_of_eternity()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_eye_of_eternity";
    pNewScript->GetInstanceData = &GetInstanceData_instance_eye_of_eternity;
    pNewScript->RegisterSelf();
}
