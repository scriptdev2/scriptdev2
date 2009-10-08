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
SDName: instance_ahnkahet
SD%Complete: 0
SDComment:
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

struct MANGOS_DLL_DECL instance_ahnkahet : public ScriptedInstance
{
    instance_ahnkahet(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    uint64 m_uiElderNadoxGUID;
    uint64 m_uiJedogaShadowseekerGUID;
    uint64 m_uiTaldaramDoorGUID;
    uint64 m_uiTaldaramVortexGUID;
    uint8 m_uiDevicesActivated;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiElderNadoxGUID = 0;
        m_uiJedogaShadowseekerGUID = 0;
        m_uiTaldaramDoorGUID = 0;
        m_uiTaldaramVortexGUID = 0;
        m_uiDevicesActivated = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_ELDER_NADOX:         m_uiElderNadoxGUID = pCreature->GetGUID();         break;
            case NPC_JEDOGA_SHADOWSEEKER: m_uiJedogaShadowseekerGUID = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_DOOR_TALDARAM:
                m_uiTaldaramDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[1] == DONE)
                    DoUseDoorOrButton(m_uiTaldaramDoorGUID);
                break;
            case GO_ANCIENT_DEVICE_L:
                if (m_auiEncounter[1] == NOT_STARTED)
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                break;
            case GO_ANCIENT_DEVICE_R:
                if (m_auiEncounter[1] == NOT_STARTED)
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                break;
            case GO_VORTEX:
                m_uiTaldaramVortexGUID = pGo->GetGUID();
                if (m_auiEncounter[1] != NOT_STARTED)
                    DoUseDoorOrButton(m_uiTaldaramVortexGUID);
                break;
        }
    }
    
    void SetData(uint32 uiType, uint32 uiData)
    {
        debug_log("SD2: Instance Ahn'Kahet: SetData received for type %u with data %u",uiType,uiData);

        switch(uiType)
        {
            case TYPE_NADOX:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_TALDARAM:
                if (uiData == SPECIAL)
                {
                    if (m_uiDevicesActivated < 2)
                        ++m_uiDevicesActivated;

                    if (m_uiDevicesActivated == 2)
                    {
                        m_auiEncounter[1] = uiData;
                        DoUseDoorOrButton(m_uiTaldaramVortexGUID);
                    }
                }
                if (uiData == DONE)
                {
                    m_auiEncounter[1] = uiData;
                    DoUseDoorOrButton(m_uiTaldaramDoorGUID);
                }
                break;
            case TYPE_JEDOGA:
                m_auiEncounter[2] = uiData;
                break;
            case TYPE_VOLAZJ:
                m_auiEncounter[3] = uiData;
                break;
            case TYPE_AMANITAR:
                m_auiEncounter[4] = uiData;
                break;
            default:
                error_log("SD2: Instance Ahn'Kahet: ERROR SetData = %u for type %u does not exist/not implemented.",uiType,uiData);
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3]
                << " " << m_auiEncounter[4];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strInstData.c_str();
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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_TALDARAM:
                return m_auiEncounter[0];
            case TYPE_JEDOGA:
                return m_auiEncounter[1];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_ELDER_NADOX:
                return m_uiElderNadoxGUID;
            case NPC_JEDOGA_SHADOWSEEKER:
                return m_uiJedogaShadowseekerGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_ahnkahet(Map* pMap)
{
    return new instance_ahnkahet(pMap);
}

void AddSC_instance_ahnkahet()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "instance_ahnkahet";
    newscript->GetInstanceData = &GetInstanceData_instance_ahnkahet;
    newscript->RegisterSelf();
}
