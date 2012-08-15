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
SDName: boss_soccothrates
SD%Complete: 20
SDComment: Basic script
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

#include "precompiled.h"
#include "arcatraz.h"

enum
{
    // Intro yells
    SAY_SOCCOTHRATES_INTRO_1        = -1552049,
    SAY_DALLIAH_INTRO_2             = -1552050,
    SAY_SOCCOTHRATES_INTRO_3        = -1552051,
    SAY_DALLIAH_INTRO_4             = -1552052,
    SAY_SOCCOTHRATES_INTRO_5        = -1552053,
    SAY_DALLIAH_INTRO_6             = -1552054,
    SAY_SOCCOTHRATES_INTRO_7        = -1552055,

    SAY_AGGRO                       = -1552048,
    SAY_KILL                        = -1552047,
    SAY_DEATH                       = -1552046,
    SAY_CHARGE_1                    = -1552044,
    SAY_CHARGE_2                    = -1552045,

    SPELL_IMMOLATION                = 36051,
    SPELL_IMMOLATION_H              = 39007,
    SPELL_KNOCK_AWAY                = 36512,
    SPELL_FELFIRE_LINE_UP           = 35770,
    SPELL_FELFIRE_SHOCK             = 35759,
    SPELL_FELFIRE_SHOCK_H           = 39006,
};

struct MANGOS_DLL_DECL boss_soccothratesAI : public ScriptedAI
{
    boss_soccothratesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_IMMOLATION : SPELL_IMMOLATION_H);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SOCCOTHRATES, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pWho)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SOCCOTHRATES, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SOCCOTHRATES, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_soccothrates(Creature* pCreature)
{
    return new boss_soccothratesAI(pCreature);
}

void AddSC_boss_soccothrates()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_soccothrates";
    pNewScript->GetAI = &GetAI_boss_soccothrates;
    pNewScript->RegisterSelf();
}
