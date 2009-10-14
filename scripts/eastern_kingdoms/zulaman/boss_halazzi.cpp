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
SDName: Boss_Halazzi
SD%Complete: 70
SDComment: Details and timers need check.
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"
#include "ObjectMgr.h"

enum
{
    SAY_AGGRO                       = -1568034,
    SAY_SPLIT                       = -1568035,
    SAY_MERGE                       = -1568036,
    SAY_SABERLASH1                  = -1568037,
    SAY_SABERLASH2                  = -1568038,
    SAY_BERSERK                     = -1568039,
    SAY_KILL1                       = -1568040,
    SAY_KILL2                       = -1568041,
    SAY_DEATH                       = -1568042,
    SAY_EVENT1                      = -1568043,
    SAY_EVENT2                      = -1568044,

    SPELL_DUAL_WIELD                = 42459,
    SPELL_SABER_LASH                = 43267,
    SPELL_FRENZY                    = 43139,
    SPELL_FLAMESHOCK                = 43303,
    SPELL_EARTHSHOCK                = 43305,
    SPELL_BERSERK                   = 45078,

    //SPELL_TRANSFORM_TO_ORIGINAL     = 43311,

    //SPELL_TRANSFIGURE               = 44054,

    SPELL_TRANSFIGURE_TO_TROLL      = 43142,
    //SPELL_TRANSFIGURE_TO_TROLL_TRIGGERED = 43573,

    SPELL_TRANSFORM_TO_LYNX_75      = 43145,
    SPELL_TRANSFORM_TO_LYNX_50      = 43271,
    SPELL_TRANSFORM_TO_LYNX_25      = 43272,

    SPELL_SUMMON_LYNX               = 43143,
    SPELL_SUMMON_TOTEM              = 43302,

    NPC_TOTEM                       = 24224
};

enum HalazziPhase
{
    PHASE_SINGLE        = 0,
    PHASE_TOTEM         = 1,
    PHASE_FINAL         = 2
};

struct MANGOS_DLL_DECL boss_halazziAI : public ScriptedAI
{
    boss_halazziAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiPhase;
    uint32 m_uiPhaseCounter;
    uint32 m_uiFrenzyTimer;
    uint32 m_uiSaberLashTimer;
    uint32 m_uiShockTimer;
    uint32 m_uiTotemTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiBerserkTimer;
    bool m_bIsBerserk;

