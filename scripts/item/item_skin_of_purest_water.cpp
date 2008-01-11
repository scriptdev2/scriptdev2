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
SDName: item_skin_of_purest_water
SD%Complete: 100
SDComment: Summons Tel'athion the Impure (Call of Water)
SDCategory: Items
EndScriptData */
 
#include "../sc_defines.h"
#include "../../../../game/Player.h"
 
bool ItemUse_item_skin_of_purest_water(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    Creature *Telathion;
    Telathion = player->SummonCreature(17359,player->GetPositionX()+25, player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,300000);
    if (Telathion)
        ((CreatureAI*)Telathion->AI())->AttackStart(player);

    Creature *WaterSpirit1;
    WaterSpirit1 = player->SummonCreature(6748,player->GetPositionX()+5, player->GetPositionY()+5, player->GetPositionZ(), 0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,180000);
    if (WaterSpirit1 && Telathion)
        ((CreatureAI*)WaterSpirit1->AI())->AttackStart(Telathion);

    Creature *WaterSpirit2;
    WaterSpirit2 = player->SummonCreature(6748,player->GetPositionX()+5, player->GetPositionY()-5, player->GetPositionZ(), 0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,180000);
    if (WaterSpirit2 && Telathion)
        ((CreatureAI*)WaterSpirit2->AI())->AttackStart(Telathion);

    return false;
}
 
void AddSC_item_skin_of_purest_water()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_skin_of_purest_water";
    newscript->pItemUse = ItemUse_item_skin_of_purest_water;
    m_scripts[nrscripts++] = newscript;
}
