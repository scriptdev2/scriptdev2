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
SDName: boss_algalon
SD%Complete: 20%
SDComment: Basic script only.
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_INTRO_1                         = -1603106,
    SAY_INTRO_2                         = -1603107,
    SAY_INTRO_3                         = -1603108,

    SAY_ENGAGE                          = -1603109,
    SAY_AGGRO                           = -1603110,
    SAY_SLAY_1                          = -1603111,
    SAY_SLAY_2                          = -1603112,
    SAY_SUMMON_STAR                     = -1603113,
    SAY_BIG_BANG_1                      = -1603114,
    SAY_BIG_BANG_2                      = -1603115,
    SAY_PHASE_2                         = -1603116,
    SAY_BERSERK                         = -1603117,

    SAY_DESPAWN_1                       = -1603118,
    SAY_DESPAWN_2                       = -1603119,
    SAY_DESPAWN_3                       = -1603120,

    SAY_OUTRO_1                         = -1603121,
    SAY_OUTRO_2                         = -1603122,
    SAY_OUTRO_3                         = -1603123,
    SAY_OUTRO_4                         = -1603124,
    SAY_OUTRO_5                         = -1603125,
    SAY_BRANN_OUTRO                     = -1603246,

    // intro spells
    SPELL_ARRIVAL                       = 64997,                // intro animation spells
    SPELL_RIDE_LIGHTNING                = 64986,
    SPELL_SUMMON_AZEROTH                = 64994,                // summons npc 34246
    SPELL_REORIGINATION                 = 64996,                // channeled animation on Azeroth globe

    // generic spells
    // SPELL_ALGALON_EVENT_BEAM         = 64367,                // puspose unk
    // SPELL_ALGALON_EVENT_CLIMAX       = 64580,
    SPELL_KILL_CREDIT                   = 65184,                // achiev check spell
    SPELL_SUPERMASSIVE_FAIL             = 65311,                // related to the Supermassive achievements
    SPELL_BERSERK                       = 47008,
    SPELL_ASCEND_HEAVENS                = 64487,                // cast when time's up
    SPELL_TELEPORT                      = 62940,

    // combat spells
    SPELL_QUANTUM_STRIKE                = 64395,
    SPELL_QUANTUM_STRIKE_H              = 64592,
    SPELL_PHASE_PUNCH                   = 64412,
    SPELL_PHASE_PUNCH_SHIFT             = 64417,
    SPELL_COSMIC_SMASH                  = 62301,                // triggers the spells which summon 33104 and 33105
    SPELL_COSMIC_SMASH_H                = 64598,
    SPELL_BIG_BANG                      = 64443,                // removes players from phase
    SPELL_BIG_BANG_H                    = 64584,

    // summoned spells
    SPELL_CONSTELATION_PHASE            = 65508,                // shift phase - use unk
    SPELL_BLACK_HOLE_CREDIT             = 65312,
    SPELL_COSMIC_SMASH_MISSILE          = 62304,                // cast by Algalon asteroid target - 33105
    SPELL_CONSTELLATION_TRIGGER         = 65508,
    SPELL_COLLAPSE                      = 62018,                // cast by Collapsing stars
    SPELL_BLACK_HOLE_EXPLOSION          = 64122,
    SPELL_BLACK_HOLE_EXPLOSION_H        = 65108,
    SPELL_BLACK_HOLE_DESPAWN            = 64391,
    SPELL_SUMMON_BLACK_HOLE             = 62189,
    SPELL_BLACK_HOLE_VISUAL             = 64135,
    SPELL_BLACK_HOLE_SPAWN              = 62003,
    SPELL_BLACK_HOLE_TRIGG              = 62185,
    SPELL_BLACK_HOLE_DMG                = 62169,
    SPELL_ARCANE_BARRAGE                = 64599,
    SPELL_ARCANE_BARRAGE_H              = 64607,

    // npcs
    NPC_AZEROTH                         = 34246,
    NPC_ASTEROID_STALKER_1              = 33104,
    NPC_ASTEROID_STALKER_2              = 33105,
    NPC_LIVING_CONSTELLATION            = 33052,
    NPC_COLLAPSING_STAR                 = 32955,
    NPC_DARK_MATTER                     = 33089,
    NPC_UNLEASHED_DARK_MATTER           = 34097,
    NPC_BLACK_HOLE                      = 32953,

    // other
    FACTION_ID_FRIENDLY                 = 35,
};

