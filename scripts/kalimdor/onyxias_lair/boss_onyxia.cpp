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
SDName: Boss_Onyxia
SD%Complete: 70
SDComment: Phase 3 need additional code. The spawning Whelps need GO-Support. Use of spells 22191 and 21131 unknown
SDCategory: Onyxia's Lair
EndScriptData */

#include "precompiled.h"
#include "onyxias_lair.h"

enum
{
    SAY_AGGRO                   = -1249000,
    SAY_KILL                    = -1249001,
    SAY_PHASE_2_TRANS           = -1249002,
    SAY_PHASE_3_TRANS           = -1249003,
    EMOTE_BREATH                = -1249004,

    SPELL_WINGBUFFET            = 18500,
    SPELL_WINGBUFFET_H          = 69293,
    SPELL_FLAMEBREATH           = 18435,
    SPELL_FLAMEBREATH_H         = 68970,
    SPELL_CLEAVE                = 68868,
    SPELL_TAILSWEEP             = 68867,
    SPELL_TAILSWEEP_H           = 69286,
    SPELL_KNOCK_AWAY            = 19633,
    SPELL_FIREBALL              = 18392,
    SPELL_FIREBALL_H            = 68926,
    SPELL_ERRUPTION             = 17731,                    // does not work
    SPELL_ERRUPTION_H           = 69294,                    // does not work

    //Not much choise about these. We have to make own defintion on the direction/start-end point
    SPELL_BREATH_NORTH_TO_SOUTH = 17086,                    // 20x in "array"
    SPELL_BREATH_SOUTH_TO_NORTH = 18351,                    // 11x in "array"

    SPELL_BREATH_EAST_TO_WEST   = 18576,                    // 7x in "array"
    SPELL_BREATH_WEST_TO_EAST   = 18609,                    // 7x in "array"

    SPELL_BREATH_SE_TO_NW       = 18564,                    // 12x in "array"
    SPELL_BREATH_NW_TO_SE       = 18584,                    // 12x in "array"
    SPELL_BREATH_SW_TO_NE       = 18596,                    // 12x in "array"
    SPELL_BREATH_NE_TO_SW       = 18617,                    // 12x in "array"

    SPELL_VISUAL_BREATH_A       = 4880,                     // Only and all of the above Breath spells (and their triggered spells) have these visuals
    SPELL_VISUAL_BREATH_B       = 4919,

    //SPELL_BREATH                = 21131,                  // 8x in "array", different initial cast than the other arrays

    SPELL_BELLOWINGROAR         = 18431,
    SPELL_HEATED_GROUND         = 22191,                    // TODO

    SPELL_SUMMONWHELP           = 17646,                    // TODO this spell is only a summon spell, but would need a spell to activate the eggs
    SPELL_SUMMON_LAIR_GUARD     = 68968,

    MAX_WHELPS_PER_PACK         = 40,

    PHASE_START                 = 1,
    PHASE_BREATH                = 2,
    PHASE_END                   = 3,
    PHASE_BREATH_PRE            = 4,
    PHASE_BREATH_POST           = 5
};

struct sOnyxMove
{
    uint32 uiLocId;
    uint32 uiLocIdEnd;
    uint32 uiSpellId;
    float fX, fY, fZ;
};

static sOnyxMove aMoveData[]=
{
    {0, 4, SPELL_BREATH_NORTH_TO_SOUTH,  22.8763f, -217.152f, -60.0548f},   //north
    {1, 5, SPELL_BREATH_NE_TO_SW,        10.2191f, -247.912f, -60.896f},    //north-east
    {2, 6, SPELL_BREATH_EAST_TO_WEST,   -31.4963f, -250.123f, -60.1278f},   //east
    {3, 7, SPELL_BREATH_SE_TO_NW,       -63.5156f, -240.096f, -60.477f},    //south-east
    {4, 0, SPELL_BREATH_SOUTH_TO_NORTH, -65.8444f, -213.809f, -60.2985f},   //south
    {5, 1, SPELL_BREATH_SW_TO_NE,       -58.2509f, -189.020f, -60.790f},    //south-west
    {6, 2, SPELL_BREATH_WEST_TO_EAST,   -33.5561f, -182.682f, -60.9457f},   //west
    {7, 3, SPELL_BREATH_NW_TO_SE,         6.8951f, -180.246f, -60.896f},    //north-west
};

