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
SDName: Boss_Keristrasza
SD%Complete: 65%
SDComment: timers tuning, add achievement
SDCategory: Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum
{
    SAY_AGGRO                   = -1576016,
    SAY_CRYSTAL_NOVA            = -1576017,
    SAY_ENRAGE                  = -1576018,
    SAY_KILL                    = -1576019,
    SAY_DEATH                   = -1576020,

    SPELL_INTENSE_COLD          = 48094,

    SPELL_CRYSTALFIRE_BREATH    = 48096,
    SPELL_CRYSTALFIRE_BREATH_H  = 57091,

    SPELL_CRYSTALLIZE           = 48179,

    SPELL_CRYSTAL_CHAINS        = 50997,

    SPELL_TAIL_SWEEP            = 50155,

    SPELL_ENRAGE                = 8599
};

/*######
## boss_keristrasza
######*/

struct MANGOS_DLL_DECL boss_keristraszaAI : public ScriptedAI
{
    boss_keristraszaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 uiCrystalChainTimer;
    uint32 uiTailSweepTimer;
    uint32 uiCrystalfireBreathTimer;
    uint32 uiCrystallizeTimer;

    bool m_bIsEnraged;

    void Reset()
    {
        uiCrystalChainTimer = 30000;
        uiTailSweepTimer = urand(5000, 7500);
        uiCrystalfireBreathTimer = urand(10000, 20000);
        uiCrystallizeTimer = urand(20000, 30000);

        m_bIsEnraged = false;

        if (!m_pInstance)
            return;

        if (m_creature->isAlive())
        {
            if (m_pInstance->GetData(TYPE_KERISTRASZA) != SPECIAL)
                m_creature->CastSpell(m_creature, SPELL_FROZEN_PRISON, true);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        m_creature->CastSpell(m_creature, SPELL_INTENSE_COLD, true);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KERISTRASZA, DONE);
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

        if (!m_bIsEnraged && m_creature->GetHealthPercent() < 25.0f)
        {
            if (!m_creature->IsNonMeleeSpellCasted(false))
            {
                m_bIsEnraged = true;
                DoScriptText(SAY_ENRAGE, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            }
        }

        if (uiCrystalChainTimer < uiDiff)
        {
            if (!m_creature->IsNonMeleeSpellCasted(false))
            {
                if (m_bIsRegularMode)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                    {
                        if (Player* pPlayer = pTarget->GetCharmerOrOwnerPlayerOrPlayerItself())
                            DoCastSpellIfCan(pPlayer, SPELL_CRYSTAL_CHAINS);

                        uiCrystalChainTimer = 30000;
                    }
                }
                else
                {
                    if (Unit* pSource = m_creature->getVictim())
                    {
                        uiCrystalChainTimer = 15000;

                        Player* pPlayer = pSource->GetCharmerOrOwnerPlayerOrPlayerItself();

                        if (!pPlayer)
                            return;

                        if (Group* pGroup = pPlayer->GetGroup())
                        {
                            for(GroupReference* pRef = pGroup->GetFirstMember(); pRef != NULL; pRef = pRef->next())
                            {
                                if (Player* pMember = pRef->getSource())
                                {
                                    if (pMember->isAlive() && pMember->IsWithinDistInMap(m_creature, 50.0f))
                                        m_creature->CastSpell(pMember, SPELL_CRYSTAL_CHAINS, true);
                                }
                            }
                        }
                        else
                            m_creature->CastSpell(pPlayer, SPELL_CRYSTAL_CHAINS, false);
                    }
                }
            }
        }
        else
            uiCrystalChainTimer -= uiDiff;

        if (uiTailSweepTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_TAIL_SWEEP) == CAST_OK)
                uiTailSweepTimer = urand(2500, 7500);
        }
        else
            uiCrystalChainTimer -= uiDiff;

        if (uiCrystalfireBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_CRYSTALFIRE_BREATH : SPELL_CRYSTALFIRE_BREATH_H) == CAST_OK)
                uiCrystalfireBreathTimer = urand(15000, 20000);
        }
        else
            uiCrystalfireBreathTimer -= uiDiff;

        if (!m_bIsRegularMode)
        {
            if (uiCrystallizeTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_CRYSTALLIZE) == CAST_OK)
                {
                    uiCrystallizeTimer = urand(15000, 25000);
                    DoScriptText(SAY_CRYSTAL_NOVA, m_creature);
                }
            }
            else
                uiCrystallizeTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_keristrasza(Creature* pCreature)
{
    return new boss_keristraszaAI(pCreature);
}

void AddSC_boss_keristrasza()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_keristrasza";
    newscript->GetAI = &GetAI_boss_keristrasza;
    newscript->RegisterSelf();
}
