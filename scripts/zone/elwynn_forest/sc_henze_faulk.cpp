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

#include "../../sc_defines.h"

uint32 NPCDialogStatus_henze_faulk(Player *player, Creature *_Creature )
{
    //Appear dead
    _Creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_STATE_DEAD);
    _Creature->SetUInt32Value(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
    
    if( _Creature->isAlive() == true )
    {
        player->CastSpell( player, 8593, true);
        _Creature->RemoveFlag (UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
        _Creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_STATE_STAND);

        return _Creature->QUEST_DIALOG_STATUS(player, DIALOG_STATUS_CHAT);
    }
    else
    {
        return _Creature->QUEST_DIALOG_STATUS(player, DIALOG_STATUS_NONE);
    }
}

void AddSC_henze_faulk()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="henze_faulk";
    newscript->pNPCDialogStatus      = &NPCDialogStatus_henze_faulk;
    m_scripts[nrscripts++] = newscript;
}