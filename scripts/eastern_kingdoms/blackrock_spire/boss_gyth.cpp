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
SDName: Boss_Gyth
SD%Complete: 100
SDComment: Whole Event needs some rewrite
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"
#include "blackrock_spire.h"

enum
{
    SPELL_CORROSIVEACID     = 20667,
    SPELL_FREEZE            = 16350,                        // ID was wrong!
    SPELL_FLAMEBREATH       = 20712,
    SPELL_ROOT_SELF         = 33356,

    MODEL_ID_INVISIBLE      = 11686,
    MODEL_ID_GYTH_MOUNTED   = 9723,
    MODEL_ID_GYTH           = 9806,

    NPC_FIRE_TONGUE         = 10372,
    NPC_CHROMATIC_WHELP     = 10442,
    NPC_CHROMATIC_DRAGON    = 10447,
    NPC_BLACKHAND_ELITE     = 10317,
    NPC_REND_BLACKHAND      = 10429
};

struct MANGOS_DLL_DECL boss_gythAI : public ScriptedAI
{
    boss_gythAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_blackrock_spire*) pCreature->GetInstanceData();
        Reset();
    }

    instance_blackrock_spire* m_pInstance;
    uint64 m_uiCombatDoorGUID;
    uint32 uiAggroTimer;
    uint32 uiDragonsTimer;
    uint32 uiOrcTimer;
    uint32 uiCorrosiveAcidTimer;
    uint32 uiFreezeTimer;
    uint32 uiFlamebreathTimer;
    uint32 uiLine1Count;
    uint32 uiLine2Count;

    bool m_bSummonedRend;
    bool m_bAggro;
    bool m_bRootSelf;

    void Reset()
    {
        uiDragonsTimer = 3000;
        uiOrcTimer = 60000;
        uiAggroTimer = 60000;
        uiCorrosiveAcidTimer = 8000;
        uiFreezeTimer = 11000;
        uiFlamebreathTimer = 4000;
        m_bSummonedRend = false;
        m_bAggro = false;
        m_bRootSelf = false;

        // how many times should the two lines of summoned creatures be spawned
        // min 2 x 2, max 7 lines of attack in total
        uiLine1Count = urand(2, 5);
        uiLine2Count = urand(2, 7 - uiLine1Count);

        // Invisible for event start
        m_creature->SetDisplayId(MODEL_ID_INVISIBLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_GYTH, IN_PROGRESS);
            m_uiCombatDoorGUID = m_pInstance->GetData64(GO_GYTH_COMBAT_DOOR);
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GYTH, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GYTH, FAIL);
    }

    void SummonCreatureWithRandomTarget(uint32 uiCreatureId)
    {
        float fX, fY, fZ;
        m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 2*INTERACTION_DISTANCE, fX, fY, fZ);
        fX = std::min(m_creature->GetPositionX(), fX);      // Halfcircle - suits better the rectangular form
        if (Creature* pSummoned = m_creature->SummonCreature(uiCreatureId, fX, fY, fZ, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000))
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bRootSelf)
        {
            DoCastSpellIfCan(m_creature, SPELL_ROOT_SELF);
            m_bRootSelf = true;
        }

        if (!m_bAggro && uiLine1Count == 0 && uiLine2Count == 0)
        {
            if (uiAggroTimer < uiDiff)
            {
                m_bAggro = true;
                // Visible now!
                m_creature->SetDisplayId(MODEL_ID_GYTH_MOUNTED);
                m_creature->setFaction(14);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveAurasDueToSpell(SPELL_ROOT_SELF);
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);

            }
            else
                uiAggroTimer -= uiDiff;
        }

        // Summon Dragon pack. 2 Dragons and 3 Whelps
        if (!m_bAggro && !m_bSummonedRend && uiLine1Count > 0)
        {
            if (uiDragonsTimer < uiDiff)
            {
                SummonCreatureWithRandomTarget(NPC_FIRE_TONGUE);
                SummonCreatureWithRandomTarget(NPC_FIRE_TONGUE);
                SummonCreatureWithRandomTarget(NPC_CHROMATIC_WHELP);
                SummonCreatureWithRandomTarget(NPC_CHROMATIC_WHELP);
                SummonCreatureWithRandomTarget(NPC_CHROMATIC_WHELP);
                --uiLine1Count;
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
                uiDragonsTimer = 60000;
            }
            else
                uiDragonsTimer -= uiDiff;
        }

        //Summon Orc pack. 1 Orc Handler 1 Elite Dragonkin and 3 Whelps
        if (!m_bAggro && !m_bSummonedRend && uiLine1Count == 0 && uiLine2Count > 0)
        {
            if (uiOrcTimer < uiDiff)
            {
                SummonCreatureWithRandomTarget(NPC_CHROMATIC_DRAGON);
                SummonCreatureWithRandomTarget(NPC_BLACKHAND_ELITE);
                SummonCreatureWithRandomTarget(NPC_CHROMATIC_WHELP);
                SummonCreatureWithRandomTarget(NPC_CHROMATIC_WHELP);
                SummonCreatureWithRandomTarget(NPC_CHROMATIC_WHELP);
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
                --uiLine2Count;
                uiOrcTimer = 60000;
            }
            else
                uiOrcTimer -= uiDiff;
        }

        // we take part in the fight
        if (m_bAggro)
        {
             // CorrosiveAcid_Timer
            if (uiCorrosiveAcidTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_CORROSIVEACID);
                uiCorrosiveAcidTimer = 7000;
            }
            else
                uiCorrosiveAcidTimer -= uiDiff;

            // Freeze_Timer
            if (uiFreezeTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FREEZE) == CAST_OK)
                    uiFreezeTimer = 16000;
            }
            else
                uiFreezeTimer -= uiDiff;

            // Flamebreath_Timer
            if (uiFlamebreathTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_FLAMEBREATH);
                uiFlamebreathTimer = 10500;
            }
            else
                uiFlamebreathTimer -= uiDiff;

            //Summon Rend
            if (!m_bSummonedRend && m_creature->GetHealthPercent() < 11.0f)
            {
                // summon Rend and Change model to normal Gyth
                // Inturrupt any spell casting
                m_creature->InterruptNonMeleeSpells(false);
                // Gyth model
                m_creature->SetDisplayId(MODEL_ID_GYTH);
                m_creature->SummonCreature(NPC_REND_BLACKHAND, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
                m_bSummonedRend = true;
            }

            DoMeleeAttackIfReady();
        }                                                   // end if Aggro
    }
};

CreatureAI* GetAI_boss_gyth(Creature* pCreature)
{
    return new boss_gythAI(pCreature);
}

void AddSC_boss_gyth()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "boss_gyth";
    pNewScript->GetAI = &GetAI_boss_gyth;
    pNewScript->RegisterSelf();
}
