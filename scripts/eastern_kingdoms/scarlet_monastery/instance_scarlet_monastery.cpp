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
SDName: Instance_Scarlet_Monastery
SD%Complete: 50
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "precompiled.h"
#include "scarlet_monastery.h"

struct MANGOS_DLL_DECL instance_scarlet_monastery : public ScriptedInstance
{
    instance_scarlet_monastery(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiMograineGUID;
    uint64 m_uiWhitemaneGUID;
    uint64 m_uiVorrelGUID;
    uint64 m_uiDoorHighInquisitorGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiMograineGUID = 0;
        m_uiWhitemaneGUID = 0;
        m_uiVorrelGUID = 0;
        m_uiDoorHighInquisitorGUID = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_MOGRAINE:  m_uiMograineGUID = pCreature->GetGUID();  break;
            case NPC_WHITEMANE: m_uiWhitemaneGUID = pCreature->GetGUID(); break;
            case NPC_VORREL:    m_uiVorrelGUID = pCreature->GetGUID();    break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        if (pGo->GetEntry() == GO_WHITEMANE_DOOR)
            m_uiDoorHighInquisitorGUID = pGo->GetGUID();
    }

    uint64 GetData64(uint32 data)
    {
        switch(data)
        {
            case NPC_MOGRAINE:      return m_uiMograineGUID;
            case NPC_WHITEMANE:     return m_uiWhitemaneGUID;
            case NPC_VORREL:        return m_uiVorrelGUID;
            case GO_WHITEMANE_DOOR: return m_uiDoorHighInquisitorGUID;

            default:
                return 0;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        if (uiType == TYPE_MOGRAINE_AND_WHITE_EVENT)
        {
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(m_uiDoorHighInquisitorGUID);
            if (uiData == FAIL)
                DoUseDoorOrButton(m_uiDoorHighInquisitorGUID);

            m_auiEncounter[0] = uiData;
        }
    }

    uint32 GetData(uint32 uiData)
    {
        if (uiData == TYPE_MOGRAINE_AND_WHITE_EVENT)
            return m_auiEncounter[0];

        return 0;
    }
};

InstanceData* GetInstanceData_instance_scarlet_monastery(Map* pMap)
{
    return new instance_scarlet_monastery(pMap);
}

void AddSC_instance_scarlet_monastery()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_scarlet_monastery";
    newscript->GetInstanceData = &GetInstanceData_instance_scarlet_monastery;
    newscript->RegisterSelf();
}
