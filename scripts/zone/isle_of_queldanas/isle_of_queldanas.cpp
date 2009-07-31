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
SDName: Isle_of_Queldanas
SD%Complete: 100
SDComment: Quest support: 11524, 11525, 11532, 11533, 11542, 11543
SDCategory: Isle Of Quel'Danas
EndScriptData */

/* ContentData
npc_ayren_cloudbreaker
npc_converted_sentry
npc_unrestrained_dragonhawk
EndContentData */

#include "precompiled.h"

/*######
## npc_ayren_cloudbreaker
######*/

bool GossipHello_npc_ayren_cloudbreaker(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(11532) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(11533) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Speaking of action, I've been ordered to undertake an air strike.",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);

    if (pPlayer->GetQuestStatus(11542) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(11543) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"I need to intercept the Dawnblade reinforcements.",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_ayren_cloudbreaker(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer,45071,true);               //TaxiPath 779
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer,45113,true);               //TaxiPath 784
    }
    return true;
}

/*######
## npc_converted_sentry
######*/

#define SAY_CONVERTED_1         -1000188
#define SAY_CONVERTED_2         -1000189

#define SPELL_CONVERT_CREDIT    45009

struct MANGOS_DLL_DECL npc_converted_sentryAI : public ScriptedAI
{
    npc_converted_sentryAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool Credit;
    uint32 Timer;

    void Reset()
    {
        Credit = false;
        Timer = 2500;
    }

    void MoveInLineOfSight(Unit *who)
    {
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!Credit)
        {
            if (Timer <= diff)
            {
                uint32 i = urand(1,2);
                if (i==1)
                    DoScriptText(SAY_CONVERTED_1, m_creature);
                else
                    DoScriptText(SAY_CONVERTED_2, m_creature);

                DoCast(m_creature,SPELL_CONVERT_CREDIT);
                ((Pet*)m_creature)->SetDuration(7500);
                Credit = true;
            }else Timer -= diff;
        }
    }
};
CreatureAI* GetAI_npc_converted_sentry(Creature* pCreature)
{
    return new npc_converted_sentryAI(pCreature);
}

/*######
## npc_unrestrained_dragonhawk
######*/

bool GossipHello_npc_unrestrained_dragonhawk(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(11542) == QUEST_STATUS_COMPLETE || pPlayer->GetQuestStatus(11543) == QUEST_STATUS_COMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"<Ride the dragonhawk to Sun's Reach>",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_unrestrained_dragonhawk(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer,45353,true);               //TaxiPath 788
    }
    return true;
}

void AddSC_isle_of_queldanas()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_ayren_cloudbreaker";
    newscript->pGossipHello = &GossipHello_npc_ayren_cloudbreaker;
    newscript->pGossipSelect = &GossipSelect_npc_ayren_cloudbreaker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_converted_sentry";
    newscript->GetAI = &GetAI_npc_converted_sentry;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_unrestrained_dragonhawk";
    newscript->pGossipHello = &GossipHello_npc_unrestrained_dragonhawk;
    newscript->pGossipSelect = &GossipSelect_npc_unrestrained_dragonhawk;
    newscript->RegisterSelf();
}
