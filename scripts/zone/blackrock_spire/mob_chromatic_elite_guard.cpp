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
SDName: Mob_Chromatic_Elite_Guard
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Spire
EndScriptData */

#include "sc_creature.h"

#define SPELL_KNOCKDOWN                20276   

struct MANGOS_DLL_DECL mob_chromatic_elite_guardAI : public ScriptedAI
{
    mob_chromatic_elite_guardAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 KnockDown_Timer;
    bool InCombat;

    void Reset()
    {       
        KnockDown_Timer = 20000;
        InCombat = false;
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //KnockDown_Timer
        if (KnockDown_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_KNOCKDOWN);

            //8 seconds
            KnockDown_Timer = 8000;
        }else KnockDown_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_mob_chromatic_elite_guard(Creature *_Creature)
{
    return new mob_chromatic_elite_guardAI (_Creature);
}


void AddSC_mob_chromatic_elite_guard()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_chromatic_elite_guard";
    newscript->GetAI = GetAI_mob_chromatic_elite_guard;
    m_scripts[nrscripts++] = newscript;
}
