/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Nadox
SD%Complete: 90%
SDComment: TODO: some more research on guardian aura needed, BroodRage needs core and db support
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

enum
{
    SAY_AGGRO                     = -1619000,
    SAY_SUMMON_EGG_1              = -1619001,
    SAY_SUMMON_EGG_2              = -1619002,
    SAY_SLAY_1                    = -1619003,
    SAY_SLAY_2                    = -1619004,
    SAY_SLAY_3                    = -1619005,
    SAY_DEATH                     = -1619006,
    EMOTE_HATCH                   = -1619007,

    SPELL_BROOD_PLAGUE            = 56130,
    SPELL_BROOD_PLAGUE_H          = 59467,
    SPELL_BERSERK                 = 26662,
    SPELL_BROOD_RAGE              = 59465,

    SPELL_GUARDIAN_AURA           = 56151,
    SPELL_GUARDIAN_AURA_TRIGGERED = 56153,

    // JustSummoned is not called for spell summoned creatures
    SPELL_SUMMON_SWARM_GUARDIAN   = 56120,
    SPELL_SUMMON_SWARMERS         = 56119,

    NPC_AHNKAHAR_GUARDIAN_EGG     = 30173,
    NPC_AHNKAHAR_SWARM_EGG        = 30172,
    NPC_AHNKAHAR_GUARDIAN         = 30176,
    NPC_AHNKAHAR_SWARMER          = 30178
};

/*######
## mob_ahnkahat_egg
######*/
struct MANGOS_DLL_DECL mob_ahnkahar_eggAI : public ScriptedAI
{
    mob_ahnkahar_eggAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}
    void MoveInLineOfSight(Unit* pWho) {}
    void AttackStart(Unit* pWho) {}

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_AHNKAHAR_GUARDIAN)
            DoScriptText(EMOTE_HATCH, m_creature);

        if (m_pInstance)
        {
            if (Creature* pElderNadox = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ELDER_NADOX)))
            {
                float fPosX, fPosY, fPosZ;
                pElderNadox->GetPosition(fPosX, fPosY, fPosZ);
                pSummoned->GetMotionMaster()->MovePoint(0, fPosX, fPosY, fPosZ);
            }
        }
    }
};

CreatureAI* GetAI_mob_ahnkahar_egg(Creature* pCreature)
{
    return new mob_ahnkahar_eggAI(pCreature);
}

/*######
## boss_nadox
######*/

struct MANGOS_DLL_DECL boss_nadoxAI : public ScriptedAI
{
    boss_nadoxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool   m_bBerserk;
    bool   m_bGuardianSummoned;
    uint32 m_uiBroodPlagueTimer;
    uint32 m_uiBroodRageTimer;
    uint32 m_uiSummonTimer;

    void Reset()
    {
        m_bBerserk = false;
        m_bGuardianSummoned = false;
        m_uiSummonTimer = 5000;
        m_uiBroodPlagueTimer = 15000;
        m_uiBroodRageTimer = 20000;
    }

    Creature* SelectRandomCreatureOfEntryInRange(uint32 uiEntry, float fRange)
    {
        std::list<Creature* > lCreatureList;
        GetCreatureListWithEntryInGrid(lCreatureList, m_creature, uiEntry, fRange);

        if (lCreatureList.empty())
            return NULL;

        std::list<Creature* >::iterator iter = lCreatureList.begin();
        advance(iter, urand(0, lCreatureList.size()-1));

        return *iter;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bGuardianSummoned && m_creature->GetHealthPercent() < 50.0f)
        {
            // guardian is summoned at 50% of boss HP
            if (Creature* pGuardianEgg = SelectRandomCreatureOfEntryInRange(NPC_AHNKAHAR_GUARDIAN_EGG, 75.0f))
                pGuardianEgg->CastSpell(pGuardianEgg, SPELL_SUMMON_SWARM_GUARDIAN, false);

            m_bGuardianSummoned = true;
        }

        if (m_uiSummonTimer < uiDiff)
        {
            DoScriptText(urand(0, 1) ? SAY_SUMMON_EGG_1 : SAY_SUMMON_EGG_2, m_creature);

            if (Creature* pSwarmerEgg = SelectRandomCreatureOfEntryInRange(NPC_AHNKAHAR_SWARM_EGG, 75.0))
                pSwarmerEgg->CastSpell(pSwarmerEgg, SPELL_SUMMON_SWARMERS, false);

            m_uiSummonTimer = 10000;
        }
        else
            m_uiSummonTimer -= uiDiff;

        if (m_uiBroodPlagueTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_BROOD_PLAGUE : SPELL_BROOD_PLAGUE_H);

            m_uiBroodPlagueTimer = 20000;
        }
        else
            m_uiBroodPlagueTimer -= uiDiff;

        if (!m_bIsRegularMode)
        {
            if (m_uiBroodRageTimer < uiDiff)
            {
                if (Creature* pRageTarget = SelectRandomCreatureOfEntryInRange(NPC_AHNKAHAR_SWARMER, 50.0))
                    DoCastSpellIfCan(pRageTarget, SPELL_BROOD_RAGE);

                m_uiBroodRageTimer = 20000;
            }
            else
                m_uiBroodRageTimer -= uiDiff;
        }

        if (!m_bBerserk && m_creature->GetPositionZ() < 24.0)
        {
            m_bBerserk = true;
            DoCastSpellIfCan(m_creature, SPELL_BERSERK);
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_nadox(Creature* pCreature)
{
    return new boss_nadoxAI(pCreature);
}

void AddSC_boss_nadox()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_nadox";
    newscript->GetAI = &GetAI_boss_nadox;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ahnkahar_egg";
    newscript->GetAI = &GetAI_mob_ahnkahar_egg;
    newscript->RegisterSelf();
}
