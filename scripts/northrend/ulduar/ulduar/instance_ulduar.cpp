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
SDName: Instance_Ulduar
SD%Complete: 
SDComment: 
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

struct MANGOS_DLL_DECL instance_ulduar : public ScriptedInstance
{
    instance_ulduar(Map* pMap) : ScriptedInstance(pMap) { Initialize(); }

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string m_strInstData;

    uint64 m_uiLeviathanGUID;
    uint64 m_uiIgnisGUID;
    uint64 m_uiRazorscaleGUID;
    uint64 m_uiXT002GUID;
    uint64 m_auiAssemblyGUIDs[3];
    uint64 m_uiKologarnGUID;
    uint64 m_uiAuriayaGUID;
    uint64 m_uiMimironGUID;
    uint64 m_uiHodirGUID;
    uint64 m_uiThorimGUID;
    uint64 m_uiFreyaGUID;
    uint64 m_uiVezaxGUID;
    uint64 m_uiYoggSaronGUID;
    uint64 m_uiAlgalonGUID;

    void Initialize()
    {
        m_uiLeviathanGUID       = 0;
        m_uiIgnisGUID           = 0;
        m_uiRazorscaleGUID      = 0;
        m_uiXT002GUID           = 0;
        m_uiKologarnGUID        = 0;
        m_uiAuriayaGUID         = 0;
        m_uiMimironGUID         = 0;
        m_uiHodirGUID           = 0;
        m_uiThorimGUID          = 0;
        m_uiFreyaGUID           = 0;
        m_uiVezaxGUID           = 0;
        m_uiYoggSaronGUID       = 0;
        m_uiAlgalonGUID         = 0;

        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
        memset(&m_auiAssemblyGUIDs, 0, sizeof(m_auiAssemblyGUIDs));
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;
        }

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_LEVIATHAN:
                m_uiLeviathanGUID = pCreature->GetGUID();
                break;
            case NPC_IGNIS:
                m_uiIgnisGUID = pCreature->GetGUID();
                break;
            case NPC_RAZORSCALE:
                m_uiRazorscaleGUID = pCreature->GetGUID();
                break;
            case NPC_XT002:
                m_uiXT002GUID = pCreature->GetGUID();
                break;

            // Assembly of Iron
            case NPC_STEELBREAKER:
                m_auiAssemblyGUIDs[0] = pCreature->GetGUID();
                break;
            case NPC_MOLGEIM:
                m_auiAssemblyGUIDs[1] = pCreature->GetGUID();
                break;
            case NPC_BRUNDIR:
                m_auiAssemblyGUIDs[2] = pCreature->GetGUID();
                break;

            case NPC_KOLOGARN:
                m_uiKologarnGUID = pCreature->GetGUID();
                break;
            case NPC_AURIAYA:
                m_uiAuriayaGUID = pCreature->GetGUID();
                break;
            case NPC_MIMIRON:
                m_uiMimironGUID = pCreature->GetGUID();
                break;
            case NPC_HODIR:
                m_uiHodirGUID = pCreature->GetGUID();
                break;
            case NPC_THORIM:
                m_uiThorimGUID = pCreature->GetGUID();
                break;
            case NPC_FREYA:
                m_uiFreyaGUID = pCreature->GetGUID();
                break;
            case NPC_VEZAX:
                m_uiVezaxGUID = pCreature->GetGUID();
                break;
            case NPC_YOGGSARON:
                m_uiYoggSaronGUID = pCreature->GetGUID();
                break;
            case NPC_ALGALON:
                m_uiAlgalonGUID = pCreature->GetGUID();
                break;
        }
    }

    /*void OnObjectCreate(GameObject *pGo)
    {
    }*/

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_LEVIATHAN:
            case TYPE_IGNIS:
            case TYPE_RAZORSCALE:
            case TYPE_XT002:
            case TYPE_ASSEMBLY:
            case TYPE_KOLOGARN:
            case TYPE_AURIAYA:
            case TYPE_MIMIRON:
            case TYPE_HODIR:
            case TYPE_THORIM:
            case TYPE_FREYA:
            case TYPE_VEZAX:
            case TYPE_YOGGSARON:
            case TYPE_ALGALON:
                m_auiEncounter[uiType] = uiData;
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;

            for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                saveStream << m_auiEncounter[i] << " ";

            m_strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case TYPE_LEVIATHAN:
                return m_uiLeviathanGUID;
            case TYPE_IGNIS:
                return m_uiIgnisGUID;
            case TYPE_RAZORSCALE:
                return m_uiRazorscaleGUID;
            case TYPE_XT002:
                return m_uiXT002GUID;
            case TYPE_KOLOGARN:
                return m_uiKologarnGUID;
            case TYPE_AURIAYA:
                return m_uiAuriayaGUID;
            case TYPE_MIMIRON:
                return m_uiMimironGUID;
            case TYPE_HODIR:
                return m_uiMimironGUID;
            case TYPE_THORIM:
                return m_uiThorimGUID;
            case TYPE_FREYA:
                return m_uiFreyaGUID;
            case TYPE_VEZAX:
                return m_uiVezaxGUID;
            case TYPE_YOGGSARON:
                return m_uiYoggSaronGUID;
            case TYPE_ALGALON:
                return m_uiAlgalonGUID;

            // Assembly of Iron
            case DATA_STEELBREAKER:
                return m_auiAssemblyGUIDs[0];
            case DATA_MOLGEIM:
                return m_auiAssemblyGUIDs[1];
            case DATA_BRUNDIR:
                return m_auiAssemblyGUIDs[2];
        }

        return 0;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_LEVIATHAN:
            case TYPE_IGNIS:
            case TYPE_RAZORSCALE:
            case TYPE_XT002:
            case TYPE_ASSEMBLY:
            case TYPE_KOLOGARN:
            case TYPE_AURIAYA:
            case TYPE_MIMIRON:
            case TYPE_HODIR:
            case TYPE_THORIM:
            case TYPE_FREYA:
            case TYPE_VEZAX:
            case TYPE_YOGGSARON:
            case TYPE_ALGALON:
                return m_auiEncounter[uiType];
        }

        return 0;
    }

    const char* Save()
    {
        return m_strInstData.c_str();
    }

    void Load(const char* strIn)
    {
        if (!strIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(strIn);

        std::istringstream loadStream(strIn);

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            loadStream >> m_auiEncounter[i];

            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_ulduar(Map* pMap)
{
    return new instance_ulduar(pMap);
}

void AddSC_instance_ulduar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_ulduar";
    newscript->GetInstanceData = &GetInstanceData_instance_ulduar;
    newscript->RegisterSelf();
}
