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

bool ItemUse_item_test(Player *player, Item* _Item)
{
    int32 basePoints0 = 50000;
    //i.DurationIndex = 20; If this required then must be used spellmod
    player->CastCustomSpell(player,32739,&basePoints0, NULL, NULL, true);
    //player->GetSession()->SendShowBank( player->GetGUID() );
    return true;
}

void AddSC_item_test()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_test";
    newscript->pItemUse = ItemUse_item_test;
    m_scripts[nrscripts++] = newscript;
}
