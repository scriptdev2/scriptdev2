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
SDName: Boss_Volazj
SD%Complete: 50%
SDComment: Insanity NYI; Timers need adjustments
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO                       = -1619033,
    SAY_INSANITY                    = -1619034,
    SAY_SLAY_1                      = -1619035,
    SAY_SLAY_2                      = -1619036,
    SAY_SLAY_3                      = -1619037,
    SAY_DEATH_1                     = -1619038,         // missing text
    SAY_DEATH_2                     = -1619039,

    SPELL_MIND_FLAY                 = 57941,
    SPELL_MIND_FLAY_H               = 59974,
    SPELL_SHADOW_BOLT               = 57942,
    SPELL_SHADOW_BOLT_H             = 59975,
    SPELL_SHIVER                    = 57949,
    SPELL_SHIVER_H                  = 59978,

    SPELL_INSANITY                  = 57496,            // start insanity phasing
    SPELL_INSANITY_CLEAR            = 57558,            // clear all insanity phasing
    SPELL_WHISPER_INSANITY          = 60292,            // whisper insanity to players

    /* Some notes about the Insanity spell */
    /*
        Related spells:
        57561 - visual effect
        57538 - switch insanity phase when a visage is killed
        57507 to 57512 - phasing spells for players
        59982 - summon twisted visage - script effect
        57500 to 57504 - summon twisted visage in each phase
        57506, 57507 - clone spells for the twisted visages
        57551, 57555 - visuals for twisted visages
    */
};

/*######
## boss_volazj
######*/

struct MANGOS_DLL_DECL boss_volazjAI : public ScriptedAI
{
    boss_volazjAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint8 m_uiCombatPhase;
    uint32 m_uiMindFlayTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiShiverTimer;

    void Reset() override
    {
        m_uiCombatPhase = 1;
        m_uiMindFlayTimer = 10000;
        m_uiShadowBoltTimer = 5000;
        m_uiShiverTimer = 18000;
    }

    void Aggro(Unit* pWho) override
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* /*pVictim*/) override
    {
        switch (urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(urand(0, 1) ? SAY_DEATH_1 : SAY_DEATH_2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        /* Note: Insanity is not yet implemented - this is just a placeholder
        if (m_creature->GetHealthPercent() < 100 - m_uiCombatPhase*33)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_INSANITY) == CAST_OK)
                ++m_uiCombatPhase;
        }
        */

        if (m_uiMindFlayTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MIND_FLAY : SPELL_MIND_FLAY_H) == CAST_OK)
                m_uiMindFlayTimer = urand(10000, 20000);
        }
        else
            m_uiMindFlayTimer -= uiDiff;

        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H) == CAST_OK)
                m_uiShadowBoltTimer = urand(8000, 13000);
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        if (m_uiShiverTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_SHIVER : SPELL_SHIVER_H) == CAST_OK)
                    m_uiShiverTimer = 30000;
            }
        }
        else
            m_uiShiverTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_volazj(Creature* pCreature)
{
    return new boss_volazjAI(pCreature);
}

void AddSC_boss_volazj()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_volazj";
    pNewScript->GetAI = &GetAI_boss_volazj;
    pNewScript->RegisterSelf();
}
