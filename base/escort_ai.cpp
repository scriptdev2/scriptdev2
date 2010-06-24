/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

/* ScriptData
SDName: EscortAI
SD%Complete: 100
SDComment:
SDCategory: Npc
EndScriptData */

#include "precompiled.h"
#include "escort_ai.h"

const float MAX_PLAYER_DISTANCE = 66.0f;

enum
{
    POINT_LAST_POINT    = 0xFFFFFF,
    POINT_HOME          = 0xFFFFFE
};

npc_escortAI::npc_escortAI(Creature* pCreature) : ScriptedAI(pCreature),
    m_uiPlayerGUID(0),
    m_uiPlayerCheckTimer(1000),
    m_uiWPWaitTimer(2500),
    m_uiEscortState(STATE_ESCORT_NONE),
    m_bIsActiveAttacker(true),
    m_bIsRunning(false),
    m_pQuestForEscort(NULL),
    m_bCanInstantRespawn(false),
    m_bCanReturnToStart(false)
{}

bool npc_escortAI::IsVisible(Unit* pWho) const
{
    if (!pWho)
        return false;

    return m_creature->IsWithinDist(pWho, VISIBLE_RANGE) && pWho->isVisibleForOrDetect(m_creature, m_creature, true);
}

void npc_escortAI::AttackStart(Unit* pWho)
{
    if (!pWho)
        return;

    if (m_creature->Attack(pWho, true))
    {
        m_creature->AddThreat(pWho);
        m_creature->SetInCombatWith(pWho);
        pWho->SetInCombatWith(m_creature);

        if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
            m_creature->GetMotionMaster()->MovementExpired();

        if (IsCombatMovement())
            m_creature->GetMotionMaster()->MoveChase(pWho);
    }
}

void npc_escortAI::EnterCombat(Unit* pEnemy)
{
    if (!pEnemy)
        return;

    Aggro(pEnemy);
}

void npc_escortAI::Aggro(Unit* pEnemy)
{
}

//see followerAI
bool npc_escortAI::AssistPlayerInCombat(Unit* pWho)
{
    if (!pWho->getVictim())
        return false;

    // experimental (unknown) flag not present
    if (!(m_creature->GetCreatureInfo()->type_flags & CREATURE_TYPEFLAGS_UNK13))
        return false;

    // unit state prevents (similar check is done in CanInitiateAttack which also include checking unit_flags. We skip those here)
    if (m_creature->hasUnitState(UNIT_STAT_STUNNED | UNIT_STAT_DIED))
        return false;

    // victim of pWho is not a player
    if (!pWho->getVictim()->GetCharmerOrOwnerPlayerOrPlayerItself())
        return false;

    // never attack friendly
    if (m_creature->IsFriendlyTo(pWho))
        return false;

    // too far away and no free sight?
    if (m_creature->IsWithinDistInMap(pWho, MAX_PLAYER_DISTANCE) && m_creature->IsWithinLOSInMap(pWho))
    {
        // already fighting someone?
        if (!m_creature->getVictim())
        {
            AttackStart(pWho);
            return true;
        }
        else
        {
            pWho->SetInCombatWith(m_creature);
            m_creature->AddThreat(pWho);
            return true;
        }
    }

    return false;
}

void npc_escortAI::MoveInLineOfSight(Unit* pWho)
{
    if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature))
    {
        // AssistPlayerInCombat can start attack, so return if true
        if (HasEscortState(STATE_ESCORT_ESCORTING) && AssistPlayerInCombat(pWho))
            return;

        if (!m_creature->CanInitiateAttack())
            return;

        if (!m_creature->canFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
            return;

        if (m_creature->IsHostileTo(pWho))
        {
            float fAttackRadius = m_creature->GetAttackDistance(pWho);
            if (m_creature->IsWithinDistInMap(pWho, fAttackRadius) && m_creature->IsWithinLOSInMap(pWho))
            {
                if (!m_creature->getVictim())
                {
                    pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                    AttackStart(pWho);
                }
                else if (m_creature->GetMap()->IsDungeon())
                {
                    pWho->SetInCombatWith(m_creature);
                    m_creature->AddThreat(pWho);
                }
            }
        }
    }
}

void npc_escortAI::JustDied(Unit* pKiller)
{
    if (!HasEscortState(STATE_ESCORT_ESCORTING) || !m_uiPlayerGUID || !m_pQuestForEscort)
        return;

    if (Player* pPlayer = GetPlayerForEscort())
    {
        if (Group* pGroup = pPlayer->GetGroup())
        {
            for(GroupReference* pRef = pGroup->GetFirstMember(); pRef != NULL; pRef = pRef->next())
            {
                if (Player* pMember = pRef->getSource())
                {
                    if (pMember->GetQuestStatus(m_pQuestForEscort->GetQuestId()) == QUEST_STATUS_INCOMPLETE)
                        pMember->FailQuest(m_pQuestForEscort->GetQuestId());
                }
            }
        }
        else
        {
            if (pPlayer->GetQuestStatus(m_pQuestForEscort->GetQuestId()) == QUEST_STATUS_INCOMPLETE)
                pPlayer->FailQuest(m_pQuestForEscort->GetQuestId());
        }
    }
}

