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
SDName: Item_Scripts
SD%Complete: 100
SDComment: Items for a range of different items. See content below (in script)
SDCategory: Items
EndScriptData */

/*#####
# item_area_52_special(i28132)       Prevents abuse of this item
# item_draenei_fishing_net(i23654)   Correctly implements chance to spawn item or creature
# item_nether_wraith_beacon(i31742)  Summons creatures for quest Becoming a Spellfire Tailor (q10832)
# item_vorenthals_presence(i30259)   Prevents abuse of this item
# item_yehkinyas_bramble(i10699)     Allow cast spell on vale screecher only and remove corpse if cast sucessful (q3520)
# item_zezzak_shard(i31463)          Quest The eyes of Grillok (q10813). Prevents abuse
# 
#####*/

#include "../sc_defines.h"
#include "../../../../game/Player.h"
#include "../../../../game/Creature.h"
#include "../../../../game/Unit.h"
#include "../../../../game/Spell.h"
#include "../../../../shared/WorldPacket.h"

/*#####
# item_area_52_special
#####*/

bool ItemUse_item_area_52_special(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if ( player->GetAreaId() == 3803 )
    {
        return false;
    }
    else
    {
        player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE,_Item,NULL);
        return true;
    }
}

/*#####
# item_draenei_fishing_net
#####*/

bool ItemUse_item_draenei_fishing_net(Player *player, Item* _Item, SpellCastTargets const& targets)
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

/*#####
# item_nether_wraith_beacon
#####*/

bool ItemUse_item_nether_wraith_beacon(Player *player, Item* _Item, SpellCastTargets const& targets)
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

/*#####
# item_vorenthals_presence
#####*/

bool ItemUse_item_vorenthals_presence(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    // allow use
    if( targets.getUnitTarget() && targets.getUnitTarget()->GetTypeId()==TYPEID_UNIT && 
        targets.getUnitTarget()->GetEntry() == 20132 )
        return false;

    // error
    player->SendEquipError(EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM,_Item,NULL);
    return true;
}

/*#####
# item_yehkinyas_bramble
#####*/

bool ItemUse_item_yehkinyas_bramble(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if (player->GetQuestStatus(3520) == QUEST_STATUS_INCOMPLETE)
    {
        Unit * unit_target = targets.getUnitTarget();
        if( unit_target && 
            unit_target->GetTypeId()==TYPEID_UNIT &&
            unit_target->isDead() &&
            (unit_target->GetEntry()==5307 || unit_target->GetEntry()==5308) )// cast only on corpse 5307 or 5308
        {
            ((Creature*)unit_target)->RemoveCorpse();       // remove corpse for cancelling second use
            return false;                                   // all ok
        }
    }
    WorldPacket data(SMSG_CAST_RESULT, (4+2));              // prepare packet error message
    data << uint32(10699);                                  // itemId
    data << uint8(SPELL_FAILED_BAD_TARGETS);                // reason
    player->GetSession()->SendPacket(&data);                // send message: Bad target
    player->SendEquipError(EQUIP_ERR_NONE,_Item,NULL);      // break spell
    return true;
}

/*#####
# item_zezzak_shard
#####*/

bool ItemUse_item_zezzak_shard(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if( targets.getUnitTarget() && targets.getUnitTarget()->GetTypeId()==TYPEID_UNIT && 
        targets.getUnitTarget()->GetEntry() == 19440 )
        return false;

    player->SendEquipError(EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM,_Item,NULL);
    return true;
}

void AddSC_item_scripts()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_area_52_special";
    newscript->pItemUse = ItemUse_item_area_52_special;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_draenei_fishing_net";
    newscript->pItemUse = ItemUse_item_draenei_fishing_net;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_nether_wraith_beacon";
    newscript->pItemUse = ItemUse_item_nether_wraith_beacon;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_vorenthals_presence";
    newscript->pItemUse = ItemUse_item_vorenthals_presence;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_yehkinyas_bramble";
    newscript->pItemUse = ItemUse_item_yehkinyas_bramble;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_zezzaks_shard";
    newscript->pItemUse = ItemUse_item_zezzak_shard;
    m_scripts[nrscripts++] = newscript;
}
