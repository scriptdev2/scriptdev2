/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Magistrate_Barthilas
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_DRAINING_BLOW     = 16793,
    SPELL_CROWD_PUMMEL      = 10887,
    SPELL_MIGHTY_BLOW       = 14099,
    SPELL_FURIOUS_ANGER     = 16792,

    MODEL_HUMAN             = 3637
};

struct MANGOS_DLL_DECL boss_magistrate_barthilasAI : public ScriptedAI
{
    boss_magistrate_barthilasAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiDrainingBlowTimer;
    uint32 m_uiCrowdPummelTimer;
    uint32 m_uiMightyBlowTimer;

    void Reset()
    {
        m_uiDrainingBlowTimer = 20000;
        m_uiCrowdPummelTimer = 15000;
        m_uiMightyBlowTimer = 10000;
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_FURIOUS_ANGER);
    }

    void JustDied(Unit* pKiller)
    {
        m_creature->SetDisplayId(MODEL_HUMAN);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // DrainingBlow
        if (m_uiDrainingBlowTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DRAINING_BLOW) == CAST_OK)
                m_uiDrainingBlowTimer = 15000;
        }
        else
            m_uiDrainingBlowTimer -= uiDiff;

        // CrowdPummel
        if (m_uiCrowdPummelTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_CROWD_PUMMEL) == CAST_OK)
                m_uiCrowdPummelTimer = 15000;
        }
        else
            m_uiCrowdPummelTimer -= uiDiff;

        // MightyBlow
        if (m_uiMightyBlowTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_BLOW) == CAST_OK)
                m_uiMightyBlowTimer = 20000;
        }
        else
            m_uiMightyBlowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_magistrate_barthilas(Creature* pCreature)
{
    return new boss_magistrate_barthilasAI(pCreature);
}

void AddSC_boss_magistrate_barthilas()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_magistrate_barthilas";
    pNewScript->GetAI = &GetAI_boss_magistrate_barthilas;
    pNewScript->RegisterSelf();
}
