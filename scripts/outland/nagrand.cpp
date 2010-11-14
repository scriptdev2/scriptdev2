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
SDName: Nagrand
SD%Complete: 90
SDComment: Quest support: 9849, 9868, 9874, 9918, 9991, 10044, 10085, 10107, 10108, 10172, 10646. TextId's unknown for altruis_the_sufferer and greatmother_geyah (npc_text)
SDCategory: Nagrand
EndScriptData */

/* ContentData
mob_shattered_rumbler
mob_lump
mob_sunspring_villager
npc_altruis_the_sufferer
npc_greatmother_geyah
npc_lantresor_of_the_blade
npc_maghar_captive
npc_creditmarker_visit_with_ancestors
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

/*######
## mob_shattered_rumbler - this should be done with ACID
######*/

struct MANGOS_DLL_DECL mob_shattered_rumblerAI : public ScriptedAI
{
    bool Spawn;

    mob_shattered_rumblerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        Spawn = false;
    }

    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
    {
        if (Spellkind->Id == 32001 && !Spawn)
        {
            float x = m_creature->GetPositionX();
            float y = m_creature->GetPositionY();
            float z = m_creature->GetPositionZ();

            Hitter->SummonCreature(18181,x+(0.7 * (rand()%30)),y+(rand()%5),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
            Hitter->SummonCreature(18181,x+(rand()%5),y-(rand()%5),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
            Hitter->SummonCreature(18181,x-(rand()%5),y+(0.5 *(rand()%60)),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
            m_creature->SetDeathState(CORPSE);
            Spawn = true;
        }
        return;
    }
};
CreatureAI* GetAI_mob_shattered_rumbler(Creature* pCreature)
{
    return new mob_shattered_rumblerAI(pCreature);
}

/*######
## mob_lump - TODO: remove gossip, can be done in database
######*/

enum
{
    QUEST_NOT_ON_MY_WATCH       = 9918,
    NPC_LUMPS_QUEST_CREDIT      = 18354,

    SAY_LUMP_AGGRO_1            = -1000190,
    SAY_LUMP_AGGRO_2            = -1000191,
    SAY_LUMP_DEFEAT             = -1000192,

    TEXT_ID_LUMP_1              = 9352,
    TEXT_ID_LUMP_2              = 9353,
    TEXT_ID_LUMP_3              = 9354,
    TEXT_ID_LUMP_4              = 9355,
    TEXT_ID_LUMP_5              = 9356,

    SPELL_VISUAL_SLEEP          = 16093,
    SPELL_SPEAR_THROW           = 32248,

    FACTION_FRIENDLY            = 35
};

struct MANGOS_DLL_DECL mob_lumpAI : public ScriptedAI
{
    mob_lumpAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bReset = false;
        Reset();
    }

    uint32 m_uiResetTimer;
    uint32 m_uiSpearThrowTimer;
    bool m_bReset;

    void Reset()
    {
        m_uiResetTimer = MINUTE*IN_MILLISECONDS;
        m_uiSpearThrowTimer = 2000;

        if (m_creature->getFaction() != m_creature->GetCreatureInfo()->faction_A)
            m_creature->setFaction(m_creature->GetCreatureInfo()->faction_A);
    }

    void AttackedBy(Unit* pAttacker)
    {
        if (m_creature->getVictim())
            return;

        if (m_creature->IsFriendlyTo(pAttacker))
            return;

        AttackStart(pAttacker);
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        if (m_creature->GetHealth() < uiDamage || (m_creature->GetHealth() - uiDamage)*100 / m_creature->GetMaxHealth() < 30)
        {
            Player* pPlayer = pDealer->GetCharmerOrOwnerPlayerOrPlayerItself();
            if (!m_bReset && pPlayer && pPlayer->GetQuestStatus(QUEST_NOT_ON_MY_WATCH) == QUEST_STATUS_INCOMPLETE)
            {
                uiDamage = 0;                               //Take 0 damage

                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop(true);

                // should get unit_flags UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_PASSIVE at faction change, but unclear why/for what reason, skipped (no flags expected as default)
                m_creature->setFaction(FACTION_FRIENDLY);

                m_creature->SetStandState(UNIT_STAND_STATE_SIT);
                DoScriptText(SAY_LUMP_DEFEAT, m_creature, pPlayer);

                m_bReset = true;
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_creature->HasAura(SPELL_VISUAL_SLEEP, EFFECT_INDEX_0))
            m_creature->RemoveAurasDueToSpell(SPELL_VISUAL_SLEEP);

        if (!m_creature->IsStandState())
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);

        DoScriptText(urand(0, 1) ? SAY_LUMP_AGGRO_1 : SAY_LUMP_AGGRO_2, m_creature, pWho);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Check if we waiting for a reset
        if (m_bReset)
        {
            if (m_uiResetTimer < uiDiff)
            {
                EnterEvadeMode();
                m_bReset = false;
            }
            else
                m_uiResetTimer -= uiDiff;
        }

        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // SpearThrow Timer
        if (m_uiSpearThrowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SPEAR_THROW);
            m_uiSpearThrowTimer = 20000;
        }
        else
            m_uiSpearThrowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_lump(Creature* pCreature)
{
    return new mob_lumpAI(pCreature);
}

bool GossipHello_mob_lump(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_NOT_ON_MY_WATCH) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I need answers, ogre!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(TEXT_ID_LUMP_1, pCreature->GetGUID());

    return true;
}

bool GossipSelect_mob_lump(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Why are Boulderfist out this far? You know that this is Kurenai territory.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_LUMP_2, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "And you think you can just eat anything you want? You're obviously trying to eat the Broken of Telaar.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_LUMP_3, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "This means war, Lump! War I say!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_LUMP_4, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_LUMP_5, pCreature->GetGUID());
            pPlayer->TalkedToCreature(NPC_LUMPS_QUEST_CREDIT, pCreature->GetGUID());
            break;
    }
    return true;
}

