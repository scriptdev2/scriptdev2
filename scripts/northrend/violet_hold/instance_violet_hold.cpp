/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

void instance_violet_hold::UpdateCellForBoss(uint32 uiBossEntry)
{
    BossToCellMap::const_iterator itrCellLower = m_mBossToCellMap.lower_bound(uiBossEntry);
    BossToCellMap::const_iterator itrCellUpper = m_mBossToCellMap.upper_bound(uiBossEntry);

    if (itrCellLower == itrCellUpper)
        return;

    for(BossToCellMap::const_iterator itr = itrCellLower; itr != itrCellUpper; ++itr)
        DoUseDoorOrButton(itr->second);
}

void instance_violet_hold::UpdateWorldState(bool bEnable)
{
    if (bEnable)
        m_uiWorldState = 1;
    else
        m_uiWorldState = 0;

    DoUpdateWorldState(WORLD_STATE_ID, m_uiWorldState);
    DoUpdateWorldState(WORLD_STATE_SEAL, m_uiWorldStateSealCount);
    DoUpdateWorldState(WORLD_STATE_PORTALS, m_uiWorldStatePortalCount);
}

void instance_violet_hold::OnPlayerEnter(Player* pPlayer)
{
    UpdateWorldState(m_auiEncounter[0] == IN_PROGRESS ? true : false);
}

void instance_violet_hold::SetData(uint32 uiType, uint32 uiData)
{
    error_log("SD2: instance_violet_hold: SetData got type % u, data %u.", uiType, uiData);

    switch(uiType)
    {
        case TYPE_MAIN:
        {
            if (uiData == m_auiEncounter[0])
                return;

            switch(uiData)
            {
                case NOT_STARTED:
                    ResetAll();
                    break;
                case IN_PROGRESS:
                    DoUseDoorOrButton(m_uiDoorSealGUID);
                    SetRandomBosses();
                    UpdateWorldState();
                    m_uiPortalId = urand(0, 2);
                    m_uiPortalTimer = 15000;
                    break;
                case FAIL:
                    if (Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
                        pSinclari->Respawn();

                    break;
                case DONE:
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
    }
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

            pController->SummonCreature(uiPortalEntry, pData->fX, pData->fY, pData->fZ, pData->fOrient, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 1800*IN_MILISECONDS);
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

        error_log("SD2: instance_violet_hold: SetPortalId %i, old was id %u.", iTemp, m_uiPortalId);

        m_uiPortalId = iTemp;
    }
    else
    {
        error_log("SD2: instance_violet_hold: SetPortalId %u (is boss), old was id %u.", m_uiMaxCountPortalLoc, m_uiPortalId);
        m_uiPortalId = m_uiMaxCountPortalLoc;
    }
}

void instance_violet_hold::SetRandomBosses()
{
    while (m_lRandomBossList.size() > 2)
    {
        uint32 uiPosition = urand(0, m_lRandomBossList.size()-1);

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

    for(std::list<uint32>::iterator itr = m_lRandomBossList.begin(); itr != m_lRandomBossList.end(); ++itr)
        error_log("SD2: instance_violet_hold random boss is entry %u", *itr);
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
    }

    return 0;
}

uint64 instance_violet_hold::GetData64(uint32 uiData)
{
    return 0;
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

InstanceData* GetInstanceData_instance_violet_hold(Map* pMap)
{
    return new instance_violet_hold(pMap);
}

void AddSC_instance_violet_hold()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_violet_hold";
    newscript->GetInstanceData = GetInstanceData_instance_violet_hold;
    newscript->RegisterSelf();
}
