/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

/* ScriptData
SDName: Npc_EscortAI
SD%Complete: 100
SDComment:
SDCategory: Npc
EndScriptData */

#include "precompiled.h"
#include "npc_escortAI.h"

#define WP_LAST_POINT   -1
#define MAX_PLAYER_DISTANCE 50

bool npc_escortAI::IsVisible(Unit* who) const
{
    if (!who)
        return false;

    return m_creature->IsWithinDist(who,VISIBLE_RANGE) && who->isVisibleForOrDetect(m_creature,true);
}

void npc_escortAI::AttackStart(Unit *who)
{
    if (!who)
        return;

    if (IsBeingEscorted && !Defend)
        return;

    if (m_creature->Attack(who, true))
    {
        m_creature->AddThreat(who, 0.0f);
        m_creature->SetInCombatWith(who);
        who->SetInCombatWith(m_creature);

        if (bCombatMovement)
        {
            m_creature->GetMotionMaster()->MovementExpired();
            m_creature->GetMotionMaster()->MoveChase(who);
        }
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

void npc_escortAI::MoveInLineOfSight(Unit *who)
{
    if (IsBeingEscorted && !Attack)
        return;

    if (!m_creature->hasUnitState(UNIT_STAT_STUNNED) && who->isTargetableForAttack() &&
        m_creature->IsHostileTo(who) && who->isInAccessablePlaceFor(m_creature))
    {
        if (!m_creature->canFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
            return;

        float attackRadius = m_creature->GetAttackDistance(who);
        if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
        {
            if (!m_creature->getVictim())
            {
                AttackStart(who);
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
            }
            else if (m_creature->GetMap()->IsDungeon())
            {
                who->SetInCombatWith(m_creature);
                m_creature->AddThreat(who, 0.0f);
            }
        }
    }
}

void npc_escortAI::JustRespawned()
{
    IsBeingEscorted = false;
    IsOnHold = false;
    bCombatMovement = true;

    //Restore original NpcFlags
    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, m_uiNpcFlags);

    Reset();
}

void npc_escortAI::EnterEvadeMode()
{
    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop(true);
    m_creature->SetLootRecipient(NULL);

    if (IsBeingEscorted)
    {
        debug_log("SD2: EscortAI has left combat and is now returning to CombatStartPosition.");
        Returning = true;
        m_creature->GetMotionMaster()->MovementExpired();

        //if default is WAYPOINT_MOTION_TYPE, must MoveIdle to prevent from using
        if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
            m_creature->GetMotionMaster()->MoveIdle();

        float fPosX, fPosY, fPosZ;
        m_creature->GetCombatStartPosition(fPosX, fPosY, fPosZ);
        m_creature->GetMotionMaster()->MovePoint(WP_LAST_POINT, fPosX, fPosY, fPosZ);
    }
    else
    {
        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->MoveTargetedHome();
    }

    Reset();
}

void npc_escortAI::UpdateAI(const uint32 diff)
{
    //Waypoint Updating
    if (IsBeingEscorted && !m_creature->isInCombat() && WaitTimer && !Returning)
    {
        if (WaitTimer <= diff)
        {
            if (ReconnectWP)
            {
                //Correct movement speed
                if (bIsRunning && m_creature->HasUnitMovementFlag(MOVEMENTFLAG_WALK_MODE))
                    m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
                else if (!bIsRunning && !m_creature->HasUnitMovementFlag(MOVEMENTFLAG_WALK_MODE))
                    m_creature->AddUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);

                //Continue with waypoints
                if (!IsOnHold)
                {
                    if (CurrentWP != WaypointList.end())
                    {
                        m_creature->GetMotionMaster()->MovePoint(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
                        debug_log("SD2: EscortAI Reconnect WP is: %u, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);

                        WaitTimer = 0;
                        ReconnectWP = false;
                        return;
                    }
                    else
                        debug_log("SD2: EscortAI Reconnected to end of WP list");
                }
            }

            //End of the line, Despawn self then immediatly respawn
            if (CurrentWP == WaypointList.end())
            {
                debug_log("SD2: EscortAI reached end of waypoints");

                m_creature->setDeathState(JUST_DIED);
                m_creature->SetHealth(0);
                m_creature->CombatStop(true);
                m_creature->DeleteThreatList();
                m_creature->Respawn();
                m_creature->GetMotionMaster()->Clear(true);

                //Restore original NpcFlags
                m_creature->SetUInt32Value(UNIT_NPC_FLAGS, m_uiNpcFlags);

                IsBeingEscorted = false;
                WaitTimer = 0;
                return;
            }

            if (!IsOnHold)
            {
                m_creature->GetMotionMaster()->MovePoint(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
                debug_log("SD2: EscortAI Next WP is: %u, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
                WaitTimer = 0;
            }
        }else WaitTimer -= diff;
    }

    //Check if player is within range
    if (IsBeingEscorted && !m_creature->isInCombat() && PlayerGUID)
    {
        if (PlayerTimer < diff)
        {
            Unit* p = Unit::GetUnit(*m_creature, PlayerGUID);

            if (!p || !m_creature->IsWithinDist(p,MAX_PLAYER_DISTANCE))
            {
                JustDied(m_creature);
                IsBeingEscorted = false;

                debug_log("SD2: EscortAI Evaded back to spawn point because pPlayer was to far away or not found");

                m_creature->setDeathState(JUST_DIED);
                m_creature->SetHealth(0);
                m_creature->CombatStop(true);
                m_creature->DeleteThreatList();
                m_creature->Respawn();
                m_creature->GetMotionMaster()->Clear(true);

                //Restore original NpcFlags
                m_creature->SetUInt32Value(UNIT_NPC_FLAGS, m_uiNpcFlags);
            }

            PlayerTimer = 1000;
        }else PlayerTimer -= diff;
    }

    //Check if we have a current target
    if (m_creature->isAlive() && m_creature->SelectHostilTarget() && m_creature->getVictim())
    {
        //If we are within range melee the target
        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            if (m_creature->isAttackReady())
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }
}

void npc_escortAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE || !IsBeingEscorted)
        return;

    //Original position reached, continue waypoint movement
    if (id == WP_LAST_POINT)
    {
        debug_log("SD2: EscortAI has returned to original position before combat");
        ReconnectWP = true;
        Returning = false;

        if (!WaitTimer)
            WaitTimer = 1;
    }
    else
    {
        //Make sure that we are still on the right waypoint
        if (CurrentWP->id != id)
        {
            debug_log("SD2 ERROR: EscortAI reached waypoint out of order %d, expected %d", id, CurrentWP->id);
            return;
        }

        debug_log("SD2: EscortAI Waypoint %d reached", CurrentWP->id);

        //Call WP function
        WaypointReached(CurrentWP->id);

        WaitTimer = CurrentWP->WaitTimeMs + 1;

        ++CurrentWP;
    }
}

void npc_escortAI::AddWaypoint(uint32 id, float x, float y, float z, uint32 WaitTimeMs)
{
    Escort_Waypoint t(id, x, y, z, WaitTimeMs);

    WaypointList.push_back(t);
}

void npc_escortAI::FillPointMovementListForCreature()
{
    UNORDERED_MAP<uint32, std::vector<PointMovement> >::iterator pPointsEntries = PointMovementMap.find(m_creature->GetEntry());

    if (pPointsEntries != PointMovementMap.end())
    {
        std::vector<PointMovement>::iterator itr;

        for (itr = pPointsEntries->second.begin(); itr != pPointsEntries->second.end(); ++itr)
        {
            Escort_Waypoint pPoint(itr->m_uiPointId,itr->m_fX,itr->m_fY,itr->m_fZ,itr->m_uiWaitTime);
            WaypointList.push_back(pPoint);
        }
    }
}

void npc_escortAI::SetRun(bool bRun)
{
    if (bRun)
    {
        if (!bIsRunning)
            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
        else
            debug_log("SD2: EscortAI attempt to set run mode, but is already running.");
    }
    else
    {
        if (bIsRunning)
            m_creature->AddUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
        else
            debug_log("SD2: EscortAI attempt to set walk mode, but is already walking.");
    }
    bIsRunning = bRun;
}

void npc_escortAI::Start(bool bAttack, bool bDefend, bool bRun, uint64 pGUID)
{
    if (m_creature->isInCombat())
    {
        debug_log("SD2 ERROR: EscortAI attempt to Start while in combat");
        return;
    }

    if (IsBeingEscorted)
    {
        error_log("SD2: EscortAI attempt to Start while already escorting");
        return;
    }

    if (WaypointList.empty())
    {
        error_db_log("SD2: EscortAI Start with 0 waypoints (possible missing entry in script_waypoint)");
        return;
    }

    if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
    {
        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->MoveIdle();
        debug_log("SD2: EscortAI start with WAYPOINT_MOTION_TYPE, changed to MoveIdle.");
    }

    Attack = bAttack;
    Defend = bDefend;
    bIsRunning = bRun;
    PlayerGUID = pGUID;

    //store original NpcFlags
    m_uiNpcFlags = m_creature->GetUInt32Value(UNIT_NPC_FLAGS);

    //remove them if any
    if (m_uiNpcFlags)
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);

    debug_log("SD2: EscortAI started with %d waypoints. Attack = %d, Defend = %d, Run = %d, PlayerGUID = %d", WaypointList.size(), Attack, Defend, bIsRunning, PlayerGUID);

    CurrentWP = WaypointList.begin();

    //Set initial speed
    if (bIsRunning)
        m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
    else
        m_creature->AddUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);

    //Start WP
    m_creature->GetMotionMaster()->MovePoint(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
    debug_log("SD2: EscortAI Next WP is: %d, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
    IsBeingEscorted = true;
    ReconnectWP = false;
    Returning = false;
    IsOnHold = false;
    bCombatMovement = true;
}
