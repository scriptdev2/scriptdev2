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
SDName: Instance_Ruins_of_Ahnqiraj
SD%Complete: 80
SDComment:
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "ruins_of_ahnqiraj.h"

instance_ruins_of_ahnqiraj::instance_ruins_of_ahnqiraj(Map* pMap) : ScriptedInstance(pMap),
    m_uiOssirianGUID(0),
    m_uiBuruGUID(0),
    m_uiKurinnaxxGUID(0),
    m_uiAndorovGUID(0),
    m_uiQeezGUID(0),
    m_uiTuubidGUID(0),
    m_uiDrennGUID(0),
    m_uiXurremGUID(0),
    m_uiYeggethGUID(0),
    m_uiPakkonGUID(0),
    m_uiZerranGUID(0),
    m_uiRajaxxGUID(0)
{
    Initialize();
}

void instance_ruins_of_ahnqiraj::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_ruins_of_ahnqiraj::OnPlayerEnter(Player* pPlayer)
{
    // Spawn andorov if necessary
    DoSapwnAndorovIfCan();
}

void instance_ruins_of_ahnqiraj::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_OSSIRIAN:        m_uiOssirianGUID  = pCreature->GetGUID(); break;
        case NPC_BURU:            m_uiBuruGUID      = pCreature->GetGUID(); break;
        case NPC_KURINNAXX:       m_uiKurinnaxxGUID = pCreature->GetGUID(); break;
        case NPC_GENERAL_ANDOROV: m_uiAndorovGUID   = pCreature->GetGUID(); break;
        case NPC_CAPTAIN_QEEZ:    m_uiQeezGUID      = pCreature->GetGUID(); break;
        case NPC_CAPTAIN_TUUBID:  m_uiTuubidGUID    = pCreature->GetGUID(); break;
        case NPC_CAPTAIN_DRENN:   m_uiDrennGUID     = pCreature->GetGUID(); break;
        case NPC_CAPTAIN_XURREM:  m_uiXurremGUID    = pCreature->GetGUID(); break;
        case NPC_MAJOR_YEGGETH:   m_uiYeggethGUID   = pCreature->GetGUID(); break;
        case NPC_MAJOR_PAKKON:    m_uiPakkonGUID    = pCreature->GetGUID(); break;
        case NPC_COLONEL_ZERRAN:  m_uiZerranGUID    = pCreature->GetGUID(); break;
        case NPC_RAJAXX:          m_uiRajaxxGUID    = pCreature->GetGUID(); break;
    }
}

void instance_ruins_of_ahnqiraj::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        // Make them unusable temporarily
        case GO_OSSIRIAN_CRYSTAL: pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND); break;
    }
}

void instance_ruins_of_ahnqiraj::OnCreatureEnterCombat(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_KURINNAXX: SetData(TYPE_KURINNAXX, IN_PROGRESS); break;
        case NPC_RAJAXX:    SetData(TYPE_RAJAXX, IN_PROGRESS);    break;
        case NPC_MOAM:      SetData(TYPE_MOAM, IN_PROGRESS);      break;
        case NPC_BURU:      SetData(TYPE_BURU, IN_PROGRESS);      break;
        case NPC_AYAMISS:   SetData(TYPE_AYAMISS, IN_PROGRESS);   break;
        case NPC_OSSIRIAN:  SetData(TYPE_OSSIRIAN, IN_PROGRESS);  break;
    }
}

void instance_ruins_of_ahnqiraj::OnCreatureEvade(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_KURINNAXX: SetData(TYPE_KURINNAXX, FAIL); break;
        case NPC_RAJAXX:    SetData(TYPE_RAJAXX, FAIL);    break;
        case NPC_MOAM:      SetData(TYPE_MOAM, FAIL);      break;
        case NPC_BURU:      SetData(TYPE_BURU, FAIL);      break;
        case NPC_AYAMISS:   SetData(TYPE_AYAMISS, FAIL);   break;
        case NPC_OSSIRIAN:  SetData(TYPE_OSSIRIAN, FAIL);  break;
    }
}

void instance_ruins_of_ahnqiraj::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_KURINNAXX: SetData(TYPE_KURINNAXX, DONE); break;
        case NPC_RAJAXX:    SetData(TYPE_RAJAXX, DONE);    break;
        case NPC_MOAM:      SetData(TYPE_MOAM, DONE);      break;
        case NPC_BURU:      SetData(TYPE_BURU, DONE);      break;
        case NPC_AYAMISS:   SetData(TYPE_AYAMISS, DONE);   break;
        case NPC_OSSIRIAN:  SetData(TYPE_OSSIRIAN, DONE);  break;
    }
}

void instance_ruins_of_ahnqiraj::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_KURINNAXX:
            if (uiData == DONE)
            {
                DoSapwnAndorovIfCan();

                // Yell after kurinnaxx
                if (Creature* pOssirian = instance->GetCreature(m_uiOssirianGUID))
                    DoScriptText(SAY_OSSIRIAN_INTRO, pOssirian);
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_RAJAXX:
        case TYPE_MOAM:
        case TYPE_BURU:
        case TYPE_AYAMISS:
        case TYPE_OSSIRIAN:
            m_auiEncounter[uiType] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2]
            << " " << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_ruins_of_ahnqiraj::GetData(uint32 uiType)
{
   if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_ruins_of_ahnqiraj::DoSapwnAndorovIfCan()
{
    // The npc is also a vendor so always spawn after kurinnaxx
    if (m_auiEncounter[TYPE_KURINNAXX] != DONE)
        return;

    if (m_uiAndorovGUID)
        return;

    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    for (uint8 i = 0; i < MAX_HELPERS; ++i)
        pPlayer->SummonCreature(aAndorovSpawnLocs[i].m_uiEntry, aAndorovSpawnLocs[i].m_fX, aAndorovSpawnLocs[i].m_fY, aAndorovSpawnLocs[i].m_fZ, aAndorovSpawnLocs[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
}

uint64 instance_ruins_of_ahnqiraj::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_OSSIRIAN:          return m_uiOssirianGUID;
        case NPC_BURU:              return m_uiBuruGUID;
        case NPC_GENERAL_ANDOROV:   return m_uiAndorovGUID;
        case NPC_KURINNAXX:         return m_uiKurinnaxxGUID;
        case NPC_CAPTAIN_QEEZ:      return m_uiQeezGUID;
        case NPC_CAPTAIN_TUUBID:    return m_uiTuubidGUID;
        case NPC_CAPTAIN_DRENN:     return m_uiDrennGUID;
        case NPC_CAPTAIN_XURREM:    return m_uiXurremGUID;
        case NPC_MAJOR_YEGGETH:     return m_uiYeggethGUID;
        case NPC_MAJOR_PAKKON:      return m_uiPakkonGUID;
        case NPC_COLONEL_ZERRAN:    return m_uiZerranGUID;
        case NPC_RAJAXX:            return m_uiRajaxxGUID;

        default:
            return 0;
    }
}

void instance_ruins_of_ahnqiraj::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);

    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2]
               >> m_auiEncounter[3] >> m_auiEncounter[4] >> m_auiEncounter[5];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_ruins_of_ahnqiraj(Map* pMap)
{
    return new instance_ruins_of_ahnqiraj(pMap);
}

void AddSC_instance_ruins_of_ahnqiraj()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_ruins_of_ahnqiraj";
    pNewScript->GetInstanceData = &GetInstanceData_instance_ruins_of_ahnqiraj;
    pNewScript->RegisterSelf();
}
