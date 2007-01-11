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

void ForcePlayerSay(Player* player, char const *text)
{
    WorldPacket data(100);//just guess size
    data.SetOpcode(SMSG_MESSAGECHAT);
    data << (uint8)5; //CHAT_MSG_YELL;
    data << uint32(LANG_UNIVERSAL);

    data << uint64(player->GetGUID());

    data << uint32(strlen(text)+1);
    data << text;
    data << uint8(0);

    player->SendMessageToSet(&data,true);
}

bool ItemUse_item_test(Player *player, Item* _Item)
{
    switch (rand() % 3)
    {
    case 0:
        ForcePlayerSay(player, "I cannot control myself...flee...flee before I begin dancing!!");
        player->HandleEmoteCommand(EMOTE_STATE_DANCE);
        break;

    case 1:
        ForcePlayerSay(player, "Portable Bank is a go!");
        player->GetSession()->SendShowBank( player->GetGUID() );
        break;

    case 2:
        ForcePlayerSay(player, "I am invincable!");
        player->CastSpell(player,6724,true);
        break;
    }

    return true;
}

void AddSC_item_test()
{
#ifdef SCRIPT_EXTENDED
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_test";
    newscript->pItemUse = ItemUse_item_test;

    m_scripts[nrscripts++] = newscript;
#endif
}
