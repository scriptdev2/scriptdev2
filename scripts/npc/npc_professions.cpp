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
SDName: Npc_Professions
SD%Complete: 60
SDComment: NOT FOR GENERAL USE YET. Provides learn/unlearn/relearn-options for all professions, including GO (soothsaying for dummies)
SDCategory: NPCs
EndScriptData */

/*
A few notes for future developement:
- Mangos need to implement support for "hidden" faction reputation. We can probably make workarounds in script, using questRewarded, 
but this is not a good solution.
- Mangos need to implement better support for unlearning spells. Player must also unlearn associated spells, 
not only the profession spell itself.
- Mangos need to implement better support for checks of equipped items _before_ player can actually unlearn spell. If user unlearn spell
and are allowed to have items equipped dependant on this spell, player will get errors on next log in (and the item(s) in mail). Script
must use/have access to this check to prevent further action, if player has items equipped (perhaps possible to script all of this, but
but preferred to do this from mangos)
- A full implementation of gossip for GO's is required. They must have the same scripting capabilities as creatures. Basically, there is
no difference here (except that default npc_text is chosen from `gameobject_template`.`data2` (for GO type2, different dataN for a few others)
*/

#include "../sc_defines.h"
#include "../../../../game/Player.h"
#include "../../../../game/QuestDef.h"
#include "../../../../game/GossipDef.h"

/*
http://forums.worldofwarcraft.com/thread.html?topicId=388089082&sid=1
http://img72.imageshack.us/img72/953/relearningdslwkq2.jpg
http://usaochan.sakura.ne.jp/harapekosoul/modules/wordpress/attach/WoWScrnShot_061607_215434.jpg
*/
/*
update `creature_template` set `ScriptName` = 'npc_prof_alchemy' where `entry` in (17909, 19052, 22427);
update `creature_template` set `ScriptName` = 'npc_prof_blacksmith' where `entry` in (5164, 11145, 11146, 11176, 11177, 11178, 11191, 11192, 11193);
update `creature_template` set `ScriptName` = 'npc_prof_leather' where `entry` in (7866, 7867, 7868, 7869, 7870, 7871);
update `creature_template` set `ScriptName` = 'npc_prof_tailor' where `entry` in (22208, 22212, 22213);

update `gameobject_template` set `ScriptName` = 'go_soothsaying_for_dummies' where `entry` = 177226;
*/

/*###
# to be removed from here (->ncp_text). This is data for database projects.
###*/
#define TALK_MUST_UNLEARN_WEAPON    "You must forget your weapon type specialty before I can help you. Go to Everlook in Winterspring and seek help there."

#define TALK_HAMMER_LEARN           "Ah, a seasoned veteran you once were. I know you are capable, you merely need to ask and I shall teach you the way of the hammersmith."
#define TALK_AXE_LEARN              "Ah, a seasoned veteran you once were. I know you are capable, you merely need to ask and I shall teach you the way of the axesmith."
#define TALK_SWORD_LEARN            "Ah, a seasoned veteran you once were. I know you are capable, you merely need to ask and I shall teach you the way of the swordsmith."

#define TALK_HAMMER_UNLEARN         "Forgetting your Hammersmithing skill is not something to do lightly. If you choose to abandon it you will forget all recipes that require Hammersmithing to create!"
#define TALK_AXE_UNLEARN            "Forgetting your Axesmithing skill is not something to do lightly. If you choose to abandon it you will forget all recipes that require Axesmithing to create!"
#define TALK_SWORD_UNLEARN          "Forgetting your Swordsmithing skill is not something to do lightly. If you choose to abandon it you will forget all recipes that require Swordsmithing to create!"

/*###
# few generic texts/defines
###*/

#define GOSSIP_ITEM_TRAINER         "Train me"
#define GOSSIP_ITEM_GOODS           "I'd like to browse your goods."

#define GOSSIP_SENDER_LEARN     50
#define GOSSIP_SENDER_UNLEARN   51
#define GOSSIP_SENDER_CHECK     52

/*###
# gossip item and box texts
###*/

#define GOSSIP_YES                  "Yes, i am sure, please continue" //temporary

#define GOSSIP_LEARN_POTION         "Please teach me how to become a Master of Potions, Lauranna"
#define GOSSIP_UNLEARN_POTION       "I wish to unlearn Potion Mastery"
#define GOSSIP_LEARN_TRANSMUTE      "Please teach me how to become a Master of Transmutations, Zarevhi"
#define GOSSIP_UNLEARN_TRANSMUTE    "I wish to unlearn Transmutation Mastery"
#define GOSSIP_LEARN_ELIXIR         "Please teach me how to become a Master of Elixirs, Lorokeem"
#define GOSSIP_UNLEARN_ELIXIR       "I wish to unlearn Elixir Mastery"

#define BOX_UNLEARN_ALCHEMY_SPEC    "Do you really want to unlearn your alchemy specialty and lose all associated recipes? \n Cost: "

