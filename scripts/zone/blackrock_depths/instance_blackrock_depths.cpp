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
SDName: Instance_Blackrock_Depths
SD%Complete: 20
SDComment: events: ring of law
SDCategory: Blackrock Depths
EndScriptData */

#include "precompiled.h"
#include "def_blackrock_depths.h"

enum
{
    ENCOUNTERS              = 6,

    NPC_EMPEROR             = 9019,
    NPC_PHALANX             = 9502,

    GO_ARENA1               = 161525,
    GO_ARENA2               = 161522,
    GO_ARENA3               = 161524,
    GO_ARENA4               = 161523,
    GO_SHADOW_LOCK          = 161460,
    GO_SHADOW_MECHANISM     = 161461,
    GO_SHADOW_GIANT_DOOR    = 157923,
    GO_SHADOW_DUMMY         = 161516,
    GO_BAR_KEG_SHOT         = 170607,
    GO_BAR_KEG_TRAP         = 171941,
    GO_BAR_DOOR             = 170571,
    GO_TOMB_ENTER           = 170576,
    GO_TOMB_EXIT            = 170577,
    GO_LYCEUM               = 170558,
    GO_GOLEM_ROOM_N         = 170573,
    GO_GOLEM_ROOM_S         = 170574,
    GO_THONE_ROOM           = 170575,

    GO_SPECTRAL_CHALICE     = 164869,
    GO_CHEST_SEVEN          = 169243
};

struct MANGOS_DLL_DECL instance_blackrock_depths : public ScriptedInstance
{
    instance_blackrock_depths(Map *map) : ScriptedInstance(map) {Initialize();};

    uint32 m_uiEncounter[ENCOUNTERS];
    std::string str_data;

    uint64 m_uiEmperorGUID;
    uint64 m_uiPhalanxGUID;

    uint64 m_uiGoArena1GUID;
    uint64 m_uiGoArena2GUID;
    uint64 m_uiGoArena3GUID;
    uint64 m_uiGoArena4GUID;
    uint64 m_uiGoShadowLockGUID;
    uint64 m_uiGoShadowMechGUID;
    uint64 m_uiGoShadowGiantGUID;
    uint64 m_uiGoShadowDummyGUID;
    uint64 m_uiGoBarKegGUID;
    uint64 m_uiGoBarKegTrapGUID;
    uint64 m_uiGoBarDoorGUID;
    uint64 m_uiGoTombEnterGUID;
    uint64 m_uiGoTombExitGUID;
    uint64 m_uiGoLyceumGUID;
    uint64 m_uiGoGolemNGUID;
    uint64 m_uiGoGolemSGUID;
    uint64 m_uiGoThoneGUID;

    uint64 m_uiSpectralChaliceGUID;
    uint64 m_uiSevensChestGUID;

    uint32 m_uiBarAleCount;

