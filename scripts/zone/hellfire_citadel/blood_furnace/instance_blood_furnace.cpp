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
SDName: Instance_Blood_Furnace
SD%Complete: 75
SDComment:
SDCategory: Blood Furnace
EndScriptData */

#include "precompiled.h"
#include "def_blood_furnace.h"

#define ENCOUNTERS 3

struct MANGOS_DLL_DECL instance_blood_furnace : public ScriptedInstance
{
    instance_blood_furnace(Map* pMap) : ScriptedInstance(pMap) {Initialize();}

    uint32 m_auiEncounter[ENCOUNTERS];
    std::string str_data;

    uint64 m_uiMakerGUID;
    uint64 m_uiBroggokGUID;
    uint64 m_uiKelidanGUID;

    uint64 m_uiDoorFinalExitGUID;
    uint64 m_uiDoorMakerFrontGUID;
    uint64 m_uiDoorMakerRearGUID;
    uint64 m_uiDoorBroggokFrontGUID;
    uint64 m_uiDoorBrokkokRearGUID;
    uint64 m_uiDoorKelidanExitGUID;

    uint64 m_uiPrisonCell1GUID;
    uint64 m_uiPrisonCell2GUID;
    uint64 m_uiPrisonCell3GUID;
    uint64 m_uiPrisonCell4GUID;
    uint64 m_uiPrisonCell5GUID;
    uint64 m_uiPrisonCell6GUID;
    uint64 m_uiPrisonCell7GUID;
    uint64 m_uiPrisonCell8GUID;