static const DialogueEntry aAlgalonDialogue[] =
{
    {SAY_INTRO_1,           NPC_ALGALON,        6000},
    {SPELL_SUMMON_AZEROTH,  0,                  5000},
    {SAY_INTRO_2,           NPC_ALGALON,        8000},
    {SAY_INTRO_3,           NPC_ALGALON,        0},
    {SAY_AGGRO,             NPC_ALGALON,        16000},
    {SAY_ENGAGE,            NPC_ALGALON,        0},
    {SPELL_ASCEND_HEAVENS,  0,                  3000},
    {SPELL_BERSERK,         0,                  0},
    {SAY_DESPAWN_1,         NPC_ALGALON,        15000},
    {SAY_DESPAWN_2,         NPC_ALGALON,        8000},
    {SAY_DESPAWN_3,         NPC_ALGALON,        10000},
    {SPELL_TELEPORT,        0,                  0},
    {NPC_ALGALON,           0,                  12000},
    {SAY_OUTRO_1,           NPC_ALGALON,        39000},
    {SAY_OUTRO_2,           NPC_ALGALON,        18000},
    {SAY_OUTRO_3,           NPC_ALGALON,        12000},
    {SAY_OUTRO_4,           NPC_ALGALON,        12000},
    {SAY_BRANN_OUTRO,       NPC_BRANN_ALGALON,  11000},
    {SAY_OUTRO_5,           NPC_ALGALON,        0},
    {0, 0, 0},
};

/*######
## boss_algalon
######*/

struct MANGOS_DLL_DECL boss_algalonAI : public ScriptedAI, private DialogueHelper
{
    boss_algalonAI(Creature* pCreature) : ScriptedAI(pCreature),
        DialogueHelper(aAlgalonDialogue)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        InitializeDialogueHelper(m_pInstance);
        m_bEventFinished = false;
        Reset();

        // start intro event on first spawn
        if (pCreature->GetPositionZ() > 450.0f)
            DoStartIntroEvent();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bEventFinished;

    uint32 m_uiBerserkTimer;

    void Reset() override
    {
        m_uiBerserkTimer = 6 * MINUTE * IN_MILLISECONDS;
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void Aggro(Unit* pWho) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ALGALON, IN_PROGRESS);

