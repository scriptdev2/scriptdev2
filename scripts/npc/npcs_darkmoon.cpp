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
SDName: Npcs_Darkmoon
SD%Complete: 90
SDComment: Sayge fortune teller
SDCategory: NPCs
EndScriptData */

#define SPELL_DMG       23768                               //dmg
#define SPELL_RES       23769                               //res
#define SPELL_ARM       23767                               //arm
#define SPELL_SPI       23738                               //spi
#define SPELL_INT       23766                               //int
#define SPELL_STM       23737                               //stm
#define SPELL_STR       23735                               //str
#define SPELL_AGI       23736                               //agi
#define SPELL_FORTUNE   23765                               //faire fortune

#include "../sc_defines.h"
#include "../../../../game/Player.h"
#include "../../../../game/GossipDef.h"

bool GossipHello_npc_sayge(Player *player, Creature *_Creature)
{
    if(_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if( player->HasSpellCooldown(SPELL_INT) || 
        player->HasSpellCooldown(SPELL_ARM) || 
        player->HasSpellCooldown(SPELL_DMG) || 
        player->HasSpellCooldown(SPELL_RES) || 
        player->HasSpellCooldown(SPELL_STR) || 
        player->HasSpellCooldown(SPELL_AGI) || 
        player->HasSpellCooldown(SPELL_STM) || 
        player->HasSpellCooldown(SPELL_SPI) )
        player->SEND_GOSSIP_MENU(7393, _Creature->GetGUID());
    else
    {
        player->ADD_GOSSIP_ITEM(0, "Yes", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->SEND_GOSSIP_MENU(7339, _Creature->GetGUID());
    }

    return true;
}

void SendAction_npc_sayge(Player *player, Creature *_Creature, uint32 action)
{
    switch(action)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        player->ADD_GOSSIP_ITEM(0, "Slay the Man",                      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        player->ADD_GOSSIP_ITEM(0, "Turn him over to liege",            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
        player->ADD_GOSSIP_ITEM(0, "Confiscate the corn",               GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
        player->ADD_GOSSIP_ITEM(0, "Let him go and have the corn",      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
        player->SEND_GOSSIP_MENU(7340, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+2:
        player->ADD_GOSSIP_ITEM(0, "Execute your friend painfully",     GOSSIP_SENDER_MAIN+1, GOSSIP_ACTION_INFO_DEF);
        player->ADD_GOSSIP_ITEM(0, "Execute your friend painlessly",    GOSSIP_SENDER_MAIN+2, GOSSIP_ACTION_INFO_DEF);
        player->ADD_GOSSIP_ITEM(0, "Let your friend go",                GOSSIP_SENDER_MAIN+3, GOSSIP_ACTION_INFO_DEF);
        player->SEND_GOSSIP_MENU(7341, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+3:
        player->ADD_GOSSIP_ITEM(0, "Confront the diplomat",             GOSSIP_SENDER_MAIN+4, GOSSIP_ACTION_INFO_DEF);
        player->ADD_GOSSIP_ITEM(0, "Show not so quiet defiance",        GOSSIP_SENDER_MAIN+5, GOSSIP_ACTION_INFO_DEF);
        player->ADD_GOSSIP_ITEM(0, "Remain quiet",                      GOSSIP_SENDER_MAIN+2, GOSSIP_ACTION_INFO_DEF);
        player->SEND_GOSSIP_MENU(7361, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+4:
        player->ADD_GOSSIP_ITEM(0, "Speak against your brother openly", GOSSIP_SENDER_MAIN+6, GOSSIP_ACTION_INFO_DEF);
        player->ADD_GOSSIP_ITEM(0, "Help your brother in",              GOSSIP_SENDER_MAIN+7, GOSSIP_ACTION_INFO_DEF);
        player->ADD_GOSSIP_ITEM(0, "Keep your brother out without letting him know", GOSSIP_SENDER_MAIN+8, GOSSIP_ACTION_INFO_DEF);
        player->SEND_GOSSIP_MENU(7362, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+5:
        player->ADD_GOSSIP_ITEM(0, "Take credit, keep gold",            GOSSIP_SENDER_MAIN+5, GOSSIP_ACTION_INFO_DEF);
        player->ADD_GOSSIP_ITEM(0, "Take credit, share the gold",       GOSSIP_SENDER_MAIN+4, GOSSIP_ACTION_INFO_DEF);
        player->ADD_GOSSIP_ITEM(0, "Let the knight take credit",        GOSSIP_SENDER_MAIN+3, GOSSIP_ACTION_INFO_DEF);
        player->SEND_GOSSIP_MENU(7363, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF:
        player->ADD_GOSSIP_ITEM(0, "Thanks",                            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
        player->SEND_GOSSIP_MENU(7364, _Creature->GetGUID());
        break;
    case GOSSIP_ACTION_INFO_DEF+6:
        _Creature->CastSpell(player, SPELL_FORTUNE, false);
        player->SEND_GOSSIP_MENU(7365, _Creature->GetGUID());
        break;
    }
}

bool GossipSelect_npc_sayge(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch(sender) 
    {
        case GOSSIP_SENDER_MAIN:
            SendAction_npc_sayge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_MAIN+1:
            _Creature->CastSpell(player, SPELL_DMG, false);
            player->AddSpellCooldown(SPELL_DMG,0,time(NULL) + 7200);
            SendAction_npc_sayge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_MAIN+2:
            _Creature->CastSpell(player, SPELL_RES, false);
            player->AddSpellCooldown(SPELL_RES,0,time(NULL) + 7200);
            SendAction_npc_sayge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_MAIN+3:
            _Creature->CastSpell(player, SPELL_ARM, false);
            player->AddSpellCooldown(SPELL_ARM,0,time(NULL) + 7200);
            SendAction_npc_sayge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_MAIN+4:
            _Creature->CastSpell(player, SPELL_SPI, false);
            player->AddSpellCooldown(SPELL_SPI,0,time(NULL) + 7200);
            SendAction_npc_sayge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_MAIN+5:
            _Creature->CastSpell(player, SPELL_INT, false);
            player->AddSpellCooldown(SPELL_INT,0,time(NULL) + 7200);
            SendAction_npc_sayge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_MAIN+6:
            _Creature->CastSpell(player, SPELL_STM, false);
            player->AddSpellCooldown(SPELL_STM,0,time(NULL) + 7200);
            SendAction_npc_sayge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_MAIN+7:
            _Creature->CastSpell(player, SPELL_STR, false);
            player->AddSpellCooldown(SPELL_STR,0,time(NULL) + 7200);
            SendAction_npc_sayge(player, _Creature, action);
            break;
        case GOSSIP_SENDER_MAIN+8:
            _Creature->CastSpell(player, SPELL_AGI, false);
            player->AddSpellCooldown(SPELL_AGI,0,time(NULL) + 7200);
            SendAction_npc_sayge(player, _Creature, action);
            break;
    }
    return true;
}

void AddSC_npcs_darkmoon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_sayge";
    newscript->pGossipHello = &GossipHello_npc_sayge;
    newscript->pGossipSelect = &GossipSelect_npc_sayge;
    m_scripts[nrscripts++] = newscript;
}
