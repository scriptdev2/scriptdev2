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
SDName: Boss_Ragnaros
SD%Complete: 60
SDComment: Intro Dialogue and event NYI, Melee/ Range Combat behavior is not correct, Some abilities are missing
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"

/* There have been quite some bugs about his spells, keep this as reference untill all finished
 * From original version, spells
 *   SPELL_HAND_OF_RAGNAROS      = 19780 // Is not linked to Ragnaros, and also not mentioned on wowwiki, timer was 25s
 *   SPELL_ERRUPTION             = 17731 // Is a spell from Onyxia-Encounter
 *   SPELL_SONSOFFLAME_DUMMY     = 21108 // Does not exist anymore, and was also unused
 * have been removed
 *
 * Missing features (based on wowwiki)
 *   Hammer of Ragnaros - Ranged Knockback and Damage
 *   Lava Splash - Localized Damage
 *
 * A few interesting spells
 *    Might of Ragnaros: Summons a trigger npc (Flame of Ragnaros), visual fits to Hammer, possible knockback spell in spell range: 21155
 */

enum
{
    SAY_REINFORCEMENTS_1        = -1409013,
    SAY_REINFORCEMENTS_2        = -1409014,
    SAY_HAMMER                  = -1409015,                 // TODO Hammer of Ragnaros
    SAY_WRATH                   = -1409016,
    SAY_KILL                    = -1409017,
    SAY_MAGMABURST              = -1409018,

    SPELL_WRATH_OF_RAGNAROS     = 20566,
    SPELL_ELEMENTAL_FIRE        = 20564,
    SPELL_MAGMA_BLAST           = 20565,                    // Ranged attack if nobody is in melee range
    SPELL_MELT_WEAPON           = 21388,                    // Passive aura
    SPELL_RAGNA_SUBMERGE        = 21107,                    // Stealth aura
    SPELL_RAGNA_EMERGE          = 20568,                    // Emerge from lava

    MAX_ADDS_IN_SUBMERGE        = 8,
    NPC_SON_OF_FLAME            = 12143
};

struct MANGOS_DLL_DECL boss_ragnarosAI : public Scripted_NoMovementAI
{
    boss_ragnarosAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (instance_molten_core*)pCreature->GetInstanceData();
        Reset();
    }

    instance_molten_core* m_pInstance;

    uint32 m_uiWrathOfRagnarosTimer;
    uint32 m_uiMagmaBlastTimer;
    uint32 m_uiElementalFireTimer;
    uint32 m_uiSubmergeTimer;
    uint32 m_uiAttackTimer;
    uint32 m_uiAddCount;

    bool m_bHasYelledMagmaBurst;
    bool m_bHasSubmergedOnce;
    bool m_bIsSubmerged;

    void Reset()
    {
        m_uiWrathOfRagnarosTimer = 30000;
        m_uiMagmaBlastTimer = 2000;
        m_uiElementalFireTimer = 3000;
        m_uiSubmergeTimer = 3*MINUTE*IN_MILLISECONDS;
        m_uiAttackTimer = 90*IN_MILLISECONDS;
        m_uiAddCount = 0;

        m_bHasYelledMagmaBurst = false;
        m_bHasSubmergedOnce = false;
        m_bIsSubmerged = false;
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 4))
            return;

        DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAGNAROS, DONE);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAGNAROS, IN_PROGRESS);

        // Passive aura
        DoCastSpellIfCan(m_creature, SPELL_MELT_WEAPON, CAST_TRIGGERED);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAGNAROS, FAIL);

        // Reset flag if had been submerged
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void SummonedCreatureJustDied(Creature* pSummmoned)
    {
        // If all Sons of Flame are dead, trigger emerge
        if (pSummmoned->GetEntry() == NPC_SON_OF_FLAME)
        {
            m_uiAddCount--;

            // If last add killed then emerge soonish
            if (m_uiAddCount == 0)
                m_uiAttackTimer = std::min(m_uiAttackTimer, (uint32)1000);
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_SON_OF_FLAME)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSummoned->AI()->AttackStart(pTarget);

            ++m_uiAddCount;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsSubmerged)
        {
            // Timer to check when Ragnaros should emerge (is set to soonish, when last add is killed)
            if (m_uiAttackTimer < uiDiff)
            {
                // Become emerged again
                DoCastSpellIfCan(m_creature, SPELL_RAGNA_EMERGE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_uiSubmergeTimer = 3*MINUTE*IN_MILLISECONDS;
                m_uiMagmaBlastTimer = 3000;                 // Delay the magma blast after emerge
                m_bIsSubmerged = false;
            }
            else
                m_uiAttackTimer -= uiDiff;

            // Do nothing while submerged
            return;
        }

        // Wrath Of Ragnaros Timer
        if (m_uiWrathOfRagnarosTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_WRATH_OF_RAGNAROS) == CAST_OK)
            {
                if (urand(0, 1))
                    DoScriptText(SAY_WRATH, m_creature);

                m_uiWrathOfRagnarosTimer = 30000;
            }
        }
        else
            m_uiWrathOfRagnarosTimer -= uiDiff;

        // Elemental FireTimer
        if (m_uiElementalFireTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ELEMENTAL_FIRE) == CAST_OK)
                m_uiElementalFireTimer = urand(10000, 14000);
        }
        else
            m_uiElementalFireTimer -= uiDiff;

        // Submerge Timer
        if (m_uiSubmergeTimer < uiDiff)
        {

            // Submerge and attack again after 90 secs
            DoCastSpellIfCan(m_creature, SPELL_RAGNA_SUBMERGE, CAST_INTERRUPT_PREVIOUS);
            m_creature->HandleEmote(EMOTE_ONESHOT_SUBMERGE);
            m_bIsSubmerged = true;
            m_uiAttackTimer = 90*IN_MILLISECONDS;

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            // Say dependend if first time or not
            DoScriptText(!m_bHasSubmergedOnce ? SAY_REINFORCEMENTS_1 : SAY_REINFORCEMENTS_2, m_creature);
            m_bHasSubmergedOnce = false;

            // Summon 8 elementals at random points around the boss
            float fX, fY, fZ;
            for(uint8 i = 0; i < MAX_ADDS_IN_SUBMERGE; ++i)
            {
                m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 30.0f, fX, fY, fZ);
                m_creature->SummonCreature(NPC_SON_OF_FLAME, fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
            }

            return;
        }
        else
            m_uiSubmergeTimer -= uiDiff;

        // TODO this actually should select _any_ enemy in melee range, not only the tank
        // Range check for melee target, if nobody is found in range, then cast magma blast on random
        // If we are within range melee the target
        if (m_creature->IsNonMeleeSpellCasted(false) || !m_creature->getVictim())
            return;

        if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            // Make sure our attack is ready
            if (m_creature->isAttackReady())
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
                m_bHasYelledMagmaBurst = false;
            }
        }
        else
        {
            // Magma Burst Timer
            if (m_uiMagmaBlastTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_MAGMA_BLAST) == CAST_OK)
                    {
                        if (!m_bHasYelledMagmaBurst)
                        {
                            DoScriptText(SAY_MAGMABURST, m_creature);
                            m_bHasYelledMagmaBurst = true;
                        }
                        m_uiMagmaBlastTimer = 1000;          // Spamm this!
                    }
                }
            }
            else
                m_uiMagmaBlastTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_boss_ragnaros(Creature* pCreature)
{
    return new boss_ragnarosAI(pCreature);
}

void AddSC_boss_ragnaros()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_ragnaros";
    pNewScript->GetAI = &GetAI_boss_ragnaros;
    pNewScript->RegisterSelf();
}