static float afSpawnLocations[4][3]=
{
    {-30.127f, -254.463f, -89.440f},                        // whelps
    {-30.817f, -177.106f, -89.258f},                        // whelps
    {-126.57f, -214.609f, -71.446f}                         // guardians
};

struct MANGOS_DLL_DECL boss_onyxiaAI : public ScriptedAI
{
    boss_onyxiaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_onyxias_lair*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_uiMaxBreathPositions = sizeof(aMoveData)/sizeof(sOnyxMove);
        Reset();
    }

    bool m_bIsRegularMode;
    instance_onyxias_lair* m_pInstance;

    uint8 m_uiPhase;
    uint8 m_uiMaxBreathPositions;

    uint32 m_uiFlameBreathTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uiWingBuffetTimer;

    uint32 m_uiMovePoint;
    uint32 m_uiMovementTimer;
    sOnyxMove* m_pPointData;

    uint32 m_uiFireballTimer;
    uint32 m_uiSummonWhelpsTimer;
    uint32 m_uiBellowingRoarTimer;
    uint32 m_uiWhelpTimer;
    uint32 m_uiSummonGuardTimer;

    uint8 m_uiSummonCount;

    bool m_bIsSummoningWhelps;

    void Reset()
    {
        if (!IsCombatMovement())
            SetCombatMovement(true);

        m_uiPhase = PHASE_START;

        m_uiFlameBreathTimer = urand(10000, 20000);
        m_uiTailSweepTimer = urand(15000, 20000);
        m_uiCleaveTimer = urand(2000, 5000);
        m_uiWingBuffetTimer = urand(10000, 20000);

        m_uiMovePoint = urand(0, m_uiMaxBreathPositions - 1);
        m_uiMovementTimer = 20000;
        m_pPointData = GetMoveData();

        m_uiFireballTimer = 15000;
        m_uiSummonWhelpsTimer = 15000;
        m_uiBellowingRoarTimer = 2000;                      // Immediately after landing
        m_uiWhelpTimer = 1000;
        m_uiSummonGuardTimer = 15000;

        m_uiSummonCount = 0;

        m_bIsSummoningWhelps = false;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ONYXIA, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        // in case evade in phase 2, see comments for hack where phase 2 is set
        m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);
        m_creature->SetByteFlag(UNIT_FIELD_BYTES_1, 3, 0);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ONYXIA, FAIL);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ONYXIA, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->GetMotionMaster()->MovePoint(0, afSpawnLocations[3][0], afSpawnLocations[3][1], afSpawnLocations[3][2]);
        pSummoned->SetInCombatWithZone();

        if (pSummoned->GetEntry() == NPC_ONYXIA_WHELP)
            ++m_uiSummonCount;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_BREATH_EAST_TO_WEST ||
            pSpell->Id == SPELL_BREATH_WEST_TO_EAST ||
            pSpell->Id == SPELL_BREATH_SE_TO_NW ||
            pSpell->Id == SPELL_BREATH_NW_TO_SE ||
            pSpell->Id == SPELL_BREATH_SW_TO_NE ||
            pSpell->Id == SPELL_BREATH_NE_TO_SW ||
            pSpell->Id == SPELL_BREATH_SOUTH_TO_NORTH ||
            pSpell->Id == SPELL_BREATH_NORTH_TO_SOUTH)
        {
            if (m_pPointData = GetMoveData())
            {
                if (!m_pInstance)
                    return;

                if (Creature* pTrigger = m_pInstance->instance->GetCreature(m_pInstance->GetOnyxiaTriggerGUID()))
                {
                    m_creature->GetMap()->CreatureRelocation(m_creature, m_pPointData->fX, m_pPointData->fY, m_pPointData->fZ, m_creature->GetAngle(pTrigger));
                    m_creature->SendMonsterMove(m_pPointData->fX, m_pPointData->fY, m_pPointData->fZ, SPLINETYPE_FACINGTARGET, m_creature->GetSplineFlags(), 1, NULL, pTrigger->GetGUID());
                }
            }
        }
    }

    sOnyxMove* GetMoveData()
    {
        for (uint32 i = 0; i < m_uiMaxBreathPositions; ++i)
        {
            if (aMoveData[i].uiLocId == m_uiMovePoint)
                return &aMoveData[i];
        }

        return NULL;
    }

    void MovementInform(uint32 uiMoveType, uint32 uiPointId)
    {
        if (uiMoveType != POINT_MOTION_TYPE || !m_pInstance)
            return;

        if (m_uiPhase == PHASE_BREATH)
        {
            if (Creature* pTrigger = m_pInstance->instance->GetCreature(m_pInstance->GetOnyxiaTriggerGUID()))
                m_creature->SetFacingToObject(pTrigger);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_uiPhase)
        {
            case PHASE_END:                                 // Here is room for additional summoned whelps and Erruption
                if (m_uiBellowingRoarTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_BELLOWINGROAR) == CAST_OK)
                        m_uiBellowingRoarTimer = 30000;
                }
                else
                    m_uiBellowingRoarTimer -= uiDiff;
                // no break, phase 3 will use same abilities as in 1
            case PHASE_START:
            {
                if (m_uiFlameBreathTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FLAMEBREATH : SPELL_FLAMEBREATH_H) == CAST_OK)
                        m_uiFlameBreathTimer = urand(10000, 20000);
                }
                else
                    m_uiFlameBreathTimer -= uiDiff;

                if (m_uiTailSweepTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TAILSWEEP : SPELL_TAILSWEEP_H) == CAST_OK)
                        m_uiTailSweepTimer = urand(15000, 20000);
                }
                else
                    m_uiTailSweepTimer -= uiDiff;

                if (m_uiCleaveTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                        m_uiCleaveTimer = urand(2000, 5000);
                }
                else
                    m_uiCleaveTimer -= uiDiff;

                if (m_uiWingBuffetTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WINGBUFFET : SPELL_WINGBUFFET_H) == CAST_OK)
                        m_uiWingBuffetTimer = urand(15000, 30000);
                }
                else
                    m_uiWingBuffetTimer -= uiDiff;

                if (m_uiPhase == PHASE_START && m_creature->GetHealthPercent() < 65.0f)
                {
                    m_uiPhase = PHASE_BREATH;

                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->MoveIdle();

                    DoScriptText(SAY_PHASE_2_TRANS, m_creature);

                    // sort of a hack, it is unclear how this really work but the values appear to be valid
                    m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
                    m_creature->AddSplineFlag(SPLINEFLAG_FLYING);

                    if (m_pPointData)
                        m_creature->GetMotionMaster()->MovePoint(m_pPointData->uiLocId, m_pPointData->fX, m_pPointData->fY, m_pPointData->fZ);

                    // TODO - this might not be the correct place to set this setting
                    if (m_pInstance)
                        m_pInstance->SetData(TYPE_ONYXIA, DATA_LIFTOFF);
                    return;
                }

                DoMeleeAttackIfReady();
                break;
            }
            case PHASE_BREATH:
            {
                if (m_creature->GetHealthPercent() < 40.0f)
                {
                    m_uiPhase = PHASE_END;
                    DoScriptText(SAY_PHASE_3_TRANS, m_creature);

                    // undo flying
                    m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, 0);
                    m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);

                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    return;
                }

                if (m_uiMovementTimer < uiDiff)
                {
                    m_uiMovementTimer = 25000;

                    // 3 possible actions
                    switch(urand(0, 2))
                    {
                        case 0:                             // breath
                            if (m_pPointData = GetMoveData())
                            {
                                DoScriptText(EMOTE_BREATH, m_creature);
                                DoCastSpellIfCan(m_creature, m_pPointData->uiSpellId, CAST_INTERRUPT_PREVIOUS);
                                m_uiMovePoint = m_pPointData->uiLocIdEnd;
                            }
                            return;
                        case 1:                             // a point on the left side
                        {
                            // C++ is stupid, so add -1 with +7
                            m_uiMovePoint += m_uiMaxBreathPositions - 1;
                            m_uiMovePoint %= m_uiMaxBreathPositions;
                            break;
                        }
                        case 2:                             // a point on the right side
                            ++m_uiMovePoint %= m_uiMaxBreathPositions;
                            break;
                    }

                    if (m_pPointData = GetMoveData())
                        m_creature->GetMotionMaster()->MovePoint(m_pPointData->uiLocId, m_pPointData->fX, m_pPointData->fY, m_pPointData->fZ);
                }
                else
                    m_uiMovementTimer -= uiDiff;

                if (m_uiFireballTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H) == CAST_OK)
                            m_uiFireballTimer = 8000;
                    }
                }
                else
                    m_uiFireballTimer -= uiDiff;            //engulfingflames is supposed to be activated by a fireball but haven't come by

                if (m_bIsSummoningWhelps)
                {
                    if (m_uiSummonCount < MAX_WHELPS_PER_PACK)
                    {
                        if (m_uiWhelpTimer < uiDiff)
                        {
                            m_creature->SummonCreature(NPC_ONYXIA_WHELP, afSpawnLocations[0][0], afSpawnLocations[0][1], afSpawnLocations[0][2], 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                            m_creature->SummonCreature(NPC_ONYXIA_WHELP, afSpawnLocations[1][0], afSpawnLocations[1][1], afSpawnLocations[1][2], 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                            m_uiWhelpTimer = 500;
                        }
                        else
                            m_uiWhelpTimer -= uiDiff;
                    }
                    else
                    {
                        m_bIsSummoningWhelps = false;
                        m_uiSummonCount = 0;
                        m_uiSummonWhelpsTimer = 80000;      // 90s -10s for summoning
                    }
                }
                else
                {
                    if (m_uiSummonWhelpsTimer < uiDiff)
                        m_bIsSummoningWhelps = true;
                    else
                        m_uiSummonWhelpsTimer -= uiDiff;
                }

                if (m_uiSummonGuardTimer < uiDiff)
                {
                    if (!m_creature->IsNonMeleeSpellCasted(false))
                    {
                        m_creature->CastSpell(afSpawnLocations[2][0], afSpawnLocations[2][1], afSpawnLocations[2][2], SPELL_SUMMON_LAIR_GUARD, true);
                        m_uiSummonGuardTimer = 30000;
                    }
                }
                else
                    m_uiSummonGuardTimer -= uiDiff;

                break;
            }
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        // Check if players are hit by Onyxia's Deep Breath
        if (pTarget->GetTypeId() != TYPEID_PLAYER || !m_pInstance)
            return;

        // All and only the Onyxia Deep Breath Spells have these visuals
        if (pSpell->SpellVisual[0] == SPELL_VISUAL_BREATH_A || pSpell->SpellVisual[0] == SPELL_VISUAL_BREATH_B)
            m_pInstance->SetData(TYPE_ONYXIA, DATA_PLAYER_TOASTED);
    }
};

