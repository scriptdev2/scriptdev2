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
SDName: Instance_Shattered_Halls
SD%Complete: 50
SDComment: currently missing info about door. instance not complete
SDCategory: Hellfire Citadel, Shattered Halls
EndScriptData */

#include "precompiled.h"
#include "shattered_halls.h"

struct MANGOS_DLL_DECL instance_shattered_halls : public ScriptedInstance
{
    instance_shattered_halls(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint64 m_uiNethekurseGUID;
    uint64 m_uiNethekurseDoorGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiNethekurseGUID = 0;
        m_uiNethekurseDoorGUID = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; i++)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;
        return false;
    }

    void OnObjectCreate(GameObject* pGo)
    {
        if (pGo->GetEntry() == GO_NETHEKURSE_DOOR)
            m_uiNethekurseDoorGUID = pGo->GetGUID();
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        if (pCreature->GetEntry() == NPC_NETHEKURSE)
            m_uiNethekurseGUID = pCreature->GetGUID();
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_NETHEKURSE:
                m_auiEncounter[0] = uiData;
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiNethekurseDoorGUID);
                break;
            case TYPE_OMROGG:
                m_auiEncounter[1] = uiData;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_NETHEKURSE:
                return m_auiEncounter[0];
            case TYPE_OMROGG:
                return m_auiEncounter[1];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_NETHEKURSE:
                return m_uiNethekurseGUID;
            case GO_NETHEKURSE_DOOR:
                return m_uiNethekurseDoorGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_shattered_halls(Map* pMap)
{
    return new instance_shattered_halls(pMap);
}

void AddSC_instance_shattered_halls()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_shattered_halls";
    pNewScript->GetInstanceData = &GetInstanceData_instance_shattered_halls;
    pNewScript->RegisterSelf();
}
