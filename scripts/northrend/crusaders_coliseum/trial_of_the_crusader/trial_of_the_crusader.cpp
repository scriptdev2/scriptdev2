/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: trial_of_the_crusader
SD%Complete: 0
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum
{
    SAY_TIRION_RAID_INTRO_LONG          = -1649000,
    SAY_RAID_TRIALS_INTRO               = -1649001,

    // Northrend Beasts
    SAY_TIRION_BEAST_1                  = -1649002,
    SAY_VARIAN_BEAST_1                  = -1649003,
    SAY_GARROSH_BEAST_1                 = -1649004,
    SAY_TIRION_BEAST_2                  = -1649005,
    SAY_TIRION_BEAST_3                  = -1649006,
    SAY_TIRION_BEAST_SLAY               = -1649007,
    SAY_TIRION_BEAST_WIPE               = -1649008,

    // Jaraxxus Encounter
    SAY_TIRION_JARAXXUS_INTRO_1         = -1649009,
    SAY_WILFRED_JARAXXUS_INTRO_1        = -1649010,
    SAY_WILFRED_JARAXXUS_INTRO_2        = -1649011,
    SAY_WILFRED_JARAXXUS_INTRO_3        = -1649012,
    SAY_JARAXXUS_JARAXXAS_INTRO_1       = -1649013,
    SAY_WILFRED_DEATH                   = -1649014,
    SAY_TIRION_JARAXXUS_INTRO_2         = -1649015,
    SAY_TIRION_JARAXXUS_EXIT_1          = -1649016,
    SAY_GARROSH_JARAXXUS_EXIT_1         = -1649017,
    SAY_VARIAN_JARAXXUS_SLAY            = -1649018,
    SAY_TIRION_JARAXXUS_EXIT_2          = -1649019,

    // Faction-Champions
    SAY_TIRION_PVP_INTRO_1              = -1649020,
    SAY_GARROSH_PVP_A_INTRO_1           = -1649021,
    SAY_VARIAN_PVP_H_INTRO_1            = -1649022,
    SAY_TIRION_PVP_INTRO_2              = -1649023,
    SAY_VARIAN_PVP_A_INTRO_2            = -1649024,
    SAY_GARROSH_PVP_H_INTRO_2           = -1649025,
    SAY_VARIAN_PVP_A_WIN                = -1649026,
    SAY_GARROSH_PVP_H_WIN               = -1649027,
    SAY_TIRION_PVP_WIN                  = -1649028,

    // Twin Valkyrs
    SAY_TIRION_TWINS_INTRO              = -1649029,
    SAY_RAID_INTRO_SHORT                = -1649030,
    SAY_VARIAN_TWINS_A_WIN              = -1649031,
    SAY_GARROSH_TWINS_H_WIN             = -1649032,
    SAY_TIRION_TWINS_WIN                = -1649033,

    // Anub'Arak Encounter
    SAY_LKING_ANUB_INTRO_1              = -1649034,
    SAY_TIRION_ABUN_INTRO_1             = -1649035,
    SAY_LKING_ANUB_INTRO_2              = -1649036,
    SAY_LKING_ANUB_INTRO_3              = -1649037,
    SAY_ANUB_ANUB_INTRO_1               = -1649038,

    NPC_GORMOK                          = 34796,
    NPC_JARAXXUS                        = 34780,

    GOSSIP_ITEM_START_EVENT1            = -3649000
};

/*######
## npc_barrett_ramsey
######*/

struct MANGOS_DLL_DECL npc_barrett_ramseyAI : public ScriptedAI
{
    npc_barrett_ramseyAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void StartEvent(Player* pPlayer)
    {
        // code starting the event here
    }
};

bool GossipHello_npc_barrett_ramsey(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START_EVENT1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_barrett_ramsey(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        if (npc_barrett_ramseyAI* pBarrettAI = dynamic_cast<npc_barrett_ramseyAI*>(pCreature->AI()))
            pBarrettAI->StartEvent(pPlayer);
    }

    return true;
}

CreatureAI* GetAI_npc_barrett_ramsey(Creature* pCreature)
{
    return new npc_barrett_ramseyAI(pCreature);
}

void AddSC_trial_of_the_crusader()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_barrett_ramsey";
    pNewScript->GetAI = &GetAI_npc_barrett_ramsey;
    pNewScript->pGossipHello = &GossipHello_npc_barrett_ramsey;
    pNewScript->pGossipSelect = &GossipSelect_npc_barrett_ramsey;
    pNewScript->RegisterSelf();
}
