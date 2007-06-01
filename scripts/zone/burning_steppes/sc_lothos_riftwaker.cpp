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

#include "../../sc_defines.h"

struct MANGOS_DLL_DECL lothos_riftwakerAI : public ScriptedAI
{
    lothos_riftwakerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    bool InCombat;

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }
};


CreatureAI* GetAI_lothos_riftwaker(Creature *_Creature)
{
    return new lothos_riftwakerAI (_Creature);
}


bool GossipHello_lothos_riftwaker(Player *player, Creature *_Creature)
{
    if (player->GetQuestRewardStatus(7487)==1 || player->GetQuestRewardStatus(7848)==1)  
    {
        player->ADD_GOSSIP_ITEM(2, "Teleport me to the Molten Core", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(384,_Creature->GetGUID());
    }
    else
    {
        uint64 guid = _Creature->GetGUID();

        if (_Creature->isQuestGiver()) {
            player->PrepareQuestMenu( guid );
            player->SendPreparedQuest( guid );
        }
    }
    return true;
}

bool GossipSelect_lothos_riftwaker(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(409, 1096, -467, -104.6, 3.64);

    }

    return true;
}

void AddSC_lothos_riftwaker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="lothos_riftwaker";
    newscript->pGossipHello          = &GossipHello_lothos_riftwaker;
    newscript->pGossipSelect         = &GossipSelect_lothos_riftwaker;
    newscript->GetAI = GetAI_lothos_riftwaker;
    m_scripts[nrscripts++] = newscript;
}