#define GOSSIP_WEAPON_LEARN         "Please teach me how to become a Weaponsmith"
#define GOSSIP_WEAPON_UNLEARN       "I wish to unlearn the art of Weaponsmithing"
#define GOSSIP_ARMOR_LEARN          "Please teach me how to become a Armorsmith"
#define GOSSIP_ARMOR_UNLEARN        "I wish to unlearn the art of Armorsmithing"

#define BOX_UNLEARN_ARMORORWEAPON   "Do you really want to unlearn your blacksmith specialty and lose all associated recipes? \n Cost: "

#define GOSSIP_LEARN_HAMMER         "Please teach me how to become a Hammersmith, Lilith"
#define GOSSIP_UNLEARN_HAMMER       "I wish to unlearn Hammersmithing"
#define GOSSIP_LEARN_AXE            "Please teach me how to become a Axesmith, Kilram"
#define GOSSIP_UNLEARN_AXE          "I wish to unlearn Axesmithing"
#define GOSSIP_LEARN_SWORD          "Please teach me how to become a Swordsmith, Seril"
#define GOSSIP_UNLEARN_SWORD        "I wish to unlearn Swordsmithing"

#define BOX_UNLEARN_WEAPON_SPEC     "Do you really want to unlearn your weaponsmith specialty and lose all associated recipes? \n Cost: "

#define GOSSIP_LEARN_DRAGON         "I am absolutely certain that i want to learn dragonscale leatherworking"
#define GOSSIP_UNLEARN_DRAGON       "I wish to unlearn Dragonscale Leatherworking"
#define GOSSIP_LEARN_ELEMENTAL      "I am absolutely certain that i want to learn elemental leatherworking"
#define GOSSIP_UNLEARN_ELEMENTAL    "I wish to unlearn Elemental Leatherworking"
#define GOSSIP_LEARN_TRIBAL         "I am absolutely certain that i want to learn tribal leatherworking"
#define GOSSIP_UNLEARN_TRIBAL       "I wish to unlearn Tribal Leatherworking"

#define BOX_UNLEARN_LEATHER_SPEC    "Do you really want to unlearn your leatherworking specialty and lose all associated recipes? \n Cost: "

#define GOSSIP_LEARN_SPELLFIRE      "Please teach me how to become a Spellcloth tailor"
#define GOSSIP_UNLEARN_SPELLFIRE    "I wish to unlearn Spellfire Tailoring"
#define GOSSIP_LEARN_MOONCLOTH      "Please teach me how to become a Mooncloth tailor"
#define GOSSIP_UNLEARN_MOONCLOTH    "I wish to unlearn Mooncloth Tailoring"
#define GOSSIP_LEARN_SHADOWEAVE     "Please teach me how to become a Shadoweave tailor"
#define GOSSIP_UNLEARN_SHADOWEAVE   "I wish to unlearn Shadoweave Tailoring"

#define BOX_UNLEARN_TAILOR_SPEC     "Do you really want to unlearn your tailoring specialty and lose all associated recipes? \n Cost: "

#define GOSSIP_LEARN_GOBLIN         "I am absolutely certain that i want to learn Goblin engineering"
#define GOSSIP_LEARN_GNOMISH        "I am absolutely certain that i want to learn Gnomish engineering"

/*###
# spells defines
###*/

#define S_WEAPON            9787
#define S_ARMOR             9788
#define S_HAMMER            17040
#define S_AXE               17041
#define S_SWORD             17039

#define S_LEARN_WEAPON      9789
#define S_LEARN_ARMOR       9790
#define S_LEARN_HAMMER      39099
#define S_LEARN_AXE         39098
#define S_LEARN_SWORD       39097

#define S_UNLEARN_WEAPON    36436
#define S_UNLEARN_ARMOR     36435
#define S_UNLEARN_HAMMER    36441
#define S_UNLEARN_AXE       36439
#define S_UNLEARN_SWORD     36438

#define S_REP_ARMOR         17451
#define S_REP_WEAPON        17452

#define REP_ARMOR           46
#define REP_WEAPON          289
#define REP_HAMMER          569
#define REP_AXE             570
#define REP_SWORD           571

#define S_DRAGON            10656
#define S_ELEMENTAL         10658
#define S_TRIBAL            10660

#define S_LEARN_DRAGON      10657
#define S_LEARN_ELEMENTAL   10659
#define S_LEARN_TRIBAL      10661

#define S_UNLEARN_DRAGON    36434
#define S_UNLEARN_ELEMENTAL 36328
#define S_UNLEARN_TRIBAL    36433

#define S_GOBLIN            20222
#define S_GNOMISH           20219

#define S_LEARN_GOBLIN      20221
#define S_LEARN_GNOMISH     20220

#define S_SPELLFIRE             26797
#define S_MOONCLOTH             26798
#define S_SHADOWEAVE            26801

