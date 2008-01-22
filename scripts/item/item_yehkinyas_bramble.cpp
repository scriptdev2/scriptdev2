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
SDName: Item_Yeh'kinya's_Bramble
SD%Complete: 100
SDComment: Allow cast spell on vale screecher only and remove corpse if cast sucessful (quest 3520)
SDCategory: Items
EndScriptData */


#include "../sc_defines.h"
#include "../../../../game/Player.h"
#include "../../../../game/Creature.h"
#include "../../../../game/Unit.h"
#include "../../../../game/Spell.h"
#include "../../../../shared/WorldPacket.h"

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
void AddSC_item_yehkinyas_bramble()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_yehkinyas_bramble";
    newscript->pItemUse = ItemUse_item_yehkinyas_bramble;
    m_scripts[nrscripts++] = newscript;
}
