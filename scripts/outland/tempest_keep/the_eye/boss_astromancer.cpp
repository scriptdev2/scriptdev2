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
SDName: Boss_Astromancer
SD%Complete: 80
SDComment:
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "the_eye.h"

enum
{
    SAY_AGGRO                           = -1550007,
    SAY_SUMMON1                         = -1550008,
    SAY_SUMMON2                         = -1550009,
    SAY_KILL1                           = -1550010,
    SAY_KILL2                           = -1550011,
    SAY_KILL3                           = -1550012,
    SAY_DEATH                           = -1550013,
    SAY_VOIDA                           = -1550014,
    SAY_VOIDB                           = -1550015,

    SPELL_ARCANE_MISSILES               = 33031,
    SPELL_WRATH_OF_THE_ASTROMANCER      = 42783,
    SPELL_BLINDING_LIGHT                = 33009,
    SPELL_FEAR                          = 34322,
    SPELL_VOID_BOLT                     = 39329,

    SPELL_SPOTLIGHT                     = 25824,
    NPC_ASTROMANCER_SOLARIAN_SPOTLIGHT  = 18928,

    NPC_SOLARIUM_AGENT                  = 18925,
    NPC_SOLARIUM_PRIEST                 = 18806,

    MODEL_HUMAN                         = 18239,
    MODEL_VOIDWALKER                    = 18988,

    SPELL_SOLARIUM_GREAT_HEAL           = 33387,
    SPELL_SOLARIUM_HOLY_SMITE           = 25054,
    SPELL_SOLARIUM_ARCANE_TORRENT       = 33390,

    WV_ARMOR                            = 31000
};

const float CENTER_X                    = 432.909f;
const float CENTER_Y                    = -373.424f;
const float CENTER_Z                    = 17.9608f;
const float CENTER_O                    = 1.06421f;
const float SMALL_PORTAL_RADIUS         = 12.6f;
const float LARGE_PORTAL_RADIUS         = 26.0f;
const float PORTAL_Z                    = 17.005f;

enum Phases
{
    PHASE_NORMAL                        = 1,
    PHASE_SUMMON_AGENTS                 = 2,
    PHASE_SUMMON_PRIESTS                = 3,
    PHASE_VOID                          = 4,
};

struct MANGOS_DLL_DECL boss_high_astromancer_solarianAI : public ScriptedAI
{
    boss_high_astromancer_solarianAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

        m_uiDefaultArmor = m_creature->GetArmor();
        m_fDefaultSize = m_creature->GetFloatValue(OBJECT_FIELD_SCALE_X);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiArcaneMissilesTimer;
    uint32 m_uiWrathOfTheAstromancerTimer;
    uint32 m_uiBlindingLightTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiVoidBoltTimer;
    uint32 m_uiPhaseOneTimer;
    uint32 m_uiPhaseTwoTimer;
    uint32 m_uiPhaseThreeTimer;
    uint32 m_uiAppearDelayTimer;
    uint32 m_uiDefaultArmor;

    float m_fDefaultSize;

    bool m_bIsAppearDelay;

    Phases m_Phase;

    float m_aPortals[3][3];

    void Reset()
    {
        m_uiArcaneMissilesTimer = 2000;
        m_uiWrathOfTheAstromancerTimer = 15000;
        m_uiBlindingLightTimer = 41000;
        m_uiFearTimer = 20000;
        m_uiVoidBoltTimer = 10000;
        m_uiPhaseOneTimer = 50000;
        m_uiPhaseTwoTimer = 8000;
        m_uiPhaseThreeTimer = 15000;
        m_uiAppearDelayTimer = 2000;
        m_bIsAppearDelay = false;
        m_Phase = PHASE_NORMAL;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ASTROMANCER, NOT_STARTED);