    void Initialize()
    {
        m_uiEmperorGUID = 0;
        m_uiPhalanxGUID = 0;

        m_uiGoArena1GUID = 0;
        m_uiGoArena2GUID = 0;
        m_uiGoArena3GUID = 0;
        m_uiGoArena4GUID = 0;
        m_uiGoShadowLockGUID = 0;
        m_uiGoShadowMechGUID = 0;
        m_uiGoShadowGiantGUID = 0;
        m_uiGoShadowDummyGUID = 0;
        m_uiGoBarKegGUID = 0;
        m_uiGoBarKegTrapGUID = 0;
        m_uiGoBarDoorGUID = 0;
        m_uiGoTombEnterGUID = 0;
        m_uiGoTombExitGUID = 0;
        m_uiGoLyceumGUID = 0;
        m_uiGoGolemNGUID = 0;
        m_uiGoGolemSGUID = 0;
        m_uiGoThoneGUID = 0;

        m_uiSpectralChaliceGUID = 0;
        m_uiSevensChestGUID = 0;

        m_uiBarAleCount = 0;

        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            m_uiEncounter[i] = NOT_STARTED;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_EMPEROR: m_uiEmperorGUID = pCreature->GetGUID(); break;
            case NPC_PHALANX: m_uiPhalanxGUID = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_ARENA1: m_uiGoArena1GUID = pGo->GetGUID(); break;
            case GO_ARENA2: m_uiGoArena2GUID = pGo->GetGUID(); break;
            case GO_ARENA3: m_uiGoArena3GUID = pGo->GetGUID(); break;
            case GO_ARENA4: m_uiGoArena4GUID = pGo->GetGUID(); break;
            case GO_SHADOW_LOCK: m_uiGoShadowLockGUID = pGo->GetGUID(); break;
            case GO_SHADOW_MECHANISM: m_uiGoShadowMechGUID = pGo->GetGUID(); break;
            case GO_SHADOW_GIANT_DOOR: m_uiGoShadowGiantGUID = pGo->GetGUID(); break;
            case GO_SHADOW_DUMMY: m_uiGoShadowDummyGUID = pGo->GetGUID(); break;
            case GO_BAR_KEG_SHOT: m_uiGoBarKegGUID = pGo->GetGUID(); break;
            case GO_BAR_KEG_TRAP: m_uiGoBarKegTrapGUID = pGo->GetGUID(); break;
            case GO_BAR_DOOR: m_uiGoBarDoorGUID = pGo->GetGUID(); break;
            case GO_TOMB_ENTER: m_uiGoTombEnterGUID = pGo->GetGUID(); break;
            case GO_TOMB_EXIT: m_uiGoTombExitGUID = pGo->GetGUID(); break;
            case GO_LYCEUM: m_uiGoLyceumGUID = pGo->GetGUID(); break;
            case GO_GOLEM_ROOM_N: m_uiGoGolemNGUID = pGo->GetGUID(); break;
            case GO_GOLEM_ROOM_S: m_uiGoGolemSGUID = pGo->GetGUID(); break;
            case GO_THONE_ROOM: m_uiGoThoneGUID = pGo->GetGUID(); break;
            case GO_SPECTRAL_CHALICE: m_uiSpectralChaliceGUID = pGo->GetGUID(); break;
            case GO_CHEST_SEVEN: m_uiSevensChestGUID = pGo->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        debug_log("SD2: Instance Blackrock Depths: SetData update (Type: %u Data %u)", uiType, uiData);

        switch(uiType)
        {
            case TYPE_RING_OF_LAW:
                m_uiEncounter[0] = uiData;
                break;
            case TYPE_VAULT:
                m_uiEncounter[1] = uiData;
                break;
            case TYPE_BAR:
                if (uiData == SPECIAL)
                    ++m_uiBarAleCount;
                else
                    m_uiEncounter[2] = uiData;
                break;
            case TYPE_TOMB_OF_SEVEN:
                if (uiData == DONE)
                    DoRespawnGameObject(m_uiSevensChestGUID, HOUR*IN_MILISECONDS);
                m_uiEncounter[3] = uiData;
                break;
            case TYPE_LYCEUM:
                m_uiEncounter[4] = uiData;
                break;
            case TYPE_IRON_HALL:
                m_uiEncounter[5] = uiData;
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_uiEncounter[0] << " " << m_uiEncounter[1] << " " << m_uiEncounter[2] << " "
                << m_uiEncounter[3] << " " << m_uiEncounter[4] << " " << m_uiEncounter[5];

            str_data = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_RING_OF_LAW:
                return m_uiEncounter[0];
            case TYPE_VAULT:
                return m_uiEncounter[1];
            case TYPE_BAR:
                if (m_uiEncounter[2] == IN_PROGRESS && m_uiBarAleCount == 3)
                    return SPECIAL;
                else
                    return m_uiEncounter[2];
            case TYPE_TOMB_OF_SEVEN:
                return m_uiEncounter[3];
            case TYPE_LYCEUM:
                return m_uiEncounter[4];
            case TYPE_IRON_HALL:
                return m_uiEncounter[5];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_EMPEROR:
                return m_uiEmperorGUID;
            case DATA_PHALANX:
                return m_uiPhalanxGUID;

            case DATA_ARENA1:
                return m_uiGoArena1GUID;
            case DATA_ARENA2:
                return m_uiGoArena2GUID;
            case DATA_ARENA3:
                return m_uiGoArena3GUID;
            case DATA_ARENA4:
                return m_uiGoArena4GUID;

            case DATA_GO_BAR_KEG:
                return m_uiGoBarKegGUID;
            case DATA_GO_BAR_KEG_TRAP:
                return m_uiGoBarKegTrapGUID;
            case DATA_GO_BAR_DOOR:
                return m_uiGoBarDoorGUID;
            case DATA_GO_CHALICE:
                return m_uiSpectralChaliceGUID;
        }
        return 0;
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
            >> m_uiEncounter[4] >> m_uiEncounter[5];

        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            if (m_uiEncounter[i] == IN_PROGRESS)
                m_uiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_blackrock_depths(Map* pMap)
{
    return new instance_blackrock_depths(pMap);
}

void AddSC_instance_blackrock_depths()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_blackrock_depths";
    newscript->GetInstanceData = &GetInstanceData_instance_blackrock_depths;
    newscript->RegisterSelf();
}
