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
SDName: boss_kologarn
SD%Complete: 20%
SDComment: Basic script
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                           = -1603126,
    SAY_SHOCKWAVE                       = -1603127,
    SAY_GRAB                            = -1603128,
    SAY_ARM_LOST_LEFT                   = -1603129,
    SAY_ARM_LOST_RIGHT                  = -1603130,
    SAY_SLAY_1                          = -1603131,
    SAY_SLAY_2                          = -1603132,
    SAY_BERSERK                         = -1603133,
    SAY_DEATH                           = -1603134,

    EMOTE_ARM_RIGHT                     = -1603135,
    EMOTE_ARM_LEFT                      = -1603136,
    EMOTE_STONE_GRIP                    = -1603137,

    // Kologarn
    SPELL_INSTAKILL_KOLOGARN_ARM        = 63628,                // kill both arms on death
    SPELL_OVERHEAD_SMASH                = 63356,                // cast if both arms are alive
    SPELL_OVERHEAD_SMASH_H              = 64003,
    SPELL_ONE_ARMED_SMASH               = 63573,                // cast if only one arm is alive
    SPELL_ONE_ARMED_SMASH_H             = 64006,
    SPELL_STONE_SHOUT                   = 63716,                // cast if no arms are alive
    SPELL_STONE_SHOUT_H                 = 64005,
    SPELL_PETRIFYING_BREATH             = 62030,                // cast if nobody is in melee range
    SPELL_PETRIFYING_BREATH_H           = 63980,
    SPELL_BERSERK                       = 64238,

    // Arms spells
    SPELL_ARM_VISUAL                    = 64753,                // spawn visual
    SPELL_ARM_DEAD_DAMAGE_KOLOGARN      = 63629,                // damage to Kologarn on arm death
    SPELL_ARM_DEAD_DAMAGE_KOLOGARN_H    = 63979,
    SPELL_RIDE_KOLOGARN_ARMS            = 65343,

    // Left arm
    SPELL_ARM_SWEEP                     = 63766,                // triggers shockwave effect and visual spells
    SPELL_ARM_SWEEP_H                   = 63983,

    // Right arm
    SPELL_STONE_GRIP                    = 62166,                // triggers vehicle control, damage and visual spells
    SPELL_STONE_GRIP_H                  = 63981,

    // Focused Eyebeam
    SPELL_FOCUSED_EYEBEAM_SUMMON        = 63342,                // triggers summons spells for npcs 33632 and 33802
    SPELL_EYEBEAM_PERIODIC              = 63347,
    SPELL_EYEBEAM_PERIODIC_H            = 63977,
    SPELL_EYEBEAM_VISUAL_LEFT           = 63348,
    SPELL_EYEBEAM_VISUAL_RIGHT          = 63676,

    // NPC ids
    NPC_FOCUSED_EYEBEAM_RIGHT           = 33802,
    NPC_FOCUSED_EYEBEAM_LEFT            = 33632,
    NPC_RUBBLE                          = 33768,
};

/*######
## boss_kologarn
######*/

struct MANGOS_DLL_DECL boss_kologarnAI : public Scripted_NoMovementAI
{
    boss_kologarnAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    void Reset() override
    {
    }

    void JustDied(Unit* pKiller) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KOLOGARN, DONE);

        DoCastSpellIfCan(m_creature, SPELL_INSTAKILL_KOLOGARN_ARM, CAST_TRIGGERED);
        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void Aggro(Unit* pWho) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KOLOGARN, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KOLOGARN, FAIL);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kologarn(Creature* pCreature)
{
    return new boss_kologarnAI(pCreature);
}

void AddSC_boss_kologarn()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_kologarn";
    pNewScript->GetAI = GetAI_boss_kologarn;
    pNewScript->RegisterSelf();
}