#define S_LEARN_SPELLFIRE       26796
#define S_LEARN_MOONCLOTH       26799
#define S_LEARN_SHADOWEAVE      26800

#define S_UNLEARN_SPELLFIRE     41299
#define S_UNLEARN_MOONCLOTH     41558
#define S_UNLEARN_SHADOWEAVE    41559

#define S_TRANSMUTE             28672
#define S_ELIXIR                28677
#define S_POTION                26801

#define S_LEARN_TRANSMUTE       28674
#define S_LEARN_ELIXIR          28678
#define S_LEARN_POTION          26800
    
#define S_UNLEARN_TRANSMUTE     41565
#define S_UNLEARN_ELIXIR        41564
#define S_UNLEARN_POTION        41563

/*###
# some formulas to calculate unlearning cost
###*/

int32 DoLearnCost(Player *player)                           //tailor, alchemy
{
    return 200000;
}
int32 DoHighUnlearnCost(Player *player)                     //tailor, alchemy
{
    return 1500000;
}
int32 DoMedUnlearnCost(Player *player)                      //blacksmith, leatherwork
{
    uint32 level = player->getLevel();
    if(level < 51)
        return 250000;
    else if (level < 66)
        return 500000;
    else
        return 1000000;
}
int32 DoLowUnlearnCost(Player *player)                      //blacksmith
{
    uint32 level = player->getLevel();
    if (level < 66)
        return 50000;
    else
        return 100000;
}

/*###
# start menues alchemy
###*/

bool IsAlchQuestRewarded(Player *player)
{
    if (player->GetQuestRewardStatus(10899) || player->GetQuestRewardStatus(10902) || player->GetQuestRewardStatus(10897))
        return true;

    return false;
}

bool GossipHello_npc_prof_alchemy(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );
    if (_Creature->isVendor())
        player->ADD_GOSSIP_ITEM( 1, GOSSIP_ITEM_GOODS,   GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
    if(_Creature->isTrainer())
        player->ADD_GOSSIP_ITEM( 2, GOSSIP_ITEM_TRAINER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRAIN);

    uint32 eCreature = _Creature->GetEntry();

    if(player->HasSkill(SKILL_ALCHEMY) && player->GetBaseSkillValue(SKILL_ALCHEMY)>=350 && player->getLevel() > 67 )
    {
        switch (eCreature)
        {
        case 22427:                                         //Zarevhi
            if(IsAlchQuestRewarded(player))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_TRANSMUTE,    GOSSIP_SENDER_LEARN,    GOSSIP_ACTION_INFO_DEF + 1);
            if(player->HasSpell(S_TRANSMUTE))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_TRANSMUTE,  GOSSIP_SENDER_UNLEARN,  GOSSIP_ACTION_INFO_DEF + 4);
            break;
        case 19052:                                         //Lorokeem
            if(IsAlchQuestRewarded(player))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_ELIXIR,       GOSSIP_SENDER_LEARN,    GOSSIP_ACTION_INFO_DEF + 2);
            if(player->HasSpell(S_ELIXIR))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_ELIXIR,     GOSSIP_SENDER_UNLEARN,  GOSSIP_ACTION_INFO_DEF + 5);
            break;
        case 17909:                                         //Lauranna Thar'well
            if(IsAlchQuestRewarded(player))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_POTION,       GOSSIP_SENDER_LEARN,    GOSSIP_ACTION_INFO_DEF + 3);
            if(player->HasSpell(S_POTION))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_POTION,     GOSSIP_SENDER_UNLEARN,  GOSSIP_ACTION_INFO_DEF + 6);
            break;
        }
    }

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

