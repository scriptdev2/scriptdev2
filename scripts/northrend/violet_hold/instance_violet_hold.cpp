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
SDName: Instance_Violet_Hold
SD%Complete: 50
SDComment: "experimental" use of header/source object
SDCategory: Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

instance_violet_hold::instance_violet_hold(Map* pMap) : ScriptedInstance(pMap),
    m_uiSinclariGUID(0),
    m_uiSinclariAltGUID(0),
    m_uiErekemGUID(0),
    m_uiMoraggGUID(0),
    m_uiIchoronGUID(0),
    m_uiXevozzGUID(0),
    m_uiLavanthorGUID(0),
    m_uiZuramatGUID(0),

    m_uiArokkoaGUID(0),
    m_uiVoidLordGUID(0),
    m_uiEtheralGUID(0),
    m_uiSwirlingGUID(0),
    m_uiWatcherGUID(0),
    m_uiLavaHoundGUID(0),

    m_uiCellErekemGuard_LGUID(0),
    m_uiCellErekemGuard_RGUID(0),
    m_uiIntroCrystalGUID(0),

    m_uiWorldState(0),
    m_uiWorldStateSealCount(100),
    m_uiWorldStatePortalCount(0),

    m_uiPortalId(0),
    m_uiPortalTimer(0),
    m_uiMaxCountPortalLoc(0)
{
    Initialize();
}

void instance_violet_hold::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    m_uiMaxCountPortalLoc = (sizeof(afPortalLocation)/sizeof(sPortalData)) - 1;
}

void instance_violet_hold::ResetVariables()
{
    m_uiWorldStateSealCount = 100;
    m_uiWorldStatePortalCount = 0;
}

void instance_violet_hold::ResetAll()
{
    ResetVariables();
    UpdateWorldState(false);
    CallGuards(true);
    SetIntroPortals(false);

    for (std::vector<sBossSpawn*>::const_iterator itr = m_vRandomBosses.begin(); itr != m_vRandomBosses.end(); itr++)
    {
        const sBossInformation* pData = GetBossInformation((*itr)->uiEntry);
        if (pData && GetData(pData->uiType) == DONE)
        {
            if (Creature* pGhostBoss = instance->GetCreature(GetData64(pData->uiGhostEntry)))
            {
                if (!pGhostBoss->isAlive())
                    pGhostBoss->Respawn();
            }
            else if (Creature* pSummoner = instance->GetCreature(m_uiSinclariAltGUID))
                pSummoner->SummonCreature(pData->uiGhostEntry, (*itr)->fX, (*itr)->fY, (*itr)->fZ, (*itr)->fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS);

            // Close Door if still open
            UpdateCellForBoss(pData->uiEntry, true);
        }
    }
}

void instance_violet_hold::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_SINCLARI: m_uiSinclariGUID = pCreature->GetGUID(); break;
        case NPC_SINCLARI_ALT: m_uiSinclariAltGUID = pCreature->GetGUID(); break;
        case NPC_DOOR_SEAL: m_uiDoorSealGUID = pCreature->GetGUID(); break;

        case NPC_EREKEM:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiErekemGUID = pCreature->GetGUID();
            break;
        case NPC_MORAGG:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiMoraggGUID = pCreature->GetGUID();
            break;
        case NPC_ICHORON:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiIchoronGUID = pCreature->GetGUID();
            break;
        case NPC_XEVOZZ:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiXevozzGUID = pCreature->GetGUID();
            break;
        case NPC_LAVANTHOR:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiLavanthorGUID = pCreature->GetGUID();
            break;
        case NPC_ZURAMAT:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiZuramatGUID = pCreature->GetGUID();
            break;

        case NPC_PORTAL_INTRO:
            m_lIntroPortalList.push_back(pCreature->GetGUID());
            break;
        case NPC_HOLD_GUARD:
            m_lGuardsList.push_back(pCreature->GetGUID());
            break;

        case NPC_ARAKKOA:    m_uiArokkoaGUID    = pCreature->GetGUID(); break;
        case NPC_VOID_LORD:  m_uiVoidLordGUID   = pCreature->GetGUID(); break;
        case NPC_ETHERAL:    m_uiEtheralGUID    = pCreature->GetGUID(); break;
        case NPC_SWIRLING:   m_uiSwirlingGUID   = pCreature->GetGUID(); break;
        case NPC_WATCHER:    m_uiWatcherGUID    = pCreature->GetGUID(); break;
        case NPC_LAVA_HOUND: m_uiLavaHoundGUID  = pCreature->GetGUID(); break;
    }
}

