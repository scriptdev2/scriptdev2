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
SDName: Instance_Utgarde_Keep
SD%Complete: 20%
SDComment:
SDCategory: Utgarde Keep
EndScriptData */

#include "precompiled.h"
#include "utgarde_keep.h"

struct MANGOS_DLL_DECL instance_utgarde_keep : public ScriptedInstance
{
    instance_utgarde_keep(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    uint64 m_uiKelesethGUID;
    uint64 m_uiSkarvaldGUID;
    uint64 m_uiDalronnGUID;

    uint64 m_uiBellow1GUID;
    uint64 m_uiBellow2GUID;
    uint64 m_uiBellow3GUID;
    uint64 m_uiForgeFire1GUID;
    uint64 m_uiForgeFire2GUID;
    uint64 m_uiForgeFire3GUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiKelesethGUID = 0;
        m_uiSkarvaldGUID = 0;
        m_uiDalronnGUID = 0;

        m_uiBellow1GUID = 0;
        m_uiBellow2GUID = 0;
        m_uiBellow3GUID = 0;
        m_uiForgeFire1GUID = 0;
        m_uiForgeFire2GUID = 0;
        m_uiForgeFire3GUID = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_KELESETH: m_uiKelesethGUID = pCreature->GetGUID(); break;
            case NPC_SKARVALD: m_uiSkarvaldGUID = pCreature->GetGUID(); break;
            case NPC_DALRONN: m_uiDalronnGUID = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_BELLOW_1:
                m_uiBellow1GUID = pGo->GetGUID();
                if (m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_BELLOW_2:
                m_uiBellow2GUID = pGo->GetGUID();
                if (m_auiEncounter[1] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_BELLOW_3:
                m_uiBellow3GUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_FORGEFIRE_1:
                m_uiForgeFire1GUID = pGo->GetGUID();
                if (m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_FORGEFIRE_2:
                m_uiForgeFire2GUID = pGo->GetGUID();
                if (m_auiEncounter[1] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_FORGEFIRE_3:
                m_uiForgeFire3GUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case GO_BELLOW_1:
                m_auiEncounter[0] = uiData;
                break;
            case GO_BELLOW_2:
                m_auiEncounter[1] = uiData;
                break;
            case GO_BELLOW_3:
                m_auiEncounter[2] = uiData;
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

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_KELESETH:
                return m_uiKelesethGUID;
            case NPC_SKARVALD:
                return m_uiSkarvaldGUID;
            case NPC_DALRONN:
                return m_uiDalronnGUID;
            case GO_BELLOW_1:
                return m_uiBellow1GUID;
            case GO_BELLOW_2:
                return m_uiBellow2GUID;
            case GO_BELLOW_3:
                return m_uiBellow3GUID;
            case GO_FORGEFIRE_1:
                return m_uiForgeFire1GUID;
            case GO_FORGEFIRE_2:
                return m_uiForgeFire2GUID;
            case GO_FORGEFIRE_3:
                return m_uiForgeFire3GUID;
        }
        return 0;
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

InstanceData* GetInstanceData_instance_utgarde_keep(Map* pMap)
{
    return new instance_utgarde_keep(pMap);
}

void AddSC_instance_utgarde_keep()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_utgarde_keep";
    newscript->GetInstanceData = GetInstanceData_instance_utgarde_keep;
    newscript->RegisterSelf();
}
