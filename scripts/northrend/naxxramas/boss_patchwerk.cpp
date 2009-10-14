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
SDName: Boss_Patchwerk
SD%Complete: 80
SDComment: TODO: confirm how hateful strike work
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1            = -1533017,
    SAY_AGGRO2            = -1533018,
    SAY_SLAY              = -1533019,
    SAY_DEATH             = -1533020,

    EMOTE_BERSERK         = -1533021,
    EMOTE_ENRAGE          = -1533022,

    SPELL_HATEFULSTRIKE   = 28308,
    SPELL_HATEFULSTRIKE_H = 59192,
    SPELL_ENRAGE          = 28131,
    SPELL_BERSERK         = 26662,
    SPELL_SLIMEBOLT       = 32309
};

const float MELEE_DISTANCE = 5.0;

struct MANGOS_DLL_DECL boss_patchwerkAI : public ScriptedAI
{
    boss_patchwerkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsHeroic();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

    uint32 m_uiHatefulStrikeTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiSlimeboltTimer;
    bool   m_bEnraged;
    bool   m_bBerserk;

    void Reset()
    {
        m_uiHatefulStrikeTimer = 1000;                      //1 second
        m_uiBerserkTimer = MINUTE*6*IN_MILISECONDS;         //6 minutes
        m_uiSlimeboltTimer = 10000;
        m_bEnraged = false;
        m_bBerserk = false;
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 4))
            return;

        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PATCHWERK, DONE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(urand(0, 1)?SAY_AGGRO1:SAY_AGGRO2, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PATCHWERK, IN_PROGRESS);
    }

    void DoHatefulStrike()
    {
        // The ability is used on highest HP target choosen of the top 2 (3 heroic) targets on threat list being in melee range
        Unit* pTarget = NULL;
        uint32 uiHighestHP = 0;
        uint32 uiTargets = m_bIsHeroicMode?3:2;

        std::list<HostileReference*>::iterator iter = m_creature->getThreatManager().getThreatList().begin();
        for (iter = m_creature->getThreatManager().getThreatList().begin(); iter != m_creature->getThreatManager().getThreatList().end(); ++iter)
        {
            if (!uiTargets)
                return;

            if (Unit* pTempTarget = Unit::GetUnit((*m_creature), (*iter)->getUnitGuid()))
            {
                if (pTempTarget->GetHealth() > uiHighestHP && m_creature->IsWithinDistInMap(pTempTarget, MELEE_DISTANCE))
                {
                    uiHighestHP = pTempTarget->GetHealth();
                    pTarget = pTempTarget;
                }
            }

            --uiTargets;
        }

        if (pTarget)
            DoCast(pTarget, m_bIsHeroicMode?SPELL_HATEFULSTRIKE_H:SPELL_HATEFULSTRIKE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Hateful Strike
        if (m_uiHatefulStrikeTimer < uiDiff)
        {
            DoHatefulStrike();
            m_uiHatefulStrikeTimer = 1000;
        }
        else
            m_uiHatefulStrikeTimer -= uiDiff;

        // Soft Enrage at 5%
        if (!m_bEnraged)
        {
            if (m_creature->GetHealth()*20 < m_creature->GetMaxHealth())
            {
                DoCast(m_creature, SPELL_ENRAGE);
                DoScriptText(EMOTE_ENRAGE, m_creature);
                m_bEnraged = true;
            }
        }

        // Berserk after 6 minutes
        if (!m_bBerserk)
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_BERSERK);
                DoScriptText(EMOTE_BERSERK, m_creature);
                m_bBerserk = true;
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }
        else
        {
            // Slimebolt - casted only while Berserking to prevent kiting
            if (m_uiSlimeboltTimer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_SLIMEBOLT);
                m_uiSlimeboltTimer = 5000;
            }
            else
                m_uiSlimeboltTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_patchwerk(Creature* pCreature)
{
    return new boss_patchwerkAI(pCreature);
}

void AddSC_boss_patchwerk()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_patchwerk";
    NewScript->GetAI = &GetAI_boss_patchwerk;
    NewScript->RegisterSelf();
}
