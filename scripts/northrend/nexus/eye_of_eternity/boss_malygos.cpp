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
SD%Complete: 10
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
    SAY_OUTRO_1             = -1616029,
    SAY_OUTRO_2             = -1616030,
    SAY_OUTRO_3             = -1616031,
    SAY_OUTRO_4             = -1616032,

    SAY_EMOTE_SPARK         = -1616033,
    SAY_EMOTE_BREATH        = -1616034,
};

/*######
## boss_malygos
######*/

struct MANGOS_DLL_DECL boss_malygosAI : public ScriptedAI
{
    boss_malygosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_eye_of_eternity*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_eye_of_eternity* m_pInstance;
    bool m_bIsRegularMode;

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

    void KilledUnit(Unit* pVictim)
    {
        // ToDo: add kill yells, based on encounter phase
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

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

    void UpdateAI(const uint32 uiDiff)
    {
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
