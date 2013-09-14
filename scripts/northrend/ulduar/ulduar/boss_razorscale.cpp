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
SDName: boss_razorscale
SD%Complete: 20%
SDComment: Intro and basic script
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_INTRO_WELCOME                   = -1603036,
    SAY_INTRO_1                         = -1603037,
    SAY_INTRO_2                         = -1603038,
    SAY_INTRO_3                         = -1603039,
    SAY_GROUNDED                        = -1603040,
    SAY_EXTINGUISH_FIRE                 = -1603041,

    EMOTE_BREATH                        = -1603042,
    EMOTE_HARPOON_READY                 = -1603043,
    EMOTE_GROUNDED                      = -1603044,

    // general spells (used in both ground and air phases)
    SPELL_BERSERK                       = 47008,
    SPELL_DEVOURING_FLAME               = 63236,
    SPELL_FLAME_BREATH                  = 63317,
    SPELL_FLAME_BREATH_H                = 64021,

    // razorscale air phase spells
    SPELL_FIREBALL                      = 62796,
    SPELL_FIREBALL_H                    = 63815,
    SPELL_STUN                          = 62794,
    // SPELL_SHACKLE                    = 62646,            // channeled on Razorscale intro by npc 33259; currently not used because of missing intro data

    // phase 2 transition spells
    SPELL_WING_BUFFET                   = 62666,
    SPELL_FIREBOLT                      = 62669,            // target npc 33282 - destroy the harpoons
    SPELL_HARPOON_FIRE                  = 62696,            // visual when harpoons are destroyed, cast by 33282

    // ground spells
    SPELL_FLAME_BUFFET                  = 64016,
    SPELL_FLAME_BUFFET_H                = 64023,
    SPELL_FUSE_ARMOR                    = 64771,

    // summoned spells
    SPELL_DEVOURING_FLAME_AURA          = 64709,
    SPELL_DEVOURING_FLAME_AURA_H        = 64734,

    // razorscale spawner spells
    // controlled by some dummy spells: 63114, 63115, 63116, 63968, 63969, 63970
    SPELL_SUMMON_DWARF_WATCHER          = 63135,            // summons npc 33453
    SPELL_SUMMON_DWARF_GUARDIAN         = 62926,            // summons npc 33388
    SPELL_SUMMON_IRON_VRYKUL            = 63798,            // summons npc 33846
    SPELL_SUMMON_MOLE_MACHINE           = 62899,            // summons go 194316

    // NPC ids
    NPC_RAZORSCALE_HARPOON              = 33282,            // harpoon visual dummy for phase 2 transition
    NPC_RAZORSCALE_CONTROLLER           = 33233,            // harpoon use npc in phase 1

    // summoned npcs
    NPC_DEVOURING_FLAME                 = 34188,
    NPC_DARK_RUNE_WATCHER               = 33453,
    NPC_DARK_RUNE_GUARDIAN              = 33388,
    NPC_DARK_RUNE_SENTINEL              = 33846,

    // harpoons
    GO_HARPOON                          = 194565,           // broken harpoon - for display only
    GO_HARPOON_GUN                      = 194542,           // usable harpoon - respawns when the broken one is repaired

    // gossip
    GOSSIP_ITEM_START_RAZORSCALE        = -3603009,
    GOSSIP_MENU_ID_WELCOME              = 14317,
};

static const DialogueEntry aIntroDialogue[] =
{
    {NPC_EXPEDITION_ENGINEER,   0,                          3000},
    {SAY_INTRO_2,               NPC_EXPEDITION_COMMANDER,   4000},
    {NPC_RAZORSCALE,            0,                          25000},
    {NPC_EXPEDITION_DEFENDER,   0,                          0},
    {0, 0, 0},
};

/*######
## boss_razorscale
######*/

