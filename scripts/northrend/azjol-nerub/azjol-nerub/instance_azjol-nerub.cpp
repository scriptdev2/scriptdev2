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
SDName: Instance_Azjol-Nerub
SD%Complete: 50
SDComment:
SDCategory: Azjol-Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

struct MANGOS_DLL_DECL instance_azjol_nerub : public ScriptedInstance
{
    instance_azjol_nerub(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    uint64 m_uiDoor_KrikthirGUID;
    uint64 m_uiDoor_Anubarak_1GUID;
    uint64 m_uiDoor_Anubarak_2GUID;
    uint64 m_uiDoor_Anubarak_3GUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiDoor_KrikthirGUID = 0;
        m_uiDoor_Anubarak_1GUID = 0;
        m_uiDoor_Anubarak_2GUID = 0;
        m_uiDoor_Anubarak_3GUID = 0;
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_DOOR_KRIKTHIR:
                m_uiDoor_KrikthirGUID = pGo->GetGUID();
                if (m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_ANUBARAK_1:
                m_uiDoor_Anubarak_1GUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE || m_auiEncounter[2] == NOT_STARTED)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_ANUBARAK_2:
                m_uiDoor_Anubarak_2GUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE || m_auiEncounter[2] == NOT_STARTED)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_ANUBARAK_3:
                m_uiDoor_Anubarak_3GUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE || m_auiEncounter[2] == NOT_STARTED)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_KRIKTHIR:
                m_auiEncounter[0] = uiData;
                if (uiData == DONE)
                    if (GameObject* pGo = instance->GetGameObject(m_uiDoor_KrikthirGUID))
                        pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case TYPE_HADRONOX:
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_ANUBARAK:
                m_auiEncounter[2] = uiData;
                if (uiData == DONE || uiData == NOT_STARTED)
                {
                    if (GameObject* pGo = instance->GetGameObject(m_uiDoor_Anubarak_1GUID))
                        pGo->SetGoState(GO_STATE_ACTIVE);
                    if (GameObject* pGo = instance->GetGameObject(m_uiDoor_Anubarak_2GUID))
                        pGo->SetGoState(GO_STATE_ACTIVE);
                    if (GameObject* pGo = instance->GetGameObject(m_uiDoor_Anubarak_3GUID))
                        pGo->SetGoState(GO_STATE_ACTIVE);
                }
                if (uiData == IN_PROGRESS)
                {
                    if (GameObject* pGo = instance->GetGameObject(m_uiDoor_Anubarak_1GUID))
                        pGo->SetGoState(GO_STATE_READY);
                    if (GameObject* pGo = instance->GetGameObject(m_uiDoor_Anubarak_2GUID))
                        pGo->SetGoState(GO_STATE_READY);
                    if (GameObject* pGo = instance->GetGameObject(m_uiDoor_Anubarak_3GUID))
                        pGo->SetGoState(GO_STATE_READY);
                }
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

    const char* Save()
    {
        return strInstData.c_str();
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

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_azjol_nerub(Map* pMap)
{
    return new instance_azjol_nerub(pMap);
}

void AddSC_instance_azjol_nerub()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_azjol-nerub";
    newscript->GetInstanceData = &GetInstanceData_instance_azjol_nerub;
    newscript->RegisterSelf();
}
