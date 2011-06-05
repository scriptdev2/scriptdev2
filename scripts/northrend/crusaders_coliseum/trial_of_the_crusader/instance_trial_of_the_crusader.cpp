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
SDName: instance_trial_of_the_crusader
SD%Complete: 100
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

/* Trial Of The Crusader encounters:
0 - Northrend Beasts
1 - Jaraxxus
2 - Faction Champions
3 - Twin Valkyr
4 - Anubarak
*/

instance_trial_of_the_crusader::instance_trial_of_the_crusader(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_trial_of_the_crusader::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_trial_of_the_crusader::IsEncounterInProgress() const
{
    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS) return true;

    return false;
}

void instance_trial_of_the_crusader::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
         case TYPE_NORTHREND_BEASTS:
            m_auiEncounter[uiType] = uiData;
            break;
         case TYPE_JARAXXUS:
            m_auiEncounter[uiType] = uiData;
            break;
         case TYPE_FACTION_CHAMPIONS:
            m_auiEncounter[uiType] = uiData;
            break;
         case TYPE_TWIN_VALKYR:
            m_auiEncounter[uiType] = uiData;
            break;
         case TYPE_ANUBARAK:
            m_auiEncounter[uiType] = uiData;
            break;
         default:
            error_log("SD2: Instance Trial of The Crusader: ERROR SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
            return;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_trial_of_the_crusader::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_trial_of_the_crusader::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)            // Do not load an encounter as "In Progress" - reset it instead.
            m_auiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_trial_of_the_crusader(Map* pMap)
{
    return new instance_trial_of_the_crusader(pMap);
}

void AddSC_instance_trial_of_the_crusader()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_trial_of_the_crusader";
    pNewScript->GetInstanceData = &GetInstanceData_instance_trial_of_the_crusader;
    pNewScript->RegisterSelf();
}