void npc_escortAI::JustRespawned()
{
    m_uiEscortState = STATE_ESCORT_NONE;

    if (!IsCombatMovement())
        SetCombatMovement(true);

    //add a small delay before going to first waypoint, normal in near all cases
    m_uiWPWaitTimer = 2500;

    if (m_creature->getFaction() != m_creature->GetCreatureInfo()->faction_A)
        m_creature->setFaction(m_creature->GetCreatureInfo()->faction_A);

    Reset();
}

void npc_escortAI::EnterEvadeMode()
{
    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop(true);
    m_creature->SetLootRecipient(NULL);

    if (HasEscortState(STATE_ESCORT_ESCORTING))
    {
        debug_log("SD2: EscortAI has left combat and is now returning to CombatStartPosition.");

        if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
        {
            AddEscortState(STATE_ESCORT_RETURNING);

            float fPosX, fPosY, fPosZ;
            m_creature->GetCombatStartPosition(fPosX, fPosY, fPosZ);
            m_creature->GetMotionMaster()->MovePoint(POINT_LAST_POINT, fPosX, fPosY, fPosZ);
        }
    }
    else
    {
        if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
            m_creature->GetMotionMaster()->MoveTargetedHome();
    }

    Reset();
}

bool npc_escortAI::IsPlayerOrGroupInRange()
{
    if (Player* pPlayer = GetPlayerForEscort())
    {
        if (Group* pGroup = pPlayer->GetGroup())
        {
            for(GroupReference* pRef = pGroup->GetFirstMember(); pRef != NULL; pRef = pRef->next())
            {
                Player* pMember = pRef->getSource();

                if (pMember && m_creature->IsWithinDistInMap(pMember, MAX_PLAYER_DISTANCE))
                {
                    return true;
                    break;
                }
            }
        }
        else
        {
            if (m_creature->IsWithinDistInMap(pPlayer, MAX_PLAYER_DISTANCE))
                return true;
        }
    }
    return false;
}

void npc_escortAI::UpdateAI(const uint32 uiDiff)
{
    //Waypoint Updating
    if (HasEscortState(STATE_ESCORT_ESCORTING) && !m_creature->getVictim() && m_uiWPWaitTimer && !HasEscortState(STATE_ESCORT_RETURNING))
    {
        if (m_uiWPWaitTimer <= uiDiff)
        {
            //End of the line
            if (CurrentWP == WaypointList.end())
            {
                debug_log("SD2: EscortAI reached end of waypoints");

                if (m_bCanReturnToStart)
                {
                    float fRetX, fRetY, fRetZ;
                    m_creature->GetRespawnCoord(fRetX, fRetY, fRetZ);

                    m_creature->GetMotionMaster()->MovePoint(POINT_HOME, fRetX, fRetY, fRetZ);

                    m_uiWPWaitTimer = 0;

                    debug_log("SD2: EscortAI are returning home to spawn location: %u, %f, %f, %f", POINT_HOME, fRetX, fRetY, fRetZ);
                    return;
                }

                if (m_bCanInstantRespawn)
                {
                    m_creature->setDeathState(JUST_DIED);
                    m_creature->Respawn();
                }
                else
                    m_creature->ForcedDespawn();

                return;
            }

            if (!HasEscortState(STATE_ESCORT_PAUSED))
            {
                m_creature->GetMotionMaster()->MovePoint(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
                debug_log("SD2: EscortAI start waypoint %u (%f, %f, %f).", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);

                WaypointStart(CurrentWP->id);

                m_uiWPWaitTimer = 0;
            }
        }
        else
            m_uiWPWaitTimer -= uiDiff;
    }

    //Check if player or any member of his group is within range
    if (HasEscortState(STATE_ESCORT_ESCORTING) && m_uiPlayerGUID && !m_creature->getVictim() && !HasEscortState(STATE_ESCORT_RETURNING))
    {
        if (m_uiPlayerCheckTimer < uiDiff)
        {
            if (!IsPlayerOrGroupInRange())
            {
                debug_log("SD2: EscortAI failed because player/group was to far away or not found");

                if (m_bCanInstantRespawn)
                {
                    m_creature->setDeathState(JUST_DIED);
                    m_creature->Respawn();
                }
                else
                    m_creature->ForcedDespawn();

                return;
            }

            m_uiPlayerCheckTimer = 1000;
        }
        else
            m_uiPlayerCheckTimer -= uiDiff;
    }

    UpdateEscortAI(uiDiff);
}

void npc_escortAI::UpdateEscortAI(const uint32 uiDiff)
{
    //Check if we have a current target
    if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        return;

    DoMeleeAttackIfReady();
}

void npc_escortAI::MovementInform(uint32 uiMoveType, uint32 uiPointId)
{
    if (uiMoveType != POINT_MOTION_TYPE || !HasEscortState(STATE_ESCORT_ESCORTING))
        return;

    //Combat start position reached, continue waypoint movement
    if (uiPointId == POINT_LAST_POINT)
    {
        debug_log("SD2: EscortAI has returned to original position before combat");

        if (m_bIsRunning && m_creature->HasSplineFlag(SPLINEFLAG_WALKMODE))
            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        else if (!m_bIsRunning && !m_creature->HasSplineFlag(SPLINEFLAG_WALKMODE))
            m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);

        RemoveEscortState(STATE_ESCORT_RETURNING);

        if (!m_uiWPWaitTimer)
            m_uiWPWaitTimer = 1;
    }
    else if (uiPointId == POINT_HOME)
    {
        debug_log("SD2: EscortAI has returned to original home location and will continue from beginning of waypoint list.");

        CurrentWP = WaypointList.begin();
        m_uiWPWaitTimer = 1;
    }
    else
    {
        //Make sure that we are still on the right waypoint
        if (CurrentWP->id != uiPointId)
        {
            error_log("SD2: EscortAI reached waypoint out of order %u, expected %u.", uiPointId, CurrentWP->id);
            return;
        }

        debug_log("SD2: EscortAI waypoint %u reached.", CurrentWP->id);

        //Call WP function
        WaypointReached(CurrentWP->id);

        m_uiWPWaitTimer = CurrentWP->WaitTimeMs + 1;

        ++CurrentWP;
    }
}

