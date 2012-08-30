/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Karazhan
SD%Complete: 100
SDComment: Quest support: 9645. Support for Barnes (Opera controller) and Berthold (Doorman).
SDCategory: Karazhan
EndScriptData */

/* ContentData
npc_barnes
npc_berthold
npc_image_of_medivh
EndContentData */

#include "precompiled.h"
#include "karazhan.h"
#include "escort_ai.h"

/*######
# npc_barnesAI
######*/

enum
{
    SAY_BARNES_EVENT_START  = -1532115,

    SAY_BARNES_OZ_1         = -1532103,
    SAY_BARNES_OZ_2         = -1532104,
    SAY_BARNES_OZ_3         = -1532105,
    SAY_BARNES_OZ_4         = -1532106,

    SAY_BARNES_HOOD_1       = -1532107,
    SAY_BARNES_HOOD_2       = -1532108,
    SAY_BARNES_HOOD_3       = -1532109,
    SAY_BARNES_HOOD_4       = -1532110,

    SAY_BARNES_RAJ_1        = -1532111,
    SAY_BARNES_RAJ_2        = -1532112,
    SAY_BARNES_RAJ_3        = -1532113,
    SAY_BARNES_RAJ_4        = -1532114,

    // ToDo: it's not very clear which is the gossip sequence for event FAIL case
    GOSSIP_ITEM_OPERA_1     = -3532001,
    GOSSIP_ITEM_OPERA_2     = -3532002,
    GOSSIP_ITEM_JUL_WIPE    = -3532003,
    GOSSIP_ITEM_WOLF_WIPE   = -3532004,

    TEXT_ID_OPERA_1         = 8970,
    TEXT_ID_OPERA_2         = 8971,
    TEXT_ID_OPERA_WOLF_WIPE = 8975,
    TEXT_ID_OPERA_OZ_WIPE   = 8781,             // guesswork, not confirmed
    //TEXT_ID_OPERA_JUL_WIPE  = ????,           // Item not found in DB: "The romantic plays are really tough, but you'll do better this time. You have TALENT. Ready?"

    //SPELL_SPOTLIGHT       = 25824,            // in creature_template_addon
    SPELL_TUXEDO            = 32616,

    NPC_SPOTLIGHT           = 19525,
};

static const DialogueEntry aIntroDialogue[] =
{
    {SAY_BARNES_OZ_1,   NPC_BARNES,  6000},
    {SAY_BARNES_OZ_2,   NPC_BARNES,  18000},
    {SAY_BARNES_OZ_3,   NPC_BARNES,  9000},
    {SAY_BARNES_OZ_4,   NPC_BARNES,  15000},
    {OPERA_EVENT_WIZARD_OZ, 0,       0},
    {SAY_BARNES_HOOD_1, NPC_BARNES,  6000},
    {SAY_BARNES_HOOD_2, NPC_BARNES,  10000},
    {SAY_BARNES_HOOD_3, NPC_BARNES,  14000},
    {SAY_BARNES_HOOD_4, NPC_BARNES,  15000},
    {OPERA_EVENT_RED_RIDING_HOOD, 0, 0},
    {SAY_BARNES_RAJ_1,  NPC_BARNES,  5000},
    {SAY_BARNES_RAJ_2,  NPC_BARNES,  7000},
    {SAY_BARNES_RAJ_3,  NPC_BARNES,  14000},
    {SAY_BARNES_RAJ_4,  NPC_BARNES,  14000},
    {OPERA_EVENT_ROMULO_AND_JUL, 0,  0},
    {0, 0, 0},
};

struct MANGOS_DLL_DECL npc_barnesAI : public npc_escortAI, private DialogueHelper
{
    npc_barnesAI(Creature* pCreature) : npc_escortAI(pCreature),
        DialogueHelper(aIntroDialogue)
    {
        m_pInstance  = (instance_karazhan*)pCreature->GetInstanceData();
        InitializeDialogueHelper(m_pInstance);
        Reset();
    }

    instance_karazhan* m_pInstance;

    ObjectGuid m_spotlightGuid;

