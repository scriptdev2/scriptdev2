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
SDName: Instance_ZulGurub
SD%Complete: 80
SDComment: Missing reset function after killing a boss for Ohgan, Thekal.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

instance_zulgurub::instance_zulgurub(Map* pMap) : ScriptedInstance(pMap),
    m_uiLorKhanGUID(0),
    m_uiZathGUID(0),
    m_uiThekalGUID(0),
    m_uiJindoGUID(0),
    m_uiHakkarGUID(0),
    m_bHasIntroYelled(false),
    m_bHasAltarYelled(false)
{
    Initialize();
}

void instance_zulgurub::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_zulgurub::DoYellAtTriggerIfCan(uint32 uiTriggerId)
{
    if (uiTriggerId == AREATRIGGER_ENTER && !m_bHasIntroYelled)
    {
        if (Creature* pHakkar = instance->GetCreature(m_uiHakkarGUID))
        {
            DoScriptText(SAY_HAKKAR_PROTECT, pHakkar);
            m_bHasIntroYelled = true;
        }
    }
    else if (uiTriggerId == AREATRIGGER_ALTAR && !m_bHasAltarYelled)
    {
        if (Creature* pHakkar = instance->GetCreature(m_uiHakkarGUID))
        {
            DoScriptText(SAY_MINION_DESTROY, pHakkar);
            m_bHasAltarYelled = true;
        }
    }
}

void instance_zulgurub::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_LORKHAN:
            m_uiLorKhanGUID = pCreature->GetGUID();
            break;
        case NPC_ZATH:
            m_uiZathGUID = pCreature->GetGUID();
            break;
        case NPC_THEKAL:
            m_uiThekalGUID = pCreature->GetGUID();
            break;
        case NPC_JINDO:
            m_uiJindoGUID = pCreature->GetGUID();
            break;
        case NPC_HAKKAR:
            m_uiHakkarGUID = pCreature->GetGUID();
            break;
    }
}

void instance_zulgurub::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_JEKLIK:
        case TYPE_VENOXIS:
        case TYPE_MARLI:
        case TYPE_THEKAL:
        case TYPE_ARLOKK:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoLowerHakkarHitPoints();
            break;
        case TYPE_OHGAN:
        case TYPE_LORKHAN:
        case TYPE_ZATH:
            m_auiEncounter[uiType] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

// Each time High Priest dies lower Hakkar's HP
void instance_zulgurub::DoLowerHakkarHitPoints()
{
    if (Creature* pHakkar = instance->GetCreature(m_uiHakkarGUID))
    {
        if (pHakkar->isAlive() && pHakkar->GetMaxHealth() > HP_LOSS_PER_PRIEST)
        {
            pHakkar->SetMaxHealth(pHakkar->GetMaxHealth() - HP_LOSS_PER_PRIEST);
            pHakkar->SetHealth(pHakkar->GetHealth() - HP_LOSS_PER_PRIEST);
         }
     }
}

void instance_zulgurub::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
    >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_zulgurub::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

uint64 instance_zulgurub::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_LORKHAN:   return m_uiLorKhanGUID;
        case NPC_ZATH:      return m_uiZathGUID;
        case NPC_THEKAL:    return m_uiThekalGUID;
        case NPC_JINDO:     return m_uiJindoGUID;
        case NPC_HAKKAR:    return m_uiHakkarGUID;
        default:
            return 0;
    }
}

InstanceData* GetInstanceData_instance_zulgurub(Map* pMap)
{
    return new instance_zulgurub(pMap);
}

bool AreaTrigger_at_zulgurub(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pAt->id == AREATRIGGER_ENTER || pAt->id == AREATRIGGER_ALTAR)
    {
        if (pPlayer->isGameMaster() || pPlayer->isDead())
            return false;

        if (instance_zulgurub* pInstance = (instance_zulgurub*)pPlayer->GetInstanceData())
            pInstance->DoYellAtTriggerIfCan(pAt->id);
    }

    return false;
}

void AddSC_instance_zulgurub()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_zulgurub";
    pNewScript->GetInstanceData = &GetInstanceData_instance_zulgurub;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_zulgurub";
    pNewScript->pAreaTrigger = &AreaTrigger_at_zulgurub;
    pNewScript->RegisterSelf();
}
