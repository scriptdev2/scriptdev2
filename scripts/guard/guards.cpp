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

/********************* CONTENT *************************
 * guard_bluffwatcher
 * guard_contested
 * guard_darnassus
 * guard_dunmorogh
 * guard_durotar
 * guard_elwynnforest
 * guard_exodar
 * guard_ironforge
 * guard_mulgore
 * guard_orgrimmar
 * guard_shattrath
 * guard_silvermoon
 * guard_stormwind
 * guard_teldrassil
 * guard_tirisfal
 * guard_undercity
 * 
 *******************************************************/

#include "../sc_defines.h"
#include "guard_ai.h"
#include "../../../../game/Player.h"
#include "../../../../game/GossipDef.h"

//script spesific action
#define GOSSIP_ACTION_TAVERN                  101
#define GOSSIP_ACTION_GEMMERCHANT             102
#define GOSSIP_ACTION_MANALOOM                103

//script spesific sender
#define GOSSIP_SENDER_SEC_GEMMERCHANT         101

//script spesific gossip text
#define GOSSIP_TEXT_TAVERN                    "Worlds End Tavern"
#define GOSSIP_TEXT_BANKSCYERS                "Scyers bank"
#define GOSSIP_TEXT_BANKALDOR                 "Aldor Bank"
#define GOSSIP_TEXT_INNSCYERS                 "Scyers Inn"
#define GOSSIP_TEXT_INNALDOR                  "Aldor Inn"
#define GOSSIP_TEXT_MAILBOXSCYERS             "Scyers Mailbox"
#define GOSSIP_TEXT_MAILBOXALDOR              "Aldor Mailbox"
#define GOSSIP_TEXT_STABLESCYERS              "Scyers Stable Master"
#define GOSSIP_TEXT_STABLEALDOR               "Aldor Stable Master"
#define GOSSIP_TEXT_BATTLEMASTERALLIANCE      "Alliance Battlemasters"
#define GOSSIP_TEXT_BATTLEMASTERHORDE         "Horde And Arena Battlemasters"
#define GOSSIP_TEXT_MANALOOM                  "Mana Loom"
#define GOSSIP_TEXT_ALCHEMYLAB                "Alchemy Lab"
#define GOSSIP_TEXT_GEMMERCHANT               "Gem Merchant"
#define GOSSIP_TEXT_GEMSCYERS                 "Scyers Gem Merchant"
#define GOSSIP_TEXT_GEMALDOR                  "Aldor Gem Merchant"

/*******************************************************
 * guard_bluffwatcher start
 *******************************************************/

