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
SDName: Boss_Maiden_of_Virtue
SD%Complete: 100
SDComment:
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO               = -1532018,
    SAY_SLAY1               = -1532019,
    SAY_SLAY2               = -1532020,
    SAY_SLAY3               = -1532021,
    SAY_REPENTANCE1         = -1532022,
    SAY_REPENTANCE2         = -1532023,
    SAY_DEATH               = -1532024,

    SPELL_REPENTANCE        = 29511,
    SPELL_HOLYFIRE          = 29522,
    SPELL_HOLYWRATH         = 32445,
    SPELL_HOLYGROUND        = 29512
};

struct MANGOS_DLL_DECL boss_maiden_of_virtueAI : public ScriptedAI
{
    boss_maiden_of_virtueAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiRepentance_Timer;
    uint32 m_uiHolyfire_Timer;
    uint32 m_uiHolywrath_Timer;
    uint32 m_uiHolyground_Timer;

    void Reset()
    {
        m_uiRepentance_Timer    = urand(25000, 40000);
        m_uiHolyfire_Timer      = urand(8000, 25000);
        m_uiHolywrath_Timer     = urand(15000, 25000);
        m_uiHolyground_Timer    = 3000;
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 5))                                    // 50% chance to say something out of 3 texts
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void Aggro(Unit *pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHolyground_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_HOLYGROUND, CAST_TRIGGERED);     //Triggered so it doesn't interrupt her at all
            m_uiHolyground_Timer = 3000;
        }
        else
            m_uiHolyground_Timer -= uiDiff;

        if (m_uiRepentance_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_REPENTANCE) == CAST_OK)
            {
                DoScriptText(urand(0, 1) ? SAY_REPENTANCE1 : SAY_REPENTANCE2, m_creature);

                //A little randomness on that spell
                m_uiRepentance_Timer = urand(25000, 35000);
            }
        }
        else
            m_uiRepentance_Timer -= uiDiff;

        if (m_uiHolyfire_Timer < uiDiff)
        {
            //Time for an omgwtfpwn code to make maiden cast holy fire only on units outside the holy ground's 18 yard range
            Unit* pTarget = NULL;
            std::vector<Unit *> target_list;

            ThreatList const& tList = m_creature->getThreatManager().getThreatList();
            for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
            {
                pTarget = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());

                if (pTarget && !pTarget->IsWithinDist(m_creature, 12.0f, false))
                    target_list.push_back(pTarget);

                pTarget = NULL;
            }

            if (target_list.size())
            {
                if (pTarget = *(target_list.begin()+rand()%target_list.size()))
                    DoCastSpellIfCan(pTarget,SPELL_HOLYFIRE);
            }

            m_uiHolyfire_Timer = urand(8000, 23000);        //Anywhere from 8 to 23 seconds, good luck having several of those in a row!
        }
        else
            m_uiHolyfire_Timer -= uiDiff;

        if (m_uiHolywrath_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(pTarget, SPELL_HOLYWRATH);

            m_uiHolywrath_Timer = urand(20000, 25000);      //20-25 secs sounds nice
        }
        else
            m_uiHolywrath_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_maiden_of_virtue(Creature* pCreature)
{
    return new boss_maiden_of_virtueAI(pCreature);
}

void AddSC_boss_maiden_of_virtue()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_maiden_of_virtue";
    newscript->GetAI = &GetAI_boss_maiden_of_virtue;
    newscript->RegisterSelf();
}