    void Reset()
    {
        m_uiPhase = PHASE_SINGLE;                           // reset phase
        m_uiPhaseCounter = 3;

        m_uiCheckTimer = IN_MILISECONDS;
        m_uiFrenzyTimer = 16*IN_MILISECONDS;
        m_uiSaberLashTimer = 20*IN_MILISECONDS;
        m_uiShockTimer = 10*IN_MILISECONDS;
        m_uiTotemTimer = 12*IN_MILISECONDS;
        m_uiBerserkTimer = 10*MINUTE*IN_MILISECONDS;
        m_bIsBerserk = false;

        m_creature->SetMaxHealth(m_creature->GetCreatureInfo()->maxhealth);

        if (m_pInstance)
        {
            if (Creature* pSpiritLynx = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SPIRIT_LYNX)))
                pSpiritLynx->ForcedDespawn();
        }
    }

    void JustReachedHome()
    {
        m_pInstance->SetData(TYPE_HALAZZI, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->SetInCombatWithZone();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HALAZZI, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HALAZZI, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_SPIRIT_LYNX)
            pSummoned->SetInCombatWithZone();
    }

    void DoUpdateStats(const CreatureInfo* pInfo)
    {
        m_creature->SetMaxHealth(pInfo->maxhealth);

        if (m_uiPhase == PHASE_SINGLE)
        {
            m_creature->SetHealth(m_creature->GetMaxHealth()/4*m_uiPhaseCounter);
            --m_uiPhaseCounter;
        }
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->EffectApplyAuraName[0] != SPELL_AURA_TRANSFORM)
            return;

        // possibly hack and health should be set by Aura::HandleAuraTransform()
        if (const CreatureInfo* pInfo = GetCreatureTemplateStore(pSpell->EffectMiscValue[0]))
            DoUpdateStats(pInfo);

        if (m_uiPhase == PHASE_TOTEM)
            DoCast(m_creature, SPELL_SUMMON_LYNX);
    }

    void PhaseChange()
    {
        if (m_uiPhase == PHASE_SINGLE)
        {
            if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 25*m_uiPhaseCounter)
            {
                if (!m_uiPhaseCounter)
                {
                    // final phase
                    m_uiPhase = PHASE_FINAL;
                    m_uiFrenzyTimer = 16*IN_MILISECONDS;
                    m_uiSaberLashTimer = 20*IN_MILISECONDS;
                }
                else
                {
                    m_uiPhase = PHASE_TOTEM;
                    m_uiShockTimer = 10*IN_MILISECONDS;
                    m_uiTotemTimer = 12*IN_MILISECONDS;

                    DoScriptText(SAY_SPLIT, m_creature);
                    m_creature->CastSpell(m_creature, SPELL_TRANSFIGURE_TO_TROLL, false);
                }
            }
        }
        else
        {
            Creature* pSpiritLynx = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SPIRIT_LYNX));

            if (m_creature->GetHealth()*10 < m_creature->GetMaxHealth() ||
                (pSpiritLynx && pSpiritLynx->GetHealth()*10 < pSpiritLynx->GetMaxHealth()))
            {
                m_uiPhase = PHASE_SINGLE;

                DoScriptText(SAY_MERGE, m_creature);

                uint32 uiSpellId;

                switch(m_uiPhaseCounter)
                {
                    case 3: uiSpellId = SPELL_TRANSFORM_TO_LYNX_75; break;
                    case 2: uiSpellId = SPELL_TRANSFORM_TO_LYNX_50; break;
                    case 1: uiSpellId = SPELL_TRANSFORM_TO_LYNX_25; break;
                }

                m_creature->CastSpell(m_creature, uiSpellId, false);

                if (pSpiritLynx)
                    pSpiritLynx->ForcedDespawn();

                m_uiFrenzyTimer = 16*IN_MILISECONDS;
                m_uiSaberLashTimer = 20*IN_MILISECONDS;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsBerserk)
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                DoCast(m_creature, SPELL_BERSERK,true);
                m_bIsBerserk = true;
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        if (m_uiPhase != PHASE_FINAL)
        {
            if (m_uiCheckTimer < uiDiff)
            {
                if (m_pInstance)
                    PhaseChange();
                else
                    m_uiPhase = PHASE_FINAL;

                m_uiCheckTimer = IN_MILISECONDS;
            }
            else
                m_uiCheckTimer -= uiDiff;
        }

        if (m_uiPhase == PHASE_FINAL || m_uiPhase == PHASE_SINGLE)
        {
            if (m_uiFrenzyTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_FRENZY);
                m_uiFrenzyTimer = 16*IN_MILISECONDS;
            }
            else
                m_uiFrenzyTimer -= uiDiff;

            if (m_uiSaberLashTimer < uiDiff)
            {
                DoScriptText(urand(0, 1) ? SAY_SABERLASH1 : SAY_SABERLASH2, m_creature);

                DoCast(m_creature->getVictim(), SPELL_SABER_LASH);
                m_uiSaberLashTimer = 20*IN_MILISECONDS;
            }
            else
                m_uiSaberLashTimer -= uiDiff;
        }

        if (m_uiPhase == PHASE_FINAL || m_uiPhase == PHASE_TOTEM)
        {
            if (m_uiTotemTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_SUMMON_TOTEM);
                m_uiTotemTimer = 20*IN_MILISECONDS;
            }
            else
                m_uiTotemTimer -= uiDiff;

            if (m_uiShockTimer < uiDiff)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                {
                    if (pTarget->IsNonMeleeSpellCasted(false))
                        DoCast(pTarget, SPELL_EARTHSHOCK);
                    else
                        DoCast(pTarget, SPELL_FLAMESHOCK);

                    m_uiShockTimer = urand(10000, 14000);
                }
            }
            else
                m_uiShockTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_halazzi(Creature* pCreature)
{
    return new boss_halazziAI(pCreature);
}

enum
{
    SPELL_LYNX_FRENZY       = 43290,
    SPELL_SHRED_ARMOR       = 43243
};

struct MANGOS_DLL_DECL boss_spirit_lynxAI : public ScriptedAI
{
    boss_spirit_lynxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiFrenzyTimer;
    uint32 m_uiShredArmorTimer;

    void Reset()
    {
        m_uiFrenzyTimer = urand(10000, 20000);              //first frenzy after 10-20 seconds
        m_uiShredArmorTimer = 4000;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void KilledUnit(Unit* pVictim)
    {
        if (!m_pInstance)
            return;

        if (Creature* pHalazzi = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_HALAZZI)))
            pHalazzi->AI()->KilledUnit(pVictim);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrenzyTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_LYNX_FRENZY);
            m_uiFrenzyTimer = urand(20000, 30000);          //subsequent frenzys casted every 20-30 seconds
        }
        else
            m_uiFrenzyTimer -= uiDiff;

        if (m_uiShredArmorTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHRED_ARMOR);
            m_uiShredArmorTimer = 4000;
        }
        else
            m_uiShredArmorTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_spirit_lynx(Creature* pCreature)
{
    return new boss_spirit_lynxAI(pCreature);
}

void AddSC_boss_halazzi()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_halazzi";
    newscript->GetAI = &GetAI_boss_halazzi;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_spirit_lynx";
    newscript->GetAI = &GetAI_boss_spirit_lynx;
    newscript->RegisterSelf();
}
