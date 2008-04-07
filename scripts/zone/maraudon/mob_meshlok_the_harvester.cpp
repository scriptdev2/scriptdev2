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
SDName: Mob_Meshlok_The_Harvester
SD%Complete: 100
SDComment: 
EndScriptData */


#include "sc_creature.h"

#define SPELL_WARSTOMP             21080        
#define SPELL_STRIKE                  8817              

struct MANGOS_DLL_DECL meshlok_the_harvesterAI : public ScriptedAI
{
    meshlok_the_harvesterAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 WarStomp_Timer;
    uint32 Strike_Timer;
    bool InCombat;

    void Reset()
    {       
        WarStomp_Timer = 9000;
        Strike_Timer = 7000;
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //WarStomp_Timer
        if (WarStomp_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_WARSTOMP);

            //18 seconds
            WarStomp_Timer = 11000;
        }else WarStomp_Timer -= diff;

        //Strike_Timer
        if (Strike_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_STRIKE);

            //12 seconds until we should cast this agian
            Strike_Timer = 9000;
        }else Strike_Timer -= diff;

        DoMeleeAttackIfReady();

    }
}; 
CreatureAI* GetAI_meshlok_the_harvester(Creature *_Creature)
{
    return new meshlok_the_harvesterAI (_Creature);
}


void AddSC_meshlok_the_harvester()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="meshlok_the_harvester";
    newscript->GetAI = GetAI_meshlok_the_harvester;
    m_scripts[nrscripts++] = newscript;
}
