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
SDName: Instance_Arcatraz
SD%Complete: 80
SDComment: Mainly Harbringer Skyriss event
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

#include "precompiled.h"
#include "arcatraz.h"

/* Arcatraz encounters:
1 - Zereketh the Unbound event
2 - Dalliah the Doomsayer event
3 - Wrath-Scryer Soccothrates event
4 - Harbinger Skyriss event, 5 sub-events
*/

struct MANGOS_DLL_DECL instance_arcatraz : public ScriptedInstance
{
    instance_arcatraz(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiCore_Security_Field_AlphaGUID;
    uint64 m_uiCore_Security_Field_BetaGUID;
    uint64 m_uiPod_AlphaGUID;
    uint64 m_uiPod_GammaGUID;
    uint64 m_uiPod_BetaGUID;
    uint64 m_uiPod_DeltaGUID;
    uint64 m_uiPod_OmegaGUID;

    uint64 m_uiGoSphereGUID;
    uint64 m_uiMellicharGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiCore_Security_Field_AlphaGUID = 0;
        m_uiCore_Security_Field_BetaGUID = 0;
        m_uiPod_AlphaGUID = 0;
        m_uiPod_BetaGUID = 0;
        m_uiPod_DeltaGUID = 0;
        m_uiPod_GammaGUID = 0;
        m_uiPod_OmegaGUID = 0;

        m_uiGoSphereGUID = 0;
        m_uiMellicharGUID = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_CORE_SECURITY_FIELD_ALPHA: m_uiCore_Security_Field_AlphaGUID = pGo->GetGUID(); break;
            case GO_CORE_SECURITY_FIELD_BETA:  m_uiCore_Security_Field_BetaGUID =  pGo->GetGUID(); break;
            case GO_SEAL_SPHERE: m_uiGoSphereGUID = pGo->GetGUID(); break;
            case GO_POD_ALPHA: m_uiPod_AlphaGUID = pGo->GetGUID(); break;
            case GO_POD_BETA:  m_uiPod_BetaGUID =  pGo->GetGUID(); break;
            case GO_POD_DELTA: m_uiPod_DeltaGUID = pGo->GetGUID(); break;
            case GO_POD_GAMMA: m_uiPod_GammaGUID = pGo->GetGUID(); break;
            case GO_POD_OMEGA: m_uiPod_OmegaGUID = pGo->GetGUID(); break;
        }
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        if (pCreature->GetEntry() == NPC_MELLICHAR)
            m_uiMellicharGUID = pCreature->GetGUID();
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_ZEREKETH:
                m_auiEncounter[0] = uiData;
                break;

            case TYPE_DALLIAH:
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiCore_Security_Field_BetaGUID);
                m_auiEncounter[1] = uiData;
                break;

            case TYPE_SOCCOTHRATES:
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiCore_Security_Field_AlphaGUID);
                m_auiEncounter[2] = uiData;
                break;

            case TYPE_HARBINGERSKYRISS:
                if (uiData == NOT_STARTED || uiData == FAIL)
                {
                    m_auiEncounter[4] = NOT_STARTED;
                    m_auiEncounter[5] = NOT_STARTED;
                    m_auiEncounter[6] = NOT_STARTED;
                    m_auiEncounter[7] = NOT_STARTED;
                    m_auiEncounter[8] = NOT_STARTED;
                }
                m_auiEncounter[3] = uiData;
                break;

            case TYPE_WARDEN_1:
                if (uiData == IN_PROGRESS)
                    DoUseDoorOrButton(m_uiPod_AlphaGUID);
                m_auiEncounter[4] = uiData;
                break;

            case TYPE_WARDEN_2:
                if (uiData == IN_PROGRESS)
                    DoUseDoorOrButton(m_uiPod_BetaGUID);
                m_auiEncounter[5] = uiData;
                break;

            case TYPE_WARDEN_3:
                if (uiData == IN_PROGRESS)
                    DoUseDoorOrButton(m_uiPod_DeltaGUID);
                m_auiEncounter[6] = uiData;
                break;

            case TYPE_WARDEN_4:
                if (uiData == IN_PROGRESS)
                    DoUseDoorOrButton(m_uiPod_GammaGUID);
                m_auiEncounter[7] = uiData;
                break;

            case TYPE_WARDEN_5:
                if (uiData == IN_PROGRESS)
                    DoUseDoorOrButton(m_uiPod_OmegaGUID);
                m_auiEncounter[8] = uiData;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_HARBINGERSKYRISS:
                return m_auiEncounter[3];
            case TYPE_WARDEN_1:
                return m_auiEncounter[4];
            case TYPE_WARDEN_2:
                return m_auiEncounter[5];
            case TYPE_WARDEN_3:
                return m_auiEncounter[6];
            case TYPE_WARDEN_4:
                return m_auiEncounter[7];
            case TYPE_WARDEN_5:
                return m_auiEncounter[8];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_MELLICHAR:
                return m_uiMellicharGUID;
            case DATA_SPHERE_SHIELD:
                return m_uiGoSphereGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_arcatraz(Map* pMap)
{
    return new instance_arcatraz(pMap);
}

void AddSC_instance_arcatraz()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_arcatraz";
    newscript->GetInstanceData = &GetInstanceData_instance_arcatraz;
    newscript->RegisterSelf();
}
