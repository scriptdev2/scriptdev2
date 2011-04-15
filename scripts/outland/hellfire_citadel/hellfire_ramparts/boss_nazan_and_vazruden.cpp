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
SDName: Boss_Nazan_And_Vazruden
SD%Complete: 30
SDComment: Encounter is not complete. TODO: re-check script when MovementInform call from core work as expected.
SDCategory: Hellfire Citadel, Hellfire Ramparts
EndScriptData */

#include "precompiled.h"
#include "hellfire_ramparts.h"

enum
{
    SAY_INTRO               = -1543017,
    SAY_AGGRO1              = -1543018,
    SAY_AGGRO2              = -1543019,
    SAY_AGGRO3              = -1543020,
    SAY_TAUNT               = -1543021,
    SAY_KILL1               = -1543022,
    SAY_KILL2               = -1543023,
    SAY_DEATH               = -1543024,
    EMOTE_DESCEND           = -1543025,

    //vazruden
    SPELL_REVENGE           = 40392,

    //nazan
    SPELL_FIREBALL          = 30691,                        // TODO: IDs not verified
    SPELL_FIREBALL_H        = 36920,

    SPELL_CONE_OF_FIRE      = 30926,
    SPELL_H_CONE_OF_FIRE    = 36921,

    SPELL_H_BELLOW_ROAR     = 39427,

    //misc
    POINT_ID_CENTER         = 100,
    POINT_ID_FLYING         = 101,
    POINT_ID_COMBAT         = 102,

    NPC_VAZRUDEN_HERALD     = 17307,
    NPC_NAZAN               = 17536,
    NPC_VAZRUDEN            = 17537
};

const float afCenterPos[3] = {-1399.401f, 1736.365f, 87.008f}; //moves here to drop off nazan
const float afCombatPos[3] = {-1413.848f, 1754.019f, 83.146f}; //moves here when decending

struct MANGOS_DLL_DECL boss_vazrudenAI : public ScriptedAI
{
    boss_vazrudenAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bHealthBelow;

