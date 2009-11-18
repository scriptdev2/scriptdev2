/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Anomalus
SD%Complete: 50%
SDComment: TODO: remove hacks, add support for rift charging
SDCategory: Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum
{
    SAY_AGGRO                          = -1576006,
    SAY_RIFT                           = -1576007,
    SAY_SHIELD                         = -1576008,
    SAY_KILL                           = -1576009,
    SAY_DEATH                          = -1576010,
    EMOTE_OPEN_RIFT                    = -1576021,
    EMOTE_SHIELD                       = -1576022,

    // Anomalus
    SPELL_CREATE_RIFT                  = 47743,
    SPELL_CHARGE_RIFT                  = 47747,
    SPELL_RIFT_SHIELD                  = 47748,

    SPELL_SPARK                        = 47751,
    SPELL_SPARK_H                      = 57062,

    SPELL_ARCANE_FORM                  = 48019,
    // Chaotic Rift
    SPELL_RIFT_AURA                    = 47687,
    SPELL_RIFT_SUMMON_AURA             = 47732,

    // Charged Chaotic Rift
    SPELL_CHARGED_RIFT_AURA            = 47733,
    SPELL_CHARGED_RIFT_SUMMON_AURA     = 47742,

    SPELL_SUMMON_CRAZED_MANA_WRAITH    = 47692,
    NPC_CHAOTIC_RIFT                   = 26918,
    NPC_CRAZED_MANA_WRAITH             = 26746
};

/*######
## boss_anomalus
######*/

struct MANGOS_DLL_DECL boss_anomalusAI : public ScriptedAI
{
    boss_anomalusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

    uint32 m_uiSparkTimer;
    uint32 m_uiCreateRiftTimer;
    uint64 m_uiChaoticRiftGUID;
    uint8 m_uiShieldCount;

    void Reset()
    {
        m_uiSparkTimer = 5000;
        m_uiCreateRiftTimer = 25000;
        m_uiChaoticRiftGUID = 0;
        m_uiShieldCount = 3;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANOMALUS, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANOMALUS, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANOMALUS, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_CHAOTIC_RIFT)
        {
            DoScriptText(SAY_RIFT, m_creature);

            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void SummonedCreatureDespawn(Creature* pSummoned)
    {
        if (pSummoned->GetGUID() == m_uiChaoticRiftGUID)
        {
            if (m_creature->HasAura(SPELL_RIFT_SHIELD))
                m_creature->RemoveAurasDueToSpell(SPELL_RIFT_SHIELD);

            m_uiChaoticRiftGUID = 0;
        }
    }

    uint64 CreateRiftAtRandomPoint()
    {
        float fPosX, fPosY, fPosZ;
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(15, 25), fPosX, fPosY, fPosZ);

        Creature* pRift = m_creature->SummonCreature(NPC_CHAOTIC_RIFT, fPosX, fPosY, fPosZ, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 1000);
        DoScriptText(EMOTE_OPEN_RIFT, m_creature);

        return pRift?pRift->GetGUID():0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || m_creature->HasAura(SPELL_RIFT_SHIELD))
             return;

        // Create additional Chaotic Rift at 75%, 50% and 25% HP
        if (m_creature->GetHealth()*4 < m_creature->GetMaxHealth()*m_uiShieldCount)
        {
            DoScriptText(EMOTE_SHIELD, m_creature);
            m_uiChaoticRiftGUID = CreateRiftAtRandomPoint();

            DoScriptText(SAY_SHIELD, m_creature);
            DoCast(m_creature, SPELL_RIFT_SHIELD);
            --m_uiShieldCount;
            return;
        }

        if (m_uiCreateRiftTimer < uiDiff)
        {
            CreateRiftAtRandomPoint();
            m_uiCreateRiftTimer = 25000;
        }
        else
            m_uiCreateRiftTimer -= uiDiff;

        if (m_uiSparkTimer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsHeroicMode?SPELL_SPARK_H:SPELL_SPARK);

            m_uiSparkTimer = 5000;
        }
        else
            m_uiSparkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anomalus(Creature* pCreature)
{
    return new boss_anomalusAI(pCreature);
}

struct MANGOS_DLL_DECL mob_chaotic_riftAI : public Scripted_NoMovementAI
{
    mob_chaotic_riftAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiSummonTimer;

    void Reset()
    {
        m_uiSummonTimer = 16000;
        DoCast(m_creature, SPELL_RIFT_AURA);
        //DoCast(m_creature, SPELL_RIFT_SUMMON_AURA);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_CRAZED_MANA_WRAITH)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_creature->HasAura(SPELL_ARCANE_FORM))
            DoCast(m_creature, SPELL_ARCANE_FORM);

        if (m_uiSummonTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_SUMMON_CRAZED_MANA_WRAITH);
            m_uiSummonTimer = 16000;
        }
        else
            m_uiSummonTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_chaotic_rift(Creature* pCreature)
{
    return new mob_chaotic_riftAI(pCreature);
}

void AddSC_boss_anomalus()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_anomalus";
    newscript->GetAI = &GetAI_boss_anomalus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_chaotic_rift";
    newscript->GetAI = &GetAI_mob_chaotic_rift;
    newscript->RegisterSelf();
}
