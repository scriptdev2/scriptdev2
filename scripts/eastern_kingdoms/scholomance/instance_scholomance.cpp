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
SDName: Instance_Scholomance
SD%Complete: 80
SDComment: Door handling for Gandling rooms after player-teleporting (close on Add Aggro, open when all Adds dead) missing
SDCategory: Scholomance
EndScriptData */

#include "precompiled.h"
#include "scholomance.h"

/* Darkmaster Gandling Encounter - Adds handling (MISSING)
 * When a player is teleported to another room, there are 3 (perhaps sometimes 4) Adds spawned.
 * When the first add Aggroes, the door to the room is closed
 * When the last add dies, the door to the room is opened
 *
 * Also possible that this is better handled within the boss-script, depends on details of implementation of teleport spells
 */

instance_scholomance::instance_scholomance(Map* pMap) : ScriptedInstance(pMap),
    m_uiDarkmasterGandlingGUID(0),
    m_uiGateKirtonosGUID(0),
    m_uiGateRasGUID(0),
    m_uiGateMiliciaGUID(0),
    m_uiGateTheolenGUID(0),
    m_uiGatePolkeltGUID(0),
    m_uiGateRavenianGUID(0),
    m_uiGateBarovGUID(0),
    m_uiGateIlluciaGUID(0),
    m_uiGateGandlingGUID(0)
{
    Initialize();
}

void instance_scholomance::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_scholomance::OnPlayerEnter(Player* pPlayer)
{
    // Summon Gandling if can
    DoSpawnGandlingIfCan(true);
}

void instance_scholomance::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_DARKMASTER_GANDLING: m_uiDarkmasterGandlingGUID = pCreature->GetGUID(); break;
    }
}

void instance_scholomance::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_GATE_KIRTONOS: m_uiGateKirtonosGUID = pGo->GetGUID(); break;
        case GO_GATE_RAS:      m_uiGateRasGUID      = pGo->GetGUID(); break;
        case GO_GATE_MALICIA:  m_uiGateMiliciaGUID  = pGo->GetGUID(); break;
        case GO_GATE_THEOLEN:  m_uiGateTheolenGUID  = pGo->GetGUID(); break;
        case GO_GATE_POLKELT:  m_uiGatePolkeltGUID  = pGo->GetGUID(); break;
        case GO_GATE_RAVENIAN: m_uiGateRavenianGUID = pGo->GetGUID(); break;
        case GO_GATE_BAROV:    m_uiGateBarovGUID    = pGo->GetGUID(); break;
        case GO_GATE_ILLUCIA:  m_uiGateIlluciaGUID  = pGo->GetGUID(); break;
        case GO_GATE_GANDLING: m_uiGateGandlingGUID = pGo->GetGUID(); break;

        case GO_VIEWING_ROOM_DOOR:
            // In normal flow of the instance, this door is opened by a dropped key
            if (m_auiEncounter[TYPE_RATTLEGORE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
    }
}

void instance_scholomance::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_KIRTONOS:
            // This door is initially closed by DB-scripts, so only use it in case of FAIL, DONE, or on aggro after wipe
            if (m_auiEncounter[uiType] != FAIL && uiData == IN_PROGRESS)
                return;
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiGateKirtonosGUID);
            break;
        case TYPE_RATTLEGORE:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_RAS_FROSTWHISPER:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiGateRasGUID);
            break;
        case TYPE_MALICIA:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiGateMiliciaGUID);
            break;
        case TYPE_THEOLEN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiGateTheolenGUID);
            break;
        case TYPE_POLKELT:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiGatePolkeltGUID);
            break;
        case TYPE_RAVENIAN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiGateRavenianGUID);
            break;
        case TYPE_ALEXEI_BAROV:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiGateBarovGUID);
            break;
        case TYPE_ILLUCIA_BAROV:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(m_uiGateIlluciaGUID);
            break;
        case TYPE_GANDLING:
            m_auiEncounter[uiType] = uiData;
            // Close the door to main room, because the encounter will take place only in the main hall and random around all the 6 rooms
            // The door to each of the room needs to be controlled by checking the summoned adds
            DoUseDoorOrButton(m_uiGateGandlingGUID);
            break;
    }

    // Summon Gandling
    if (uiData == DONE)
        DoSpawnGandlingIfCan(false);

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << m_auiEncounter[9];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_scholomance::DoSpawnGandlingIfCan(bool bByPlayerEnter)
{
    // Summon only once
    if (m_uiDarkmasterGandlingGUID)
        return;

    // Do not summon, if event finished
    if (m_auiEncounter[TYPE_GANDLING] == DONE)
        return;

    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    // Check if all the six bosses are done first
    if (m_auiEncounter[TYPE_MALICIA] == DONE && m_auiEncounter[TYPE_THEOLEN] == DONE && m_auiEncounter[TYPE_POLKELT] == DONE &&
        m_auiEncounter[TYPE_RAVENIAN] == DONE && m_auiEncounter[TYPE_ALEXEI_BAROV] == DONE && m_auiEncounter[TYPE_ILLUCIA_BAROV] == DONE)
    {
        if (Creature* pGandling = pPlayer->SummonCreature(NPC_DARKMASTER_GANDLING, m_aGandlingSpawnLocs[0].m_fX, m_aGandlingSpawnLocs[0].m_fY, m_aGandlingSpawnLocs[0].m_fZ, m_aGandlingSpawnLocs[0].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0))
            if (!bByPlayerEnter)
                DoScriptText(SAY_GANDLING_SPAWN, pGandling);
    }
}

