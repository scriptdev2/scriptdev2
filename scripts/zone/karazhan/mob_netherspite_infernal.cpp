/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/** This Script is still under development */

#include "../../sc_defines.h"

#define SPELL_HELLFIRE          39132


struct MANGOS_DLL_DECL netherspite_infernalAI : public ScriptedAI
{
    netherspite_infernalAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    uint32 HellfireTimer;
    Unit* Malchezzar;

    void EnterEvadeMode()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        Malchezzar = NULL;
        HellfireTimer = 1000;
    }

    void SetMalchezzarTarget(Unit* target)
    {
        if (Malchezzar)
            Malchezzar = target;
    }

    void AttackStart(Unit *who)
    {        

    }

    void MoveInLineOfSight(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if(HellfireTimer < diff)
        {
            DoCast(m_creature, SPELL_HELLFIRE);

            //Destroy ourselves if Malchezzar is dead
            //if (Malchezzar && !Malchezzar->isAlive())
            //    m_creature->DealDamage(m_creature,m_creature->GetMaxHealth(),DIRECT_DAMAGE,0,NULL,0,0);
            //else 
            HellfireTimer = 1000;

        }else HellfireTimer -= diff;
    }
};


CreatureAI* GetAI_netherspite_infernal(Creature *_Creature)
{
    return new netherspite_infernalAI (_Creature);
}

void AddSC_netherspite_infernal()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="netherspite_infernal";
    newscript->GetAI = GetAI_netherspite_infernal;

    m_scripts[nrscripts++] = newscript;
}
