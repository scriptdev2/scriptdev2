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
SDName: Instance_Deadmines
SD%Complete: 0
SDComment: Placeholder
SDCategory: Deadmines
EndScriptData */

#include "precompiled.h"
#include "deadmines.h"

instance_deadmines::instance_deadmines(Map* pMap) : ScriptedInstance(pMap),
    m_uiIronDoorTimer(0),
    m_uiDoorStep(0)
{
    Initialize();
}

void instance_deadmines::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_deadmines::OnCreatureCreate(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_MR_SMITE)
        m_mNpcEntryGuidStore[NPC_MR_SMITE] = pCreature->GetObjectGuid();
}

void instance_deadmines::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_FACTORY_DOOR:
            if (GetData(TYPE_RHAHKZOR) == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);

            break;
        case GO_MAST_ROOM_DOOR:
            if (GetData(TYPE_SNEED) == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);

            break;
        case GO_FOUNDRY_DOOR:
            if (GetData(TYPE_GILNID) == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);

            break;
        case GO_IRON_CLAD_DOOR:
        case GO_DEFIAS_CANNON:
        case GO_SMITE_CHEST:
            break;

        default:
            return;
    }

    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_deadmines::OnCreatureDeath(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_RHAHKZOR:
            SetData(TYPE_RHAHKZOR, DONE);
            break;
        case NPC_SNEED:
            SetData(TYPE_SNEED, DONE);
            break;
        case NPC_GILNID:
            SetData(TYPE_GILNID, DONE);
            break;
    }
}

void instance_deadmines::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_RHAHKZOR:
        {
            if (uiData == DONE)
                DoUseDoorOrButton(GO_FACTORY_DOOR);

            m_auiEncounter[1] = uiData;
            break;
        }
        case TYPE_SNEED:
        {
            if (uiData == DONE)
                DoUseDoorOrButton(GO_MAST_ROOM_DOOR);

            m_auiEncounter[2] = uiData;
            break;
        }
        case TYPE_GILNID:
        {
            if (uiData == DONE)
                DoUseDoorOrButton(GO_FOUNDRY_DOOR);

            m_auiEncounter[3] = uiData;
            break;
        }
        case TYPE_DEFIAS_ENDDOOR:
        {
            if (uiData == IN_PROGRESS)
            {
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_IRON_CLAD_DOOR))
                {
                    pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                    m_uiIronDoorTimer = 3000;
                }
            }
            m_auiEncounter[0] = uiData;
            break;
        }
    }
}

uint32 instance_deadmines::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_DEFIAS_ENDDOOR:
            return m_auiEncounter[0];
        case TYPE_RHAHKZOR:
            return m_auiEncounter[1];
        case TYPE_SNEED:
            return m_auiEncounter[2];
        case TYPE_GILNID:
            return m_auiEncounter[3];
    }

    return 0;
}

void instance_deadmines::Update(uint32 uiDiff)
{
    if (m_uiIronDoorTimer)
    {
        if (m_uiIronDoorTimer <= uiDiff)
        {
            if (Creature* pMrSmite = GetSingleCreatureFromStorage(NPC_MR_SMITE))
            {
                switch(m_uiDoorStep)
                {
                    case 0:
                        DoScriptText(INST_SAY_ALARM1,pMrSmite);
                        m_uiIronDoorTimer = 2000;
                        ++m_uiDoorStep;
                        break;
                    case 1:
                    {
                        if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_IRON_CLAD_DOOR))
                        {
                            // should be static spawns, fetch the closest ones at the pier
                            if (Creature* pi1 = GetClosestCreatureWithEntry(pDoor, NPC_PIRATE, 40.0f))
                            {
                                pi1->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pi1->GetMotionMaster()->MovePoint(0, pDoor->GetPositionX(), pDoor->GetPositionY(), pDoor->GetPositionZ());
                            }

                            if (Creature* pi2 = GetClosestCreatureWithEntry(pDoor, NPC_SQUALLSHAPER, 40.0f))
                            {
                                pi2->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pi2->GetMotionMaster()->MovePoint(0, pDoor->GetPositionX(), pDoor->GetPositionY(), pDoor->GetPositionZ());
                            }
                        }

                        ++m_uiDoorStep;
                        m_uiIronDoorTimer = 10000;
                        break;
                    }
                    case 2:
                        DoScriptText(INST_SAY_ALARM2,pMrSmite);
                        m_uiDoorStep = 0;
                        m_uiIronDoorTimer = 0;
                        debug_log("SD2: Instance Deadmines: Iron door event reached end.");
                        break;
                }
            }
            else
                m_uiIronDoorTimer = 0;
        }
        else
            m_uiIronDoorTimer -= uiDiff;
    }
}

InstanceData* GetInstanceData_instance_deadmines(Map* pMap)
{
    return new instance_deadmines(pMap);
}

void AddSC_instance_deadmines()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_deadmines";
    pNewScript->GetInstanceData = &GetInstanceData_instance_deadmines;
    pNewScript->RegisterSelf();
}
