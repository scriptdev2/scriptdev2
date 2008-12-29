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
SDName: Blackrock_Depths
SD%Complete: 95
SDComment: Quest support: 4001, 4342, 7604. Vendor Lokhtos Darkbargainer.
SDCategory: Blackrock Depths
EndScriptData */

/* ContentData
mob_phalanx
npc_kharan_mighthammer
npc_lokhtos_darkbargainer
EndContentData */

#include "precompiled.h"
#include "def_blackrock_depths.h"

bool AreaTrigger_at_ring_of_law(Player *player, AreaTriggerEntry *at)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)player->GetInstanceData());
    return false;
}

struct MANGOS_DLL_DECL npc_grimstoneAI : public ScriptedAI
{
    npc_grimstoneAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    void Reset()
    {
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_grimstone(Creature *_Creature)
{
    return new npc_grimstoneAI (_Creature);
}

/*######
## mob_phalanx
######*/

#define SPELL_THUNDERCLAP       15588
#define SPELL_FIREBALLVOLLEY    15285
#define SPELL_MIGHTYBLOW        14099

struct MANGOS_DLL_DECL mob_phalanxAI : public ScriptedAI
{
    mob_phalanxAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 ThunderClap_Timer;
    uint32 FireballVolley_Timer;
    uint32 MightyBlow_Timer;

    void Reset()
    {
        ThunderClap_Timer = 12000;
        FireballVolley_Timer = 0;
        MightyBlow_Timer = 15000;
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //ThunderClap_Timer
        if (ThunderClap_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_THUNDERCLAP);
            ThunderClap_Timer = 10000;
        }else ThunderClap_Timer -= diff;

        //FireballVolley_Timer
        if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 51)
        {
            if (FireballVolley_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_FIREBALLVOLLEY);
                FireballVolley_Timer = 15000;
            }else FireballVolley_Timer -= diff;
        }

        //MightyBlow_Timer
        if (MightyBlow_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MIGHTYBLOW);
            MightyBlow_Timer = 10000;
        }else MightyBlow_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_phalanx(Creature *_Creature)
{
    return new mob_phalanxAI (_Creature);
}

/*######
## npc_kharan_mighthammer
######*/

#define QUEST_4001      4001
#define QUEST_4342      4342

#define GOSSIP_ITEM_KHARAN_1    "I need to know where the princess are, Kharan!"
#define GOSSIP_ITEM_KHARAN_2    "All is not lost, Kharan!"

#define GOSSIP_ITEM_KHARAN_3    "Gor'shak is my friend, you can trust me."
#define GOSSIP_ITEM_KHARAN_4    "Not enough, you need to tell me more."
#define GOSSIP_ITEM_KHARAN_5    "So what happened?"
#define GOSSIP_ITEM_KHARAN_6    "Continue..."
#define GOSSIP_ITEM_KHARAN_7    "So you suspect that someone on the inside was involved? That they were tipped off?"
#define GOSSIP_ITEM_KHARAN_8    "Continue with your story please."
#define GOSSIP_ITEM_KHARAN_9    "Indeed."
#define GOSSIP_ITEM_KHARAN_10   "The door is open, Kharan. You are a free man."

bool GossipHello_npc_kharan_mighthammer(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu(_Creature->GetGUID());

    if (player->GetQuestStatus(QUEST_4001) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_KHARAN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    if (player->GetQuestStatus(4342) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_KHARAN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

    if (player->GetTeam() == HORDE)
        player->SEND_GOSSIP_MENU(2473, _Creature->GetGUID());
    else
        player->SEND_GOSSIP_MENU(2474, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_kharan_mighthammer(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(2475, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->SEND_GOSSIP_MENU(2476, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->SEND_GOSSIP_MENU(2477, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU(2478, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            player->SEND_GOSSIP_MENU(2479, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_8, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
            player->SEND_GOSSIP_MENU(2480, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_9, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+8);
            player->SEND_GOSSIP_MENU(2481, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+8:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_10, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+9);
            player->SEND_GOSSIP_MENU(2482, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+9:
            player->CLOSE_GOSSIP_MENU();
            if (player->GetTeam() == HORDE)
                player->AreaExploredOrEventHappens(QUEST_4001);
            else
                player->AreaExploredOrEventHappens(QUEST_4342);
            break;
    }
    return true;
}

/*######
## npc_lokhtos_darkbargainer
######*/

#define ITEM_THRORIUM_BROTHERHOOD_CONTRACT               18628
#define ITEM_SULFURON_INGOT                              17203
#define QUEST_A_BINDING_CONTRACT                         7604
#define SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT_DND    23059

#define GOSSIP_ITEM_SHOW_ACCESS     "Show me what I have access to, Lothos."
#define GOSSIP_ITEM_GET_CONTRACT    "Get Thorium Brotherhood Contract"

bool GossipHello_npc_lokhtos_darkbargainer(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu(_Creature->GetGUID());

    if (_Creature->isVendor() && player->GetReputationRank(59) >= REP_FRIENDLY)
        player->ADD_GOSSIP_ITEM( 1, GOSSIP_ITEM_SHOW_ACCESS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    if (player->GetQuestRewardStatus(QUEST_A_BINDING_CONTRACT) != 1 &&
        !player->HasItemCount(ITEM_THRORIUM_BROTHERHOOD_CONTRACT, 1, true) &&
        player->HasItemCount(ITEM_SULFURON_INGOT, 1))
    {
        player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_GET_CONTRACT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    }

    if (player->GetReputationRank(59) < REP_FRIENDLY)
        player->SEND_GOSSIP_MENU(3673, _Creature->GetGUID());
    else
        player->SEND_GOSSIP_MENU(3677, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_lokhtos_darkbargainer(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
        player->CastSpell(player, SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT_DND, false);
    }

    if (action == GOSSIP_ACTION_TRADE)
        player->SEND_VENDORLIST(_Creature->GetGUID());

    return true;
}

/*######
##
######*/

void AddSC_blackrock_depths()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "at_ring_of_law";
    newscript->pAreaTrigger = &AreaTrigger_at_ring_of_law;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_grimstone";
    newscript->GetAI = &GetAI_npc_grimstone;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_phalanx";
    newscript->GetAI = &GetAI_mob_phalanx;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_kharan_mighthammer";
    newscript->pGossipHello =  &GossipHello_npc_kharan_mighthammer;
    newscript->pGossipSelect = &GossipSelect_npc_kharan_mighthammer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_lokhtos_darkbargainer";
    newscript->pGossipHello =  &GossipHello_npc_lokhtos_darkbargainer;
    newscript->pGossipSelect = &GossipSelect_npc_lokhtos_darkbargainer;
    newscript->RegisterSelf();
}