    void Initialize()
    {
        m_uiMakerGUID = 0;
        m_uiBroggokGUID = 0;
        m_uiKelidanGUID = 0;

        m_uiDoorFinalExitGUID = 0;
        m_uiDoorMakerFrontGUID = 0;
        m_uiDoorMakerRearGUID = 0;
        m_uiDoorBroggokFrontGUID = 0;
        m_uiDoorBrokkokRearGUID = 0;
        m_uiDoorKelidanExitGUID = 0;

        m_uiPrisonCell1GUID = 0;
        m_uiPrisonCell2GUID = 0;
        m_uiPrisonCell3GUID = 0;
        m_uiPrisonCell4GUID = 0;
        m_uiPrisonCell5GUID = 0;
        m_uiPrisonCell6GUID = 0;
        m_uiPrisonCell7GUID = 0;
        m_uiPrisonCell8GUID = 0;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            m_auiEncounter[i] = NOT_STARTED;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 17381: m_uiMakerGUID = pCreature->GetGUID(); break;
            case 17380: m_uiBroggokGUID = pCreature->GetGUID(); break;
            case 17377: m_uiKelidanGUID = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch (pGo->GetEntry())
        {
            case GO_DOOR_MAKER_FRONT:                       //the maker front door
                m_uiDoorMakerFrontGUID = pGo->GetGUID();
                break;
            case GO_DOOR_MAKER_REAR:                        //the maker rear door
                m_uiDoorMakerRearGUID = pGo->GetGUID();
                if (m_auiEncounter[0] == DONE && pGo->GetGoState() == GO_STATE_READY)
                    DoUseDoorOrButton(m_uiDoorMakerRearGUID);
                break;
            case GO_DOOR_BROGGOK_FRONT:                     //broggok front door
                m_uiDoorBroggokFrontGUID = pGo->GetGUID();
                break;
            case GO_DOOR_BROGGOK_REAR:                      //broggok rear door
                m_uiDoorBrokkokRearGUID = pGo->GetGUID();
                if (m_auiEncounter[1] == DONE && pGo->GetGoState() == GO_STATE_READY)
                    DoUseDoorOrButton(m_uiDoorBrokkokRearGUID);
                break;
            case GO_DOOR_KELIDAN_EXIT:                      //kelidan exit door
                m_uiDoorKelidanExitGUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE && pGo->GetGoState() == GO_STATE_READY)
                    DoUseDoorOrButton(m_uiDoorKelidanExitGUID);
                break;
            case GO_DOOR_FINAL_EXIT:                        //final exit door
                m_uiDoorFinalExitGUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE && pGo->GetGoState() == GO_STATE_READY)
                    DoUseDoorOrButton(m_uiDoorFinalExitGUID);
                break;
            case 181813: m_uiPrisonCell1GUID = pGo->GetGUID(); break;//the maker cell front right
            case 181814: m_uiPrisonCell2GUID = pGo->GetGUID(); break;//the maker cell back right
            case 181816: m_uiPrisonCell3GUID = pGo->GetGUID(); break;//the maker cell front left
            case 181815: m_uiPrisonCell4GUID = pGo->GetGUID(); break;//the maker cell back left
            case 181821: m_uiPrisonCell5GUID = pGo->GetGUID(); break;//broggok cell front right
            case 181818: m_uiPrisonCell6GUID = pGo->GetGUID(); break;//broggok cell back right
            case 181820: m_uiPrisonCell7GUID = pGo->GetGUID(); break;//broggok cell front left
            case 181817: m_uiPrisonCell8GUID = pGo->GetGUID(); break;//broggok cell back left
        }
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_THE_MAKER:            return m_uiMakerGUID;
            case DATA_BROGGOK:              return m_uiBroggokGUID;
            case DATA_PRISON_CELL_MAKER1:   return m_uiPrisonCell1GUID;
            case DATA_PRISON_CELL_MAKER2:   return m_uiPrisonCell2GUID;
            case DATA_PRISON_CELL_MAKER3:   return m_uiPrisonCell3GUID;
            case DATA_PRISON_CELL_MAKER4:   return m_uiPrisonCell4GUID;
            case DATA_PRISON_CELL_BROGGOK1: return m_uiPrisonCell5GUID;
            case DATA_PRISON_CELL_BROGGOK2: return m_uiPrisonCell6GUID;
            case DATA_PRISON_CELL_BROGGOK3: return m_uiPrisonCell7GUID;
            case DATA_PRISON_CELL_BROGGOK4: return m_uiPrisonCell8GUID;
        }

        return 0;
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_THE_MAKER_EVENT:
                if (uiData == IN_PROGRESS)
                    DoUseDoorOrButton(m_uiDoorMakerFrontGUID);
                if (uiData == FAIL)
                    DoUseDoorOrButton(m_uiDoorMakerFrontGUID);
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiDoorMakerFrontGUID);
                    DoUseDoorOrButton(m_uiDoorMakerRearGUID);
                }
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_BROGGOK_EVENT:
                if (uiData == IN_PROGRESS)
                    DoUseDoorOrButton(m_uiDoorBroggokFrontGUID);
                if (uiData == FAIL)
                    DoUseDoorOrButton(m_uiDoorBroggokFrontGUID);
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiDoorBroggokFrontGUID);
                    DoUseDoorOrButton(m_uiDoorBrokkokRearGUID);
                }
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_KELIDAN_EVENT:
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiDoorKelidanExitGUID);
                    DoUseDoorOrButton(m_uiDoorFinalExitGUID);
                }
                m_auiEncounter[2] = uiData;
                break;
            default:
                error_log("SD2: Instance Blood Furnace SetData with Type %u Data %u, but this is not implemented.",uiType,uiData);
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2];

            str_data = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 uiData)
    {
        switch(uiData)
        {
            case TYPE_THE_MAKER_EVENT:  return m_auiEncounter[0];
            case TYPE_BROGGOK_EVENT:    return m_auiEncounter[1];
            case TYPE_KELIDAN_EVENT:    return m_auiEncounter[2];
        }

        return 0;
    }

    const char* Save()
    {
        return str_data.c_str();
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);

        std::istringstream loadStream(in);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2];

        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS || m_auiEncounter[i] == FAIL)
                m_auiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_blood_furnace(Map* pMap)
{
    return new instance_blood_furnace(pMap);
}

void AddSC_instance_blood_furnace()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_blood_furnace";
    newscript->GetInstanceData = &GetInstanceData_instance_blood_furnace;
    newscript->RegisterSelf();
}