    void Reset()
    {
        m_spotlightGuid.Clear();
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_SPOTLIGHT)
            m_spotlightGuid = pSummoned->GetObjectGuid();
    }

    void WaypointReached(uint32 uiPointId)
    {
        if (!m_pInstance)
            return;

        switch(uiPointId)
        {
            case 0:
                DoCastSpellIfCan(m_creature, SPELL_TUXEDO);
                m_pInstance->DoUseDoorOrButton(GO_STAGE_DOOR_LEFT);
                break;
            case 4:
                switch (m_pInstance->GetData(TYPE_OPERA_PERFORMANCE))
                {
                    case OPERA_EVENT_WIZARD_OZ:
                        StartNextDialogueText(SAY_BARNES_OZ_1);
                        break;
                    case OPERA_EVENT_RED_RIDING_HOOD:
                        StartNextDialogueText(SAY_BARNES_HOOD_1);
                        break;
                    case OPERA_EVENT_ROMULO_AND_JUL:
                        StartNextDialogueText(SAY_BARNES_RAJ_1);
                        break;
                }
                SetEscortPaused(true);
                m_creature->SummonCreature(NPC_SPOTLIGHT, 0, 0, 0, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
                break;
            case 8:
                m_pInstance->DoUseDoorOrButton(GO_STAGE_DOOR_LEFT);
                break;
            case 9:
                m_pInstance->DoPrepareOperaStage(m_creature);
                break;
        }
    }

    void JustDidDialogueStep(int32 iEntry)
    {
        switch (iEntry)
        {
            case OPERA_EVENT_WIZARD_OZ:
            case OPERA_EVENT_RED_RIDING_HOOD:
            case OPERA_EVENT_ROMULO_AND_JUL:
                // Despawn spotlight and resume escort
                if (Creature* pSpotlight = m_creature->GetMap()->GetCreature(m_spotlightGuid))
                    pSpotlight->ForcedDespawn();
                SetEscortPaused(false);
                break;
        }
    }

    void UpdateEscortAI(const uint32 uiDiff) { DialogueUpdate(uiDiff); }
};

CreatureAI* GetAI_npc_barnesAI(Creature* pCreature)
{
    return new npc_barnesAI(pCreature);
}

bool GossipHello_npc_barnes(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        // Check if opera event is not yet in progress
        if (pInstance->GetData(TYPE_OPERA) == IN_PROGRESS || pInstance->GetData(TYPE_OPERA) == DONE)
            return true;

        // Check for death of Moroes
        if (pInstance->GetData(TYPE_MOROES) == DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_OPERA_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            // for GMs we add the possibility to change the event
            if (pPlayer->isGameMaster())
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[GM] Change event to EVENT_OZ",   GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[GM] Change event to EVENT_HOOD", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[GM] Change event to EVENT_RAJ",  GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            }

            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_OPERA_1, pCreature->GetObjectGuid());

            return true;
        }
    }

    return true;
}

bool GossipSelect_npc_barnes(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_OPERA_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_OPERA_2, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            DoScriptText(SAY_BARNES_EVENT_START, pCreature);
            // start the stage escort
            if (npc_barnesAI* pBarnesAI = dynamic_cast<npc_barnesAI*>(pCreature->AI()))
                pBarnesAI->Start(false, NULL, NULL, true);
            break;
        // GM gossip options
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->CLOSE_GOSSIP_MENU();
            if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
            {
                pInstance->SetData(TYPE_OPERA_PERFORMANCE, OPERA_EVENT_WIZARD_OZ);
                outstring_log("SD2: %s manually set Opera event to EVENT_OZ", pPlayer->GetGuidStr().c_str());
            }
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->CLOSE_GOSSIP_MENU();
            if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
            {
                pInstance->SetData(TYPE_OPERA_PERFORMANCE, OPERA_EVENT_RED_RIDING_HOOD);
                outstring_log("SD2: %s manually set Opera event to EVENT_HOOD", pPlayer->GetGuidStr().c_str());
            }
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->CLOSE_GOSSIP_MENU();
            if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
            {
                pInstance->SetData(TYPE_OPERA_PERFORMANCE, OPERA_EVENT_ROMULO_AND_JUL);
                outstring_log("SD2: %s manually set Opera event to EVENT_RAJ", pPlayer->GetGuidStr().c_str());
            }
            break;
    }

    return true;
}

/*###
# npc_berthold
####*/

enum
{
    GOSSIP_ITEM_TELEPORT        = -3532000,

    SPELL_TELEPORT              = 39567
};

