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
SDName: Boss_Baron_Rivendare
SD%Complete: 70
SDComment: aura applied/defined in database
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"
#include "stratholme.h"

enum
{
    SPELL_SHADOW_BOLT   = 17393,
    SPELL_CLEAVE        = 15284,
    SPELL_MORTAL_STRIKE = 15708,

    SPELL_RAISE_DEAD    = 17473,                           //triggers death pact (17471)

    SPELL_RAISE_DEAD_1  = 17475,
    SPELL_RAISE_DEAD_2  = 17476,
    SPELL_RAISE_DEAD_3  = 17477,
    SPELL_RAISE_DEAD_4  = 17478,
    SPELL_RAISE_DEAD_5  = 17479,
    SPELL_RAISE_DEAD_6  = 17480
};

struct MANGOS_DLL_DECL boss_baron_rivendareAI : public ScriptedAI
{
    boss_baron_rivendareAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShadowBoltTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiMortalStrikeTimer;
    uint32 m_uiRaiseDeadTimer;

    void Reset()
    {
        m_uiShadowBoltTimer     = 5000;
        m_uiCleaveTimer         = 8000;
        m_uiMortalStrikeTimer   = 12000;
        m_uiRaiseDeadTimer      = 30000;
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void SpellHit(Unit* pWho, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_RAISE_DEAD)
        {
            DoCastSpellIfCan(m_creature, SPELL_RAISE_DEAD_1, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_RAISE_DEAD_2, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_RAISE_DEAD_3, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_RAISE_DEAD_4, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_RAISE_DEAD_5, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_RAISE_DEAD_6, CAST_TRIGGERED);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // ShadowBolt
        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_BOLT) == CAST_OK)
                    m_uiShadowBoltTimer = 10000;
            }
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                m_uiCleaveTimer = urand(7000, 17000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // MortalStrike
        if (m_uiMortalStrikeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MORTAL_STRIKE) == CAST_OK)
                m_uiMortalStrikeTimer = urand(10000, 25000);
        }
        else
            m_uiMortalStrikeTimer -= uiDiff;

        // RaiseDead
        if (m_uiRaiseDeadTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_RAISE_DEAD) == CAST_OK)
                m_uiRaiseDeadTimer = 45000;
        }
        else
            m_uiRaiseDeadTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_baron_rivendare(Creature* pCreature)
{
    return new boss_baron_rivendareAI(pCreature);
}

void AddSC_boss_baron_rivendare()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_baron_rivendare";
    pNewScript->GetAI = &GetAI_boss_baron_rivendare;
    pNewScript->RegisterSelf();
}
