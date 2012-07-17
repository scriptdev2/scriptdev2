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
SDName: boss_malygos
SD%Complete: 20
SDComment: Basic script
SDCategory: Eye of Eternity
EndScriptData */

#include "precompiled.h"
#include "eye_of_eternity.h"

enum
{
    SAY_INTRO_1             = -1616000,
    SAY_INTRO_2             = -1616001,
    SAY_INTRO_3             = -1616002,
    SAY_INTRO_4             = -1616003,
    SAY_INTRO_5             = -1616004,
    SAY_AGGRO               = -1616005,
    SAY_VORTEX              = -1616006,
    SAY_SPARK_BUFF          = -1616007,
    SAY_SLAY_1_A            = -1616008,
    SAY_SLAY_1_B            = -1616009,
    SAY_SLAY_1_C            = -1616010,
    SAY_END_PHASE_1         = -1616011,
    SAY_START_PHASE_2       = -1616012,
    SAY_DEEP_BREATH         = -1616013,
    SAY_SHELL               = -1616014,
    SAY_SLAY_2_A            = -1616015,
    SAY_SLAY_2_B            = -1616016,
    SAY_SLAY_2_C            = -1616017,
    SAY_END_PHASE_2         = -1616018,
    SAY_INTRO_PHASE_3       = -1616019,
    SAY_START_PHASE_3       = -1616020,
    SAY_SLAY_3_A            = -1616021,
    SAY_SLAY_3_B            = -1616022,
    SAY_SLAY_3_C            = -1616023,
    SAY_SURGE               = -1616024,
    SAY_SPELL_1             = -1616025,
    SAY_SPELL_2             = -1616026,
    SAY_SPELL_3             = -1616027,
    SAY_DEATH               = -1616028,

    SAY_EMOTE_SPARK         = -1616033,
    SAY_EMOTE_BREATH        = -1616034,

    // phase 1 spells
    SPELL_BERSERK                   = 26662,
    SPELL_ARCANE_BREATH             = 56272,
    SPELL_ARCANE_BREATH_H           = 60072,
    SPELL_SUMMON_SPARK              = 56140,
    SPELL_VORTEX                    = 56105,

    // phase 2 spells
    SPELL_ARCANE_STORM              = 57459,            // related to spells 57473 and 61693
    SPELL_ARCANE_STORM_H            = 61694,
    SPELL_SUMMON_ARCANE_BOMB        = 56429,
    SPELL_ARCANE_PULSE              = 57432,

    // transition spells
    SPELL_DESTROY_PLATFORM_PRE      = 58842,
    SPELL_DESTROY_PLATFORM_BOOM     = 59084,
    SPELL_DESTROY_PLATFORM_EVENT    = 59099,
    SPELL_SUMMON_RED_DRAGON         = 58846,
    SPELL_RIDE_RED_DRAGON           = 56072,

    // phase 3 spells
    SPELL_STATIC_FIELD_SUMMON       = 57430,
    SPELL_SURGE_OF_POWER            = 56505,
    SPELL_SURGE_OF_POWER_DUMMY      = 60939,

    // power spark
    SPELL_POWER_SPARK_MALYGOS       = 56152,
    SPELL_POWER_SPARK_PLAYERS       = 55852,
    SPELL_POWER_SPARK_VISUAL        = 55845,

    // vortex - thse spells require additional research
    // related auras: 55853, 55883, 56263, 56264, 56265, 56266, 59666, 61071, 61072, 61073, 61074, 61075
    SPELL_VORTEX_SPAWN              = 59670,
    SPELL_VORTEX_VISUAL             = 55873,
    SPELL_VORTEX_CHANNEL            = 56237,

    // arcane overload
    SPELL_ARCANE_OVERLOAD           = 56432,

    // static field
    SPELL_STATIC_FIELD              = 57428,

    // summoned npcs
    NPC_VORTEX                      = 30090,
    NPC_POWER_SPARK                 = 30084,

    NPC_HOVER_DISC                  = 30248,
    NPC_NEXUS_LORD                  = 30245,
    NPC_SCION_OF_ETERNITY           = 30249,
    NPC_ARCANE_OVERLOAD             = 30282,

