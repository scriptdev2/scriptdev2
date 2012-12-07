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
SDName: boss_saviana
SD%Complete: 20
SDComment: only yells
SDCategory: Ruby Sanctum
EndScriptData */

#include "precompiled.h"
#include "ruby_sanctum.h"

enum
{
    SAY_AGGRO                   = -1724015,
    SAY_SLAY_1                  = -1724016,
    SAY_SLAY_2                  = -1724017,
    SAY_SPECIAL                 = -1724018,
    SOUND_DEATH                 = 17531,                    // On death it has only a screaming sound
};

struct MANGOS_DLL_DECL boss_savianaAI : public ScriptedAI
{
    boss_savianaAI(Creature* pCreature) : ScriptedAI(pCreature)
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
            m_pInstance->SetData(TYPE_SAVIANA, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoPlaySoundToSet(m_creature, SOUND_DEATH);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAVIANA, DONE);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAVIANA, FAIL);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_saviana(Creature* pCreature)
{
    return new boss_savianaAI(pCreature);
}

void AddSC_boss_saviana()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_saviana";
    pNewScript->GetAI = &GetAI_boss_saviana;
    pNewScript->RegisterSelf();
}