        m_creature->SetArmor(m_uiDefaultArmor);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fDefaultSize);
        m_creature->SetDisplayId(MODEL_HUMAN);

        SetCombatMovement(true);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
            case 2: DoScriptText(SAY_KILL3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fDefaultSize);
        m_creature->SetDisplayId(MODEL_HUMAN);

        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ASTROMANCER, DONE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ASTROMANCER, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_ASTROMANCER_SOLARIAN_SPOTLIGHT:
                pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pSummoned->CastSpell(pSummoned, SPELL_SPOTLIGHT, false);
                break;
            case NPC_SOLARIUM_AGENT:
            case NPC_SOLARIUM_PRIEST:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pSummoned->AI()->AttackStart(pTarget);
                break;
        }
    }

    float Portal_X(float fRadius)
    {
        if (urand(0, 1))
            fRadius = -fRadius;

        return (fRadius * (float)(rand()%100)/100.0f + CENTER_X);
    }

    float Portal_Y(float fX, float fRadius)
    {
        float fZ = urand(0, 1) ? 1.0f : -1.0f;

        return (fZ * sqrt(fRadius*fRadius - (fX - CENTER_X)*(fX - CENTER_X)) + CENTER_Y);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // When Solarian reaches 20% she will transform into a huge void walker.
        if (m_Phase != PHASE_VOID && m_creature->GetHealthPercent() < 20.0f)
        {
            m_Phase = PHASE_VOID;

            // To make sure she behaves like expected
            m_bIsAppearDelay = false;
            if (!IsCombatMovement())
            {
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetVisibility(VISIBILITY_ON);

            DoScriptText(SAY_VOIDA, m_creature);
            DoScriptText(SAY_VOIDB, m_creature);

            m_creature->SetArmor(WV_ARMOR);
            m_creature->SetDisplayId(MODEL_VOIDWALKER);
            m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fDefaultSize*2.5f);
        }

        if (m_bIsAppearDelay)
        {
            if (m_uiAppearDelayTimer < uiDiff)
            {
                m_bIsAppearDelay = false;

                if (m_Phase == PHASE_SUMMON_AGENTS)
                {
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->SetVisibility(VISIBILITY_OFF);
                }
                // Let move and attack again
                else if (!IsCombatMovement())
                {
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                }

                m_uiAppearDelayTimer = 2000;
            }
            else
                m_uiAppearDelayTimer -= uiDiff;

            // Combat is still on hold
            return;
        }

        switch (m_Phase)
        {
            case PHASE_NORMAL:
                // Wrath of the Astromancer targets a random player which will explode after 6 secondes
                if (m_uiWrathOfTheAstromancerTimer < uiDiff)
                {
                    // Target the tank ?
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                    {
                        if (pTarget->GetTypeId() == TYPEID_PLAYER)
                        {
                            if (DoCastSpellIfCan(pTarget, SPELL_WRATH_OF_THE_ASTROMANCER, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                                m_uiWrathOfTheAstromancerTimer = 25000;
                        }
                        else
                            m_uiWrathOfTheAstromancerTimer = 1000;
                    }
                    else
                        m_uiWrathOfTheAstromancerTimer = 10000;
                }
                else
                    m_uiWrathOfTheAstromancerTimer -= uiDiff;

                // Blinding Light Timer
                if (m_uiBlindingLightTimer < uiDiff)
                {
                    // She casts this spell every 45 seconds. It is a kind of Moonfire spell, which she strikes down on the whole raid simultaneously. It hits everyone in the raid for 2280 to 2520 arcane damage.
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLINDING_LIGHT) == CAST_OK)
                        m_uiBlindingLightTimer = 45000;
                }
                else
                    m_uiBlindingLightTimer -= uiDiff;

                // Arcane Missiles Timer - TODO - check timer, if this spell is cast always, CombatMovement should be disabled here
                if (m_uiArcaneMissilesTimer < uiDiff)
                {
                    // Solarian casts Arcane Missiles on on random targets in the raid.
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (!m_creature->HasInArc(2.5f, pTarget))
                            pTarget = m_creature->getVictim();

                        if (pTarget)
                            DoCastSpellIfCan(pTarget, SPELL_ARCANE_MISSILES);
                    }

                    m_uiArcaneMissilesTimer = 5000;
                }
                else
                    m_uiArcaneMissilesTimer -= uiDiff;

                // Phase 1 Timer
                if (m_uiPhaseOneTimer < uiDiff)
                {
                    m_Phase = PHASE_SUMMON_AGENTS;

                    // After these 50 seconds she portals to the middle of the room and disappears, leaving 3 light portals behind.
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->StopMoving();
                    m_creature->AttackStop();

                    SetCombatMovement(false);

                    m_creature->NearTeleportTo(CENTER_X, CENTER_Y, CENTER_Z, CENTER_O, true);

                    for(int i = 0; i <= 2; ++i)
                    {
                        if (!i)
                        {
                            m_aPortals[i][0] = Portal_X(SMALL_PORTAL_RADIUS);
                            m_aPortals[i][1] = Portal_Y(m_aPortals[i][0], SMALL_PORTAL_RADIUS);
                            m_aPortals[i][2] = CENTER_Z;
                        }
                        else
                        {
                            m_aPortals[i][0] = Portal_X(LARGE_PORTAL_RADIUS);
                            m_aPortals[i][1] = Portal_Y(m_aPortals[i][0], LARGE_PORTAL_RADIUS);
                            m_aPortals[i][2] = PORTAL_Z;
                        }
                    }

                    if ((abs(int(m_aPortals[2][0]) - int(m_aPortals[1][0])) < 7) && (abs(int(m_aPortals[2][1]) - int(m_aPortals[1][1])) < 7))
                    {
                        int i = 1;
                        if (abs(int(CENTER_X) + int(26.0f) - int(m_aPortals[2][0])) < 7)
                            i = -1;

                        m_aPortals[2][0] = m_aPortals[2][0]+7*i;
                        m_aPortals[2][1] = Portal_Y(m_aPortals[2][0], LARGE_PORTAL_RADIUS);
                    }

                    for (int i = 0; i <= 2; ++i)
                        m_creature->SummonCreature(NPC_ASTROMANCER_SOLARIAN_SPOTLIGHT, m_aPortals[i][0], m_aPortals[i][1], m_aPortals[i][2], CENTER_O, TEMPSUMMON_TIMED_DESPAWN, m_uiPhaseTwoTimer+m_uiPhaseThreeTimer+m_uiAppearDelayTimer+1700);

                    m_bIsAppearDelay = true;
                    m_uiPhaseOneTimer = 48000;            //  2s for appearDelay

                    // Do nothing more, if phase switched
                    return;
                }
                else
                    m_uiPhaseOneTimer -= uiDiff;

                DoMeleeAttackIfReady();
                break;

            case PHASE_SUMMON_AGENTS:
                // Check Phase 2 Timer
                if (m_uiPhaseTwoTimer < uiDiff)
                {
                    //10 seconds after Solarian disappears, 12 mobs spawn out of the three portals.
                    m_Phase = PHASE_SUMMON_PRIESTS;
                    for (int i = 0; i <= 2; ++i)
                    {
                        for (int j = 1; j <= 4; ++j)
                            m_creature->SummonCreature(NPC_SOLARIUM_AGENT, m_aPortals[i][0], m_aPortals[i][1], m_aPortals[i][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                    }

                    DoScriptText(SAY_SUMMON1, m_creature);

                    m_uiPhaseTwoTimer = 8000;               //  2s for appearDelay
                }
                else
                    m_uiPhaseTwoTimer -= uiDiff;

                break;

            case PHASE_SUMMON_PRIESTS:
                // Check Phase 3 Timer
                if (m_uiPhaseThreeTimer < uiDiff)
                {
                    m_Phase = PHASE_NORMAL;

                    // 15 seconds later Solarian reappears out of one of the 3 portals. Simultaneously, 2 healers appear in the two other portals.
                    int i = irand(0, 2);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMap()->CreatureRelocation(m_creature, m_aPortals[i][0], m_aPortals[i][1], m_aPortals[i][2], CENTER_O);

                    for (int j = 0; j <= 2; ++j)
                        if (j != i)
                            m_creature->SummonCreature(NPC_SOLARIUM_PRIEST, m_aPortals[j][0], m_aPortals[j][1], m_aPortals[j][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);

                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->SetVisibility(VISIBILITY_ON);

                    DoScriptText(SAY_SUMMON2, m_creature);

                    m_bIsAppearDelay = true;
                    m_uiPhaseThreeTimer = 15000;
                }
                else
                    m_uiPhaseThreeTimer -= uiDiff;

                break;

            case PHASE_VOID:
                // Fear Timer
                if (m_uiFearTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_FEAR) == CAST_OK)
                        m_uiFearTimer = 20000;
                }
                else
                    m_uiFearTimer -= uiDiff;

                // Void Bolt Timer
                if (m_uiVoidBoltTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_VOID_BOLT) == CAST_OK)
                        m_uiVoidBoltTimer = 10000;
                }
                else
                    m_uiVoidBoltTimer -= uiDiff;

                DoMeleeAttackIfReady();
                break;
        }
    }
};

