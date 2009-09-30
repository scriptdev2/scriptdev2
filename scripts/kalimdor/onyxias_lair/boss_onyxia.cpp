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

#define SAY_AGGRO                   -1249000
#define SAY_KILL                    -1249001
#define SAY_PHASE_2_TRANS           -1249002
#define SAY_PHASE_3_TRANS           -1249003
#define EMOTE_BREATH                -1249004

#define SPELL_WINGBUFFET            18500
#define SPELL_FLAMEBREATH           18435
#define SPELL_CLEAVE                19983
#define SPELL_TAILSWEEP             15847
#define SPELL_KNOCK_AWAY            19633

#define SPELL_ENGULFINGFLAMES       20019
#define SPELL_DEEPBREATH            23461
#define SPELL_FIREBALL              18392

#define SPELL_BELLOWINGROAR         18431
#define SPELL_HEATED_GROUND         22191

#define SPELL_SUMMONWHELP           17646

static float MovementLocations[7][3]=
{
    {-65.8444, -213.809, -60.2985},
    {22.87639, -217.152, -60.0548},
    {-33.5561, -182.682, -60.9457},
    {-31.4963, -250.123, -60.1278},
    {-2.78999, -181.431, -60.8962},
    {-54.9415, -232.242, -60.5555},
    {10.56655, -241.478, -60.9426},
};

static float SpawnLocations[4][3]=
{
    {-30.127, -254.463, -89.440},
    {-30.817, -177.106, -89.258},
    {14.480, -241.560, -85.6300},
    {17.372, -190.840, -85.2810},
};

#define CREATURE_WHELP              11262

struct MANGOS_DLL_DECL boss_onyxiaAI : public ScriptedAI
{
    boss_onyxiaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Phase;

    uint32 FlameBreathTimer;
    uint32 CleaveTimer;
    uint32 TailSweepTimer;
    uint32 MovementTimer;
    uint32 EngulfingFlamesTimer;
    uint32 SummonWhelpsTimer;
    uint32 BellowingRoarTimer;
    uint32 WingBuffetTimer;
    uint32 WhelpTimer;

    uint8 m_uiSummonCount;
    bool m_bIsSummoningWhelps;

    void Reset()
    {
        Phase = 1;

        FlameBreathTimer = 20000;
        TailSweepTimer = 2000;
        CleaveTimer = 15000;
        MovementTimer = 5000;
        EngulfingFlamesTimer = 15000;
        SummonWhelpsTimer = 45000;
        BellowingRoarTimer = 30000;
        WingBuffetTimer = 17000;
        WhelpTimer = 1000;

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

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 60) && (Phase == 1))
        {
            Phase = 2;
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            DoScriptText(SAY_PHASE_2_TRANS, m_creature);
        }

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 40) && (Phase == 2))
        {
            Phase = 3;
            DoScriptText(SAY_PHASE_3_TRANS, m_creature);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        }

        if (Phase == 1 || Phase == 3)
        {
            if (FlameBreathTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_FLAMEBREATH);
                FlameBreathTimer = 15000;
            }else FlameBreathTimer -= diff;

            if (TailSweepTimer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);
                if (target && !m_creature->HasInArc(M_PI, target))
                    DoCast(target, SPELL_TAILSWEEP);

                TailSweepTimer = 10000;
            }else TailSweepTimer -= diff;

            if (CleaveTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                CleaveTimer = 7000;
            }else CleaveTimer -= diff;

            if (WingBuffetTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_WINGBUFFET);
                WingBuffetTimer = urand(7000, 14000);
            }else WingBuffetTimer -= diff;

            DoMeleeAttackIfReady();
        }

        if (Phase == 2)
        {
            if (!m_creature->isHover())
            {
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                m_creature->SetHover(true);
            }

            if (!m_creature->GetMotionMaster()->empty() && (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE))
                m_creature->GetMotionMaster()->Clear(false);

            if (MovementTimer < diff)
            {
                uint32 random = rand()%8;
                if (random < 7)
                    m_creature->GetMotionMaster()->MovePoint(0, MovementLocations[random][0], MovementLocations[random][1], MovementLocations[random][2]);
                else
                {
                    DoScriptText(EMOTE_BREATH, m_creature);
                    DoCast(m_creature->getVictim(), SPELL_DEEPBREATH);
                }
                MovementTimer = 25000;
            }else MovementTimer -= diff;

            if (EngulfingFlamesTimer < diff)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_FIREBALL);

                EngulfingFlamesTimer = 8000;
            }else EngulfingFlamesTimer -= diff;             //engulfingflames is supposed to be activated by a fireball but haven't come by

            if (m_bIsSummoningWhelps)
            {
                if (m_uiSummonCount <= 14)
                {
                    if (WhelpTimer < diff)
                    {
                        m_creature->SummonCreature(CREATURE_WHELP, SpawnLocations[0][0], SpawnLocations[0][1], SpawnLocations[0][2], 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                        m_creature->SummonCreature(CREATURE_WHELP, SpawnLocations[1][0], SpawnLocations[1][1], SpawnLocations[1][2], 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                        WhelpTimer = 1000;
                    }
                    else
                        WhelpTimer -= diff;
                }
                else
                {
                    m_bIsSummoningWhelps = false;
                    m_uiSummonCount = 0;
                    SummonWhelpsTimer = 30000;
                }
            }
            else
            {
                if (SummonWhelpsTimer < diff)
                    m_bIsSummoningWhelps = true;
                else
                    SummonWhelpsTimer -= diff;
            }
        }

        if (Phase == 3)
        {
            if (BellowingRoarTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_BELLOWINGROAR);
                BellowingRoarTimer = 30000;
            }else BellowingRoarTimer -= diff;
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
