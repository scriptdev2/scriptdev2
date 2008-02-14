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
SDName: Item_Zerraks_Shard
SD%Complete: 100
SDComment: Prevents abuse of this item
SDCategory: Items
EndScriptData */

//Zezzak`s Shard (id 31463)
//quest The eyes of Grillok (entry 10813 ) 
#include "../sc_defines.h"
#include "../../../../game/Player.h"
#include "../../../../game/Spell.h"

bool ItemUse_zezzak_shard(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if( targets.getUnitTarget() && targets.getUnitTarget()->GetTypeId()==TYPEID_UNIT && 
        targets.getUnitTarget()->GetEntry() == 19440 )
        return false;

    player->SendEquipError(EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM,_Item,NULL);
    return true;
}
void AddSC_item_zezzak_shard()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_zezzaks_shard";
    newscript->pItemUse = ItemUse_zezzak_shard;
    m_scripts[nrscripts++] = newscript;
}
