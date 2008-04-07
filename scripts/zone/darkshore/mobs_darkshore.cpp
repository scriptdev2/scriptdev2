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
SDName: Mobs_Darkshore
SD%Complete: 100
SDComment: Quest support: 
SDCategory: Darkshore
EndScriptData */

#include "sc_creature.h"

/*######
## mob_lunaclaw
######*/

struct MANGOS_DLL_DECL mob_lunaclawAI : public ScriptedAI
{
    mob_lunaclawAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void DamageTaken(Unit *done_by, uint32 &damage) 
    {
        if (m_creature->GetHealth() <= damage)
        {
            m_creature->CastSpell(m_creature,18986,true);//summon lunaclaw spirit
        }
    }

    void Aggro(Unit *who)
    {
    }
};
CreatureAI* GetAI_mob_lunaclaw(Creature *_Creature)
{
    return new mob_lunaclawAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_mobs_darkshore()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_lunaclaw";
    newscript->GetAI = GetAI_mob_lunaclaw;
    m_scripts[nrscripts++] = newscript;
}