bool GossipHello_npc_berthold(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        // Check if Shade of Aran event is done
        if (pInstance->GetData(TYPE_ARAN) == DONE)
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELEPORT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_berthold(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT, true);

    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

/*######
# npc_image_of_medivh
######*/

enum
{
    // yells
    SAY_MEDIVH_1            = -1532116,
    SAY_ARCANAGOS_2         = -1532117,
    SAY_MEDIVH_3            = -1532118,
    SAY_ARCANAGOS_4         = -1532119,
    SAY_MEDIVH_5            = -1532120,
    SAY_ARCANAGOS_6         = -1532121,
    EMOTE_CAST_SPELL        = -1532122,
    SAY_ARCANAGOS_7         = -1532123,
    SAY_MEDIVH_8            = -1532124,

    // spells
    // Arcanagos
    SPELL_NOTIFY_FLEE       = 30985,
    SPELL_PREPARE_FIREBALL  = 30970,
    SPELL_REFLECTION        = 30969,
    SPELL_SHOOT_FIREBALL    = 30968,
    SPELL_FIREBALL_REFLECT  = 30971,

    // Medivh
    SPELL_FROST_BREATH      = 30974,
    SPELL_CONFLAG_BLAST     = 30977,
    SPELL_EVOCATION         = 30972,
    SPELL_FIREBALL          = 30967,
    SPELL_FLY_TO_DEATH      = 30936,
    SPELL_MANA_SHIELD       = 30973,

    //NPC_ARCANAGOS_CREDIT  = 17665,            // purpose unk

    POINT_ID_INTRO          = 1,
    POINT_ID_DESPAWN        = 2,
};

// Note: all coords are guesswork
static const float afMedivhSpawnLoc[4] = {-11153.18f, -1889.65f, 91.47f, 2.07f};
static const float afArcanagosSpawnLoc[4] = {-11242.66f, -1778.55f, 125.35f};
static const float afArcanagosMoveLoc[3] = {-11170.28f, -1865.09f, 125.35f};

static const DialogueEntry aMedivhDialogue[] =
{
    // ToDo:
    {0, 0, 0},
};

struct MANGOS_DLL_DECL npc_image_of_medivhAI : public ScriptedAI, private DialogueHelper
{
    npc_image_of_medivhAI(Creature* pCreature) : ScriptedAI(pCreature),
        DialogueHelper(aMedivhDialogue)
    {
        m_pInstance  = (instance_karazhan*)pCreature->GetInstanceData();
        InitializeDialogueHelper(m_pInstance);
        Reset();
    }

    instance_karazhan* m_pInstance;

    void Reset()
    {
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_IMAGE_OF_ARCANAGOS)
        {
            pSummoned->SetLevitate(true);
            pSummoned->SetWalk(false);
            pSummoned->GetMotionMaster()->MovePoint(1, afArcanagosMoveLoc[0], afArcanagosMoveLoc[1], afArcanagosMoveLoc[2]);
            pSummoned->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE || pSummoned->GetEntry() != NPC_IMAGE_OF_ARCANAGOS)
            return;

        switch (uiPointId)
        {
            case POINT_ID_INTRO:
                // ToDo: start dialogue here
                break;
            case POINT_ID_DESPAWN:
                break;
        }
    }

    void JustDidDialogueStep(int32 iEntry)
    {
        // ToDo:
    }

    void UpdateAI(const uint32 uiDiff) { DialogueUpdate(uiDiff); }
};

CreatureAI* GetAI_npc_image_of_medivhAI(Creature* pCreature)
{
    return new npc_image_of_medivhAI(pCreature);
}

bool ProcessEventId_event_spell_medivh_journal(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        // Summon Medivh and Arcanagos
        if (Creature* pMedivh = ((Player*)pSource)->SummonCreature(NPC_IMAGE_OF_MEDIVH, afMedivhSpawnLoc[0], afMedivhSpawnLoc[1], afMedivhSpawnLoc[2], afMedivhSpawnLoc[3], TEMPSUMMON_DEAD_DESPAWN, 0))
            pMedivh->SummonCreature(NPC_IMAGE_OF_ARCANAGOS, afArcanagosSpawnLoc[0], afArcanagosSpawnLoc[1], afArcanagosSpawnLoc[2], afArcanagosSpawnLoc[2], TEMPSUMMON_DEAD_DESPAWN, 0);
    }

    return true;
}

void AddSC_karazhan()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_barnes";
    pNewScript->GetAI = &GetAI_npc_barnesAI;
    pNewScript->pGossipHello = &GossipHello_npc_barnes;
    pNewScript->pGossipSelect = &GossipSelect_npc_barnes;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_berthold";
    pNewScript->pGossipHello = &GossipHello_npc_berthold;
    pNewScript->pGossipSelect = &GossipSelect_npc_berthold;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_image_of_medivh";
    pNewScript->GetAI = &GetAI_npc_image_of_medivhAI;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_spell_medivh_journal";
    pNewScript->pProcessEventId = &ProcessEventId_event_spell_medivh_journal;
    pNewScript->RegisterSelf();
}
