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
SDName: boss_forgemaster_garfrost
SD%Complete: 70
SDComment: TODO movement to the forges currently workaround (need core support for Jump-MMGen)
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    SAY_AGGRO                           = -1658014,
    SAY_SLAY_1                          = -1658015,
    SAY_BOULDER_HIT                     = -1658016,         // TODO How must this be handled?
    SAY_DEATH                           = -1658017,
    SAY_FORGE_1                         = -1658018,
    SAY_FORGE_2                         = -1658019,
    SAY_TYRANNUS_GARFROST               = -1658020,
    SAY_GENERAL_GARFROST                = -1658021,

    EMOTE_THROW_SARONITE                = -1658022,
    EMOTE_DEEP_FREEZE                   = -1658023,

    SPELL_PERMAFROST                    = 70326,
    SPELL_THROW_SARONITE                = 68788,
    SPELL_THUNDERING_STOMP              = 68771,
    SPELL_FORGE_FROZEN_BLADE            = 68774,
    SPELL_CHILLING_WAVE                 = 68778,
    SPELL_FORGE_FROSTBORN_MACE          = 68785,
    SPELL_DEEP_FREEZE                   = 70381,

    PHASE_NO_ENCHANTMENT                = 1,
    PHASE_BLADE_ENCHANTMENT             = 2,
    PHASE_MACE_ENCHANTMENT              = 3,
    PHASE_MOVEMENT                      = 4,
};

static const float aGarfrostMoveLocs[2][3] =
{
    {719.785f, -230.227f, 527.033f},
    {657.539f, -203.564f, 526.691f},
};

struct MANGOS_DLL_DECL boss_forgemaster_garfrostAI : public ScriptedAI

{
    boss_forgemaster_garfrostAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiThrowSaroniteTimer;
    uint32 m_uiPhase;
    uint32 m_uiChillingWaveTimer;
    uint32 m_uiDeepFreezeTimer;

    void Reset()
    {
        m_uiThrowSaroniteTimer = 13000;
        m_uiChillingWaveTimer = 10000;
        m_uiDeepFreezeTimer = 10000;
        SetCombatMovement(true);
        m_uiPhase = PHASE_NO_ENCHANTMENT;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature, pWho);
        DoCastSpellIfCan(m_creature, SPELL_PERMAFROST);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature, pKiller);
    }

    void KilledUnit()
    {
        DoScriptText(SAY_SLAY_1, m_creature);
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        // TODO Change to jump movement type when proper implemented
        if (uiMotionType != POINT_MOTION_TYPE)
            return;

        if (uiPointId != PHASE_BLADE_ENCHANTMENT && uiPointId != PHASE_MACE_ENCHANTMENT)
            return;

        // Cast and say expected spell
        DoCastSpellIfCan(m_creature, uiPointId == PHASE_BLADE_ENCHANTMENT ? SPELL_FORGE_FROZEN_BLADE : SPELL_FORGE_FROSTBORN_MACE);
        DoScriptText(uiPointId == PHASE_BLADE_ENCHANTMENT ? SAY_FORGE_1 : SAY_FORGE_2, m_creature);

        m_uiThrowSaroniteTimer += 5000;                     // Delay next Saronit
        m_uiPhase = uiPointId;
        SetCombatMovement(true);

        if (m_creature->getVictim())
        {
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Do nothing more while moving
        if (m_uiPhase == PHASE_MOVEMENT)
            return;

        // Casted in every phase
        if (m_uiThrowSaroniteTimer < uiDiff)
        {
            // TODO - only target players?
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_THROW_SARONITE) == CAST_OK)
                {
                    DoScriptText(EMOTE_THROW_SARONITE, m_creature, pTarget);
                    m_uiThrowSaroniteTimer = 16000;
                }
            }
        }
        else
            m_uiThrowSaroniteTimer -= uiDiff;

        switch (m_uiPhase)
        {
            case PHASE_NO_ENCHANTMENT:
            {
                if (m_creature->GetHealthPercent() < 66.0f)
                {
                    DoCastSpellIfCan(m_creature, SPELL_THUNDERING_STOMP, CAST_INTERRUPT_PREVIOUS);
                    SetCombatMovement(false);

                    // TODO This should actually be jump movement
                    m_creature->GetMotionMaster()->MovePoint(PHASE_BLADE_ENCHANTMENT, aGarfrostMoveLocs[0][0], aGarfrostMoveLocs[0][1], aGarfrostMoveLocs[0][2]);
                    m_uiPhase = PHASE_MOVEMENT;

                    // Stop further action
                    return;
                }
                break;
            }
            case PHASE_BLADE_ENCHANTMENT:
            {
                if (m_creature->GetHealthPercent() < 33.0f)
                {
                    DoCastSpellIfCan(m_creature, SPELL_THUNDERING_STOMP, CAST_INTERRUPT_PREVIOUS);
                    SetCombatMovement(false);

                    // TODO This should actually be jump movement
                    m_creature->GetMotionMaster()->MovePoint(PHASE_MACE_ENCHANTMENT, aGarfrostMoveLocs[1][0], aGarfrostMoveLocs[1][1], aGarfrostMoveLocs[1][2]);
                    m_uiPhase = PHASE_MOVEMENT;

                    // Stop further action
                    return;
                }

                if (m_uiChillingWaveTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHILLING_WAVE) == CAST_OK)
                        m_uiChillingWaveTimer = 14000;
                }
                else
                    m_uiChillingWaveTimer -= uiDiff;

                break;
            }
            case PHASE_MACE_ENCHANTMENT:
            {
                if (m_uiDeepFreezeTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_DEEP_FREEZE) == CAST_OK)
                        {
                            DoScriptText(EMOTE_DEEP_FREEZE, m_creature, pTarget);
                            m_uiDeepFreezeTimer = 20000;
                        }
                    }
                }
                else
                    m_uiDeepFreezeTimer -= uiDiff;

                break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_forgemaster_garfrost(Creature* pCreature)
{
    return new boss_forgemaster_garfrostAI(pCreature);
}

void AddSC_boss_garfrost()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_forgemaster_garfrost";
    pNewScript->GetAI = &GetAI_boss_forgemaster_garfrost;
    pNewScript->RegisterSelf();
}
