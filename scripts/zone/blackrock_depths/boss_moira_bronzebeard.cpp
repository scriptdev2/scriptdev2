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
SDName: Boss_Moira_Bronzbeard
SD%Complete: 95
SDCategory: Blackrock Depths
EndScriptData */

#include "precompiled.h"
#include "def_blackrock_depths.h"

enum
{
    SPELL_HEAL                  = 15586,
    SPELL_RENEW                 = 10929,
    SPELL_SHIELD                = 10901,
    SPELL_MINDBLAST             = 15587,
    SPELL_SHADOWWORDPAIN        = 15654,
    SPELL_SMITE                 = 10934
};

struct MANGOS_DLL_DECL boss_moira_bronzebeardAI : public ScriptedAI
{
    boss_moira_bronzebeardAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiHeal_Timer;
    uint32 m_uiMindBlast_Timer;
    uint32 m_uiShadowWordPain_Timer;
    uint32 m_uiSmite_Timer;

    Creature* pEmperor;

    void Reset()
    {
        pEmperor = NULL;

        m_uiHeal_Timer = 12000;                                 //These times are probably wrong
        m_uiMindBlast_Timer = 16000;
        m_uiShadowWordPain_Timer = 2000;
        m_uiSmite_Timer = 8000;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            pEmperor = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_EMPEROR));

        if (pEmperor)
            pEmperor->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //MindBlast_Timer
        if (m_uiMindBlast_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_MINDBLAST);
            m_uiMindBlast_Timer = 14000;
        }
        else
            m_uiMindBlast_Timer -= uiDiff;

        //ShadowWordPain_Timer
        if (m_uiShadowWordPain_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_SHADOWWORDPAIN);
            m_uiShadowWordPain_Timer = 18000;
        }
        else
            m_uiShadowWordPain_Timer -= uiDiff;

        //Smite_Timer
        if (m_uiSmite_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_SMITE);
            m_uiSmite_Timer = 10000;
        }
        else
            m_uiSmite_Timer -= uiDiff;

        //Heal_Timer
        if (m_uiHeal_Timer < uiDiff && pEmperor)
        {
            DoCast(pEmperor,SPELL_HEAL);
            m_uiHeal_Timer = 10000;
        }
        else
            m_uiHeal_Timer -= uiDiff;

        //No meele?
    }
};

CreatureAI* GetAI_boss_moira_bronzebeard(Creature* pCreature)
{
    return new boss_moira_bronzebeardAI(pCreature);
}

void AddSC_boss_moira_bronzebeard()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_moira_bronzebeard";
    newscript->GetAI = &GetAI_boss_moira_bronzebeard;
    newscript->RegisterSelf();
}