void instance_violet_hold::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_CELL_LAVANTHOR:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_LAVANTHOR, pGo->GetGUID()));
            break;
        case GO_CELL_MORAGG:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_MORAGG, pGo->GetGUID()));
            break;
        case GO_CELL_ZURAMAT:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_ZURAMAT, pGo->GetGUID()));
            break;
        case GO_CELL_XEVOZZ:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_XEVOZZ, pGo->GetGUID()));
            break;
        case GO_CELL_ICHORON:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_ICHORON, pGo->GetGUID()));
            break;
        case GO_CELL_EREKEM:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_EREKEM, pGo->GetGUID()));
            break;
        case GO_CELL_EREKEM_GUARD_L:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_EREKEM, pGo->GetGUID()));
            break;
        case GO_CELL_EREKEM_GUARD_R:
            m_mBossToCellMap.insert(BossToCellMap::value_type(NPC_EREKEM, pGo->GetGUID()));
            break;

        case GO_INTRO_CRYSTAL:
            m_uiIntroCrystalGUID = pGo->GetGUID();
            break;
        case GO_PRISON_SEAL_DOOR:
            m_uiDoorSealGUID = pGo->GetGUID();
            break;
    }
}

void instance_violet_hold::UpdateCellForBoss(uint32 uiBossEntry, bool bForceClosing /*= false*/)
{
    BossToCellMap::const_iterator itrCellLower = m_mBossToCellMap.lower_bound(uiBossEntry);
    BossToCellMap::const_iterator itrCellUpper = m_mBossToCellMap.upper_bound(uiBossEntry);

    if (itrCellLower == itrCellUpper)
        return;

    for(BossToCellMap::const_iterator itr = itrCellLower; itr != itrCellUpper; ++itr)
        if (!bForceClosing)
            DoUseDoorOrButton(itr->second);
        else
        {
            GameObject* pGo = instance->GetGameObject(itr->second);
            if (pGo && (pGo->GetGoType() == GAMEOBJECT_TYPE_DOOR || pGo->GetGoType() == GAMEOBJECT_TYPE_BUTTON) && pGo->getLootState() == GO_ACTIVATED)
                pGo->ResetDoorOrButton();
        }
}

void instance_violet_hold::UpdateWorldState(bool bEnable)
{
    m_uiWorldState = bEnable ? 1 : 0;

    DoUpdateWorldState(WORLD_STATE_ID, m_uiWorldState);
    DoUpdateWorldState(WORLD_STATE_SEAL, m_uiWorldStateSealCount);
    DoUpdateWorldState(WORLD_STATE_PORTALS, m_uiWorldStatePortalCount);
}

void instance_violet_hold::OnPlayerEnter(Player* pPlayer)
{
    UpdateWorldState(m_auiEncounter[0] == IN_PROGRESS ? true : false);

    if (m_vRandomBosses.empty())
    {
        SetRandomBosses();
        ResetAll();
    }
}

