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
SDName: Npcs_Zangarmarsh
SD%Complete: 95
SDComment: Quest support: 9785, 9803. Mark Of ... buffs.
SDCategory: Zangarmarsh
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"
#include "../../../../../game/QuestDef.h"

/*######
## npcs_ashyen_and_keleth
######*/

#define GOSSIP_ITEM_BLESS_ASH     "Grant me your mark, wise ancient."
#define GOSSIP_ITEM_BLESS_KEL     "Grant me your mark, mighty ancient."
#define GOSSIP_REWARD_BLESS       "You have my blessing"
//#define TEXT_BLESSINGS        "<You need higher standing with Cenarion Expedition to recive a blessing.>"

bool GossipHello_npcs_ashyen_and_keleth(Player *player, Creature *_Creature )
{
    if (player->GetReputationRank(942) > REP_NEUTRAL)
    {
        if ( _Creature->GetEntry() == 17900)
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_BLESS_ASH, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        if ( _Creature->GetEntry() == 17901)
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_BLESS_KEL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    }
    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npcs_ashyen_and_keleth(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        _Creature->setPowerType(POWER_MANA);
        _Creature->SetMaxPower(POWER_MANA,200);             //set a "fake" mana value, we can't depend on database doing it in this case
        _Creature->SetPower(POWER_MANA,200);

        if ( _Creature->GetEntry() == 17900)                //check which creature we are dealing with
        {
            switch (player->GetReputationRank(942))
            {                                               //mark of lore
                case REP_FRIENDLY:
                    _Creature->CastSpell(player, 31808, true);
                    _Creature->Say(GOSSIP_REWARD_BLESS, LANG_UNIVERSAL, 0);
                    break;
                case REP_HONORED:
                    _Creature->CastSpell(player, 31810, true);
                    _Creature->Say(GOSSIP_REWARD_BLESS, LANG_UNIVERSAL, 0);
                    break;
                case REP_REVERED:
                    _Creature->CastSpell(player, 31811, true);
                    _Creature->Say(GOSSIP_REWARD_BLESS, LANG_UNIVERSAL, 0);
                    break;
                case REP_EXALTED:
                    _Creature->CastSpell(player, 31815, true);
                    _Creature->Say(GOSSIP_REWARD_BLESS, LANG_UNIVERSAL, 0);
                    break;
            }
        }

        if ( _Creature->GetEntry() == 17901)
        {
            switch (player->GetReputationRank(942))         //mark of war
            {
                case REP_FRIENDLY:
                    _Creature->CastSpell(player, 31807, true);
                    _Creature->Say(GOSSIP_REWARD_BLESS, LANG_UNIVERSAL, 0);
                    break;
                case REP_HONORED:
                    _Creature->CastSpell(player, 31812, true);
                    _Creature->Say(GOSSIP_REWARD_BLESS, LANG_UNIVERSAL, 0);
                    break;
                case REP_REVERED:
                    _Creature->CastSpell(player, 31813, true);
                    _Creature->Say(GOSSIP_REWARD_BLESS, LANG_UNIVERSAL, 0);
                    break;
                case REP_EXALTED:
                    _Creature->CastSpell(player, 31814, true);
                    _Creature->Say(GOSSIP_REWARD_BLESS, LANG_UNIVERSAL, 0);
                    break;
            }
        }
        player->CLOSE_GOSSIP_MENU();
        player->TalkedToCreature(_Creature->GetEntry(), _Creature->GetGUID());
    }
    return true;
}

/*######
## npc_elder_kuruti
######*/

#define GOSSIP_ITEM_KUR1 "Offer treat"
#define GOSSIP_ITEM_KUR2 "Im a messenger for Draenei"
#define GOSSIP_ITEM_KUR3 "Get message"

bool GossipHello_npc_elder_kuruti(Player *player, Creature *_Creature )
{
    if (!player->HasItemCount(24573,1))                    //only allow if player does not have message
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_KUR1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->SEND_GOSSIP_MENU(9226,_Creature->GetGUID());

    return true;
}
bool GossipSelect_npc_elder_kuruti(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_KUR2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(9227, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_KUR3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(9229, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            if ( !player->HasItemCount(24573,1))
            {
                ItemPosCountVec dest;
                uint8 msg = player->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, 24573, 1 );
                if( msg == EQUIP_ERR_OK )
                {
                    Item * item = player->StoreNewItem( dest, 24573, 1 );
                    if( item )
                    {
                        player->SEND_GOSSIP_MENU(9231, _Creature->GetGUID());
                    }
                    else
                        player->SendEquipError( msg,NULL,NULL);
                }
            }
            else
                player->SEND_GOSSIP_MENU(9231, _Creature->GetGUID());
            break;
    }
    return true;
}

/*######
## AddSC
######*/


void AddSC_npcs_zangarmarsh()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npcs_ashyen_and_keleth";
    newscript->pGossipHello =  &GossipHello_npcs_ashyen_and_keleth;
    newscript->pGossipSelect = &GossipSelect_npcs_ashyen_and_keleth;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_elder_kuruti";
    newscript->pGossipHello =  &GossipHello_npc_elder_kuruti;
    newscript->pGossipSelect = &GossipSelect_npc_elder_kuruti;
    m_scripts[nrscripts++] = newscript;
}
