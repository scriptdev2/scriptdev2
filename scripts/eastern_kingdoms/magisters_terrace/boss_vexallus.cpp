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
SDName: Boss_Vexallus
SD%Complete: 90
SDComment: Heroic and Normal support. Needs further testing.
SDCategory: Magister's Terrace
EndScriptData */

#include "precompiled.h"
#include "magisters_terrace.h"
#include "TemporarySummon.h"

enum
{
    SAY_AGGRO                       = -1585007,
    SAY_ENERGY                      = -1585008,
    SAY_OVERLOAD                    = -1585009,
    SAY_KILL                        = -1585010,
    EMOTE_DISCHARGE_ENERGY          = -1585011,

    //is this text for real?
    //#define SAY_DEATH             "What...happen...ed."

    //Pure energy spell info
    SPELL_ENERGY_BOLT               = 46156,
    SPELL_ENERGY_FEEDBACK           = 44335,

    //Vexallus spell info
    SPELL_CHAIN_LIGHTNING           = 44318,
    SPELL_CHAIN_LIGHTNING_H         = 46380,                //heroic spell
    SPELL_OVERLOAD                  = 44353,
    SPELL_ARCANE_SHOCK              = 44319,
    SPELL_ARCANE_SHOCK_H            = 46381,                //heroic spell

    SPELL_SUMMON_PURE_ENERGY        = 44322,                //mod scale -10
    SPELL_SUMMON_PURE_ENERGY1_H     = 46154,                //mod scale -5
    SPELL_SUMMON_PURE_ENERGY2_H     = 46159,                //mod scale -5

    //Creatures
    NPC_PURE_ENERGY                 = 24745,

    INTERVAL_MODIFIER               = 15,
    INTERVAL_SWITCH                 = 6
};

struct MANGOS_DLL_DECL boss_vexallusAI : public ScriptedAI
{
    boss_vexallusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 ChainLightningTimer;
    uint32 ArcaneShockTimer;
    uint32 OverloadTimer;
    uint32 IntervalHealthAmount;
    bool Enraged;

    void Reset()
    {
        ChainLightningTimer = 8000;
        ArcaneShockTimer = 5000;
        OverloadTimer = 1200;
        IntervalHealthAmount = 1;
        Enraged = false;

        if (m_pInstance)
            m_pInstance->SetData(DATA_VEXALLUS_EVENT, NOT_STARTED);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit *victim)
    {
        if (m_pInstance)
            m_pInstance->SetData(DATA_VEXALLUS_EVENT, DONE);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(DATA_VEXALLUS_EVENT, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->GetMotionMaster()->MoveFollow(pTarget, 0.0f, 0.0f);

        pSummoned->CastSpell(pSummoned, SPELL_ENERGY_BOLT, false, NULL, NULL, m_creature->GetGUID());
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!Enraged)
        {
            //used for check, when Vexallus cast adds 85%, 70%, 55%, 40%, 25%
            if (m_creature->GetHealthPercent() <= float(100 - INTERVAL_MODIFIER*IntervalHealthAmount))
            {
                //increase amount, unless we're at 10%, then we switch and return
                if (IntervalHealthAmount == INTERVAL_SWITCH)
                {
                    Enraged = true;
                    return;
                }
                else
                    ++IntervalHealthAmount;

                DoScriptText(SAY_ENERGY, m_creature);
                DoScriptText(EMOTE_DISCHARGE_ENERGY, m_creature);

                if (m_bIsRegularMode)
                    m_creature->CastSpell(m_creature, SPELL_SUMMON_PURE_ENERGY, true);
                else
                {
                    m_creature->CastSpell(m_creature, SPELL_SUMMON_PURE_ENERGY1_H, true);
                    m_creature->CastSpell(m_creature, SPELL_SUMMON_PURE_ENERGY2_H, true);
                }
            }

            if (ChainLightningTimer < diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING_H);

                ChainLightningTimer = 8000;
            }else ChainLightningTimer -= diff;

            if (ArcaneShockTimer < diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_ARCANE_SHOCK : SPELL_ARCANE_SHOCK_H);

                ArcaneShockTimer = 8000;
            }else ArcaneShockTimer -= diff;
        }
        else
        {
            if (OverloadTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_OVERLOAD);

                OverloadTimer = 2000;
            }else OverloadTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_vexallus(Creature* pCreature)
{
    return new boss_vexallusAI(pCreature);
};

struct MANGOS_DLL_DECL mob_pure_energyAI : public ScriptedAI
{
    mob_pure_energyAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() { }

    void JustDied(Unit* pKiller)
    {
        if (m_creature->IsTemporarySummon())
        {
            TemporarySummon* pTemporary = (TemporarySummon*)m_creature;

            if (pTemporary->GetSummonerGuid().IsCreature())
            {
                Creature* pVex = m_creature->GetMap()->GetCreature(pTemporary->GetSummonerGuid());

                if (!pVex || !pVex->isAlive())
                    return;

                if (Player* pPlayer = pKiller->GetCharmerOrOwnerPlayerOrPlayerItself())
                    pPlayer->CastSpell(pPlayer, SPELL_ENERGY_FEEDBACK, true, NULL, NULL, pVex->GetGUID());
            }
        }
    }

    void MoveInLineOfSight(Unit *who) { }
    void AttackStart(Unit *who) { }
};

CreatureAI* GetAI_mob_pure_energy(Creature* pCreature)
{
    return new mob_pure_energyAI(pCreature);
};

void AddSC_boss_vexallus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_vexallus";
    newscript->GetAI = &GetAI_boss_vexallus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_pure_energy";
    newscript->GetAI = &GetAI_mob_pure_energy;
    newscript->RegisterSelf();
}
