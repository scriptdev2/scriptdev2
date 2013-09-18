/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: ulduar
SD%Complete: 10%
SDComment: Teleporters are hacked until solved in core
SDCategory: Ulduar
EndScriptData */

/* ContentData
go_ulduar_teleporter
EndContentData */

#include "precompiled.h"
#include "ulduar.h"

/*#####
## go_ulduar_teleporter
#####*/

/* ****
* The teleporter spells cannot be used atm, because target-type TARGET_SCRIPT_COORDINATES, NO_TARGET is not yet suitable for needed targeting. (Current core-Design)
* All teleporters are GO with entry 194569 - on them are npcs of entry 32780 spawned.
* However for reload case we would need to be able to target these npcs of not yet loaded grids (currently impossible)
* And in general we would need some "good" way of selecting appropriate target-npcs for each spell, but sorting is nearly impossible, as there are > 50 of these npcs spawned in Ulduar

* So --  TODO -- remove the TeleportTo Hacks when correct target selection for this spell is working.
*/

enum TeleporterSpells
{
    SPELL_TELE_EXPEDITION_BASE_CAMP         = 64014,
    SPELL_TELE_FORMATION_GROUNDS            = 64032,
    SPELL_TELE_COLOSSAL_FORGE               = 64028,
    SPELL_TELE_SCRAPYARD                    = 64031,
    SPELL_TELE_ANTECHAMBER_OF_ULDUAR        = 64030,
    SPELL_TELE_SHATTERED_WALKWAY            = 64029,
    SPELL_TELE_CONSERVATORY_OF_LIFE         = 64024,
    SPELL_TELE_SPARK_OF_IMAGINATION         = 65061,
    SPELL_TELE_PRISON_OF_YOGG               = 65042,
};

// Teleporter Gossip handled by SD2 because depending on Instance Data
enum TeleporterGossipItems
{
    GOSSIP_ITEM_TELE_BASE_CAMP              = -3603000,
    GOSSIP_ITEM_TELE_FORMATION_GROUNDS      = -3603001,
    GOSSIP_ITEM_TELE_COLOSSAL_FORGE         = -3603002,
    GOSSIP_ITEM_TELE_SCRAPYARD              = -3603003,
    GOSSIP_ITEM_TELE_ANTECHAMBER            = -3603004,
    GOSSIP_ITEM_TELE_WALKWAY                = -3603005,
    GOSSIP_ITEM_TELE_CONSERVATORY           = -3603006,
    GOSSIP_ITEM_TELE_SPARK_IMAGINATION      = -3603007,
    GOSSIP_ITEM_TELE_YOGG_SARON             = -3603008,
};

