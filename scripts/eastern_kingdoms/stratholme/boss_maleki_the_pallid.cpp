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
SDName: Boss_Maleki_the_Pallid
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"
#include "stratholme.h"

#define SPELL_FROSTBOLT     17503
#define SPELL_DRAIN_LIFE    17238
#define SPELL_DRAIN_MANA    17243
#define SPELL_ICETOMB       16869

struct MANGOS_DLL_DECL boss_maleki_the_pallidAI : public ScriptedAI
{
    boss_maleki_the_pallidAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 FrostNova_Timer;
    uint32 Frostbolt_Timer;
    uint32 IceTomb_Timer;
    uint32 DrainLife_Timer;

    void Reset()
    {
        FrostNova_Timer = 11000;
        Frostbolt_Timer = 1000;
        IceTomb_Timer = 16000;
        DrainLife_Timer = 31000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Frostbolt
        if (Frostbolt_Timer < diff)
        {
            if (rand()%100 < 90)
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROSTBOLT);

            Frostbolt_Timer = 3500;
        }else Frostbolt_Timer -= diff;

        //IceTomb
        if (IceTomb_Timer < diff)
        {
            if (rand()%100 < 65)
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_ICETOMB);

            IceTomb_Timer = 28000;
        }else IceTomb_Timer -= diff;

        //DrainLife
        if (DrainLife_Timer < diff)
        {
            if (rand()%100 < 55)
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_DRAIN_LIFE);

            DrainLife_Timer = 31000;
        }else DrainLife_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_maleki_the_pallid(Creature* pCreature)
{
    return new boss_maleki_the_pallidAI(pCreature);
}

void AddSC_boss_maleki_the_pallid()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_maleki_the_pallid";
    newscript->GetAI = &GetAI_boss_maleki_the_pallid;
    newscript->RegisterSelf();
}
