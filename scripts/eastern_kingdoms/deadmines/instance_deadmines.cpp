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

struct MANGOS_DLL_DECL instance_deadmines : public ScriptedInstance
{
    instance_deadmines(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiFactoryDoorGUID;
    uint64 m_uiMastRoomDoorGUID;
    uint64 m_uiFoundryDoorGUID;
    uint64 m_uiIronCladGUID;
    uint64 m_uiCannonGUID;
    uint64 m_uiSmiteChestGUID;
    uint64 m_uiSmiteGUID;

    uint32 m_uiIronDoor_Timer;
    uint32 m_uiDoor_Step;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiFactoryDoorGUID = 0;
        m_uiMastRoomDoorGUID = 0;
        m_uiFoundryDoorGUID = 0;
        m_uiIronCladGUID = 0;
        m_uiCannonGUID = 0;
        m_uiSmiteChestGUID = 0;
        m_uiSmiteGUID = 0;

        m_uiIronDoor_Timer = 0;
        m_uiDoor_Step = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        if (pCreature->GetEntry() == NPC_MR_SMITE)
            m_uiSmiteGUID = pCreature->GetGUID();
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_FACTORY_DOOR:
                m_uiFactoryDoorGUID = pGo->GetGUID();

                if (GetData(TYPE_RHAHKZOR) == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);

                break;
            case GO_MAST_ROOM_DOOR:
                m_uiMastRoomDoorGUID = pGo->GetGUID();

                if (GetData(TYPE_SNEED) == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);

                break;
            case GO_FOUNDRY_DOOR:
                m_uiFoundryDoorGUID = pGo->GetGUID();

                if (GetData(TYPE_GILNID) == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);

                break;
            case GO_IRON_CLAD_DOOR:
                m_uiIronCladGUID = pGo->GetGUID();
                break;
            case GO_DEFIAS_CANNON:
                m_uiCannonGUID = pGo->GetGUID();
                break;
            case GO_SMITE_CHEST:
                m_uiSmiteChestGUID = pGo->GetGUID();
                break;
        }
    }

    void OnCreatureDeath(Creature* pCreature)
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

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_RHAHKZOR:
            {
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiFactoryDoorGUID);

                m_auiEncounter[1] = uiData;
                break;
            }
            case TYPE_SNEED:
            {
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiMastRoomDoorGUID);

                m_auiEncounter[2] = uiData;
                break;
            }
            case TYPE_GILNID:
            {
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiFoundryDoorGUID);

                m_auiEncounter[3] = uiData;
                break;
            }
            case TYPE_DEFIAS_ENDDOOR:
            {
                if (uiData == IN_PROGRESS)
                {
                    if (GameObject* pGo = instance->GetGameObject(m_uiIronCladGUID))
                    {
                        pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                        m_uiIronDoor_Timer = 3000;
                    }
                }
                m_auiEncounter[0] = uiData;
                break;
            }
        }
    }

    uint32 GetData(uint32 uiType)
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

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case GO_IRON_CLAD_DOOR:
                return m_uiIronCladGUID;
            case GO_SMITE_CHEST:
                return m_uiSmiteChestGUID;
        }

        return 0;
    }

    void Update(uint32 uiDiff)
    {
        if (m_uiIronDoor_Timer)
        {
            if (m_uiIronDoor_Timer <= uiDiff)
            {
                if (Creature* pMrSmite = instance->GetCreature(m_uiSmiteGUID))
                {
                    switch(m_uiDoor_Step)
                    {
                        case 0:
                            DoScriptText(INST_SAY_ALARM1,pMrSmite);
                            m_uiIronDoor_Timer = 2000;
                            ++m_uiDoor_Step;
                            break;
                        case 1:
                        {
                            if (GameObject* pDoor = instance->GetGameObject(m_uiIronCladGUID))
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

                            ++m_uiDoor_Step;
                            m_uiIronDoor_Timer = 10000;
                            break;
                        }
                        case 2:
                            DoScriptText(INST_SAY_ALARM2,pMrSmite);
                            m_uiDoor_Step = 0;
                            m_uiIronDoor_Timer = 0;
                            debug_log("SD2: Instance Deadmines: Iron door event reached end.");
                            break;
                    }
                }
                else
                    m_uiIronDoor_Timer = 0;
            }
            else
                m_uiIronDoor_Timer -= uiDiff;
        }
    }
};

InstanceData* GetInstanceData_instance_deadmines(Map* pMap)
{
    return new instance_deadmines(pMap);
}

void AddSC_instance_deadmines()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_deadmines";
    newscript->GetInstanceData = &GetInstanceData_instance_deadmines;
    newscript->RegisterSelf();
}
