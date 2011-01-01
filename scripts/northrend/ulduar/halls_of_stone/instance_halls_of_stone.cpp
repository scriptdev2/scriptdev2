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
SDName: Instance_Halls_of_Stone
SD%Complete: 10%
SDComment:
SDCategory: Halls of Stone
EndScriptData */

#include "precompiled.h"
#include "halls_of_stone.h"

struct MANGOS_DLL_DECL instance_halls_of_stone : public ScriptedInstance
{
    instance_halls_of_stone(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiBrannGUID;
    uint64 m_uiKaddrakGUID;
    uint64 m_uiAbedneumGUID;
    uint64 m_uiMarnakGUID;

    uint64 m_uiSjonnirDoorGUID;
    uint64 m_uiTribunalDoorGUID;
    uint64 m_uiTribunalChestGUID;
    uint64 m_uiTribunalHeadRightGUID;
    uint64 m_uiTribunalHeadCenterGUID;
    uint64 m_uiTribunalHeadLeftGUID;
    uint64 m_uiTribunalConsoleGUID;
    uint64 m_uiTribunalFloorGUID;
    uint64 m_uiSjonnirConsoleGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiBrannGUID               = 0;
        m_uiKaddrakGUID             = 0;
        m_uiAbedneumGUID            = 0;
        m_uiMarnakGUID              = 0;

        m_uiSjonnirDoorGUID         = 0;
        m_uiTribunalDoorGUID        = 0;
        m_uiTribunalChestGUID       = 0;
        m_uiTribunalHeadRightGUID   = 0;
        m_uiTribunalHeadCenterGUID  = 0;
        m_uiTribunalHeadLeftGUID    = 0;
        m_uiTribunalConsoleGUID     = 0;
        m_uiTribunalFloorGUID       = 0;
        m_uiSjonnirConsoleGUID      = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_BRANN:
                m_uiBrannGUID = pCreature->GetGUID();
                break;
            case NPC_KADDRAK:
                m_uiKaddrakGUID = pCreature->GetGUID();
                break;
            case NPC_ABEDNEUM:
                m_uiAbedneumGUID = pCreature->GetGUID();
                break;
            case NPC_MARNAK:
                m_uiMarnakGUID = pCreature->GetGUID();
                break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_DOOR_SJONNIR:
                m_uiSjonnirDoorGUID = pGo->GetGUID();
                break;
            case GO_DOOR_TRIBUNAL:
                m_uiTribunalDoorGUID = pGo->GetGUID();
                break;
            case GO_TRIBUNAL_CHEST:
            case GO_TRIBUNAL_CHEST_H:
                m_uiTribunalChestGUID = pGo->GetGUID();
                break;
            case GO_TRIBUNAL_HEAD_RIGHT:
                m_uiTribunalHeadRightGUID = pGo->GetGUID();
                break;
            case GO_TRIBUNAL_HEAD_CENTER:
                m_uiTribunalHeadCenterGUID = pGo->GetGUID();
                break;
            case GO_TRIBUNAL_HEAD_LEFT:
                m_uiTribunalHeadLeftGUID = pGo->GetGUID();
                break;
            case GO_TRIBUNAL_CONSOLE:
                m_uiTribunalConsoleGUID = pGo->GetGUID();
                break;
            case GO_TRIBUNAL_FLOOR:
                m_uiTribunalFloorGUID = pGo->GetGUID();
                break;
            case GO_SJONNIR_CONSOLE:
                m_uiSjonnirConsoleGUID = pGo->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_TRIBUNAL:
                m_auiEncounter[0] = uiData;
                if (uiData == DONE)
                    DoRespawnGameObject(m_uiTribunalChestGUID);
                break;
            case TYPE_MAIDEN:
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_KRYSTALLUS:
                m_auiEncounter[2] = uiData;
                break;
            case TYPE_SJONNIR:
                m_auiEncounter[3] = uiData;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_TRIBUNAL:
                return m_auiEncounter[0];
            case TYPE_MAIDEN:
                return m_auiEncounter[1];
            case TYPE_KRYSTALLUS:
                return m_auiEncounter[2];
            case TYPE_SJONNIR:
                return m_auiEncounter[3];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_BRANN:
                return m_uiBrannGUID;
            case NPC_KADDRAK:
                return m_uiKaddrakGUID;
            case NPC_ABEDNEUM:
                return m_uiAbedneumGUID;
            case NPC_MARNAK:
                return m_uiMarnakGUID;
            case GO_DOOR_SJONNIR:
                return m_uiSjonnirDoorGUID;
            case GO_DOOR_TRIBUNAL:
                return m_uiTribunalDoorGUID;
            case GO_TRIBUNAL_CHEST:
            case GO_TRIBUNAL_CHEST_H:
                return m_uiTribunalChestGUID;
            case GO_TRIBUNAL_HEAD_RIGHT:
                return m_uiTribunalHeadRightGUID;
            case GO_TRIBUNAL_HEAD_CENTER:
                return m_uiTribunalHeadCenterGUID;
            case GO_TRIBUNAL_HEAD_LEFT:
                return m_uiTribunalHeadLeftGUID;
            case GO_TRIBUNAL_CONSOLE:
                return m_uiTribunalConsoleGUID;
            case GO_TRIBUNAL_FLOOR:
                return m_uiTribunalFloorGUID;
            case GO_SJONNIR_CONSOLE:
                return m_uiSjonnirConsoleGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_halls_of_stone(Map* pMap)
{
    return new instance_halls_of_stone(pMap);
}

void AddSC_instance_halls_of_stone()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_halls_of_stone";
    newscript->GetInstanceData = &GetInstanceData_instance_halls_of_stone;
    newscript->RegisterSelf();
}
