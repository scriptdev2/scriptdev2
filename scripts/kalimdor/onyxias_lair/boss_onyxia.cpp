/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SD%Complete: 50
SDComment: Phase 2 has many errors. Recommend Rewrite of entire script.
SDCategory: Onyxia's Lair
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO                   = -1249000,
    SAY_KILL                    = -1249001,
    SAY_PHASE_2_TRANS           = -1249002,
    SAY_PHASE_3_TRANS           = -1249003,
    EMOTE_BREATH                = -1249004,

    SPELL_WINGBUFFET            = 18500,
    SPELL_FLAMEBREATH           = 18435,
    SPELL_CLEAVE                = 19983,
    SPELL_TAILSWEEP             = 15847,
    SPELL_KNOCK_AWAY            = 19633,

    SPELL_ENGULFINGFLAMES       = 20019,
    SPELL_DEEPBREATH            = 23461,
    SPELL_FIREBALL              = 18392,

    SPELL_BELLOWINGROAR         = 18431,
    SPELL_HEATED_GROUND         = 22191,

    SPELL_SUMMONWHELP           = 17646,
    NPC_WHELP                   = 11262,
    MAX_WHELP                   = 16,

    PHASE_START                 = 1,
    PHASE_BREATH                = 2,
    PHASE_END                   = 3
};

static float afMovementLocations[7][3]=
{
    {-65.8444, -213.809, -60.2985},
    {22.87639, -217.152, -60.0548},
    {-33.5561, -182.682, -60.9457},
    {-31.4963, -250.123, -60.1278},
    {-2.78999, -181.431, -60.8962},
    {-54.9415, -232.242, -60.5555},
    {10.56655, -241.478, -60.9426},
};

static float afSpawnLocations[2][3]=
{
    {-30.127, -254.463, -89.440},
    {-30.817, -177.106, -89.258}
};

struct MANGOS_DLL_DECL boss_onyxiaAI : public ScriptedAI
{
    boss_onyxiaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiPhase;

    uint32 m_uiFlameBreathTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uiWingBuffetTimer;

    uint32 m_uiMovementTimer;

    uint32 m_uiEngulfingFlamesTimer;
    uint32 m_uiSummonWhelpsTimer;
    uint32 m_uiBellowingRoarTimer;
    uint32 m_uiWhelpTimer;

    uint8 m_uiSummonCount;
    bool m_bIsSummoningWhelps;

    void Reset()
    {
        m_uiPhase = PHASE_START;

        m_uiFlameBreathTimer = urand(10000, 20000);
        m_uiTailSweepTimer = urand(15000, 20000);
        m_uiCleaveTimer = urand(2000, 5000);
        m_uiWingBuffetTimer = urand(10000, 20000);

        m_uiMovementTimer = 5000;

        m_uiEngulfingFlamesTimer = 15000;
        m_uiSummonWhelpsTimer = 45000;
        m_uiBellowingRoarTimer = 30000;
        m_uiWhelpTimer = 1000;

        m_uiSummonCount = 0;
        m_bIsSummoningWhelps = false;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->SetInCombatWithZone();
    }

    void JustSummoned(Creature *pSummoned)
    {
        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
            pSummoned->AI()->AttackStart(pTarget);

        ++m_uiSummonCount;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_uiPhase == PHASE_START || m_uiPhase == PHASE_END)
        {
            if (m_uiFlameBreathTimer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_FLAMEBREATH);
                m_uiFlameBreathTimer = urand(10000, 20000);
            }
            else
                m_uiFlameBreathTimer -= uiDiff;

            if (m_uiTailSweepTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_TAILSWEEP);
                m_uiTailSweepTimer = urand(15000, 20000);
            }
            else
                m_uiTailSweepTimer -= uiDiff;

            if (m_uiCleaveTimer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = urand(2000, 5000);
            }
            else
                m_uiCleaveTimer -= uiDiff;

            if (m_uiWingBuffetTimer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_WINGBUFFET);
                m_uiWingBuffetTimer = urand(15000, 30000);
            }
            else
                m_uiWingBuffetTimer -= uiDiff;

            if (m_uiPhase == PHASE_END)
            {
                if (m_uiBellowingRoarTimer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), SPELL_BELLOWINGROAR);
                    m_uiBellowingRoarTimer = 30000;
                }
                else
                    m_uiBellowingRoarTimer -= uiDiff;
            }
            else
            {
                if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 60)
                {
                    m_uiPhase = PHASE_BREATH;

                    SetCombatMovement(false);

                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->MoveIdle();

                    DoScriptText(SAY_PHASE_2_TRANS, m_creature);

                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                }
            }

            DoMeleeAttackIfReady();
        }
        else
        {
            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 40)
            {
                m_uiPhase = PHASE_END;
                DoScriptText(SAY_PHASE_3_TRANS, m_creature);

                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());

                return;
            }

            if (m_uiMovementTimer < uiDiff)
            {
                uint32 uiRandom = urand(0, 7);

                if (uiRandom < 7)
                    m_creature->GetMotionMaster()->MovePoint(0, afMovementLocations[uiRandom][0], afMovementLocations[uiRandom][1], afMovementLocations[uiRandom][2]);
                else
                {
                    DoScriptText(EMOTE_BREATH, m_creature);
                    DoCast(m_creature->getVictim(), SPELL_DEEPBREATH);
                }

                m_uiMovementTimer = 25000;
            }
            else
                m_uiMovementTimer -= uiDiff;

            if (m_uiEngulfingFlamesTimer < uiDiff)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_FIREBALL);

                m_uiEngulfingFlamesTimer = 8000;
            }
            else
                m_uiEngulfingFlamesTimer -= uiDiff;           //engulfingflames is supposed to be activated by a fireball but haven't come by

            if (m_bIsSummoningWhelps)
            {
                if (m_uiSummonCount < MAX_WHELP)
                {
                    if (m_uiWhelpTimer < uiDiff)
                    {
                        m_creature->SummonCreature(NPC_WHELP, afSpawnLocations[0][0], afSpawnLocations[0][1], afSpawnLocations[0][2], 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                        m_creature->SummonCreature(NPC_WHELP, afSpawnLocations[1][0], afSpawnLocations[1][1], afSpawnLocations[1][2], 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                        m_uiWhelpTimer = 1000;
                    }
                    else
                        m_uiWhelpTimer -= uiDiff;
                }
                else
                {
                    m_bIsSummoningWhelps = false;
                    m_uiSummonCount = 0;
                    m_uiSummonWhelpsTimer = 30000;
                }
            }
            else
            {
                if (m_uiSummonWhelpsTimer < uiDiff)
                    m_bIsSummoningWhelps = true;
                else
                    m_uiSummonWhelpsTimer -= uiDiff;
            }
        }
    }
};

CreatureAI* GetAI_boss_onyxiaAI(Creature* pCreature)
{
    return new boss_onyxiaAI(pCreature);
}

void AddSC_boss_onyxia()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_onyxia";
    newscript->GetAI = &GetAI_boss_onyxiaAI;
    newscript->RegisterSelf();
}
