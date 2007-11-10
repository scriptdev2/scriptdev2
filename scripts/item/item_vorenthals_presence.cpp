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
#include "../../../../game/Item.h"
#include "../../../../game/Player.h"
#include "../../../../game/Spell.h"

bool ItemUse_vorenthals_presence(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    // allow use
    if( targets.getUnitTarget() && targets.getUnitTarget()->GetTypeId()==TYPEID_UNIT && 
        targets.getUnitTarget()->GetEntry() == 20132 )
        return false;

    // error
    player->SendEquipError(EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM,_Item,NULL);
    return true;
}
void AddSC_vorenthals_presence()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="vorenthals_presence";
    newscript->pItemUse = ItemUse_vorenthals_presence;
    m_scripts[nrscripts++] = newscript;
}
