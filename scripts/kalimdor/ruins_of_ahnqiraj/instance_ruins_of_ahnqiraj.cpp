/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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


struct MANGOS_DLL_DECL instance_ruins_of_ahnqiraj : public ScriptedInstance
{
    instance_ruins_of_ahnqiraj(Map* pMap) : ScriptedInstance(pMap) {Initialize();}

    std::string strInstData;
    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiOssirianGUID;
    uint64 m_uiBuruGUID;
    uint64 m_uiKurinnaxxGUID;
    uint64 m_uiAndorovGUID;
    uint64 m_uiRajaxxGUID;
    uint64 m_uiQeezGUID;
    uint64 m_uiTuubidGUID;
    uint64 m_uiDrennGUID;
    uint64 m_uiXurremGUID;
    uint64 m_uiYeggethGUID;
    uint64 m_uiPakkonGUID;
    uint64 m_uiZerranGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiOssirianGUID  = 0;
        m_uiBuruGUID      = 0;
        m_uiKurinnaxxGUID = 0;
        m_uiAndorovGUID   = 0;
        m_uiQeezGUID      = 0;
        m_uiTuubidGUID    = 0;
        m_uiDrennGUID     = 0;
        m_uiXurremGUID    = 0;
        m_uiYeggethGUID   = 0;
        m_uiPakkonGUID    = 0;
        m_uiZerranGUID    = 0;
        m_uiRajaxxGUID    = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
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

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_OSSIRIAN_CRYSTAL: pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND); break; //to make them unusable temporarily
        }
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    const char* Save()
    {
        return strInstData.c_str();
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_OSSIRIAN:  return m_uiOssirianGUID;
            case DATA_BURU:      return m_uiBuruGUID;
            case DATA_ANDOROV:   return m_uiAndorovGUID;
            case DATA_KURINNAXX: return m_uiKurinnaxxGUID;
            case DATA_QEEZ:      return m_uiQeezGUID;
            case DATA_TUUBID:    return m_uiTuubidGUID;
            case DATA_DRENN:     return m_uiDrennGUID;
            case DATA_XURREM:    return m_uiXurremGUID;
            case DATA_YEGGETH:   return m_uiYeggethGUID;
            case DATA_PAKKON:    return m_uiPakkonGUID;
            case DATA_ZERRAN:    return m_uiZerranGUID;
            case DATA_RAJAXX:    return m_uiRajaxxGUID;
        }
        return 0;
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_RAJAXX:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_KURINNAXX:
                m_auiEncounter[1] = uiData;
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " ";

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    void Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);

        std::istringstream loadStream(chrIn);

        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;

        //spawn andorov on load
        if (m_auiEncounter[0] == DONE)
            if (Creature* pRajaxx = instance->GetCreature(m_uiRajaxxGUID))
                pRajaxx->SummonCreature(NPC_GENERAL_ANDOROV, -8873.42f, 1647.67f, 21.386f, 5.69141f, TEMPSUMMON_CORPSE_DESPAWN, 0);
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_RAJAXX:
                return m_auiEncounter[0];
            case TYPE_KURINNAXX:
                return m_auiEncounter[1];
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_ruins_of_ahnqiraj(Map* pMap)
{
    return new instance_ruins_of_ahnqiraj(pMap);
}

void AddSC_instance_ruins_of_ahnqiraj()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "instance_ruins_of_ahnqiraj";
    newscript->GetInstanceData = &GetInstanceData_instance_ruins_of_ahnqiraj;
    newscript->RegisterSelf();
}