void instance_violet_hold::SetData(uint32 uiType, uint32 uiData)
{
    debug_log("SD2: instance_violet_hold: SetData got type % u, data %u.", uiType, uiData);

    switch(uiType)
    {
        case TYPE_MAIN:
        {
            if (uiData == m_auiEncounter[0])
                return;
            if (m_auiEncounter[0] == DONE)
                return;

            switch(uiData)
            {
                case NOT_STARTED:
                    ResetAll();
                    break;
                case IN_PROGRESS:
                    DoUseDoorOrButton(m_uiDoorSealGUID);
                    UpdateWorldState();
                    m_uiPortalId = urand(0, 2);
                    m_uiPortalTimer = 15000;
                    break;
                case FAIL:
                    if (Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
                        pSinclari->Respawn();
                    break;
                case DONE:
                    UpdateWorldState(false);
                    break;
                case SPECIAL:
                    break;
            }
            m_auiEncounter[0] = uiData;
            break;
        }
        case TYPE_SEAL:
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_PORTAL:
        {
            switch(uiData)
            {
                case SPECIAL:                               // timer to next
                    m_uiPortalTimer = 90000;
                    break;
                case DONE:                                  // portal done, set timer to 5 secs
                    m_uiPortalTimer = 5000;
                    break;
            }
            m_auiEncounter[2] = uiData;
            break;
        }
        case TYPE_LAVANTHOR:
            if (uiData == DONE)
                m_uiPortalTimer = 35000;
            if (m_auiEncounter[3] != DONE)                  // Keep the DONE-information stored
                m_auiEncounter[3] = uiData;
            break;
        case TYPE_MORAGG:
            if (uiData == DONE)
                m_uiPortalTimer = 35000;
            if (m_auiEncounter[4] != DONE)                  // Keep the DONE-information stored
                m_auiEncounter[4] = uiData;
            break;
        case TYPE_EREKEM:
            if (uiData == DONE)
                m_uiPortalTimer = 35000;
            if (m_auiEncounter[5] != DONE)                  // Keep the DONE-information stored
                m_auiEncounter[5] = uiData;
            break;
        case TYPE_ICHORON:
            if (uiData == DONE)
                m_uiPortalTimer = 35000;
            if (m_auiEncounter[6] != DONE)                  // Keep the DONE-information stored
                m_auiEncounter[6] = uiData;
            break;
        case TYPE_XEVOZZ:
            if (uiData == DONE)
                m_uiPortalTimer = 35000;
            if (m_auiEncounter[7] != DONE)                  // Keep the DONE-information stored
                m_auiEncounter[7] = uiData;
            break;
        case TYPE_ZURAMAT:
            if (uiData == DONE)
                m_uiPortalTimer = 35000;
            if (m_auiEncounter[8] != DONE)                  // Keep the DONE-information stored
                m_auiEncounter[8] = uiData;
            break;
        case TYPE_CYANIGOSA:
            if (uiData == DONE)
                m_auiEncounter[9] = uiData;
            if (uiData == DONE)
                SetData(TYPE_MAIN, DONE);
            break;
        default:
            return;
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

uint32 instance_violet_hold::GetData(uint32 uiType)
{
    switch (uiType)
    {
        case TYPE_MAIN:         return m_auiEncounter[0];
        case TYPE_LAVANTHOR:    return m_auiEncounter[3];
        case TYPE_MORAGG:       return m_auiEncounter[4];
        case TYPE_EREKEM:       return m_auiEncounter[5];
        case TYPE_ICHORON:      return m_auiEncounter[6];
        case TYPE_XEVOZZ:       return m_auiEncounter[7];
        case TYPE_ZURAMAT:      return m_auiEncounter[8];
        case TYPE_CYANIGOSA:    return m_auiEncounter[9];
        default:
            return 0;
    }
}

void instance_violet_hold::Load(const char* chrIn)
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
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_violet_hold::SetIntroPortals(bool bDeactivate)
{
    for(std::list<uint64>::iterator i = m_lIntroPortalList.begin(); i != m_lIntroPortalList.end(); ++i)
    {
        if (Creature* pPortal = instance->GetCreature(*i))
        {
            if (bDeactivate)
                pPortal->ForcedDespawn();
            else
                pPortal->Respawn();
        }
    }
}

void instance_violet_hold::SpawnPortal()
{
    if (const sPortalData* pData = GetPortalData())
    {
        if (Creature* pController = instance->GetCreature(m_uiSinclariAltGUID))
        {
            uint32 uiPortalEntry;

            switch(pData->pPortalType)
            {
                case PORTAL_TYPE_NORM: uiPortalEntry = NPC_PORTAL; break;
                case PORTAL_TYPE_SQUAD:
                case PORTAL_TYPE_BOSS: uiPortalEntry = NPC_PORTAL_ELITE; break;
            }

            pController->SummonCreature(uiPortalEntry, pData->fX, pData->fY, pData->fZ, pData->fOrient, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 1800*IN_MILLISECONDS);
        }
    }
}

void instance_violet_hold::SetPortalId()
{
    if (IsCurrentPortalForTrash())
    {
        int iTemp = rand()%(m_uiMaxCountPortalLoc - 1);

        if (iTemp >= m_uiPortalId)
            ++iTemp;

        debug_log("SD2: instance_violet_hold: SetPortalId %i, old was id %u.", iTemp, m_uiPortalId);

        m_uiPortalId = iTemp;
    }
    else if (GetCurrentPortalNumber() == 18)
    {
        debug_log("SD2: instance_violet_hold: SetPortalId %u (Cyanigosa), old was id %u.", 0, m_uiPortalId);
        m_uiPortalId = 0;
    }
    else
    {
        debug_log("SD2: instance_violet_hold: SetPortalId %u (is boss), old was id %u.", m_uiMaxCountPortalLoc, m_uiPortalId);
        m_uiPortalId = m_uiMaxCountPortalLoc;
    }
}

sBossSpawn* instance_violet_hold::CreateBossSpawnByEntry(uint32 uiEntry)
{
    sBossSpawn* pBossSpawn = new sBossSpawn;
    pBossSpawn->uiEntry = uiEntry;

    if (Creature* pBoss = instance->GetCreature(GetData64(uiEntry)))
        pBoss->GetRespawnCoord(pBossSpawn->fX, pBossSpawn->fY, pBossSpawn->fZ, &(pBossSpawn->fO));

    return pBossSpawn;
}

void instance_violet_hold::SetRandomBosses()
{
    if (m_vRandomBosses.empty())
        for (uint8 i = 0; i < MAX_MINIBOSSES; ++i)
        {
            if (GetData(aBossInformation[i].uiType) == DONE)
                m_vRandomBosses.push_back(CreateBossSpawnByEntry(aBossInformation[i].uiEntry));
        }

    if (m_vRandomBosses.size() >= 2)
        return;

    while (m_lRandomBossList.size() > 2)
    {
        uint32 uiPosition = urand(0, m_lRandomBossList.size() - 1);

        for(std::list<uint32>::iterator itr = m_lRandomBossList.begin(); itr != m_lRandomBossList.end(); ++itr, --uiPosition)
        {
            if (!*itr)
                continue;

            if (!uiPosition)
            {
                m_lRandomBossList.erase(itr);
                break;
            }
        }
    }

    if (!m_vRandomBosses.empty())
    {
        for (std::list<uint32>::const_iterator itr = m_lRandomBossList.begin(); itr != m_lRandomBossList.end(); ++itr)
        {
            if (m_vRandomBosses.at(0)->uiEntry != *itr)
                m_vRandomBosses.push_back(CreateBossSpawnByEntry(*itr));
        }
    }
    else
    {
        for (std::list<uint32>::const_iterator itr = m_lRandomBossList.begin(); itr != m_lRandomBossList.end(); ++itr)
            m_vRandomBosses.push_back(CreateBossSpawnByEntry(*itr));
    }

    for (std::vector<sBossSpawn*>::const_iterator itr = m_vRandomBosses.begin(); itr != m_vRandomBosses.end(); ++itr)
        debug_log("SD2: instance_violet_hold first random boss is entry %u", (*itr)->uiEntry);
}

void instance_violet_hold::CallGuards(bool bRespawn)
{
    for(std::list<uint64>::iterator i = m_lGuardsList.begin(); i != m_lGuardsList.end(); ++i)
    {
        if (Creature* pGuard = instance->GetCreature(*i))
        {
            if (bRespawn)
            {
                pGuard->Respawn();
            }
            else if (pGuard->isAlive())
            {
                pGuard->AI()->EnterEvadeMode();

                if (Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
                    pGuard->GetMotionMaster()->MoveFollow(pSinclari, 0.0f, 0.0f);

                pGuard->ForcedDespawn(20000);
            }
        }
    }
}

void instance_violet_hold::ProcessActivationCrystal(Unit* pUser, bool bIsIntro)
{
    if (Creature* pSummon = pUser->SummonCreature(NPC_DEFENSE_SYSTEM, fDefenseSystemLoc[0], fDefenseSystemLoc[1], fDefenseSystemLoc[2], fDefenseSystemLoc[3], TEMPSUMMON_TIMED_DESPAWN, 10000))
    {
        pSummon->CastSpell(pSummon, SPELL_DEFENSE_SYSTEM_VISUAL, true);

        // TODO: figure out how the rest work
        // NPC's NPC_DEFENSE_DUMMY_TARGET are probably channeling some spell to the defense system
    }

    if (bIsIntro)
        DoUseDoorOrButton(m_uiIntroCrystalGUID);

    // else, kill (and despawn?) certain trash mobs. Also boss affected, but not killed.
}

uint32 instance_violet_hold::GetRandomPortalEliteEntry()
{
    return (urand(0, 1) ? NPC_PORTAL_GUARDIAN : NPC_PORTAL_KEEPER);
}

uint32 instance_violet_hold::GetRandomMobForNormalPortal()
{
    switch(urand(1, 4))
    {
        case 1: return NPC_AZURE_INVADER;
        case 2: return NPC_MAGE_HUNTER;
        case 3: return NPC_AZURE_SPELLBREAKER;
        case 4: return NPC_AZURE_BINDER;
        default:
            return 0;
    }
}

uint64 instance_violet_hold::GetData64(uint32 uiData)
{
    switch (uiData)
    {
        case NPC_EREKEM:     return m_uiErekemGUID;
        case NPC_MORAGG:     return m_uiMoraggGUID;
        case NPC_ICHORON:    return m_uiIchoronGUID;
        case NPC_XEVOZZ:     return m_uiXevozzGUID;
        case NPC_LAVANTHOR:  return m_uiLavanthorGUID;
        case NPC_ZURAMAT:    return m_uiZuramatGUID;

        case NPC_ARAKKOA:    return m_uiArokkoaGUID;
        case NPC_VOID_LORD:  return m_uiVoidLordGUID;
        case NPC_ETHERAL:    return m_uiEtheralGUID;
        case NPC_SWIRLING:   return m_uiSwirlingGUID;
        case NPC_WATCHER:    return m_uiWatcherGUID;
        case NPC_LAVA_HOUND: return m_uiLavaHoundGUID;
        default:
            return 0;
    }
}

void instance_violet_hold::OnCreatureEnterCombat(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ZURAMAT:
        case NPC_VOID_LORD:
            SetData(TYPE_ZURAMAT, IN_PROGRESS);
            break;
        case NPC_XEVOZZ:
        case NPC_ETHERAL:
            SetData(TYPE_XEVOZZ, IN_PROGRESS);
            break;
        case NPC_LAVANTHOR:
        case NPC_LAVA_HOUND:
            SetData(TYPE_LAVANTHOR, IN_PROGRESS);
            break;
        case NPC_MORAGG:
        case NPC_WATCHER:
            SetData(TYPE_MORAGG, IN_PROGRESS);
            break;
        case NPC_EREKEM:
        case NPC_ARAKKOA:
            SetData(TYPE_EREKEM, IN_PROGRESS);
            break;
        case NPC_ICHORON:
        case NPC_SWIRLING:
            SetData(TYPE_ICHORON, IN_PROGRESS);
            break;
        case NPC_CYANIGOSA:
            SetData(TYPE_CYANIGOSA, IN_PROGRESS);
            break;
    }
}

void instance_violet_hold::OnCreatureEvade(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ZURAMAT:
        case NPC_VOID_LORD:
            SetData(TYPE_ZURAMAT, FAIL);
            break;
        case NPC_XEVOZZ:
        case NPC_ETHERAL:
            SetData(TYPE_XEVOZZ, FAIL);
            break;
        case NPC_LAVANTHOR:
        case NPC_LAVA_HOUND:
            SetData(TYPE_LAVANTHOR, FAIL);
            break;
        case NPC_MORAGG:
        case NPC_WATCHER:
            SetData(TYPE_MORAGG, FAIL);
            break;
        case NPC_EREKEM:
        case NPC_ARAKKOA:
            SetData(TYPE_EREKEM, FAIL);
            break;
        case NPC_ICHORON:
        case NPC_SWIRLING:
            SetData(TYPE_ICHORON, FAIL);
            break;
        case NPC_CYANIGOSA:
            SetData(TYPE_CYANIGOSA, FAIL);
            break;
    }
}

void instance_violet_hold::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ZURAMAT:
        case NPC_VOID_LORD:
            SetData(TYPE_ZURAMAT, DONE);
            break;
        case NPC_XEVOZZ:
        case NPC_ETHERAL:
            SetData(TYPE_XEVOZZ, DONE);
            break;
        case NPC_LAVANTHOR:
        case NPC_LAVA_HOUND:
            SetData(TYPE_LAVANTHOR, DONE);
            break;
        case NPC_MORAGG:
        case NPC_WATCHER:
            SetData(TYPE_MORAGG, DONE);
            break;
        case NPC_EREKEM:
        case NPC_ARAKKOA:
            SetData(TYPE_EREKEM, DONE);
            break;
        case NPC_ICHORON:
        case NPC_SWIRLING:
            SetData(TYPE_ICHORON, DONE);
            break;
        case NPC_CYANIGOSA:
            SetData(TYPE_CYANIGOSA, DONE);
            break;
    }
}

