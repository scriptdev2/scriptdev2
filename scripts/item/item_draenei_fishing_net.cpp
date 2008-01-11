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
SDName: Item_Draenei_Fishing_Net
SD%Complete: 100
SDComment: Correctly implements chance to spawn item or creature
SDCategory: Items
EndScriptData */


#include "../sc_defines.h"
#include "../../../../game/Player.h"

bool ItemUse_draenei_fishing_net(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    Item* item = NULL;
    uint16 dest;

    if (player->GetQuestStatus(9452) == QUEST_STATUS_INCOMPLETE)
    {
        if (rand()%100 < 35)
        {
            Creature *Murloc;
            Murloc = player->SummonCreature(17102,player->GetPositionX() ,player->GetPositionY()+20, player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,180000);
            if (Murloc)
                ((CreatureAI*)Murloc->AI())->AttackStart(player);
        }    
        else
        {    
            uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 23614, 1, false);
            if(msg == EQUIP_ERR_OK)
            {
                item = player->StoreNewItem(dest, 23614, 1, true);
                player->SendNewItem(item, 1, true, false);    

            }

        }
    }
    return false;
}

void AddSC_draenei_fishing_net()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="draenei_fishing_net";
    newscript->pItemUse = ItemUse_draenei_fishing_net;
    m_scripts[nrscripts++] = newscript;
}
