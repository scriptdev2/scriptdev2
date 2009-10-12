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
SDName: Instance_Naxxramas
SD%Complete: 90%
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

struct MANGOS_DLL_DECL instance_naxxramas : public ScriptedInstance
{
    instance_naxxramas(Map* pMap) : ScriptedInstance(pMap) {Initialize();}

    std::string strInstData;
    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiAracEyeRampGUID;
    uint64 m_uiPlagEyeRampGUID;
    uint64 m_uiMiliEyeRampGUID;
    uint64 m_uiConsEyeRampGUID;

    uint64 m_uiAracPortalGUID;
    uint64 m_uiPlagPortalGUID;
    uint64 m_uiMiliPortalGUID;
    uint64 m_uiConsPortalGUID;

    uint64 m_uiAnubRekhanGUID;
    uint64 m_uiFaerlinanGUID;

    uint64 m_uiZeliekGUID;
    uint64 m_uiThaneGUID;
    uint64 m_uiBlaumeuxGUID;
    uint64 m_uiRivendareGUID;

    uint64 m_uiThaddiusGUID;
    uint64 m_uiStalaggGUID;
    uint64 m_uiFeugenGUID;

    uint64 m_uiPathExitDoorGUID;
    uint64 m_uiGlutExitDoorGUID;
    uint64 m_uiThadDoorGUID;

    uint64 m_uiAnubDoorGUID;
    uint64 m_uiAnubGateGUID;
    uint64 m_uiFaerDoorGUID;
    uint64 m_uiFaerWebGUID;
    uint64 m_uiMaexOuterGUID;
    uint64 m_uiMaexInnerGUID;

    uint64 m_uiGothCombatGateGUID;
    uint64 m_uiGothikEntryDoorGUID;
    uint64 m_uiGothikExitDoorGUID;
    uint64 m_uiHorsemenDoorGUID;
    uint64 m_uiHorsemenChestGUID;

    uint64 m_uiNothEntryDoorGUID;
    uint64 m_uiNothExitDoorGUID;
    uint64 m_uiHeigEntryDoorGUID;
    uint64 m_uiHeigExitDoorGUID;
    uint64 m_uiLoathebDoorGUID;

    uint64 m_uiKelthuzadDoorGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiAracEyeRampGUID     = 0;
        m_uiPlagEyeRampGUID     = 0;
        m_uiMiliEyeRampGUID     = 0;
        m_uiConsEyeRampGUID     = 0;

        m_uiAracPortalGUID      = 0;
        m_uiPlagPortalGUID      = 0;
        m_uiMiliPortalGUID      = 0;
        m_uiConsPortalGUID      = 0;

        m_uiAnubRekhanGUID      = 0;
        m_uiFaerlinanGUID       = 0;

        m_uiZeliekGUID          = 0;
        m_uiThaneGUID           = 0;
        m_uiBlaumeuxGUID        = 0;
        m_uiRivendareGUID       = 0;

        m_uiThaddiusGUID        = 0;
        m_uiStalaggGUID         = 0;
        m_uiFeugenGUID          = 0;

        m_uiPathExitDoorGUID    = 0;
        m_uiGlutExitDoorGUID    = 0;
        m_uiThadDoorGUID        = 0;

        m_uiAnubDoorGUID        = 0;
        m_uiAnubGateGUID        = 0;
        m_uiFaerDoorGUID        = 0;
        m_uiFaerWebGUID         = 0;
        m_uiMaexOuterGUID       = 0;
        m_uiMaexInnerGUID       = 0;

        m_uiGothCombatGateGUID  = 0;
        m_uiGothikEntryDoorGUID = 0;
        m_uiGothikExitDoorGUID  = 0;
        m_uiHorsemenDoorGUID    = 0;
        m_uiHorsemenChestGUID   = 0;

        m_uiNothEntryDoorGUID   = 0;
        m_uiNothExitDoorGUID    = 0;
        m_uiHeigEntryDoorGUID   = 0;
        m_uiHeigExitDoorGUID    = 0;
        m_uiLoathebDoorGUID     = 0;

