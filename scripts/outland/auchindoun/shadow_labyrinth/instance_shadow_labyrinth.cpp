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
SDName: Instance_Shadow_Labyrinth
SD%Complete: 85
SDComment: Some cleanup left along with save
SDCategory: Auchindoun, Shadow Labyrinth
EndScriptData */

#include "precompiled.h"
#include "shadow_labyrinth.h"

/* Shadow Labyrinth encounters:
1 - Ambassador Hellmaw event
2 - Blackheart the Inciter event
3 - Grandmaster Vorpil event
4 - Murmur event
*/

struct MANGOS_DLL_DECL instance_shadow_labyrinth : public ScriptedInstance
{
    instance_shadow_labyrinth(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    uint64 m_uiRefectoryDoorGUID;
    uint64 m_uiScreamingHallDoorGUID;

    uint64 m_uiGrandmasterVorpil;
    uint32 m_uiFelOverseerCount;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiRefectoryDoorGUID = 0;
        m_uiScreamingHallDoorGUID = 0;

        m_uiGrandmasterVorpil = 0;
        m_uiFelOverseerCount = 0;
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
            case GO_REFECTORY_DOOR:
                m_uiRefectoryDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_SCREAMING_HALL_DOOR:
                m_uiScreamingHallDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[3] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
        }
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 18732:
                m_uiGrandmasterVorpil = pCreature->GetGUID();
                break;
            case 18796:
                if (pCreature->isAlive())
                {
                    ++m_uiFelOverseerCount;
                    debug_log("SD2: Shadow Labyrinth: counting %u Fel Overseers.", m_uiFelOverseerCount);
                }
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_HELLMAW:
                m_auiEncounter[0] = uiData;
                break;

            case TYPE_OVERSEER:
                if (uiData != DONE)
                {
                    error_log("SD2: Shadow Labyrinth: TYPE_OVERSEER did not expect other data than DONE");
                    return;
                }
                if (m_uiFelOverseerCount)
                {
                    --m_uiFelOverseerCount;

                    if (m_uiFelOverseerCount)
                        debug_log("SD2: Shadow Labyrinth: %u Fel Overseers left to kill.", m_uiFelOverseerCount);
                    else
                    {
                        m_auiEncounter[1] = DONE;
                        debug_log("SD2: Shadow Labyrinth: TYPE_OVERSEER == DONE");
                    }
                }
                break;

            case TYPE_INCITER:
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiRefectoryDoorGUID);
                m_auiEncounter[2] = uiData;
                break;

            case TYPE_VORPIL:
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiScreamingHallDoorGUID);
                m_auiEncounter[3] = uiData;
                break;

            case TYPE_MURMUR:
                m_auiEncounter[4] = uiData;
                break;
        }

        if (uiData == DONE)
        {
            if (uiType == TYPE_OVERSEER && m_uiFelOverseerCount != 0)
                return;

            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " "
                << m_auiEncounter[2] << " " << m_auiEncounter[3] << " " << m_auiEncounter[4];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_HELLMAW:
                return m_auiEncounter[0];
            case TYPE_OVERSEER:
                return m_auiEncounter[1];
        }
        return false;
    }

    const char* Save()
    {
        return strInstData.c_str();
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);

        std::istringstream loadStream(in);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_shadow_labyrinth(Map* pMap)
{
    return new instance_shadow_labyrinth(pMap);
}

void AddSC_instance_shadow_labyrinth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_shadow_labyrinth";
    newscript->GetInstanceData = &GetInstanceData_instance_shadow_labyrinth;
    newscript->RegisterSelf();
}
