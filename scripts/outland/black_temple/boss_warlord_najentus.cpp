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
SDName: Boss_Warlord_Najentus
SD%Complete: 90
SDComment: Does the GO need script? Uncomment code to test.
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "black_temple.h"

enum
{
    SAY_AGGRO                       = -1564000,
    SAY_NEEDLE1                     = -1564001,
    SAY_NEEDLE2                     = -1564002,
    SAY_SLAY1                       = -1564003,
    SAY_SLAY2                       = -1564004,
    SAY_SPECIAL1                    = -1564005,
    SAY_SPECIAL2                    = -1564006,
    SAY_ENRAGE1                     = -1564007,             //is this text actually in use?
    SAY_ENRAGE2                     = -1564008,
    SAY_DEATH                       = -1564009,

    SPELL_CRASHINGWAVE              = 40100,
    SPELL_NEEDLE_SPINE              = 39835,
    SPELL_NEEDLE_AOE                = 39968,
    SPELL_TIDAL_BURST               = 39878,
    SPELL_TIDAL_SHIELD              = 39872,                // Not going to use this since Hurl Spine doesn't dispel it.
    SPELL_IMPALING_SPINE            = 39837,
    SPELL_CREATE_NAJENTUS_SPINE     = 39956,
    SPELL_HURL_SPINE                = 39948,
    SPELL_BERSERK                   = 26662
};

struct MANGOS_DLL_DECL boss_najentusAI : public ScriptedAI
{
    boss_najentusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiNeedleSpineTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiSpecialYellTimer;
    uint32 m_uiTidalShieldTimer;
    uint32 m_uiImpalingSpineTimer;

    bool m_bIsShielded;

    void Reset()
    {
        m_bIsShielded = false;

        m_uiNeedleSpineTimer = 10000;
        m_uiEnrageTimer = MINUTE*8*IN_MILLISECONDS;
        m_uiSpecialYellTimer = urand(45000, 120000);
        m_uiTidalShieldTimer = 60000;
        m_uiImpalingSpineTimer = 20000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NAJENTUS, NOT_STARTED);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit *victim)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NAJENTUS, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (m_bIsShielded)
        {
            if (spell->Id == SPELL_HURL_SPINE)
            {
                if (m_creature->HasAura(SPELL_TIDAL_SHIELD, EFFECT_INDEX_0))
                    m_creature->RemoveAurasDueToSpell(SPELL_TIDAL_SHIELD);

                DoCastSpellIfCan(m_creature->getVictim(), SPELL_TIDAL_BURST);
                m_bIsShielded = false;
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NAJENTUS, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEnrageTimer < diff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);

            DoScriptText(SAY_ENRAGE2, m_creature);
            DoCastSpellIfCan(m_creature, SPELL_BERSERK);
            m_uiEnrageTimer = MINUTE*8*IN_MILLISECONDS;
        }else m_uiEnrageTimer -= diff;

        if (m_bIsShielded)
        {
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();

            return;                                         // Don't cast or do anything while Shielded
        }

        // Needle
        if (m_uiNeedleSpineTimer < diff)
        {
            for(uint8 i = 0; i < 3; ++i)
            {
                Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);

                if (!target)
                    target = m_creature->getVictim();

                DoCastSpellIfCan(target, SPELL_NEEDLE_SPINE);
                target->CastSpell(target, SPELL_NEEDLE_AOE, false);
            }

            m_uiNeedleSpineTimer = 3000;
        }else m_uiNeedleSpineTimer -= diff;

        if (m_uiSpecialYellTimer < diff)
        {
            DoScriptText(urand(0, 1) ? SAY_SPECIAL1 : SAY_SPECIAL2, m_creature);
            m_uiSpecialYellTimer = urand(25000, 100000);
        }else m_uiSpecialYellTimer -= diff;

        if (m_uiImpalingSpineTimer < diff)
        {
            Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);

            if (!target)
                target = m_creature->getVictim();

            if (target && (target->GetTypeId() == TYPEID_PLAYER))
            {
                DoCastSpellIfCan(target, SPELL_IMPALING_SPINE);
                m_uiImpalingSpineTimer = 20000;

                DoScriptText(urand(0, 1) ? SAY_NEEDLE1 : SAY_NEEDLE2, m_creature);
            }
        }else m_uiImpalingSpineTimer -= diff;

        if (m_uiTidalShieldTimer < diff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCastSpellIfCan(m_creature, SPELL_TIDAL_SHIELD, CAST_TRIGGERED);

            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();

            m_bIsShielded = true;
            m_uiTidalShieldTimer = 60000;
        }else m_uiTidalShieldTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_najentus(Creature* pCreature)
{
    return new boss_najentusAI(pCreature);
}

void AddSC_boss_najentus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_najentus";
    newscript->GetAI = &GetAI_boss_najentus;
    newscript->RegisterSelf();
}
