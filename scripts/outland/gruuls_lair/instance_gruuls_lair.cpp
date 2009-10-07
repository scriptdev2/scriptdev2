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
SDName: Instance_Gruuls_Lair
SD%Complete: 100
SDComment:
SDCategory: Gruul's Lair
EndScriptData */

#include "precompiled.h"
#include "gruuls_lair.h"

/* Gruuls Lair encounters:
1 - High King Maulgar event
2 - Gruul event
*/

struct MANGOS_DLL_DECL instance_gruuls_lair : public ScriptedInstance
{
    instance_gruuls_lair(Map *pMap) : ScriptedInstance(pMap) {Initialize();}

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strSaveData;

    uint64 m_uiMaulgarGUID;
    uint64 m_uiKigglerGUID;
    uint64 m_uiBlindeyeGUID;
    uint64 m_uiOlmGUID;
    uint64 m_uiKroshGUID;
    uint64 m_uiMaulgarDoorGUID;
    uint64 m_uiGruulEncounterDoorGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiMaulgarGUID             = 0;
        m_uiKigglerGUID             = 0;
        m_uiBlindeyeGUID            = 0;
        m_uiOlmGUID                 = 0;
        m_uiKroshGUID               = 0;

        m_uiMaulgarDoorGUID         = 0;
        m_uiGruulEncounterDoorGUID  = 0;
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch (pCreature->GetEntry())
        {
            case 18831: m_uiMaulgarGUID     = pCreature->GetGUID(); break;
            case 18832: m_uiKroshGUID       = pCreature->GetGUID(); break;
            case 18834: m_uiOlmGUID         = pCreature->GetGUID(); break;
            case 18835: m_uiKigglerGUID     = pCreature->GetGUID(); break;
            case 18836: m_uiBlindeyeGUID    = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch (pGo->GetEntry())
        {
            case 184468:
                m_uiMaulgarDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case 184662:
                m_uiGruulEncounterDoorGUID = pGo->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch (uiType)
        {
            case TYPE_MAULGAR_EVENT:
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiMaulgarDoorGUID);
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_GRUUL_EVENT:
                DoUseDoorOrButton(m_uiGruulEncounterDoorGUID);
                m_auiEncounter[1] = uiData;
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1];

            strSaveData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strSaveData.c_str();
    }

    uint32 GetData(uint32 uiType)
    {
        switch (uiType)
        {
            case TYPE_MAULGAR_EVENT:    return m_auiEncounter[0];
            case TYPE_GRUUL_EVENT:      return m_auiEncounter[1];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch (uiData)
        {
            case DATA_MAULGAR:              return m_uiMaulgarGUID;
            case DATA_BLINDEYE:             return m_uiBlindeyeGUID;
            case DATA_KIGGLER:              return m_uiKigglerGUID;
            case DATA_KROSH:                return m_uiKroshGUID;
            case DATA_OLM:                  return m_uiOlmGUID;
        }
        return 0;
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
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_gruuls_lair(Map* pMap)
{
    return new instance_gruuls_lair(pMap);
}

void AddSC_instance_gruuls_lair()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_gruuls_lair";
    newscript->GetInstanceData = &GetInstanceData_instance_gruuls_lair;
    newscript->RegisterSelf();
}