        StartNextDialogueText(SAY_AGGRO);
    }

    void DamageTaken(Unit* /*pDoneBy*/, uint32& uiDamage) override
    {
        if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;

            if (!m_bEventFinished)
            {
                if (m_pInstance)
                    m_pInstance->SetData(TYPE_ALGALON, DONE);

                DoCastSpellIfCan(m_creature, SPELL_KILL_CREDIT, CAST_TRIGGERED | CAST_INTERRUPT_PREVIOUS);
                DoCastSpellIfCan(m_creature, SPELL_SUPERMASSIVE_FAIL, CAST_TRIGGERED | CAST_INTERRUPT_PREVIOUS);

                m_creature->setFaction(FACTION_ID_FRIENDLY);
                m_bEventFinished = true;
                EnterEvadeMode();
            }
        }
    }

    void JustReachedHome() override
    {
        if (!m_pInstance)
            return;

        if (m_bEventFinished)
        {
            if (m_pInstance->GetData(TYPE_ALGALON) == DONE)
                StartNextDialogueText(NPC_ALGALON);
            else
                StartNextDialogueText(SAY_DESPAWN_1);
        }
        else
            m_pInstance->SetData(TYPE_ALGALON, FAIL);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        switch (pSummoned->GetEntry())
        {
            // move Brann to the center of the platform (and override pathfinding because of missing GO support)
            case NPC_BRANN_ALGALON:
                pSummoned->SetWalk(false);
                pSummoned->GetMotionMaster()->MovePoint(0, 1631.986f, -297.7831f, 417.321f, false);
                break;
            case NPC_AZEROTH:
                pSummoned->ForcedDespawn(30000);
                break;
        }
    }

    void MovementInform(uint32 uiMoveType, uint32 uiPointId) override
    {
        if (uiMoveType != POINT_MOTION_TYPE || !uiPointId)
            return;

        // start intro and reset home position
        StartNextDialogueText(SAY_INTRO_1);
        m_creature->SetLevitate(false);
        m_creature->RemoveAurasDueToSpell(SPELL_RIDE_LIGHTNING);
        m_creature->SetRespawnCoord(afAlgalonMovePos[0], afAlgalonMovePos[1], afAlgalonMovePos[2], afAlgalonMovePos[3]);
    }

    void JustDidDialogueStep(int32 iEntry) override
    {
        if (!m_pInstance)
            return;

        switch (iEntry)
        {
            case SPELL_SUMMON_AZEROTH:
                DoCastSpellIfCan(m_creature, SPELL_SUMMON_AZEROTH, CAST_TRIGGERED);
                break;
            case SAY_INTRO_2:
                DoCastSpellIfCan(m_creature, SPELL_REORIGINATION);
                break;
            case SAY_ENGAGE:
                // ToDo: change environment
                break;
            case SPELL_BERSERK:
                EnterEvadeMode();
                break;
            case SPELL_TELEPORT:
                DoCastSpellIfCan(m_creature, SPELL_TELEPORT, CAST_TRIGGERED);
                m_creature->ForcedDespawn(2000);
                break;
            case NPC_ALGALON:
                m_creature->SummonCreature(NPC_BRANN_ALGALON, 1631.962f, -208.6464f, 420.8867f, 4.71f, TEMPSUMMON_DEAD_DESPAWN, 0);
                break;
            case SAY_OUTRO_1:
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                break;
        }
    }

    void ReceiveAIEvent(AIEventType eventType, Creature* /*pSender*/, Unit* pInvoker, uint32 /*uiMiscValue*/) override
    {
        // notify boss that time is over
        // this will trigger the wipe spell and make the boss evade and finally despawn
        if (eventType == AI_EVENT_CUSTOM_A)
        {
            m_bEventFinished = true;
            DoCastSpellIfCan(m_creature, SPELL_ASCEND_HEAVENS, CAST_INTERRUPT_PREVIOUS);
            StartNextDialogueText(SPELL_ASCEND_HEAVENS);
        }
    }

    // function to start the intro part on first spawn
    void DoStartIntroEvent()
    {
        m_creature->SetLevitate(true);
        DoCastSpellIfCan(m_creature, SPELL_ARRIVAL, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_RIDE_LIGHTNING, CAST_TRIGGERED);
        m_creature->GetMotionMaster()->MovePoint(1, afAlgalonMovePos[0], afAlgalonMovePos[1], afAlgalonMovePos[2]);

        // start timer event
        if (m_pInstance)
        {
            m_pInstance->DoUpdateWorldState(WORLD_STATE_TIMER, 1);
            m_pInstance->SetData(TYPE_ALGALON_TIMER, 60);
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        DialogueUpdate(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    m_uiBerserkTimer = 0;
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_algalon(Creature* pCreature)
{
    return new boss_algalonAI(pCreature);
}

/*######
## go_celestial_access
######*/

bool GOUse_go_celestial_access(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();
    if (!pInstance)
        return true;

    if (pInstance->GetData(TYPE_ALGALON) != NOT_STARTED)
        return true;

    // Set instance data and allow DB scripts to continue the event
    pInstance->SetData(TYPE_ALGALON, SPECIAL);
    return false;
}

void AddSC_boss_algalon()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_algalon";
    pNewScript->GetAI = GetAI_boss_algalon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_celestial_access";
    pNewScript->pGOUse = &GOUse_go_celestial_access;
    pNewScript->RegisterSelf();
}
