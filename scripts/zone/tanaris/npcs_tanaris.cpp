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
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

// **** This script is still under Developement ****

/*######
## npc_steward_of_time
######*/

bool QuestAccept_npc_steward_of_time(Player *player, Creature *creature, Quest const *quest )
{
        std::vector<uint32> nodes;                          //To The Master's Lair, q-id 10279

        nodes.resize(2);
        nodes[0] = 144;                                     //from top of cave
        nodes[1] = 143;                                     //end at bottom of cave
        player->ActivateTaxiPathTo(nodes);

        return true;
}

/*######
## AddSC
######*/

void AddSC_npcs_tanaris()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_steward_of_time";
    newscript->pQuestAccept =   &QuestAccept_npc_steward_of_time;
    m_scripts[nrscripts++] = newscript;
}
