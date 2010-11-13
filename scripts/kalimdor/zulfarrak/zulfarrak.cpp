/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Zulfarrak
SD%Complete: 50
SDComment: Consider it temporary, no instance script made for this instance yet.
SDCategory: Zul'Farrak
EndScriptData */

/* ContentData
npc_sergeant_bly
npc_weegli_blastfuse
EndContentData */

#include "precompiled.h"
#include "zulfarrak.h"

/*######
## npc_sergeant_bly
######*/

enum
{
    FACTION_HOSTILE   = 14,
    FACTION_FRIENDLY  = 35,

    SPELL_SHIELD_BASH = 11972,
    SPELL_REVENGE     = 12170
};
#define GOSSIP_BLY                  "That's it! I'm tired of helping you out. It's time we settled things on the battlefield!"

//find Bly's gossip menus

struct MANGOS_DLL_DECL npc_sergeant_blyAI : public ScriptedAI
{
    npc_sergeant_blyAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        //m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    //ScriptedInstance* m_pInstance;

    uint32 m_uiShieldBashTimer;
    uint32 m_uiRevengeTimer;                                //this is wrong, spell should never be used unless m_creature->getVictim() dodge, parry or block attack. Mangos support required.

    void Reset()
    {
        m_uiShieldBashTimer = 5000;
        m_uiRevengeTimer    = 8000;

        m_creature->setFaction(FACTION_FRIENDLY);

        /*if (m_pInstance)
            m_pInstance->SetData(0, NOT_STARTED);*/
    }

    void Aggro(Unit* pWho)
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, IN_PROGRESS);*/
    }

    void JustDied(Unit* pVictim)
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, DONE);*/
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShieldBashTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SHIELD_BASH);
            m_uiShieldBashTimer = 15000;
        }
        else
            m_uiShieldBashTimer -= uiDiff;

        if (m_uiRevengeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_REVENGE);
            m_uiRevengeTimer = 10000;
        }
        else
            m_uiRevengeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_sergeant_bly(Creature* pCreature)
{
    return new npc_sergeant_blyAI(pCreature);
}

bool GossipHello_npc_sergeant_bly(Player* pPlayer, Creature* pCreature)
{
    /*if (m_pInstance->GetData(0) == DONE)
    {*/
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_BLY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(1517, pCreature->GetGUID());
    /*}
    else if (m_pInstance->GetData(0) == IN_PROGRESS)
        pPlayer->SEND_GOSSIP_MENU(1516, pCreature->GetGUID());
    else
        pPlayer->SEND_GOSSIP_MENU(1515, pCreature->GetGUID());*/

    return true;
}

bool GossipSelect_npc_sergeant_bly(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->setFaction(FACTION_HOSTILE);
        pCreature->AI()->AttackStart(pPlayer);
    }
    return true;
}

/*######
## npc_weegli_blastfuse
######*/

enum
{
    SPELL_BOMB             = 8858,
    SPELL_GOBLIN_LAND_MINE = 21688,
    SPELL_SHOOT            = 6660,
    SPELL_WEEGLIS_BARREL   = 10772
};

#define GOSSIP_WEEGLI               "[PH] Please blow up the door."

struct MANGOS_DLL_DECL npc_weegli_blastfuseAI : public ScriptedAI
{
    npc_weegli_blastfuseAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        //m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    //ScriptedInstance* m_pInstance;

    void Reset()
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, NOT_STARTED);*/
    }

    void Aggro(Unit* pWho)
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, IN_PROGRESS);*/
    }

    void JustDied(Unit* pVictim)
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, DONE);*/
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_weegli_blastfuse(Creature* pCreature)
{
    return new npc_weegli_blastfuseAI(pCreature);
}

bool GossipHello_npc_weegli_blastfuse(Player* pPlayer, Creature* pCreature)
{
    //event not implemented yet, this is only placeholder for future developement
    /*if (m_pInstance->GetData(0) == DONE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_WEEGLI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(1514, pCreature->GetGUID());//if event can proceed to end
    }
    else if (m_pInstance->GetData(0) == IN_PROGRESS)
        pPlayer->SEND_GOSSIP_MENU(1513, pCreature->GetGUID());//if event are in progress
    else*/
    pPlayer->SEND_GOSSIP_MENU(1511, pCreature->GetGUID());   //if event not started
    return true;
}

bool GossipSelect_npc_weegli_blastfuse(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        //here we make him run to door, set the charge and run away off to nowhere
    }
    return true;
}

bool ProcessEventId_event_go_zulfarrak_gong(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (instance_zulfarrak* pInstance = (instance_zulfarrak*)((Player*)pSource)->GetInstanceData())
        {
            if (pInstance->GetData(TYPE_GAHZRILLA) == NOT_STARTED || pInstance->GetData(TYPE_GAHZRILLA) == FAIL)
            {
                pInstance->SetData(TYPE_GAHZRILLA, IN_PROGRESS);
                return false;                               // Summon Gahz'rilla by Database Script
            }
            else
                return true;                                // Prevent DB script summoning Gahz'rilla
        }
    }
    return false;
}

bool AreaTrigger_at_zulfarrak(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pAt->id == AREATRIGGER_ANTUSUL)
    {
        if (pPlayer->isGameMaster() || pPlayer->isDead())
            return false;

        instance_zulfarrak* pInstance = (instance_zulfarrak*)pPlayer->GetInstanceData();

        if (!pInstance)
            return false;

        if (pInstance->GetData(TYPE_ANTUSUL) == NOT_STARTED || pInstance->GetData(TYPE_ANTUSUL) == FAIL)
        {
            if (Creature* pAntuSul = pInstance->instance->GetCreature(pInstance->GetData64(NPC_ANTUSUL)))
            {
                if (pAntuSul->isAlive())
                    pAntuSul->AI()->AttackStart(pPlayer);
            }
        }
    }

    return false;
}

void AddSC_zulfarrak()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_sergeant_bly";
    pNewScript->GetAI = &GetAI_npc_sergeant_bly;
    pNewScript->pGossipHello =  &GossipHello_npc_sergeant_bly;
    pNewScript->pGossipSelect = &GossipSelect_npc_sergeant_bly;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_weegli_blastfuse";
    pNewScript->GetAI = &GetAI_npc_weegli_blastfuse;
    pNewScript->pGossipHello =  &GossipHello_npc_weegli_blastfuse;
    pNewScript->pGossipSelect = &GossipSelect_npc_weegli_blastfuse;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_go_zulfarrak_gong";
    pNewScript->pProcessEventId = &ProcessEventId_event_go_zulfarrak_gong;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_zulfarrak";
    pNewScript->pAreaTrigger = &AreaTrigger_at_zulfarrak;
    pNewScript->RegisterSelf();
}
