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
SDName: Instance_Naxxramas
SD%Complete: 90%
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

instance_naxxramas::instance_naxxramas(Map* pMap) : ScriptedInstance(pMap),
    m_fChamberCenterX(0.0f),
    m_fChamberCenterY(0.0f),
    m_fChamberCenterZ(0.0f)
{
    Initialize();
}

void instance_naxxramas::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

    for (uint8 i = 0; i < MAX_SPECIAL_ACHIEV_CRITS; ++i)
        m_abAchievCriteria[i] = false;
}

void instance_naxxramas::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_ANUB_REKHAN:
        case NPC_FAERLINA:
        case NPC_THADDIUS:
        case NPC_STALAGG:
        case NPC_FEUGEN:
        case NPC_ZELIEK:
        case NPC_THANE:
        case NPC_BLAUMEUX:
        case NPC_RIVENDARE:
        case NPC_GOTHIK:
        case NPC_KELTHUZAD:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;

        case NPC_SUB_BOSS_TRIGGER:  m_lGothTriggerList.push_back(pCreature->GetObjectGuid()); break;
        case NPC_TESLA_COIL:        m_lThadTeslaCoilList.push_back(pCreature->GetObjectGuid()); break;

        case NPC_NAXXRAMAS_FOLLOWER:
        case NPC_NAXXRAMAS_WORSHIPPER:
            m_lFaerlinaAddGUIDs.push_back(pCreature->GetObjectGuid());
            break;
    }
}

