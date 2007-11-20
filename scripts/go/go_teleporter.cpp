
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
SDName: GO_teleporter
SD%Complete: 0
SDComment: Unsure what this is used for
EndScriptData */


#include "../sc_defines.h"
#include "../../../../game/Player.h"

bool GOHello_go_teleporter(Player *player, GameObject* _GO)
{
    player->TeleportTo(0, 1807.07f,336.105f,70.3975f,0.0f);
    return false;
}

void AddSC_go_teleporter()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="go_teleporter";
    newscript->pGOHello = GOHello_go_teleporter;
    m_scripts[nrscripts++] = newscript;
}