void SendActionMenu_npc_prof_alchemy(Player *player, Creature *_Creature, uint32 action)
{
    switch(action)
    {
    case GOSSIP_ACTION_TRADE:
        player->SEND_VENDORLIST( _Creature->GetGUID() );
        break;
    case GOSSIP_ACTION_TRAIN:
        player->SEND_TRAINERLIST( _Creature->GetGUID() );
        break;
    //Learn Alchemy
    case GOSSIP_ACTION_INFO_DEF + 1:
        if(!player->HasSpell(S_TRANSMUTE) && player->GetMoney() >= DoLearnCost(player))
        {
            _Creature->CastSpell(player, S_LEARN_TRANSMUTE, true);
            player->ModifyMoney(-DoLearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 2:
        if(!player->HasSpell(S_ELIXIR) && player->GetMoney() >= DoLearnCost(player))
        {
            _Creature->CastSpell(player, S_LEARN_ELIXIR, true);
            player->ModifyMoney(-DoLearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 3:
        if(!player->HasSpell(S_POTION) && player->GetMoney() >= DoLearnCost(player))
        {
            _Creature->CastSpell(player, S_LEARN_POTION, true);
            player->ModifyMoney(-DoLearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    //Unlearn Alchemy
    case GOSSIP_ACTION_INFO_DEF + 4:
        if(player->GetMoney() >= DoHighUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_TRANSMUTE, true);
            player->ModifyMoney(-DoHighUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 5:
        if(player->GetMoney() >= DoHighUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_ELIXIR, true);
            player->ModifyMoney(-DoHighUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 6:
        if(player->GetMoney() >= DoHighUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_POTION, true);
            player->ModifyMoney(-DoHighUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    }
}

void SendConfirmLearn_npc_prof_alchemy(Player *player, Creature *_Creature, uint32 action)
{
    if(action)
    {
        uint32 eCreature = _Creature->GetEntry();
        switch(eCreature)
        {
            case 22427:
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_TRANSMUTE, GOSSIP_SENDER_CHECK, action);
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 19052:
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_ELIXIR,    GOSSIP_SENDER_CHECK, action);
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 17909:
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_POTION,    GOSSIP_SENDER_CHECK, action);
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
        }
    }
}
void SendConfirmUnlearn_npc_prof_alchemy(Player *player, Creature *_Creature, uint32 action)
{
    if(action)
    {
        uint32 eCreature = _Creature->GetEntry();
        switch(eCreature)
        {
            case 22427:                                     //Zarevhi
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_TRANSMUTE, GOSSIP_SENDER_CHECK, action, BOX_UNLEARN_ALCHEMY_SPEC, DoHighUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 19052:                                     //Lorokeem
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_ELIXIR, GOSSIP_SENDER_CHECK, action,    BOX_UNLEARN_ALCHEMY_SPEC, DoHighUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 17909:                                     //Lauranna Thar'well
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_POTION, GOSSIP_SENDER_CHECK, action,    BOX_UNLEARN_ALCHEMY_SPEC, DoHighUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
        }
    }
}

bool GossipSelect_npc_prof_alchemy(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch(sender) 
    {
        case GOSSIP_SENDER_MAIN:    SendActionMenu_npc_prof_alchemy(player, _Creature, action);
            break;
        case GOSSIP_SENDER_LEARN:   SendConfirmLearn_npc_prof_alchemy(player, _Creature, action);
            break;
        case GOSSIP_SENDER_UNLEARN: SendConfirmUnlearn_npc_prof_alchemy(player, _Creature, action);
            break;
        case GOSSIP_SENDER_CHECK:   SendActionMenu_npc_prof_alchemy(player, _Creature, action);
            break;
    }
    return true;
}

/*###
# start menues blacksmith
###*/

bool HasWeaponSub(Player *player)
{
    if (player->HasSpell(S_HAMMER) || player->HasSpell(S_AXE) || player->HasSpell(S_SWORD))
        return true;

    return false;
}

bool GossipHello_npc_prof_blacksmith(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );
    if (_Creature->isVendor())
        player->ADD_GOSSIP_ITEM( 1, GOSSIP_ITEM_GOODS,   GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
    if(_Creature->isTrainer())
        player->ADD_GOSSIP_ITEM( 2, GOSSIP_ITEM_TRAINER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRAIN);

    uint32 eCreature = _Creature->GetEntry();
    //WEAPONSMITH & ARMORSMITH
    if(player->GetBaseSkillValue(SKILL_BLACKSMITHING)>=225)
    {
        switch (eCreature)
        {
        case 11145:                                         //Myolor Sunderfury
        case 11176:                                         //Krathok Moltenfist
            if(!player->HasSpell(S_ARMOR) && player->GetReputationRank(REP_WEAPON) >  REP_NEUTRAL)                         //prevent display before after unlearned once and not when having specialty
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_ARMOR_LEARN,   GOSSIP_SENDER_LEARN,         GOSSIP_ACTION_INFO_DEF + 1);
            if(!player->HasSpell(S_WEAPON) && player->GetReputationRank(REP_ARMOR)  > REP_NEUTRAL)                         //prevent display before after unlearned once and not when having specialty
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_WEAPON_LEARN,  GOSSIP_SENDER_LEARN,         GOSSIP_ACTION_INFO_DEF + 2);
            break;
        case 11146:                                         //Ironus Coldsteel
        case 11178:                                         //Borgosh Corebender
            if(player->HasSpell(S_WEAPON))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_WEAPON_UNLEARN,    GOSSIP_SENDER_UNLEARN,   GOSSIP_ACTION_INFO_DEF + 3);
            break;
        case 5164:                                          //Grumnus Steelshaper
        case 11177:                                         //Okothos Ironrager
            if(player->HasSpell(S_ARMOR))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_ARMOR_UNLEARN,     GOSSIP_SENDER_UNLEARN,   GOSSIP_ACTION_INFO_DEF + 4);
            break;
        }
    }
    //WEAPONSMITH SPEC
    if(player->HasSpell(S_WEAPON) && player->getLevel() > 49 && player->GetBaseSkillValue(SKILL_BLACKSMITHING)>=250)
    {
        switch (eCreature)
        {
        case 11191:                                         //Lilith the Lithe
            if(!HasWeaponSub(player))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_HAMMER,       GOSSIP_SENDER_LEARN,    GOSSIP_ACTION_INFO_DEF + 5);
            if(player->HasSpell(S_HAMMER))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_HAMMER,     GOSSIP_SENDER_UNLEARN,  GOSSIP_ACTION_INFO_DEF + 8);
            break;
        case 11192:                                         //Kilram
            if(!HasWeaponSub(player))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_AXE,          GOSSIP_SENDER_LEARN,    GOSSIP_ACTION_INFO_DEF + 6);
            if(player->HasSpell(S_AXE))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_AXE,        GOSSIP_SENDER_UNLEARN,  GOSSIP_ACTION_INFO_DEF + 9);
            break;
        case 11193:                                         //Seril Scourgebane
            if(!HasWeaponSub(player))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_SWORD,        GOSSIP_SENDER_LEARN,    GOSSIP_ACTION_INFO_DEF + 7);
            if(player->HasSpell(S_SWORD))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_SWORD,      GOSSIP_SENDER_UNLEARN,  GOSSIP_ACTION_INFO_DEF + 10);
            break;
        }
    }

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

