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
SDComment: Quest support: 10004, 10009, 10211, 10231. Flask vendors, Teleport to Caverns of Time
SDCategory: Shattrath City
EndScriptData */

/* ContentData
npc_dirty_larry
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

enum
{
    SAY_START               = -1000274,
    SAY_COUNT               = -1000275,
    SAY_COUNT_1             = -1000276,
    SAY_COUNT_2             = -1000277,
    SAY_ATTACK              = -1000278,
    SAY_GIVEUP              = -1000279,
    QUEST_WHAT_BOOK         = 10231,
    ENTRY_CREEPJACK         = 19726,
    ENTRY_MALONE            = 19725,
};

#define GOSSIP_ITEM_BOOK    "Ezekiel said that you might have a certain book..."

struct MANGOS_DLL_DECL npc_dirty_larryAI : public ScriptedAI
{
    npc_dirty_larryAI(Creature* pC) : ScriptedAI(pC)
    {
        m_uiNpcFlags = pC->GetUInt32Value(UNIT_NPC_FLAGS);
        m_uiCreepjackGUID = 0;
        m_uiMaloneGUID = 0;
        Reset();
    }

    uint32 m_uiNpcFlags;

    uint64 m_uiCreepjackGUID;
    uint64 m_uiMaloneGUID;
    uint64 m_uiPlayerGUID;

    bool bEvent;
    bool bActiveAttack;

    uint32 m_uiSayTimer;
    uint32 m_uiStep;

    void Reset()
    {
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, m_uiNpcFlags);

        m_uiPlayerGUID = 0;
        m_uiCreepjackGUID = 0;
        m_uiMaloneGUID = 0;

        bEvent = false;
        bActiveAttack = false;

        m_uiSayTimer = 1000;
        m_uiStep = 0;

        //expect database to have correct faction (1194) and then only unit flags set/remove needed
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_8);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_9);
    }

    void SetRuffies(uint64 guid, bool bAttack, bool bReset)
    {
        Creature* pCreature = (Creature*)Unit::GetUnit(*m_creature, guid);

        if (!pCreature)
            return;

        if (bReset)
        {
            if (!pCreature->IsInEvadeMode() && pCreature->isAlive())
                pCreature->AI()->EnterEvadeMode();

            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_8);
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_9);
        }
        else
        {
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_8);
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_9);

            if (!pCreature->isAlive())
                return;

            pCreature->SetStandState(UNIT_STAND_STATE_STAND);

            if (bAttack)
            {
                if (Unit* pUnit = Unit::GetUnit(*m_creature, m_uiPlayerGUID))
                {
                    if (pUnit->isAlive())
                        pCreature->AI()->AttackStart(pUnit);
                }
            }
        }
    }

    void StartEvent()
    {
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_8);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_9);

        if (Creature* pCreepjack = GetClosestCreatureWithEntry(m_creature, ENTRY_CREEPJACK, 20.0f))
            m_uiCreepjackGUID = pCreepjack->GetGUID();

        if (Creature* pMalone = GetClosestCreatureWithEntry(m_creature, ENTRY_MALONE, 20.0f))
            m_uiMaloneGUID = pMalone->GetGUID();

        bEvent = true;
    }

    uint32 NextStep(uint32 uiStep)
    {
        Unit* pUnit = Unit::GetUnit(*m_creature, m_uiPlayerGUID);

        if (!pUnit || pUnit->GetTypeId() != TYPEID_PLAYER)
        {
            SetRuffies(m_uiCreepjackGUID,false,true);
            SetRuffies(m_uiMaloneGUID,false,true);
            EnterEvadeMode();
            return 0;
        }

        switch(uiStep)
        {
            case 1:
                DoScriptText(SAY_START, m_creature, pUnit);
                SetRuffies(m_uiCreepjackGUID,false,false);
                SetRuffies(m_uiMaloneGUID,false,false);
                return 3000;
            case 2: DoScriptText(SAY_COUNT, m_creature, pUnit); return 5000;
            case 3: DoScriptText(SAY_COUNT_1, m_creature, pUnit); return 3000;
            case 4: DoScriptText(SAY_COUNT_2, m_creature, pUnit); return 3000;
            case 5: DoScriptText(SAY_ATTACK, m_creature, pUnit); return 3000;
            case 6:
                if (!m_creature->isInCombat() && pUnit->isAlive())
                    AttackStart(pUnit);

                SetRuffies(m_uiCreepjackGUID,true,false);
                SetRuffies(m_uiMaloneGUID,true,false);
                bActiveAttack = true;
                return 2000;
            default: return 0;
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &damage)
    {
        if (damage < m_creature->GetHealth())
            return;

        //damage will kill, this is pretty much the same as 1%HP left
        if (bEvent)
        {
            damage = 0;

            if (Player* pPlayer = (Player*)Unit::GetUnit(*m_creature, m_uiPlayerGUID))
            {
                DoScriptText(SAY_GIVEUP, m_creature, pPlayer);
                pPlayer->GroupEventHappens(QUEST_WHAT_BOOK, m_creature);
            }

            SetRuffies(m_uiCreepjackGUID,false,true);
            SetRuffies(m_uiMaloneGUID,false,true);
            EnterEvadeMode();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (bEvent && !bActiveAttack)
        {
            if (m_uiSayTimer < diff)
                m_uiSayTimer = NextStep(++m_uiStep);
            else
                m_uiSayTimer -= diff;
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

bool GossipHello_npc_dirty_larry(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_WHAT_BOOK) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BOOK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_dirty_larry(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        ((npc_dirty_larryAI*)pCreature->AI())->m_uiPlayerGUID = pPlayer->GetGUID();
        ((npc_dirty_larryAI*)pCreature->AI())->StartEvent();
        pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

CreatureAI* GetAI_npc_dirty_larry(Creature* pCreature)
{
    return new npc_dirty_larryAI(pCreature);
}

/*######
## npc_ishanah
######*/

