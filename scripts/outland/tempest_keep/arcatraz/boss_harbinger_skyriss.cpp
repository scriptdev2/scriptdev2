/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Harbinger_Skyriss
SD%Complete: 45
SDComment: CombatAI not fully implemented. Timers will need adjustments. On wipe despawn all, and let warden_mellichar respawn after ~1min
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

/* ContentData
boss_harbinger_skyriss
boss_harbinger_skyriss_illusion -- TODO implement or move to ACID
EndContentData */

#include "precompiled.h"
#include "arcatraz.h"

enum
{
    SAY_INTRO                   = -1552000,
    SAY_AGGRO                   = -1552001,
    SAY_KILL_1                  = -1552002,
    SAY_KILL_2                  = -1552003,
    SAY_MIND_1                  = -1552004,
    SAY_MIND_2                  = -1552005,
    SAY_FEAR_1                  = -1552006,
    SAY_FEAR_2                  = -1552007,
    SAY_IMAGE                   = -1552008,
    SAY_DEATH                   = -1552009,

    SPELL_FEAR                  = 39415,
    SPELL_MIND_REND             = 36924,
    SPELL_MIND_REND_H           = 39017,
    SPELL_DOMINATION            = 37162,
    SPELL_DOMINATION_H          = 39019,
    SPELL_MANA_BURN_H           = 39020,
    SPELL_66_ILLUSION           = 36931,                    // Summons 21466
    SPELL_33_ILLUSION           = 36932,                    // Summons 21467
};

struct MANGOS_DLL_DECL boss_harbinger_skyrissAI : public ScriptedAI
{
    boss_harbinger_skyrissAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bIntroFinished = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIntroFinished;
    bool m_bDidSplitImage33;
    bool m_bDidSplitImage66;

    uint32 m_uiIntroPhase;
    uint32 m_uiIntroTimer;
    uint32 m_uiMindRendTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiDominationTimer;
    uint32 m_uiManaBurnTimer;

    void Reset()
    {
        m_bDidSplitImage33 = false;
        m_bDidSplitImage66 = false;

        m_uiIntroPhase = 1;
        m_uiIntroTimer = 5000;
        m_uiMindRendTimer = 3000;
        m_uiFearTimer = 15000;
        m_uiDominationTimer = 30000;
        m_uiManaBurnTimer = 25000;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bIntroFinished)
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_bIntroFinished)
            return;

        ScriptedAI::AttackStart(pWho);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HARBINGERSKYRISS, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        //won't yell killing pet/other unit
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(m_creature->getVictim());
    }

    void DoSplit()
    {
        DoScriptText(SAY_IMAGE, m_creature);

        DoCastSpellIfCan(m_creature, m_bDidSplitImage33 ? SPELL_33_ILLUSION : SPELL_66_ILLUSION, CAST_INTERRUPT_PREVIOUS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIntroFinished && !m_creature->isInCombat())
        {
            if (!m_pInstance)
                return;

            if (m_uiIntroTimer < uiDiff)
            {
                switch(m_uiIntroPhase)
                {
                    case 1:
                        DoScriptText(SAY_INTRO, m_creature);
                        if (GameObject* pSphere = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_SEAL_SPHERE)))
                            pSphere->SetGoState(GO_STATE_ACTIVE);
                        ++m_uiIntroPhase;
                        m_uiIntroTimer = 25000;
                        break;
                    case 2:
                        DoScriptText(SAY_AGGRO, m_creature);
                        if (Creature* pMellic = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_MELLICHAR)))
                        {
                            // should have a better way to do this. possibly spell exist.
                            pMellic->SetDeathState(JUST_DIED);
                            pMellic->SetHealth(0);
                        }
                        ++m_uiIntroPhase;
                        m_uiIntroTimer = 3000;
                        break;
                    case 3:
                        m_bIntroFinished = true;
                        // TODO - Let Attack here, or change Unit_flag
                        break;
                }
            }
            else
                m_uiIntroTimer -=uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bDidSplitImage66 && m_creature->GetHealthPercent() <= 66.0f)
        {
            m_bDidSplitImage66 = true;
            DoSplit();
        }

        if (!m_bDidSplitImage33 && m_creature->GetHealthPercent() <= 33.0f)
        {
            m_bDidSplitImage33 = true;
            DoSplit();
        }

        if (m_uiMindRendTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
            if (!pTarget)
                pTarget = m_creature->getVictim();

            if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_MIND_REND : SPELL_MIND_REND_H) == CAST_OK)
                m_uiMindRendTimer = 8000;
        }
        else
            m_uiMindRendTimer -=uiDiff;

        if (m_uiFearTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
            if (!pTarget)
                pTarget = m_creature->getVictim();

            if (DoCastSpellIfCan(pTarget, SPELL_FEAR) == CAST_OK)
            {
                DoScriptText(urand(0, 1) ? SAY_FEAR_1 : SAY_FEAR_2, m_creature);
                m_uiFearTimer = 25000;
            }
        }
        else
            m_uiFearTimer -=uiDiff;

        if (m_uiDominationTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
            if (!pTarget)
                pTarget = m_creature->getVictim();

            if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_DOMINATION : SPELL_DOMINATION_H) == CAST_OK)
            {
                DoScriptText(urand(0, 1) ? SAY_MIND_1 : SAY_MIND_2, m_creature);
                m_uiDominationTimer = urand(16000, 32000);
            }
        }
        else
            m_uiDominationTimer -=uiDiff;

        if (!m_bIsRegularMode)
        {
            if (m_uiManaBurnTimer < uiDiff)
            {
                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
                if (!pTarget)
                    pTarget = m_creature->getVictim();

                if (DoCastSpellIfCan(pTarget, SPELL_MANA_BURN_H) == CAST_OK)
                    m_uiManaBurnTimer = urand(16000, 32000);
            }
            else
                m_uiManaBurnTimer -=uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_harbinger_skyriss(Creature* pCreature)
{
    return new boss_harbinger_skyrissAI(pCreature);
}

enum
{
    SPELL_MIND_REND_IMAGE       = 36929,
    SPELL_MIND_REND_IMAGE_H     = 39021,
};

struct MANGOS_DLL_DECL boss_harbinger_skyriss_illusionAI : public ScriptedAI
{
    boss_harbinger_skyriss_illusionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset() { }
};

CreatureAI* GetAI_boss_harbinger_skyriss_illusion(Creature* pCreature)
{
    return new boss_harbinger_skyriss_illusionAI(pCreature);
}

void AddSC_boss_harbinger_skyriss()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_harbinger_skyriss";
    pNewScript->GetAI = &GetAI_boss_harbinger_skyriss;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_harbinger_skyriss_illusion";
    pNewScript->GetAI = &GetAI_boss_harbinger_skyriss_illusion;
    pNewScript->RegisterSelf();
}
