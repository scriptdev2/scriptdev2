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
SDName: instance_gundrak
SD%Complete: 80
SDComment: Reload case for bridge support is missing, achievement support is missing
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

bool GOUse_go_gundrak_altar(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    switch(pGo->GetEntry())
    {
        case GO_ALTAR_OF_SLADRAN:  pInstance->SetData(TYPE_SLADRAN, SPECIAL);  break;
        case GO_ALTAR_OF_MOORABI:  pInstance->SetData(TYPE_MOORABI, SPECIAL);  break;
        case GO_ALTAR_OF_COLOSSUS: pInstance->SetData(TYPE_COLOSSUS, SPECIAL); break;
    }

    pGo->UseDoorOrButton(0, true);
    return true;
}

instance_gundrak::instance_gundrak(Map* pMap) : ScriptedInstance(pMap),
    m_uiEckDoorGUID(0),
    m_uiEckUnderwaterDoorGUID(0),
    m_uiGaldarahDoorGUID(0),
    m_uiExitDoorLeftGUID(0),
    m_uiExitDoorRightGUID(0),
    m_uiSnakeKeyGUID(0),
    m_uiMammothKeyGUID(0),
    m_uiTrollKeyGUID(0),
    m_uiRhinoKeyGUID(0),
    m_uiAltarOfSladranGUID(0),
    m_uiAltarOfMoorabiGUID(0),
    m_uiAltarOfColossusGUID(0),
    m_uiBridgeGUID(0),
    m_uiCollisionGUID(0),

    m_uiSladranGUID(0),
    m_uiElementalGUID(0),
    m_uiColossusGUID(0)
{
    Initialize();
}

void instance_gundrak::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_gundrak::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_SLADRAN:   m_uiSladranGUID   = pCreature->GetGUID(); break;
        case NPC_ELEMENTAL: m_uiElementalGUID = pCreature->GetGUID(); break;
        case NPC_COLOSSUS:  m_uiColossusGUID  = pCreature->GetGUID(); break;

        case NPC_INVISIBLE_STALKER: m_luiStalkerGUIDs.push_back(pCreature->GetGUID()); break;
    }
}

/* TODO: Reload case need some love!
*  Problem is to get the bridge/ collision work correct in relaod case.
*  To provide correct functionality(expecting testers to activate all altars in reload case), the Keys aren't loaded, too
*  TODO: When fixed, also remove the SPECIAL->DONE data translation in Load().
*
*  For the Keys should be used something like this, and for bridge and collision similar
*
*   if (m_auiEncounter[0] == SPECIAL && m_auiEncounter[1] == SPECIAL && m_auiEncounter[2] == SPECIAL)
*       pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
*   else
*       pGo->SetGoState(GO_STATE_READY);
*/

