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
SDName: Instance_Molten_Core
SD%Complete: 0
SDComment: Place Holder
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"

struct MANGOS_DLL_DECL instance_molten_core : public ScriptedInstance
{
    instance_molten_core(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    uint64 m_uiLucifronGUID, m_uiMagmadarGUID, m_uiGehennasGUID, m_uiGarrGUID, m_uiGeddonGUID, m_uiShazzrahGUID, m_uiSulfuronGUID, m_uiGolemaggGUID, m_uiMajorDomoGUID, m_uiRagnarosGUID, m_uiFlamewakerPriestGUID;
    uint64 m_uiRuneKoroGUID, m_uiRuneZethGUID, m_uiRuneMazjGUID, m_uiRuneTheriGUID, m_uiRuneBlazGUID, m_uiRuneKressGUID, m_uiRuneMohnGUID, m_uiFirelordCacheGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiLucifronGUID = 0;
        m_uiMagmadarGUID = 0;
        m_uiGehennasGUID = 0;
        m_uiGarrGUID = 0;
        m_uiGeddonGUID = 0;
        m_uiShazzrahGUID = 0;
        m_uiSulfuronGUID = 0;
        m_uiGolemaggGUID = 0;
        m_uiMajorDomoGUID = 0;
        m_uiRagnarosGUID = 0;
        m_uiFlamewakerPriestGUID = 0;

        m_uiRuneKoroGUID = 0;
        m_uiRuneZethGUID = 0;
        m_uiRuneMazjGUID = 0;
        m_uiRuneTheriGUID = 0;
        m_uiRuneBlazGUID = 0;
        m_uiRuneKressGUID = 0;
        m_uiRuneMohnGUID = 0;

        m_uiFirelordCacheGUID = 0;
    }

    bool IsEncounterInProgress() const
    {
        return false;
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case 176951:                                    //Sulfuron
                m_uiRuneKoroGUID = pGo->GetGUID();
                break;
            case 176952:                                    //Geddon
                m_uiRuneZethGUID = pGo->GetGUID();
                break;
            case 176953:                                    //Shazzrah
                m_uiRuneMazjGUID = pGo->GetGUID();
                break;
            case 176954:                                    //Golemagg
                m_uiRuneTheriGUID = pGo->GetGUID();
                break;
            case 176955:                                    //Garr
                m_uiRuneBlazGUID = pGo->GetGUID();
                break;
            case 176956:                                    //Magmadar
                m_uiRuneKressGUID = pGo->GetGUID();
                break;
            case 176957:                                    //Gehennas
                m_uiRuneMohnGUID = pGo->GetGUID();
                break;
            case 179703:
                m_uiFirelordCacheGUID = pGo->GetGUID();     //majordomo event chest
                break;
        }
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_LUCIFRON:
                m_uiLucifronGUID = pCreature->GetGUID();
                break;
            case NPC_MAGMADAR:
                m_uiMagmadarGUID = pCreature->GetGUID();
                break;
            case NPC_GEHENNAS:
                m_uiGehennasGUID = pCreature->GetGUID();
                break;
            case NPC_GARR:
                m_uiGarrGUID = pCreature->GetGUID();
                break;
            case NPC_GEDDON:
                m_uiGeddonGUID = pCreature->GetGUID();
                break;
            case NPC_SHAZZRAH:
                m_uiShazzrahGUID = pCreature->GetGUID();
                break;
            case NPC_SULFURON:
                m_uiSulfuronGUID = pCreature->GetGUID();
                break;
            case NPC_GOLEMAGG:
                m_uiGolemaggGUID = pCreature->GetGUID();
                break;
            case NPC_DOMO:
                m_uiMajorDomoGUID = pCreature->GetGUID();
                break;
            case NPC_RAGNAROS:
                m_uiRagnarosGUID = pCreature->GetGUID();
                break;
            case NPC_FLAMEWAKERPRIEST:
                m_uiFlamewakerPriestGUID = pCreature->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_SULFURON:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_GEDDON:
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_SHAZZRAH:
                m_auiEncounter[2] = uiData;
                break;
            case TYPE_GOLEMAGG:
                m_auiEncounter[3] = uiData;
                break;
            case TYPE_GARR:
                m_auiEncounter[4] = uiData;
                break;
            case TYPE_MAGMADAR:
                m_auiEncounter[5] = uiData;
                break;
            case TYPE_GEHENNAS:
                m_auiEncounter[6] = uiData;
                break;
            case TYPE_LUCIFRON:
                m_auiEncounter[7] = uiData;
                break;
            case TYPE_MAJORDOMO:
                if (uiData == DONE)
                    DoRespawnGameObject(m_uiFirelordCacheGUID);
                m_auiEncounter[8] = uiData;
                break;
            case TYPE_RAGNAROS:
                m_auiEncounter[9] = uiData;
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
                << m_auiEncounter[9];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strInstData.c_str();
    }

    bool CanSpawnMajorDomo()
    {
        return m_auiEncounter[0] && m_auiEncounter[1] && m_auiEncounter[2] && m_auiEncounter[3] &&
            m_auiEncounter[4] && m_auiEncounter[5] && m_auiEncounter[6];
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_SULFURON:
                return m_auiEncounter[0];
            case TYPE_GEDDON:
                return m_auiEncounter[1];
            case TYPE_SHAZZRAH:
                return m_auiEncounter[2];
            case TYPE_GOLEMAGG:
                return m_auiEncounter[3];
            case TYPE_GARR:
                return m_auiEncounter[4];
            case TYPE_MAGMADAR:
                return m_auiEncounter[5];
            case TYPE_GEHENNAS:
                return m_auiEncounter[6];
            case TYPE_LUCIFRON:
                return m_auiEncounter[7];
            case TYPE_MAJORDOMO:
                return m_auiEncounter[8];
            case TYPE_RAGNAROS:
                return m_auiEncounter[9];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_SULFURON:
                return m_uiSulfuronGUID;
            case DATA_GOLEMAGG:
                return m_uiGolemaggGUID;
            case DATA_GARR:
                return m_uiGarrGUID;
            case DATA_MAJORDOMO:
                return m_uiMajorDomoGUID;
        }

        return 0;
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
            >> m_auiEncounter[8] >> m_auiEncounter[9];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)           // Do not load an encounter as "In Progress" - reset it instead.
                m_auiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstance_instance_molten_core(Map* pMap)
{
    return new instance_molten_core(pMap);
}

void AddSC_instance_molten_core()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_molten_core";
    newscript->GetInstanceData = &GetInstance_instance_molten_core;
    newscript->RegisterSelf();
}