void instance_naxxramas::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        // Arachnid Quarter
        case GO_ARAC_ANUB_DOOR:
            break;
        case GO_ARAC_ANUB_GATE:
            if (m_auiEncounter[TYPE_ANUB_REKHAN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARAC_FAER_WEB:
            break;
        case GO_ARAC_FAER_DOOR:
            if (m_auiEncounter[TYPE_FAERLINA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARAC_MAEX_INNER_DOOR:
            break;
        case GO_ARAC_MAEX_OUTER_DOOR:
            if (m_auiEncounter[TYPE_FAERLINA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

        // Plague Quarter
        case GO_PLAG_NOTH_ENTRY_DOOR:
            break;
        case GO_PLAG_NOTH_EXIT_DOOR:
            if (m_auiEncounter[TYPE_NOTH] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PLAG_HEIG_ENTRY_DOOR:
            if (m_auiEncounter[TYPE_NOTH] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PLAG_HEIG_EXIT_DOOR:
            if (m_auiEncounter[TYPE_HEIGAN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PLAG_LOAT_DOOR:
            break;

        // Military Quarter
        case GO_MILI_GOTH_ENTRY_GATE:
            break;
        case GO_MILI_GOTH_EXIT_GATE:
            if (m_auiEncounter[TYPE_GOTHIK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_MILI_GOTH_COMBAT_GATE:
            break;
        case GO_MILI_HORSEMEN_DOOR:
            if (m_auiEncounter[TYPE_GOTHIK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CHEST_HORSEMEN_NORM:
        case GO_CHEST_HORSEMEN_HERO:
            break;

        // Construct Quarter
        case GO_CONS_PATH_EXIT_DOOR:
            if (m_auiEncounter[TYPE_PATCHWERK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CONS_GLUT_EXIT_DOOR:
            if (m_auiEncounter[TYPE_GLUTH] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CONS_THAD_DOOR:
            if (m_auiEncounter[TYPE_GLUTH] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CONS_NOX_TESLA_FEUGEN:
            if (m_auiEncounter[TYPE_THADDIUS] == DONE)
                pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_CONS_NOX_TESLA_STALAGG:
            if (m_auiEncounter[TYPE_THADDIUS] == DONE)
                pGo->SetGoState(GO_STATE_READY);
            break;

        // Frostwyrm Lair
        case GO_KELTHUZAD_WATERFALL_DOOR:
            if (m_auiEncounter[TYPE_SAPPHIRON] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_KELTHUZAD_EXIT_DOOR:
            break;

        // Eyes
        case GO_ARAC_EYE_RAMP:
            if (m_auiEncounter[TYPE_MAEXXNA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PLAG_EYE_RAMP:
            if (m_auiEncounter[TYPE_LOATHEB] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_MILI_EYE_RAMP:
            if (m_auiEncounter[TYPE_FOUR_HORSEMEN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CONS_EYE_RAMP:
            if (m_auiEncounter[TYPE_THADDIUS] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

        // Portals
        case GO_ARAC_PORTAL:
        case GO_PLAG_PORTAL:
        case GO_MILI_PORTAL:
        case GO_CONS_PORTAL:
            break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_naxxramas::OnPlayerDeath(Player* pPlayer)
{
    if (IsEncounterInProgress())
        SetData(TYPE_UNDYING_FAILED, DONE);

    if (GetData(TYPE_HEIGAN) == IN_PROGRESS)
        SetSpecialAchievementCriteria(TYPE_ACHIEV_SAFETY_DANCE, false);
}

void instance_naxxramas::OnCreatureDeath(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_MR_BIGGLESWORTH && m_auiEncounter[TYPE_KELTHUZAD] != DONE)
        DoOrSimulateScriptTextForThisInstance(SAY_KELTHUZAD_CAT_DIED, NPC_KELTHUZAD);
}

bool instance_naxxramas::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < TYPE_KELTHUZAD; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS || m_auiEncounter[i] == SPECIAL)
            return true;

    return false;
}

void instance_naxxramas::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ANUB_REKHAN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_ARAC_ANUB_DOOR);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ARAC_ANUB_GATE);
                DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_START_MAEXXNA_ID);
            }
            break;
        case TYPE_FAERLINA:
            DoUseDoorOrButton(GO_ARAC_FAER_WEB);
            if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEV_KNOCK_YOU_OUT, true);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ARAC_FAER_DOOR);
                DoUseDoorOrButton(GO_ARAC_MAEX_OUTER_DOOR);
            }
            if (uiData == FAIL)
            {
                for (GUIDList::const_iterator itr = m_lFaerlinaAddGUIDs.begin(); itr != m_lFaerlinaAddGUIDs.end(); ++itr)
                {
                    Creature* pAdd = instance->GetCreature(*itr);
                    if (pAdd && !pAdd->isAlive())
                        pAdd->Respawn();
                }
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_MAEXXNA:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_ARAC_MAEX_INNER_DOOR, uiData);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ARAC_EYE_RAMP);
                DoRespawnGameObject(GO_ARAC_PORTAL, 30*MINUTE);
                DoTaunt();
            }
            break;
        case TYPE_NOTH:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_PLAG_NOTH_ENTRY_DOOR);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_PLAG_NOTH_EXIT_DOOR);
                DoUseDoorOrButton(GO_PLAG_HEIG_ENTRY_DOOR);
            }
            break;
        case TYPE_HEIGAN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_PLAG_HEIG_ENTRY_DOOR);
            // uncomment when eruption is implemented
            //if (uiData == IN_PROGRESS)
            //    SetSpecialAchievementCriteria(TYPE_ACHIEV_SAFETY_DANCE, true);
            if (uiData == DONE)
                DoUseDoorOrButton(GO_PLAG_HEIG_EXIT_DOOR);
            break;
        case TYPE_LOATHEB:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_PLAG_LOAT_DOOR);
            if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEV_SPORE_LOSER, true);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_PLAG_EYE_RAMP);
                DoRespawnGameObject(GO_PLAG_PORTAL, 30*MINUTE);
                DoTaunt();
            }
            break;
        case TYPE_RAZUVIOUS:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_GOTHIK:
            switch(uiData)
            {
                case IN_PROGRESS:
                    DoUseDoorOrButton(GO_MILI_GOTH_ENTRY_GATE);
                    DoUseDoorOrButton(GO_MILI_GOTH_COMBAT_GATE);
                    break;
                case SPECIAL:
                    DoUseDoorOrButton(GO_MILI_GOTH_COMBAT_GATE);
                    break;
                case FAIL:
                    if (m_auiEncounter[uiType] == IN_PROGRESS)
                        DoUseDoorOrButton(GO_MILI_GOTH_COMBAT_GATE);

                    DoUseDoorOrButton(GO_MILI_GOTH_ENTRY_GATE);
                    break;
                case DONE:
                    DoUseDoorOrButton(GO_MILI_GOTH_ENTRY_GATE);
                    DoUseDoorOrButton(GO_MILI_GOTH_EXIT_GATE);
                    DoUseDoorOrButton(GO_MILI_HORSEMEN_DOOR);
                    break;
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_FOUR_HORSEMEN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_MILI_HORSEMEN_DOOR);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_MILI_EYE_RAMP);
                DoRespawnGameObject(GO_MILI_PORTAL, 30*MINUTE);
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CHEST_HORSEMEN_NORM : GO_CHEST_HORSEMEN_HERO, 30*MINUTE);
                DoTaunt();
            }
            break;
        case TYPE_PATCHWERK:
            m_auiEncounter[uiType] = uiData;
            if (uiData == IN_PROGRESS)
                DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_START_PATCHWERK_ID);
            if (uiData == DONE)
                DoUseDoorOrButton(GO_CONS_PATH_EXIT_DOOR);
            break;
        case TYPE_GROBBULUS:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_GLUTH:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_CONS_GLUT_EXIT_DOOR);
                DoUseDoorOrButton(GO_CONS_THAD_DOOR);
            }
            break;
        case TYPE_THADDIUS:
            // Only process real changes here
            if (m_auiEncounter[uiType] == uiData)
                return;

            m_auiEncounter[uiType] = uiData;
            if (uiData != SPECIAL)
                DoUseDoorOrButton(GO_CONS_THAD_DOOR, uiData);
            // Uncomment when this achievement is implemented
            //if (uiData == IN_PROGRESS)
            //    SetSpecialAchievementCriteria(TYPE_ACHIEV_SHOCKING, true);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_CONS_EYE_RAMP);
                DoRespawnGameObject(GO_CONS_PORTAL, 30*MINUTE);
                DoTaunt();
            }
            break;
        case TYPE_SAPPHIRON:
            m_auiEncounter[uiType] = uiData;
            // Uncomment when achiev check implemented
            //if (uiData == IN_PROGRESS)
            //    SetSpecialAchievementCriteria(TYPE_ACHIEV_HUNDRED_CLUB, true);
            if (uiData == DONE)
                DoUseDoorOrButton(GO_KELTHUZAD_WATERFALL_DOOR);
            break;
        case TYPE_KELTHUZAD:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_KELTHUZAD_EXIT_DOOR);
            if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEV_GET_ENOUGH, false);
            break;
        case TYPE_UNDYING_FAILED:
            m_auiEncounter[uiType] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
            << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11] << " "
            << m_auiEncounter[12] << " " << m_auiEncounter[13] << " " << m_auiEncounter[14] << " " << m_auiEncounter[15];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_naxxramas::Load(const char* chrIn)
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
        >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11]
        >> m_auiEncounter[12] >> m_auiEncounter[13] >> m_auiEncounter[14] >> m_auiEncounter[15];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_naxxramas::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_naxxramas::SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet)
{
    if (uiType < MAX_SPECIAL_ACHIEV_CRITS)
        m_abAchievCriteria[uiType] = bIsMet;
}

