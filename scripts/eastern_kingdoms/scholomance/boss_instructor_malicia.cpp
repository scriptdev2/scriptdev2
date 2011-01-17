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
SDName: Boss_instructormalicia
SD%Complete: 80
SDComment: Spell IDs need confirmation, Slow is missing, Verify mechanics
SDCategory: Scholomance
EndScriptData */

#include "precompiled.h"
#include "scholomance.h"

enum
{
    SPELL_CALLOFGRAVES          = 17831,
    SPELL_CORRUPTION            = 18376,                    // old spell id = 11672,
    SPELL_FLASHHEAL             = 17843,                    // old spell id = 10917,
    SPELL_RENEW                 = 8362,                     // old spell id = 10929,
    SPELL_HEALINGTOUCH          = 15586,                    // old spell id = 9889
    SPELL_SLOW                  = 13747,                    // TODO Unused
};

struct MANGOS_DLL_DECL boss_instructormaliciaAI : public ScriptedAI
{
    boss_instructormaliciaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiCallOfGravesTimer;
    uint32 m_uiCorruptionTimer;
    uint32 m_uiFlashHealTimer;
    uint32 m_uiRenewTimer;
    uint32 m_uiHealingTouchTimer;
    uint32 m_uiFlashCounter;
    uint32 m_uiTouchCounter;

    void Reset()
    {
        m_uiCallOfGravesTimer = 4000;
        m_uiCorruptionTimer = 8000;
        m_uiFlashHealTimer = 38000;
        m_uiRenewTimer = 32000;
        m_uiHealingTouchTimer = 45000;
        m_uiFlashCounter = 0;
        m_uiTouchCounter = 0;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALICIA, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALICIA, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALICIA, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Call Of Graves Timer
        if (m_uiCallOfGravesTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_CALLOFGRAVES) == CAST_OK)
                m_uiCallOfGravesTimer = 65000;
        }
        else
            m_uiCallOfGravesTimer -= uiDiff;

        // Corruption Timer
        if (m_uiCorruptionTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_CORRUPTION) == CAST_OK)
                    m_uiCorruptionTimer = 24000;
            }
        }
        else
            m_uiCorruptionTimer -= uiDiff;

        // Renew Timer
        if (m_uiRenewTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_RENEW);
            m_uiRenewTimer = 10000;
        }
        else
            m_uiRenewTimer -= uiDiff;

        // Flash Heal Timer
        if (m_uiFlashHealTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FLASHHEAL);

            // TODO - 5 or 2?
            // 5 Flashheals will be casted
            if (m_uiFlashCounter < 2)
            {
                m_uiFlashHealTimer = 5000;
                ++m_uiFlashCounter;
            }
            else
            {
                m_uiFlashCounter = 0;
                m_uiFlashHealTimer = 30000;
            }
        }
        else
            m_uiFlashHealTimer -= uiDiff;

        // Healing Touch Timer
        if (m_uiHealingTouchTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_HEALINGTOUCH);

            // TODO - 3 or 2?
            // 3 Healingtouchs will be casted
            if (m_uiHealingTouchTimer < 2)
            {
                m_uiHealingTouchTimer = 5500;
                ++m_uiTouchCounter;
            }
            else
            {
                m_uiTouchCounter = 0;
                m_uiHealingTouchTimer = 30000;
            }
        }
        else
            m_uiHealingTouchTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_instructormalicia(Creature* pCreature)
{
    return new boss_instructormaliciaAI(pCreature);
}

void AddSC_boss_instructormalicia()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_instructor_malicia";
    pNewScript->GetAI = &GetAI_boss_instructormalicia;
    pNewScript->RegisterSelf();
}
