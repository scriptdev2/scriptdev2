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
SDName: Boss_Baroness_Anastari
SD%Complete: 90
SDComment: MC disabled
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"
#include "stratholme.h"

#define SPELL_BANSHEEWAIL   16565
#define SPELL_BANSHEECURSE  16867
#define SPELL_SILENCE       18327
//#define SPELL_POSSESS       17244

struct MANGOS_DLL_DECL boss_baroness_anastariAI : public ScriptedAI
{
    boss_baroness_anastariAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 BansheeWail_Timer;
    uint32 BansheeCurse_Timer;
    uint32 Silence_Timer;
    //uint32 Possess_Timer;

    void Reset()
    {
        BansheeWail_Timer = 1000;
        BansheeCurse_Timer = 11000;
        Silence_Timer = 13000;
        //Possess_Timer = 35000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //BansheeWail
        if (BansheeWail_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_BANSHEEWAIL);
            BansheeWail_Timer = 4000;
        }else BansheeWail_Timer -= diff;

        //BansheeCurse
        if (BansheeCurse_Timer < diff)
        {
            if (!urand(0, 3))
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_BANSHEECURSE);

            BansheeCurse_Timer = 18000;
        }else BansheeCurse_Timer -= diff;

        //Silence
        if (Silence_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SILENCE);
            Silence_Timer = 13000;
        }else Silence_Timer -= diff;

        //Possess
        /*            if (Possess_Timer < diff)
        {
        //Cast
        if (rand()%100 < 65)
        {
        Unit* target = NULL;
        target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
        if (target)DoCastSpellIfCan(target,SPELL_POSSESS);
        }
        Possess_Timer = 50000;
        }else Possess_Timer -= diff;
        */

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_baroness_anastari(Creature* pCreature)
{
    return new boss_baroness_anastariAI(pCreature);
}

void AddSC_boss_baroness_anastari()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_baroness_anastari";
    newscript->GetAI = &GetAI_boss_baroness_anastari;
    newscript->RegisterSelf();
}