void instance_gundrak::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_ECK_DOOR:
            m_uiEckDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_MOORABI] == DONE && !instance->IsRegularDifficulty())
                DoUseDoorOrButton(m_uiEckDoorGUID);
            break;
        case GO_ECK_UNDERWATER_DOOR:
            m_uiEckUnderwaterDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_ECK] == DONE)
                DoUseDoorOrButton(m_uiEckUnderwaterDoorGUID);
            break;
        case GO_GALDARAH_DOOR:
            m_uiGaldarahDoorGUID = pGo->GetGUID();
            DoUseDoorOrButton(m_uiGaldarahDoorGUID);
            break;
        case GO_EXIT_DOOR_L:
            m_uiExitDoorLeftGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_GALDARAH] == DONE)
                DoUseDoorOrButton(m_uiExitDoorLeftGUID);
            break;
        case GO_EXIT_DOOR_R:
            m_uiExitDoorRightGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_GALDARAH] == DONE)
                DoUseDoorOrButton(m_uiExitDoorRightGUID);
            break;
        case GO_ALTAR_OF_SLADRAN:
            m_uiAltarOfSladranGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_SLADRAN] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        case GO_ALTAR_OF_MOORABI:
            m_uiAltarOfMoorabiGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_MOORABI] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        case GO_ALTAR_OF_COLOSSUS:
            m_uiAltarOfColossusGUID = pGo->GetGUID();
            if (m_auiEncounter[TYPE_COLOSSUS] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                break;
        case GO_SNAKE_KEY:
            m_uiSnakeKeyGUID = pGo->GetGUID();
            break;
        case GO_TROLL_KEY:
            m_uiTrollKeyGUID = pGo->GetGUID();
            break;
        case GO_MAMMOTH_KEY:
            m_uiMammothKeyGUID = pGo->GetGUID();
            break;
        case GO_RHINO_KEY:
            m_uiRhinoKeyGUID = pGo->GetGUID();
            break;
        case GO_BRIDGE:
            m_uiBridgeGUID = pGo->GetGUID();
            break;
        case GO_COLLISION:
            m_uiCollisionGUID = pGo->GetGUID();
            break;
    }
}
void instance_gundrak::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[TYPE_SLADRAN] >> m_auiEncounter[TYPE_MOORABI] >> m_auiEncounter[TYPE_COLOSSUS] >> m_auiEncounter[TYPE_GALDARAH] >> m_auiEncounter[TYPE_ECK];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;

        // TODO: REMOVE when bridge/ collision reloading correctly working
        if (m_auiEncounter[i] == SPECIAL)
            m_auiEncounter[i] = DONE;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_gundrak::SetData(uint32 uiType, uint32 uiData)
{
    debug_log("SD2: Instance Gundrak: SetData received for type %u with data %u", uiType, uiData);

    switch(uiType)
    {
        case TYPE_SLADRAN:
            m_auiEncounter[TYPE_SLADRAN] = uiData;
            if (uiData == DONE)
                if (GameObject* pGo = instance->GetGameObject(m_uiAltarOfSladranGUID))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            if (uiData == SPECIAL)
                m_mAltarInProgress.insert(TypeTimerPair(TYPE_SLADRAN, TIMER_VISUAL_ALTAR));
            break;
        case TYPE_MOORABI:
            m_auiEncounter[TYPE_MOORABI] = uiData;
            if (uiData == DONE)
            {
                if (!instance->IsRegularDifficulty())
                    DoUseDoorOrButton(m_uiEckDoorGUID);
                if (GameObject* pGo = instance->GetGameObject(m_uiAltarOfMoorabiGUID))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            }
            if (uiData == SPECIAL)
                m_mAltarInProgress.insert(TypeTimerPair(TYPE_MOORABI, TIMER_VISUAL_ALTAR));
            break;
        case TYPE_COLOSSUS:
            m_auiEncounter[TYPE_COLOSSUS] = uiData;
            if (uiData == DONE)
                if (GameObject* pGo = instance->GetGameObject(m_uiAltarOfColossusGUID))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            if (uiData == SPECIAL)
                m_mAltarInProgress.insert(TypeTimerPair(TYPE_COLOSSUS, TIMER_VISUAL_ALTAR));
            break;
        case TYPE_GALDARAH:
            m_auiEncounter[TYPE_GALDARAH] = uiData;
            DoUseDoorOrButton(m_uiGaldarahDoorGUID);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiExitDoorLeftGUID);
                DoUseDoorOrButton(m_uiExitDoorRightGUID);
            }
            break;
        case TYPE_ECK:
            m_auiEncounter[TYPE_ECK] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiEckUnderwaterDoorGUID);
            break;
        default:
            error_log("SD2: Instance Gundrak: ERROR SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
            break;
    }

    if (uiData == DONE || uiData == SPECIAL)                // Save activated altars, too
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[TYPE_SLADRAN] << " " << m_auiEncounter[TYPE_MOORABI] << " " << m_auiEncounter[TYPE_COLOSSUS] << " " << m_auiEncounter[TYPE_GALDARAH] << " "
            << m_auiEncounter[TYPE_ECK];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_gundrak::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_SLADRAN:
            return m_auiEncounter[TYPE_SLADRAN];
        case TYPE_MOORABI:
            return m_auiEncounter[TYPE_MOORABI];
        case TYPE_COLOSSUS:
            return m_auiEncounter[TYPE_COLOSSUS];
        case TYPE_GALDARAH:
            return m_auiEncounter[TYPE_GALDARAH];
        case TYPE_ECK:
            return m_auiEncounter[TYPE_ECK];
    }
    return 0;
}