    void Reset()
    {
        m_bHealthBelow = false;
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VAZRUDEN, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void PrepareAndDescendMount()
    {
        if (Creature* pHerald = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_HERALD)))
        {
            if (pHerald->HasSplineFlag(SPLINEFLAG_WALKMODE))
                pHerald->RemoveSplineFlag(SPLINEFLAG_WALKMODE);

            pHerald->GetMotionMaster()->MovePoint(POINT_ID_COMBAT, afCombatPos[0], afCombatPos[1], afCombatPos[2]);

            DoScriptText(EMOTE_DESCEND, pHerald);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bHealthBelow && m_creature->GetHealthPercent() <= 30.0f)
        {
            if (m_pInstance)
                PrepareAndDescendMount();

            m_bHealthBelow = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_vazruden(Creature* pCreature)
{
    return new boss_vazrudenAI(pCreature);
}

// Creature fly around platform by default.
// After "dropping off" Vazruden, transforms to mount (Nazan) and are then ready to fight when
// Vazruden reach 30% HP
struct MANGOS_DLL_DECL boss_vazruden_heraldAI : public ScriptedAI
{
    boss_vazruden_heraldAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetActiveObjectState(true);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsEventInProgress;
    uint32 m_uiMovementTimer;
    uint32 m_uiFireballTimer;

    uint64 m_uiLastSeenPlayerGUID;
    uint64 m_uiVazrudenGUID;

    void Reset()
    {
        if (m_creature->GetEntry() != NPC_VAZRUDEN_HERALD)
            m_creature->UpdateEntry(NPC_VAZRUDEN_HERALD);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        m_uiMovementTimer = 0;
        m_bIsEventInProgress = false;
        m_uiLastSeenPlayerGUID = 0;
        m_uiVazrudenGUID = 0;
        m_uiFireballTimer = 0;

        // see boss_onyxia
        // sort of a hack, it is unclear how this really work but the values appear to be valid
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
        m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_bIsEventInProgress && !m_uiLastSeenPlayerGUID && pWho->GetTypeId() == TYPEID_PLAYER && pWho->isAlive() && !((Player*)pWho)->isGameMaster())
        {
            if (m_creature->IsWithinDistInMap(pWho, 40.0f))
                m_uiLastSeenPlayerGUID = pWho->GetGUID();
        }

        if (m_pInstance && m_pInstance->GetData(TYPE_NAZAN) != IN_PROGRESS)
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (!m_pInstance)
            return;

        if (uiType == WAYPOINT_MOTION_TYPE)
        {
            if (m_uiMovementTimer || m_bIsEventInProgress)
                return;

            if (m_pInstance->GetData(TYPE_NAZAN) == SPECIAL)
            {
                m_creature->SetCombatStartPosition(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                m_uiMovementTimer = 1000;
                m_bIsEventInProgress = true;
            }
        }

        if (uiType == POINT_MOTION_TYPE)
        {
            switch (uiPointId)
            {
                case POINT_ID_CENTER:
                    DoSplit();
                    break;
                case POINT_ID_COMBAT:
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_pInstance->SetData(TYPE_NAZAN, IN_PROGRESS);

                    // Landing
                    // undo flying
                    m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, 0);
                    m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);

                    Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiLastSeenPlayerGUID);
                    if (pPlayer && pPlayer->isAlive())
                        AttackStart(pPlayer);

                    break;
                }
                case POINT_ID_FLYING:
                    m_uiFireballTimer = 3000;
                    break;
            }
        }
    }

    void DoMoveToCenter()
    {
        DoScriptText(SAY_INTRO, m_creature);
        m_creature->GetMotionMaster()->MovePoint(POINT_ID_CENTER, afCenterPos[0], afCenterPos[1], afCenterPos[2]);
    }

    void DoMoveToAir()
    {
        float fX, fY, fZ;
        m_creature->GetCombatStartPosition(fX, fY, fZ);

        // Remove Idle MMGen
        if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == IDLE_MOTION_TYPE)
            m_creature->GetMotionMaster()->MovementExpired(false);

        m_creature->GetMotionMaster()->MovePoint(POINT_ID_FLYING, fX, fY, fZ);
    }

    void DoSplit()
    {
        m_creature->UpdateEntry(NPC_NAZAN);

        m_creature->SummonCreature(NPC_VAZRUDEN, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0);

        m_uiMovementTimer = 3000;

        // Let him idle for now
        m_creature->GetMotionMaster()->MoveIdle();
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() != NPC_VAZRUDEN)
            return;

        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiLastSeenPlayerGUID))
            pSummoned->AI()->AttackStart(pPlayer);

        m_uiVazrudenGUID = pSummoned->GetGUID();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VAZRUDEN, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NAZAN, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (m_uiMovementTimer)
            {
                if (m_uiMovementTimer <= uiDiff)
                {
                    if (m_pInstance)
                    {
                        if (m_pInstance->GetData(TYPE_VAZRUDEN) == IN_PROGRESS)
                            DoMoveToAir();
                        else
                            DoMoveToCenter();
                    }
                    m_uiMovementTimer = 0;
                }
                else
                    m_uiMovementTimer -= uiDiff;
            }

            if (m_uiVazrudenGUID && m_uiFireballTimer)
            {
                if (m_uiFireballTimer <= uiDiff)
                {
                    if (Creature* pVazruden = m_creature->GetMap()->GetCreature(m_uiVazrudenGUID))
                    {
                        if (Unit* pEnemy = pVazruden->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            DoCastSpellIfCan(pEnemy, m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H, 0, pVazruden->GetGUID());
                    }
                    m_uiFireballTimer = urand(4000, 8000);
                }
                else
                    m_uiFireballTimer -= uiDiff;
            }

            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_vazruden_herald(Creature* pCreature)
{
    return new boss_vazruden_heraldAI(pCreature);
}

void AddSC_boss_nazan_and_vazruden()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_vazruden";
    newscript->GetAI = &GetAI_boss_vazruden;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_vazruden_herald";
    newscript->GetAI = &GetAI_boss_vazruden_herald;
    newscript->RegisterSelf();
}
