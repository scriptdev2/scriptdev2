/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: boss_varos
SD%Complete: 20
SDComment: Basic script
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

enum
{
    SAY_AGGRO                   = -1578020,
    SAY_CALL_CAPTAIN_1          = -1578021,
    SAY_CALL_CAPTAIN_2          = -1578022,
    SAY_CALL_CAPTAIN_3          = -1578023,
    SAY_KILL_1                  = -1578026,
    SAY_KILL_2                  = -1578027,
    SAY_DEATH                   = -1578028,
    EMOTE_CAPTAIN               = -1578029,

    // spells
    SPELL_CENTRIFUGE_SHIELD     = 50053,
    SPELL_AMPLIFY_MAGIC         = 51054,
    SPELL_AMPLIFY_MAGIC_H       = 59371,
    SPELL_ENERGIZE_CORES        = 50785,
    SPELL_ENERGIZE_CORES_H      = 59372,
    SPELL_CALL_CAPTAIN_1        = 51008,                // sends event 18455
    SPELL_CALL_CAPTAIN_2        = 51002,                // sends event 12229
    SPELL_CALL_CAPTAIN_3        = 51006,                // sends event 10665
    SPELL_CALL_CAPTAIN_4        = 51007,                // sends event 18454
};

struct MANGOS_DLL_DECL boss_varosAI : public ScriptedAI
{
    boss_varosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset() override
    {
    }

    void Aggro(Unit* pWho) override
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VAROS, IN_PROGRESS);
    }

    void KilledUnit(Unit* /*pVictim*/) override
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(SAY_DEATH, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_DEATH_SPELL, CAST_TRIGGERED);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VAROS, DONE);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VAROS, FAIL);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_varos(Creature* pCreature)
{
    return new boss_varosAI(pCreature);
}

void AddSC_boss_varos()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_varos";
    pNewScript->GetAI = &GetAI_boss_varos;
    pNewScript->RegisterSelf();
}