        m_uiKelthuzadDoorGUID   = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_ANUB_REKHAN: m_uiAnubRekhanGUID = pCreature->GetGUID(); break;
            case NPC_FAERLINA:    m_uiFaerlinanGUID = pCreature->GetGUID();  break;
            case NPC_THADDIUS:    m_uiThaddiusGUID = pCreature->GetGUID();   break;
            case NPC_STALAGG:     m_uiStalaggGUID = pCreature->GetGUID();    break;
            case NPC_FEUGEN:      m_uiFeugenGUID = pCreature->GetGUID();     break;
            case NPC_ZELIEK:      m_uiZeliekGUID = pCreature->GetGUID();     break;
            case NPC_THANE:       m_uiThaneGUID = pCreature->GetGUID();      break;
            case NPC_BLAUMEUX:    m_uiBlaumeuxGUID = pCreature->GetGUID();   break;
            case NPC_RIVENDARE:   m_uiRivendareGUID = pCreature->GetGUID();  break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_ARAC_ANUB_DOOR:
                m_uiAnubDoorGUID = pGo->GetGUID();
                break;
            case GO_ARAC_ANUB_GATE:
                m_uiAnubGateGUID = pGo->GetGUID();
                if (m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_ARAC_FAER_WEB:
                m_uiFaerWebGUID = pGo->GetGUID();
                break;
            case GO_ARAC_FAER_DOOR:
                m_uiFaerDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[1] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_ARAC_MAEX_INNER_DOOR:
                m_uiMaexInnerGUID = pGo->GetGUID();
                break;
            case GO_ARAC_MAEX_OUTER_DOOR:
                m_uiMaexOuterGUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;

            case GO_PLAG_NOTH_ENTRY_DOOR:
                m_uiNothEntryDoorGUID = pGo->GetGUID();
                break;
            case GO_PLAG_NOTH_EXIT_DOOR:
                m_uiNothExitDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[3] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_PLAG_HEIG_ENTRY_DOOR:
                m_uiHeigEntryDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[3] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_PLAG_HEIG_EXIT_DOOR:
                m_uiHeigExitDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[4] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_PLAG_LOAT_DOOR:
                m_uiLoathebDoorGUID = pGo->GetGUID();
                break;

            case GO_MILI_GOTH_ENTRY_GATE:
                m_uiGothikEntryDoorGUID = pGo->GetGUID();
                break;
            case GO_MILI_GOTH_EXIT_GATE:
                m_uiGothikExitDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[7] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_MILI_GOTH_COMBAT_GATE:
                m_uiGothCombatGateGUID = pGo->GetGUID();
                break;
            case GO_MILI_HORSEMEN_DOOR:
                m_uiHorsemenDoorGUID  = pGo->GetGUID();
                if (m_auiEncounter[7] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;

            case GO_CHEST_HORSEMEN_NORM:
                m_uiHorsemenChestGUID = pGo->GetGUID();
                break;

            case GO_CONS_PATH_EXIT_DOOR:
                m_uiPathExitDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[9] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_CONS_GLUT_EXIT_DOOR:
                m_uiGlutExitDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[11] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_CONS_THAD_DOOR:
                m_uiThadDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[11] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;

            case GO_KELTHUZAD_WATERFALL_DOOR:
                m_uiKelthuzadDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[13] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;

            case GO_ARAC_EYE_RAMP:
                m_uiAracEyeRampGUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_PLAG_EYE_RAMP:
                m_uiPlagEyeRampGUID = pGo->GetGUID();
                if (m_auiEncounter[5] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_MILI_EYE_RAMP:
                m_uiMiliEyeRampGUID = pGo->GetGUID();
                if (m_auiEncounter[8] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_CONS_EYE_RAMP:
                m_uiConsEyeRampGUID = pGo->GetGUID();
                if (m_auiEncounter[12] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;

            case GO_ARAC_PORTAL:
                m_uiAracPortalGUID = pGo->GetGUID();
                break;
            case GO_PLAG_PORTAL:
                m_uiPlagPortalGUID = pGo->GetGUID();
                break;
            case GO_MILI_PORTAL:
                m_uiMiliPortalGUID = pGo->GetGUID();
                break;
            case GO_CONS_PORTAL:
                m_uiConsPortalGUID = pGo->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_ANUB_REKHAN:
                m_auiEncounter[0] = uiData;
                DoUseDoorOrButton(m_uiAnubDoorGUID);
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiAnubGateGUID);
                break;
            case TYPE_FAERLINA:
                m_auiEncounter[1] = uiData;
                DoUseDoorOrButton(m_uiFaerWebGUID);
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiFaerDoorGUID);
                    DoUseDoorOrButton(m_uiMaexOuterGUID);
                }
                break;
            case TYPE_MAEXXNA:
                m_auiEncounter[2] = uiData;
                DoUseDoorOrButton(m_uiMaexInnerGUID, uiData);
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiAracEyeRampGUID);
                    DoRespawnGameObject(m_uiAracPortalGUID, 30*MINUTE);
                }
                break;
            case TYPE_NOTH:
                m_auiEncounter[3] = uiData;
                DoUseDoorOrButton(m_uiNothEntryDoorGUID);
                if (uiData == DONE)
                {
                     DoUseDoorOrButton(m_uiNothExitDoorGUID);
                     DoUseDoorOrButton(m_uiHeigEntryDoorGUID);
                }
                break;
            case TYPE_HEIGAN:
                m_auiEncounter[4] = uiData;
                DoUseDoorOrButton(m_uiHeigEntryDoorGUID);
                if (uiData == DONE)
                     DoUseDoorOrButton(m_uiHeigExitDoorGUID);
                break;
            case TYPE_LOATHEB:
                m_auiEncounter[5] = uiData;
                DoUseDoorOrButton(m_uiLoathebDoorGUID);
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiPlagEyeRampGUID);
                    DoRespawnGameObject(m_uiPlagPortalGUID, 30*MINUTE);
                }
                break;
            case TYPE_RAZUVIOUS:
                m_auiEncounter[6] = uiData;
                if (uiData == DONE)
                     DoUseDoorOrButton(m_uiGothikEntryDoorGUID);
                break;
            case TYPE_GOTHIK:
                m_auiEncounter[7] = uiData;
                DoUseDoorOrButton(m_uiGothikEntryDoorGUID);
                if (uiData == DONE)
                {
                     DoUseDoorOrButton(m_uiGothikExitDoorGUID);
                     DoUseDoorOrButton(m_uiHorsemenDoorGUID);
                }
                break;
            case TYPE_FOUR_HORSEMEN:
                m_auiEncounter[8] = uiData;
                DoUseDoorOrButton(m_uiHorsemenDoorGUID);
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiMiliEyeRampGUID);
                    DoRespawnGameObject(m_uiMiliPortalGUID, 30*MINUTE);
                    DoRespawnGameObject(m_uiHorsemenChestGUID, 30*MINUTE);
                }
                break;
        case TYPE_PATCHWERK:
                m_auiEncounter[9] = uiData;
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiPathExitDoorGUID);
                break;
            case TYPE_GROBBULUS:
                m_auiEncounter[10] = uiData;
                break;
            case TYPE_GLUTH:
                m_auiEncounter[11] = uiData;
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiGlutExitDoorGUID);
                    DoUseDoorOrButton(m_uiThadDoorGUID);
                }
                break;
            case TYPE_THADDIUS:
                m_auiEncounter[12] = uiData;
                DoUseDoorOrButton(m_uiThadDoorGUID, uiData);
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiConsEyeRampGUID);
                    DoRespawnGameObject(m_uiConsPortalGUID, 30*MINUTE);
                }
                break;
            case TYPE_SAPPHIRON:
                m_auiEncounter[13] = uiData;
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiKelthuzadDoorGUID);
                break;
            case TYPE_KELTHUZAD:
                m_auiEncounter[14] = uiData;
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
                << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11] << " "
                << m_auiEncounter[12] << " " << m_auiEncounter[13] << " " << m_auiEncounter[14];

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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
            >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
            >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11]
            >> m_auiEncounter[12] >> m_auiEncounter[13] >> m_auiEncounter[14];

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
            case TYPE_ANUB_REKHAN:
                return m_auiEncounter[0];
            case TYPE_FAERLINA:
                return m_auiEncounter[1];
            case TYPE_MAEXXNA:
                return m_auiEncounter[2];
            case TYPE_NOTH:
                return m_auiEncounter[3];
            case TYPE_HEIGAN:
                return m_auiEncounter[4];
            case TYPE_LOATHEB:
                return m_auiEncounter[5];
            case TYPE_RAZUVIOUS:
                return m_auiEncounter[6];
            case TYPE_GOTHIK:
                return m_auiEncounter[7];
            case TYPE_FOUR_HORSEMEN:
                return m_auiEncounter[8];
            case TYPE_PATCHWERK:
                return m_auiEncounter[9];
            case TYPE_GROBBULUS:
                return m_auiEncounter[10];
            case TYPE_GLUTH:
                return m_auiEncounter[11];
            case TYPE_THADDIUS:
                return m_auiEncounter[12];
            case TYPE_SAPPHIRON:
                return m_auiEncounter[13];
            case TYPE_KELTHUZAD:
                return m_auiEncounter[14];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_ANUB_REKHAN:
                return m_uiAnubRekhanGUID;
            case NPC_FAERLINA:
                return m_uiFaerlinanGUID;
            case GO_MILI_GOTH_COMBAT_GATE:
                return m_uiGothCombatGateGUID;
            case NPC_ZELIEK:
                return m_uiZeliekGUID;
            case NPC_THANE:
                return m_uiThaneGUID;
            case NPC_BLAUMEUX:
                return m_uiBlaumeuxGUID;
            case NPC_RIVENDARE:
                return m_uiRivendareGUID;
            case NPC_THADDIUS:
                return m_uiThaddiusGUID;
            case NPC_STALAGG:
                return m_uiStalaggGUID;
            case NPC_FEUGEN:
                return m_uiFeugenGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_naxxramas(Map* pMap)
{
    return new instance_naxxramas(pMap);
}

void AddSC_instance_naxxramas()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_naxxramas";
    pNewScript->GetInstanceData = &GetInstanceData_instance_naxxramas;
    pNewScript->RegisterSelf();
}
