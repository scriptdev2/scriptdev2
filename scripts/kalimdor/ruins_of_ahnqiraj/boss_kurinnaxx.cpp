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
SDName: Boss_Kurinnaxx
SD%Complete: 100
SDComment: VERIFY SCRIPT AND SQL
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_TRASH        = 3391,
    SPELL_WIDE_SLASH   = 25814,
    SPELL_MORTAL_WOUND = 25646,
    SPELL_SANDTRAP     = 25656,
    SPELL_ENRAGE       = 28798
};

struct MANGOS_DLL_DECL boss_kurinnaxxAI : public ScriptedAI
{
    boss_kurinnaxxAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiMortalWoundTimer;
    uint32 m_uiSandTrapTimer;
    bool m_bEnraged;

    void Reset()
    {
        m_bEnraged = false;

        m_uiMortalWoundTimer = 30000;
        m_uiSandTrapTimer    = 30000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // If we are belowe 30% HP cast enrage
        if (!m_bEnraged && m_creature->GetHealthPercent() <= 30.0f)
        {
            m_bEnraged = true;
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ENRAGE);
        }

        // Mortal Wound
        if (m_uiMortalWoundTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MORTAL_WOUND);
            m_uiMortalWoundTimer = 30000;
        }
        else
            m_uiMortalWoundTimer -= uiDiff;

        // Sand Trap
        if (m_uiSandTrapTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SANDTRAP);
            m_uiSandTrapTimer = 30000;
        }
        else
            m_uiSandTrapTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kurinnaxx(Creature* pCreature)
{
    return new boss_kurinnaxxAI(pCreature);
}

void AddSC_boss_kurinnaxx()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_kurinnaxx";
    newscript->GetAI = &GetAI_boss_kurinnaxx;
    newscript->RegisterSelf();
}
