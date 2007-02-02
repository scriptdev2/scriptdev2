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

#include "../sc_defines.h"

//THIS SCRIPT IS UNDER DEVELOPMENT

struct MANGOS_DLL_DECL LumberjackAI : public ScriptedAI
{
    LumberjackAI(Creature *c) : ScriptedAI(c) {}
    
    void MoveInLineOfSight(Unit *who)
    {
        //If a player walks into LOS we should begin our chopping animation
        if (who->GetTypeId() == TYPEID_PLAYER)
            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, 234);
    }
};

CreatureAI* GetAI_Lumberjack(Creature *_Creature)
{
    return new LumberjackAI (_Creature);
}

void AddSC_lumberjack()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="lumberjack";
    newscript->GetAI = GetAI_Lumberjack;
    m_scripts[nrscripts++] = newscript;
}