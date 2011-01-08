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
SDName: Boss_Ouro
SD%Complete: 50
SDComment: script needs to be reworked
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "temple_of_ahnqiraj.h"

enum
{
    SPELL_SWEEP             = 26103,
    SPELL_SANDBLAST         = 26102,
    SPELL_GROUND_RUPTURE    = 26100,
    SPELL_BIRTH             = 26262,                        //The Birth Animation
    SPELL_BOULDER           = 26616,
    SPELL_BERSERK           = 26615,

    SPELL_SUMMON_SCARABS    = 26060,
    SPELL_SUMMON_OURO_MOUND = 26058,
    SPELL_SUMMON_OURO       = 26642,

    SPELL_DIRTMOUND_PASSIVE = 26092,
    SPELL_SUBMERGE_VISUAL   = 26063,

    NPC_OURO_SCARAB         = 15718,
    NPC_OURO_SPAWNER        = 15957,
    NPC_OURO_TRIGGER        = 15717
};

struct MANGOS_DLL_DECL boss_ouroAI : public ScriptedAI
{
    boss_ouroAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiSweepTimer;
    uint32 m_uiSandBlastTimer;
    uint32 m_uiSubmergeTimer;
    uint32 m_uiBackTimer;
    uint32 m_uiChangeTargetTimer;
    uint32 m_uiSpawnTimer;

    bool m_bEnraged;
    bool m_bSubmerged;

    void Reset()
    {
        m_uiSweepTimer = urand(5000, 10000);
        m_uiSandBlastTimer = urand(20000, 35000);
        m_uiSubmergeTimer = urand(90000, 150000);
        m_uiBackTimer = urand(30000, 45000);
        m_uiChangeTargetTimer = urand(5000, 8000);
        m_uiSpawnTimer = urand(10000, 20000);

        m_bEnraged = false;
        m_bSubmerged = false;
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_BIRTH);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no pTarget
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bSubmerged)
        {
            // Sweep
            if (m_uiSweepTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SWEEP);
                m_uiSweepTimer = urand(15000, 30000);
            }
            else
                m_uiSweepTimer -= uiDiff;

            // Sand Blast
            if (m_uiSandBlastTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SANDBLAST);
                m_uiSandBlastTimer = urand(20000, 35000);
            }
            else
                m_uiSandBlastTimer -= uiDiff;

            if (!m_bEnraged)
            {
                if (m_creature->GetHealthPercent() < 20.0f)
                {
                    DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                    m_bEnraged = true;
                    return;
                }

                // Submerge
                if (m_uiSubmergeTimer < uiDiff)
                {
                    //Cast
                    m_creature->HandleEmote(EMOTE_ONESHOT_SUBMERGE);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->setFaction(35);
                    DoCastSpellIfCan(m_creature, SPELL_DIRTMOUND_PASSIVE);

                    m_bSubmerged = true;
                    m_uiBackTimer = urand(30000, 45000);
                }
                else
                    m_uiSubmergeTimer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }
        else
        {
            // Change Target
            if (m_uiChangeTargetTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_creature->GetMap()->CreatureRelocation(m_creature, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0.0f);
                    m_creature->SendMonsterMove(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), SPLINETYPE_NORMAL, SPLINEFLAG_WALKMODE, 1);
                }

                m_uiChangeTargetTimer = urand(10000, 20000);
            }
            else
                m_uiChangeTargetTimer -= uiDiff;

            // Back
            if (m_uiBackTimer < uiDiff)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->setFaction(14);

                DoCastSpellIfCan(m_creature->getVictim(), SPELL_GROUND_RUPTURE);

                m_bSubmerged = false;
                m_uiSubmergeTimer = urand(60000, 120000);
            }
            else
                m_uiBackTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_boss_ouro(Creature* pCreature)
{
    return new boss_ouroAI(pCreature);
}

void AddSC_boss_ouro()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_ouro";
    newscript->GetAI = &GetAI_boss_ouro;
    newscript->RegisterSelf();
}
