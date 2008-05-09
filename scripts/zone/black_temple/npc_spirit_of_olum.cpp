
/* ScriptData
SDName: npc_spirit_of_olum
SD%Complete: 95
SDComment: Teleport player to Seer Kanai after killing Najentus & Supremus, TODO: correct gossip item
SDCategory: Shadowmoon Valley
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"
#include "def_black_temple.h"

/*###
# npc_spirit_of_olum
####*/

#define SPELL_TELEPORT 41566 // s41566 - Teleport to Ashtongue NPC's
#define GOSSIP_OLUM1 "Teleport me to the other Ashtongue Deathsworn"

bool GossipHello_npc_spirit_of_olum(Player* player, Creature* _Creature)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)_Creature->GetInstanceData());

    if(pInstance && (pInstance->GetData(DATA_SUPREMUSEVENT) >= 3) && (pInstance->GetData(DATA_HIGHWARLORDNAJENTUSEVENT) >= 3))
        player->ADD_GOSSIP_ITEM(0, GOSSIP_OLUM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_spirit_of_olum(Player* player, Creature* _Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_INFO_DEF + 1)
        player->CLOSE_GOSSIP_MENU();

    player->InterruptNonMeleeSpells(false);
    player->CastSpell(player, SPELL_TELEPORT, false);
    return true;
}
void AddSC_npc_spirit_of_olum()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_spirit_of_olum";
    newscript->pGossipHello = GossipHello_npc_spirit_of_olum;
    newscript->pGossipSelect = GossipSelect_npc_spirit_of_olum;
    m_scripts[nrscripts++] = newscript;
}