struct MANGOS_DLL_DECL boss_razorscaleAI : public ScriptedAI
{
    boss_razorscaleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        m_creature->GetMotionMaster()->MoveRandomAroundPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 10.0f);
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBerserkTimer;

    void Reset() override
    {
        m_uiBerserkTimer = 10 * MINUTE * IN_MILLISECONDS;

        // no combat movement in phase 1
        SetCombatMovement(false);
    }

    void JustDied(Unit* pKiller) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORSCALE, DONE);
    }

    void Aggro(Unit* pWho) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORSCALE, IN_PROGRESS);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORSCALE, FAIL);

        m_creature->GetMotionMaster()->MoveRandomAroundPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 10.0f);
    }

    void JustSummoned(Creature* pSummoned) override
    {
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                    m_uiBerserkTimer = 0;
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_razorscale(Creature* pCreature)
{
    return new boss_razorscaleAI(pCreature);
}

/*######
## npc_expedition_commander
######*/

struct MANGOS_DLL_DECL npc_expedition_commanderAI : public ScriptedAI, private DialogueHelper
{
    npc_expedition_commanderAI(Creature* pCreature) : ScriptedAI(pCreature),
        DialogueHelper(aIntroDialogue)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        InitializeDialogueHelper(m_pInstance);
        m_bIntroDone = false;
        Reset();
    }

    instance_ulduar* m_pInstance;

    bool m_bIntroDone;

    void Reset() override { }

    void MoveInLineOfSight(Unit* pWho) override
    {
        // ToDo: verify if all this is correct. There may other parts of the intro which are currently missing
        if (!m_bIntroDone && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 20.0f))
        {
            DoScriptText(SAY_INTRO_WELCOME, m_creature);
            m_bIntroDone = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void JustDidDialogueStep(int32 iEntry) override
    {
        if (!m_pInstance)
        {
            script_error_log("Instance Ulduar: ERROR Failed to load instance data for this instace.");
            return;
        }

        switch (iEntry)
        {
            case NPC_EXPEDITION_ENGINEER:
                if (Creature* pEngineer = GetClosestCreatureWithEntry(m_creature, NPC_EXPEDITION_ENGINEER, 15.0f))
                    DoScriptText(SAY_INTRO_1, pEngineer);
                // ToDo: move the defenders to fight the incoming dwarfes
                break;
            case NPC_RAZORSCALE:
                // ToDo: set Razorscale in combat at this point
                break;
            case NPC_EXPEDITION_DEFENDER:
                if (Creature* pEngineer = GetClosestCreatureWithEntry(m_creature, NPC_EXPEDITION_ENGINEER, 15.0f))
                    DoScriptText(SAY_INTRO_3, pEngineer);
                // ToDo: make the engineers move and repair the Harpoon Turret
                break;
        }
    }

    void ReceiveAIEvent(AIEventType eventType, Creature* /*pSender*/, Unit* pInvoker, uint32 /*uiMiscValue*/) override
    {
        // start intro dialogue
        if (eventType == AI_EVENT_CUSTOM_A && pInvoker->GetTypeId() == TYPEID_PLAYER)
            StartNextDialogueText(NPC_EXPEDITION_ENGINEER);
    }

    void UpdateAI(const uint32 uiDiff) override { DialogueUpdate(uiDiff); }
};

CreatureAI* GetAI_npc_expedition_commander(Creature* pCreature)
{
    return new npc_expedition_commanderAI(pCreature);
}

bool GossipHello_npc_expedition_commander(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_RAZORSCALE) == NOT_STARTED || pInstance->GetData(TYPE_RAZORSCALE) == FAIL)
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START_RAZORSCALE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ID_WELCOME, pCreature->GetObjectGuid());
        return true;
    }

    return false;
}

bool GossipSelect_npc_expedition_commander(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        // start intro dialogue
        pCreature->AI()->SendAIEvent(AI_EVENT_CUSTOM_A, pPlayer, pCreature);
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

void AddSC_boss_razorscale()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_razorscale";
    pNewScript->GetAI = GetAI_boss_razorscale;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_expedition_commander";
    pNewScript->GetAI = &GetAI_npc_expedition_commander;
    pNewScript->pGossipHello = GossipHello_npc_expedition_commander;
    pNewScript->pGossipSelect = GossipSelect_npc_expedition_commander;
    pNewScript->RegisterSelf();
}
