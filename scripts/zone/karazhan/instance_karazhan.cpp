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
SDName: Instance_Karazhan
SD%Complete: 70
SDComment: Instance Script for Karazhan to help in various encounters. TODO: GameObject visibility for Opera event.
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "def_karazhan.h"

#define ENCOUNTERS      12

/*
0  - Attumen + Midnight (optional)
1  - Moroes
2  - Maiden of Virtue (optional)
3  - Hyakiss the Lurker /  Rokad the Ravager  / Shadikith the Glider
4  - Opera Event
5  - Curator
6  - Shade of Aran (optional)
7  - Terestian Illhoof (optional)
8  - Netherspite (optional)
9  - Chess Event
10 - Prince Malchezzar
11 - Nightbane
*/

struct MANGOS_DLL_DECL instance_karazhan : public ScriptedInstance
{
    instance_karazhan(Map* pMap) : ScriptedInstance(pMap) {Initialize();}

    uint32 m_uiEncounter[ENCOUNTERS];
    std::string str_data;

    uint32 m_uiOperaEvent;
    uint32 m_uiOzDeathCount;

    uint64 m_uiCurtainGUID;
    uint64 m_uiStageDoorLeftGUID;
    uint64 m_uiStageDoorRightGUID;
    uint64 m_uiKilrekGUID;
    uint64 m_uiTerestianGUID;
    uint64 m_uiMoroesGUID;
    uint64 m_uiLibraryDoor;                                 // Door at Shade of Aran
    uint64 m_uiMassiveDoor;                                 // Door at Netherspite
    uint64 m_uiSideEntranceDoor;                            // Side Entrance
    uint64 m_uiGamesmansDoor;                               // Door before Chess
    uint64 m_uiGamesmansExitDoor;                           // Door after Chess
    uint64 m_uiNetherspaceDoor;                             // Door at Malchezaar
    uint64 m_uiDustCoveredChest;                            // Chest respawn at event complete

    void Initialize()
    {
        for (uint8 i = 0; i < ENCOUNTERS; ++i)
            m_uiEncounter[i] = NOT_STARTED;

        m_uiOperaEvent          = urand(EVENT_OZ,EVENT_RAJ);// 1 - OZ, 2 - HOOD, 3 - RAJ, this never gets altered.
        m_uiOzDeathCount        = 0;

        m_uiCurtainGUID         = 0;
        m_uiStageDoorLeftGUID   = 0;
        m_uiStageDoorRightGUID  = 0;

        m_uiKilrekGUID          = 0;
        m_uiTerestianGUID       = 0;
        m_uiMoroesGUID          = 0;

        m_uiLibraryDoor         = 0;
        m_uiMassiveDoor         = 0;
        m_uiSideEntranceDoor    = 0;
        m_uiGamesmansDoor       = 0;
        m_uiGamesmansExitDoor   = 0;
        m_uiNetherspaceDoor     = 0;
        m_uiDustCoveredChest    = 0;
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < ENCOUNTERS; ++i)
            if (m_uiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    uint32 GetData(uint32 uiData)
    {
        switch (uiData)
        {
            case DATA_ATTUMEN_EVENT:          return m_uiEncounter[0];
            case DATA_MOROES_EVENT:           return m_uiEncounter[1];
            case DATA_MAIDENOFVIRTUE_EVENT:   return m_uiEncounter[2];
            case DATA_OPTIONAL_BOSS_EVENT:    return m_uiEncounter[3];
            case DATA_OPERA_EVENT:            return m_uiEncounter[4];
            case DATA_CURATOR_EVENT:          return m_uiEncounter[5];
            case DATA_SHADEOFARAN_EVENT:      return m_uiEncounter[6];
            case DATA_TERESTIAN_EVENT:        return m_uiEncounter[7];
            case DATA_NETHERSPITE_EVENT:      return m_uiEncounter[8];
            case DATA_CHESS_EVENT:            return m_uiEncounter[9];
            case DATA_MALCHEZZAR_EVENT:       return m_uiEncounter[10];
            case DATA_NIGHTBANE_EVENT:        return m_uiEncounter[11];
            case DATA_OPERA_PERFORMANCE:      return m_uiOperaEvent;
            case DATA_OPERA_OZ_DEATHCOUNT:    return m_uiOzDeathCount;
        }

        return 0;
    }

    void OnCreatureCreate(Creature* pCreature, uint32 entry)
    {
        switch (pCreature->GetEntry())
        {
            case 17229: m_uiKilrekGUID = pCreature->GetGUID(); break;
            case 15688: m_uiTerestianGUID = pCreature->GetGUID(); break;
            case 15687: m_uiMoroesGUID = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case 183932: m_uiCurtainGUID        = pGo->GetGUID(); break;
            case 184278: m_uiStageDoorLeftGUID  = pGo->GetGUID(); break;
            case 184279: m_uiStageDoorRightGUID = pGo->GetGUID(); break;
            case 184517: m_uiLibraryDoor        = pGo->GetGUID(); break;
            case 185521: m_uiMassiveDoor        = pGo->GetGUID(); break;
            case 184276: m_uiGamesmansDoor      = pGo->GetGUID(); break;
            case 184277: m_uiGamesmansExitDoor  = pGo->GetGUID(); break;
            case 185134: m_uiNetherspaceDoor    = pGo->GetGUID(); break;
            case 184275:
                m_uiSideEntranceDoor = pGo->GetGUID();
                if (GetData(DATA_OPERA_EVENT) != DONE)
                    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED);
                else
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED);
                break;
            case 185119: m_uiDustCoveredChest = pGo->GetGUID(); break;
        }