struct MANGOS_DLL_DECL mob_solarium_priestAI : public ScriptedAI
{
    mob_solarium_priestAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiHealTimer;
    uint32 m_uiHolySmiteTimer;
    uint32 m_uiAoESilenceTimer;

    void Reset()
    {
        m_uiHealTimer = 9000;
        m_uiHolySmiteTimer = 1;
        m_uiAoESilenceTimer = 15000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHealTimer < uiDiff)
        {
            Creature* pTarget = NULL;

            switch(urand(0, 1))
            {
                case 0:
                    if (m_pInstance)
                        pTarget = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ASTROMANCER));
                    break;
                case 1:
                    pTarget = m_creature;
                    break;
            }

            if (pTarget)
            {
                DoCastSpellIfCan(pTarget, SPELL_SOLARIUM_GREAT_HEAL);
                m_uiHealTimer = 9000;
            }
        }
        else
            m_uiHealTimer -= uiDiff;

        if (m_uiHolySmiteTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SOLARIUM_HOLY_SMITE);
            m_uiHolySmiteTimer = 4000;
        }
        else
            m_uiHolySmiteTimer -= uiDiff;

        if (m_uiAoESilenceTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SOLARIUM_ARCANE_TORRENT);
            m_uiAoESilenceTimer = 13000;
        }
        else
            m_uiAoESilenceTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_solarium_priest(Creature* pCreature)
{
    return new mob_solarium_priestAI(pCreature);
}

CreatureAI* GetAI_boss_high_astromancer_solarian(Creature* pCreature)
{
    return new boss_high_astromancer_solarianAI(pCreature);
}

void AddSC_boss_high_astromancer_solarian()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_high_astromancer_solarian";
    pNewScript->GetAI = &GetAI_boss_high_astromancer_solarian;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_solarium_priest";
    pNewScript->GetAI = &GetAI_mob_solarium_priest;
    pNewScript->RegisterSelf();
}
