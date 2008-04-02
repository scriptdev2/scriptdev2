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
SDName: Npcs_Shadowmoon_Valley
SD%Complete: 100
SDComment: Vendor Drake Dealer Hurlunk. Teleporter TO Invasion Point: Cataclysm
SDCategory: Shadowmoon Valley
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## npc_drake_dealer_hurlunk
######*/

bool GossipHello_npc_drake_dealer_hurlunk(Player *player, Creature *_Creature)
{
    if (_Creature->isVendor() && player->GetReputationRank(1015) == REP_EXALTED)
        player->ADD_GOSSIP_ITEM( 1, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_drake_dealer_hurlunk(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_TRADE)
    {
        player->SEND_VENDORLIST( _Creature->GetGUID() );
    }
    return true;
}

/*######
## npc_invis_legion_teleporter
######*/

struct MANGOS_DLL_DECL npc_invis_legion_teleporterAI : public ScriptedAI
{
    npc_invis_legion_teleporterAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint64 PlayerGuid;
    uint32 TeleTimer;

    void Reset()
    {
        PlayerGuid=0;
        TeleTimer = 5000;
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || who->GetTypeId() != TYPEID_PLAYER)
            return;

        if(who->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance(who)<4)
        {
            if (who->isAlive() || !who->isInCombat())
                PlayerGuid = who->GetGUID();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(TeleTimer < diff)
        {
            if(PlayerGuid)
            {
                Player* player = ((Player*)Unit::GetUnit((*m_creature), PlayerGuid));
                if(m_creature->GetDistance(player)<4)
                {
                    if(player->GetTeam()== ALLIANCE && player->GetQuestRewardStatus(10589))
                        player->CastSpell(player,37387,false);
                    if(player->GetTeam()== HORDE && player->GetQuestRewardStatus(10604))
                        player->CastSpell(player,37389,false);
                    //return? Cannot confirm this same npc should also be the one controlling return
                    /*if(player->GetQuestRewardStatus(10589) || player->GetQuestRewardStatus(10604))
                        player->CastSpell(player,37532,false);*/
                }
                PlayerGuid=0;
            }
            TeleTimer = 5000;
        }else TeleTimer -= diff;
    }
};
CreatureAI* GetAI_npc_invis_legion_teleporter(Creature *_Creature)
{
    return new npc_invis_legion_teleporterAI (_Creature);
}

void AddSC_npcs_shadowmoon_valley()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_drake_dealer_hurlunk";
    newscript->pGossipHello =  &GossipHello_npc_drake_dealer_hurlunk;
    newscript->pGossipSelect = &GossipSelect_npc_drake_dealer_hurlunk;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_invis_legion_teleporter";
    newscript->GetAI = GetAI_npc_invis_legion_teleporter;
    m_scripts[nrscripts++] = newscript;
}
