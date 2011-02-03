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
SDName: Boss_Jeklik
SD%Complete: 85
SDComment: Problem in finding the right flying batriders for spawning and making them fly.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    SAY_AGGRO                   = -1309002,
    SAY_RAIN_FIRE               = -1309003,
    SAY_DEATH                   = -1309004,

    SPELL_CHARGE                = 22911,
    SPELL_SONICBURST            = 23918,
    SPELL_SCREECH               = 6605,
    SPELL_SHADOW_WORD_PAIN      = 23952,
    SPELL_MIND_FLAY             = 23953,
    SPELL_CHAIN_MIND_FLAY       = 26044,                    // Right ID unknown. So disabled
    SPELL_GREATERHEAL           = 23954,
    SPELL_BAT_FORM              = 23966,

    // Batriders Spell
    SPELL_BOMB                  = 40332,                    // Wrong ID but Magmadars bomb is not working...

    NPC_BLOODSEEKER_BAT         = 11368,
    NPC_FRENZIED_BAT            = 14965,
};

struct MANGOS_DLL_DECL boss_jeklikAI : public ScriptedAI
{
    boss_jeklikAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiChargeTimer;
    uint32 m_uiSonicBurstTimer;
    uint32 m_uiScreechTimer;
    uint32 m_uiSpawnBatsTimer;
    uint32 m_uiShadowWordPainTimer;
    uint32 m_uiMindFlayTimer;
    uint32 m_uiChainMindFlayTimer;
    uint32 m_uiGreaterHealTimer;
    uint32 m_uiSpawnFlyingBatsTimer;

    bool m_bIsPhaseOne;

    void Reset()
    {
        m_uiChargeTimer = 20000;
        m_uiSonicBurstTimer = 8000;
        m_uiScreechTimer = 13000;
        m_uiSpawnBatsTimer = 60000;
        m_uiShadowWordPainTimer = 6000;
        m_uiMindFlayTimer = 11000;
        m_uiChainMindFlayTimer = 26000;
        m_uiGreaterHealTimer = 50000;
        m_uiSpawnFlyingBatsTimer = 10000;

        m_bIsPhaseOne = true;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_BAT_FORM);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_JEKLIK, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsPhaseOne)
        {
            // Phase Switch at 50%
            if (m_creature->GetHealthPercent() < 50.0f)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_BAT_FORM);
                DoResetThreat();
                m_bIsPhaseOne = false;
                return;
            }

            if (m_uiChargeTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    if (DoCastSpellIfCan(pTarget, SPELL_CHARGE) == CAST_OK)
                        m_uiChargeTimer = urand(15000, 30000);
            }
            else
                m_uiChargeTimer -= uiDiff;

            if (m_uiSonicBurstTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SONICBURST) == CAST_OK)
                    m_uiSonicBurstTimer = urand(8000, 13000);
            }
            else
                m_uiSonicBurstTimer -= uiDiff;

            if (m_uiScreechTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SCREECH) == CAST_OK)
                    m_uiScreechTimer = urand(18000, 26000);
            }
            else
                m_uiScreechTimer -= uiDiff;

            if (m_uiSpawnBatsTimer < uiDiff)
            {
                // TODO There are some bats in the cave behind the boss, perhaps they should be called
                float fX, fY, fZ, fO, fNewX, fNewY, fNewZ;
                m_creature->GetRespawnCoord(fX, fY, fZ, &fO);
                for (uint8 i = 0; i < 6; ++i)
                {
                    // Get a point a little bit behind Jeklik respawn pos
                    m_creature->GetRandomPoint(fX - 5.0f, fY + 5.0f, fZ, 5.0f, fNewX, fNewY, fNewZ);
                    m_creature->SummonCreature(NPC_BLOODSEEKER_BAT, fNewX, fNewY, fNewZ, fO, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                }

                m_uiSpawnBatsTimer = 60000;
            }
            else
                m_uiSpawnBatsTimer -= uiDiff;
        }
        else                                                // Phase Two
        {
            if (m_uiShadowWordPainTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_WORD_PAIN) == CAST_OK)
                        m_uiShadowWordPainTimer = urand(12000, 18000);
                }
            }
            else
                m_uiShadowWordPainTimer -= uiDiff;

            if (m_uiMindFlayTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIND_FLAY) == CAST_OK)
                    m_uiMindFlayTimer = 16000;
            }
            else
                m_uiMindFlayTimer -= uiDiff;

            if (m_uiChainMindFlayTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHAIN_MIND_FLAY) == CAST_OK)
                    m_uiChainMindFlayTimer = urand(15000, 30000);
            }
            else
                m_uiChainMindFlayTimer -= uiDiff;

            if (m_uiGreaterHealTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_GREATERHEAL, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                    m_uiGreaterHealTimer = urand(25000, 35000);
            }
            else
                m_uiGreaterHealTimer -= uiDiff;

            if (m_uiSpawnFlyingBatsTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->SummonCreature(NPC_FRENZIED_BAT, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ() + 15.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);

                m_uiSpawnFlyingBatsTimer = urand(10000, 15000);
            }
            else
                m_uiSpawnFlyingBatsTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

// Flying Bat
struct MANGOS_DLL_DECL mob_batriderAI : public ScriptedAI
{
    mob_batriderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiBombTimer;
    uint32 m_uiCheckTimer;

    void Reset()
    {
        m_uiBombTimer = 2000;
        m_uiCheckTimer = 1000;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void UpdateAI (const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Bomb Timer
        if (m_uiBombTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(pTarget, SPELL_BOMB);
                m_uiBombTimer = 5000;
            }
        }
        else
            m_uiBombTimer -= uiDiff;

        // Check Timer
        if (m_uiCheckTimer < uiDiff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_JEKLIK) == DONE)
                {
                    m_creature->SetDeathState(JUST_DIED);
                    m_creature->RemoveCorpse();
                    return;
                }
            }
            m_uiCheckTimer = 1000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jeklik(Creature* pCreature)
{
    return new boss_jeklikAI(pCreature);
}

CreatureAI* GetAI_mob_batrider(Creature* pCreature)
{
    return new mob_batriderAI(pCreature);
}

void AddSC_boss_jeklik()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_jeklik";
    pNewScript->GetAI = &GetAI_boss_jeklik;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_batrider";
    pNewScript->GetAI = &GetAI_mob_batrider;
    pNewScript->RegisterSelf();
}
