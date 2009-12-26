/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Telestra
SD%Complete: 30%
SDComment: phase 2 and 3 todo
SDCategory: Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum
{
    SAY_AGGRO               = -1576000,
    SAY_SPLIT_1             = -1576001,
    SAY_SPLIT_2             = -1576002,
    SAY_MERGE               = -1576003,
    SAY_KILL                = -1576004,
    SAY_DEATH               = -1576005,

    SPELL_FIREBOMB          = 47773,
    SPELL_FIREBOMB_H        = 56934,

    SPELL_ICE_NOVA          = 47772,
    SPELL_ICE_NOVA_H        = 56935,

    SPELL_GRAVITY_WELL      = 47756,

    SPELL_SUMMON_CLONES     = 47710,

    SPELL_ARCANE_VISUAL     = 47704,
    SPELL_FIRE_VISUAL       = 47705,
    SPELL_FROST_VISUAL      = 47706,

    SPELL_SUMMON_FIRE       = 47707,
    SPELL_SUMMON_ARCANE     = 47708,
    SPELL_SUMMON_FROST      = 47709,

    SPELL_FIRE_DIES         = 47711,
    SPELL_ARCANE_DIES       = 47713,
    SPELL_FROST_DIES        = 47712,

    SPELL_SPAWN_BACK_IN     = 47714,

    NPC_TELEST_FIRE         = 26928,
    NPC_TELEST_ARCANE       = 26929,
    NPC_TELEST_FROST        = 26930,

    PHASE_1                 = 1,
    PHASE_2                 = 2,
    PHASE_3                 = 3
};

/*######
## boss_telestra
######*/

struct MANGOS_DLL_DECL boss_telestraAI : public ScriptedAI
{
    boss_telestraAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint8 m_uiPhase;

    uint32 m_uiFirebombTimer;
    uint32 m_uiIceNovaTimer;
    uint32 m_uiGravityWellTimer;

    void Reset()
    {
        m_uiPhase = PHASE_1;

        m_uiFirebombTimer = urand(2000, 4000);
        m_uiIceNovaTimer = urand(8000, 12000);
        m_uiGravityWellTimer = urand(15000, 25000);
    }

    void AttackStart(Unit* pWho)
    {
        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, 15.0f);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_TELESTRA, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            DoScriptText(SAY_KILL, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(m_uiPhase)
        {
            case PHASE_1:
            {
                if (!m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                {
                    if (m_uiFirebombTimer < uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FIREBOMB : SPELL_FIREBOMB_H) == CAST_OK)
                            m_uiFirebombTimer = urand(4000, 6000);
                    }
                    else
                        m_uiFirebombTimer -= uiDiff;

                    if (m_uiIceNovaTimer < uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ICE_NOVA : SPELL_ICE_NOVA_H, CAST_INTURRUPT_PREVIOUS) == CAST_OK)
                            m_uiIceNovaTimer = urand(10000, 15000);
                    }
                    else
                        m_uiIceNovaTimer -= uiDiff;

                    DoMeleeAttackIfReady();
                }

                if (m_uiGravityWellTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_GRAVITY_WELL) == CAST_OK)
                        m_uiGravityWellTimer = urand(15000, 30000);
                }
                else
                    m_uiGravityWellTimer -= uiDiff;

                break;
            }
            case PHASE_2:
            {
                break;
            }
            case PHASE_3:
            {
                DoMeleeAttackIfReady();
                break;
            }
        }
    }
};

CreatureAI* GetAI_boss_telestra(Creature* pCreature)
{
    return new boss_telestraAI(pCreature);
}

void AddSC_boss_telestra()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_telestra";
    newscript->GetAI = &GetAI_boss_telestra;
    newscript->RegisterSelf();
}
