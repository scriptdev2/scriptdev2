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
SDName: Instance_Mount_Hyjal
SD%Complete: 100
SDComment: Instance Data Scripts and functions to acquire mobs and set encounter status for use in various Hyjal Scripts
SDCategory: Caverns of Time, Mount Hyjal
EndScriptData */

#include "precompiled.h"
#include "def_hyjal.h"

enum
{
    MAX_ENCOUNTER       = 5,

    GO_ANCIENT_GEM      = 185557
};

/* Battle of Mount Hyjal encounters:
0 - Rage Winterchill event
1 - Anetheron event
2 - Kaz'rogal event
3 - Azgalor event
4 - Archimonde event
*/

struct MANGOS_DLL_DECL instance_mount_hyjal : public ScriptedInstance
{
    instance_mount_hyjal(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strSaveData;

    std::list<uint64> lAncientGemGUIDList;

    uint64 m_uiRageWinterchill;
    uint64 m_uiAnetheron;
    uint64 m_uiKazrogal;
    uint64 m_uiAzgalor;
    uint64 m_uiArchimonde;
    uint64 m_uiJainaProudmoore;
    uint64 m_uiThrall;
    uint64 m_uiTyrandeWhisperwind;

    uint32 m_uiTrashCount;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        lAncientGemGUIDList.clear();

        m_uiRageWinterchill = 0;
        m_uiAnetheron = 0;
        m_uiKazrogal = 0;
        m_uiAzgalor = 0;
        m_uiArchimonde = 0;
        m_uiJainaProudmoore = 0;
        m_uiThrall = 0;
        m_uiTyrandeWhisperwind = 0;

        m_uiTrashCount = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 17767: m_uiRageWinterchill = pCreature->GetGUID(); break;
            case 17808: m_uiAnetheron = pCreature->GetGUID(); break;
            case 17888: m_uiKazrogal = pCreature->GetGUID();  break;
            case 17842: m_uiAzgalor = pCreature->GetGUID(); break;
            case 17968: m_uiArchimonde = pCreature->GetGUID(); break;
            case 17772: m_uiJainaProudmoore = pCreature->GetGUID(); break;
            case 17852: m_uiThrall = pCreature->GetGUID(); break;
            case 17948: m_uiTyrandeWhisperwind = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        if (pGo->GetEntry() == GO_ANCIENT_GEM)
            lAncientGemGUIDList.push_back(pGo->GetGUID());
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_RAGEWINTERCHILL: return m_uiRageWinterchill;
            case DATA_ANETHERON: return m_uiAnetheron;
            case DATA_KAZROGAL: return m_uiKazrogal;
            case DATA_AZGALOR: return m_uiAzgalor;
            case DATA_ARCHIMONDE: return m_uiArchimonde;
            case DATA_JAINAPROUDMOORE: return m_uiJainaProudmoore;
            case DATA_THRALL: return m_uiThrall;
            case DATA_TYRANDEWHISPERWIND: return m_uiTyrandeWhisperwind;
        }

        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case DATA_RAGEWINTERCHILLEVENT: m_auiEncounter[0] = data; break;
            case DATA_ANETHERONEVENT:       m_auiEncounter[1] = data; break;
            case DATA_KAZROGALEVENT:        m_auiEncounter[2] = data; break;
            case DATA_AZGALOREVENT:         m_auiEncounter[3] = data; break;
            case DATA_ARCHIMONDEEVENT:      m_auiEncounter[4] = data; break;
            case DATA_RESET_TRASH_COUNT:    m_uiTrashCount = 0;       break;

            case DATA_TRASH:
                if (data)
                    m_uiTrashCount = data;
                else
                    --m_uiTrashCount;

                UpdateWorldState(WORLD_STATE_ENEMYCOUNT, m_uiTrashCount);
                break;

            case TYPE_RETREAT:
                if (data == SPECIAL)
                {
                    if (!lAncientGemGUIDList.empty())
                    {
                        for(std::list<uint64>::iterator itr = lAncientGemGUIDList.begin(); itr != lAncientGemGUIDList.end(); ++itr)
                        {
                            //don't know how long it expected
                            DoRespawnGameObject(*itr,DAY);
                        }
                    }
                }
                break;
        }

        debug_log("SD2: Instance Hyjal: Instance data updated for event %u (Data=%u)",type,data);

        if (data == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4];

            strSaveData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case DATA_RAGEWINTERCHILLEVENT: return m_auiEncounter[0];
            case DATA_ANETHERONEVENT:       return m_auiEncounter[1];
            case DATA_KAZROGALEVENT:        return m_auiEncounter[2];
            case DATA_AZGALOREVENT:         return m_auiEncounter[3];
            case DATA_ARCHIMONDEEVENT:      return m_auiEncounter[4];
            case DATA_TRASH:                return m_uiTrashCount;
        }
        return 0;
    }

    void UpdateWorldState(uint32 id, uint32 state)
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* pPlayer = itr->getSource())
                    pPlayer->SendUpdateWorldState(id,state);
            }
        }
        else
            debug_log("SD2: Instance Hyjal: UpdateWorldState, but PlayerList is empty!");
    }

    const char* Save()
    {
        return strSaveData.c_str();
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
            if (m_auiEncounter[i] == IN_PROGRESS)                // Do not load an encounter as IN_PROGRESS - reset it instead.
                m_auiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_mount_hyjal(Map* pMap)
{
    return new instance_mount_hyjal(pMap);
}

void AddSC_instance_mount_hyjal()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_hyjal";
    newscript->GetInstanceData = &GetInstanceData_instance_mount_hyjal;
    newscript->RegisterSelf();
}
