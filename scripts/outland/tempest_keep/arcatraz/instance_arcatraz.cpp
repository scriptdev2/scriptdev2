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
SDName: Instance_Arcatraz
SD%Complete: 80
SDComment: Mainly Harbringer Skyriss event
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

#include "precompiled.h"
#include "arcatraz.h"

/* Arcatraz encounters:
1 - Zereketh the Unbound event
2 - Dalliah the Doomsayer event
3 - Wrath-Scryer Soccothrates event
4 - Harbinger Skyriss event, 5 sub-events
*/

instance_arcatraz::instance_arcatraz(Map* pMap) : ScriptedInstance(pMap) { Initialize(); }

void instance_arcatraz::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_arcatraz::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_CORE_SECURITY_FIELD_ALPHA:
        case GO_CORE_SECURITY_FIELD_BETA:
        case GO_SEAL_SPHERE:
        case GO_POD_ALPHA:
        case GO_POD_BETA:
        case GO_POD_DELTA:
        case GO_POD_GAMMA:
        case GO_POD_OMEGA:
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
    }
}

void instance_arcatraz::OnCreatureCreate(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_MELLICHAR)
        m_mNpcEntryGuidStore[NPC_MELLICHAR] = pCreature->GetObjectGuid();
}

void instance_arcatraz::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ZEREKETH:
            m_auiEncounter[0] = uiData;
            break;

        case TYPE_DALLIAH:
            if (uiData == DONE)
                DoUseDoorOrButton(GO_CORE_SECURITY_FIELD_BETA);
            m_auiEncounter[1] = uiData;
            break;

        case TYPE_SOCCOTHRATES:
            if (uiData == DONE)
                DoUseDoorOrButton(GO_CORE_SECURITY_FIELD_ALPHA);
            m_auiEncounter[2] = uiData;
            break;

        case TYPE_HARBINGERSKYRISS:
            if (uiData == NOT_STARTED || uiData == FAIL)
            {
                m_auiEncounter[4] = NOT_STARTED;
                m_auiEncounter[5] = NOT_STARTED;
                m_auiEncounter[6] = NOT_STARTED;
                m_auiEncounter[7] = NOT_STARTED;
                m_auiEncounter[8] = NOT_STARTED;
            }
            m_auiEncounter[3] = uiData;
            break;

        case TYPE_WARDEN_1:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_ALPHA);
            m_auiEncounter[4] = uiData;
            break;

        case TYPE_WARDEN_2:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_BETA);
            m_auiEncounter[5] = uiData;
            break;

        case TYPE_WARDEN_3:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_DELTA);
            m_auiEncounter[6] = uiData;
            break;

        case TYPE_WARDEN_4:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_GAMMA);
            m_auiEncounter[7] = uiData;
            break;

        case TYPE_WARDEN_5:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_OMEGA);
            m_auiEncounter[8] = uiData;
            break;
    }
}

uint32 instance_arcatraz::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_HARBINGERSKYRISS: return m_auiEncounter[3];
        case TYPE_WARDEN_1:         return m_auiEncounter[4];
        case TYPE_WARDEN_2:         return m_auiEncounter[5];
        case TYPE_WARDEN_3:         return m_auiEncounter[6];
        case TYPE_WARDEN_4:         return m_auiEncounter[7];
        case TYPE_WARDEN_5:         return m_auiEncounter[8];

        default:
            return 0;
    }
}

InstanceData* GetInstanceData_instance_arcatraz(Map* pMap)
{
    return new instance_arcatraz(pMap);
}

void AddSC_instance_arcatraz()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_arcatraz";
    pNewScript->GetInstanceData = &GetInstanceData_instance_arcatraz;
    pNewScript->RegisterSelf();
}
