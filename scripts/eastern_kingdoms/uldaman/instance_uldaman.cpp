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
SDName: Instance_Uldaman
SD%Complete:
SDComment:
SDCategory: Uldaman
EndScriptData
*/

#include "precompiled.h"
#include "uldaman.h"

struct MANGOS_DLL_DECL instance_uldaman : public ScriptedInstance
{
    instance_uldaman(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_uiEncounter[MAX_ENCOUNTER];
    uint64 m_uiTempleDoor1GUID;
    uint64 m_uiTempleDoor2GUID;
    uint64 m_uiAncientVaultGUID;
    uint64 m_uiPlayerGUID;

    uint8  m_uiStoneKeepersFallen;

    void Initialize()
    {
        memset(&m_uiEncounter, 0, sizeof(m_uiEncounter));
        
        m_uiStoneKeepersFallen = 0;
        m_uiTempleDoor1GUID    = 0;
        m_uiTempleDoor2GUID    = 0;
        m_uiAncientVaultGUID   = 0;
        m_uiPlayerGUID         = 0;
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_TEMPLE_DOOR1:
                m_uiTempleDoor1GUID = pGo->GetGUID();
                break;
            case GO_TEMPLE_DOOR2:
                m_uiTempleDoor2GUID = pGo->GetGUID();
                break;
            case GO_ANCIENT_VAULT:
                m_uiAncientVaultGUID = pGo->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_ALTAR_EVENT:
                if (uiData == SPECIAL)
                    ++m_uiStoneKeepersFallen;
                if (m_uiStoneKeepersFallen > 3)
                {
                    uiData = DONE;
                    DoUseDoorOrButton(m_uiTempleDoor1GUID);
                    DoUseDoorOrButton(m_uiTempleDoor2GUID);
                }
                m_uiEncounter[0] = uiData;
                break;
            case TYPE_ARCHAEDAS_EVENT:
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiAncientVaultGUID);
                m_uiEncounter[1] = uiData;
                break;
        }
    }

    void SetData64(uint32 uiData, uint64 uiGuid)
    {
       switch(uiData)
       {
            case DATA_EVENT_STARTER:
                m_uiPlayerGUID = uiGuid;
                break;
       }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_ARCHAEDAS_EVENT:
                return m_uiEncounter[1];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_EVENT_STARTER:
                return m_uiPlayerGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_uldaman(Map* pMap)
{
    return new instance_uldaman(pMap);
}

void AddSC_instance_uldaman()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "instance_uldaman";
    newscript->GetInstanceData = &GetInstanceData_instance_uldaman;
    newscript->RegisterSelf();
}
