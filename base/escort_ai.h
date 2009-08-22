/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_ESCORTAI_H
#define SC_ESCORTAI_H

#include "../system/system.h"

struct Escort_Waypoint
{
    Escort_Waypoint(uint32 _id, float _x, float _y, float _z, uint32 _w)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
        WaitTimeMs = _w;
    }

    uint32 id;
    float x;
    float y;
    float z;
    uint32 WaitTimeMs;
};

struct MANGOS_DLL_DECL npc_escortAI : public ScriptedAI
{
    public:
        explicit npc_escortAI(Creature* pCreature);
        ~npc_escortAI() {}

        // Pure Virtual Functions
        virtual void WaypointReached(uint32 uiPointId) = 0;
        virtual void WaypointStart(uint32 uiPointId) {}

        virtual void Aggro(Unit*);

        virtual void Reset() = 0;

        // CreatureAI functions
        bool IsVisible(Unit*) const;

        void AttackStart(Unit*);

        void EnterCombat(Unit*);

        void MoveInLineOfSight(Unit*);

        void JustDied(Unit*);

        void JustRespawned();

        void EnterEvadeMode();

        void UpdateAI(const uint32);                        //the "internal" update, calls UpdateEscortAI()
        virtual void UpdateEscortAI(const uint32);          //used when it's needed to add code in update (abilities, scripted events, etc)

        void MovementInform(uint32, uint32);

        // EscortAI functions
        //void AddWaypoint(uint32 id, float x, float y, float z, uint32 WaitTimeMs = 0);

        bool IsPlayerOrGroupInRange();

        Player* GetPlayerForEscort()
        {
            return (Player*)Unit::GetUnit(*m_creature, m_uiPlayerGUID);
        }

        void FillPointMovementListForCreature();

        void Start(bool bIsActiveAttacker = true, bool bRun = false, uint64 uiPlayerGUID = 0, const Quest* pQuest = NULL, bool bInstantRespawn = false, bool bCanLoopPath = false);

        void SetRun(bool bRun = true);

    // EscortAI variables
    protected:
        bool IsBeingEscorted;
        bool IsOnHold;

    private:
        uint64 m_uiPlayerGUID;
        uint32 m_uiWPWaitTimer;
        uint32 m_uiPlayerCheckTimer;

        const Quest* m_pQuestForEscort;                     //generally passed in Start() when regular escort script.

        std::list<Escort_Waypoint> WaypointList;
        std::list<Escort_Waypoint>::iterator CurrentWP;

        bool m_bIsActiveAttacker;                           //possible obsolete, and should be determined with db only (civilian)
        bool m_bIsReturning;                                //in use when creature leave combat, and are returning to combat start position
        bool m_bIsRunning;                                  //all creatures are walking by default (has flag MONSTER_MOVE_WALK)
        bool m_bCanInstantRespawn;                          //if creature should respawn instantly after escort over (if not, database respawntime are used)
        bool m_bCanReturnToStart;                           //if creature can walk same path (loop) without despawn. Not for regular escort quests.
};
#endif