/*####
# mob_sunspring_villager - should be done with ACID
####*/

struct MANGOS_DLL_DECL mob_sunspring_villagerAI : public ScriptedAI
{
    mob_sunspring_villagerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        m_creature->SetUInt32Value(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
        m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (spell->Id == 32146)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            m_creature->RemoveCorpse();
        }
    }
};
CreatureAI* GetAI_mob_sunspring_villager(Creature* pCreature)
{
    return new mob_sunspring_villagerAI(pCreature);
}

/*######
## npc_altruis_the_sufferer
######*/

enum
{
    QUEST_SURVEY        = 9991,
    QUEST_PUPIL         = 10646,

    TAXI_PATH_ID        = 532
};

bool GossipHello_npc_altruis_the_sufferer(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    //gossip before obtaining Survey the Land
    if (pPlayer->GetQuestStatus(QUEST_SURVEY) == QUEST_STATUS_NONE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I see twisted steel and smell sundered earth.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+10);

    //gossip when Survey the Land is incomplete (technically, after the flight)
    if (pPlayer->GetQuestStatus(QUEST_SURVEY) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Well...?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+20);

    //wowwiki.com/Varedis
    if (pPlayer->GetQuestStatus(QUEST_PUPIL) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[PH] Story about Illidan's Pupil", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+30);

    pPlayer->SEND_GOSSIP_MENU(9419, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_altruis_the_sufferer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+10:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Legion?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            pPlayer->SEND_GOSSIP_MENU(9420, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+11:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "And now?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
            pPlayer->SEND_GOSSIP_MENU(9421, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+12:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "How do you see them now?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
            pPlayer->SEND_GOSSIP_MENU(9422, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+13:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Forge camps?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
            pPlayer->SEND_GOSSIP_MENU(9423, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+14:
            pPlayer->SEND_GOSSIP_MENU(9424, pCreature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF+20:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Ok.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
            pPlayer->SEND_GOSSIP_MENU(9427, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+21:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(QUEST_SURVEY);
            break;

        case GOSSIP_ACTION_INFO_DEF+30:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[PH] Story done", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
            pPlayer->SEND_GOSSIP_MENU(384, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+31:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(QUEST_PUPIL);
            break;
    }
    return true;
}

bool QuestAccept_npc_altruis_the_sufferer(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (!pPlayer->GetQuestRewardStatus(QUEST_SURVEY))       //Survey the Land
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID);
    }
    return true;
}

/*######
## npc_greatmother_geyah
######*/

//all the textId's for the below is unknown, but i do believe the gossip item texts are proper.
bool GossipHello_npc_greatmother_geyah(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(10044) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Hello, Greatmother. Garrosh told me that you wanted to speak with me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    }
    else if (pPlayer->GetQuestStatus(10172) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Garrosh is beyond redemption, Greatmother. I fear that in helping the Mag'har, I have convinced Garrosh that he is unfit to lead.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    }
    else

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_greatmother_geyah(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "You raised all of the orcs here, Greatmother?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Do you believe that?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What can be done? I have tried many different things. I have done my best to help the people of Nagrand. Each time I have approached Garrosh, he has dismissed me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Left? How can you choose to leave?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What is this duty?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Is there anything I can do for you, Greatmother?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:
            pPlayer->AreaExploredOrEventHappens(10044);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;

        case GOSSIP_ACTION_INFO_DEF + 10:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I have done all that I could, Greatmother. I thank you for your kind words.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Greatmother, you are the mother of Durotan?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Greatmother, I never had the honor. Durotan died long before my time, but his heroics are known to all on my world. The orcs of Azeroth reside in a place known as Durotar, named after your son. And ... (You take a moment to breathe and think through what you are about to tell the Greatmother.)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 13:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "It is my Warchief, Greatmother. The leader of my people. From my world. He ... He is the son of Durotan. He is your grandchild.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 14:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I will return to Azeroth at once, Greatmother.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 15:
            pPlayer->AreaExploredOrEventHappens(10172);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
    }
    return true;
}

/*######
## npc_lantresor_of_the_blade
######*/

bool GossipHello_npc_lantresor_of_the_blade(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(10107) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(10108) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I have killed many of your ogres, Lantresor. I have no fear.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(9361, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_lantresor_of_the_blade(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Should I know? You look like an orc to me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(9362, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "And the other half?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(9363, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I have heard of your kind, but I never thought to see the day when I would meet a half-breed.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(9364, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "My apologies. I did not mean to offend. I am here on behalf of my people.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(9365, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "My people ask that you pull back your Boulderfist ogres and cease all attacks on our territories. In return, we will also pull back our forces.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            pPlayer->SEND_GOSSIP_MENU(9366, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "We will fight you until the end, then, Lantresor. We will not stand idly by as you pillage our towns and kill our people.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            pPlayer->SEND_GOSSIP_MENU(9367, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What do I need to do?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            pPlayer->SEND_GOSSIP_MENU(9368, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            pPlayer->SEND_GOSSIP_MENU(9369, pCreature->GetGUID());
            if (pPlayer->GetQuestStatus(10107) == QUEST_STATUS_INCOMPLETE)
                pPlayer->AreaExploredOrEventHappens(10107);
            if (pPlayer->GetQuestStatus(10108) == QUEST_STATUS_INCOMPLETE)
                pPlayer->AreaExploredOrEventHappens(10108);
            break;
    }
    return true;
}

/*#####
## npc_maghar_captive
#####*/

enum
{
    SAY_MAG_START               = -1000482,
    SAY_MAG_NO_ESCAPE           = -1000483,
    SAY_MAG_MORE                = -1000484,
    SAY_MAG_MORE_REPLY          = -1000485,
    SAY_MAG_LIGHTNING           = -1000486,
    SAY_MAG_SHOCK               = -1000487,
    SAY_MAG_COMPLETE            = -1000488,

    SPELL_CHAIN_LIGHTNING       = 16006,
    SPELL_EARTHBIND_TOTEM       = 15786,
    SPELL_FROST_SHOCK           = 12548,
    SPELL_HEALING_WAVE          = 12491,

    QUEST_TOTEM_KARDASH_H       = 9868,

    NPC_MURK_RAIDER             = 18203,
    NPC_MURK_BRUTE              = 18211,
    NPC_MURK_SCAVENGER          = 18207,
    NPC_MURK_PUTRIFIER          = 18202
};

static float m_afAmbushA[]= {-1568.805786f, 8533.873047f, 1.958f};
static float m_afAmbushB[]= {-1491.554321f, 8506.483398f, 1.248f};

struct MANGOS_DLL_DECL npc_maghar_captiveAI : public npc_escortAI
{
    npc_maghar_captiveAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    uint32 m_uiChainLightningTimer;
    uint32 m_uiHealTimer;
    uint32 m_uiFrostShockTimer;

    void Reset()
    {
        m_uiChainLightningTimer = 1000;
        m_uiHealTimer = 0;
        m_uiFrostShockTimer = 6000;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->CastSpell(m_creature, SPELL_EARTHBIND_TOTEM, false);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 7:
                DoScriptText(SAY_MAG_MORE, m_creature);

                if (Creature* pTemp = m_creature->SummonCreature(NPC_MURK_PUTRIFIER, m_afAmbushB[0], m_afAmbushB[1], m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000))
                    DoScriptText(SAY_MAG_MORE_REPLY, pTemp);

                m_creature->SummonCreature(NPC_MURK_PUTRIFIER, m_afAmbushB[0]-2.5f, m_afAmbushB[1]-2.5f, m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);

                m_creature->SummonCreature(NPC_MURK_SCAVENGER, m_afAmbushB[0]+2.5f, m_afAmbushB[1]+2.5f, m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(NPC_MURK_SCAVENGER, m_afAmbushB[0]+2.5f, m_afAmbushB[1]-2.5f, m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                break;
            case 16:
                DoScriptText(SAY_MAG_COMPLETE, m_creature);

                if (Player* pPlayer = GetPlayerForEscort())
                    pPlayer->GroupEventHappens(QUEST_TOTEM_KARDASH_H, m_creature);

                SetRun();
                break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_MURK_BRUTE)
            DoScriptText(SAY_MAG_NO_ESCAPE, pSummoned);

        if (pSummoned->IsTotem())
            return;

        pSummoned->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        pSummoned->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_CHAIN_LIGHTNING)
        {
            if (urand(0, 9))
                return;

            DoScriptText(SAY_MAG_LIGHTNING, m_creature);
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiChainLightningTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHAIN_LIGHTNING);
            m_uiChainLightningTimer = urand(7000, 14000);
        }
        else
            m_uiChainLightningTimer -= uiDiff;

        if (m_creature->GetHealthPercent() < 30.0f)
        {
            if (m_uiHealTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_HEALING_WAVE);
                m_uiHealTimer = 5000;
            }
            else
                m_uiHealTimer -= uiDiff;
        }

        if (m_uiFrostShockTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_SHOCK);
            m_uiFrostShockTimer = urand(7500, 15000);
        }
        else
            m_uiFrostShockTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

bool QuestAccept_npc_maghar_captive(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_TOTEM_KARDASH_H)
    {
        if (npc_maghar_captiveAI* pEscortAI = dynamic_cast<npc_maghar_captiveAI*>(pCreature->AI()))
        {
            pCreature->SetStandState(UNIT_STAND_STATE_STAND);
            pCreature->setFaction(FACTION_ESCORT_H_NEUTRAL_ACTIVE);

            pEscortAI->Start(false, pPlayer->GetGUID(), pQuest);

            DoScriptText(SAY_MAG_START, pCreature);

            pCreature->SummonCreature(NPC_MURK_RAIDER, m_afAmbushA[0]+2.5f, m_afAmbushA[1]-2.5f, m_afAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
            pCreature->SummonCreature(NPC_MURK_PUTRIFIER, m_afAmbushA[0]-2.5f, m_afAmbushA[1]+2.5f, m_afAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
            pCreature->SummonCreature(NPC_MURK_BRUTE, m_afAmbushA[0], m_afAmbushA[1], m_afAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
        }
    }
    return true;
}

CreatureAI* GetAI_npc_maghar_captive(Creature* pCreature)
{
    return new npc_maghar_captiveAI(pCreature);
}

/*######
## npc_creditmarker_visist_with_ancestors (Quest 10085)
######*/

enum
{
    QUEST_VISIT_WITH_ANCESTORS  = 10085
};

struct MANGOS_DLL_DECL npc_creditmarker_visit_with_ancestorsAI : public ScriptedAI
{
    npc_creditmarker_visit_with_ancestorsAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() {}

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 30.0f))
        {
            if (((Player*)pWho)->GetQuestStatus(QUEST_VISIT_WITH_ANCESTORS) == QUEST_STATUS_INCOMPLETE)
            {
                uint32 creditMarkerId = m_creature->GetEntry();
                if ((creditMarkerId >= 18840) && (creditMarkerId <= 18843))
                {
                    // 18840: Sunspring, 18841: Laughing, 18842: Garadar, 18843: Bleeding
                    if (!((Player*)pWho)->GetReqKillOrCastCurrentCount(QUEST_VISIT_WITH_ANCESTORS, creditMarkerId))
                        ((Player*)pWho)->KilledMonsterCredit(creditMarkerId, m_creature->GetGUID());
                }
            }
        }
    }
};

CreatureAI* GetAI_npc_creditmarker_visit_with_ancestors(Creature* pCreature)
{
    return new npc_creditmarker_visit_with_ancestorsAI(pCreature);
}

/*######
## AddSC
######*/

void AddSC_nagrand()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mob_shattered_rumbler";
    pNewScript->GetAI = &GetAI_mob_shattered_rumbler;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_lump";
    pNewScript->GetAI = &GetAI_mob_lump;
    pNewScript->pGossipHello =  &GossipHello_mob_lump;
    pNewScript->pGossipSelect = &GossipSelect_mob_lump;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_sunspring_villager";
    pNewScript->GetAI = &GetAI_mob_sunspring_villager;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_altruis_the_sufferer";
    pNewScript->pGossipHello =  &GossipHello_npc_altruis_the_sufferer;
    pNewScript->pGossipSelect = &GossipSelect_npc_altruis_the_sufferer;
    pNewScript->pQuestAccept =  &QuestAccept_npc_altruis_the_sufferer;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_greatmother_geyah";
    pNewScript->pGossipHello =  &GossipHello_npc_greatmother_geyah;
    pNewScript->pGossipSelect = &GossipSelect_npc_greatmother_geyah;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_lantresor_of_the_blade";
    pNewScript->pGossipHello =  &GossipHello_npc_lantresor_of_the_blade;
    pNewScript->pGossipSelect = &GossipSelect_npc_lantresor_of_the_blade;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_maghar_captive";
    pNewScript->GetAI = &GetAI_npc_maghar_captive;
    pNewScript->pQuestAccept = &QuestAccept_npc_maghar_captive;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_creditmarker_visit_with_ancestors";
    pNewScript->GetAI = &GetAI_npc_creditmarker_visit_with_ancestors;
    pNewScript->RegisterSelf();
}
