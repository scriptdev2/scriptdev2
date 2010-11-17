/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Instance_Azjol-Nerub
SD%Complete: 50
SDComment:
SDCategory: Azjol-Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

instance_azjol_nerub::instance_azjol_nerub(Map* pMap) : ScriptedInstance(pMap),
    m_uiDoorKrikthirGUID(0),
    m_uiDoorAnubarak1GUID(0),
    m_uiDoorAnubarak2GUID(0),
    m_uiDoorAnubarak3GUID(0),

    m_uiKrikthirGUID(0),
    m_uiGashraGUID(0),
    m_uiNarjilGUID(0),
    m_uiSilthikGUID(0),

    m_uiPlayerGUID(0),

    m_uiWatcherTimer(0)
{
    Initialize();
}

void instance_azjol_nerub::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiWatcherGUIDS, 0, sizeof(m_auiWatcherGUIDS));
}

void instance_azjol_nerub::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_KRIKTHIR:
            m_uiDoorKrikthirGUID = pGo->GetGUID();
            if (m_auiEncounter[0] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_ANUBARAK_1:
            m_uiDoorAnubarak1GUID = pGo->GetGUID();
            if (m_auiEncounter[2] == DONE || m_auiEncounter[2] == NOT_STARTED)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_ANUBARAK_2:
            m_uiDoorAnubarak2GUID = pGo->GetGUID();
            if (m_auiEncounter[2] == DONE || m_auiEncounter[2] == NOT_STARTED)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_ANUBARAK_3:
            m_uiDoorAnubarak3GUID = pGo->GetGUID();
            if (m_auiEncounter[2] == DONE || m_auiEncounter[2] == NOT_STARTED)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
    }
}

void instance_azjol_nerub::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_KRIKTHIR: m_uiKrikthirGUID = pCreature->GetGUID();     break;
        case NPC_GASHRA:   m_auiWatcherGUIDS[0] = pCreature->GetGUID(); break;
        case NPC_NARJIL:   m_auiWatcherGUIDS[1] = pCreature->GetGUID(); break;
        case NPC_SILTHIK:  m_auiWatcherGUIDS[2] = pCreature->GetGUID(); break;
    }
}

void instance_azjol_nerub::OnCreatureDeath(Creature* pCreature)
{
    uint32 uiEntry = pCreature->GetEntry();
    if (uiEntry == NPC_GASHRA || uiEntry == NPC_NARJIL || uiEntry == NPC_SILTHIK)
    {
        if (m_auiEncounter[0] == NOT_STARTED)
            m_uiWatcherTimer = 5000;
    }
}

void instance_azjol_nerub::OnCreatureEnterCombat(Creature* pCreature)
{
    uint32 uiEntry = pCreature->GetEntry();

    if (uiEntry == NPC_GASHRA || uiEntry == NPC_NARJIL || uiEntry == NPC_SILTHIK)
    {
        // Creature enter combat is not equal to having a victim yet.
        if (!m_uiPlayerGUID && pCreature->getVictim())
            m_uiPlayerGUID = pCreature->getVictim()->GetCharmerOrOwnerPlayerOrPlayerItself()->GetGUID();
    }
}

void instance_azjol_nerub::OnCreatureEvade(Creature* pCreature)
{
    uint32 uiEntry = pCreature->GetEntry();
    if (uiEntry == NPC_GASHRA || uiEntry == NPC_NARJIL || uiEntry == NPC_SILTHIK)
        m_uiPlayerGUID = 0;
}

void instance_azjol_nerub::Update(uint32 uiDiff)
{
    if (m_uiWatcherTimer)
    {
        if (m_uiWatcherTimer <= uiDiff)
        {
            DoSendWatcherOrKrikthir();
            m_uiWatcherTimer = 0;
        }
        else
            m_uiWatcherTimer -= uiDiff;
    }
}

void instance_azjol_nerub::DoSendWatcherOrKrikthir()
{
    Creature* pAttacker = NULL;
    Creature* pKrikthir = instance->GetCreature(m_uiKrikthirGUID);

    if (!pKrikthir)
        return;

    for (uint8 i = 0; i < MAX_WATCHERS; ++i)
    {
        if (Creature* pTemp = instance->GetCreature(m_auiWatcherGUIDS[i]))
        {
            if (pTemp->isAlive())
            {
                if (pAttacker && urand(0, 1))
                    continue;
                else
                    pAttacker = pTemp;
            }
        }
    }

    if (pAttacker)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SEND_GROUP_1, pKrikthir); break;
            case 1: DoScriptText(SAY_SEND_GROUP_2, pKrikthir); break;
            case 2: DoScriptText(SAY_SEND_GROUP_3, pKrikthir); break;
        }
    }
    else
        pAttacker = pKrikthir;

    if (Unit* pTarget = instance->GetUnit(m_uiPlayerGUID))
    {
        if (pTarget->isAlive())
            pAttacker->AI()->AttackStart(pTarget);
    }
}

void instance_azjol_nerub::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_KRIKTHIR:
            m_auiEncounter[0] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiDoorKrikthirGUID);
            break;
        case TYPE_HADRONOX:
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_ANUBARAK:
            m_auiEncounter[2] = uiData;
            DoUseDoorOrButton(m_uiDoorAnubarak1GUID);
            DoUseDoorOrButton(m_uiDoorAnubarak2GUID);
            DoUseDoorOrButton(m_uiDoorAnubarak3GUID);
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_azjol_nerub::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_azjol_nerub(Map* pMap)
{
    return new instance_azjol_nerub(pMap);
}

void AddSC_instance_azjol_nerub()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_azjol-nerub";
    pNewScript->GetInstanceData = &GetInstanceData_instance_azjol_nerub;
    pNewScript->RegisterSelf();
}