void SendActionMenu_npc_prof_blacksmith(Player *player, Creature *_Creature, uint32 action)
{
    switch(action)
    {
    case GOSSIP_ACTION_TRADE:
        player->SEND_VENDORLIST( _Creature->GetGUID() );
        break;
    case GOSSIP_ACTION_TRAIN:
        player->SEND_TRAINERLIST( _Creature->GetGUID() );
        break;
    //Learn Armor/Weapon
    case GOSSIP_ACTION_INFO_DEF + 1:
        if(!player->HasSpell(S_WEAPON))
        {
            _Creature->CastSpell(player, S_LEARN_WEAPON, true);
            _Creature->CastSpell(player, S_REP_WEAPON, true);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 2:
        if(!player->HasSpell(S_ARMOR))
        {
            _Creature->CastSpell(player, S_LEARN_ARMOR, true);
            _Creature->CastSpell(player, S_REP_ARMOR, true);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    //Unlearn Armor/Weapon
    case GOSSIP_ACTION_INFO_DEF + 3:
        if(HasWeaponSub(player))
        {
            player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID (TALK_MUST_UNLEARN_WEAPON)
            _Creature->MonsterSay(TALK_MUST_UNLEARN_WEAPON,0,player->GetGUID());         //Temporary, not offilike
        }
        else if(player->GetMoney() >= DoLowUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_WEAPON, true);
            player->ModifyMoney(-DoLowUnlearnCost(player));
            _Creature->CastSpell(player, S_REP_ARMOR, true);
            player->CLOSE_GOSSIP_MENU();
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
            player->CLOSE_GOSSIP_MENU();
        }
        break;
    case GOSSIP_ACTION_INFO_DEF + 4:
        if(player->GetMoney() >= DoLowUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_ARMOR, true);
            player->ModifyMoney(-DoLowUnlearnCost(player));
            _Creature->CastSpell(player, S_REP_WEAPON, true);
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    //Learn Hammer/Axe/Sword
    case GOSSIP_ACTION_INFO_DEF + 5:
        _Creature->CastSpell(player, S_LEARN_HAMMER, true);
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 6:
        _Creature->CastSpell(player, S_LEARN_AXE, true);
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 7:
        _Creature->CastSpell(player, S_LEARN_SWORD, true);
        player->CLOSE_GOSSIP_MENU();
        break;
    //Unlearn Hammer/Axe/Sword
    case GOSSIP_ACTION_INFO_DEF + 8:
        if(player->GetMoney() >= DoMedUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_HAMMER, true);
            player->ModifyMoney(-DoMedUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 9:
        if(player->GetMoney() >= DoMedUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_AXE, true);
            player->ModifyMoney(-DoMedUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 10:
        if(player->GetMoney() >= DoMedUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_SWORD, true);
            player->ModifyMoney(-DoMedUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    }
}

void SendConfirmLearn_npc_prof_blacksmith(Player *player, Creature *_Creature, uint32 action)
{
    if(action)
    {
        uint32 eCreature = _Creature->GetEntry();
        switch(eCreature)
        {
            case 11191:
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_HAMMER, GOSSIP_SENDER_CHECK, action);
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID (TALK_HAMMER_LEARN)
                break;
            case 11192:
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_AXE,    GOSSIP_SENDER_CHECK, action);
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID (TALK_AXE_LEARN)
                break;
            case 11193:
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_SWORD,  GOSSIP_SENDER_CHECK, action);
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID (TALK_SWORD_LEARN)
                break;
        }
    }
}
void SendConfirmUnlearn_npc_prof_blacksmith(Player *player, Creature *_Creature, uint32 action)
{
    if(action)
    {
        uint32 eCreature = _Creature->GetEntry();
        switch(eCreature)
        {
            case 11146:                                     //Ironus Coldsteel
            case 11178:                                     //Borgosh Corebender
            case 5164:                                      //Grumnus Steelshaper
            case 11177:                                     //Okothos Ironrager
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_YES, GOSSIP_SENDER_CHECK, action,               BOX_UNLEARN_ARMORORWEAPON, DoLowUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID (TALK_UNLEARN_AXEORWEAPON)
                break;

            case 11191:
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_HAMMER, GOSSIP_SENDER_CHECK, action,    BOX_UNLEARN_WEAPON_SPEC, DoMedUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID (TALK_HAMMER_UNLEARN)
                break;
            case 11192:
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_AXE, GOSSIP_SENDER_CHECK, action,       BOX_UNLEARN_WEAPON_SPEC, DoMedUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID (TALK_AXE_UNLEARN)
                break;
            case 11193:
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_SWORD, GOSSIP_SENDER_CHECK, action,     BOX_UNLEARN_WEAPON_SPEC, DoMedUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID (TALK_SWORD_UNLEARN)
                break;
        }
    }
}

bool GossipSelect_npc_prof_blacksmith(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch(sender) 
    {
        case GOSSIP_SENDER_MAIN:    SendActionMenu_npc_prof_blacksmith(player, _Creature, action);
            break;
        case GOSSIP_SENDER_LEARN:   SendConfirmLearn_npc_prof_blacksmith(player, _Creature, action);
            break;
        case GOSSIP_SENDER_UNLEARN: SendConfirmUnlearn_npc_prof_blacksmith(player, _Creature, action);
            break;
        case GOSSIP_SENDER_CHECK:   SendActionMenu_npc_prof_blacksmith(player, _Creature, action);
            break;
    }
    return true;
}

/*bool QuestComplete_npc_prof_blacksmith( Player *player, Creature *_Creature, Quest const *_Quest )
{
    if ( (_Quest->GetQuestId() == 5283) || (_Quest->GetQuestId() == 5301) )             //armorsmith
        _Creature->CastSpell(player, 17451, true);

    if ( (_Quest->GetQuestId() == 5284) || (_Quest->GetQuestId() == 5302) )             //weaponsmith
        _Creature->CastSpell(player, 17452, true);

    return true;
}*/

/*###
# start menues leatherworking
###*/

bool GossipHello_npc_prof_leather(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );
    if (_Creature->isVendor())
        player->ADD_GOSSIP_ITEM( 1, GOSSIP_ITEM_GOODS,   GOSSIP_SENDER_MAIN,                GOSSIP_ACTION_TRADE);
    if(_Creature->isTrainer())
        player->ADD_GOSSIP_ITEM( 2, GOSSIP_ITEM_TRAINER, GOSSIP_SENDER_MAIN,                GOSSIP_ACTION_TRAIN);

    uint32 eCreature = _Creature->GetEntry();

    if(player->HasSkill(SKILL_LEATHERWORKING) && player->GetBaseSkillValue(SKILL_LEATHERWORKING)>=250 && player->getLevel() > 49 )
    {
        switch (eCreature)
        {
        case 7866:                                          //Peter Galen
        case 7867:                                          //Thorkaf Dragoneye
            if(player->HasSpell(S_DRAGON))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_DRAGON,      GOSSIP_SENDER_UNLEARN, GOSSIP_ACTION_INFO_DEF + 1);
            break;
        case 7868:                                          //Sarah Tanner
        case 7869:                                          //Brumn Winterhoof
            if(player->HasSpell(S_ELEMENTAL))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_ELEMENTAL,   GOSSIP_SENDER_UNLEARN, GOSSIP_ACTION_INFO_DEF + 2);
            break;
        case 7870:                                          //Caryssia Moonhunter
        case 7871:                                          //Se'Jib
            if(player->HasSpell(S_TRIBAL))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_TRIBAL,      GOSSIP_SENDER_UNLEARN, GOSSIP_ACTION_INFO_DEF + 3);
            break;
        }
    }

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

