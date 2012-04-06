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
SDName: Boss_Blackheart_the_Inciter
SD%Complete: 60
SDComment: Incite Chaos needs further research and core support
SDCategory: Auchindoun, Shadow Labyrinth
EndScriptData */

#include "precompiled.h"
#include "shadow_labyrinth.h"

enum
{
    SPELL_INCITE_CHAOS      = 33676,                        // triggers 33684 on party members - needs core support
    SPELL_CHARGE            = 33709,
    SPELL_WAR_STOMP         = 33707,

    SAY_INTRO1              = -1555008,
    SAY_INTRO2              = -1555009,
    SAY_INTRO3              = -1555010,
    SAY_AGGRO1              = -1555011,
    SAY_AGGRO2              = -1555012,
    SAY_AGGRO3              = -1555013,
    SAY_SLAY1               = -1555014,
    SAY_SLAY2               = -1555015,
    SAY_HELP                = -1555016,
    SAY_DEATH               = -1555017,

    SAY2_INTRO1             = -1555018,
    SAY2_INTRO2             = -1555019,
    SAY2_INTRO3             = -1555020,
    SAY2_AGGRO1             = -1555021,
    SAY2_AGGRO2             = -1555022,
    SAY2_AGGRO3             = -1555023,
    SAY2_SLAY1              = -1555024,
    SAY2_SLAY2              = -1555025,
    SAY2_HELP               = -1555026,
    SAY2_DEATH              = -1555027,
};

struct MANGOS_DLL_DECL boss_blackheart_the_inciterAI : public ScriptedAI
{
    boss_blackheart_the_inciterAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiInciteChaosTimer;
    uint32 m_uiChargeTimer;
    uint32 m_uiKnockbackTimer;

    void Reset()
    {
        m_uiInciteChaosTimer = 15000;
        m_uiChargeTimer      = 5000;
        m_uiKnockbackTimer   = 15000;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_INCITER, DONE);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_INCITER, IN_PROGRESS);
    }

    void JustReachedHome()
    {

        if (m_pInstance)
            m_pInstance->SetData(TYPE_INCITER, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no pTarget
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // ToDo: this needs future core and script support
        /*if (m_uiInciteChaosTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_INCITE_CHAOS) == CAST_OK)
                m_uiInciteChaosTimer = 40000;
        }
        else
            m_uiInciteChaosTimer -= uiDiff;*/

        // Charge Timer
        if (m_uiChargeTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget && DoCastSpellIfCan(pTarget, SPELL_CHARGE) == CAST_OK)
                m_uiChargeTimer = urand(15000, 25000);
        }
        else
            m_uiChargeTimer -= uiDiff;

        // Knockback Timer
        if (m_uiKnockbackTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_WAR_STOMP) == CAST_OK)
                m_uiKnockbackTimer = urand(18000, 24000);
        }
        else
            m_uiKnockbackTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_blackheart_the_inciter(Creature* pCreature)
{
    return new boss_blackheart_the_inciterAI(pCreature);
}

void AddSC_boss_blackheart_the_inciter()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_blackheart_the_inciter";
    pNewScript->GetAI = &GetAI_boss_blackheart_the_inciter;
    pNewScript->RegisterSelf();
}