bool instance_naxxramas::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_SAFETY_DANCE_N:
        case ACHIEV_CRIT_SAFETY_DANCE_H:
            return m_abAchievCriteria[TYPE_ACHIEV_SAFETY_DANCE];
        case ACHIEV_CRIT_KNOCK_YOU_OUT_N:
        case ACHIEV_CRIT_KNOCK_YOU_OUT_H:
            return m_abAchievCriteria[TYPE_ACHIEV_KNOCK_YOU_OUT];
        case ACHIEV_CRIT_HUNDRED_CLUB_N:
        case ACHIEV_CRIT_HUNDRED_CLUB_H:
            return m_abAchievCriteria[TYPE_ACHIEV_HUNDRED_CLUB];
        case ACHIEV_CRIT_SHOCKING_N:
        case ACHIEV_CRIT_SHOCKING_H:
            return m_abAchievCriteria[TYPE_ACHIEV_SHOCKING];
        case ACHIEV_CRIT_SPORE_LOSER_N:
        case ACHIEV_CRIT_SPORE_LOSER_H:
            return m_abAchievCriteria[TYPE_ACHIEV_SPORE_LOSER];
        case ACHIEV_CRIT_GET_ENOUGH_N:
        case ACHIEV_CRIT_GET_ENOUGH_H:
            return m_abAchievCriteria[TYPE_ACHIEV_GET_ENOUGH];
        // 'The Immortal'(25m) or 'Undying'(10m) - (achievs 2186, 2187)
        case ACHIEV_CRIT_IMMORTAL_KEL:
        case ACHIEV_CRIT_IMMOORTAL_LOA:
        case ACHIEV_CRIT_IMMOORTAL_THAD:
        case ACHIEV_CRIT_IMMOORTAL_MAEX:
        case ACHIEV_CRIT_IMMOORTAL_HORSE:
        case ACHIEV_CRIT_UNDYING_KEL:
        case ACHIEV_CRIT_UNDYING_HORSE:
        case ACHIEV_CRIT_UNDYING_MAEX:
        case ACHIEV_CRIT_UNDYING_LOA:
        case ACHIEV_CRIT_UNDYING_THAD:
        {
            // First, check if all bosses are killed (except the last encounter)
            uint8 uiEncounterDone = 0;
            for (uint8 i = 0; i < TYPE_KELTHUZAD; ++i)
                if (m_auiEncounter[i] == DONE)
                    ++uiEncounterDone;

            return uiEncounterDone >= 14 && GetData(TYPE_UNDYING_FAILED) != DONE;
        }
        default:
            return false;
    }
}