#define GOSSIP_ISHANAH_1    "Who are the Sha'tar?"
#define GOSSIP_ISHANAH_2    "Isn't Shattrath a draenei city? Why do you allow others here?"

bool GossipHello_npc_ishanah(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ISHANAH_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ISHANAH_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_ishanah(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
        pPlayer->SEND_GOSSIP_MENU(9458, pCreature->GetGUID());
    else if (action == GOSSIP_ACTION_INFO_DEF+2)
        pPlayer->SEND_GOSSIP_MENU(9459, pCreature->GetGUID());

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
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, KHADGAR_GOSSIP_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(9243, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_khadgar(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    switch(action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, KHADGAR_GOSSIP_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(9876, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, KHADGAR_GOSSIP_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(9877, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, KHADGAR_GOSSIP_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->SEND_GOSSIP_MENU(9878, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, KHADGAR_GOSSIP_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(9879, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, KHADGAR_GOSSIP_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            pPlayer->SEND_GOSSIP_MENU(9880, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, KHADGAR_GOSSIP_7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
            pPlayer->SEND_GOSSIP_MENU(9881, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, KHADGAR_GOSSIP_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
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
    npc_khadgars_servantAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        if (pCreature->GetOwnerGUID() && IS_PLAYER_GUID(pCreature->GetOwnerGUID()))
            pguid = pCreature->GetOwnerGUID();
        else
            error_log("SD2: npc_khadgars_servant can not obtain ownerGUID or ownerGUID is not a player.");

        pCreature->SetSpeed(MOVE_WALK,1.3f);
        Reset();
    }

    uint64 pguid;

    void Reset() {}

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

    servantAI->FillPointMovementListForCreature();

    return (CreatureAI*)servantAI;
}

/*######
## npc_raliq_the_drunk
######*/

enum
{
    SPELL_UPPERCUT          = 10966,
    QUEST_CRACK_SKULLS      = 10009,
    FACTION_HOSTILE_RD      = 45
};

#define GOSSIP_RALIQ        "You owe Sim'salabim money. Hand them over or die!"

struct MANGOS_DLL_DECL npc_raliq_the_drunkAI : public ScriptedAI
{
    npc_raliq_the_drunkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiNormFaction = pCreature->getFaction();
        Reset();
    }

    uint32 m_uiNormFaction;
    uint32 m_uiUppercut_Timer;

    void Reset()
    {
        m_uiUppercut_Timer = 5000;

        if (m_creature->getFaction() != m_uiNormFaction)
            m_creature->setFaction(m_uiNormFaction);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_uiUppercut_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_UPPERCUT);
            m_uiUppercut_Timer = 15000;
        }else m_uiUppercut_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_raliq_the_drunk(Creature* pCreature)
{
    return new npc_raliq_the_drunkAI(pCreature);
}

bool GossipHello_npc_raliq_the_drunk(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_CRACK_SKULLS) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_RALIQ, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(9440, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_raliq_the_drunk(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->setFaction(FACTION_HOSTILE_RD);
        ((npc_raliq_the_drunkAI*)pCreature->AI())->AttackStart(pPlayer);
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
    npc_salsalabimAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 MagneticPull_Timer;

    void Reset()
    {
        MagneticPull_Timer = 15000;
        m_creature->setFaction(FACTION_FRIENDLY_SA);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (done_by->GetTypeId() == TYPEID_PLAYER)
            if ((m_creature->GetHealth()-damage)*100 / m_creature->GetMaxHealth() < 20)
        {
            ((Player*)done_by)->GroupEventHappens(QUEST_10004,m_creature);
            damage = 0;
            EnterEvadeMode();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (MagneticPull_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MAGNETIC_PULL);
            MagneticPull_Timer = 15000;
        }else MagneticPull_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_npc_salsalabim(Creature* pCreature)
{
    return new npc_salsalabimAI(pCreature);
}

bool GossipHello_npc_salsalabim(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_10004) == QUEST_STATUS_INCOMPLETE)
    {
        pCreature->setFaction(FACTION_HOSTILE_SA);
        ((npc_salsalabimAI*)pCreature->AI())->AttackStart(pPlayer);
    }
    else
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
        pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
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

bool GossipHello_npc_shattrathflaskvendors(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->GetEntry() == 23484)
    {
        // Aldor vendor
        if (pCreature->isVendor() && (pPlayer->GetReputationRank(932) == REP_EXALTED) && (pPlayer->GetReputationRank(935) == REP_EXALTED) && (pPlayer->GetReputationRank(942) == REP_EXALTED))
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            pPlayer->SEND_GOSSIP_MENU(11085, pCreature->GetGUID());
        }
        else
        {
            pPlayer->SEND_GOSSIP_MENU(11083, pCreature->GetGUID());
        }
    }

    if (pCreature->GetEntry() == 23483)
    {
        // Scryers vendor
        if (pCreature->isVendor() && (pPlayer->GetReputationRank(934) == REP_EXALTED) && (pPlayer->GetReputationRank(935) == REP_EXALTED) && (pPlayer->GetReputationRank(942) == REP_EXALTED))
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            pPlayer->SEND_GOSSIP_MENU(11085, pCreature->GetGUID());
        }
        else
        {
            pPlayer->SEND_GOSSIP_MENU(11084, pCreature->GetGUID());
        }
    }

    return true;
}

bool GossipSelect_npc_shattrathflaskvendors(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetGUID());

    return true;
}

/*######
# npc_zephyr
######*/

bool GossipHello_npc_zephyr(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetReputationRank(989) >= REP_REVERED)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Take me to the Caverns of Time.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_zephyr(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
        pPlayer->CastSpell(pPlayer,37778,false);

    return true;
}

void AddSC_shattrath_city()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_dirty_larry";
    newscript->GetAI = &GetAI_npc_dirty_larry;
    newscript->pGossipHello = &GossipHello_npc_dirty_larry;
    newscript->pGossipSelect = &GossipSelect_npc_dirty_larry;
    newscript->RegisterSelf();

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
    newscript->GetAI = &GetAI_npc_raliq_the_drunk;
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
