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
SDName: Item_Nether_Wraith_Beacon
SD%Complete: 100
SDComment: Summons creatures
SDCategory: Items
EndScriptData */


#include "../sc_defines.h"
#include "../../../../game/Player.h"

bool ItemUse_nether_wraith_beacon(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if (player->GetQuestStatus(10832) == QUEST_STATUS_INCOMPLETE)
    {
        Creature *Nether;
        Nether = player->SummonCreature(22408,player->GetPositionX() ,player->GetPositionY()+20, player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,180000);
        Nether = player->SummonCreature(22408,player->GetPositionX() ,player->GetPositionY()-20, player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,180000);
        if (Nether)
            ((CreatureAI*)Nether->AI())->AttackStart(player);
    }
    return false;
}

void AddSC_nether_wraith_beacon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="nether_wraith_beacon";
    newscript->pItemUse = ItemUse_nether_wraith_beacon;
    m_scripts[nrscripts++] = newscript;
}
