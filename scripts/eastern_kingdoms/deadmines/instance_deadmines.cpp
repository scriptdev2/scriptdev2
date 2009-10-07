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

    uint64 m_uiIronCladGUID;
    uint64 m_uiCannonGUID;
    uint64 m_uiSmiteGUID;

    uint32 m_uiIronDoor_Timer;
    uint32 m_uiDoor_Step;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiIronCladGUID = 0;
        m_uiCannonGUID = 0;
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
        if (pGo->GetEntry() == GO_IRON_CLAD)
            m_uiIronCladGUID = pGo->GetGUID();

        if (pGo->GetEntry() == GO_DEFIAS_CANNON)
            m_uiCannonGUID = pGo->GetGUID();
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        if (uiType == TYPE_DEFIAS_ENDDOOR)
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
        }
    }

    uint32 GetData(uint32 uiType)
    {
        if (uiType == TYPE_DEFIAS_ENDDOOR)
            return m_auiEncounter[0];

        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        if (uiData == DATA_DEFIAS_DOOR)
            return m_uiIronCladGUID;

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
                            if (Creature* pi1 = pMrSmite->SummonCreature(NPC_PIRATE, 93.68,-678.63,7.71,2.09, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000))
                                pi1->GetMotionMaster()->MovePoint(0,100.11,-670.65,7.42);
                            if (Creature* pi2 = pMrSmite->SummonCreature(NPC_PIRATE,102.63,-685.07,7.42,1.28, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000))
                                pi2->GetMotionMaster()->MovePoint(0,100.11,-670.65,7.42);
                            ++m_uiDoor_Step;
                            m_uiIronDoor_Timer = 10000;
                            break;
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
