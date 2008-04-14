#include "..\ScriptMgr.h"
#include "npc_escortAI.h"
#include "TargetedMovementGenerator.h"
#include "PointMovementGenerator.h"

#define VISIBLE_RANGE 26.46f

#define WP_LAST_POINT   -1

bool npc_escortAI::IsVisible(Unit* who) const
{
    if (!who)
        return false;

    return m_creature->IsWithinDistInMap(who, VISIBLE_RANGE) && who->isVisibleForOrDetect(m_creature,true);
}

void npc_escortAI::AttackStart(Unit *who)
{
    if (!who)
        return;

    if (IsBeingEscorted && !DefendAttackStart)
        return;

    if (who->isTargetableForAttack())
    {
        //Begin attack
        if ( m_creature->Attack(who) )
        {
            m_creature->GetMotionMaster()->MovementExpired();
            m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*who));
            m_creature->AddThreat(who, 0.0f);
            m_creature->resetAttackTimer();

            if (who->GetTypeId() == TYPEID_PLAYER)
                m_creature->SetLootRecipient((Player*)who);
        }

        if (!InCombat)
        {
            InCombat = true;

            //Store last position
            m_creature->GetPosition(LastPos.x, LastPos.y, LastPos.z);

            //Aggro(who);
        }
    }
}

void npc_escortAI::MoveInLineOfSight(Unit *who)
{
    if (IsBeingEscorted && !AttackMLOS)
        return;

    if( !m_creature->getVictim() && who->isTargetableForAttack() && ( m_creature->IsHostileTo( who )) && who->isInAccessablePlaceFor(m_creature) )
    {
        if (m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
            return;

        float attackRadius = m_creature->GetAttackDistance(who);
        if(m_creature->IsWithinDistInMap(who, attackRadius))
        {
            // Check first that object is in an angle in front of this one before LoS check
            if( m_creature->HasInArc(M_PI/2.0f, who) && m_creature->IsWithinLOSInMap(who) )
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin attack
                if ( m_creature->Attack(who) )
                {
                    m_creature->GetMotionMaster()->MovementExpired();
                    m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*who));
                    m_creature->AddThreat(who, 0.0f);
                    m_creature->resetAttackTimer();

                    if (who->GetTypeId() == TYPEID_PLAYER)
                        m_creature->SetLootRecipient((Player*)who);
                }
                
                if (!InCombat)
                {
                    InCombat = true;

                    //Store last position
                    m_creature->GetPosition(LastPos.x, LastPos.y, LastPos.z);
                }
            }
        }
    }
}

void npc_escortAI::JustDied(Unit* killer)
{
    InCombat = false;
    IsBeingEscorted = false;
}

void npc_escortAI::EnterEvadeMode()
{
    InCombat = false;

    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop();

    if (IsBeingEscorted)
    {
        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(WP_LAST_POINT, LastPos.x, LastPos.y, LastPos.z ));

    }else 
    {
        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->TargetedHome();
    }
}

void npc_escortAI::UpdateAI(const uint32 diff)
{
    if (IsBeingEscorted)
    if (WaitTimer)
        if (WaitTimer <= diff)
        {
            if (ReconnectWP)
            {
                m_creature->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z ));
                error_log("Reconnect WP is: %d, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
                WaitTimer = 0;
                ReconnectWP = false;
                return;
            }

            ++CurrentWP;

            //End of the line, Despawn self then immediatly respawn
            if (CurrentWP == WaypointList.end())
            {
                error_log("SD2 NOTICE: EscortAI reached end of waypoints");
                m_creature->SetHealth(0);
                m_creature->setDeathState(JUST_DIED);

                IsBeingEscorted = false;
                WaitTimer = 0;
                return;
            }

            m_creature->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z ));
            error_log("Next WP is: %d, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);

            WaitTimer = 0;
        }else WaitTimer -= diff;

    //Check if we have a current target
    if( m_creature->isAlive() && m_creature->SelectHostilTarget() && m_creature->getVictim())
    {
        //If we are within range melee the target
        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            if( m_creature->isAttackReady() )
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }
}

void npc_escortAI::MovementInform(uint32 type, uint32 id)
{
    //if (type != POINT_MOTION_TYPE || !IsBeingEscorted)
    //    return;

    //Original position reached, continue waypoint movement
    if (id == WP_LAST_POINT)
    {
        error_log("Returned to position before combat %d", CurrentWP->id);
        ReconnectWP = true;
        WaitTimer = 1;

    }else
    {
        //Make sure that we are still on the right waypoint
        if (CurrentWP->id != id)
        {
            error_log("SD2 ERROR: EscortAI reached waypoint out of order %d, expected %d", id, CurrentWP->id);
            return;
        }

        error_log("Waypoint %d reached", CurrentWP->id);

        //Call WP function
        if (CurrentWP->pCallBack)
            CurrentWP->pCallBack(m_creature, NULL, CurrentWP->id);

        WaitTimer = CurrentWP->WaitTimeMs + 1;
    }
}

void npc_escortAI::AddWaypoint(uint32 id, float x, float y, float z, uint32 WaitTimeMs, EscortAICallback pCallBack)
{
    Escort_Waypoint t(id, x, y, z, pCallBack, WaitTimeMs);

    WaypointList.push_back(t);
}

void npc_escortAI::Start(bool Attack, bool Defend)
{

    if (InCombat)
        return;

    if (WaypointList.empty())
    {
        error_log("SD2 ERROR: Call to escortAI::Start with 0 waypoints");
        return;
    }

    error_log("SD2 ERROR: Call to escortAI::Start with %d waypoints", WaypointList.size());

    CurrentWP = WaypointList.begin();
    m_creature->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z ));
    error_log("Next WP is: %d, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
    IsBeingEscorted = true;
    ReconnectWP = false;
}