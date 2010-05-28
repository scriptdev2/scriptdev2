/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_The_Best
SD%Complete: 100
SDComment:
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_FLAMEBREAK     = 16785,
    SPELL_IMMOLATE       = 20294,
    SPELL_TERRIFYINGROAR = 14100
};

struct MANGOS_DLL_DECL boss_thebeastAI : public ScriptedAI
{
    boss_thebeastAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiFlamebreakTimer;
    uint32 m_uiImmolateTimer;
    uint32 m_uiTerrifyingRoarTimer;

    void Reset()
    {
        m_uiFlamebreakTimer     = 12000;
        m_uiImmolateTimer       = 3000;
        m_uiTerrifyingRoarTimer = 23000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Flamebreak
        if (m_uiFlamebreakTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FLAMEBREAK);
            m_uiFlamebreakTimer = 10000;
        }
        else
            m_uiFlamebreakTimer -= uiDiff;

        // Immolate
        if (m_uiImmolateTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_IMMOLATE);
            
            m_uiImmolateTimer = 8000;
        }
        else
            m_uiImmolateTimer -= uiDiff;

        // Terrifying Roar
        if (m_uiTerrifyingRoarTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_TERRIFYINGROAR);
            m_uiTerrifyingRoarTimer = 20000;
        }
        else
            m_uiTerrifyingRoarTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thebeast(Creature* pCreature)
{
    return new boss_thebeastAI(pCreature);
}

void AddSC_boss_thebeast()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_the_beast";
    newscript->GetAI = &GetAI_boss_thebeast;
    newscript->RegisterSelf();
}
