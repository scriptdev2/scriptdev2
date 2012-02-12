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
SDName: Boss_Nerubenkan
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_ENCASING_WEBS         = 4962,
    SPELL_PIERCE_ARMOR          = 6016,
    SPELL_CRYPT_SCARABS         = 31602,
    SPELL_RAISE_UNDEAD_SCARAB   = 17235
};

struct MANGOS_DLL_DECL boss_nerubenkanAI : public ScriptedAI
{
    boss_nerubenkanAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiEncasingWebsTimer;
    uint32 m_uiPierceArmorTimer;
    uint32 m_uiCryptScarabsTimer;
    uint32 m_uiRaiseUndeadScarabTimer;

    void Reset()
    {
        m_uiCryptScarabsTimer       = 3000;
        m_uiEncasingWebsTimer       = 7000;
        m_uiPierceArmorTimer        = 19000;
        m_uiRaiseUndeadScarabTimer  = 3000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // EncasingWebs
        if (m_uiEncasingWebsTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_ENCASING_WEBS) == CAST_OK)
                    m_uiEncasingWebsTimer = 30000;
            }
        }
        else
            m_uiEncasingWebsTimer -= uiDiff;

        // PierceArmor
        if (m_uiPierceArmorTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_PIERCE_ARMOR) == CAST_OK)
                m_uiPierceArmorTimer = 35000;
        }
        else
            m_uiPierceArmorTimer -= uiDiff;

        // CryptScarabs
        if (m_uiCryptScarabsTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_CRYPT_SCARABS) == CAST_OK)
                    m_uiCryptScarabsTimer = 16000;
            }
        }
        else
            m_uiCryptScarabsTimer -= uiDiff;

        // RaiseUndeadScarab
        if (m_uiRaiseUndeadScarabTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_RAISE_UNDEAD_SCARAB) == CAST_OK)
                m_uiRaiseUndeadScarabTimer = 18000;
        }
        else
            m_uiRaiseUndeadScarabTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_nerubenkan(Creature* pCreature)
{
    return new boss_nerubenkanAI(pCreature);
}

void AddSC_boss_nerubenkan()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_nerubenkan";
    pNewScript->GetAI = &GetAI_boss_nerubenkan;
    pNewScript->RegisterSelf();
}
