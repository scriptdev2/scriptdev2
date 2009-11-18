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
SDName: Boss_Ambassador_Hellmaw
SD%Complete: 80
SDComment: Enrage spell missing/not known
SDCategory: Auchindoun, Shadow Labyrinth
EndScriptData */

#include "precompiled.h"
#include "shadow_labyrinth.h"
#include "escort_ai.h"

#define SAY_INTRO       -1555000

#define SAY_AGGRO1      -1555001
#define SAY_AGGRO2      -1555002
#define SAY_AGGRO3      -1555003

#define SAY_HELP        -1555004

#define SAY_SLAY1       -1555005
#define SAY_SLAY2       -1555006

#define SAY_DEATH       -1555007

#define SPELL_BANISH            30231
#define SPELL_CORROSIVE_ACID    33551
#define SPELL_FEAR              33547
#define SPELL_ENRAGE            34970

struct MANGOS_DLL_DECL boss_ambassador_hellmawAI : public npc_escortAI
{
    boss_ambassador_hellmawAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

    uint32 EventCheck_Timer;
    uint32 CorrosiveAcid_Timer;
    uint32 Fear_Timer;
    uint32 Enrage_Timer;
    bool Intro;
    bool IsBanished;
    bool Enraged;

    void Reset()
    {
        EventCheck_Timer = 5000;
        CorrosiveAcid_Timer = urand(5000, 10000);
        Fear_Timer = urand(25000, 30000);
        Enrage_Timer = 180000;
        Intro = false;
        IsBanished = true;
        Enraged = false;

        if (m_pInstance && m_creature->isAlive())
        {
            if (m_pInstance->GetData(TYPE_OVERSEER) != DONE)
                m_creature->CastSpell(m_creature, SPELL_BANISH, true);
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HELLMAW, FAIL);
    }

    void WaypointReached(uint32 i)
    {
    }

    void DoIntro()
    {
        if (m_creature->HasAura(SPELL_BANISH,0))
            m_creature->RemoveAurasDueToSpell(SPELL_BANISH);

        IsBanished = false;
        Intro = true;

        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_HELLMAW) != FAIL)
            {
                DoScriptText(SAY_INTRO, m_creature);
                Start(true, false, 0, NULL, false, true);
            }

            m_pInstance->SetData(TYPE_HELLMAW, IN_PROGRESS);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (m_creature->HasAura(SPELL_BANISH,0))
            return;

        npc_escortAI::MoveInLineOfSight(who);
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HELLMAW, DONE);
    }

    void UpdateEscortAI(const uint32 diff)
    {
        if (!Intro && !HasEscortState(STATE_ESCORT_ESCORTING))
        {
            if (EventCheck_Timer < diff)
            {
                if (m_pInstance)
                {
                    if (m_pInstance->GetData(TYPE_OVERSEER) == DONE)
                    {
                        DoIntro();
                        return;
                    }
                }
                EventCheck_Timer = 5000;
                return;
            }
            else
            {
                EventCheck_Timer -= diff;
                return;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (CorrosiveAcid_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_CORROSIVE_ACID);
            CorrosiveAcid_Timer = urand(15000, 25000);
        }else CorrosiveAcid_Timer -= diff;

        if (Fear_Timer < diff)
        {
            DoCast(m_creature,SPELL_FEAR);
            Fear_Timer = urand(20000, 35000);
        }else Fear_Timer -= diff;

        if (m_bIsHeroicMode)
        {
            if (!Enraged && Enrage_Timer < diff)
            {
                DoCast(m_creature,SPELL_ENRAGE);
                Enraged = true;
            }else Enrage_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ambassador_hellmaw(Creature* pCreature)
{
    return new boss_ambassador_hellmawAI(pCreature);
}

void AddSC_boss_ambassador_hellmaw()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ambassador_hellmaw";
    newscript->GetAI = &GetAI_boss_ambassador_hellmaw;
    newscript->RegisterSelf();
}