void instance_naxxramas::SetGothTriggers()
{
    Creature* pGoth = GetSingleCreatureFromStorage(NPC_GOTHIK);

    if (!pGoth)
        return;

    for(GUIDList::const_iterator itr = m_lGothTriggerList.begin(); itr != m_lGothTriggerList.end(); ++itr)
    {
        if (Creature* pTrigger = instance->GetCreature(*itr))
        {
            GothTrigger pGt;
            pGt.bIsAnchorHigh = (pTrigger->GetPositionZ() >= (pGoth->GetPositionZ() - 5.0f));
            pGt.bIsRightSide = IsInRightSideGothArea(pTrigger);

            m_mGothTriggerMap[pTrigger->GetObjectGuid()] = pGt;
        }
    }
}

Creature* instance_naxxramas::GetClosestAnchorForGoth(Creature* pSource, bool bRightSide)
{
    std::list<Creature* > lList;

    for (UNORDERED_MAP<ObjectGuid, GothTrigger>::iterator itr = m_mGothTriggerMap.begin(); itr != m_mGothTriggerMap.end(); ++itr)
    {
        if (!itr->second.bIsAnchorHigh)
            continue;

        if (itr->second.bIsRightSide != bRightSide)
            continue;

        if (Creature* pCreature = instance->GetCreature(itr->first))
            lList.push_back(pCreature);
    }

    if (!lList.empty())
    {
        lList.sort(ObjectDistanceOrder(pSource));
        return lList.front();
    }

    return NULL;
}

void instance_naxxramas::GetGothSummonPointCreatures(std::list<Creature*> &lList, bool bRightSide)
{
    for (UNORDERED_MAP<ObjectGuid, GothTrigger>::iterator itr = m_mGothTriggerMap.begin(); itr != m_mGothTriggerMap.end(); ++itr)
    {
        if (itr->second.bIsAnchorHigh)
            continue;

        if (itr->second.bIsRightSide != bRightSide)
            continue;

        if (Creature* pCreature = instance->GetCreature(itr->first))
            lList.push_back(pCreature);
    }
}

