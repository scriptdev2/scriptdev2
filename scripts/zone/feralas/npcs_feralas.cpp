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
SDName: Npcs_Feralas
SD%Complete: 100
SDComment: Quest support: 3520
SDCategory: Feralas
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

/*######
## npc_screecher_spirit
######*/

bool GossipHello_npc_screecher_spirit(Player *player, Creature *_Creature)
{
    player->SEND_GOSSIP_MENU(2039,_Creature->GetGUID() );
    player->TalkedToCreature(_Creature->GetEntry(), _Creature->GetGUID());
    _Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

    return true;
}

/*######
## AddSC
######*/

void AddSC_npcs_feralas()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_screecher_spirit";
    newscript->pGossipHello = &GossipHello_npc_screecher_spirit;
    m_scripts[nrscripts++] = newscript;
}
