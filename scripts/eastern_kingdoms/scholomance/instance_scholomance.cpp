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
SDName: Instance_Scholomance
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "precompiled.h"
#include "scholomance.h"

struct MANGOS_DLL_DECL instance_scholomance : public ScriptedInstance
{
    instance_scholomance(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiGateKirtonosGUID;
    uint64 m_uiGateGandlingGUID;
    uint64 m_uiGateMiliciaGUID;
    uint64 m_uiGateTheolenGUID;
    uint64 m_uiGatePolkeltGUID;
    uint64 m_uiGateRavenianGUID;
    uint64 m_uiGateBarovGUID;
    uint64 m_uiGateIlluciaGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiGateKirtonosGUID = 0;
        m_uiGateGandlingGUID = 0;
        m_uiGateMiliciaGUID = 0;
        m_uiGateTheolenGUID = 0;
        m_uiGatePolkeltGUID = 0;
        m_uiGateRavenianGUID = 0;
        m_uiGateBarovGUID = 0;
        m_uiGateIlluciaGUID = 0;
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_GATE_KIRTONOS: m_uiGateKirtonosGUID = pGo->GetGUID(); break;
            case GO_GATE_GANDLING: m_uiGateGandlingGUID = pGo->GetGUID(); break;
            case GO_GATE_MALICIA:  m_uiGateMiliciaGUID = pGo->GetGUID(); break;
            case GO_GATE_THEOLEN:  m_uiGateTheolenGUID = pGo->GetGUID(); break;
            case GO_GATE_POLKELT:  m_uiGatePolkeltGUID = pGo->GetGUID(); break;
            case GO_GATE_RAVENIAN: m_uiGateRavenianGUID = pGo->GetGUID(); break;
            case GO_GATE_BAROV:    m_uiGateBarovGUID = pGo->GetGUID(); break;
            case GO_GATE_ILLUCIA:  m_uiGateIlluciaGUID = pGo->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_GANDLING:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_KIRTONOS:
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_ALEXEIBAROV:
                m_auiEncounter[2] = uiData;
                break;
            case TYPE_THEOLEN:
                m_auiEncounter[3] = uiData;
                break;
            case TYPE_RAVENIAN:
                m_auiEncounter[4] = uiData;
                break;
            case TYPE_POLKELT:
                m_auiEncounter[5] = uiData;
                break;
            case TYPE_MALICIA:
                m_auiEncounter[6] = uiData;
                break;
            case TYPE_ILLUCIABAROV:
                m_auiEncounter[7] = uiData;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        if (uiType == TYPE_GANDLING)
        {
            if (m_auiEncounter[2] == DONE && m_auiEncounter[3] == DONE && m_auiEncounter[4] == DONE &&
                m_auiEncounter[5] == DONE && m_auiEncounter[6] == DONE && m_auiEncounter[7] == DONE)
            {
                m_auiEncounter[0] = SPECIAL;
                return SPECIAL;
            }
        }

        return 0;
    }
};

InstanceData* GetInstanceData_instance_scholomance(Map* pMap)
{
    return new instance_scholomance(pMap);
}

void AddSC_instance_scholomance()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_scholomance";
    newscript->GetInstanceData = &GetInstanceData_instance_scholomance;
    newscript->RegisterSelf();
}
