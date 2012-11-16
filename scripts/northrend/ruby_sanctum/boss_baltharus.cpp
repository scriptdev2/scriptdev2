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
SDName: boss_baltharus
SD%Complete: 20
SDComment: only yells
SDCategory: Ruby Sanctum
EndScriptData */

#include "precompiled.h"
#include "ruby_sanctum.h"

enum
{
    // Xerestrasza intro and outro texts
    SAY_HELP                    = -1724000,
    SAY_INTRO                   = -1724001,

    SAY_THANKS                  = -1724002,
    SAY_OUTRO_1                 = -1724003,
    SAY_OUTRO_2                 = -1724004,
    SAY_OUTRO_3                 = -1724005,
    SAY_OUTRO_4                 = -1724006,
    SAY_OUTRO_5                 = -1724007,
    SAY_OUTRO_6                 = -1724008,
    SAY_OUTRO_7                 = -1724009,

    // Baltharus texts
    SAY_AGGRO                   = -1724010,
    SAY_SLAY_1                  = -1724011,
    SAY_SLAY_2                  = -1724012,
    SAY_DEATH                   = -1724013,
    SAY_SPLIT                   = -1724014,

    SPELL_BARRIER_CHANNEL       = 76221,            // channeled on the tree
};

static const DialogueEntry aIntroDialogue[] =
{
    {SAY_HELP,  NPC_XERESTRASZA,  7000},
    {SAY_INTRO, NPC_BALTHARUS,    0},
    {0, 0, 0},
};

struct MANGOS_DLL_DECL boss_baltharusAI : public ScriptedAI, private DialogueHelper
{
    boss_baltharusAI(Creature* pCreature) : ScriptedAI(pCreature),
        DialogueHelper(aIntroDialogue)
    {
        m_pInstance = (instance_ruby_sanctum*)pCreature->GetInstanceData();
        InitializeDialogueHelper(m_pInstance);
        m_bHasDoneIntro = false;
        Reset();
    }

    instance_ruby_sanctum* m_pInstance;

    bool m_bHasDoneIntro;

    void Reset() override
    {
    }

    void Aggro(Unit* pWho) override
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BALTHARUS, IN_PROGRESS);
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        if (!m_bHasDoneIntro && pWho->GetTypeId() == TYPEID_PLAYER && !((Player*)pWho)->isGameMaster())
        {
            StartNextDialogueText(SAY_HELP);
            m_bHasDoneIntro = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller) override
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BALTHARUS, DONE);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BALTHARUS, FAIL);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        DialogueUpdate(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_baltharus(Creature* pCreature)
{
    return new boss_baltharusAI(pCreature);
}

void AddSC_boss_baltharus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_baltharus";
    pNewScript->GetAI = &GetAI_boss_baltharus;
    pNewScript->RegisterSelf();
}
