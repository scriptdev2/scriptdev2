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
SDName: Mob_Overmaster_Pyron
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Depths
EndScriptData */

#include "sc_creature.h"

// **** This script is still under Developement ****


#define SPELL_FIRENOVA                11307             
#define SPELL_FIREBLAST                10199

struct MANGOS_DLL_DECL overmaster_pyronAI : public ScriptedAI
{
    overmaster_pyronAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 FireNova_Timer;
    uint32 FireBlast_Timer;

    void Reset()
    {       
        FireNova_Timer = 15000;
        FireBlast_Timer = 8000;
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //FireNova_Timer
        if (FireNova_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FIRENOVA);

            //10 seconds
            FireNova_Timer = 10000;
        }else FireNova_Timer -= diff;

        //FireBlast_Timer
        if (FireBlast_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FIREBLAST);

            //12 seconds
            FireBlast_Timer = 12000;
        }else FireBlast_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_overmaster_pyron(Creature *_Creature)
{
    return new overmaster_pyronAI (_Creature);
}


void AddSC_overmaster_pyron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="overmaster_pyron";
    newscript->GetAI = GetAI_overmaster_pyron;
    m_scripts[nrscripts++] = newscript;
}