bool instance_naxxramas::IsInRightSideGothArea(Unit* pUnit)
{
    if (GameObject* pCombatGate = GetSingleGameObjectFromStorage(GO_MILI_GOTH_COMBAT_GATE))
        return (pCombatGate->GetPositionY() >= pUnit->GetPositionY());

    error_log("SD2: left/right side check, Gothik combat area failed.");
    return true;
}

void instance_naxxramas::SetChamberCenterCoords(float fX, float fY, float fZ)
{
    m_fChamberCenterX = fX;
    m_fChamberCenterY = fY;
    m_fChamberCenterZ = fZ;
}

void instance_naxxramas::DoTaunt()
{
    if (m_auiEncounter[TYPE_KELTHUZAD] != DONE)
    {
        uint8 uiWingsCleared = 0;

        if (m_auiEncounter[TYPE_MAEXXNA] == DONE)
            ++uiWingsCleared;

        if (m_auiEncounter[TYPE_LOATHEB] == DONE)
            ++uiWingsCleared;

        if (m_auiEncounter[TYPE_FOUR_HORSEMEN] == DONE)
            ++uiWingsCleared;

        if (m_auiEncounter[TYPE_THADDIUS] == DONE)
            ++uiWingsCleared;

        switch(uiWingsCleared)
        {
            case 1: DoOrSimulateScriptTextForThisInstance(SAY_KELTHUZAD_TAUNT1, NPC_KELTHUZAD); break;
            case 2: DoOrSimulateScriptTextForThisInstance(SAY_KELTHUZAD_TAUNT2, NPC_KELTHUZAD); break;
            case 3: DoOrSimulateScriptTextForThisInstance(SAY_KELTHUZAD_TAUNT3, NPC_KELTHUZAD); break;
            case 4: DoOrSimulateScriptTextForThisInstance(SAY_KELTHUZAD_TAUNT4, NPC_KELTHUZAD); break;
        }
    }
}

InstanceData* GetInstanceData_instance_naxxramas(Map* pMap)
{
    return new instance_naxxramas(pMap);
}

bool AreaTrigger_at_naxxramas(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pAt->id == AREATRIGGER_KELTHUZAD)
    {
        if (pPlayer->isGameMaster() || pPlayer->isDead())
            return false;

        instance_naxxramas* pInstance = (instance_naxxramas*)pPlayer->GetInstanceData();

        if (!pInstance)
            return false;

        pInstance->SetChamberCenterCoords(pAt->x, pAt->y, pAt->z);

        if (pInstance->GetData(TYPE_KELTHUZAD) == NOT_STARTED)
        {
            if (Creature* pKelthuzad = pInstance->GetSingleCreatureFromStorage(NPC_KELTHUZAD))
            {
                if (pKelthuzad->isAlive())
                {
                    pInstance->SetData(TYPE_KELTHUZAD, IN_PROGRESS);
                    pKelthuzad->SetInCombatWithZone();
                }
            }
        }
    }

    if (pAt->id == AREATRIGGER_THADDIUS_DOOR)
    {
        if (instance_naxxramas* pInstance = (instance_naxxramas*)pPlayer->GetInstanceData())
        {
            if (pInstance->GetData(TYPE_THADDIUS) == NOT_STARTED)
            {
                if (Creature* pThaddius = pInstance->GetSingleCreatureFromStorage(NPC_THADDIUS))
                {
                    pInstance->SetData(TYPE_THADDIUS, SPECIAL);
                    DoScriptText(SAY_THADDIUS_GREET, pThaddius);
                }
            }
        }
    }

    return false;
}

void AddSC_instance_naxxramas()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_naxxramas";
    pNewScript->GetInstanceData = &GetInstanceData_instance_naxxramas;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_naxxramas";
    pNewScript->pAreaTrigger = &AreaTrigger_at_naxxramas;
    pNewScript->RegisterSelf();
}
