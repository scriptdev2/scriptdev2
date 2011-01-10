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
SDName: Boss_Sartura
SD%Complete: 85
SDComment: Targeting currently doesn't work as expected
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO                   = -1531008,
    SAY_SLAY                    = -1531009,
    SAY_DEATH                   = -1531010,

    SPELL_WHIRLWIND             = 26083,
    SPELL_ENRAGE                = 28747,                    // Not sure if right ID.
    SPELL_ENRAGEHARD            = 28798,

    // Guard Spell
    SPELL_WHIRLWIND_ADD         = 26038,
    SPELL_KNOCKBACK             = 26027,
};

struct MANGOS_DLL_DECL boss_sarturaAI : public ScriptedAI
{
    boss_sarturaAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiWhirlWindTimer;
    uint32 m_uiWhirlWindRandomTimer;
    uint32 m_uiWhirlWindEndTimer;
    uint32 m_uiAggroResetTimer;
    uint32 m_uiAggroResetEndTimer;
    uint32 m_uiEnrageHardTimer;

    bool m_bIsEnraged;
    bool m_bIsEnragedHard;
    bool m_bIsWhirlWind;
    bool m_bAggroReset;

    void Reset()
    {
        m_uiWhirlWindTimer = 30000;
        m_uiWhirlWindRandomTimer = urand(3000, 7000);
        m_uiWhirlWindEndTimer = 15000;
        m_uiAggroResetTimer = urand(45000, 55000);
        m_uiAggroResetEndTimer = 5000;
        m_uiEnrageHardTimer = 10*60000;

        m_bIsWhirlWind = false;
        m_bAggroReset = false;
        m_bIsEnraged = false;
        m_bIsEnragedHard = false;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsWhirlWind)
        {
            // While whirlwind, switch to random targets often
            if (m_uiWhirlWindRandomTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                {
                    m_creature->AddThreat(pTarget);
                    m_creature->TauntApply(pTarget);
                    AttackStart(pTarget);
                }

                m_uiWhirlWindRandomTimer = urand(3000, 7000);
            }
            else
                m_uiWhirlWindRandomTimer -= uiDiff;

            // End Whirlwind Phase
            if (m_uiWhirlWindEndTimer < uiDiff)
                m_bIsWhirlWind = false;
            else
                m_uiWhirlWindEndTimer -= uiDiff;
        }
        else // if (!m_bIsWhirlWind)
        {
            // Enter Whirlwind Phase
            if (m_uiWhirlWindTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND) == CAST_OK)
                {
                    m_bIsWhirlWind = true;
                    m_uiWhirlWindEndTimer = 15000;
                    m_uiWhirlWindTimer = urand(25000, 40000);
                }
            }
            else
                m_uiWhirlWindTimer -= uiDiff;

            // Aquire a new target sometimes
            if (m_uiAggroResetTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                {
                    m_creature->AddThreat(pTarget);
                    m_creature->TauntApply(pTarget);
                    AttackStart(pTarget);
                }

                m_bAggroReset = true;
                m_uiAggroResetTimer = urand(2000, 5000);
            }
            else
                m_uiAggroResetTimer -= uiDiff;

            // Remove remaining taunts, TODO
            if (m_bAggroReset)
            {
                if (m_uiAggroResetEndTimer < uiDiff)
                {
                    m_bAggroReset = false;
                    m_uiAggroResetEndTimer = 5000;
                    m_uiAggroResetTimer = urand(35000, 45000);
                }
                else
                    m_uiAggroResetEndTimer -= uiDiff;
            }
        }

        // If she is 20% enrage
        if (!m_bIsEnraged && m_creature->GetHealthPercent() <= 20.0f)
		{
			if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE, m_bIsWhirlWind ? CAST_TRIGGERED : 0) == CAST_OK)
                m_bIsEnraged = true;
        }

        // After 10 minutes hard enrage
        if (!m_bIsEnragedHard)
        {
            if (m_uiEnrageHardTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ENRAGEHARD, m_bIsWhirlWind ? CAST_TRIGGERED : 0) == CAST_OK)
                    m_bIsEnragedHard = true;
            }
            else
                m_uiEnrageHardTimer -= uiDiff;
        }

        // No melee damage while in whirlwind
        if (!m_bIsWhirlWind)
            DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_sartura_royal_guardAI : public ScriptedAI
{
    mob_sartura_royal_guardAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiWhirlWindTimer;
    uint32 m_uiWhirlWindRandomTimer;
    uint32 m_uiWhirlWindEndTimer;
    uint32 m_uiAggroResetTimer;
    uint32 m_uiAggroResetEndTimer;
    uint32 m_uiKnockBackTimer;

    bool m_IsWhirlWind;
    bool m_bAggroReset;

    void Reset()
    {
        m_uiWhirlWindTimer = 30000;
        m_uiWhirlWindRandomTimer = urand(3000, 7000);
        m_uiWhirlWindEndTimer = 15000;
        m_uiAggroResetTimer = urand(45000, 55000);
        m_uiAggroResetEndTimer = 5000;
        m_uiKnockBackTimer = 10000;

        m_IsWhirlWind = false;
        m_bAggroReset = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_IsWhirlWind)
        {
            // While whirlwind, switch to random targets often
            if (m_uiWhirlWindRandomTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                {
                    m_creature->AddThreat(pTarget);
                    m_creature->TauntApply(pTarget);
                    AttackStart(pTarget);
                }

                m_uiWhirlWindRandomTimer = urand(3000, 7000);
            }
            else
                m_uiWhirlWindRandomTimer -= uiDiff;

            // End Whirlwind Phase
            if (m_uiWhirlWindEndTimer < uiDiff)
                m_IsWhirlWind = false;
            else
                m_uiWhirlWindEndTimer -= uiDiff;
        }
        else // if (!m_IsWhirlWind)
        {
            // Enter Whirlwind Phase
            if (m_uiWhirlWindTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND_ADD) == CAST_OK)
                {
                    m_IsWhirlWind = true;
                    m_uiWhirlWindEndTimer = 8000;
                    m_uiWhirlWindTimer = urand(25000, 40000);
                }
            }
            else
                m_uiWhirlWindTimer -= uiDiff;

            // Aquire a new target sometimes
            if (m_uiAggroResetTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                {
                    m_creature->AddThreat(pTarget);
                    m_creature->TauntApply(pTarget);
                    AttackStart(pTarget);
                }

                m_bAggroReset = true;
                m_uiAggroResetTimer = urand(2000, 5000);
            }
            else
                m_uiAggroResetTimer -= uiDiff;

            // Remove remaining taunts, TODO
            if (m_bAggroReset)
            {
                if (m_uiAggroResetEndTimer <uiDiff)
                {
                    m_bAggroReset = false;
                    m_uiAggroResetEndTimer = 5000;
                    m_uiAggroResetTimer = urand(30000, 40000);
                }
                else
                    m_uiAggroResetEndTimer -= uiDiff;
            }

            // Knockback nearby enemies
            if (m_uiKnockBackTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_KNOCKBACK) == CAST_OK)
                    m_uiKnockBackTimer = urand(10000, 20000);
            }
            else
                m_uiKnockBackTimer -= uiDiff;
        }

        // No melee damage while in whirlwind
        if (!m_IsWhirlWind)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sartura(Creature* pCreature)
{
    return new boss_sarturaAI(pCreature);
}

CreatureAI* GetAI_mob_sartura_royal_guard(Creature* pCreature)
{
    return new mob_sartura_royal_guardAI(pCreature);
}

void AddSC_boss_sartura()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_sartura";
    pNewScript->GetAI = &GetAI_boss_sartura;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_sartura_royal_guard";
    pNewScript->GetAI = &GetAI_mob_sartura_royal_guard;
    pNewScript->RegisterSelf();
}