void instance_violet_hold::Update(uint32 uiDiff)
{
    if (m_auiEncounter[0] != IN_PROGRESS)
        return;

    if (m_uiPortalTimer)
    {
        if (m_uiPortalTimer <= uiDiff)
        {
            DoUpdateWorldState(WORLD_STATE_PORTALS, ++m_uiWorldStatePortalCount);

            SetPortalId();
            SpawnPortal();

            m_uiPortalTimer = 0;
        }
        else
            m_uiPortalTimer -= uiDiff;
    }
}

sBossInformation const* instance_violet_hold::GetBossInformation(uint32 uiEntry/* = 0*/)
{
    uint32 mEntry = uiEntry;
    if (!mEntry)
    {
        if (GetCurrentPortalNumber() == 6 && m_vRandomBosses.size() >= 1)
            mEntry = m_vRandomBosses[0]->uiEntry;
        else if (GetCurrentPortalNumber() == 12 && m_vRandomBosses.size() >= 2)
            mEntry = m_vRandomBosses[1]->uiEntry;
    }

    if (!mEntry)
        return NULL;

    for (uint8 i = 0; i < MAX_MINIBOSSES; ++i)
    {
        if (aBossInformation[i].uiEntry == mEntry)
            return &aBossInformation[i];
    }

    return NULL;
}

instance_violet_hold::~instance_violet_hold()
{
    // Need to free std::vector<sBossSpawn*> m_vRandomBosses;
    for (std::vector<sBossSpawn*>::const_iterator itr = m_vRandomBosses.begin(); itr != m_vRandomBosses.end(); ++itr)
    {
        if (*itr)
            delete (*itr);
    }
}

InstanceData* GetInstanceData_instance_violet_hold(Map* pMap)
{
    return new instance_violet_hold(pMap);
}

void AddSC_instance_violet_hold()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_violet_hold";
    pNewScript->GetInstanceData = GetInstanceData_instance_violet_hold;
    pNewScript->RegisterSelf();
}
