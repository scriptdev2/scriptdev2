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

    if (IsBeingEscorted && !m_bCanDefendSelf)
        return;

    if (m_creature->Attack(who, true))
    {
        m_creature->AddThreat(who, 0.0f);
        m_creature->SetInCombatWith(who);
        who->SetInCombatWith(m_creature);

        if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
            m_creature->GetMotionMaster()->MovementExpired();

        if (IsCombatMovement())
            m_creature->GetMotionMaster()->MoveChase(who);
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
    if (IsBeingEscorted && !m_bIsActiveAttacker)
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

    if (!IsCombatMovement())
        SetCombatMovement(true);

    m_uiWPWaitTimer = 0;

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

        if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
        {
            m_bIsReturning = true;

            float fPosX, fPosY, fPosZ;
            m_creature->GetCombatStartPosition(fPosX, fPosY, fPosZ);
            m_creature->GetMotionMaster()->MovePoint(WP_LAST_POINT, fPosX, fPosY, fPosZ);
        }
    }
    else
    {
        if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
            m_creature->GetMotionMaster()->MoveTargetedHome();
    }

    Reset();
}

void npc_escortAI::UpdateAI(const uint32 diff)
{
    //Waypoint Updating
    if (IsBeingEscorted && !m_creature->getVictim() && m_uiWPWaitTimer && !m_bIsReturning)
    {
        if (m_uiWPWaitTimer <= diff)
        {
            //End of the line, Despawn self then immediatly respawn
            if (CurrentWP == WaypointList.end())
            {
                debug_log("SD2: EscortAI reached end of waypoints");

                m_creature->setDeathState(JUST_DIED);

                //TODO: add option to set instant respawn? Then use db respawn value as default
                m_creature->Respawn();

                return;
            }

            if (!IsOnHold)
            {
                m_creature->GetMotionMaster()->MovePoint(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
                debug_log("SD2: EscortAI Next WP is: %u, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
                m_uiWPWaitTimer = 0;
            }
        }else m_uiWPWaitTimer -= diff;
    }

    //Check if player or any member of his group is within range
    if (IsBeingEscorted && PlayerGUID && !m_creature->getVictim() && !m_bIsReturning)
    {
        if (m_uiPlayerCheckTimer < diff)
        {
            bool bIsMaxRangeExceeded = true;

            if (Player* pPlayer = (Player*)Unit::GetUnit(*m_creature, PlayerGUID))
            {
                if (Group* pGroup = pPlayer->GetGroup())
                {
                    for(GroupReference* pRef = pGroup->GetFirstMember(); pRef != NULL; pRef = pRef->next())
                    {
                        Player* pMember = pRef->getSource();

                        if (pMember && m_creature->IsWithinDistInMap(pMember, MAX_PLAYER_DISTANCE))
                        {
                            bIsMaxRangeExceeded = false;
                            break;
                        }
                    }
                }
                else
                {
                    if (m_creature->IsWithinDistInMap(pPlayer, MAX_PLAYER_DISTANCE))
                        bIsMaxRangeExceeded = false;
                }
            }

            if (bIsMaxRangeExceeded)
            {
                debug_log("SD2: EscortAI Evaded back to spawn point because player/group was to far away or not found");

                JustDied(m_creature);
                m_creature->setDeathState(JUST_DIED);

                //TODO: add option to set instant respawn? Then use db respawn value as default
                m_creature->Respawn();

                return;
            }

            m_uiPlayerCheckTimer = 1000;
        }else m_uiPlayerCheckTimer -= diff;
    }

    //Check if we have a current target
    if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
        return;

    DoMeleeAttackIfReady();
}

void npc_escortAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE || !IsBeingEscorted)
        return;

    //Combat start position reached, continue waypoint movement
    if (id == WP_LAST_POINT)
    {
        debug_log("SD2: EscortAI has returned to original position before combat");

        if (m_bIsRunning && m_creature->HasMonsterMoveFlag(MONSTER_MOVE_WALK))
            m_creature->RemoveMonsterMoveFlag(MONSTER_MOVE_WALK);
        else if (!m_bIsRunning && !m_creature->HasMonsterMoveFlag(MONSTER_MOVE_WALK))
            m_creature->AddMonsterMoveFlag(MONSTER_MOVE_WALK);

        m_bIsReturning = false;

        if (!m_uiWPWaitTimer)
            m_uiWPWaitTimer = 1;
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

        m_uiWPWaitTimer = CurrentWP->WaitTimeMs + 1;

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
        if (!m_bIsRunning)
            m_creature->RemoveMonsterMoveFlag(MONSTER_MOVE_WALK);
        else
            debug_log("SD2: EscortAI attempt to set run mode, but is already running.");
    }
    else
    {
        if (m_bIsRunning)
            m_creature->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
        else
            debug_log("SD2: EscortAI attempt to set walk mode, but is already walking.");
    }
    m_bIsRunning = bRun;
}

void npc_escortAI::Start(bool bIsActiveAttacker, bool bCanDefendSelf, bool bRun, uint64 uiPlayerGUID)
{
    if (m_creature->getVictim())
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

    m_bIsActiveAttacker = bIsActiveAttacker;
    m_bCanDefendSelf = bCanDefendSelf;
    m_bIsRunning = bRun;
    PlayerGUID = uiPlayerGUID;

    //disable npcflags
    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);

    debug_log("SD2: EscortAI started with %d waypoints. ActiveAttacker = %d, Defend = %d, Run = %d, PlayerGUID = %d", WaypointList.size(), m_bIsActiveAttacker, m_bCanDefendSelf, m_bIsRunning, PlayerGUID);

    CurrentWP = WaypointList.begin();

    //Set initial speed
    if (m_bIsRunning)
        m_creature->RemoveMonsterMoveFlag(MONSTER_MOVE_WALK);

    //Start WP
    m_creature->GetMotionMaster()->MovePoint(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
    debug_log("SD2: EscortAI Next WP is: %d, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);

    IsBeingEscorted = true;
}
