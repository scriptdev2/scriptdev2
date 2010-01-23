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
SDName: Violet_Hold
SD%Complete: 25
SDComment:
SDCategory: Violet Hold
EndScriptData */

/* ContentData
npc_door_seal
npc_sinclari
EndContentData */

#include "precompiled.h"
#include "violet_hold.h"
#include "escort_ai.h"

/*######
## npc_door_seal
######*/

bool EffectDummyCreature_npc_door_seal(Unit* pCaster, uint32 uiSpellId, uint32 uiEffIndex, Creature* pCreatureTarget)
{
    //always check spellid and effectindex
    if (uiSpellId == SPELL_DESTROY_DOOR_SEAL && uiEffIndex == 0)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreatureTarget->GetInstanceData())
            pInstance->SetData(TYPE_SEAL, SPECIAL);

        //always return true when we are handling this spell and effect
        return true;
    }

    return false;
}

/*######
## npc_sinclari
######*/

enum
{
    SAY_BEGIN                   = -1608000,
    SAY_LOCK_DOOR               = -1608001,

    GOSSIP_TEXT_ID_INTRO        = 13853,
    GOSSIP_TEXT_ID_START        = 13854,
};

#define GOSSIP_ITEM_INTRO       "Activate the crystals when we get in trouble, right?"
#define GOSSIP_ITEM_START       "Get your people to safety, we'll keep the Blue Dragonflight's forces at bay."

struct MANGOS_DLL_DECL npc_sinclariAI : public npc_escortAI
{
    npc_sinclariAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    void Reset()
    {
    }

    void WaypointReached(uint32 uiPointId)
    {
    }
};

CreatureAI* GetAI_npc_sinclari(Creature* pCreature)
{
    return new npc_sinclariAI(pCreature);
}

bool GossipHello_npc_sinclari(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INTRO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_INTRO, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_sinclari(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreature->GetInstanceData())
        {
            if (pInstance->GetData(TYPE_MAIN) == NOT_STARTED)
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_START, pCreature->GetGUID());
            }
        }
        else
            pPlayer->CLOSE_GOSSIP_MENU();
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreature->GetInstanceData())
        {
            if (pInstance->GetData(TYPE_MAIN) == NOT_STARTED)
                pInstance->SetData(TYPE_MAIN, IN_PROGRESS);

            pPlayer->CLOSE_GOSSIP_MENU();
        }
        else
            pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

void AddSC_violet_hold()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_door_seal";
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_door_seal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_sinclari";
    newscript->GetAI = &GetAI_npc_sinclari;
    newscript->pGossipHello = &GossipHello_npc_sinclari;
    newscript->pGossipSelect = &GossipSelect_npc_sinclari;
    newscript->RegisterSelf();
}
