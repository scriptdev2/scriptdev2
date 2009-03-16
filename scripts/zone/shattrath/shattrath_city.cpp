/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Shattrath_City
SD%Complete: 100
SDComment: Quest support: 10004, 10009. Flask vendors, Teleport to Caverns of Time
SDCategory: Shattrath City
EndScriptData */

/* ContentData
npc_ishanah
npc_khadgar
npc_khadgars_servant
npc_raliq_the_drunk
npc_salsalabim
npc_shattrathflaskvendors
npc_zephyr
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*######
## npc_ishanah
######*/

#define GOSSIP_ISHANAH_1    "Who are the Sha'tar?"
#define GOSSIP_ISHANAH_2    "Isn't Shattrath a draenei city? Why do you allow others here?"

bool GossipHello_npc_ishanah(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu(_Creature->GetGUID());

    player->ADD_GOSSIP_ITEM(0, GOSSIP_ISHANAH_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ISHANAH_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_ishanah(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
        player->SEND_GOSSIP_MENU(9458, _Creature->GetGUID());
    else if (action == GOSSIP_ACTION_INFO_DEF+2)
        player->SEND_GOSSIP_MENU(9459, _Creature->GetGUID());

    return true;
}

/*######
## npc_khadgar
######*/

enum
{
    QUEST_CITY_LIGHT        = 10211,
};

#define KHADGAR_GOSSIP_1    "I've heard your name spoken only in whispers, mage. Who are you?"
#define KHADGAR_GOSSIP_2    "Go on, please."
#define KHADGAR_GOSSIP_3    "I see."
#define KHADGAR_GOSSIP_4    "What did you do then?"
#define KHADGAR_GOSSIP_5    "What happened next?"
#define KHADGAR_GOSSIP_7    "There was something else I wanted to ask you."

bool GossipHello_npc_khadgar(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestRewardStatus(QUEST_CITY_LIGHT))
        pPlayer->ADD_GOSSIP_ITEM(0, KHADGAR_GOSSIP_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(9243, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_khadgar(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    switch(action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(0, KHADGAR_GOSSIP_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(9876, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(0, KHADGAR_GOSSIP_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(9877, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(0, KHADGAR_GOSSIP_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->SEND_GOSSIP_MENU(9878, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(0, KHADGAR_GOSSIP_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(9879, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(0, KHADGAR_GOSSIP_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            pPlayer->SEND_GOSSIP_MENU(9880, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->ADD_GOSSIP_ITEM(0, KHADGAR_GOSSIP_7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
            pPlayer->SEND_GOSSIP_MENU(9881, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            pPlayer->ADD_GOSSIP_ITEM(0, KHADGAR_GOSSIP_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(9243, pCreature->GetGUID());
            break;
    }
    return true;
}

/*######
## npc_khadgars_servant
######*/

enum
{
    SAY_KHAD_SERV_0     = -1000234,
    SAY_KHAD_SERV_1     = -1000235,
    SAY_KHAD_SERV_2     = -1000236,
    SAY_KHAD_SERV_3     = -1000237,
    SAY_KHAD_SERV_4     = -1000238,
    SAY_KHAD_SERV_5     = -1000239,
    SAY_KHAD_SERV_6     = -1000240,
    SAY_KHAD_SERV_7     = -1000241,
    SAY_KHAD_SERV_8     = -1000242,
    SAY_KHAD_SERV_9     = -1000243,
    SAY_KHAD_SERV_10    = -1000244,
    SAY_KHAD_SERV_11    = -1000245,
    SAY_KHAD_SERV_12    = -1000246,
    SAY_KHAD_SERV_13    = -1000247,
    SAY_KHAD_SERV_14    = -1000248,
    SAY_KHAD_SERV_15    = -1000249,
    SAY_KHAD_SERV_16    = -1000250,
    SAY_KHAD_SERV_17    = -1000251,
    SAY_KHAD_SERV_18    = -1000252,
    SAY_KHAD_SERV_19    = -1000253,
    SAY_KHAD_SERV_20    = -1000254,
    SAY_KHAD_SERV_21    = -1000255,
};

struct MANGOS_DLL_DECL npc_khadgars_servantAI : public npc_escortAI
{
    npc_khadgars_servantAI(Creature *c) : npc_escortAI(c)
    {
        if (c->GetOwnerGUID() && IS_PLAYER_GUID(c->GetOwnerGUID()))
            pguid = c->GetOwnerGUID();
        else
            error_log("SD2: npc_khadgars_servant can not obtain ownerGUID or ownerGUID is not a player.");

        Reset();
    }

    uint64 pguid;

    void Reset() {}
    void Aggro(Unit* who) {}

    void WaypointReached(uint32 i)
    {
        Unit *pUnit = Unit::GetUnit(*m_creature,PlayerGUID);

        if (!pUnit || pUnit->GetTypeId() != TYPEID_PLAYER)
            return;

        switch(i)
        {
            case 1: DoScriptText(SAY_KHAD_SERV_0, m_creature, pUnit); break;
            case 4: DoScriptText(SAY_KHAD_SERV_1, m_creature, pUnit); break;
            case 6: DoScriptText(SAY_KHAD_SERV_2, m_creature, pUnit); break;
            case 7: DoScriptText(SAY_KHAD_SERV_3, m_creature, pUnit); break;
            case 8: DoScriptText(SAY_KHAD_SERV_4, m_creature, pUnit); break;
            case 17: DoScriptText(SAY_KHAD_SERV_5, m_creature, pUnit); break;
            case 18: DoScriptText(SAY_KHAD_SERV_6, m_creature, pUnit); break;
            case 19: DoScriptText(SAY_KHAD_SERV_7, m_creature, pUnit); break;
            case 33: DoScriptText(SAY_KHAD_SERV_8, m_creature, pUnit); break;
            case 34: DoScriptText(SAY_KHAD_SERV_9, m_creature, pUnit); break;
            case 35: DoScriptText(SAY_KHAD_SERV_10, m_creature, pUnit); break;
            case 36: DoScriptText(SAY_KHAD_SERV_11, m_creature, pUnit); break;
            case 43: DoScriptText(SAY_KHAD_SERV_12, m_creature, pUnit); break;
            case 44: DoScriptText(SAY_KHAD_SERV_13, m_creature, pUnit); break;
            case 49: DoScriptText(SAY_KHAD_SERV_14, m_creature, pUnit); break;
            case 50: DoScriptText(SAY_KHAD_SERV_15, m_creature, pUnit); break;
            case 51: DoScriptText(SAY_KHAD_SERV_16, m_creature, pUnit); break;
            case 52: DoScriptText(SAY_KHAD_SERV_17, m_creature, pUnit); break;
            case 53: DoScriptText(SAY_KHAD_SERV_18, m_creature, pUnit); break;
            case 54: DoScriptText(SAY_KHAD_SERV_19, m_creature, pUnit); break;
            case 55: DoScriptText(SAY_KHAD_SERV_20, m_creature, pUnit); break;
            case 56:
                DoScriptText(SAY_KHAD_SERV_21, m_creature, pUnit);
                ((Player*)pUnit)->AreaExploredOrEventHappens(QUEST_CITY_LIGHT);
                break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!IsBeingEscorted && pguid)
        {
            Start(false, false, false, pguid);
            pguid = 0;
        }

        npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_khadgars_servant(Creature* pCreature)
{
    npc_khadgars_servantAI* servantAI = new npc_khadgars_servantAI(pCreature);

    servantAI->AddWaypoint(0, -1863.369019, 5419.517090, -10.463668, 5000);
    servantAI->AddWaypoint(1, -1861.749023, 5416.465332, -10.508068);
    servantAI->AddWaypoint(2, -1857.036133, 5410.966309, -12.428039);
    servantAI->AddWaypoint(3, -1831.539185, 5365.472168, -12.428039);
    servantAI->AddWaypoint(4, -1813.416504, 5333.776855, -12.428039);
    servantAI->AddWaypoint(5, -1800.354370, 5313.290039, -12.428039);
    servantAI->AddWaypoint(6, -1775.624878, 5268.786133, -38.809181);
    servantAI->AddWaypoint(7, -1770.147339, 5259.268066, -38.829231);
    servantAI->AddWaypoint(8, -1762.814209, 5261.098145, -38.848995);
    servantAI->AddWaypoint(9, -1740.110474, 5268.858398, -40.208965);
    servantAI->AddWaypoint(10, -1725.837402, 5270.936035, -40.208965);
    servantAI->AddWaypoint(11, -1701.580322, 5290.323242, -40.209187);
    servantAI->AddWaypoint(12, -1682.877808, 5291.406738, -34.429646);
    servantAI->AddWaypoint(13, -1670.101685, 5291.201172, -32.786007);
    servantAI->AddWaypoint(14, -1656.666870, 5294.333496, -37.862648);
    servantAI->AddWaypoint(15, -1652.035767, 5295.413086, -40.245499);
    servantAI->AddWaypoint(16, -1620.860596, 5300.133301, -40.208992);
    servantAI->AddWaypoint(17, -1607.630981, 5293.983398, -38.577045, 5000);
    servantAI->AddWaypoint(18, -1607.630981, 5293.983398, -38.577045, 5000);
    servantAI->AddWaypoint(19, -1607.630981, 5293.983398, -38.577045, 5000);
    servantAI->AddWaypoint(20, -1622.140869, 5301.955566, -40.208897);
    servantAI->AddWaypoint(21, -1621.131836, 5333.112793, -40.208897);
    servantAI->AddWaypoint(22, -1637.598999, 5342.134277, -40.208790);
    servantAI->AddWaypoint(23, -1648.521606, 5352.309570, -47.496170);
    servantAI->AddWaypoint(24, -1654.606934, 5357.419434, -45.870892);
    servantAI->AddWaypoint(25, -1633.670044, 5422.067871, -42.835541);
    servantAI->AddWaypoint(25, -1656.567505, 5426.236328, -40.405815);
    servantAI->AddWaypoint(26, -1664.932373, 5425.686523, -38.846405);
    servantAI->AddWaypoint(27, -1681.406006, 5425.871094, -38.810928);
    servantAI->AddWaypoint(28, -1730.875977, 5427.413574, -12.427910);
    servantAI->AddWaypoint(29, -1743.509521, 5369.599121, -12.427910);
    servantAI->AddWaypoint(30, -1877.217041, 5303.710449, -12.427989);
    servantAI->AddWaypoint(31, -1890.371216, 5289.273438, -12.428268);
    servantAI->AddWaypoint(32, -1905.505737, 5266.534668, 2.630672);
    servantAI->AddWaypoint(33, -1909.381348, 5273.008301, 1.663714, 10000);
    servantAI->AddWaypoint(34, -1909.381348, 5273.008301, 1.663714, 12000);
    servantAI->AddWaypoint(35, -1909.381348, 5273.008301, 1.663714, 8000);
    servantAI->AddWaypoint(36, -1909.381348, 5273.008301, 1.663714, 15000);
    servantAI->AddWaypoint(37, -1927.561401, 5275.324707, 1.984987);
    servantAI->AddWaypoint(38, -1927.385498, 5300.879883, -12.427236);
    servantAI->AddWaypoint(39, -1921.063965, 5314.318359, -12.427236);
    servantAI->AddWaypoint(40, -1965.425415, 5379.298828, -12.427236);
    servantAI->AddWaypoint(41, -1981.233154, 5450.743652, -12.427236);
    servantAI->AddWaypoint(42, -1958.022461, 5455.904297, 0.487659);
    servantAI->AddWaypoint(43, -1951.991455, 5463.580566, 0.874490, 10000);
    servantAI->AddWaypoint(44, -1951.991455, 5463.580566, 0.874490, 12000);
    servantAI->AddWaypoint(45, -1968.730225, 5481.752930, -12.427846);
    servantAI->AddWaypoint(46, -1881.839844, 5554.040039, -12.427846);
    servantAI->AddWaypoint(47, -1841.566650, 5545.965332, -12.427846);
    servantAI->AddWaypoint(48, -1837.658325, 5523.780273, 0.558756);
    servantAI->AddWaypoint(49, -1831.321777, 5534.821777, 1.221819, 6000);
    servantAI->AddWaypoint(50, -1831.321777, 5534.821777, 1.221819, 8000);
    servantAI->AddWaypoint(51, -1831.321777, 5534.821777, 1.221819, 5000);
    servantAI->AddWaypoint(52, -1850.060669, 5472.610840, 0.857320, 6000);
    servantAI->AddWaypoint(53, -1850.060669, 5472.610840, 0.857320, 8000);
    servantAI->AddWaypoint(54, -1850.060669, 5472.610840, 0.857320, 9000);
    servantAI->AddWaypoint(55, -1850.060669, 5472.610840, 0.857320, 9000);
    servantAI->AddWaypoint(56, -1850.060669, 5472.610840, 0.857320, 4000);

    return (CreatureAI*)servantAI;
}

/*######
## npc_raliq_the_drunk
######*/

#define GOSSIP_RALIQ            "You owe Sim'salabim money. Hand them over or die!"

#define FACTION_HOSTILE_RD      45
#define FACTION_FRIENDLY_RD     35

#define SPELL_UPPERCUT          10966

struct MANGOS_DLL_DECL npc_raliq_the_drunkAI : public ScriptedAI
{
    npc_raliq_the_drunkAI(Creature* c) : ScriptedAI(c) { Reset(); }

    uint32 Uppercut_Timer;

    void Reset()
    {
        Uppercut_Timer = 5000;
        m_creature->setFaction(FACTION_FRIENDLY_RD);
    }

    void Aggro(Unit *who) {}

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if( Uppercut_Timer < diff )
        {
            DoCast(m_creature->getVictim(),SPELL_UPPERCUT);
            Uppercut_Timer = 15000;
        }else Uppercut_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_npc_raliq_the_drunk(Creature *_Creature)
{
    return new npc_raliq_the_drunkAI (_Creature);
}

bool GossipHello_npc_raliq_the_drunk(Player *player, Creature *_Creature )
{
    if( player->GetQuestStatus(10009) == QUEST_STATUS_INCOMPLETE )
        player->ADD_GOSSIP_ITEM(1, GOSSIP_RALIQ, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(9440, _Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_raliq_the_drunk(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if( action == GOSSIP_ACTION_INFO_DEF+1 )
    {
        player->CLOSE_GOSSIP_MENU();
        _Creature->setFaction(FACTION_HOSTILE_RD);
        ((npc_raliq_the_drunkAI*)_Creature->AI())->AttackStart(player);
    }
    return true;
}

/*######
# npc_salsalabim
######*/

#define FACTION_HOSTILE_SA              90
#define FACTION_FRIENDLY_SA             35
#define QUEST_10004                     10004

#define SPELL_MAGNETIC_PULL             31705

struct MANGOS_DLL_DECL npc_salsalabimAI : public ScriptedAI
{
    npc_salsalabimAI(Creature* c) : ScriptedAI(c) { Reset(); }

    uint32 MagneticPull_Timer;

    void Reset()
    {
        MagneticPull_Timer = 15000;
        m_creature->setFaction(FACTION_FRIENDLY_SA);
    }

    void Aggro(Unit *who) {}

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if( done_by->GetTypeId() == TYPEID_PLAYER )
            if( (m_creature->GetHealth()-damage)*100 / m_creature->GetMaxHealth() < 20 )
        {
            ((Player*)done_by)->GroupEventHappens(QUEST_10004,m_creature);
            damage = 0;
            EnterEvadeMode();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if( MagneticPull_Timer < diff )
        {
            DoCast(m_creature->getVictim(),SPELL_MAGNETIC_PULL);
            MagneticPull_Timer = 15000;
        }else MagneticPull_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_npc_salsalabim(Creature *_Creature)
{
    return new npc_salsalabimAI (_Creature);
}

bool GossipHello_npc_salsalabim(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(QUEST_10004) == QUEST_STATUS_INCOMPLETE )
    {
        _Creature->setFaction(FACTION_HOSTILE_SA);
        ((npc_salsalabimAI*)_Creature->AI())->AttackStart(player);
    }
    else
    {
        if( _Creature->isQuestGiver() )
            player->PrepareQuestMenu( _Creature->GetGUID() );
        player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());
    }
    return true;
}

/*
##################################################
Shattrath City Flask Vendors provides flasks to people exalted with 3 factions:
Haldor the Compulsive
Arcanist Xorith
Both sell special flasks for use in Outlands 25man raids only,
purchasable for one Mark of Illidari each
Purchase requires exalted reputation with Scryers/Aldor, Cenarion Expedition and The Sha'tar
##################################################
*/

bool GossipHello_npc_shattrathflaskvendors(Player *player, Creature *_Creature)
{
    if(_Creature->GetEntry() == 23484)
    {
        // Aldor vendor
        if( _Creature->isVendor() && (player->GetReputationRank(932) == REP_EXALTED) && (player->GetReputationRank(935) == REP_EXALTED) && (player->GetReputationRank(942) == REP_EXALTED) )
        {
            player->ADD_GOSSIP_ITEM(1, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            player->SEND_GOSSIP_MENU(11085, _Creature->GetGUID());
        }
        else
        {
            player->SEND_GOSSIP_MENU(11083, _Creature->GetGUID());
        }
    }

    if(_Creature->GetEntry() == 23483)
    {
        // Scryers vendor
        if( _Creature->isVendor() && (player->GetReputationRank(934) == REP_EXALTED) && (player->GetReputationRank(935) == REP_EXALTED) && (player->GetReputationRank(942) == REP_EXALTED) )
        {
            player->ADD_GOSSIP_ITEM(1, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            player->SEND_GOSSIP_MENU(11085, _Creature->GetGUID());
        }
        else
        {
            player->SEND_GOSSIP_MENU(11084, _Creature->GetGUID());
        }
    }

    return true;
}

bool GossipSelect_npc_shattrathflaskvendors(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if( action == GOSSIP_ACTION_TRADE )
        player->SEND_VENDORLIST( _Creature->GetGUID() );

    return true;
}

/*######
# npc_zephyr
######*/

bool GossipHello_npc_zephyr(Player *player, Creature *_Creature)
{
    if( player->GetReputationRank(989) >= REP_REVERED )
        player->ADD_GOSSIP_ITEM(0, "Take me to the Caverns of Time.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_zephyr(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if( action == GOSSIP_ACTION_INFO_DEF+1 )
        player->CastSpell(player,37778,false);

    return true;
}

void AddSC_shattrath_city()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_ishanah";
    newscript->pGossipHello = &GossipHello_npc_ishanah;
    newscript->pGossipSelect = &GossipSelect_npc_ishanah;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_khadgar";
    newscript->pGossipHello = &GossipHello_npc_khadgar;
    newscript->pGossipSelect = &GossipSelect_npc_khadgar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_khadgars_servant";
    newscript->GetAI = &GetAI_npc_khadgars_servant;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_raliq_the_drunk";
    newscript->pGossipHello = &GossipHello_npc_raliq_the_drunk;
    newscript->pGossipSelect = &GossipSelect_npc_raliq_the_drunk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_salsalabim";
    newscript->GetAI = &GetAI_npc_salsalabim;
    newscript->pGossipHello = &GossipHello_npc_salsalabim;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_shattrathflaskvendors";
    newscript->pGossipHello = &GossipHello_npc_shattrathflaskvendors;
    newscript->pGossipSelect = &GossipSelect_npc_shattrathflaskvendors;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_zephyr";
    newscript->pGossipHello = &GossipHello_npc_zephyr;
    newscript->pGossipSelect = &GossipSelect_npc_zephyr;
    newscript->RegisterSelf();
}
