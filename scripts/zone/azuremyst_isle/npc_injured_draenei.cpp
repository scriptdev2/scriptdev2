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

/* ScriptData
SDName: npc_injured_draenei
SD%Complete: 100
SDComment: Cosmetic only
EndScriptData */

#include "../../sc_defines.h"

struct MANGOS_DLL_DECL injured_draeneiAI : public ScriptedAI
{
    injured_draeneiAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        DoGoHome();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
        m_creature->SetHealth(int(m_creature->GetMaxHealth()*.15));
        switch (rand()%2)
        {
            case 0: m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 1); break;
            case 1: m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 3); break;
        }
    }
    
    void AttackStart(Unit *who)
    {
        return; //ignore all attackstart commands
    }

    void MoveInLineOfSight(Unit *who)
    {
        return; //ignore everyone around them (won't aggro anything)
    }

    void UpdateAI(const uint32 diff)
    {
        return;
    }

};
CreatureAI* GetAI_injured_draenei(Creature *_Creature)
{
    return new injured_draeneiAI (_Creature);
}


void AddSC_injured_draenei()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="injured_draenei";
    newscript->GetAI = GetAI_injured_draenei;
    m_scripts[nrscripts++] = newscript;
}
