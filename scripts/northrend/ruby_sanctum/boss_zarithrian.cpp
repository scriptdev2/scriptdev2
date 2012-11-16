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
SDName: boss_zarithrian
SD%Complete: 20
SDComment: only texts
SDCategory: Ruby Sanctum
EndScriptData */

#include "precompiled.h"
#include "ruby_sanctum.h"

enum
{
    SAY_AGGRO           = -1724019,
    SAY_SLAY_1          = -1724020,
    SAY_SLAY_2          = -1724021,
    SAY_DEATH           = -1724022,
    SAY_SUMMON          = -1724023,
};

struct MANGOS_DLL_DECL boss_zarithrianAI : public ScriptedAI
{
    boss_zarithrianAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ruby_sanctum*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ruby_sanctum* m_pInstance;

    void Reset() override
    {
    }

    void Aggro(Unit* pWho) override
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZARITHRIAN, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller) override
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZARITHRIAN, DONE);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZARITHRIAN, FAIL);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_zarithrian(Creature* pCreature)
{
    return new boss_zarithrianAI(pCreature);
}

void AddSC_boss_zarithrian()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_zarithrian";
    pNewScript->GetAI = &GetAI_boss_zarithrian;
    pNewScript->RegisterSelf();
}
