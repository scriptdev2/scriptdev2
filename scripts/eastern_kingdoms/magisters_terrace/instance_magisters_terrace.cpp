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
SDName: Instance_Magisters_Terrace
SD%Complete: 60
SDComment:  Designed only for Selin Fireheart
SDCategory: Magister's Terrace
EndScriptData */

#include "precompiled.h"
#include "magisters_terrace.h"

#define MAX_ENCOUNTER   4

/*
0  - Selin Fireheart
1  - Vexallus
2  - Priestess Delrissa
3  - Kael'thas Sunstrider
*/

struct MANGOS_DLL_DECL instance_magisters_terrace : public ScriptedInstance
{
    instance_magisters_terrace(Map* pMap) : ScriptedInstance(pMap) {Initialize();}

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint32 m_uiDelrissaDeathCount;

    std::list<uint64> FelCrystals;
    std::list<uint64>::iterator CrystalItr;

    uint64 m_uiSelinGUID;
    uint64 m_uiDelrissaGUID;
    uint64 m_uiVexallusDoorGUID;
    uint64 m_uiSelinDoorGUID;
    uint64 m_uiSelinEncounterDoorGUID;
    uint64 m_uiDelrissaDoorGUID;
    uint64 m_uiKaelDoorGUID;
    uint64 m_auiKaelStatue[2];

    bool m_bInitializedItr;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
        memset(&m_auiKaelStatue, 0, sizeof(m_auiKaelStatue));

        FelCrystals.clear();

        m_uiDelrissaDeathCount = 0;

        m_uiSelinGUID = 0;
        m_uiDelrissaGUID = 0;
        m_uiVexallusDoorGUID = 0;
        m_uiSelinDoorGUID = 0;
        m_uiSelinEncounterDoorGUID = 0;
        m_uiDelrissaDoorGUID = 0;
        m_uiKaelDoorGUID = 0;

        m_bInitializedItr = false;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;
        return false;
    }

    uint32 GetData(uint32 identifier)
    {
        switch(identifier)
        {
            case DATA_SELIN_EVENT:          return m_auiEncounter[0];
            case DATA_VEXALLUS_EVENT:       return m_auiEncounter[1];
            case DATA_DELRISSA_EVENT:       return m_auiEncounter[2];
            case DATA_KAELTHAS_EVENT:       return m_auiEncounter[3];
            case DATA_DELRISSA_DEATH_COUNT: return m_uiDelrissaDeathCount;
            case DATA_FEL_CRYSTAL_SIZE:     return FelCrystals.size();
        }
        return 0;
    }

    void SetData(uint32 identifier, uint32 data)
    {
        switch(identifier)
        {
            case DATA_SELIN_EVENT:
                m_auiEncounter[0] = data;
                break;
            case DATA_VEXALLUS_EVENT:
                if (data == DONE)
                    DoUseDoorOrButton(m_uiVexallusDoorGUID);
                m_auiEncounter[1] = data;
                break;
            case DATA_DELRISSA_EVENT:
                if (data == DONE)
                    DoUseDoorOrButton(m_uiDelrissaDoorGUID);
                if (data == IN_PROGRESS)
                    m_uiDelrissaDeathCount = 0;
                m_auiEncounter[2] = data;
                break;
            case DATA_KAELTHAS_EVENT:
                m_auiEncounter[3] = data;
                break;
            case DATA_DELRISSA_DEATH_COUNT:
                if (data == SPECIAL)
                    ++m_uiDelrissaDeathCount;
                else
                    m_uiDelrissaDeathCount = 0;
                break;
        }
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 24723: m_uiSelinGUID = pCreature->GetGUID(); break;
            case 24560: m_uiDelrissaGUID = pCreature->GetGUID(); break;
            case 24722: FelCrystals.push_back(pCreature->GetGUID()); break;
        }
    }

    void OnObjectCreate(GameObject* go)
    {
        switch(go->GetEntry())
        {
            case 187896:  m_uiVexallusDoorGUID = go->GetGUID();       break;
            //SunwellRaid Gate 02
            case 187979:  m_uiSelinDoorGUID = go->GetGUID();          break;
            //Assembly Chamber Door
            case 188065:  m_uiSelinEncounterDoorGUID = go->GetGUID(); break;
            case 187770:  m_uiDelrissaDoorGUID = go->GetGUID();       break;
            case 188064:  m_uiKaelDoorGUID = go->GetGUID();           break;
            case 188165:  m_auiKaelStatue[0] = go->GetGUID();         break;
            case 188166:  m_auiKaelStatue[1] = go->GetGUID();         break;
        }
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case DATA_SELIN:                return m_uiSelinGUID;
            case DATA_DELRISSA:             return m_uiDelrissaGUID;
            case DATA_VEXALLUS_DOOR:        return m_uiVexallusDoorGUID;
            case DATA_SELIN_DOOR:           return m_uiSelinDoorGUID;
            case DATA_SELIN_ENCOUNTER_DOOR: return m_uiSelinEncounterDoorGUID;
            case DATA_DELRISSA_DOOR:        return m_uiDelrissaDoorGUID;
            case DATA_KAEL_DOOR:            return m_uiKaelDoorGUID;
            case DATA_KAEL_STATUE_LEFT:     return m_auiKaelStatue[0];
            case DATA_KAEL_STATUE_RIGHT:    return m_auiKaelStatue[1];

            case DATA_FEL_CRYSTAL:
            {
                if (FelCrystals.empty())
                {
                    error_log("SD2: Magisters Terrace: No Fel Crystals loaded in Inst Data");
                    return 0;
                }

                if (!m_bInitializedItr)
                {
                    CrystalItr = FelCrystals.begin();
                    m_bInitializedItr = true;
                }

                uint64 guid = *CrystalItr;
                ++CrystalItr;
                return guid;
            }
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_magisters_terrace(Map* pMap)
{
    return new instance_magisters_terrace(pMap);
}

void AddSC_instance_magisters_terrace()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "instance_magisters_terrace";
    newscript->GetInstanceData = &GetInstanceData_instance_magisters_terrace;
    newscript->RegisterSelf();
}
