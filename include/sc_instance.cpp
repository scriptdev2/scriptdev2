/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#include "precompiled.h"

/**
   Function that uses a door or a button

   @param   guid The ObjectGuid of the Door/ Button that will be used
   @param   uiWithRestoreTime (in seconds) if == 0 autoCloseTime will be used (if not 0 by default in *_template)
   @param   bUseAlternativeState Use to alternative state
 */
void ScriptedInstance::DoUseDoorOrButton(ObjectGuid guid, uint32 uiWithRestoreTime, bool bUseAlternativeState)
{
    if (!guid)
        return;

    if (GameObject* pGo = instance->GetGameObject(guid))
    {
        if (pGo->GetGoType() == GAMEOBJECT_TYPE_DOOR || pGo->GetGoType() == GAMEOBJECT_TYPE_BUTTON)
        {
            if (pGo->getLootState() == GO_READY)
                pGo->UseDoorOrButton(uiWithRestoreTime, bUseAlternativeState);
            else if (pGo->getLootState() == GO_ACTIVATED)
                pGo->ResetDoorOrButton();
        }
        else
            error_log("SD2: Script call DoUseDoorOrButton, but gameobject entry %u is type %u.", pGo->GetEntry(), pGo->GetGoType());
    }
}

/// Function that uses a door or button that is stored in m_mGoEntryGuidStore
void ScriptedInstance::DoUseDoorOrButton(uint32 uiEntry, uint32 uiWithRestoreTime /*= 0*/, bool bUseAlternativeState /*= false*/)
{
    EntryGuidMap::iterator find = m_mGoEntryGuidStore.find(uiEntry);
    if (find != m_mGoEntryGuidStore.end())
        DoUseDoorOrButton(find->second, uiWithRestoreTime, bUseAlternativeState);
    else
        // Output log, possible reason is not added GO to storage, or not yet loaded
        debug_log("SD2: Script call DoUseDoorOrButton(by Entry), but no gameobject of entry %u was created yet, or it was not stored by script for map %u.", uiEntry, instance->GetId());
}

/**
   Function that respawns a despawned GameObject with given time

   @param   guid The ObjectGuid of the GO that will be respawned
   @param   uiTimeToDespawn (in seconds) Despawn the GO after this time, default is a minute
 */
void ScriptedInstance::DoRespawnGameObject(ObjectGuid guid, uint32 uiTimeToDespawn)
{
    if (!guid)
        return;

    if (GameObject* pGo = instance->GetGameObject(guid))
    {
        //not expect any of these should ever be handled
        if (pGo->GetGoType() == GAMEOBJECT_TYPE_FISHINGNODE || pGo->GetGoType() == GAMEOBJECT_TYPE_DOOR ||
            pGo->GetGoType() == GAMEOBJECT_TYPE_BUTTON || pGo->GetGoType() == GAMEOBJECT_TYPE_TRAP)
            return;

        if (pGo->isSpawned())
            return;

        pGo->SetRespawnTime(uiTimeToDespawn);
        pGo->Refresh();
    }
}

/// Function that respawns a despawned GO that is stored in m_mGoEntryGuidStore
void ScriptedInstance::DoRespawnGameObject(uint32 uiEntry, uint32 uiTimeToDespawn)
{
    EntryGuidMap::iterator find = m_mGoEntryGuidStore.find(uiEntry);
    if (find != m_mGoEntryGuidStore.end())
        DoRespawnGameObject(find->second, uiTimeToDespawn);
    else
        // Output log, possible reason is not added GO to storage, or not yet loaded;
        debug_log("SD2: Script call DoRespawnGameObject(by Entry), but no gameobject of entry %u was created yet, or it was not stored by script for map %u.", uiEntry, instance->GetId());
}

/**
   Helper function to update a world state for all players in the map

   @param   uiStateId The WorldState that will be set for all players in the map
   @param   uiStateData The Value to which the State will be set to
 */
void ScriptedInstance::DoUpdateWorldState(uint32 uiStateId, uint32 uiStateData)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    if (!lPlayers.isEmpty())
    {
        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                pPlayer->SendUpdateWorldState(uiStateId, uiStateData);
        }
    }
    else
        debug_log("SD2: DoUpdateWorldState attempt send data but no players in map.");
}

/// Get the first found Player* (with requested properties) in the map. Can return NULL.
Player* ScriptedInstance::GetPlayerInMap(bool bOnlyAlive /*=false*/, bool bCanBeGamemaster /*=true*/)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
    {
        Player* pPlayer = itr->getSource();
        if (pPlayer && (!bOnlyAlive || pPlayer->isAlive()) && (bCanBeGamemaster || !pPlayer->isGameMaster()))
            return pPlayer;
    }

    return NULL;
}

/// Returns a pointer to a loaded GameObject that was stored in m_mGoEntryGuidStore. Can return NULL
GameObject* ScriptedInstance::GetSingleGameObjectFromStorage(uint32 uiEntry)
{
    EntryGuidMap::iterator find = m_mGoEntryGuidStore.find(uiEntry);
    if (find != m_mGoEntryGuidStore.end())
        return instance->GetGameObject(find->second);

    // Output log, possible reason is not added GO to map, or not yet loaded;
    debug_log("SD2: Script requested gameobject with entry %u, but no gameobject of this entry was created yet, or it was not stored by script for map %u.", uiEntry, instance->GetId());

    return NULL;
}

/// Returns a pointer to a loaded Creature that was stored in m_mGoEntryGuidStore. Can return NULL
Creature* ScriptedInstance::GetSingleCreatureFromStorage(uint32 uiEntry, bool bSkipDebugLog /*=false*/)
{
    EntryGuidMap::iterator find = m_mNpcEntryGuidStore.find(uiEntry);
    if (find != m_mNpcEntryGuidStore.end())
        return instance->GetCreature(find->second);

    // Output log, possible reason is not added GO to map, or not yet loaded;
    if (!bSkipDebugLog)
        debug_log("SD2: Script requested creature with entry %u, but no npc of this entry was created yet, or it was not stored by script for map %u.", uiEntry, instance->GetId());

    return NULL;
}

/**
   Helper function to start a timed achievement criteria for players in the map

   @param   criteriaType The Type that is required to complete the criteria, see enum AchievementCriteriaTypes in MaNGOS
   @param   uiTimedCriteriaMiscId The ID that identifies how the criteria is started
 */
void ScriptedInstance::DoStartTimedAchievement(AchievementCriteriaTypes criteriaType, uint32 uiTimedCriteriaMiscId)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    if (!lPlayers.isEmpty())
    {
        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                pPlayer->StartTimedAchievementCriteria(criteriaType, uiTimedCriteriaMiscId);
        }
    }
    else
        debug_log("SD2: DoStartTimedAchievement attempt start achievements but no players in map.");
}
