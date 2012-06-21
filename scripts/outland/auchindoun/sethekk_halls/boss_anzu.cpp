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
SDName: boss_anzu
SD%Complete: 10
SDComment: Basic script
SDCategory: Auchindoun, Sethekk Halls
EndScriptData */

#include "precompiled.h"
#include "sethekk_halls.h"

enum
{
    SPELL_SCREECH       = 40184,
    SPELL_SPELL_BOMB    = 40303,
    SPELL_CYCLONE       = 40321,
    SPELL_BANISH_SELF   = 42354,

    NPC_BROOD_OF_ANZU   = 23132,
};

struct MANGOS_DLL_DECL boss_anzuAI : public ScriptedAI
{
    boss_anzuAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_sethekk_halls*)pCreature->GetInstanceData();
        Reset();
    }

    instance_sethekk_halls* m_pInstance;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANZU, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANZU, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANZU, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anzu(Creature* pCreature)
{
    return new boss_anzuAI(pCreature);
}

void AddSC_boss_anzu()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_anzu";
    pNewScript->GetAI = &GetAI_boss_anzu;
    pNewScript->RegisterSelf();
}
