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
SDName: Boss_Grizzle
SD%Complete: 100
SDComment:
SDCategory: Blackrock Depths
EndScriptData */

#include "precompiled.h"

enum
{
    EMOTE_GENERIC_FRENZY_KILL   = -1000001,

    SPELL_GROUNDTREMOR          = 6524,
    SPELL_FRENZY                = 28371,
};

struct MANGOS_DLL_DECL boss_grizzleAI : public ScriptedAI
{
    boss_grizzleAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiGroundTremorTimer;
    uint32 m_uiFrenzyTimer;

    void Reset()
    {
        m_uiGroundTremorTimer = 12000;
        m_uiFrenzyTimer = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //GroundTremor_Timer
        if (m_uiGroundTremorTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_GROUNDTREMOR);
            m_uiGroundTremorTimer = 8000;
        }
        else
            m_uiGroundTremorTimer -= uiDiff;

        //Frenzy_Timer
        if (m_creature->GetHealthPercent() < 51.0f)
        {
            if (m_uiFrenzyTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FRENZY) == CAST_OK)
                {
                    DoScriptText(EMOTE_GENERIC_FRENZY_KILL, m_creature);
                    m_uiFrenzyTimer = 15000;
                }
            }
            else
                m_uiFrenzyTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_grizzle(Creature* pCreature)
{
    return new boss_grizzleAI(pCreature);
}

void AddSC_boss_grizzle()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_grizzle";
    pNewScript->GetAI = &GetAI_boss_grizzle;
    pNewScript->RegisterSelf();
}