    NPC_STATIC_FIELD                = 30592,
};

static const DialogueEntry aIntroDialogue[] =
{
    {SAY_INTRO_1,   NPC_MALYGOS,    11000},
    {SAY_INTRO_2,   NPC_MALYGOS,    13000},
    {SAY_INTRO_3,   NPC_MALYGOS,    14000},
    {SAY_INTRO_4,   NPC_MALYGOS,    12000},
    {SAY_INTRO_5,   NPC_MALYGOS,    0},
    {0, 0, 0},
};

static const float aAlextraszaSpawnPos[4] = {700.354f, 1310.718f, 298.13f, 6.02f};
static const float aAlextraszaMovePos[3] = {726.754f, 1307.259f, 282.679f};

/*######
## boss_malygos
######*/

struct MANGOS_DLL_DECL boss_malygosAI : public ScriptedAI, private DialogueHelper
{
    boss_malygosAI(Creature* pCreature) : ScriptedAI(pCreature),
        DialogueHelper(aIntroDialogue)
    {
        m_pInstance = (instance_eye_of_eternity*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        InitializeDialogueHelper(m_pInstance);
        m_bHasDoneIntro = false;
        Reset();
    }

    instance_eye_of_eternity* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bHasDoneIntro;

    void Reset()
    {
        SetCombatMovement(false);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALYGOS, IN_PROGRESS);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasDoneIntro && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 110.0f))
        {
            StartNextDialogueText(SAY_INTRO_1);
            m_bHasDoneIntro = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void KilledUnit(Unit* pVictim)
    {
        // ToDo: add kill yells, based on encounter phase
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        m_creature->SummonCreature(NPC_ALEXSTRASZA, aAlextraszaSpawnPos[0], aAlextraszaSpawnPos[1], aAlextraszaSpawnPos[2], aAlextraszaSpawnPos[3], TEMPSUMMON_TIMED_DESPAWN, 5*MINUTE*IN_MILLISECONDS);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALYGOS, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALYGOS, FAIL);
    }

    void MovementInform(uint32 uiMoveType, uint32 uiPointId)
    {
        if (uiMoveType != POINT_MOTION_TYPE)
            return;

        if (uiPointId)
        {
            m_creature->SetLevitate(false);
            SetCombatMovement(true);
            DoStartMovement(m_creature->getVictim());
            m_creature->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_ALEXSTRASZA)
        {
            pSummoned->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
            pSummoned->GetMotionMaster()->MovePoint(0, aAlextraszaMovePos[0], aAlextraszaMovePos[1], aAlextraszaMovePos[2]);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        DialogueUpdate(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_malygos(Creature* pCreature)
{
    return new boss_malygosAI(pCreature);
}

bool ProcessEventId_event_go_focusing_iris(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (instance_eye_of_eternity* pInstance = (instance_eye_of_eternity*)((Creature*)pSource)->GetInstanceData())
    {
        if (pSource->GetTypeId() != TYPEID_PLAYER)
            return false;

        if (pInstance->GetData(TYPE_MALYGOS) == IN_PROGRESS || pInstance->GetData(TYPE_MALYGOS) == DONE)
            return false;

        Creature* pMalygos = pInstance->GetSingleCreatureFromStorage(NPC_MALYGOS);
        Creature* pTrigger = pInstance->GetSingleCreatureFromStorage(NPC_LARGE_TRIGGER);
        if (!pMalygos || !pTrigger)
            return false;

        // Enter combat area
        float fX, fY, fZ;
        pTrigger->GetNearPoint(pTrigger, fX, fY, fZ, 0, 30.0f, pTrigger->GetAngle(pMalygos));
        pMalygos->GetMotionMaster()->MovePoint(1, fX, fY, fZ);
        pMalygos->AI()->AttackStart((Player*)pSource);

        return true;
    }
    return false;
}

void AddSC_boss_malygos()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_malygos";
    pNewScript->GetAI = &GetAI_boss_malygos;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_go_focusing_iris";
    pNewScript->pProcessEventId = &ProcessEventId_event_go_focusing_iris;
    pNewScript->RegisterSelf();
}
