/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_archivist_galford
SD%Complete: 100
SDComment: 
SDCategory: Stratholme
EndScriptData */

#include "sc_creature.h"

// **** This script is still under Developement ****

//Status: check for timers

#define SPELL_FIRENOVA    23462
#define SPELL_BURNINGWINDS    17293
#define SPELL_PYROBLAST    17274

struct MANGOS_DLL_DECL boss_archivist_galfordAI : public ScriptedAI
{
    boss_archivist_galfordAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 FireNova_Timer;
    uint32 BurningWinds_Timer;
    uint32 Pyroblast_Timer;
    bool InCombat;

    void Reset()
    {
        FireNova_Timer = 15000;
        BurningWinds_Timer = 10000;
        Pyroblast_Timer = 3000;
        InCombat = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //FireNova
        if (FireNova_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FIRENOVA);

            //10 seconds until we should cast this again
            FireNova_Timer = 10000;
        }else FireNova_Timer -= diff;

        //BurningWinds
        if (BurningWinds_Timer < diff)
        {
            //Cast
            if (rand()%100 < 70) //70% chance to cast
            {
                    //Cast
                    Unit* target = NULL;

                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if (target)
                    DoCast(target,SPELL_BURNINGWINDS);
            }

            //15 seconds until we should cast this again
            BurningWinds_Timer = 15000;
        }else BurningWinds_Timer -= diff;

        //Pyroblast
        if (Pyroblast_Timer < diff)
        {
            //Cast
            if (rand()%100 < 25) //25% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_PYROBLAST);
            }
            //18 seconds until we should cast this again
            Pyroblast_Timer = 18000;
        }else Pyroblast_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_archivist_galford(Creature *_Creature)
{
    return new boss_archivist_galfordAI (_Creature);
}


void AddSC_boss_archivist_galford()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_archivist_galford";
    newscript->GetAI = GetAI_boss_archivist_galford;
    m_scripts[nrscripts++] = newscript;
}
