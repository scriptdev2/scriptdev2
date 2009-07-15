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
SDName: Boss_Emperor_Dagran_Thaurissan
SD%Complete: 90
SDCategory: Blackrock Depths
EndScriptData */

#include "precompiled.h"
#include "def_blackrock_depths.h"

enum
{
    FACTION_NEUTRAL             = 734,
    SAY_AGGRO                   = -1230001,
    SAY_SLAY                    = -1230002,

    SPELL_HANDOFTHAURISSAN      = 17492,
    SPELL_AVATAROFFLAME         = 15636
};

struct MANGOS_DLL_DECL boss_draganthaurissanAI : public ScriptedAI
{
    boss_draganthaurissanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiHandOfThaurissan_Timer;
    uint32 m_uiAvatarOfFlame_Timer;
    Creature* pPrincess;
    //uint32 m_uiCounter;

    void Reset()
    {
        m_uiHandOfThaurissan_Timer = 4000;
        m_uiAvatarOfFlame_Timer = 25000;
        pPrincess = NULL;
        //m_uiCounter= 0;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->CallForHelp(VISIBLE_RANGE);

        if (m_pInstance)
            pPrincess = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_PRINCESS));
    }

    void JustDied(Unit* pVictim)
    {
        if (pPrincess)
            if (pPrincess->isAlive() && !pPrincess->isDead()) // is there a difference between isalive and !isdead?
            {
                pPrincess->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pPrincess->setFaction(FACTION_NEUTRAL);
                pPrincess->AI()->EnterEvadeMode();
            }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_uiHandOfThaurissan_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget,SPELL_HANDOFTHAURISSAN);

            //3 Hands of Thaurissan will be casted
            //if (m_uiCounter < 3)
            //{
            //    m_uiHandOfThaurissan_Timer = 1000;
            //    m_uiCounter++;
            //}
            //else
            //{
                m_uiHandOfThaurissan_Timer = 5000;
                //Counter=0;
            //}
        }
        else
            m_uiHandOfThaurissan_Timer -= uiDiff;

        //AvatarOfFlame_Timer
        if (m_uiAvatarOfFlame_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_AVATAROFFLAME);
            m_uiAvatarOfFlame_Timer = 18000;
        }
        else
            m_uiAvatarOfFlame_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_draganthaurissan(Creature* pCreature)
{
    return new boss_draganthaurissanAI(pCreature);
}

void AddSC_boss_draganthaurissan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_emperor_dagran_thaurissan";
    newscript->GetAI = &GetAI_boss_draganthaurissan;
    newscript->RegisterSelf();
}