uint32 instance_scholomance::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_scholomance::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4]
               >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8] >> m_auiEncounter[9];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_scholomance::OnCreatureEnterCombat(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_KIRTONOS:            SetData(TYPE_KIRTONOS, IN_PROGRESS);         break;
        case NPC_RATTLEGORE:          SetData(TYPE_RATTLEGORE, IN_PROGRESS);       break;
        case NPC_RAS_FROSTWHISPER:    SetData(TYPE_RAS_FROSTWHISPER, IN_PROGRESS); break;
        case NPC_THEOLEN_KRASTINOV:   SetData(TYPE_THEOLEN, IN_PROGRESS);          break;
        case NPC_LOREKEEPER_POLKELT:  SetData(TYPE_POLKELT, IN_PROGRESS);          break;
        case NPC_RAVENIAN:            SetData(TYPE_RAVENIAN, IN_PROGRESS);         break;
        case NPC_ILLUCIA_BAROV:       SetData(TYPE_ILLUCIA_BAROV, IN_PROGRESS);    break;
        case NPC_ALEXEI_BAROV:        SetData(TYPE_ALEXEI_BAROV, IN_PROGRESS);     break;
        case NPC_INSTRUCTOR_MALICIA:  SetData(TYPE_MALICIA, IN_PROGRESS);          break;
        case NPC_DARKMASTER_GANDLING: SetData(TYPE_GANDLING, IN_PROGRESS);         break;
    }
}

void instance_scholomance::OnCreatureEvade(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_KIRTONOS:            SetData(TYPE_KIRTONOS, FAIL);         break;
        case NPC_RATTLEGORE:          SetData(TYPE_RATTLEGORE, FAIL);       break;
        case NPC_RAS_FROSTWHISPER:    SetData(TYPE_RAS_FROSTWHISPER, FAIL); break;
        case NPC_THEOLEN_KRASTINOV:   SetData(TYPE_THEOLEN, FAIL);          break;
        case NPC_LOREKEEPER_POLKELT:  SetData(TYPE_POLKELT, FAIL);          break;
        case NPC_RAVENIAN:            SetData(TYPE_RAVENIAN, FAIL);         break;
        case NPC_ILLUCIA_BAROV:       SetData(TYPE_ILLUCIA_BAROV, FAIL);    break;
        case NPC_ALEXEI_BAROV:        SetData(TYPE_ALEXEI_BAROV, FAIL);     break;
        case NPC_INSTRUCTOR_MALICIA:  SetData(TYPE_MALICIA, FAIL);          break;
        case NPC_DARKMASTER_GANDLING: SetData(TYPE_GANDLING, FAIL);         break;
    }
}

void instance_scholomance::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_KIRTONOS:            SetData(TYPE_KIRTONOS, DONE);         break;
        case NPC_RATTLEGORE:          SetData(TYPE_RATTLEGORE, DONE);       break;
        case NPC_RAS_FROSTWHISPER:    SetData(TYPE_RAS_FROSTWHISPER, DONE); break;
        case NPC_THEOLEN_KRASTINOV:   SetData(TYPE_THEOLEN, DONE);          break;
        case NPC_LOREKEEPER_POLKELT:  SetData(TYPE_POLKELT, DONE);          break;
        case NPC_RAVENIAN:            SetData(TYPE_RAVENIAN, DONE);         break;
        case NPC_ILLUCIA_BAROV:       SetData(TYPE_ILLUCIA_BAROV, DONE);    break;
        case NPC_ALEXEI_BAROV:        SetData(TYPE_ALEXEI_BAROV, DONE);     break;
        case NPC_INSTRUCTOR_MALICIA:  SetData(TYPE_MALICIA, DONE);          break;
        case NPC_DARKMASTER_GANDLING: SetData(TYPE_GANDLING, DONE);         break;
    }
}

InstanceData* GetInstanceData_instance_scholomance(Map* pMap)
{
    return new instance_scholomance(pMap);
}

void AddSC_instance_scholomance()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_scholomance";
    pNewScript->GetInstanceData = &GetInstanceData_instance_scholomance;
    pNewScript->RegisterSelf();
}