void SendActionMenu_npc_prof_leather(Player *player, Creature *_Creature, uint32 action)
{
    switch(action)
    {
    case GOSSIP_ACTION_TRADE:
        player->SEND_VENDORLIST( _Creature->GetGUID() );
        break;
    case GOSSIP_ACTION_TRAIN:
        player->SEND_TRAINERLIST( _Creature->GetGUID() );
        break;
    //Unlearn Leather
	case GOSSIP_ACTION_INFO_DEF + 1:
        if(player->GetMoney() >= DoMedUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_DRAGON, true);
            player->ModifyMoney(-DoMedUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 2:
        if(player->GetMoney() >= DoMedUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_ELEMENTAL, true);
            player->ModifyMoney(-DoMedUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 3:
        if(player->GetMoney() >= DoMedUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_TRIBAL, true);
            player->ModifyMoney(-DoMedUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    }
}

void SendConfirmUnlearn_npc_prof_leather(Player *player, Creature *_Creature, uint32 action)
{
    if(action)
    {
        uint32 eCreature = _Creature->GetEntry();
        switch(eCreature)
        {
            case 7866:                                      //Peter Galen
            case 7867:                                      //Thorkaf Dragoneye
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_DRAGON, GOSSIP_SENDER_CHECK, action,    BOX_UNLEARN_LEATHER_SPEC, DoMedUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 7868:                                      //Sarah Tanner
            case 7869:                                      //Brumn Winterhoof
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_ELEMENTAL, GOSSIP_SENDER_CHECK, action, BOX_UNLEARN_LEATHER_SPEC, DoMedUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 7870:                                      //Caryssia Moonhunter
            case 7871:                                      //Se'Jib
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_TRIBAL, GOSSIP_SENDER_CHECK, action,    BOX_UNLEARN_LEATHER_SPEC, DoMedUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
        }
    }
}

bool GossipSelect_npc_prof_leather(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch(sender) 
    {
        case GOSSIP_SENDER_MAIN:    SendActionMenu_npc_prof_leather(player, _Creature, action);
            break;
        case GOSSIP_SENDER_UNLEARN: SendConfirmUnlearn_npc_prof_leather(player, _Creature, action);
            break;
        case GOSSIP_SENDER_CHECK:   SendActionMenu_npc_prof_leather(player, _Creature, action);
            break;
    }
    return true;
}

/*###
# start menues tailoring
###*/

bool IsTailQuestRewarded(Player *player)
{
    if (player->GetQuestRewardStatus(10831) || player->GetQuestRewardStatus(10832) || player->GetQuestRewardStatus(10833))
        return true;

    return false;
}

bool GossipHello_npc_prof_tailor(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );
    if (_Creature->isVendor())
        player->ADD_GOSSIP_ITEM( 1, GOSSIP_ITEM_GOODS,   GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
    if(_Creature->isTrainer())
        player->ADD_GOSSIP_ITEM( 2, GOSSIP_ITEM_TRAINER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRAIN);

    uint32 eCreature = _Creature->GetEntry();
                                                            //TAILORING SPEC
    if(player->HasSkill(SKILL_TAILORING) && player->GetBaseSkillValue(SKILL_TAILORING)>=350 && player->getLevel() > 59 )
    {
        switch (eCreature)
        {
        case 22213:                                         //Gidge Spellweaver
            if(IsTailQuestRewarded(player))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_SPELLFIRE,    GOSSIP_SENDER_LEARN,    GOSSIP_ACTION_INFO_DEF + 1);
            if(player->HasSpell(S_SPELLFIRE))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_SPELLFIRE,  GOSSIP_SENDER_UNLEARN,  GOSSIP_ACTION_INFO_DEF + 4);
            break;
        case 22208:                                         //Nasmara Moonsong
            if(IsTailQuestRewarded(player))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_MOONCLOTH,    GOSSIP_SENDER_LEARN,    GOSSIP_ACTION_INFO_DEF + 2);
            if(player->HasSpell(S_MOONCLOTH))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_MOONCLOTH,  GOSSIP_SENDER_UNLEARN,  GOSSIP_ACTION_INFO_DEF + 5);
            break;
        case 22212:                                         //Andrion Darkspinner
            if(IsTailQuestRewarded(player))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_SHADOWEAVE,   GOSSIP_SENDER_LEARN,    GOSSIP_ACTION_INFO_DEF + 3);
            if(player->HasSpell(S_SHADOWEAVE))
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_UNLEARN_SHADOWEAVE, GOSSIP_SENDER_UNLEARN,  GOSSIP_ACTION_INFO_DEF + 6);
            break;
        }
    }

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