/*void npc_escortAI::AddWaypoint(uint32 id, float x, float y, float z, uint32 WaitTimeMs)
{
    Escort_Waypoint t(id, x, y, z, WaitTimeMs);

    WaypointList.push_back(t);
}*/

void npc_escortAI::FillPointMovementListForCreature()
{
    std::vector<ScriptPointMove> const &pPointsEntries = pSystemMgr.GetPointMoveList(m_creature->GetEntry());

    if (pPointsEntries.empty())
        return;

    std::vector<ScriptPointMove>::const_iterator itr;

    for (itr = pPointsEntries.begin(); itr != pPointsEntries.end(); ++itr)
    {
        Escort_Waypoint pPoint(itr->uiPointId, itr->fX, itr->fY, itr->fZ, itr->uiWaitTime);
        WaypointList.push_back(pPoint);
    }
}

void npc_escortAI::SetRun(bool bRun)
{
    if (bRun)
    {
        if (!m_bIsRunning)
            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        else
            debug_log("SD2: EscortAI attempt to set run mode, but is already running.");
    }
    else
    {
        if (m_bIsRunning)
            m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
        else
            debug_log("SD2: EscortAI attempt to set walk mode, but is already walking.");
    }
    m_bIsRunning = bRun;
}

//TODO: get rid of this many variables passed in function.
void npc_escortAI::Start(bool bIsActiveAttacker, bool bRun, uint64 uiPlayerGUID, const Quest* pQuest, bool bInstantRespawn, bool bCanLoopPath)
{
    if (m_creature->getVictim())
    {
        error_log("SD2: EscortAI attempt to Start while in combat.");
        return;
    }

    if (HasEscortState(STATE_ESCORT_ESCORTING))
    {
        error_log("SD2: EscortAI attempt to Start while already escorting.");
        return;
    }

    if (!WaypointList.empty())
        WaypointList.clear();

    FillPointMovementListForCreature();

    if (WaypointList.empty())
    {
        error_db_log("SD2: EscortAI Start with 0 waypoints (possible missing entry in script_waypoint).");
        return;
    }

    //set variables
    m_bIsActiveAttacker = bIsActiveAttacker;
    m_bIsRunning = bRun;

    m_uiPlayerGUID = uiPlayerGUID;
    m_pQuestForEscort = pQuest;

    m_bCanInstantRespawn = bInstantRespawn;
    m_bCanReturnToStart = bCanLoopPath;

    if (m_bCanReturnToStart && m_bCanInstantRespawn)
        debug_log("SD2: EscortAI is set to return home after waypoint end and instant respawn at waypoint end. Creature will never despawn.");

    if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
    {
        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->MoveIdle();
        debug_log("SD2: EscortAI start with WAYPOINT_MOTION_TYPE, changed to MoveIdle.");
    }

    //disable npcflags
    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);

    debug_log("SD2: EscortAI started with " SIZEFMTD " waypoints. ActiveAttacker = %d, Run = %d, PlayerGUID = " UI64FMTD, WaypointList.size(), m_bIsActiveAttacker, m_bIsRunning, m_uiPlayerGUID);

    CurrentWP = WaypointList.begin();

    //Set initial speed
    if (m_bIsRunning)
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);

    AddEscortState(STATE_ESCORT_ESCORTING);

    JustStartedEscort();
}

void npc_escortAI::SetEscortPaused(bool bPaused)
{
    if (!HasEscortState(STATE_ESCORT_ESCORTING))
        return;

    if (bPaused)
        AddEscortState(STATE_ESCORT_PAUSED);
    else
        RemoveEscortState(STATE_ESCORT_PAUSED);
}
