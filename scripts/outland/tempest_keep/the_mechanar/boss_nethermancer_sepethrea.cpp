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
SDName: Boss_Nethermancer_Sepethrea
SD%Complete: 90
SDComment: May need some small adjustments
SDCategory: Tempest Keep, The Mechanar
EndScriptData */

#include "precompiled.h"
#include "mechanar.h"

enum
{
    SAY_AGGRO                       = -1554013,
    SAY_SUMMON                      = -1554014,
    SAY_DRAGONS_BREATH_1            = -1554015,
    SAY_DRAGONS_BREATH_2            = -1554016,
    SAY_SLAY1                       = -1554017,
    SAY_SLAY2                       = -1554018,
    SAY_DEATH                       = -1554019,

    SPELL_SUMMON_RAGIN_FLAMES       = 35275,
    SPELL_FROST_ATTACK              = 35263,
    SPELL_ARCANE_BLAST              = 35314,
    SPELL_DRAGONS_BREATH            = 35250,
    SPELL_KNOCKBACK                 = 37317,
    SPELL_SOLARBURN                 = 35267,

    NPC_RAGING_FLAMES               = 20481,

    // Summons spells
    SPELL_INFERNO                   = 35268,
    SPELL_INFERNO_H                 = 39346,
    SPELL_FIRE_TAIL                 = 35278,
};

struct MANGOS_DLL_DECL boss_nethermancer_sepethreaAI : public ScriptedAI
{
    boss_nethermancer_sepethreaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFrostAttackTimer;
    uint32 m_uiArcaneBlastTimer;
    uint32 m_uiDragonsBreathTimer;
    uint32 m_uiKnockbackTimer;
    uint32 m_uiSolarburnTimer;

    void Reset()
    {
        m_uiFrostAttackTimer    = urand(7000, 10000);
        m_uiArcaneBlastTimer    = urand(12000, 18000);
        m_uiDragonsBreathTimer  = urand(18000, 22000);
        m_uiKnockbackTimer      = urand(22000, 28000);
        m_uiSolarburnTimer      = 30000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_RAGIN_FLAMES);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SEPETHREA, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_RAGING_FLAMES)
        {
            pSummoned->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
            pSummoned->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);

            // ToDo: need to fixate target and make them walk!
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSummoned->GetMotionMaster()->MoveChase(pTarget);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Frost Attack
        if (m_uiFrostAttackTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_ATTACK) == CAST_OK)
                m_uiFrostAttackTimer = urand(7000, 10000);
        }
        else
            m_uiFrostAttackTimer -= uiDiff;

        // Arcane Blast
        if (m_uiArcaneBlastTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_BLAST) == CAST_OK)
                m_uiArcaneBlastTimer = 15000;
        }
        else
            m_uiArcaneBlastTimer -= uiDiff;

        // Dragons Breath
        if (m_uiDragonsBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_DRAGONS_BREATH) == CAST_OK)
            {
                if (urand(0, 1))
                    DoScriptText(urand(0, 1) ? SAY_DRAGONS_BREATH_1 : SAY_DRAGONS_BREATH_2, m_creature);

                m_uiDragonsBreathTimer = urand(12000, 22000);
            }
        }
        else
            m_uiDragonsBreathTimer -= uiDiff;

        // Knockback
        if (m_uiKnockbackTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_KNOCKBACK) == CAST_OK)
                m_uiKnockbackTimer = urand(15000, 25000);
        }
        else
            m_uiKnockbackTimer -= uiDiff;

        // Solarburn
        if (m_uiSolarburnTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_SOLARBURN) == CAST_OK)
                    m_uiSolarburnTimer = 30000;
            }
        }
        else
            m_uiSolarburnTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_nethermancer_sepethrea(Creature* pCreature)
{
    return new boss_nethermancer_sepethreaAI(pCreature);
}

struct MANGOS_DLL_DECL mob_ragin_flamesAI : public ScriptedAI
{
    mob_ragin_flamesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 inferno_Timer;
    uint32 flame_timer;

    void Reset()
    {
        inferno_Timer = 10000;
        flame_timer = 500;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (inferno_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_INFERNO : SPELL_INFERNO_H);

            m_creature->TauntApply(m_creature->getVictim());

            inferno_Timer = 10000;
        }else inferno_Timer -= diff;

        if (flame_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_FIRE_TAIL);
            flame_timer = 500;
        }else flame_timer -=diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ragin_flames(Creature* pCreature)
{
    return new mob_ragin_flamesAI(pCreature);
}

void AddSC_boss_nethermancer_sepethrea()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_nethermancer_sepethrea";
    pNewScript->GetAI = &GetAI_boss_nethermancer_sepethrea;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_ragin_flames";
    pNewScript->GetAI = &GetAI_mob_ragin_flames;
    pNewScript->RegisterSelf();
}