uint64 instance_gundrak::GetData64(uint32 uiType)
{
    switch(uiType)
    {
        case NPC_SLADRAN:
            return m_uiSladranGUID;
        case NPC_ELEMENTAL:
            return m_uiElementalGUID;
        case NPC_COLOSSUS:
            return m_uiColossusGUID;
    }
    return 0;
}

static bool sortFromEastToWest(Creature* pFirst, Creature* pSecond)
{
    return pFirst && pSecond && pFirst->GetPositionY() < pSecond->GetPositionY();
}

void instance_gundrak::DoAltarVisualEffect(uint8 uiType)
{
    // Sort the lists if not yet done
    if (!m_luiStalkerGUIDs.empty())
    {
        float fHeight = 10.0f;                              // A bit higher than the altar is needed
        if (GameObject* pCollusAltar = instance->GetGameObject(m_uiAltarOfColossusGUID))
            fHeight += pCollusAltar->GetPositionZ();

        std::list<Creature*> lStalkerTargets, lStalkerCasters;
        for (std::list<uint64>::const_iterator itr = m_luiStalkerGUIDs.begin(); itr != m_luiStalkerGUIDs.end(); ++itr)
        {
            if (Creature* pStalker = instance->GetCreature(*itr))
            {
                if (pStalker->GetPositionZ() > fHeight)
                    lStalkerTargets.push_back(pStalker);
                else
                    lStalkerCasters.push_back(pStalker);
            }
        }
        m_luiStalkerGUIDs.clear();

        lStalkerTargets.sort(sortFromEastToWest);
        lStalkerCasters.sort(sortFromEastToWest);

        for (std::list<Creature*>::const_iterator itr = lStalkerTargets.begin(); itr != lStalkerTargets.end(); ++itr)
            m_luiStalkerTargetGUIDs.push_back((*itr)->GetGUID());
        for (std::list<Creature*>::const_iterator itr = lStalkerCasters.begin(); itr != lStalkerCasters.end(); ++itr)
            m_luiStalkerCasterGUIDs.push_back((*itr)->GetGUID());
    }

    // Verify that the DB has enough trigger spawned
    if (m_luiStalkerTargetGUIDs.size() < 3 || m_luiStalkerCasterGUIDs.size() < 3)
        return;

    // Get the Index from the bosses
    uint8 uiIndex = 0;
    switch (uiType)
    {
        case TYPE_SLADRAN:  uiIndex = 0; break;
        case TYPE_COLOSSUS: uiIndex = 1; break;
        case TYPE_MOORABI:  uiIndex = 2; break;
        default:
            return;
    }

    std::list<uint64>::iterator targetItr = m_luiStalkerTargetGUIDs.begin();
    std::list<uint64>::iterator casterItr = m_luiStalkerCasterGUIDs.begin();

    advance(targetItr, uiIndex);
    advance(casterItr, uiIndex);

    Creature* pTarget = instance->GetCreature(*targetItr);
    Creature* pCaster = instance->GetCreature(*casterItr);

    if (!pTarget || !pCaster)
        return;

    uint32 auiFireBeamSpells[3] = {SPELL_BEAM_SNAKE, SPELL_BEAM_ELEMENTAL, SPELL_BEAM_MAMMOTH};

    // Cast from Caster to Target
    pCaster->CastSpell(pTarget, auiFireBeamSpells[uiIndex], true);
}

