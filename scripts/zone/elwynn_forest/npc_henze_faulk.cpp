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
SDName: Npc_Henze_Faulk
SD%Complete: 50
SDComment: Core keeps breaking support for this quest
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"

// **** This script is still under Developement ****

#define SAY_HEAL "Thank you, dear Paladin, you just saved my life."

struct MANGOS_DLL_DECL npc_henze_faulkAI : public ScriptedAI
{
    uint32 lifeTimer;
    bool spellHit;

    npc_henze_faulkAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        lifeTimer = 120000;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        DoGoHome();
        m_creature->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 32);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,7); // lay down
        spellHit = false;
    }

    void AttackStart(Unit *who)
    {
        return; //ignore all attackstart commands
    }

    void MoveInLineOfSight(Unit *who)
    {
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->GetUInt32Value(UNIT_FIELD_BYTES_1))
        {
            if(lifeTimer < diff)
                m_creature->AI()->EnterEvadeMode();
            else 
                lifeTimer -= diff;
        }
    }

    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
    {
        if(Spellkind->Id == 8593 && !spellHit)
        {
            DoCast(m_creature,32343);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
            m_creature->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
            m_creature->RemoveAllAuras();
            DoSay(SAY_HEAL,LANG_COMMON,NULL);
            spellHit = true;
        }
    }

};
CreatureAI* GetAI_npc_henze_faulk(Creature *_Creature)
{
    return new npc_henze_faulkAI (_Creature);
}
void AddSC_npc_henze_faulk()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_henze_faulk";
    newscript->GetAI = GetAI_npc_henze_faulk;
    m_scripts[nrscripts++] = newscript;
}