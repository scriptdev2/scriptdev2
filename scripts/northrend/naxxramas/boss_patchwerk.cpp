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
SDComment: Some issues with hateful strike inturrupting the melee swing timer.
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
    H_SPELL_HATEFULSTRIKE = 59192,
    SPELL_ENRAGE          = 28131,
    SPELL_BERSERK         = 26662,
    SPELL_SLIMEBOLT       = 32309
};

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

    uint32 m_uiHatefullStrikeTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiSlimeboltTimer;
    bool m_bEnraged;

    void Reset()
    {
        m_uiHatefullStrikeTimer = 1200;                     //1.2 seconds
        m_uiEnrageTimer = 420000;                           //7 minutes 420,000
        m_uiSlimeboltTimer = 450000;                        //7.5 minutes 450,000
        m_bEnraged = false;
    }

    void KilledUnit(Unit* Victim)
    {
        if (urand(0, 4))
            return;

        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PATCHWERK, DONE);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(urand(0, 1)?SAY_AGGRO1:SAY_AGGRO2, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PATCHWERK, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        // Hatefull Strike
        if (m_uiHatefullStrikeTimer < uiDiff)
        {
            //Cast Hateful strike on the player with the highest
            //amount of HP within melee distance
            uint32 MostHP = 0;
            Unit* pMostHPTarget = NULL;
            Unit* pTemp = NULL;
            std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();

            for (i = m_creature->getThreatManager().getThreatList().begin(); i!=m_creature->getThreatManager().getThreatList().end(); ++i)
            {
                pTemp = Unit::GetUnit((*m_creature),(*i)->getUnitGuid());
                if (pTemp && pTemp->isAlive() && pTemp->GetHealth() > MostHP && m_creature->IsWithinDist(pTemp, 5.0f, false))
                {
                    MostHP = pTemp->GetHealth();
                    pMostHPTarget = pTemp;
                }
            }

            if (pMostHPTarget)
                DoCast(pMostHPTarget, SPELL_HATEFULSTRIKE);

            m_uiHatefullStrikeTimer = 1200;
        }
        else
            m_uiHatefullStrikeTimer -= uiDiff;

        // Enrage
        if (m_uiEnrageTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(EMOTE_BERSERK, m_creature);
            m_uiEnrageTimer = 300000;
        }
        else
            m_uiEnrageTimer -= uiDiff;

        // Slimebolt
        if (m_uiSlimeboltTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SLIMEBOLT);
            m_uiSlimeboltTimer = 5000;
        }
        else
            m_uiSlimeboltTimer -= uiDiff;

        //Enrage if not already enraged and below 5%
        if (!m_bEnraged && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 5)
        {
            DoCast(m_creature, SPELL_ENRAGE);
            DoScriptText(EMOTE_ENRAGE, NULL);
            m_bEnraged = true;
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
