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
SDName: culling_of_stratholme
SD%Complete: 5%
SDComment: Placeholder
SDCategory: Culling of Stratholme
EndScriptData */

#include "precompiled.h"
#include "culling_of_stratholme.h"

/* *************
** npc_chromie (gossip, quest-accept)
************* */

enum
{
    QUEST_DISPELLING_ILLUSIONS = 13149,
    QUEST_A_ROYAL_ESCORT       = 13151,

    ITEM_ARCANE_DISRUPTOR      = 37888,

    GOSSIP_MENU_INN_1          = 12939,
    GOSSIP_MENU_INN_2          = 12949,
    GOSSIP_MENU_INN_3          = 12950,
    GOSSIP_MENU_INN_4          = 12952,

    GOSSIP_MENU_ENTRANCE_1     = 12992,
    GOSSIP_MENU_ENTRANCE_2     = 12993,
    GOSSIP_MENU_ENTRANCE_3     = 12994,
    GOSSIP_MENU_ENTRANCE_4     = 12995,
};

const std::string GOSSIP_ITEM_INN_1      = "Why have I been sent back to this particular place and time?";
const std::string GOSSIP_ITEM_INN_2      = "What was this decision?";
const std::string GOSSIP_ITEM_INN_3      = "So how does the Infinite Dragonflight plan to interfere?";

const std::string GOSSIP_ITEM_ENTRANCE_1 = "What do you think they're up to?";
const std::string GOSSIP_ITEM_ENTRANCE_2 = "You want me to do what?";
const std::string GOSSIP_ITEM_ENTRANCE_3 = "Very well, Chromie";


bool GossipHello_npc_chromie(Player *pPlayer, Creature *pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
    {
        switch (pCreature->GetEntry())
        {
            case NPC_CHROMIE_INN:
                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) != DONE)
                {
                    if (pPlayer->GetQuestRewardStatus(QUEST_DISPELLING_ILLUSIONS) && !pPlayer->HasItemCount(ITEM_ARCANE_DISRUPTOR, 1))
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                }
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_INN_1, pCreature->GetGUID());
                break;
            case NPC_CHROMIE_ENTRANCE:
                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) == DONE && m_pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED && pPlayer->GetQuestRewardStatus(QUEST_A_ROYAL_ESCORT))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ENTRANCE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ENTRANCE_1, pCreature->GetGUID());
                break;
        }
    }
       return true;
}

bool GossipSelect_npc_chromie(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 uiAction)
{
    switch (pCreature->GetEntry())
    {
        case NPC_CHROMIE_INN:
            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF+1:
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_INN_2, pCreature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF+2:
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INN_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_INN_3, pCreature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF+3:
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_INN_4, pCreature->GetGUID());
                    if (!pPlayer->HasItemCount(ITEM_ARCANE_DISRUPTOR, 1))
                    {
                        if (Item* pItem = pPlayer->StoreNewItemInInventorySlot(ITEM_ARCANE_DISRUPTOR, 1))
                        {
                            pPlayer->SendNewItem(pItem, 1, true, false);
                            if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
                                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) == NOT_STARTED)
                                    m_pInstance->SetData(TYPE_GRAIN_EVENT, SPECIAL);
                        }
                    }
                    break;
            }
            break;
        case NPC_CHROMIE_ENTRANCE:
            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF+1:
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ENTRANCE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ENTRANCE_2, pCreature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF+2:
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ENTRANCE_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ENTRANCE_3, pCreature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF+3:
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ENTRANCE_4, pCreature->GetGUID());
                    if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
                        if (m_pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED)
                            m_pInstance->DoSpawnArthasIfNeeded();
                    break;
            }
            break;
    }
       return true;
}

bool QuestAccept_npc_chromie(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    switch (pQuest->GetQuestId())
    {
        case QUEST_DISPELLING_ILLUSIONS:
            if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) == NOT_STARTED)
                    m_pInstance->SetData(TYPE_GRAIN_EVENT, SPECIAL);
            break;
        case QUEST_A_ROYAL_ESCORT:
            if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
                if (m_pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED)
                    m_pInstance->DoSpawnArthasIfNeeded();
            break;
    }
    return true;
}

/* *************
** npc_crates_bunny (spell aura effect dummy)
************* */

enum
{
    SPELL_ARCANE_DISRUPTION = 49590
};

bool EffectAuraDummy_spell_aura_dummy_npc_crates_dummy(const Aura* pAura, bool bApply)
{
    if (pAura->GetId() == SPELL_ARCANE_DISRUPTION && pAura->GetEffIndex() == EFFECT_INDEX_0 && bApply)
    {
        if (Creature* pTarget = (Creature*)pAura->GetTarget())
        {
            std::list<Creature*> lCrateBunnyList;
            if (instance_culling_of_stratholme* pInstance = (instance_culling_of_stratholme*)pTarget->GetInstanceData())
            {
                pInstance->GetCratesBunnyOrderedList(lCrateBunnyList);
                uint8 i = 0;
                for (std::list<Creature*>::const_iterator itr = lCrateBunnyList.begin(); itr != lCrateBunnyList.end(); ++itr)
                {
                    i++;
                    if (*itr == pTarget)
                        break;
                }

                switch (i)
                {
                    case 1:
                        // Start NPC_ROGER_OWENS Event
                        break;
                    case 2:
                        // Start NPC_SERGEANT_MORIGAN  Event
                        break;
                    case 3:
                        // Start NPC_JENA_ANDERSON Event
                        break;
                    case 4:
                        // Start NPC_MALCOM_MOORE Event
                        break;
                    case 5:
                        // Start NPC_BARTLEBY_BATTSON Event
                        break;
                }

                if (pInstance->GetData(TYPE_GRAIN_EVENT) != DONE)
                    pInstance->SetData(TYPE_GRAIN_EVENT, IN_PROGRESS);
                // pTarget->ForcedDespawn();    // direct despawn has influence on visual effects,
                                                // but despawning makes it impossible to multi-use the spell at the same place
                // perhaps some add. GO-Visual
            }
        }
    }
    return true;
}

void AddSC_culling_of_stratholme()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_chromie";
    newscript->pGossipHello = &GossipHello_npc_chromie;
    newscript->pGossipSelect = &GossipSelect_npc_chromie;
    newscript->pQuestAccept = &QuestAccept_npc_chromie;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_dummy_npc_crates_bunny";
    newscript->pEffectAuraDummy = &EffectAuraDummy_spell_aura_dummy_npc_crates_dummy;
    newscript->RegisterSelf();
}
