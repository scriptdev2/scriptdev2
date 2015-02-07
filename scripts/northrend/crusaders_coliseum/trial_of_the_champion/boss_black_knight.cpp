/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
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
SDName: boss_black_knight
SD%Complete: 20
SDComment: Basic script only
SDCategory: Crusader Coliseum, Trial of the Champion
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_champion.h"

enum
{
    SPELL_KILL_CREDIT               = 68663,
};

/*######
## boss_black_knight
######*/

struct boss_black_knightAI : public ScriptedAI
{
    boss_black_knightAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_champion*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_trial_of_the_champion* m_pInstance;
    bool m_bIsRegularMode;

    void Reset() override
    {
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLACK_KNIGHT, IN_PROGRESS);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLACK_KNIGHT, FAIL);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLACK_KNIGHT, DONE);

        DoCastSpellIfCan(m_creature, SPELL_KILL_CREDIT, CAST_TRIGGERED);
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        // no aggro during the intro
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_black_knight(Creature* pCreature)
{
    return new boss_black_knightAI(pCreature);
}

/*######
## npc_black_knight_gryphon
######*/

// TODO Remove this 'script' when combat can be proper prevented from core-side
struct npc_black_knight_gryphonAI : public ScriptedAI
{
    npc_black_knight_gryphonAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() override { }
    void AttackStart(Unit* /*pWho*/) override { }
    void MoveInLineOfSight(Unit* /*pWho*/) override { }
    void UpdateAI(const uint32 /*uiDiff*/) override { }
};

CreatureAI* GetAI_npc_black_knight_gryphon(Creature* pCreature)
{
    return new npc_black_knight_gryphonAI(pCreature);
}

void AddSC_boss_black_knight()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_black_knight";
    pNewScript->GetAI = &GetAI_boss_black_knight;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_black_knight_gryphon";
    pNewScript->GetAI = &GetAI_npc_black_knight_gryphon;
    pNewScript->RegisterSelf();
}