        switch(m_uiOperaEvent)
        {
            //TODO: Set DoRespawnGameObject for Opera based on performance
            case EVENT_OZ:
                break;
            case EVENT_HOOD:
                break;
            case EVENT_RAJ:
                break;
        }
    }

    uint64 GetData64(uint32 data)
    {
        switch (data)
        {
            case DATA_KILREK:                       return m_uiKilrekGUID;
            case DATA_TERESTIAN:                    return m_uiTerestianGUID;
            case DATA_MOROES:                       return m_uiMoroesGUID;
            case DATA_GAMEOBJECT_STAGEDOORLEFT:     return m_uiStageDoorLeftGUID;
            case DATA_GAMEOBJECT_STAGEDOORRIGHT:    return m_uiStageDoorRightGUID;
            case DATA_GAMEOBJECT_CURTAINS:          return m_uiCurtainGUID;
            case DATA_GAMEOBJECT_LIBRARY_DOOR:      return m_uiLibraryDoor;
            case DATA_GAMEOBJECT_MASSIVE_DOOR:      return m_uiMassiveDoor;
            case DATA_GO_SIDE_ENTRANCE_DOOR:        return m_uiSideEntranceDoor;
            case DATA_GAMEOBJECT_GAME_DOOR:         return m_uiGamesmansDoor;
            case DATA_GAMEOBJECT_GAME_EXIT_DOOR:    return m_uiGamesmansExitDoor;
            case DATA_GAMEOBJECT_NETHER_DOOR:       return m_uiNetherspaceDoor;
        }

        return 0;
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case DATA_ATTUMEN_EVENT:            m_uiEncounter[0] = uiData; break;
            case DATA_MOROES_EVENT:
                if (m_uiEncounter[1] == DONE)
                    break;
                m_uiEncounter[1] = uiData;
                break;
            case DATA_MAIDENOFVIRTUE_EVENT:     m_uiEncounter[2] = uiData; break;
            case DATA_OPTIONAL_BOSS_EVENT:      m_uiEncounter[3] = uiData; break;
            case DATA_OPERA_EVENT:              m_uiEncounter[4] = uiData; break;
            case DATA_CURATOR_EVENT:            m_uiEncounter[5] = uiData; break;
            case DATA_SHADEOFARAN_EVENT:        m_uiEncounter[6] = uiData; break;
            case DATA_TERESTIAN_EVENT:          m_uiEncounter[7] = uiData; break;
            case DATA_NETHERSPITE_EVENT:        m_uiEncounter[8] = uiData; break;
            case DATA_CHESS_EVENT:
                if (uiData == DONE)
                    DoRespawnGameObject(m_uiDustCoveredChest,DAY);
                m_uiEncounter[9] = uiData;
                break;
            case DATA_MALCHEZZAR_EVENT:         m_uiEncounter[10] = uiData; break;
            case DATA_NIGHTBANE_EVENT:          m_uiEncounter[11] = uiData; break;
            case DATA_OPERA_OZ_DEATHCOUNT:      ++m_uiOzDeathCount; break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_uiEncounter[0] << " " << m_uiEncounter[1] << " " << m_uiEncounter[2] << " "
                << m_uiEncounter[3] << " " << m_uiEncounter[4] << " " << m_uiEncounter[5] << " "
                << m_uiEncounter[6] << " " << m_uiEncounter[7] << " " << m_uiEncounter[8] << " "
                << m_uiEncounter[9] << " " << m_uiEncounter[10] << " " << m_uiEncounter[11];

            str_data = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return str_data.c_str();
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
        loadStream >> m_uiEncounter[0] >> m_uiEncounter[1] >> m_uiEncounter[2] >> m_uiEncounter[3]
            >> m_uiEncounter[4] >> m_uiEncounter[5] >> m_uiEncounter[6] >> m_uiEncounter[7]
            >> m_uiEncounter[8] >> m_uiEncounter[9] >> m_uiEncounter[10] >> m_uiEncounter[11];

        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            if (m_uiEncounter[i] == IN_PROGRESS)            // Do not load an encounter as "In Progress" - reset it instead.
                m_uiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_karazhan(Map* pMap)
{
    return new instance_karazhan(pMap);
}

void AddSC_instance_karazhan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_karazhan";
    newscript->GetInstanceData = &GetInstanceData_instance_karazhan;
    newscript->RegisterSelf();
}
