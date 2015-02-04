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
SDName: argent_challenge
SD%Complete: 20
SDComment: Basic support
SDCategory: Crusader Coliseum, Trial of the Champion
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_champion.h"

enum
{
    FACTION_CHAMPION_FRIENDLY           = 35,
};

/*######
## argent_companion_common
######*/

struct argent_champion_commonAI : public ScriptedAI
{
    argent_champion_commonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_champion*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_trial_of_the_champion* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bDefeated;

    void Reset() override
    {
        m_bDefeated = false;
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARGENT_CHAMPION, IN_PROGRESS);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARGENT_CHAMPION, FAIL);
    }

    void DamageTaken(Unit* /*pDealer*/, uint32& uiDamage) override
    {
        if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;

            if (m_bDefeated)
                return;

            m_creature->SetFactionTemporary(FACTION_CHAMPION_FRIENDLY, TEMPFACTION_NONE);
            EnterEvadeMode();

            // ToDo: start the encounter complete event

            if (m_pInstance)
                m_pInstance->SetData(TYPE_ARGENT_CHAMPION, DONE);

            m_bDefeated = true;
        }
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId) override
    {
        if (uiMotionType != POINT_MOTION_TYPE || uiPointId != POINT_ID_CENTER)
            return;

        m_creature->SetRespawnCoord(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation());
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId) override
    {
        if (uiMotionType != POINT_MOTION_TYPE || uiPointId != POINT_ID_CENTER)
            return;

        pSummoned->SetRespawnCoord(pSummoned->GetPositionX(), pSummoned->GetPositionY(), pSummoned->GetPositionZ(), pSummoned->GetOrientation());
    }

    // Return true to handle shared timers and MeleeAttack
    virtual bool UpdateChampionAI(const uint32 /*uiDiff*/) { return true; }

    void UpdateAI(const uint32 uiDiff) override
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Call specific virtual function
        if (!UpdateChampionAI(uiDiff))
            return;

        DoMeleeAttackIfReady();
    }
};

enum
{
    SPELL_KILL_CREDIT_EADRIC        = 68575,
};

/*######
## boss_eadric
######*/

struct boss_eadricAI : public argent_champion_commonAI
{
    boss_eadricAI(Creature* pCreature) : argent_champion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        argent_champion_commonAI::Reset();
    }

    bool UpdateCompanionAI(const uint32 uiDiff)
    {
        return true;
    }
};

CreatureAI* GetAI_boss_eadric(Creature* pCreature)
{
    return new boss_eadricAI(pCreature);
}

enum
{
    SPELL_KILL_CREDIT_PALETRESS     = 68574,
};

/*######
## boss_paletress
######*/

struct boss_paletressAI : public argent_champion_commonAI
{
    boss_paletressAI(Creature* pCreature) : argent_champion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        argent_champion_commonAI::Reset();
    }

    bool UpdateCompanionAI(const uint32 uiDiff)
    {
        return true;
    }
};

CreatureAI* GetAI_boss_paletress(Creature* pCreature)
{
    return new boss_paletressAI(pCreature);
}

void AddSC_boss_argent_challenge()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_eadric";
    pNewScript->GetAI = &GetAI_boss_eadric;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_paletress";
    pNewScript->GetAI = &GetAI_boss_paletress;
    pNewScript->RegisterSelf();
}