void SendActionMenu_npc_prof_tailor(Player *player, Creature *_Creature, uint32 action)
{
    switch(action)
    {
    case GOSSIP_ACTION_TRADE:
        player->SEND_VENDORLIST( _Creature->GetGUID() );
        break;
    case GOSSIP_ACTION_TRAIN:
        player->SEND_TRAINERLIST( _Creature->GetGUID() );
        break;
    //Learn Tailor
    case GOSSIP_ACTION_INFO_DEF + 1:
        if(!player->HasSpell(S_SPELLFIRE) && player->GetMoney() >= DoLearnCost(player))
        {
            _Creature->CastSpell(player, S_LEARN_SPELLFIRE, true);
            player->ModifyMoney(-DoLearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 2:
        if(!player->HasSpell(S_MOONCLOTH) && player->GetMoney() >= DoLearnCost(player))
        {
            _Creature->CastSpell(player, S_LEARN_MOONCLOTH, true);
            player->ModifyMoney(-DoLearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 3:
        if(!player->HasSpell(S_SHADOWEAVE) && player->GetMoney() >= DoLearnCost(player))
        {
            _Creature->CastSpell(player, S_LEARN_SHADOWEAVE, true);
            player->ModifyMoney(-DoLearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    //Unlearn Tailor
    case GOSSIP_ACTION_INFO_DEF + 4:
        if(player->GetMoney() >= DoHighUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_SPELLFIRE, true);
            player->ModifyMoney(-DoHighUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 5:
        if(player->GetMoney() >= DoHighUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_MOONCLOTH, true);
            player->ModifyMoney(-DoHighUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF + 6:
        if(player->GetMoney() >= DoHighUnlearnCost(player))
        {
            _Creature->CastSpell(player, S_UNLEARN_SHADOWEAVE, true);
            player->ModifyMoney(-DoHighUnlearnCost(player));
        }else{
            player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
        }
        player->CLOSE_GOSSIP_MENU();
        break;
    }
}

void SendConfirmLearn_npc_prof_tailor(Player *player, Creature *_Creature, uint32 action)
{
    if(action)
    {
        uint32 eCreature = _Creature->GetEntry();
        switch(eCreature)
        {
            case 22213:
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_SPELLFIRE, GOSSIP_SENDER_CHECK, action);
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 22208:
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_MOONCLOTH,    GOSSIP_SENDER_CHECK, action);
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 22212:
                player->ADD_GOSSIP_ITEM( 0, GOSSIP_LEARN_SHADOWEAVE,  GOSSIP_SENDER_CHECK, action);
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
        }
    }
}
void SendConfirmUnlearn_npc_prof_tailor(Player *player, Creature *_Creature, uint32 action)
{
    if(action)
    {
        uint32 eCreature = _Creature->GetEntry();
        switch(eCreature)
        {
            case 22213:                                     //Gidge Spellweaver
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_SPELLFIRE, GOSSIP_SENDER_CHECK, action, BOX_UNLEARN_TAILOR_SPEC, DoHighUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 22208:                                     //Nasmara Moonsong
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_MOONCLOTH, GOSSIP_SENDER_CHECK, action, BOX_UNLEARN_TAILOR_SPEC, DoHighUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
            case 22212:                                     //Andrion Darkspinner
                player->ADD_GOSSIP_ITEM_EXTENDED( 0, GOSSIP_UNLEARN_SHADOWEAVE, GOSSIP_SENDER_CHECK, action,BOX_UNLEARN_TAILOR_SPEC, DoHighUnlearnCost(player));
                player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());   //unknown textID ()
                break;
        }
    }
}

bool GossipSelect_npc_prof_tailor(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch(sender) 
    {
        case GOSSIP_SENDER_MAIN:    SendActionMenu_npc_prof_tailor(player, _Creature, action);
            break;
        case GOSSIP_SENDER_LEARN:   SendConfirmLearn_npc_prof_tailor(player, _Creature, action);
            break;
        case GOSSIP_SENDER_UNLEARN: SendConfirmUnlearn_npc_prof_tailor(player, _Creature, action);
            break;
        case GOSSIP_SENDER_CHECK:   SendActionMenu_npc_prof_tailor(player, _Creature, action);
            break;
    }
    return true;
}

/*###
# start menues for GO (engineering and leatherworking)
###*/

/*bool GOHello_go_soothsaying_for_dummies(Player *player, GameObject* _GO)
{
    player->PlayerTalkClass->GetGossipMenu()->AddMenuItem(0,GOSSIP_LEARN_DRAGON, GOSSIP_SENDER_INFO, GOSSIP_ACTION_INFO_DEF, "", 0);

    player->SEND_GOSSIP_MENU(5584, _GO->GetGUID());

    return true;
}*/

/*###
# 
###*/

void AddSC_npc_professions()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_prof_alchemy";
    newscript->pGossipHello =  &GossipHello_npc_prof_alchemy;
    newscript->pGossipSelect = &GossipSelect_npc_prof_alchemy;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_prof_blacksmith";
    newscript->pGossipHello =  &GossipHello_npc_prof_blacksmith;
    newscript->pGossipSelect = &GossipSelect_npc_prof_blacksmith;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_prof_leather";
    newscript->pGossipHello =  &GossipHello_npc_prof_leather;
    newscript->pGossipSelect = &GossipSelect_npc_prof_leather;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_prof_tailor";
    newscript->pGossipHello =  &GossipHello_npc_prof_tailor;
    newscript->pGossipSelect = &GossipSelect_npc_prof_tailor;
    m_scripts[nrscripts++] = newscript;

    /*newscript = new Script;
    newscript->Name="go_soothsaying_for_dummies";
    newscript->pGOHello =  &GOHello_go_soothsaying_for_dummies;
    //newscript->pGossipSelect = &GossipSelect_go_soothsaying_for_dummies;
    m_scripts[nrscripts++] = newscript;*/
}
