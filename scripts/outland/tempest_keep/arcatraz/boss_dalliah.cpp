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
SDName: boss_dalliah
SD%Complete: 20
SDComment: Basic script
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

#include "precompiled.h"
#include "arcatraz.h"

enum
{
    SAY_AGGRO                       = -1552031,
    SAY_SOCCOTHRATES_AGGRO_1        = -1552039,
    SAY_SOCCOTHRATES_TAUNT_1        = -1552040,
    SAY_SOCCOTHRATES_TAUNT_2        = -1552041,
    SAY_SOCCOTHRATES_TAUNT_3        = -1552042,
    SAY_HEAL_1                      = -1552032,
    SAY_HEAL_2                      = -1552033,
    SAY_KILL_1                      = -1552034,
    SAY_KILL_2                      = -1552035,
    SAY_WHIRLWIND_1                 = -1552036,
    SAY_WHIRLWIND_2                 = -1552037,
    SAY_DEATH                       = -1552038,
    SAY_SOCCOTHRATES_DEATH          = -1552043,

    SPELL_GIFT_DOOMSAYER            = 36173,
    SPELL_GIFT_DOOMSAYER_H          = 39009,
    SPELL_HEAL                      = 36144,
    SPELL_HEAL_H                    = 39013,
    SPELL_WHIRLWIND                 = 36142,
    SPELL_SHADOW_WAVE               = 39016,                // heroic spell only
};

struct MANGOS_DLL_DECL boss_dalliahAI : public ScriptedAI
{
    boss_dalliahAI(Creature* pCreature) : ScriptedAI(pCreature)
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

        if (m_pInstance)
            m_pInstance->SetData(TYPE_DALLIAH, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustDied(Unit* pWho)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_DALLIAH, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DALLIAH, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_dalliah(Creature* pCreature)
{
    return new boss_dalliahAI(pCreature);
}

void AddSC_boss_dalliah()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_dalliah";
    pNewScript->GetAI = &GetAI_boss_dalliah;
    pNewScript->RegisterSelf();
}