bool GossipHello_guard_bluffwatcher(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK         , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_WINDRIDER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->SEND_GOSSIP_MENU(3543,_Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Bank
            player->SEND_POI(-1257.8, 24.14, 6, 6, 0, "Thunder Bluff Bank");
            player->SEND_GOSSIP_MENU(1292,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Wind master
            player->SEND_POI(-1196.43, 28.26, 6, 6, 0, "Wind Rider Roost");
            player->SEND_GOSSIP_MENU(1293,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Guild master
            player->SEND_POI(-1296.5, 127.57, 6, 6, 0, "Thunder Bluff Civic Information");
            player->SEND_GOSSIP_MENU(1291,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Inn
            player->SEND_POI(-1296, 39.7, 6, 6, 0, "Thunder Bluff Inn");
            player->SEND_GOSSIP_MENU(3153,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Mailbox
            player->SEND_POI(-1263.59, 44.36, 6, 6, 0, "Thunder Bluff Mailbox");
            player->SEND_GOSSIP_MENU(3154,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Auction House
            player->SEND_POI(1381.77, -4371.16, 6, 6, 0, GOSSIP_TEXT_AUCTIONHOUSE);
            player->SEND_GOSSIP_MENU(3155,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Weapon master
            player->SEND_POI(-1282.31, 89.56, 6, 6, 0, "Ansekhwa");
            player->SEND_GOSSIP_MENU(4520,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Stable master
            player->SEND_POI(-1270.19, 48.84, 6, 6, 0, "Bulrug");
            player->SEND_GOSSIP_MENU(5977,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //battlemaster
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY       , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN         , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH         , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(7527,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->SEND_GOSSIP_MENU(3542,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->SEND_GOSSIP_MENU(3541,_Creature->GetGUID());
            break;
    }
}

void SendBattleMasterMenu_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //AV
            player->SEND_POI(-1387.82, -97.55, 6, 6, 0, "Taim Ragetotem");
            player->SEND_GOSSIP_MENU(7522,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //AB
            player->SEND_POI(-997, 214.12, 6, 6, 0, "Martin Lindsey");
            player->SEND_GOSSIP_MENU(7648,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //WSG
            player->SEND_POI(-1384.94, -75.91, 6, 6, 0, "Kergul Bloodaxe");
            player->SEND_GOSSIP_MENU(7523,_Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Druid
            player->SEND_POI(-1054.47, -285, 6, 6, 0, "Hall of Elders");
            player->SEND_GOSSIP_MENU(1294,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Hunter
            player->SEND_POI(-1416.32, -114.28, 6, 6, 0, "Hunter's Hall");
            player->SEND_GOSSIP_MENU(1295,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Mage
            player->SEND_POI(-1061.2, 195.5, 6, 6, 0, "Pools of Vision");
            player->SEND_GOSSIP_MENU(1296,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Priest
            player->SEND_POI(-1061.2, 195.5, 6, 6, 0, "Pools of Vision");
            player->SEND_GOSSIP_MENU(1297,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Shaman
            player->SEND_POI(-989.54, 278.25, 6, 6, 0, "Hall of Spirits");
            player->SEND_GOSSIP_MENU(1298,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Warrior
            player->SEND_POI(-1416.32, -114.28, 6, 6, 0, "Hunter's Hall");
            player->SEND_GOSSIP_MENU(1299,_Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(-1085.56, 27.29, 6, 6, 0, "Bena's Alchemy");
            player->SEND_GOSSIP_MENU(1332,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(-1239.75, 104.88, 6, 6, 0, "Karn's Smithy");
            player->SEND_GOSSIP_MENU(1333,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(-1214.5, -21.23, 6, 6, 0, "Aska's Kitchen");
            player->SEND_GOSSIP_MENU(1334,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_POI(-1112.65, 48.26, 6, 6, 0, "Dawnstrider Enchanters");
            player->SEND_GOSSIP_MENU(1335,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //First Aid
            player->SEND_POI(-996.58, 200.5, 6, 6, 0, "Spiritual Healing");
            player->SEND_GOSSIP_MENU(1336,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Fishing
            player->SEND_POI(-1169.35, -68.87, 6, 6, 0, "Mountaintop Bait & Tackle");
            player->SEND_GOSSIP_MENU(1337,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Herbalism
            player->SEND_POI(-1137.7, -1.51, 6, 6, 0, "Holistic Herbalism");
            player->SEND_GOSSIP_MENU(1338,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Leatherworking
            player->SEND_POI(-1156.22, 66.86, 6, 6, 0, "Thunder Bluff Armorers");
            player->SEND_GOSSIP_MENU(1339,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Mining
            player->SEND_POI(-1249.17, 155, 6, 6, 0, "Stonehoof Geology");
            player->SEND_GOSSIP_MENU(1340,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                    //Skinning
            player->SEND_POI(-1148.56, 51.18, 6, 6, 0, "Mooranta");
            player->SEND_GOSSIP_MENU(1343,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Tailoring
            player->SEND_POI(-1156.22, 66.86, 6, 6, 0, "Thunder Bluff Armorers");
            player->SEND_GOSSIP_MENU(1341,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_bluffwatcher(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_bluffwatcher(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_bluffwatcher(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_bluffwatcher(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:  SendBattleMasterMenu_guard_bluffwatcher(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_bluffwatcher end
 *******************************************************/

CreatureAI* GetAI_guard_bluffwatcher(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_contested start
 *******************************************************/

struct MANGOS_DLL_DECL guard_contested : public guardAI
{
    guard_contested(Creature *c) : guardAI(c) {}
    void MoveInLineOfSight(Unit *who)
    {
        if ( who->isAttackingPlayer() )
        {
            if(who->GetTypeId() == TYPEID_PLAYER || who->GetOwnerGUID() && GUID_HIPART(who->GetOwnerGUID())==HIGHGUID_PLAYER)
            {
                m_creature->AddThreat(who, 0.0f);
                if(Unit* owner = who->GetOwner())
                    m_creature->AddThreat(owner, 0.0f);

                if(!m_creature->isInCombat())
                {
                    if (m_creature->GetEntry() == 15184)//Cenarion Hold Infantry
                    {
                        srand (time(NULL));
                        if (rand()%100 <= 30)
                        {
                            DoSay("Taste blade, mongrel!", LANG_UNIVERSAL,NULL);
                        }
                        else if (rand()%100 > 30 && rand()%100 < 50)
                        {
                            DoSay("Please tell me that you didn`t just do what I think you just did. Please tell me that I`m not going to have to hurt you...", LANG_UNIVERSAL,NULL);
                        }
                        else if (rand()%100 >= 50)
                        {
                            DoSay("As if we don`t have enough problems, you go and create more!", LANG_UNIVERSAL,NULL);
                        }
                    }
                    else 
                    {
                        SpellEntry const *spell = m_creature->reachWithSpellAttack(who);
                        DoCastSpell(who, spell);
                    }
                }
                DoStartMeleeAttack(who);
            }
        }
        
    }
};
/*******************************************************
 * guard_contested end
 *******************************************************/

CreatureAI* GetAI_guard_contested(Creature *_Creature)
{
    return new guard_contested (_Creature);
}

/*******************************************************
 * guard_darnassus start
 *******************************************************/

bool GossipHello_guard_darnassus(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_HIPPOGRYPH     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN            , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->SEND_GOSSIP_MENU(3016, _Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_darnassus(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Auction house
            player->SEND_POI(9861.23, 2334.55, 6, 6, 0, "Darnassus Auction House");
            player->SEND_GOSSIP_MENU(3833, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Bank
            player->SEND_POI(9938.45, 2512.35, 6, 6, 0, "Darnassus Bank");
            player->SEND_GOSSIP_MENU(3017, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Wind master
            player->SEND_POI(9945.65, 2618.94, 6, 6, 0, "Rut'theran Village");
            player->SEND_GOSSIP_MENU(3018, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Guild master
            player->SEND_POI(10076.40, 2199.59, 6, 6, 0, "Darnassus Guild Master");
            player->SEND_GOSSIP_MENU(3019, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Inn
            player->SEND_POI(10133.29, 2222.52, 6, 6, 0, "Darnassus Inn");
            player->SEND_GOSSIP_MENU(3020, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Mailbox
            player->SEND_POI(9942.17, 2495.48, 6, 6, 0, "Darnassus Mailbox");
            player->SEND_GOSSIP_MENU(3021, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Stable master
            player->SEND_POI(10167.20, 2522.66, 6, 6, 0, "Alassin");
            player->SEND_GOSSIP_MENU(5980, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Weapon trainer
            player->SEND_POI(9907.11, 2329.70, 6, 6, 0, "Ilyenia Moonfire");
            player->SEND_GOSSIP_MENU(4517, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Battlemaster
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(7519, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID           , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE           , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR         , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(4264, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING  , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->SEND_GOSSIP_MENU(4273, _Creature->GetGUID());
            break;
    }
}

void SendBattleMasterMenu_guard_darnassus(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //AV
            player->SEND_POI(9923.61, 2327.43, 6, 6, 0, "Brogun Stoneshield");
            player->SEND_GOSSIP_MENU(7518, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //AB
            player->SEND_POI(9977.37, 2324.39, 6, 6, 0, "Keras Wolfheart");
            player->SEND_GOSSIP_MENU(7651,  _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //WSG
            player->SEND_POI(9979.84, 2315.79, 6, 6, 0, "Aethalas");
            player->SEND_GOSSIP_MENU(7482, _Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_darnassus(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Druid
            player->SEND_POI(10186, 2570.46, 6, 6, 0, "Darnassus Druid Trainer");
            player->SEND_GOSSIP_MENU(3024, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Hunter
            player->SEND_POI(10177.29, 2511.10, 6, 6, 0, "Darnassus Hunter Trainer");
            player->SEND_GOSSIP_MENU(3023, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Priest
            player->SEND_POI(9659.12, 2524.88, 6, 6, 0, "Temple of the Moon");
            player->SEND_GOSSIP_MENU(3025, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Rogue
            player->SEND_POI(10122, 2599.12, 6, 6, 0, "Darnassus Rogue Trainer");
            player->SEND_GOSSIP_MENU(3026, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Warrior
            player->SEND_POI(9951.91, 2280.38, 6, 6, 0, "Warrior's Terrace");
            player->SEND_GOSSIP_MENU(3033, _Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_darnassus(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(10075.90, 2356.76, 6, 6, 0, "Darnassus Alchemy Trainer");
            player->SEND_GOSSIP_MENU(3035, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Cooking
            player->SEND_POI(10088.59, 2419.21, 6, 6, 0, "Darnassus Cooking Trainer");
            player->SEND_GOSSIP_MENU(3036, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Enchanting
            player->SEND_POI(10146.09, 2313.42, 6, 6, 0, "Darnassus Enchanting Trainer");
            player->SEND_GOSSIP_MENU(3337, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //First Aid
            player->SEND_POI(10150.09, 2390.43, 6, 6, 0, "Darnassus First Aid Trainer");
            player->SEND_GOSSIP_MENU(3037, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Fishing
            player->SEND_POI(9836.20, 2432.17, 6, 6, 0, "Darnassus Fishing Trainer");
            player->SEND_GOSSIP_MENU(3038, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Herbalism
            player->SEND_POI(9757.17, 2430.16, 6, 6, 0, "Darnassus Herbalism Trainer");
            player->SEND_GOSSIP_MENU(3039, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Leatherworking
            player->SEND_POI(10086.59, 2255.77, 6, 6, 0, "Darnassus Leatherworking Trainer");
            player->SEND_GOSSIP_MENU(3040, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Skinning
            player->SEND_POI(10081.40, 2257.18, 6, 6, 0, "Darnassus Skinning Trainer");
            player->SEND_GOSSIP_MENU(3042, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Tailoring
            player->SEND_POI(10079.70, 2268.19, 6, 6, 0, "Darnassus Tailor");
            player->SEND_GOSSIP_MENU(3044, _Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_darnassus(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_darnassus(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_darnassus(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_darnassus(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:  SendBattleMasterMenu_guard_darnassus(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_darnassus end
 *******************************************************/

CreatureAI* GetAI_guard_darnassus(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_dunmorogh start
 *******************************************************/

bool GossipHello_guard_dunmorogh(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK             , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_HIPPOGRYPH       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN              , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_dunmorogh(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Bank
            player->SEND_GOSSIP_MENU(4288,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Gryphon master
            player->SEND_GOSSIP_MENU(4289,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Guild master
            player->SEND_GOSSIP_MENU(4290,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Inn
            player->SEND_POI(-5582.66, -525.89, 6, 6, 0, "Thunderbrew Distillery");
            player->SEND_GOSSIP_MENU(4291,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Stable Master
            player->SEND_POI(-5604, -509.58, 6, 6, 0, "Shelby Stoneflint");
            player->SEND_GOSSIP_MENU(5985,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                 , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->SEND_GOSSIP_MENU(4292,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING               , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(4300,_Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_dunmorogh(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Hunter
            player->SEND_POI(-5618.29, -454.25, 6, 6, 0, "Grif Wildheart");
            player->SEND_GOSSIP_MENU(4293,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Mage
            player->SEND_POI(-5585.6, -539.99, 6, 6, 0, "Magis Sparkmantle");
            player->SEND_GOSSIP_MENU(4294,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Paladin
            player->SEND_POI(-5585.6, -539.99, 6, 6, 0, "Azar Stronghammer");
            player->SEND_GOSSIP_MENU(4295,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Priest
            player->SEND_POI(-5591.74, -525.61, 6, 6, 0, "Maxan Anvol");
            player->SEND_GOSSIP_MENU(4296,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Rogue
            player->SEND_POI(-5602.75, -542.4, 6, 6, 0, "Hogral Bakkan");
            player->SEND_GOSSIP_MENU(4297,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Warlock
            player->SEND_POI(-5641.97, -523.76, 6, 6, 0, "Gimrizz Shadowcog");
            player->SEND_GOSSIP_MENU(4298,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Warrior
            player->SEND_POI(-5604.79, -529.38, 6, 6, 0, "Granis Swiftaxe");
            player->SEND_GOSSIP_MENU(4299,_Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_dunmorogh(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_GOSSIP_MENU(4301,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(-5584.72, -428.41, 6, 6, 0, "Tognus Flintfire");
            player->SEND_GOSSIP_MENU(4302,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(-5596.85, -541.43, 6, 6, 0, "Gremlock Pilsnor");
            player->SEND_GOSSIP_MENU(4303,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_GOSSIP_MENU(4304,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_POI(-5531, -666.53, 6, 6, 0, "Bronk Guzzlegear");
            player->SEND_GOSSIP_MENU(4305,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(-5603.67, -523.57, 6, 6, 0, "Thamner Pol");
            player->SEND_GOSSIP_MENU(4306,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(-5202.39, -51.36, 6, 6, 0, "Paxton Ganter");
            player->SEND_GOSSIP_MENU(4307,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Herbalism
            player->SEND_GOSSIP_MENU(4308,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Leatherworking
            player->SEND_GOSSIP_MENU(4310,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Mining
            player->SEND_POI(-5531, -666.53, 6, 6, 0, "Yarr Hamerstone");
            player->SEND_GOSSIP_MENU(4311,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Skinning
            player->SEND_GOSSIP_MENU(4312,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Tailoring
            player->SEND_GOSSIP_MENU(4313,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_dunmorogh(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_dunmorogh(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_dunmorogh(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_dunmorogh(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_dunmorogh end
 *******************************************************/

CreatureAI* GetAI_guard_dunmorogh(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_durotar start
 *******************************************************/

bool GossipHello_guard_durotar(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK         , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_WINDRIDER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->SEND_GOSSIP_MENU(4037,_Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_durotar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Bank
            player->SEND_GOSSIP_MENU(4032,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Wind rider
            player->SEND_GOSSIP_MENU(4033,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Inn
            player->SEND_POI(338.7, -4688.87, 6, 6, 0, "Razor Hill Inn");
            player->SEND_GOSSIP_MENU(4034,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Stable master
            player->SEND_POI(330.31, -4710.66, 6, 6, 0, "Shoja'my");
            player->SEND_GOSSIP_MENU(5973,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->SEND_GOSSIP_MENU(4035,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(4036,_Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_durotar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Hunter
            player->SEND_POI(276, -4706.72, 6, 6, 0, "Thotar");
            player->SEND_GOSSIP_MENU(4013,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Mage
            player->SEND_POI(-839.33, -4935.6, 6, 6, 0, "Un'Thuwa");
            player->SEND_GOSSIP_MENU(4014,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Priest
            player->SEND_POI(296.22, -4828.1, 6, 6, 0, "Tai'jin");
            player->SEND_GOSSIP_MENU(4015,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Rogue
            player->SEND_POI(265.76, -4709, 6, 6, 0, "Kaplak");
            player->SEND_GOSSIP_MENU(4016,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Shaman
            player->SEND_POI(307.79, -4836.97, 6, 6, 0, "Swart");
            player->SEND_GOSSIP_MENU(4017,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Warlock
            player->SEND_POI(355.88, -4836.45, 6, 6, 0, "Dhugru Gorelust");
            player->SEND_GOSSIP_MENU(4018,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Warrior
            player->SEND_POI(312.3, -4824.66, 6, 6, 0, "Tarshaw Jaggedscar");
            player->SEND_GOSSIP_MENU(4019,_Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_durotar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(-800.25, -4894.33, 6, 6, 0, "Miao'zan");
            player->SEND_GOSSIP_MENU(4020,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(373.24, -4716.45, 6, 6, 0, "Dwukk");
            player->SEND_GOSSIP_MENU(4021,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_GOSSIP_MENU(4022,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_GOSSIP_MENU(4023,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_POI(368.95, -4723.95, 6, 6, 0, "Mukdrak");
            player->SEND_GOSSIP_MENU(4024,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(327.17, -4825.62, 6, 6, 0, "Rawrk");
            player->SEND_GOSSIP_MENU(4025,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(-1065.48, -4777.43, 6, 6, 0, "Lau'Tiki");
            player->SEND_GOSSIP_MENU(4026,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Herbalism
            player->SEND_POI(-836.25, -4896.89, 6, 6, 0, "Mishiki");
            player->SEND_GOSSIP_MENU(4027,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Leatherworking
            player->SEND_GOSSIP_MENU(4028,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Mining
            player->SEND_POI(366.94, -4705, 6, 6, 0, "Krunn");
            player->SEND_GOSSIP_MENU(4029,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Skinning
            player->SEND_GOSSIP_MENU(4030,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Tailoring
            player->SEND_GOSSIP_MENU(4031,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_durotar(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_durotar(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_durotar(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_durotar(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_durotar end
 *******************************************************/

CreatureAI* GetAI_guard_durotar(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_elwynnforest start
 *******************************************************/

bool GossipHello_guard_elwynnforest(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_GRYPHON       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_elwynnforest(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Bank
            player->SEND_GOSSIP_MENU(4260,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Gryphon master
            player->SEND_GOSSIP_MENU(4261,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Guild master
            player->SEND_GOSSIP_MENU(4262,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Inn
            player->SEND_POI(-9459.34, 42.08, 6, 6, 0, "Lion's Pride Inn");
            player->SEND_GOSSIP_MENU(4263,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Stable Master
            player->SEND_POI(-9466.62, 45.87, 6, 6, 0, "Erma");
            player->SEND_GOSSIP_MENU(5983,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->SEND_GOSSIP_MENU(4264,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(4273,_Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_elwynnforest(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Druid
            player->SEND_GOSSIP_MENU(4265,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Hunter
            player->SEND_GOSSIP_MENU(4266,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Mage
            player->SEND_POI(-9471.12, 33.44, 6, 6, 0, "Zaldimar Wefhellt");
            player->SEND_GOSSIP_MENU(4268,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Paladin
            player->SEND_POI(-9469, 108.05, 6, 6, 0, "Brother Wilhelm");
            player->SEND_GOSSIP_MENU(4269,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Priest
            player->SEND_POI(-9461.07, 32.6, 6, 6, 0, "Priestess Josetta");
            player->SEND_GOSSIP_MENU(4267,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Rogue
            player->SEND_POI(-9465.13, 13.29, 6, 6, 0, "Keryn Sylvius");
            player->SEND_GOSSIP_MENU(4270,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Warlock
            player->SEND_POI(-9473.21, -4.08, 6, 6, 0, "Maximillian Crowe");
            player->SEND_GOSSIP_MENU(4272,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Warrior
            player->SEND_POI(-9461.82, 109.50, 6, 6, 0, "Lyria Du Lac");
            player->SEND_GOSSIP_MENU(4271,_Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_elwynnforest(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(-9057.04, 153.63, 6, 6, 0, "Alchemist Mallory");
            player->SEND_GOSSIP_MENU(4274,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(-9456.58, 87.90, 6, 6, 0, "Smith Argus");
            player->SEND_GOSSIP_MENU(4275,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(-9467.54, -3.16, 6, 6, 0, "Tomas");
            player->SEND_GOSSIP_MENU(4276,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_GOSSIP_MENU(4277,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_GOSSIP_MENU(4278,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(-9456.82, 30.49, 6, 6, 0, "Michelle Belle");
            player->SEND_GOSSIP_MENU(4279,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(-9386.54, -118.73, 6, 6, 0, "Lee Brown");
            player->SEND_GOSSIP_MENU(4280,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Herbalism
            player->SEND_POI(-9060.70, 149.23, 6, 6, 0, "Herbalist Pomeroy");
            player->SEND_GOSSIP_MENU(4281,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Leatherworking
            player->SEND_POI(-9376.12, -75.23, 6, 6, 0, "Adele Fielder");
            player->SEND_GOSSIP_MENU(4282,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Mining
            player->SEND_GOSSIP_MENU(4283,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Skinning
            player->SEND_POI(-9536.91, -1212.76, 6, 6, 0, "Helene Peltskinner");
            player->SEND_GOSSIP_MENU(4284,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Tailoring
            player->SEND_POI(-9376.12, -75.23, 6, 6, 0, "Eldrin");
            player->SEND_GOSSIP_MENU(4285,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_elwynnforest(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_elwynnforest(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_elwynnforest(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_elwynnforest(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_elwynnforest end
 *******************************************************/

CreatureAI* GetAI_guard_elwynnforest(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_exodar start
 *******************************************************/

bool GossipHello_guard_exodar(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_HIPPOGRYPH     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN            , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_exodar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Auction house
            player->SEND_POI(-4023.6, -11739.3, 6, 6, 0, "Exodar Auction House");//18349
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Bank
            player->SEND_POI(-3923.89, -11544.5, 6, 6, 0, "Exodar Bank");//17773
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Guild master
            player->SEND_POI(-4092.57, -11626.5, 6, 6, 0, "Exodar Guild Master");//16734
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Hippogryph master
            player->SEND_POI(-4060.46, -11787.1, 6, 6, 0, "Exodar Hippogryph Master");//17555
            player->SEND_GOSSIP_MENU(9530, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                     //Inn
            player->SEND_POI(-3741.87, -11695.1, 6, 6, 0, "Exodar Inn");//16739
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Mailbox
            //player->SEND_POI(1, 1, 6, 6, 0, "Exodar Mailbox");//go 
            player->SEND_GOSSIP_MENU(10254, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Stable master
            player->SEND_POI(-3786.5, -11702.5, 6, 6, 0, "Stable Master Arthaid");//16764
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Weapon trainer
            player->SEND_POI(-4215.68, -11628.9, 6, 6, 0, "Weapon Master Handiir");//16773
            player->SEND_GOSSIP_MENU(9565, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Battlemaster
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY       , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN         , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARENA               , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_EYEOFTHESTORM       , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 4);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH         , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(9531, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_JEWELCRAFTING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 13);
            player->SEND_GOSSIP_MENU(9555, _Creature->GetGUID());
            break;
    }
}

void SendBattleMasterMenu_guard_exodar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //AV
            player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Alterac Valley Battlemaster");
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());//incorrect id
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //AB
            player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Arathi Basin Battlemaster");
            player->SEND_GOSSIP_MENU(9551,  _Creature->GetGUID());//incorrect id
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //A
            player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Arena Battlemaster");
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());//incorrect id
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //EOS
            player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Eye Of The Storm Battlemaster");
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());//incorrect id
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //WSG
            player->SEND_POI(-3735.7, -11683.63, 6, 6, 0, "Warsong Gulch Battlemaster");
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());//incorrect id
            break;
    }
}

void SendClassTrainerMenu_guard_exodar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Druid
            player->SEND_POI(-4276, -11495, 6, 6, 0, "Exodar Druid Trainer");//16721
            player->SEND_GOSSIP_MENU(9555, _Creature->GetGUID());//incorrect id
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Hunter
            player->SEND_POI(-4232.18, -11569.2, 6, 6, 0, "Exodar Hunter Trainer");//17122
            player->SEND_GOSSIP_MENU(9555, _Creature->GetGUID());//incorrect id
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Mage
            player->SEND_POI(-4057.32, -11556.5, 6, 6, 0, "Exodar Mage Trainer");//17513
            player->SEND_GOSSIP_MENU(9550, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Paladin
            player->SEND_POI(-4191.2, -11470.4, 6, 6, 0, "Exodar Paladin Trainer");//17509
            player->SEND_GOSSIP_MENU(9553, _Creature->GetGUID());//incorrect id
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Priest
            player->SEND_POI(-3969.63, -11482.8, 6, 6, 0, "Exodar Priest Trainer");//17510
            player->SEND_GOSSIP_MENU(9555, _Creature->GetGUID());//incorrect id
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Shaman
            player->SEND_POI(-3886.88, -11339.7, 6, 6, 0, "Exodar Shaman Trainer");//17520
            player->SEND_GOSSIP_MENU(9556, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Warrior
            player->SEND_POI(-4189.43, -11653.7, 6, 6, 0, "Exodar Warrior Trainer");//17120
            player->SEND_GOSSIP_MENU(9555, _Creature->GetGUID());//incorrect id
            break;
    }
}

void SendProfTrainerMenu_guard_exodar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(-4146.6, -12531.8, 6, 6, 0, "Exodar Alchemy Trainer");//17215
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(-4728, -12389, 6, 6, 0, "Exodar Blacksmithing Trainer");//17245
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(-4710, -12393.8, 6, 6, 0, "Exodar Cooking Trainer");//17246
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_POI(-3889.3, -11495, 6, 6, 0, "Exodar Enchanting Trainer");//16742
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_POI(-4257.68, -11640.3, 6, 6, 0, "Exodar Engineering Trainer");//16743
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(-1928.4, -11857, 6, 6, 0, "Exodar First Aid Trainer");//17424
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(-4266.52, -12985.2, 6, 6, 0, "Exodar Fishing Trainer");//17101
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Jewelcrafting
            player->SEND_POI(-3783, -11546, 6, 6, 0, "Exodar Jewelcrafting Trainer");//16727,16744,19777,19778
            player->SEND_GOSSIP_MENU(9547, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Herbalism
            player->SEND_POI(-4052, -11356, 6, 6, 0, "Exodar Herbalist Trainer");//16736
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Leatherworking
            player->SEND_POI(-4140.6, -11776.7, 6, 6, 0, "Exodar Leatherworking Trainer");//16745
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Mining
            player->SEND_POI(-4228, -11697, 6, 6, 0, "Exodar Mining Trainer");//16752
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Skinning
            player->SEND_POI(-4134.97, -11760.5, 6, 6, 0, "Exodar Skinning Trainer");//16763
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 13:                   //Tailoring
            player->SEND_POI(-4092.5, -11744.5, 6, 6, 0, "Exodar Tailor Trainer");//16746, 16729
            player->SEND_GOSSIP_MENU(9551, _Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_exodar(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_exodar(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_exodar(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_exodar(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:  SendBattleMasterMenu_guard_exodar(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_exodar end
 *******************************************************/

CreatureAI* GetAI_guard_exodar(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_ironforge start
 *******************************************************/

bool GossipHello_guard_ironforge(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_IRONFORGE_BANK    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_DEEPRUNTRAM       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_GRYPHON           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN               , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
    player->SEND_GOSSIP_MENU(2760, _Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_ironforge(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Auction House
            player->SEND_POI(-4957.39, -911.6, 6, 6, 0, "Ironforge Auction House");
            player->SEND_GOSSIP_MENU(3014, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Bank
            player->SEND_POI(-4891.91, -991.47, 6, 6, 0, "The Vault");
            player->SEND_GOSSIP_MENU(2761, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Tram
            player->SEND_POI(-4835.27, -1294.69, 6, 6, 0, "Deeprun Tram");
            player->SEND_GOSSIP_MENU(3814, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Gryphon Master
            player->SEND_POI(-4821.52, -1152.3, 6, 6, 0, "Ironforge Gryphon Master");
            player->SEND_GOSSIP_MENU(2762, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Guild Master
            player->SEND_POI(-5021, -996.45, 6, 6, 0, "Ironforge Visitor's Center");
            player->SEND_GOSSIP_MENU(2764, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Inn
            player->SEND_POI(-4850.47, -872.57, 6, 6, 0, "Stonefire Tavern");
            player->SEND_GOSSIP_MENU(2768, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Mailbox
            player->SEND_POI(-4845.7, -880.55, 6, 6, 0, "Ironforge Mailbox");
            player->SEND_GOSSIP_MENU(2769, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Stable Master
            player->SEND_POI(-5010.2, -1262, 6, 6, 0, "Ulbrek Firehand");
            player->SEND_GOSSIP_MENU(5986, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Weapons Trainer
            player->SEND_POI(-5040, -1201.88, 6, 6, 0, "Bixi and Buliwyf");
            player->SEND_GOSSIP_MENU(4518, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Battlemaster
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(7529, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Class Trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE            , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN         , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE           , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK         , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR         , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->SEND_GOSSIP_MENU(2766, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Profession Trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING   , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING     , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING  , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(2793, _Creature->GetGUID());
            break;
    }
}

void SendBattleMasterMenu_guard_ironforge(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //AV
            player->SEND_POI(-5047.87, -1263.77, 6, 6, 0, "Glordrum Steelbeard");
            player->SEND_GOSSIP_MENU(7483, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //AB
            player->SEND_POI(-5038.37, -1266.39, 6, 6, 0, "Donal Osgood");
            player->SEND_GOSSIP_MENU(7649, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //WSG
            player->SEND_POI(-5037.24, -1274.82, 6, 6, 0, "Lylandris");
            player->SEND_GOSSIP_MENU(7528, _Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_ironforge(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Hunter
            player->SEND_POI(-5023, -1253.68, 6, 6, 0, "Hall of Arms");
            player->SEND_GOSSIP_MENU(2770, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Mage
            player->SEND_POI(-4627, -926.45, 6, 6, 0, "Hall of Mysteries");
            player->SEND_GOSSIP_MENU(2771, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Paladin
            player->SEND_POI(-4627.02, -926.45, 6, 6, 0, "Hall of Mysteries");
            player->SEND_GOSSIP_MENU(2773, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Priest
            player->SEND_POI(-4627, -926.45, 6, 6, 0, "Hall of Mysteries");
            player->SEND_GOSSIP_MENU(2772, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Rogue
            player->SEND_POI(-4647.83, -1124, 6, 6, 0, "Ironforge Rogue Trainer");
            player->SEND_GOSSIP_MENU(2774, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Warlock
            player->SEND_POI(-4605, -1110.45, 6, 6, 0, "Ironforge Warlock Trainer");
            player->SEND_GOSSIP_MENU(2775, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Warrior
            player->SEND_POI(-5023.08, -1253.68, 6, 6, 0, "Hall of Arms");
            player->SEND_GOSSIP_MENU(2776, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Shaman
            player->SEND_POI(-4732, -1147, 6, 6, 0, "Ironforge Shaman Trainer");
            player->SEND_GOSSIP_MENU(2766, _Creature->GetGUID());//incorrect id
            break;
    }
}

void SendProfTrainerMenu_guard_ironforge(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(-4858.5, -1241.83, 6, 6, 0, "Berryfizz's Potions and Mixed Drinks");
            player->SEND_GOSSIP_MENU(2794, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(-4796.97, -1110.17, 6, 6, 0, "The Great Forge");
            player->SEND_GOSSIP_MENU(2795, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(-4767.83, -1184.59, 6, 6, 0, "The Bronze Kettle");
            player->SEND_GOSSIP_MENU(2796, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_POI(-4803.72, -1196.53, 6, 6, 0, "Thistlefuzz Arcanery");
            player->SEND_GOSSIP_MENU(2797, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_POI(-4799.56, -1250.23, 6, 6, 0, "Springspindle's Gadgets");
            player->SEND_GOSSIP_MENU(2798, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(-4881.6, -1153.13, 6, 6, 0, "Ironforge Physician");
            player->SEND_GOSSIP_MENU(2799, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(-4597.91, -1091.93, 6, 6, 0, "Traveling Fisherman");
            player->SEND_GOSSIP_MENU(2800, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Herbalism
            player->SEND_POI(-4876.9, -1151.92, 6, 6, 0, "Ironforge Physician");
            player->SEND_GOSSIP_MENU(2801, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Leatherworking
            player->SEND_POI(-4745, -1027.57, 6, 6, 0, "Finespindle's Leather Goods");
            player->SEND_GOSSIP_MENU(2802, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Minning
            player->SEND_POI(-4705.06, -1116.43, 6, 6, 0, "Deepmountain Mining Guild");
            player->SEND_GOSSIP_MENU(2804, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Skinning
            player->SEND_POI(-4745, -1027.57, 6, 6, 0, "Finespindle's Leather Goods");
            player->SEND_GOSSIP_MENU(2805, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Tailoring
            player->SEND_POI(-4719.60, -1056.96, 6, 6, 0, "Stonebrow's Clothier");
            player->SEND_GOSSIP_MENU(2807, _Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_ironforge(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_ironforge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_ironforge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_ironforge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:  SendBattleMasterMenu_guard_ironforge(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_ironforge end
 *******************************************************/

CreatureAI* GetAI_guard_ironforge(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_mulgore start
 *******************************************************/

bool GossipHello_guard_mulgore(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK         , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_WINDRIDER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->SEND_GOSSIP_MENU(3543,_Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_mulgore(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Bank
            player->SEND_GOSSIP_MENU(4051,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Wind rider
            player->SEND_GOSSIP_MENU(4052,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Inn
            player->SEND_POI(-2361.38, -349.19, 6, 6, 0, "Bloodhoof Village Inn");
            player->SEND_GOSSIP_MENU(4053,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Stable master
            player->SEND_POI(-2338.86, -357.56, 6, 6, 0, "Seikwa");
            player->SEND_GOSSIP_MENU(5976,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID           , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR         , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->SEND_GOSSIP_MENU(4069,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING   , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING  , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->SEND_GOSSIP_MENU(3541,_Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_mulgore(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Druid
            player->SEND_POI(-2312.15, -443.69, 6, 6, 0, "Gennia Runetotem");
            player->SEND_GOSSIP_MENU(4054,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Hunter
            player->SEND_POI(-2178.14, -406.14, 6, 6, 0, "Yaw Sharpmane");
            player->SEND_GOSSIP_MENU(4055,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Shaman
            player->SEND_POI(-2301.5, -439.87, 6, 6, 0, "Narm Skychaser");
            player->SEND_GOSSIP_MENU(4056,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Warrior
            player->SEND_POI(-2345.43, -494.11, 6, 6, 0, "Krang Stonehoof");
            player->SEND_GOSSIP_MENU(4057,_Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_mulgore(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_GOSSIP_MENU(4058,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_GOSSIP_MENU(4059,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(-2263.34, -287.91, 6, 6, 0, "Pyall Silentstride");
            player->SEND_GOSSIP_MENU(4060,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_GOSSIP_MENU(4061,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //First Aid
            player->SEND_POI(-2353.52, -355.82, 6, 6, 0, "Vira Younghoof");
            player->SEND_GOSSIP_MENU(4062,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Fishing
            player->SEND_POI(-2349.21, -241.37, 6, 6, 0, "Uthan Stillwater");
            player->SEND_GOSSIP_MENU(4063,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Herbalism
            player->SEND_GOSSIP_MENU(4064,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Leatherworking
            player->SEND_POI(-2257.12, -288.63, 6, 6, 0, "Chaw Stronghide");
            player->SEND_GOSSIP_MENU(4065,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Mining
            player->SEND_GOSSIP_MENU(4066,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                    //Skinning
            player->SEND_POI(-2252.94, -291.32, 6, 6, 0, "Yonn Deepcut");
            player->SEND_GOSSIP_MENU(4067,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                    //Tailoring
            player->SEND_GOSSIP_MENU(4068,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_mulgore(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_mulgore(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_mulgore(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_mulgore(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_mulgore end
 *******************************************************/

CreatureAI* GetAI_guard_mulgore(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_orgrimmar start
 *******************************************************/

bool GossipHello_guard_orgrimmar(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_WINDRIDER     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_ZEPPLINMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 9, GOSSIP_TEXT_OFFICERS      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
    player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());

    return true;
}

/*******************************************************
 *                Start of GOSSIP_MENU
 *******************************************************/

void SendDefaultMenu_guard_orgrimmar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Bank
            player->SEND_POI(1631.51, -4375.33, 6, 6, 0, "Bank of Orgrimmar");
            player->SEND_GOSSIP_MENU(2554,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //wind rider
            player->SEND_POI(1676.6, -4332.72, 6, 6, 0, "The Sky Tower");
            player->SEND_GOSSIP_MENU(2555,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //guild master
            player->SEND_POI(1576.93, -4294.75, 6, 6, 0, "Horde Embassy");
            player->SEND_GOSSIP_MENU(2556,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Inn
            player->SEND_POI(1644.51, -4447.27, 6, 6, 0, "Orgrimmar Inn");
            player->SEND_GOSSIP_MENU(2557,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //mailbox
            player->SEND_POI(1622.53, -4388.79, 6, 6, 0, "Orgrimmar Mailbox");
            player->SEND_GOSSIP_MENU(2558,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //auction house
            player->SEND_POI(1679.21, -4450.1, 6, 6, 0, "Orgrimmar Auction House");
            player->SEND_GOSSIP_MENU(3075,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //zeppelin
            player->SEND_POI(1337.36, -4632.7, 6, 6, 0, "Orgrimmar Zeppelin Tower");
            player->SEND_GOSSIP_MENU(3173,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //weapon master
            player->SEND_POI(2092.56, -4823.95, 6, 6, 0, "Sayoc & Hanashi");
            player->SEND_GOSSIP_MENU(4519,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //stable master
            player->SEND_POI(2133.12, -4663.93, 6, 6, 0, "Xon'cha");
            player->SEND_GOSSIP_MENU(5974,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //officers lounge
            player->SEND_POI(1633.56, -4249.37, 6, 6, 0, "Hall of Legends");
            player->SEND_GOSSIP_MENU(7046,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                    //battlemaster
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY       , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN         , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH         , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(7521,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR             , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->SEND_GOSSIP_MENU(2599,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 13:                   //profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(2594,_Creature->GetGUID());
            break;
    }
}

void SendBattleMasterMenu_guard_orgrimmar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //AV
            player->SEND_POI(1983.92, -4794.2, 6, 6, 0, "Hall of the Brave");
            player->SEND_GOSSIP_MENU(7484,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //AB
            player->SEND_POI(1983.92, -4794.2, 6, 6, 0, "Hall of the Brave");
            player->SEND_GOSSIP_MENU(7644,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //WSG
            player->SEND_POI(1983.92, -4794.2, 6, 6, 0, "Hall of the Brave");
            player->SEND_GOSSIP_MENU(7520,_Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_orgrimmar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Hunter
            player->SEND_POI(2114.84, -4625.31, 6, 6, 0, "Orgrimmar Hunter's Hall");
            player->SEND_GOSSIP_MENU(2559,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Mage
            player->SEND_POI(1451.26, -4223.33, 6, 6, 0, "Darkbriar Lodge");
            player->SEND_GOSSIP_MENU(2560,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Priest
            player->SEND_POI(1442.21, -4183.24, 6, 6, 0, "Spirit Lodge");
            player->SEND_GOSSIP_MENU(2561,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Shaman
            player->SEND_POI(1925.34, -4181.89, 6, 6, 0, "Thrall's Fortress");
            player->SEND_GOSSIP_MENU(2562,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Rogue
            player->SEND_POI(1773.39, -4278.97, 6, 6, 0, "Shadowswift Brotherhood");
            player->SEND_GOSSIP_MENU(2563,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Warlock
            player->SEND_POI(1849.57, -4359.68, 6, 6, 0, "Darkfire Enclave");
            player->SEND_GOSSIP_MENU(2564,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Warrior
            player->SEND_POI(1983.92, -4794.2, 6, 6, 0, "Hall of the Brave");
            player->SEND_GOSSIP_MENU(2565,_Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_orgrimmar(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(1955.17, -4475.79, 6, 6, 0, "Yelmak's Alchemy and Potions");
            player->SEND_GOSSIP_MENU(2497,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(2054.34, -4831.85, 6, 6, 0, "The Burning Anvil");
            player->SEND_GOSSIP_MENU(2499,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(1780.96, -4481.31, 6, 6, 0, "Borstan's Firepit");
            player->SEND_GOSSIP_MENU(2500,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_POI(1917.5, -4434.95, 6, 6, 0, "Godan's Runeworks");
            player->SEND_GOSSIP_MENU(2501,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_POI(2038.45, -4744.75, 6, 6, 0, "Nogg's Machine Shop");
            player->SEND_GOSSIP_MENU(2653,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(1485.21, -4160.91, 6, 6, 0, "Survival of the Fittest");
            player->SEND_GOSSIP_MENU(2502,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(1994.15, -4655.7, 6, 6, 0, "Lumak's Fishing");
            player->SEND_GOSSIP_MENU(2503,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Herbalism
            player->SEND_POI(1898.61, -4454.93, 6, 6, 0, "Jandi's Arboretum");
            player->SEND_GOSSIP_MENU(2504,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Leatherworking
            player->SEND_POI(1852.82, -4562.31, 6, 6, 0, "Kodohide Leatherworkers");
            player->SEND_GOSSIP_MENU(2513,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Mining
            player->SEND_POI(2029.79, -4704, 6, 6, 0, "Red Canyon Mining");
            player->SEND_GOSSIP_MENU(2515,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Skinning
            player->SEND_POI(1852.82, -4562.31, 6, 6, 0, "Kodohide Leatherworkers");
            player->SEND_GOSSIP_MENU(2516,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                    //Tailoring
            player->SEND_POI(1802.66, -4560.66, 6, 6, 0, "Magar's Cloth Goods");
            player->SEND_GOSSIP_MENU(2518,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_orgrimmar(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_orgrimmar(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_orgrimmar(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_orgrimmar(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:  SendBattleMasterMenu_guard_orgrimmar(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_orgrimmar end
 *******************************************************/

CreatureAI* GetAI_guard_orgrimmar(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_shattrath start
 *******************************************************/

bool GossipHello_guard_shattrath(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_TAVERN                       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK                         , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN                          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_FLIGHTMASTER                 , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX                      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER                 , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER                 , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER                  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_MANALOOM                     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_ALCHEMYLAB                   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GEMMERCHANT                  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Tavern
            player->SEND_POI(-1759.5, 5165, 6, 6, 0, "Worlds End Tavern");
            player->SEND_GOSSIP_MENU(10394, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Bank
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BANKSCYERS           , GOSSIP_SENDER_SEC_BANK, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BANKALDOR            , GOSSIP_SENDER_SEC_BANK, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(10379, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Inn
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_INNSCYERS            , GOSSIP_SENDER_SEC_INN, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_INNALDOR             , GOSSIP_SENDER_SEC_INN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Flight master
            player->SEND_POI(-1832, 5299, 6, 6, 0, "Shattrath Flight Master");//18940
            player->SEND_GOSSIP_MENU(10385, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Mailbox
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_MAILBOXSCYERS        , GOSSIP_SENDER_SEC_MAILBOX, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_MAILBOXALDOR         , GOSSIP_SENDER_SEC_MAILBOX, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Stable master
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_STABLESCYERS         , GOSSIP_SENDER_SEC_STABLEMASTER, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_STABLEALDOR          , GOSSIP_SENDER_SEC_STABLEMASTER, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Battlemaster
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTERALLIANCE , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTERHORDE    , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Profession master
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_JEWELCRAFTING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Mana Loom
            player->SEND_POI(-2070, 5265.5, 6, 6, 0, "Mana Loom");//GO in the Lower City
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Alchemy Lab
            player->SEND_POI(-1648.5, 5540, 6, 6, 0, "Alchemy Lab");//GO in the Lower City, arokkar tower
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Gem Merchant
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_GEMSCYERS            , GOSSIP_SENDER_SEC_GEMMERCHANT, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_GEMALDOR             , GOSSIP_SENDER_SEC_GEMMERCHANT, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(10697, _Creature->GetGUID());
            break;
    }
}

void SendBankMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-1997.7, 5363, 6, 6, 0, "Scyers Bank");     //19246, 19338
            player->SEND_GOSSIP_MENU(10379, _Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1730.5, 5496, 6, 6, 0, "Aldor Bank");      //19318, 19034
            player->SEND_GOSSIP_MENU(10380,  _Creature->GetGUID());
    }
}

void SendInnMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-2178, 5405, 6, 6, 0, "Scyers Inn");        //19232
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1895, 5767, 6, 6, 0, "Aldor Inn");         //19046
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
    }
}

void SendMailboxMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-2028.5, 5347.5, 6, 6, 0, "Scyers Mailbox");//in bank
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1701, 5510.5, 6, 6, 0, "Aldor Mailbox");   //in bank
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
    }
}

void SendStableMasterMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-2170, 5404, 6, 6, 0, "Scyers Stable Master");//21517
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1888.5, 5761, 6, 6, 0, "Aldor Stable Master");//21518
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
    }
}

void SendBattleMasterMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)                            //battlemaster, ally
    {
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY        , GOSSIP_SENDER_SUB1_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN          , GOSSIP_SENDER_SUB1_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH          , GOSSIP_SENDER_SUB1_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_EYEOFTHESTORM        , GOSSIP_SENDER_SUB1_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 4);
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)                            //battlemaster, horde and arena
    {
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY        , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN          , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH          , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_EYEOFTHESTORM        , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARENA                , GOSSIP_SENDER_SUB2_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
    }
}

void SendBattleMasterSub1Menu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->SEND_POI(-1774, 5251, 6, 6, 0, "Alterac Valley Battle Master");//20271
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            player->SEND_POI(-1774, 5251, 6, 6, 0, "Arathi Basin Battle Master");//20273
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            player->SEND_POI(-1774, 5251, 6, 6, 0, "Warsong Gulch Battle Master");//20272
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:
            player->SEND_POI(-1774, 5251, 6, 6, 0, "Eye of The Storm Battle Master");//20362
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;
    }
}

void SendBattleMasterSub2Menu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Alterac Valley Battle Master");//20276
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Arathi Basin Battle Master");//20274
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Warsong Gulch Battle Master");//20269
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Eye of The Storm Battle Master");//20384
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:
            player->SEND_POI(-1965.5, 5262, 6, 6, 0, "Arena Battle Master");//21235
            player->SEND_GOSSIP_MENU(10321,  _Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(-1648.5, 5534, 6, 6, 0, "Shattrath Alchemy Trainer");//19052
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(-1847, 5222, 6, 6, 0, "Shattrath Blacksmithing Trainers");//20125,20124
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(-2069, 5309.5, 6, 6, 0, "Shattrath Cooking Trainer");//19185
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_POI(-2263.5, 5563.5, 6, 6, 0, "Shattrath Enchanting Trainers");//19252,19250,19249,19248
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //First Aid
            player->SEND_POI(-1591, 5265.5, 6, 6, 0, "Shattrath First Aid Trainer");//19184
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Jewelcrafting
            player->SEND_POI(-1654, 5667.5, 6, 6, 0, "Shattrath Jewelcrafting Trainer");//19063
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Leatherworking
            player->SEND_POI(-2060.5, 5256.5, 6, 6, 0, "Shattrath Leatherworking Trainer");//19187
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Skinning
            player->SEND_POI(-2048, 5300, 6, 6, 0, "Shattrath Skinning Trainer");//19180
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
            break;
    }
}

void SendGemMerchantMenu_guard_shattrath(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
            player->SEND_POI(-2193, 5424.5, 6, 6, 0, "Scyers Gem Merchant");//19330
            player->SEND_GOSSIP_MENU(10321, _Creature->GetGUID());
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
            player->SEND_POI(-1645, 5669.5, 6, 6, 0, "Aldor Gem Merchant");//19065
            player->SEND_GOSSIP_MENU(10698,  _Creature->GetGUID());
    }
}

bool GossipSelect_guard_shattrath(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:              SendDefaultMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BANK:          SendBankMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_INN:           SendInnMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_STABLEMASTER:  SendStableMasterMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_GEMMERCHANT:   SendGemMerchantMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_MAILBOX:       SendMailboxMenu_guard_shattrath(player, _Creature, action);    
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:     SendProfTrainerMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:    SendBattleMasterMenu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SUB1_BATTLEINFO:   SendBattleMasterSub1Menu_guard_shattrath(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SUB2_BATTLEINFO:   SendBattleMasterSub2Menu_guard_shattrath(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_shattrath end
 *******************************************************/

CreatureAI* GetAI_guard_shattrath(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_silvermoon start
 *******************************************************/

bool GossipHello_guard_silvermoon(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN            , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_WINDRIDER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_silvermoon(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Auction house
            player->SEND_POI(9672.73, -7528.61, 6, 6, 0, "Silvermoon Auction House");//16628
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Bank
            player->SEND_POI(9808.4, -7488.16, 6, 6, 0, "Silvermoon Bank");//16616
            player->SEND_GOSSIP_MENU(9322, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Guild master
            player->SEND_POI(9474.97, -7345.21, 6, 6, 0, "Silvermoon Guild Master");//16568
            player->SEND_GOSSIP_MENU(9324, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Inn
            player->SEND_POI(9683.82, -7365.19, 6, 6, 0, "Silvermoon Inn");//16618
            player->SEND_GOSSIP_MENU(9325, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Mailbox
            player->SEND_POI(9658.33, -7492.17, 6, 6, 0, "Silvermoon Mailbox");//go 181980
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Stable master
            player->SEND_POI(9904.95, -7404.31, 6, 6, 0, "Shalenn");//16656
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Weapon trainer
            player->SEND_POI(9841.17, -7505.13, 6, 6, 0, "Ileda");//16621
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Wind master
            player->SEND_POI(9378.45, -7163.94, 6, 6, 0, "Silvermoon Wind Master");//16192
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Battlemaster
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARENA           , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_EYEOFTHESTORM   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 4);        
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                 , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_JEWELCRAFTING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING               , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 13);
            player->SEND_GOSSIP_MENU(9338, _Creature->GetGUID());
            break;
    }
}

void SendBattleMasterMenu_guard_silvermoon(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //AV
            player->SEND_POI(9850.49, -7572.26, 6, 6, 0, "Gurak");//16695
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //AB
            player->SEND_POI(9857.18, -7564.36, 6, 6, 0, "Karen Wentworth");//16694
            player->SEND_GOSSIP_MENU(9316,  _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //A
            player->SEND_POI(9850.6, -7559.25, 6, 6, 0, "Bipp Glizzitor");//19923
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //EOS
            player->SEND_POI(9857.18, -7564.36, 6, 6, 0, "Karen Wentworth");//16694
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //WSG
            player->SEND_POI(9845.45, -7562.58, 6, 6, 0, "Krukk");//16696
            player->SEND_GOSSIP_MENU(1, _Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_silvermoon(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Druid
            player->SEND_POI(9700.55, -7262.57, 6, 6, 0, "Silvermoon Druid Trainer");//16655
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Hunter
            player->SEND_POI(9927.48, -7426.14, 6, 6, 0, "Silvermoon Hunter Trainer");//16674
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Mage
            player->SEND_POI(9995.07, -7118.17, 6, 6, 0, "Silvermoon Mage Trainer");//16653
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Paladin
            player->SEND_POI(9850.22, -7516.93, 6, 6, 0, "Silvermoon Paladin Trainer");//16681
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Priest
            player->SEND_POI(9926.79, -7066.66, 6, 6, 0, "Silvermoon Priest Trainer");//16658
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Rogue
            player->SEND_POI(9739.88, -7374.33, 6, 6, 0, "Silvermoon Rogue Trainer");//16684
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Warlock
            player->SEND_POI(9787.57, -7284.63, 6, 6, 0, "Silvermoon Warlock Trainer");//16648
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_silvermoon(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(9998.09, -7214.36, 6, 6, 0, "Silvermoon Alchemy Trainer");//16642 isn't spawned, so use 16641-Melaris
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(9841.43, -7361.53, 6, 6, 0, "Silvermoon Blacksmithing Trainer");//16669
            player->SEND_GOSSIP_MENU(9340, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(9577.26, -7243.6, 6, 6, 0, "Silvermoon Cooking Trainer");//16676
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_POI(9962.57, -7246.18, 6, 6, 0, "Silvermoon Enchanting Trainer");//16633
            player->SEND_GOSSIP_MENU(9341, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_POI(9820.18, -7329.56, 6, 6, 0, "Silvermoon Engineering Trainer");//16667
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(9579.8, -7343.71, 6, 6, 0, "Silvermoon First Aid Trainer");//16662
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(9607.73, -7324.3, 6, 6, 0, "Silvermoon Fishing Trainer");//16780
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Jewelcrafting
            player->SEND_POI(9553.54, -7506.43, 6, 6, 0, "Silvermoon Jewelcrafting Trainer");//16702-Telia isn't spawned, so use 16703-Amin
            player->SEND_GOSSIP_MENU(9346, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Herbalism
            player->SEND_POI(10004.4, -7216.86, 6, 6, 0, "Silvermoon Herbalism Trainer");//16644 and 22023 :)
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Leatherworking
            player->SEND_POI(9503.72, -7430.16, 6, 6, 0, "Silvermoon Leatherworking Trainer");//16688
            player->SEND_GOSSIP_MENU(9347, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Mining
            player->SEND_POI(9805.1, -7355.56, 6, 6, 0, "Silvermoon Mining Trainer");//16663
            player->SEND_GOSSIP_MENU(9348, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Skinning
            player->SEND_POI(9513.37, -7429.4, 6, 6, 0, "Silvermoon Skinning Trainer");//16692
            player->SEND_GOSSIP_MENU(9316, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 13:                   //Tailoring
            player->SEND_POI(9750.55, -7095.28, 6, 6, 0, "Silvermoon Tailor");//16640
            player->SEND_GOSSIP_MENU(9350, _Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_silvermoon(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_silvermoon(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_silvermoon(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_silvermoon(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:  SendBattleMasterMenu_guard_silvermoon(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_silvermoon end
 *******************************************************/

CreatureAI* GetAI_guard_silvermoon(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_stormwind start
 *******************************************************/

bool GossipHello_guard_stormwind(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);  //added
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_STORMWIND_BANK    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_DEEPRUNTRAM          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);  //added
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN               , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_GRYPHON           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);    
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);  //added
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);  //added
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_OFFICERS          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11); //added
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
    player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_stormwind(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Auction House
            player->SEND_POI(-8811.46, 667.46, 6, 6, 0, "Stormwind Auction House");
            player->SEND_GOSSIP_MENU(3834,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Bank
            player->SEND_POI(-8916.87, 622.87, 6, 6, 0, "Stormwind Bank");
            player->SEND_GOSSIP_MENU(764,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Deeprun tram
            player->SEND_POI(-8378.88, 554.23, 6, 6, 0, "The Deeprun Tram");
            player->SEND_GOSSIP_MENU(3813,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Inn
            player->SEND_POI(-8869.0, 675.4, 6, 6, 0, "The Gilded Rose");
            player->SEND_GOSSIP_MENU(3860,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Gryphon Master
            player->SEND_POI(-8837.0, 493.5, 6, 6, 0, "Stormwind Gryphon Master");
            player->SEND_GOSSIP_MENU(879,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Guild Master
            player->SEND_POI(-8894.0, 611.2, 6, 6, 0, "Stormwind Vistor`s Center");
            player->SEND_GOSSIP_MENU(882,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Mailbox
            player->SEND_POI(-8876.48, 649.18, 6, 6, 0, "Stormwind Mailbox");
            player->SEND_GOSSIP_MENU(3861,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Stable Master
            player->SEND_POI(-8433.0, 554.7, 6, 6, 0, "Jenova Stoneshield");
            player->SEND_GOSSIP_MENU(5984,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Weapon Trainer
            player->SEND_POI(-8797.0, 612.8, 6, 6, 0, "Woo Ping");
            player->SEND_GOSSIP_MENU(4516,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Officers Lounge
            player->SEND_POI(-8759.92, 399.69, 6, 6, 0, "Champions` Hall");
            player->SEND_GOSSIP_MENU(7047,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Battlemasters
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY        , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN          , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH          , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(7499,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Class trainers
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                 , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PALADIN              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SHAMAN               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->SEND_GOSSIP_MENU(4264,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 13:                   //Profession trainers
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING               , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(4273,_Creature->GetGUID());
            break;
    }
}
void SendBattleMasterMenu_guard_stormwind(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //AV
            player->SEND_POI(-8443.88, 335.99, 6, 6, 0, "Thelman Slatefist");
            player->SEND_GOSSIP_MENU(7500, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //AB
            player->SEND_POI(-8443.88, 335.99, 6, 6, 0, "Lady Hoteshem");
            player->SEND_GOSSIP_MENU(7650, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //WSG
            player->SEND_POI(-8443.88, 335.99, 6, 6, 0, "Elfarran");
            player->SEND_GOSSIP_MENU(7501, _Creature->GetGUID());
            break;
    }
}
void SendClassTrainerMenu_guard_stormwind(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Mage
            player->SEND_POI(-9012.0, 867.6, 6, 6, 0, "Wizard`s Sanctum");
            player->SEND_GOSSIP_MENU(899,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Rogue
            player->SEND_POI(-8753.0, 367.8, 6, 6, 0, "Stormwind - Rogue House");
            player->SEND_GOSSIP_MENU(900,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Warrior
            player->SEND_POI(-8624.54, 402.61, 6, 6, 0, "Pig and Whistle Tavern");
            player->SEND_GOSSIP_MENU(901,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Druid
            player->SEND_POI(-8751.0, 1124.5, 6, 6, 0, "The Park");
            player->SEND_GOSSIP_MENU(902,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Priest
            player->SEND_POI(-8512.0, 862.4, 6, 6, 0, "Catedral Of Light");
            player->SEND_GOSSIP_MENU(903,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Paladin
            player->SEND_POI(-8577.0, 881.7, 6, 6, 0, "Catedral Of Light");
            player->SEND_GOSSIP_MENU(904,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Hunter
            player->SEND_POI(-8413.0, 541.5, 6, 6, 0, "Hunter Lodge");
            player->SEND_GOSSIP_MENU(905,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Warlock
            player->SEND_POI(-8948.91, 998.35, 6, 6, 0, "The Slaughtered Lamb");
            player->SEND_GOSSIP_MENU(906,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Shaman
            player->SEND_POI(-9033, 550, 6, 6, 0, "Valley Of Heroes");
            player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());//incorrect id
            break;
    }
}

void SendProfTrainerMenu_guard_stormwind(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(-8988.0, 759.60, 6, 6, 0, "Alchemy Needs");
            player->SEND_GOSSIP_MENU(919,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(-8424.0, 616.9, 6, 6, 0, "Therum Deepforge");
            player->SEND_GOSSIP_MENU(920,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(-8611.0, 364.6, 6, 6, 0, "Pig and Whistle Tavern");
            player->SEND_GOSSIP_MENU(921,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_POI(-8858.0, 803.7, 6, 6, 0, "Lucan Cordell");
            player->SEND_GOSSIP_MENU(941,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_POI(-8347.0, 644.1, 6, 6, 0, "Lilliam Sparkspindle");
            player->SEND_GOSSIP_MENU(922,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(-8513.0, 801.8, 6, 6, 0, "Shaina Fuller");
            player->SEND_GOSSIP_MENU(923,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(-8803.0, 767.5, 6, 6, 0, "Arnold Leland");
            player->SEND_GOSSIP_MENU(940,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Herbalism
            player->SEND_POI(-8967.0, 779.5, 6, 6, 0, "Alchemy Needs");
            player->SEND_GOSSIP_MENU(924,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Leatherworking
            player->SEND_POI(-8726.0, 477.4, 6, 6, 0, "The Protective Hide");
            player->SEND_GOSSIP_MENU(925,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                    //Mining
            player->SEND_POI(-8434.0, 692.8, 6, 6, 0, "Gelman Stonehand");
            player->SEND_GOSSIP_MENU(927,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                    //Skinning
            player->SEND_POI(-8716.0, 469.4, 6, 6, 0, "The Protective Hide");
            player->SEND_GOSSIP_MENU(928,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                    //Tailoring
            player->SEND_POI(-8938.0, 800.7, 6, 6, 0, "Duncan`s Textiles");
            player->SEND_GOSSIP_MENU(929,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_stormwind(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_stormwind(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_stormwind(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_stormwind(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:  SendBattleMasterMenu_guard_stormwind(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_stormwind end
 *******************************************************/

CreatureAI* GetAI_guard_stormwind(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_teldrassil start
 *******************************************************/

bool GossipHello_guard_teldrassil(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK         , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_FERRY        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->SEND_GOSSIP_MENU(4316,_Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_teldrassil(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Bank
            player->SEND_GOSSIP_MENU(4317,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Rut`theran
            player->SEND_GOSSIP_MENU(4318,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Guild master
            player->SEND_GOSSIP_MENU(4319,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Inn
            player->SEND_POI(9821.49, 960.13, 6, 6, 0, "Dolanaar Inn");
            player->SEND_GOSSIP_MENU(4320,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //stable master
            player->SEND_POI(9808.37, 931.1, 6, 6, 0, "Seriadne");
            player->SEND_GOSSIP_MENU(5982,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_DRUID                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HUNTER               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(4264,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->SEND_GOSSIP_MENU(4273,_Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_teldrassil(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Druid
            player->SEND_POI(9741.58, 963.7, 6, 6, 0, "Kal");
            player->SEND_GOSSIP_MENU(4323,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Hunter
            player->SEND_POI(9815.12, 926.28, 6, 6, 0, "Dazalar");
            player->SEND_GOSSIP_MENU(4324,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Priest
            player->SEND_POI(9906.16, 986.63, 6, 6, 0, "Laurna Morninglight");
            player->SEND_GOSSIP_MENU(4325,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Rogue
            player->SEND_POI(9789, 942.86, 6, 6, 0, "Jannok Breezesong");
            player->SEND_GOSSIP_MENU(4326,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Warrior
            player->SEND_POI(9821.96, 950.61, 6, 6, 0, "Kyra Windblade");
            player->SEND_GOSSIP_MENU(4327,_Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_teldrassil(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(9767.59, 878.81, 6, 6, 0, "Cyndra Kindwhisper");
            player->SEND_GOSSIP_MENU(4329,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Cooking
            player->SEND_POI(9751.19, 906.13, 6, 6, 0, "Zarrin");
            player->SEND_GOSSIP_MENU(4330,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Enchanting
            player->SEND_POI(10677.59, 1946.56, 6, 6, 0, "Alanna Raveneye");
            player->SEND_GOSSIP_MENU(4331,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //First Aid
            player->SEND_POI(9903.12, 999, 6, 6, 0, "Byancie");
            player->SEND_GOSSIP_MENU(4332,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Fishing
            player->SEND_GOSSIP_MENU(4333,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Herbalism
            player->SEND_POI(9773.78, 875.88, 6, 6, 0, "Malorne Bladeleaf");
            player->SEND_GOSSIP_MENU(4334,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Leatherworking
            player->SEND_POI(10152.59, 1681.46, 6, 6, 0, "Nadyia Maneweaver");
            player->SEND_GOSSIP_MENU(4335,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Skinning
            player->SEND_POI(10135.59, 1673.18, 6, 6, 0, "Radnaal Maneweaver");
            player->SEND_GOSSIP_MENU(4336,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Tailoring
            player->SEND_GOSSIP_MENU(4337,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_teldrassil(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_teldrassil(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_teldrassil(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_teldrassil(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_teldrassil end
 *******************************************************/

CreatureAI* GetAI_guard_teldrassil(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_tirisfal start
 *******************************************************/

bool GossipHello_guard_tirisfal(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK            , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_BATHANDLER      , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN             , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->SEND_GOSSIP_MENU(2593,_Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_tirisfal(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Bank
            player->SEND_GOSSIP_MENU(4074,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //bat handler
            player->SEND_GOSSIP_MENU(4075,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Inn
            player->SEND_POI(2246.68, 241.89, 6, 6, 0, "Gallows` End Tavern");
            player->SEND_GOSSIP_MENU(4076,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Stable Master
            player->SEND_POI(2267.66, 319.32, 6, 6, 0, "Morganus");
            player->SEND_GOSSIP_MENU(5978,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE                 , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST               , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE                , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR              , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(4292,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING           , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING              , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING               , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING             , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING            , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(4300,_Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_tirisfal(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Mage
            player->SEND_POI(2259.18, 240.93, 6, 6, 0, "Cain Firesong");
            player->SEND_GOSSIP_MENU(4077,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Priest
            player->SEND_POI(2259.18, 240.93, 6, 6, 0, "Dark Cleric Beryl");
            player->SEND_GOSSIP_MENU(4078,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Rogue
            player->SEND_POI(2259.18, 240.93, 6, 6, 0, "Marion Call");
            player->SEND_GOSSIP_MENU(4079,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Warlock
            player->SEND_POI(2259.18, 240.93, 6, 6, 0, "Rupert Boch");
            player->SEND_GOSSIP_MENU(4080,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Warrior
            player->SEND_POI(2256.48, 240.32, 6, 6, 0, "Austil de Mon");
            player->SEND_GOSSIP_MENU(4081,_Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_tirisfal(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(2263.25, 344.23, 6, 6, 0, "Carolai Anise");
            player->SEND_GOSSIP_MENU(4082,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_GOSSIP_MENU(4083,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_GOSSIP_MENU(4084,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_POI(2250.35, 249.12, 6, 6, 0, "Vance Undergloom");
            player->SEND_GOSSIP_MENU(4085,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_GOSSIP_MENU(4086,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(2246.68, 241.89, 6, 6, 0, "Nurse Neela");
            player->SEND_GOSSIP_MENU(4087,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(2292.37, -10.72, 6, 6, 0, "Clyde Kellen");
            player->SEND_GOSSIP_MENU(4088,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Herbalism
            player->SEND_POI(2268.21, 331.69, 6, 6, 0, "Faruza");
            player->SEND_GOSSIP_MENU(4089,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Leatherworking
            player->SEND_POI(2027, 78.72, 6, 6, 0, "Shelene Rhobart");
            player->SEND_GOSSIP_MENU(4090,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Mining
            player->SEND_GOSSIP_MENU(4091,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Skinning
            player->SEND_POI(2027, 78.72, 6, 6, 0, "Rand Rhobart");
            player->SEND_GOSSIP_MENU(4092,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Tailoring
            player->SEND_POI(2160.45, 659.93, 6, 6, 0, "Bowen Brisboise");
            player->SEND_GOSSIP_MENU(4093,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_tirisfal(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_tirisfal(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_tirisfal(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_tirisfal(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_tirisfal end
 *******************************************************/

CreatureAI* GetAI_guard_tirisfal(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * guard_undercity start
 *******************************************************/

bool GossipHello_guard_undercity(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 6, GOSSIP_TEXT_BANK          , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_BATHANDLER    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_GUILDMASTER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_INN           , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM( 4, GOSSIP_TEXT_MAILBOX       , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM( 1, GOSSIP_TEXT_AUCTIONHOUSE  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM( 2, GOSSIP_TEXT_ZEPPLINMASTER , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_TEXT_WEAPONMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM( 5, GOSSIP_TEXT_STABLEMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_BATTLEMASTER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_CLASSTRAINER  , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PROFTRAINER   , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
    player->SEND_GOSSIP_MENU(3543,_Creature->GetGUID());

    return true;
}

void SendDefaultMenu_guard_undercity(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Bank
            player->SEND_POI(1595.64, 232.45, 6, 6, 0, "Undercity Bank");
            player->SEND_GOSSIP_MENU(3514,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Bat handler
            player->SEND_POI(1565.9, 271.43, 6, 6, 0, "Undercity Bat Handler");
            player->SEND_GOSSIP_MENU(3515,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Guild master
            player->SEND_POI(1594.17, 205.57, 6, 6, 0, "Undercity Guild Master");
            player->SEND_GOSSIP_MENU(3516,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Inn
            player->SEND_POI(1639.43, 220.99, 6, 6, 0, "Undercity Inn");
            player->SEND_GOSSIP_MENU(3517,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Mailbox
            player->SEND_POI(1632.68, 219.4, 6, 6, 0, "Undercity Mailbox");
            player->SEND_GOSSIP_MENU(3518,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //Auction House
            player->SEND_POI(1647.9, 258.49, 6, 6, 0, "Undercity Auction House");
            player->SEND_GOSSIP_MENU(3519,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Zeppelin
            player->SEND_POI(2059, 274.86, 6, 6, 0, "Undercity Zeppelin");
            player->SEND_GOSSIP_MENU(3520,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Weapon Master
            player->SEND_POI(1670.31, 324.66, 6, 6, 0, "Archibald");
            player->SEND_GOSSIP_MENU(4521,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Stable master
            player->SEND_POI(1634.18, 226.76, 6, 6, 0, "Anya Maulray");
            player->SEND_GOSSIP_MENU(5979,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Battlemaster
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ALTERACVALLEY   , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_ARATHIBASIN     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 8, GOSSIP_TEXT_WARSONGULCH     , GOSSIP_SENDER_SEC_BATTLEINFO, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(7527,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Class trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MAGE            , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_PRIEST          , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ROGUE           , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARLOCK         , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_WARRIOR         , GOSSIP_SENDER_SEC_CLASSTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(3542,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Profession trainer
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ALCHEMY         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_BLACKSMITHING   , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_COOKING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENCHANTING      , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_ENGINEERING     , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FIRSTAID        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_FISHING         , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_HERBALISM       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_LEATHERWORKING  , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_MINING          , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_SKINNING        , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM( 3, GOSSIP_TEXT_TAILORING       , GOSSIP_SENDER_SEC_PROFTRAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(3541,_Creature->GetGUID());
            break;
    }
}

void SendBattleMasterMenu_guard_undercity(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //AV
            player->SEND_POI(1329, 333.92, 6, 6, 0, "Grizzle Halfmane");
            player->SEND_GOSSIP_MENU(7525,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //AB
            player->SEND_POI(1283.3, 287.16, 6, 6, 0, "Sir Malory Wheeler");
            player->SEND_GOSSIP_MENU(7646,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //WSG
            player->SEND_POI(1265, 351.18, 6, 6, 0, "Kurden Bloodclaw");
            player->SEND_GOSSIP_MENU(7526,_Creature->GetGUID());
            break;
    }
}

void SendClassTrainerMenu_guard_undercity(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Mage
            player->SEND_POI(1781, 53, 6, 6, 0, "Undercity Mage Trainers");
            player->SEND_GOSSIP_MENU(3513,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Priest
            player->SEND_POI(1758.33, 401.5, 6, 6, 0, "Undercity Priest Trainers");
            player->SEND_GOSSIP_MENU(3521,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Rogue
            player->SEND_POI(1418.56, 65, 6, 6, 0, "Undercity Rogue Trainers");
            player->SEND_GOSSIP_MENU(3524,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Warlock
            player->SEND_POI(1780.92, 53.16, 6, 6, 0, "Undercity Warlock Trainers");
            player->SEND_GOSSIP_MENU(3526,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Warrior
            player->SEND_POI(1775.59, 418.19, 6, 6, 0, "Undercity Warrior Trainers");
            player->SEND_GOSSIP_MENU(3527,_Creature->GetGUID());
            break;
    }
}

void SendProfTrainerMenu_guard_undercity(Player *player, Creature *_Creature, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:                    //Alchemy
            player->SEND_POI(1419.82, 417.19, 6, 6, 0, "The Apothecarium");
            player->SEND_GOSSIP_MENU(3528,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:                    //Blacksmithing
            player->SEND_POI(1696, 285, 6, 6, 0, "Undercity Blacksmithing Trainer");
            player->SEND_GOSSIP_MENU(3529,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:                    //Cooking
            player->SEND_POI(1596.34, 274.68, 6, 6, 0, "Undercity Cooking Trainer");
            player->SEND_GOSSIP_MENU(3530,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:                    //Enchanting
            player->SEND_POI(1488.54, 280.19, 6, 6, 0, "Undercity Enchanting Trainer");
            player->SEND_GOSSIP_MENU(3531,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:                    //Engineering
            player->SEND_POI(1408.58, 143.43, 6, 6, 0, "Undercity Engineering Trainer");
            player->SEND_GOSSIP_MENU(3532,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:                    //First Aid
            player->SEND_POI(1519.65, 167.19, 6, 6, 0, "Undercity First Aid Trainer");
            player->SEND_GOSSIP_MENU(3533,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:                    //Fishing
            player->SEND_POI(1679.9, 89, 6, 6, 0, "Undercity Fishing Trainer");
            player->SEND_GOSSIP_MENU(3534,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:                    //Herbalism
            player->SEND_POI(1558, 349.36, 6, 6, 0, "Undercity Herbalism Trainer");
            player->SEND_GOSSIP_MENU(3535,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:                    //Leatherworking
            player->SEND_POI(1498.76, 196.43, 6, 6, 0, "Undercity Leatherworking Trainer");
            player->SEND_GOSSIP_MENU(3536,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:                   //Mining
            player->SEND_POI(1642.88, 335.58, 6, 6, 0, "Undercity Mining Trainer");
            player->SEND_GOSSIP_MENU(3537,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:                   //Skinning
            player->SEND_POI(1498.6, 196.46, 6, 6, 0, "Undercity Skinning Trainer");
            player->SEND_GOSSIP_MENU(3538,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:                   //Tailoring
            player->SEND_POI(1689.55, 193, 6, 6, 0, "Undercity Tailoring Trainer");
            player->SEND_GOSSIP_MENU(3539,_Creature->GetGUID());
            break;
    }
}

bool GossipSelect_guard_undercity(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (sender)
    {
        case GOSSIP_SENDER_MAIN:            SendDefaultMenu_guard_undercity(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_CLASSTRAIN:  SendClassTrainerMenu_guard_undercity(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_PROFTRAIN:   SendProfTrainerMenu_guard_undercity(player, _Creature, action);
            break;
        case GOSSIP_SENDER_SEC_BATTLEINFO:  SendBattleMasterMenu_guard_undercity(player, _Creature, action);
            break;
    }
    return true;
}

/*******************************************************
 * guard_undercity end
 *******************************************************/

CreatureAI* GetAI_guard_undercity(Creature *_Creature)
{
    return new guardAI (_Creature);
}

/*******************************************************
 * AddSC
 *******************************************************/

void AddSC_guards()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="guard_bluffwatcher";
    newscript->pGossipHello          = &GossipHello_guard_bluffwatcher;
    newscript->pGossipSelect         = &GossipSelect_guard_bluffwatcher;
    newscript->GetAI = GetAI_guard_bluffwatcher;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_contested";
    newscript->GetAI = GetAI_guard_contested;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_darnassus";
    newscript->pGossipHello          = &GossipHello_guard_darnassus;
    newscript->pGossipSelect         = &GossipSelect_guard_darnassus;
    newscript->GetAI = GetAI_guard_darnassus;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_dunmorogh";
    newscript->pGossipHello          = &GossipHello_guard_dunmorogh;
    newscript->pGossipSelect         = &GossipSelect_guard_dunmorogh;
    newscript->GetAI = GetAI_guard_dunmorogh;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_durotar";
    newscript->pGossipHello          = &GossipHello_guard_durotar;
    newscript->pGossipSelect         = &GossipSelect_guard_durotar;
    newscript->GetAI = GetAI_guard_durotar;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_elwynnforest";
    newscript->pGossipHello          = &GossipHello_guard_elwynnforest;
    newscript->pGossipSelect         = &GossipSelect_guard_elwynnforest;
    newscript->GetAI = GetAI_guard_elwynnforest;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_exodar";
    newscript->pGossipHello          = &GossipHello_guard_exodar;
    newscript->pGossipSelect         = &GossipSelect_guard_exodar;
    newscript->GetAI = GetAI_guard_exodar;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_ironforge";
    newscript->pGossipHello          = &GossipHello_guard_ironforge;
    newscript->pGossipSelect         = &GossipSelect_guard_ironforge;
    newscript->GetAI = GetAI_guard_ironforge;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_mulgore";
    newscript->pGossipHello          = &GossipHello_guard_mulgore;
    newscript->pGossipSelect         = &GossipSelect_guard_mulgore;
    newscript->GetAI = GetAI_guard_mulgore;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_orgrimmar";
    newscript->pGossipHello          = &GossipHello_guard_orgrimmar;
    newscript->pGossipSelect         = &GossipSelect_guard_orgrimmar;
    newscript->GetAI = GetAI_guard_orgrimmar;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_shattrath";
    newscript->pGossipHello          = &GossipHello_guard_shattrath;
    newscript->pGossipSelect         = &GossipSelect_guard_shattrath;
    newscript->GetAI = GetAI_guard_shattrath;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_silvermoon";
    newscript->pGossipHello          = &GossipHello_guard_silvermoon;
    newscript->pGossipSelect         = &GossipSelect_guard_silvermoon;
    newscript->GetAI = GetAI_guard_silvermoon;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_stormwind";
    newscript->pGossipHello          = &GossipHello_guard_stormwind;
    newscript->pGossipSelect         = &GossipSelect_guard_stormwind;
    newscript->GetAI = GetAI_guard_stormwind;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_teldrassil";
    newscript->pGossipHello          = &GossipHello_guard_teldrassil;
    newscript->pGossipSelect         = &GossipSelect_guard_teldrassil;
    newscript->GetAI = GetAI_guard_teldrassil;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_tirisfal";
    newscript->pGossipHello          = &GossipHello_guard_tirisfal;
    newscript->pGossipSelect         = &GossipSelect_guard_tirisfal;
    newscript->GetAI = GetAI_guard_tirisfal;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="guard_undercity";
    newscript->pGossipHello          = &GossipHello_guard_undercity;
    newscript->pGossipSelect         = &GossipSelect_guard_undercity;
    newscript->GetAI = GetAI_guard_undercity;
    m_scripts[nrscripts++] = newscript;
}
