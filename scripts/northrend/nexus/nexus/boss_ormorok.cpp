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
SDName: Boss_Ormorok
SD%Complete: 50%
SDComment: TODO: Correct timers. Research how spikes work, and attempt code it properly from mangos side.
SDCategory: Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum
{
    SAY_AGGRO                   = -1576011,
    SAY_KILL                    = -1576012,
    SAY_REFLECT                 = -1576013,
    SAY_ICESPIKE                = -1576014,
    SAY_DEATH                   = -1576015,
    EMOTE_BOSS_GENERIC_FRENZY   = -1000005,

    SPELL_REFLECTION            = 47981,

    SPELL_CRYSTAL_SPIKES        = 47958,
    SPELL_CRYSTAL_SPIKES_H1     = 57082,
    SPELL_CRYSTAL_SPIKES_H2     = 57083,

    SPELL_FRENZY                = 48017,
    SPELL_FRENZY_H              = 57086,

    SPELL_TRAMPLE               = 48016,
    SPELL_TRAMPLE_H             = 57066,

    SPELL_SUMMON_TANGLER_H      = 61564
};

/*######
## boss_ormorok
######*/

struct MANGOS_DLL_DECL boss_ormorokAI : public ScriptedAI
{
    boss_ormorokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

    bool m_bIsEnraged;

    uint32 m_uiTrampleTimer;
    uint32 m_uiSpellReflectTimer;
    uint32 m_uiCrystalSpikeTimer;
    uint32 m_uiTanglerTimer;

    void Reset()
    {
        m_bIsEnraged = false;

        m_uiTrampleTimer = urand(10000, 35000);
        m_uiSpellReflectTimer = urand(5000, 10000);
        m_uiCrystalSpikeTimer = urand(15000, 30000);
        m_uiTanglerTimer = 20000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ORMOROK, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsEnraged && m_creature->GetHealth()*100 < m_creature->GetMaxHealth()*25)
        {
            if (!m_creature->IsNonMeleeSpellCasted(false))
            {
                m_bIsEnraged = true;
                DoScriptText(EMOTE_BOSS_GENERIC_FRENZY, m_creature);
                DoCast(m_creature, m_bIsHeroicMode ? SPELL_FRENZY_H : SPELL_FRENZY);
            }
        }

        if (m_uiTrampleTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsHeroicMode ? SPELL_TRAMPLE_H : SPELL_TRAMPLE);
            m_uiTrampleTimer = urand(10000, 35000);
        }
        else
            m_uiTrampleTimer -= uiDiff;

        if (m_uiSpellReflectTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_REFLECTION);
            m_uiSpellReflectTimer = urand(25000, 40000);
        }
        else
            m_uiSpellReflectTimer -= uiDiff;

        if (m_uiCrystalSpikeTimer < uiDiff)
        {
            DoScriptText(SAY_ICESPIKE, m_creature);
            DoCast(m_creature, SPELL_CRYSTAL_SPIKES);
            m_uiCrystalSpikeTimer = urand(15000, 30000);
        }
        else
            m_uiCrystalSpikeTimer -= uiDiff;

        if (m_bIsHeroicMode)
        {
            if (m_uiTanglerTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_SUMMON_TANGLER_H);
                m_uiTanglerTimer = urand(15000, 25000);
            }
            else
                m_uiTanglerTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ormorok(Creature* pCreature)
{
    return new boss_ormorokAI(pCreature);
}

void AddSC_boss_ormorok()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ormorok";
    newscript->GetAI = &GetAI_boss_ormorok;
    newscript->RegisterSelf();
}
