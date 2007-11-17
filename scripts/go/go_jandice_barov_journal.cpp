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
#include "../../../../game/Player.h"

bool GOHello_go_barov_journal(Player *player, GameObject* _GO)
{
    if (player->HasSkill(197) && player->GetSkillValue(197) >= 280 && !player->HasSpell(26086)) 
    {
        player->CastSpell(player,26089,false);
    }
    return true;
}

void AddSC_go_barov_journal()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="go_barov_journal";
    newscript->pGOHello		  = &GOHello_go_barov_journal;
    m_scripts[nrscripts++] = newscript;
}