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
SDName: Boss_Anubshiah
SD%Complete: 100
SDComment:
SDCategory: Blackrock Depths
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_SHADOWBOLT            = 17228,
    SPELL_CURSEOFTONGUES        = 15470,
    SPELL_CURSEOFWEAKNESS       = 17227,
    SPELL_DEMONARMOR            = 11735,
    SPELL_ENVELOPINGWEB         = 15471
};

struct MANGOS_DLL_DECL boss_anubshiahAI : public ScriptedAI
{
    boss_anubshiahAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiShadowBoltTimer;
    uint32 m_uiCurseOfTonguesTimer;
    uint32 m_uiCurseOfWeaknessTimer;
    uint32 m_uiDemonArmorTimer;
    uint32 m_uiEnvelopingWebTimer;

    void Reset()
    {
        m_uiShadowBoltTimer = 7000;
        m_uiCurseOfTonguesTimer = 24000;
        m_uiCurseOfWeaknessTimer = 12000;
        m_uiDemonArmorTimer = 3000;
        m_uiEnvelopingWebTimer = 16000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //ShadowBolt_Timer
        if (m_uiShadowBoltTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWBOLT);
            m_uiShadowBoltTimer = 7000;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        //CurseOfTongues_Timer
        if (m_uiCurseOfTonguesTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_CURSEOFTONGUES);
            m_uiCurseOfTonguesTimer = 18000;
        }
        else
            m_uiCurseOfTonguesTimer -= uiDiff;

        //CurseOfWeakness_Timer
        if (m_uiCurseOfWeaknessTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CURSEOFWEAKNESS);
            m_uiCurseOfWeaknessTimer = 45000;
        }
        else
            m_uiCurseOfWeaknessTimer -= uiDiff;

        //DemonArmor_Timer
        if (m_uiDemonArmorTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DEMONARMOR);
            m_uiDemonArmorTimer = 300000;
        }
        else
            m_uiDemonArmorTimer -= uiDiff;

        //EnvelopingWeb_Timer
        if (m_uiEnvelopingWebTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_ENVELOPINGWEB);
            m_uiEnvelopingWebTimer = 12000;
        }
        else
            m_uiEnvelopingWebTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anubshiah(Creature* pCreature)
{
    return new boss_anubshiahAI(pCreature);
}

void AddSC_boss_anubshiah()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_anubshiah";
    pNewScript->GetAI = &GetAI_boss_anubshiah;
    pNewScript->RegisterSelf();
}
