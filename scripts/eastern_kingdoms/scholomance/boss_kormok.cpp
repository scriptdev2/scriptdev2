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
SDName: Boss_Kormok
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "precompiled.h"

enum
{
    EMOTE_GENERIC_FRENZY        = -1000002,

    SPELL_SHADOWBOLT_VOLLEY     = 20741,
    SPELL_BONE_SHIELD           = 27688,
    SPELL_BLOODLUST             = 27689,
    SPELL_SUMMON_BONE_MAGES     = 27695,        // triggers 27696, 27697, 27698, 27699
    SPELL_SUMMON_BONE_MINIONS   = 27687,        // triggers 27690, 27691, 27692, 27693 (spells were removed after vanilla)
    SPELL_FRENZY                = 8269,
};

struct MANGOS_DLL_DECL boss_kormokAI : public ScriptedAI
{
    boss_kormokAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiShadowVolleyTimer;
    uint32 m_uiBoneShieldTimer;
    uint32 m_uiMinionTimer;
    uint32 m_uiBloodlustTimer;
    bool m_bHasMages;
    bool m_bIsFrenzy;

    void Reset()
    {
        m_uiShadowVolleyTimer   = urand(10000, 12000);
        m_uiMinionTimer         = 15000;
        m_uiBloodlustTimer      = urand(20000, 25000);
        m_bHasMages             = false;
        m_bIsFrenzy             = false;

        // the boss casts this on spawn because he is summoned
        DoCastSpellIfCan(m_creature, SPELL_BONE_SHIELD);
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_BONE_SHIELD, CAST_AURA_NOT_PRESENT);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // ShadowVolley_Timer
        if (m_uiShadowVolleyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHADOWBOLT_VOLLEY) == CAST_OK)
                m_uiShadowVolleyTimer = 15000;
        }
        else
            m_uiShadowVolleyTimer -= uiDiff;

        // Minion_Timer
        if (m_uiMinionTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_BONE_MINIONS) == CAST_OK)
                m_uiMinionTimer = urand(12000, 15000);
        }
        else
            m_uiMinionTimer -= uiDiff;

        // Bloodlust
        if (m_uiBloodlustTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BLOODLUST) == CAST_OK)
                m_uiBloodlustTimer = urand(25000, 30000);
        }
        else
            m_uiBloodlustTimer -= uiDiff;

        // Summon Bone Mages
        if (!m_bHasMages && m_creature->GetHealthPercent() < 26.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_BONE_MAGES) == CAST_OK)
                m_bHasMages = true;
        }

        // Frenzy
        if (!m_bIsFrenzy && m_creature->GetHealthPercent() < 20.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FRENZY) == CAST_OK)
            {
                DoScriptText(EMOTE_GENERIC_FRENZY, m_creature);
                m_bIsFrenzy = true;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kormok(Creature* pCreature)
{
    return new boss_kormokAI(pCreature);
}

void AddSC_boss_kormok()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_kormok";
    pNewScript->GetAI = &GetAI_boss_kormok;
    pNewScript->RegisterSelf();
}