bool GossipHello_go_ulduar_teleporter(Player* pPlayer, GameObject* pGo)
{
    instance_ulduar* pInstance = (instance_ulduar*)pPlayer->GetInstanceData();
    if (!pInstance)
        return true;

    // Base camp
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_BASE_CAMP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    // Formation grounds
    if (pInstance->GetData(TYPE_LEVIATHAN) != NOT_STARTED || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_FORMATION_GROUNDS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    // Colossal Forge
    if (pInstance->GetData(TYPE_LEVIATHAN) == DONE || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_COLOSSAL_FORGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

    // Scrapyard
    if (pInstance->GetData(TYPE_XT002) != NOT_STARTED || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_SCRAPYARD, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

    // Antechamber
    if (pInstance->GetData(TYPE_XT002) == DONE || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_ANTECHAMBER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);

    // Shattered walkway
    if (pInstance->GetData(TYPE_KOLOGARN) == DONE || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_WALKWAY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);

    // Conservatory of life
    if (pInstance->GetData(TYPE_AURIAYA) == DONE || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_CONSERVATORY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);

    // Spark of imagination
    if (pInstance->GetData(TYPE_MIMIRON) != NOT_STARTED || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_SPARK_IMAGINATION, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);

    // Prison of Yogg-Saron
    if (pInstance->GetData(TYPE_VEZAX) == DONE || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_YOGG_SARON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pGo->GetGOInfo()->GetGossipMenuId(), pGo), pGo->GetObjectGuid());

    return true;
}

bool GossipSelect_go_ulduar_teleporter(Player* pPlayer, GameObject* pGO, uint32 uiSender, uint32 uiAction)
{
    instance_ulduar* pInstance = (instance_ulduar*)pPlayer->GetInstanceData();
    if (!pInstance)
        return true;

    // Additional checks for the teleporters to prevent exploiting
    // -- TODO -- HACK HERE, use spells when possible!

    // There needs to be displayed a msg when in Combat, it is likely that this is to be handled by core and spell can-cast check
    // -- TODO -- Remove the combat check when spells are correctly working
    if (pPlayer->isInCombat())
        return true;

    switch (uiAction)
    {
            // Basecamp
        case GOSSIP_ACTION_INFO_DEF:
            // pPlayer->CastSpell(pPlayer, SPELL_TELE_EXPEDITION_BASE_CAMP, true, NULL, NULL, pGo->GetObjectGuid());
            pPlayer->TeleportTo(603, -706.122f, -92.6024f, 429.876f, 0);
            break;
            // Formation Grounds
        case GOSSIP_ACTION_INFO_DEF + 1:
                // pPlayer->CastSpell(pPlayer, SPELL_TELE_FORMATION_GROUNDS, true, NULL, NULL, pGo->GetObjectGuid());
                pPlayer->TeleportTo(603, 131.248f, -35.3802f, 409.804f, 0);
            break;
            // Colossal Forge
        case GOSSIP_ACTION_INFO_DEF + 2:
                // pPlayer->CastSpell(pPlayer, SPELL_TELE_COLOSSAL_FORGE, true, NULL, NULL, pGo->GetObjectGuid());
                pPlayer->TeleportTo(603, 553.233f, -12.3247f, 409.679f, 0);
            break;
            // Scrapyard
        case GOSSIP_ACTION_INFO_DEF + 3:
                // pPlayer->CastSpell(pPlayer, SPELL_TELE_SCRAPYARD, true, NULL, NULL, pGo->GetObjectGuid());
                pPlayer->TeleportTo(603, 926.292f, -11.4635f, 418.595f, 0);
            break;
            // Antechamber
        case GOSSIP_ACTION_INFO_DEF + 4:
                // pPlayer->CastSpell(pPlayer, SPELL_TELE_ANTECHAMBER_OF_ULDUAR, true, NULL, NULL, pGo->GetObjectGuid());
                pPlayer->TeleportTo(603, 1498.09f, -24.246f, 420.967f, 0);
            break;
            // Shattered walkway
        case GOSSIP_ACTION_INFO_DEF + 5:
                // pPlayer->CastSpell(pPlayer, SPELL_TELE_SHATTERED_WALKWAY, true, NULL, NULL, pGo->GetObjectGuid());
                pPlayer->TeleportTo(603, 1859.45f, -24.1f, 448.9f, 0);
            break;
            // Conservatory of life
        case GOSSIP_ACTION_INFO_DEF + 6:
                // pPlayer->CastSpell(pPlayer, SPELL_TELE_CONSERVATORY_OF_LIFE, true, NULL, NULL, pGo->GetObjectGuid());
                pPlayer->TeleportTo(603, 2086.27f, -24.3134f, 421.239f, 0);
            break;
            // Spark of imagination
        case GOSSIP_ACTION_INFO_DEF + 7:
                // pPlayer->CastSpell(pPlayer, SPELL_TELE_SPARK_OF_IMAGINATION, true, NULL, NULL, pGo->GetObjectGuid());
                pPlayer->TeleportTo(603, 2518.16f, 2569.03f, 412.299f, 0);
            break;
            // Prison of Yogg-Saron
        case GOSSIP_ACTION_INFO_DEF + 8:
                // pPlayer->CastSpell(pPlayer, SPELL_TELE_PRISON_OF_YOGG, true, NULL, NULL, pGo->GetObjectGuid());
                pPlayer->TeleportTo(603, 1854.82f, -11.56f, 334.175f, 4.71f);
            break;
        default:
            return true;
    }

    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

void AddSC_ulduar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "go_ulduar_teleporter";
    pNewScript->pGossipHelloGO = &GossipHello_go_ulduar_teleporter;
    pNewScript->pGossipSelectGO = &GossipSelect_go_ulduar_teleporter;
    pNewScript->RegisterSelf();
}
