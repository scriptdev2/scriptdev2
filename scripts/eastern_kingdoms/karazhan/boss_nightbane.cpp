/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Nightbane
SD%Complete: 50
SDComment: Only ground phase. Intro movement is a little choppy because of the lack of waypoint movement support. Timers need adjustment.
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"
#include "escort_ai.h"

enum
{
    EMOTE_AWAKEN                = -1532125,
    SAY_AGGRO                   = -1532126,
    SAY_AIR_PHASE               = -1532127,
    SAY_LAND_PHASE_1            = -1532128,
    SAY_LAND_PHASE_2            = -1532129,
    EMOTE_DEEP_BREATH           = -1532130,

    // ground phase spells
    SPELL_BELLOWING_ROAR        = 39427,
    SPELL_CHARRED_EARTH         = 30129,                    //Also 30209 (Target Charred Earth) triggers this
    SPELL_DISTRACTING_ASH       = 30130,
    SPELL_SMOLDERING_BREATH     = 30210,
    SPELL_TAIL_SWEEP            = 25653,
    SPELL_CLEAVE                = 30131,

    // air phase spells
    SPELL_RAIN_OF_BONES         = 37098,                    // triggers 30170 on dummy aura apply
    SPELL_SMOKING_BLAST         = 37057,
    SPELL_FIREBALL_BARRAGE      = 30282
};

struct MANGOS_DLL_DECL boss_nightbaneAI : public npc_escortAI
{
    boss_nightbaneAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bIsGroundPhase;
    uint8 m_uiPhase;

    uint32 m_uiBellowingRoarTimer;
    uint32 m_uiCharredEarthTimer;
    uint32 m_uiDistractingAshTimer;
    uint32 m_uiSmolderingBreathTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uiCleavetimer;

    void Reset()
    {
        m_bIsGroundPhase            = true;
        m_uiPhase                   = 3;

        m_uiBellowingRoarTimer      = urand(20000, 30000);
        m_uiCharredEarthTimer       = urand(10000, 15000);
        m_uiDistractingAshTimer     = urand(7000, 17000);
        m_uiSmolderingBreathTimer   = urand(9000, 13000);
        m_uiTailSweepTimer          = urand(12000, 15000);
        m_uiCleavetimer             = urand(4000, 8000);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NIGHTBANE, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NIGHTBANE, FAIL);

        // Reset escort waypoints
        npc_escortAI::JustRespawned();
    }

    void EnterEvadeMode()
    {
        // Use standard AI evade, in order to reset position
        ScriptedAI::EnterEvadeMode();
    }

    void WaypointReached(uint32 uiPointId)
    {
        // Set in combat after the intro is done
        if (uiPointId == 31)
        {
            SetEscortPaused(true);
            m_creature->SetLevitate(false);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);

            m_creature->SetInCombatWithZone();
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Ground phase
        if (m_bIsGroundPhase)
        {
            if (m_uiBellowingRoarTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BELLOWING_ROAR) == CAST_OK)
                    m_uiBellowingRoarTimer = urand(20000, 30000);
            }
            else
                m_uiBellowingRoarTimer -= uiDiff;

            if (m_uiSmolderingBreathTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SMOLDERING_BREATH) == CAST_OK)
                    m_uiSmolderingBreathTimer = urand(14000, 20000);
            }
            else
                m_uiSmolderingBreathTimer -= uiDiff;

            if (m_uiDistractingAshTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_DISTRACTING_ASH) == CAST_OK)
                        m_uiDistractingAshTimer = urand(35000, 45000);
                }
            }
            else
                m_uiDistractingAshTimer -= uiDiff;

            if (m_uiCharredEarthTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_CHARRED_EARTH) == CAST_OK)
                        m_uiCharredEarthTimer = urand(25000, 35000);
                }
            }
            else
                m_uiCharredEarthTimer -= uiDiff;

            if (m_uiTailSweepTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_TAIL_SWEEP) == CAST_OK)
                    m_uiTailSweepTimer = urand(14000, 20000);
            }
            else
                m_uiTailSweepTimer -= uiDiff;

            if (m_uiCleavetimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                    m_uiCleavetimer = urand(6000, 12000);
            }
            else
                m_uiCleavetimer -= uiDiff;

            if (m_creature->GetHealthPercent() < 100 - 25*m_uiPhase)
            {
                // ToDo: change phase
                --m_uiPhase;
            }

            DoMeleeAttackIfReady();
        }
        // Air phase
        else
        {
            // ToDo:
        }
    }
};

CreatureAI* GetAI_boss_nightbane(Creature* pCreature)
{
    return new boss_nightbaneAI(pCreature);
}

bool ProcessEventId_event_spell_summon_nightbane(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)((Player*)pSource)->GetInstanceData();
        if (!pInstance)
            return false;

        if (pInstance->GetData(TYPE_NIGHTBANE) == NOT_STARTED || pInstance->GetData(TYPE_NIGHTBANE) == FAIL)
        {
            if (Creature* pNightbane = pInstance->GetSingleCreatureFromStorage(NPC_NIGHTBANE))
            {
                DoScriptText(EMOTE_AWAKEN, ((Player*)pSource));
                pInstance->SetData(TYPE_NIGHTBANE, IN_PROGRESS);

                // Sort of a hack, it is unclear how this really work but the values appear to be valid (see Onyxia, too)
                pNightbane->SetStandState(UNIT_STAND_STATE_STAND);
                pNightbane->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
                pNightbane->SetLevitate(true);

                // Switch to waypoint movement
                if (boss_nightbaneAI* pNightbaneAI = dynamic_cast<boss_nightbaneAI*>(pNightbane->AI()))
                    pNightbaneAI->Start(true);
            }
        }
    }

    return true;
}

void AddSC_boss_nightbane()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_nightbane";
    pNewScript->GetAI = &GetAI_boss_nightbane;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_spell_summon_nightbane";
    pNewScript->pProcessEventId = &ProcessEventId_event_spell_summon_nightbane;
    pNewScript->RegisterSelf();
}