void instance_gundrak::Update(uint32 uiDiff)
{
    // Possible multible altars used at the same time, process their timers
    if (!m_mAltarInProgress.empty())
    {
        for (TypeTimerMap::iterator itr = m_mAltarInProgress.begin(); itr != m_mAltarInProgress.end();)
        {
            if (itr->second < uiDiff)
            {
                // Do Visual Effect
                DoAltarVisualEffect(itr->first);
                // Set Timer for Beam-Duration
                m_mBeamInProgress.insert(TypeTimerPair(itr->first, TIMER_VISUAL_BEAM));
                // Remove this timer, as processed
                m_mAltarInProgress.erase(itr++);
            }
            else
            {
                itr->second -= uiDiff;
                ++itr;
            }
        }
    }

    // Possible multible beams used at the same time, process their timers
    if (!m_mBeamInProgress.empty())
    {
        for (TypeTimerMap::iterator itr = m_mBeamInProgress.begin(); itr != m_mBeamInProgress.end();)
        {
            if (itr->second < uiDiff)
            {
                // Use Key
                switch (itr->first)
                {
                    case TYPE_SLADRAN: DoUseDoorOrButton(m_uiSnakeKeyGUID); break;
                    case TYPE_MOORABI: DoUseDoorOrButton(m_uiMammothKeyGUID); break;
                    case TYPE_COLOSSUS: DoUseDoorOrButton(m_uiTrollKeyGUID); break;
                }
                // Set Timer for Beam-Duration
                m_mKeyInProgress.insert(TypeTimerPair(itr->first, TIMER_VISUAL_KEY));
                m_mBeamInProgress.erase(itr++);
            }
            else
            {
                itr->second -= uiDiff;
                ++itr;
            }
        }
    }

    // Activate Bridge if all Three Encounters are used
    if (!m_mKeyInProgress.empty())
    {
        for (TypeTimerMap::iterator itr = m_mKeyInProgress.begin(); itr != m_mKeyInProgress.end();)
        {
            if (itr->second < uiDiff)
            {
                // Activate Bridge (and all other Keys) if we are on the last Key, and all other keys are already set
                if (m_auiEncounter[0] == SPECIAL && m_auiEncounter[1] == SPECIAL && m_auiEncounter[2] == SPECIAL
                    && m_mAltarInProgress.empty() && m_mBeamInProgress.empty() && m_mKeyInProgress.size() == 1)
                {
                    DoUseDoorOrButton(m_uiCollisionGUID);
                    DoUseDoorOrButton(m_uiRhinoKeyGUID, 0, true);

                    // The already closed keys cannot be done with DoUseDoorOrButton
                    if (GameObject* pTrollKey = instance->GetGameObject(m_uiTrollKeyGUID))
                        pTrollKey->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                    if (GameObject* pMammothKey = instance->GetGameObject(m_uiMammothKeyGUID))
                        pMammothKey->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                    if (GameObject* pSnakeKey = instance->GetGameObject(m_uiSnakeKeyGUID))
                        pSnakeKey->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);

                    // GO_BRIDGE is type 35 (TRAP_DOOR) and needs to be handled directly
                    // Real Use of this GO is unknown, but this change of state is expected
                    if (GameObject* pBridge = instance->GetGameObject(m_uiBridgeGUID))
                        pBridge->SetGoState(GO_STATE_READY);
                }
                // Remove this timer, as processed
                m_mKeyInProgress.erase(itr++);
            }
            else
            {
                itr->second -= uiDiff;
                ++itr;
            }
        }
    }
}

InstanceData* GetInstanceData_instance_gundrak(Map* pMap)
{
    return new instance_gundrak(pMap);
}

void AddSC_instance_gundrak()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "go_gundrak_altar";
    pNewScript->pGOUse = &GOUse_go_gundrak_altar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "instance_gundrak";
    pNewScript->GetInstanceData = &GetInstanceData_instance_gundrak;
    pNewScript->RegisterSelf();
}
