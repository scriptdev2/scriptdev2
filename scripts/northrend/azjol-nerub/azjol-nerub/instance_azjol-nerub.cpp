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
SDName: Instance_Azjol-Nerub
SD%Complete: 50
SDComment:
SDCategory: Azjol-Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

instance_azjol_nerub::instance_azjol_nerub(Map* pMap) : ScriptedInstance(pMap),
    m_uiWatcherTimer(0)
{
    Initialize();
}

void instance_azjol_nerub::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_azjol_nerub::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_KRIKTHIR:
            if (m_auiEncounter[TYPE_KRIKTHIR] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        // TODO - check if these (encounter) doors are really closed by default in the database
        case GO_DOOR_ANUBARAK_1:
            if (m_auiEncounter[TYPE_ANUBARAK] == DONE || m_auiEncounter[TYPE_ANUBARAK] == NOT_STARTED)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_ANUBARAK_2:
            if (m_auiEncounter[TYPE_ANUBARAK] == DONE || m_auiEncounter[TYPE_ANUBARAK] == NOT_STARTED)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_ANUBARAK_3:
            if (m_auiEncounter[TYPE_ANUBARAK] == DONE || m_auiEncounter[TYPE_ANUBARAK] == NOT_STARTED)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_azjol_nerub::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_KRIKTHIR:
        case NPC_GASHRA:
        case NPC_NARJIL:
        case NPC_SILTHIK:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_azjol_nerub::OnCreatureDeath(Creature* pCreature)
{
    uint32 uiEntry = pCreature->GetEntry();
    if (uiEntry == NPC_GASHRA || uiEntry == NPC_NARJIL || uiEntry == NPC_SILTHIK)
    {
        if (m_auiEncounter[TYPE_KRIKTHIR] == NOT_STARTED)
            m_uiWatcherTimer = 5000;
    }
}

void instance_azjol_nerub::OnCreatureEnterCombat(Creature* pCreature)
{
    uint32 uiEntry = pCreature->GetEntry();

    if (uiEntry == NPC_GASHRA || uiEntry == NPC_NARJIL || uiEntry == NPC_SILTHIK)
    {
        // Creature enter combat is not equal to having a victim yet.
        if (!m_playerGuid && pCreature->getVictim())
            m_playerGuid = pCreature->getVictim()->GetCharmerOrOwnerPlayerOrPlayerItself()->GetObjectGuid();
    }
}

void instance_azjol_nerub::OnCreatureEvade(Creature* pCreature)
{
    uint32 uiEntry = pCreature->GetEntry();
    if (uiEntry == NPC_GASHRA || uiEntry == NPC_NARJIL || uiEntry == NPC_SILTHIK)
        m_playerGuid.Clear();
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
    Creature* pKrikthir = GetSingleCreatureFromStorage(NPC_KRIKTHIR);

    if (!pKrikthir)
        return;

    for (uint8 i = 0; i < countof(aWatchers); ++i)
    {
        if (Creature* pTemp = GetSingleCreatureFromStorage(aWatchers[i]))
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

    if (Player* pTarget = instance->GetPlayer(m_playerGuid))
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
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(GO_DOOR_KRIKTHIR);
            break;
        case TYPE_HADRONOX:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_ANUBARAK:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_DOOR_ANUBARAK_1);
            DoUseDoorOrButton(GO_DOOR_ANUBARAK_2);
            DoUseDoorOrButton(GO_DOOR_ANUBARAK_3);
            if (uiData == IN_PROGRESS)
                DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_START_ANUB_ID);
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2];

        m_strInstData = saveStream.str();

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
