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
SDName: Instance_Serpent_Shrine
SD%Complete: 90
SDComment:
SDCategory: Coilfang Resevoir, Serpent Shrine Cavern
EndScriptData */

#include "precompiled.h"
#include "serpent_shrine.h"

/* Serpentshrine cavern encounters:
0 - Hydross The Unstable event
1 - Leotheras The Blind Event
2 - The Lurker Below Event
3 - Fathom-Lord Karathress Event
4 - Morogrim Tidewalker Event
5 - Lady Vashj Event
*/

const int MAX_ENCOUNTER = 6;
const int MAX_GENERATOR = 4;

struct MANGOS_DLL_DECL instance_serpentshrine_cavern : public ScriptedInstance
{
    instance_serpentshrine_cavern(Map* pMap) : ScriptedInstance(pMap) { Initialize(); };

    uint64 m_uiSharkkis;
    uint64 m_uiTidalvess;
    uint64 m_uiCaribdis;
    uint64 m_uiLadyVashj;
    uint64 m_uiKarathress;
    uint64 m_uiKarathressEvent_Starter;

    uint32 m_auiShieldGenerator[MAX_GENERATOR];
    uint32 m_auiEncounter[MAX_ENCOUNTER];

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
        memset(&m_auiShieldGenerator, 0, sizeof(m_auiShieldGenerator));

        m_uiSharkkis = 0;
        m_uiTidalvess = 0;
        m_uiCaribdis = 0;
        m_uiLadyVashj = 0;
        m_uiKarathress = 0;
        m_uiKarathressEvent_Starter = 0;
    }

    bool IsEncounterInProgress()
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 21212: m_uiLadyVashj  = pCreature->GetGUID(); break;
            case 21214: m_uiKarathress = pCreature->GetGUID(); break;
            case 21966: m_uiSharkkis   = pCreature->GetGUID(); break;
            case 21965: m_uiTidalvess  = pCreature->GetGUID(); break;
            case 21964: m_uiCaribdis   = pCreature->GetGUID(); break;
        }
    }

    void SetData64(uint32 uiType, uint64 uiData)
    {
        if (uiType == DATA_KARATHRESS_STARTER)
            m_uiKarathressEvent_Starter = uiData;
    }

    uint64 GetData64(uint32 uiIdentifier)
    {
        switch(uiIdentifier)
        {
            case DATA_SHARKKIS:
                return m_uiSharkkis;
            case DATA_TIDALVESS:
                return m_uiTidalvess;
            case DATA_CARIBDIS:
                return m_uiCaribdis;
            case DATA_LADYVASHJ:
                return m_uiLadyVashj;
            case DATA_KARATHRESS:
                return m_uiKarathress;
            case DATA_KARATHRESS_STARTER:
                return m_uiKarathressEvent_Starter;
        }
        return 0;
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_HYDROSS_EVENT:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_LEOTHERAS_EVENT:
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_THELURKER_EVENT:
                m_auiEncounter[2] = uiData;
                break;
            case TYPE_KARATHRESS_EVENT:
                m_auiEncounter[3] = uiData;
                break;
            case TYPE_MOROGRIM_EVENT:
                m_auiEncounter[4] = uiData;
                break;
            case TYPE_LADYVASHJ_EVENT:
                if (uiData == NOT_STARTED)
                    memset(&m_auiShieldGenerator, 0, sizeof(m_auiShieldGenerator));
                m_auiEncounter[5] = uiData;
                break;
            case TYPE_SHIELDGENERATOR1:
                m_auiShieldGenerator[0] = uiData;
                break;
            case TYPE_SHIELDGENERATOR2:
                m_auiShieldGenerator[1] = uiData;
                break;
            case TYPE_SHIELDGENERATOR3:
                m_auiShieldGenerator[2] = uiData;
                break;
            case TYPE_SHIELDGENERATOR4:
                m_auiShieldGenerator[3] = uiData;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_HYDROSS_EVENT:
                return m_auiEncounter[0];

            case TYPE_LEOTHERAS_EVENT:
                return m_auiEncounter[1];

            case TYPE_THELURKER_EVENT:
                return m_auiEncounter[2];

            case TYPE_KARATHRESS_EVENT:
                return m_auiEncounter[3];

            case TYPE_MOROGRIM_EVENT:
                return m_auiEncounter[4];

            case TYPE_LADYVASHJ_EVENT:
                return m_auiEncounter[5];

            case TYPE_SHIELDGENERATOR1:
                return m_auiShieldGenerator[0];

            case TYPE_SHIELDGENERATOR2:
                return m_auiShieldGenerator[1];

            case TYPE_SHIELDGENERATOR3:
                return m_auiShieldGenerator[2];

            case TYPE_SHIELDGENERATOR4:
                return m_auiShieldGenerator[3];

            case TYPE_VASHJ_PHASE3_CHECK:
                for(uint8 i = 0; i < MAX_GENERATOR; ++i)
                {
                    if (m_auiShieldGenerator[i] != DONE)
                        return NOT_STARTED;
                }
                return DONE;
        }

        return 0;
    }
};

InstanceData* GetInstanceData_instance_serpentshrine_cavern(Map* pMap)
{
    return new instance_serpentshrine_cavern(pMap);
}

void AddSC_instance_serpentshrine_cavern()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_serpent_shrine";
    newscript->GetInstanceData = &GetInstanceData_instance_serpentshrine_cavern;
    newscript->RegisterSelf();
}