CreatureAI* GetAI_boss_onyxia(Creature* pCreature)
{
    return new boss_onyxiaAI(pCreature);
}

void AddSC_boss_onyxia()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_onyxia";
    pNewScript->GetAI = &GetAI_boss_onyxia;
    pNewScript->RegisterSelf();
}

/*
-- SPELL_BREATH_EAST_TO_WEST
DELETE FROM spell_target_position WHERE id IN (18576, 18578, 18579, 18580, 18581, 18582, 18583);
INSERT INTO spell_target_position VALUES (18576, 249, -37.743851, -243.667923, -88.217651, 1.416);
INSERT INTO spell_target_position VALUES (18578, 249, -35.805332, -232.028900, -87.749153, 1.416);
INSERT INTO spell_target_position VALUES (18579, 249, -34.045738, -224.714661, -85.529465, 1.416);
INSERT INTO spell_target_position VALUES (18580, 249, -32.081570, -214.916962, -88.327438, 1.416);
INSERT INTO spell_target_position VALUES (18581, 249, -36.611721, -202.684677, -85.653786, 1.416);
INSERT INTO spell_target_position VALUES (18582, 249, -37.067261, -195.758652, -87.745834, 1.416);
INSERT INTO spell_target_position VALUES (18583, 249, -37.728523, -188.616806, -88.074898, 1.416);
-- SPELL_BREATH_WEST_TO_EAST
DELETE FROM spell_target_position WHERE id IN (18609, 18611, 18612, 18613, 18614, 18615, 18616);
INSERT INTO spell_target_position VALUES (18609, 249, -37.728523, -188.616806, -88.074898, 4.526);
INSERT INTO spell_target_position VALUES (18611, 249, -37.067261, -195.758652, -87.745834, 4.526);
INSERT INTO spell_target_position VALUES (18612, 249, -36.611721, -202.684677, -85.653786, 4.526);
INSERT INTO spell_target_position VALUES (18613, 249, -32.081570, -214.916962, -88.327438, 4.526);
INSERT INTO spell_target_position VALUES (18614, 249, -34.045738, -224.714661, -85.529465, 4.526);
INSERT INTO spell_target_position VALUES (18615, 249, -35.805332, -232.028900, -87.749153, 4.526);
INSERT INTO spell_target_position VALUES (18616, 249, -37.743851, -243.667923, -88.217651, 4.526);
-- SPELL_BREATH_NW_TO_SE
DELETE FROM spell_target_position WHERE id IN (18584, 18585, 18586, 18587, 18588, 18589, 18590, 18591, 18592, 18593, 18594, 18595);
INSERT INTO spell_target_position VALUES (18584, 249, 6.016711, -181.305771, -85.654648, 3.776);
INSERT INTO spell_target_position VALUES (18585, 249, 3.860220, -183.227249, -86.375381, 3.776);
INSERT INTO spell_target_position VALUES (18586, 249, -2.529650, -188.690491, -87.172859, 3.776);
INSERT INTO spell_target_position VALUES (18587, 249, -8.449303, -193.957962, -87.564957, 3.776);
INSERT INTO spell_target_position VALUES (18588, 249, -14.321238, -199.462219, -87.922478, 3.776);
INSERT INTO spell_target_position VALUES (18589, 249, -15.602085, -216.893936, -88.403183, 3.776);
INSERT INTO spell_target_position VALUES (18590, 249, -23.650263, -221.969086, -89.172699, 3.776);
INSERT INTO spell_target_position VALUES (18591, 249, -29.495876, -213.014359, -88.910423, 3.776);
INSERT INTO spell_target_position VALUES (18592, 249, -35.439922, -217.260284, -87.336311, 3.776);
INSERT INTO spell_target_position VALUES (18593, 249, -41.762104, -221.896545, -86.114113, 3.776);
INSERT INTO spell_target_position VALUES (18594, 249, -51.067528, -228.909988, -85.765556, 3.776);
INSERT INTO spell_target_position VALUES (18595, 249, -56.559654, -241.223923, -85.423607, 3.776);
-- SPELL_BREATH_SE_TO_NW
DELETE FROM spell_target_position WHERE id IN (18564, 18565, 18566, 18567, 18568, 18569, 18570, 18571, 18572, 18573, 18574, 18575);
INSERT INTO spell_target_position VALUES (18564, 249, -56.559654, -241.223923, -85.423607, 0.666);
INSERT INTO spell_target_position VALUES (18565, 249, -51.067528, -228.909988, -85.765556, 0.666);
INSERT INTO spell_target_position VALUES (18566, 249, -41.762104, -221.896545, -86.114113, 0.666);
INSERT INTO spell_target_position VALUES (18567, 249, -35.439922, -217.260284, -87.336311, 0.666);
INSERT INTO spell_target_position VALUES (18568, 249, -29.495876, -213.014359, -88.910423, 0.666);
INSERT INTO spell_target_position VALUES (18569, 249, -23.650263, -221.969086, -89.172699, 0.666);
INSERT INTO spell_target_position VALUES (18570, 249, -15.602085, -216.893936, -88.403183, 0.666);
INSERT INTO spell_target_position VALUES (18571, 249, -14.321238, -199.462219, -87.922478, 0.666);
INSERT INTO spell_target_position VALUES (18572, 249, -8.449303, -193.957962, -87.564957, 0.666);
INSERT INTO spell_target_position VALUES (18573, 249, -2.529650, -188.690491, -87.172859, 0.666);
INSERT INTO spell_target_position VALUES (18574, 249, 3.860220, -183.227249, -86.375381, 0.666);
INSERT INTO spell_target_position VALUES (18575, 249, 6.016711, -181.305771, -85.654648, 0.666);
-- SPELL_BREATH_SW_TO_NE
DELETE FROM spell_target_position WHERE id IN (18596, 18597, 18598, 18599, 18600, 18601, 18602, 18603, 18604, 18605, 18606, 18607);
INSERT INTO spell_target_position VALUES (18596, 249, -58.250900, -189.020004, -85.292267, 5.587);
INSERT INTO spell_target_position VALUES (18597, 249, -52.006271, -193.796570, -85.808769, 5.587);
INSERT INTO spell_target_position VALUES (18598, 249, -46.135464, -198.548553, -85.901764, 5.587);
INSERT INTO spell_target_position VALUES (18599, 249, -40.500187, -203.001053, -85.555107, 5.587);
INSERT INTO spell_target_position VALUES (18600, 249, -30.907579, -211.058197, -88.592125, 5.587);
INSERT INTO spell_target_position VALUES (18601, 249, -20.098139, -218.681427, -88.937088, 5.587);
INSERT INTO spell_target_position VALUES (18602, 249, -12.223192, -224.666168, -87.856300, 5.587);
INSERT INTO spell_target_position VALUES (18603, 249, -6.475297, -229.098724, -87.076401, 5.587);
INSERT INTO spell_target_position VALUES (18604, 249, -2.010256, -232.541992, -86.995140, 5.587);
INSERT INTO spell_target_position VALUES (18605, 249, 2.736300, -236.202347, -86.790367, 5.587);
INSERT INTO spell_target_position VALUES (18606, 249, 7.197779, -239.642868, -86.307297, 5.587);
INSERT INTO spell_target_position VALUES (18607, 249, 12.120926, -243.439407, -85.874260, 5.587);
-- SPELL_BREATH_NE_TO_SW
DELETE FROM spell_target_position WHERE id IN (18617, 18619, 18620, 18621, 18622, 18623, 18624, 18625, 18626, 18627, 18628, 18618);
INSERT INTO spell_target_position VALUES (18617, 249, 12.120926, -243.439407, -85.874260, 2.428);
INSERT INTO spell_target_position VALUES (18619, 249, 7.197779, -239.642868, -86.307297, 2.428);
INSERT INTO spell_target_position VALUES (18620, 249, 2.736300, -236.202347, -86.790367, 2.428);
INSERT INTO spell_target_position VALUES (18621, 249, -2.010256, -232.541992, -86.995140, 2.428);
INSERT INTO spell_target_position VALUES (18622, 249, -6.475297, -229.098724, -87.076401, 2.428);
INSERT INTO spell_target_position VALUES (18623, 249, -12.223192, -224.666168, -87.856300, 2.428);
INSERT INTO spell_target_position VALUES (18624, 249, -20.098139, -218.681427, -88.937088, 2.428);
INSERT INTO spell_target_position VALUES (18625, 249, -30.907579, -211.058197, -88.592125, 2.428);
INSERT INTO spell_target_position VALUES (18626, 249, -40.500187, -203.001053, -85.555107, 2.428);
INSERT INTO spell_target_position VALUES (18627, 249, -46.135464, -198.548553, -85.901764, 2.428);
INSERT INTO spell_target_position VALUES (18628, 249, -52.006271, -193.796570, -85.808769, 2.428);
INSERT INTO spell_target_position VALUES (18618, 249, -58.250900, -189.020004, -85.292267, 2.428);

-- SPELL_BREATH_SOUTH_TO_NORTH
DELETE FROM spell_target_position WHERE id IN (18351, 18352, 18353, 18354, 18355, 18356, 18357, 18358, 18359, 18360, 18361);
INSERT INTO spell_target_position VALUES (18351, 249, -68.834236, -215.036163, -84.018875, 6.280);
INSERT INTO spell_target_position VALUES (18352, 249, -61.834255, -215.051910, -84.673416, 6.280);
INSERT INTO spell_target_position VALUES (18353, 249, -53.343277, -215.071014, -85.597191, 6.280);
INSERT INTO spell_target_position VALUES (18354, 249, -42.619305, -215.095139, -86.663605, 6.280);
INSERT INTO spell_target_position VALUES (18355, 249, -35.899323, -215.110245, -87.196548, 6.280);
INSERT INTO spell_target_position VALUES (18356, 249, -28.248341, -215.127457, -89.191750, 6.280);
INSERT INTO spell_target_position VALUES (18357, 249, -20.324360, -215.145279, -88.963997, 6.280);
INSERT INTO spell_target_position VALUES (18358, 249, -11.189384, -215.165833, -87.817093, 6.280);
INSERT INTO spell_target_position VALUES (18359, 249, -2.047405, -215.186386, -86.279655, 6.280);
INSERT INTO spell_target_position VALUES (18360, 249, 7.479571, -215.207809, -86.075531, 6.280);
INSERT INTO spell_target_position VALUES (18361, 249, 20.730539, -215.237610, -85.254387, 6.280);
-- SPELL_BREATH_NORTH_TO_SOUTH
DELETE FROM spell_target_position WHERE id IN (17086, 17087, 17088, 17089, 17090, 17091, 17092, 17093, 17094, 17095, 17097, 22267, 22268, 21132, 21133, 21135, 21136, 21137, 21138, 21139);
INSERT INTO spell_target_position VALUES (17086, 249, 20.730539, -215.237610, -85.254387, 3.142);
INSERT INTO spell_target_position VALUES (17087, 249, 7.479571, -215.207809, -86.075531, 3.142);
INSERT INTO spell_target_position VALUES (17088, 249, -2.047405, -215.186386, -86.279655, 3.142);
INSERT INTO spell_target_position VALUES (17089, 249, -11.189384, -215.165833, -87.817093, 3.142);
INSERT INTO spell_target_position VALUES (17090, 249, -20.324360, -215.145279, -88.963997, 3.142);
INSERT INTO spell_target_position VALUES (17091, 249, -28.248341, -215.127457, -89.191750, 3.142);
INSERT INTO spell_target_position VALUES (17092, 249, -35.899323, -215.110245, -87.196548, 3.142);
INSERT INTO spell_target_position VALUES (17093, 249, -42.619305, -215.095139, -86.663605, 3.142);
INSERT INTO spell_target_position VALUES (17094, 249, -53.343277, -215.071014, -85.597191, 3.142);
INSERT INTO spell_target_position VALUES (17095, 249, -61.834255, -215.051910, -84.673416, 3.142);
INSERT INTO spell_target_position VALUES (17097, 249, -68.834236, -215.036163, -84.018875, 3.142);
INSERT INTO spell_target_position VALUES (22267, 249, -75.736046, -214.984970, -83.394188, 3.142);
INSERT INTO spell_target_position VALUES (22268, 249, -84.087578, -214.857834, -82.640053, 3.142);
INSERT INTO spell_target_position VALUES (21132, 249, -90.424416, -214.601974, -82.482697, 3.142);
INSERT INTO spell_target_position VALUES (21133, 249, -96.572411, -214.353745, -82.239967, 3.142);
INSERT INTO spell_target_position VALUES (21135, 249, -102.069931, -214.131775, -80.571190, 3.142);
INSERT INTO spell_target_position VALUES (21136, 249, -107.385597, -213.917145, -77.447037, 3.142);
INSERT INTO spell_target_position VALUES (21137, 249, -114.281258, -213.866486, -73.851128, 3.142);
INSERT INTO spell_target_position VALUES (21138, 249, -123.328560, -213.607910, -71.559921, 3.142);
INSERT INTO spell_target_position VALUES (21139, 249, -130.788300, -213.424026, -70.751007, 3.142);
*/
