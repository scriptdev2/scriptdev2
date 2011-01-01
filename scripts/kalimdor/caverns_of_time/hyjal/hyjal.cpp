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
SDName: Hyjal
SD%Complete: 80
SDComment: gossip text id's unknown
SDCategory: Caverns of Time, Mount Hyjal
EndScriptData */

/* ContentData
npc_jaina_proudmoore
npc_thrall
npc_tyrande_whisperwind
EndContentData */

#include "precompiled.h"
#include "hyjalAI.h"

#define GOSSIP_ITEM_BEGIN_ALLY      "My companions and I are with you, Lady Proudmoore."
#define GOSSIP_ITEM_ANETHERON       "We are ready for whatever Archimonde might send our way, Lady Proudmoore."
#define GOSSIP_ITEM_BEGIN_HORDE     "I am with you, Thrall."
#define GOSSIP_ITEM_AZGALOR         "We have nothing to fear."

#define GOSSIP_ITEM_RETREAT         "We can't keep this up. Let's retreat!"

#define GOSSIP_ITEM_TYRANDE         "Aid us in defending Nordrassil"

CreatureAI* GetAI_npc_jaina_proudmoore(Creature* pCreature)
{
    hyjalAI* pTempAI = new hyjalAI(pCreature);

    pTempAI->m_aSpells[0].m_uiSpellId = SPELL_BLIZZARD;
    pTempAI->m_aSpells[0].m_uiCooldown = urand(15000, 35000);
    pTempAI->m_aSpells[0].m_pType = TARGETTYPE_RANDOM;

    pTempAI->m_aSpells[1].m_uiSpellId = SPELL_PYROBLAST;
    pTempAI->m_aSpells[1].m_uiCooldown = urand(2000, 9000);
    pTempAI->m_aSpells[1].m_pType = TARGETTYPE_RANDOM;

    pTempAI->m_aSpells[2].m_uiSpellId = SPELL_SUMMON_ELEMENTALS;
    pTempAI->m_aSpells[2].m_uiCooldown = urand(15000, 45000);
    pTempAI->m_aSpells[2].m_pType = TARGETTYPE_SELF;

    return pTempAI;
}

bool GossipHello_npc_jaina_proudmoore(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        if (hyjalAI* pJainaAI = dynamic_cast<hyjalAI*>(pCreature->AI()))
        {
            if (!pJainaAI->m_bIsEventInProgress)
            {
                // Should not happen that jaina is here now, but for safe we check
                if (pInstance->GetData(TYPE_KAZROGAL) != DONE)
                {
                    if (pInstance->GetData(TYPE_WINTERCHILL) == NOT_STARTED)
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BEGIN_ALLY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    else if (pInstance->GetData(TYPE_WINTERCHILL) == DONE && pInstance->GetData(TYPE_ANETHERON) == NOT_STARTED)
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ANETHERON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    else if (pInstance->GetData(TYPE_ANETHERON) == DONE)
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RETREAT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

                    if (pPlayer->isGameMaster())
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[GM] Toggle Debug Timers", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                }
            }
        }
    }

    pPlayer->SEND_GOSSIP_MENU(907, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_jaina_proudmoore(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (hyjalAI* pJainaAI = dynamic_cast<hyjalAI*>(pCreature->AI()))
    {
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                pJainaAI->StartEvent();
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                pJainaAI->m_bIsFirstBossDead = true;
                pJainaAI->m_uiWaveCount = 9;
                pJainaAI->StartEvent();
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                pJainaAI->Retreat();
                break;
            case GOSSIP_ACTION_INFO_DEF:
                pJainaAI->m_bDebugMode = !pJainaAI->m_bDebugMode;
                debug_log("SD2: HyjalAI - Debug mode has been toggled");
                break;
        }
    }

    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

CreatureAI* GetAI_npc_thrall(Creature* pCreature)
{
    hyjalAI* pTempAI = new hyjalAI(pCreature);

    pTempAI->m_aSpells[0].m_uiSpellId = SPELL_CHAIN_LIGHTNING;
    pTempAI->m_aSpells[0].m_uiCooldown = urand(2000, 7000);
    pTempAI->m_aSpells[0].m_pType = TARGETTYPE_VICTIM;

    pTempAI->m_aSpells[1].m_uiSpellId = SPELL_FERAL_SPIRIT;
    pTempAI->m_aSpells[1].m_uiCooldown = urand(6000, 41000);
    pTempAI->m_aSpells[1].m_pType = TARGETTYPE_RANDOM;

    return pTempAI;
}

bool GossipHello_npc_thrall(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        if (hyjalAI* pThrallAI = dynamic_cast<hyjalAI*>(pCreature->AI()))
        {
            if (!pThrallAI->m_bIsEventInProgress)
            {
                // Only let them start the Horde phases if Anetheron is dead.
                if (pInstance->GetData(TYPE_ANETHERON) == DONE)
                {
                    if (pInstance->GetData(TYPE_KAZROGAL) == NOT_STARTED)
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BEGIN_HORDE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    else if (pInstance->GetData(TYPE_KAZROGAL) == DONE && pInstance->GetData(TYPE_AZGALOR) == NOT_STARTED)
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_AZGALOR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    else if (pInstance->GetData(TYPE_AZGALOR) == DONE)
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RETREAT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

                    if (pPlayer->isGameMaster())
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[GM] Toggle Debug Timers", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                }
            }
        }
    }

    pPlayer->SEND_GOSSIP_MENU(907, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_thrall(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (hyjalAI* pThrallAI = dynamic_cast<hyjalAI*>(pCreature->AI()))
    {
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                pThrallAI->StartEvent();
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                pThrallAI->m_bIsFirstBossDead = true;
                pThrallAI->m_uiWaveCount = 9;
                pThrallAI->StartEvent();
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                pThrallAI->Retreat();
                break;
            case GOSSIP_ACTION_INFO_DEF:
                pThrallAI->m_bDebugMode = !pThrallAI->m_bDebugMode;
                debug_log("SD2: HyjalAI - Debug mode has been toggled");
                break;
        }
    }

    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

bool GossipHello_npc_tyrande_whisperwind(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        // Only let them get item if Azgalor is dead.
        if (pInstance->GetData(TYPE_AZGALOR) == DONE && !pPlayer->HasItemCount(ITEM_TEAR_OF_GODDESS,1))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TYRANDE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
    }

    pPlayer->SEND_GOSSIP_MENU(907, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_tyrande_whisperwind(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        if (Item* pItem = pPlayer->StoreNewItemInInventorySlot(ITEM_TEAR_OF_GODDESS, 1))
            pPlayer->SendNewItem(pItem, 1, true, false);
    }

    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

void AddSC_hyjal()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_jaina_proudmoore";
    newscript->GetAI = &GetAI_npc_jaina_proudmoore;
    newscript->pGossipHello = &GossipHello_npc_jaina_proudmoore;
    newscript->pGossipSelect = &GossipSelect_npc_jaina_proudmoore;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_thrall";
    newscript->GetAI = &GetAI_npc_thrall;
    newscript->pGossipHello = &GossipHello_npc_thrall;
    newscript->pGossipSelect = &GossipSelect_npc_thrall;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tyrande_whisperwind";
    newscript->pGossipHello = &GossipHello_npc_tyrande_whisperwind;
    newscript->pGossipSelect = &GossipSelect_npc_tyrande_whisperwind;
    newscript->RegisterSelf();
}
