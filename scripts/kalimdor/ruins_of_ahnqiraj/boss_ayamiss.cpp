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
SDName: Boss_Ayamiss
SD%Complete: 50
SDComment: VERIFY SCRIPT
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"

/*
To do:
make him fly from 70-100%
*/

enum
{
    SPELL_STINGERSPRAY  = 25749,
    SPELL_POISONSTINGER = 25748,                            //only used in phase1
    SPELL_SUMMONSWARMER = 25844,                            //might be 25708
    //SPELL_PARALYZE 23414 doesnt work correct (core)

    PHASE_AIR           = 0,
    PHASE_GROUND        = 1
};

struct MANGOS_DLL_DECL boss_ayamissAI : public ScriptedAI
{
    boss_ayamissAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiStingerSprayTimer;
    uint32 m_uiPoisonStingerTimer;
    uint32 m_uiSummonSwarmerTimer;
    uint8 m_uiPhase;

    void Reset()
    {
        m_uiStingerSprayTimer  = 30000;
        m_uiPoisonStingerTimer = 30000;
        m_uiSummonSwarmerTimer = 60000;

        m_uiPhase = PHASE_AIR;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Stinger Spray
        if (m_uiStingerSprayTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_STINGERSPRAY);
            m_uiStingerSprayTimer = 30000;
        }
        else
            m_uiStingerSprayTimer -= uiDiff;

        if (m_uiPhase == PHASE_AIR)
        {
            // Start ground phase at 70% of HP
            if (m_creature->GetHealthPercent() <= 70.0f)
            {
                m_uiPhase = PHASE_GROUND;
                DoResetThreat();
            }

            // Poison Stinger
            if (m_uiPoisonStingerTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_POISONSTINGER);
                m_uiPoisonStingerTimer = 30000;
            }
            else
                m_uiPoisonStingerTimer -= uiDiff;
        }
        else
        {
            //m_uiSummonSwarmerTimer
            if (m_uiSummonSwarmerTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SUMMONSWARMER);
                m_uiSummonSwarmerTimer = 60000;
            }
            else
                m_uiSummonSwarmerTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};
CreatureAI* GetAI_boss_ayamiss(Creature* pCreature)
{
    return new boss_ayamissAI(pCreature);
}

void AddSC_boss_ayamiss()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_ayamiss";
    newscript->GetAI = &GetAI_boss_ayamiss;
    newscript->RegisterSelf();
}
