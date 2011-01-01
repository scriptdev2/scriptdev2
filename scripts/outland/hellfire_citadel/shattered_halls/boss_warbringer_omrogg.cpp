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
SDName: Boss_Warbringer_Omrogg
SD%Complete: 85
SDComment: Heroic enabled. Spell timing may need additional tweaks
SDCategory: Hellfire Citadel, Shattered Halls
EndScriptData */

/* ContentData
mob_omrogg_heads
boss_warbringer_omrogg
EndContentData */

#include "precompiled.h"
#include "shattered_halls.h"

enum
{
    YELL_DIE_L                  = -1540039,
    YELL_DIE_R                  = -1540040,
    EMOTE_ENRAGE                = -1540041,

    SPELL_BLAST_WAVE            = 30600,
    SPELL_FEAR                  = 30584,
    SPELL_THUNDERCLAP           = 30633,

    SPELL_BURNING_MAUL          = 30598,
    H_SPELL_BURNING_MAUL        = 36056,

    NPC_LEFT_HEAD               = 19523,
    NPC_RIGHT_HEAD              = 19524
};

struct Yell
{
    int32 id;
    uint32 creature;
};

static Yell GoCombat[]=
{
    {-1540018, NPC_LEFT_HEAD},
    {-1540019, NPC_LEFT_HEAD},
    {-1540020, NPC_LEFT_HEAD},
};
static Yell GoCombatDelay[]=
{
    {-1540021, NPC_RIGHT_HEAD},
    {-1540022, NPC_RIGHT_HEAD},
    {-1540023, NPC_RIGHT_HEAD},
};

static Yell Threat[]=
{
    {-1540024, NPC_LEFT_HEAD},
    {-1540025, NPC_RIGHT_HEAD},
    {-1540026, NPC_LEFT_HEAD},
    {-1540027, NPC_LEFT_HEAD},
};
static Yell ThreatDelay1[]=
{
    {-1540028, NPC_RIGHT_HEAD},
    {-1540029, NPC_LEFT_HEAD},
    {-1540030, NPC_RIGHT_HEAD},
    {-1540031, NPC_RIGHT_HEAD},
};
static Yell ThreatDelay2[]=
{
    {-1540032, NPC_LEFT_HEAD},
    {-1540033, NPC_RIGHT_HEAD},
    {-1540034, NPC_LEFT_HEAD},
    {-1540035, NPC_LEFT_HEAD},
};

static Yell Killing[]=
{
    {-1540036, NPC_LEFT_HEAD},
    {-1540037, NPC_RIGHT_HEAD},
};
static Yell KillingDelay[]=
{
    {-1540038, NPC_RIGHT_HEAD},
    {-1000000, NPC_LEFT_HEAD},
};

struct MANGOS_DLL_DECL mob_omrogg_headsAI : public ScriptedAI
{
    mob_omrogg_headsAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiDeath_Timer;
    bool m_bDeathYell;

    void Reset()
    {
        m_uiDeath_Timer = 4000;
        m_bDeathYell = false;
    }

    void DoDeathYell()
    {
        m_bDeathYell = true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bDeathYell)
            return;

        if (m_uiDeath_Timer < uiDiff)
        {
            DoScriptText(YELL_DIE_R, m_creature);
            m_uiDeath_Timer = false;
            m_creature->SetDeathState(JUST_DIED);
        }else m_uiDeath_Timer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_warbringer_omroggAI : public ScriptedAI
{
    boss_warbringer_omroggAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiLeftHeadGUID  = 0;
        m_uiRightHeadGUID = 0;
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_uiLeftHeadGUID;
    uint64 m_uiRightHeadGUID;

    int m_iAggro;
    int m_iThreat;
    int m_iKilling;

    bool m_bAggroYell;
    bool m_bThreatYell;
    bool m_bThreatYell2;
    bool m_bKillingYell;

    uint32 m_uiDelay_Timer;
    uint32 m_uiBlastWave_Timer;
    uint32 m_uiBlastCount;
    uint32 m_uiFear_Timer;
    uint32 m_uiBurningMaul_Timer;
    uint32 m_uiThunderClap_Timer;
    uint32 m_uiResetThreat_Timer;

    void Reset()
    {
        if (Creature* pLeftHead = m_creature->GetMap()->GetCreature(m_uiLeftHeadGUID))
        {
            pLeftHead->SetDeathState(JUST_DIED);
            m_uiLeftHeadGUID = 0;
        }

        if (Creature* pRightHead = m_creature->GetMap()->GetCreature(m_uiRightHeadGUID))
        {
            pRightHead->SetDeathState(JUST_DIED);
            m_uiRightHeadGUID = 0;
        }

        m_bAggroYell = false;
        m_bThreatYell = false;
        m_bThreatYell2 = false;
        m_bKillingYell = false;

        m_uiDelay_Timer = 4000;
        m_uiBlastWave_Timer = 0;
        m_uiBlastCount = 0;
        m_uiFear_Timer = 8000;
        m_uiBurningMaul_Timer = 25000;
        m_uiThunderClap_Timer = 15000;
        m_uiResetThreat_Timer = 30000;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_OMROGG, NOT_STARTED); //End boss can use this later. O'mrogg must be defeated(DONE) or he will come to aid.
    }

    void DoYellForThreat()
    {
        Creature* pLeftHead  = m_creature->GetMap()->GetCreature(m_uiLeftHeadGUID);
        Creature* pRightHead = m_creature->GetMap()->GetCreature(m_uiRightHeadGUID);

        if (!pLeftHead || !pRightHead)
            return;

        m_iThreat = irand(0, 3);

        Unit* pSource = (pLeftHead->GetEntry() == Threat[m_iThreat].creature ? pLeftHead : pRightHead);

        DoScriptText(Threat[m_iThreat].id, pSource);

        m_uiDelay_Timer = 3500;
        m_bThreatYell = true;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SummonCreature(NPC_LEFT_HEAD, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0);
        m_creature->SummonCreature(NPC_RIGHT_HEAD, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0);

        if (Creature* pLeftHead = m_creature->GetMap()->GetCreature(m_uiLeftHeadGUID))
        {
            m_iAggro = irand(0, 2);

            DoScriptText(GoCombat[m_iAggro].id, pLeftHead);

            m_uiDelay_Timer = 3500;
            m_bAggroYell = true;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_OMROGG, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_LEFT_HEAD)
            m_uiLeftHeadGUID = pSummoned->GetGUID();

        if (pSummoned->GetEntry() == NPC_RIGHT_HEAD)
            m_uiRightHeadGUID = pSummoned->GetGUID();

        //summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        //summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pSummoned->SetVisibility(VISIBILITY_OFF);
    }

    void KilledUnit(Unit* pVictim)
    {
        Creature* pLeftHead  = m_creature->GetMap()->GetCreature(m_uiLeftHeadGUID);
        Creature* pRightHead = m_creature->GetMap()->GetCreature(m_uiRightHeadGUID);

        if (!pLeftHead || !pRightHead)
            return;

        m_iKilling = irand(0, 1);

        Creature* pSource = (pLeftHead->GetEntry() == Killing[m_iKilling].creature ? pLeftHead : pRightHead);

        switch(m_iKilling)
        {
            case 0:
                DoScriptText(Killing[m_iKilling].id, pSource);
                m_uiDelay_Timer = 3500;
                m_bKillingYell = true;
                break;
            case 1:
                DoScriptText(Killing[m_iKilling].id, pSource);
                m_bKillingYell = false;
                break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        Creature* pLeftHead  = m_creature->GetMap()->GetCreature(m_uiLeftHeadGUID);
        Creature* pRightHead = m_creature->GetMap()->GetCreature(m_uiRightHeadGUID);

        if (!pLeftHead || !pRightHead)
            return;

        DoScriptText(YELL_DIE_L, pLeftHead);
        pLeftHead->SetDeathState(JUST_DIED);

        if (mob_omrogg_headsAI* pHeadAI = dynamic_cast<mob_omrogg_headsAI*>(pRightHead->AI()))
            pHeadAI->DoDeathYell();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_OMROGG, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiDelay_Timer < uiDiff)
        {
            m_uiDelay_Timer = 3500;

            Creature* pLeftHead  = m_creature->GetMap()->GetCreature(m_uiLeftHeadGUID);
            Creature* pRightHead = m_creature->GetMap()->GetCreature(m_uiRightHeadGUID);

            if (!pLeftHead || !pRightHead)
                return;

            if (m_bAggroYell)
            {
                DoScriptText(GoCombatDelay[m_iAggro].id, pRightHead);
                m_bAggroYell = false;
            }

            if (m_bThreatYell2)
            {
                Creature* pSource = (pLeftHead->GetEntry() == ThreatDelay2[m_iThreat].creature ? pLeftHead : pRightHead);

                DoScriptText(ThreatDelay2[m_iThreat].id, pSource);
                m_bThreatYell2 = false;
            }

            if (m_bThreatYell)
            {
                Creature* pSource = (pLeftHead->GetEntry() == ThreatDelay1[m_iThreat].creature ? pLeftHead : pRightHead);

                DoScriptText(ThreatDelay1[m_iThreat].id, pSource);
                m_bThreatYell = false;
                m_bThreatYell2 = true;
            }

            if (m_bKillingYell)
            {
                Creature* pSource = (pLeftHead->GetEntry() == KillingDelay[m_iKilling].creature ? pLeftHead : pRightHead);

                DoScriptText(KillingDelay[m_iKilling].id, pSource);
                m_bKillingYell = false;
            }
        }else m_uiDelay_Timer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBlastCount && m_uiBlastWave_Timer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature,SPELL_BLAST_WAVE);
            m_uiBlastWave_Timer = 5000;
            ++m_uiBlastCount;

            if (m_uiBlastCount == 3)
                m_uiBlastCount = 0;
        }else m_uiBlastWave_Timer -= uiDiff;

        if (m_uiBurningMaul_Timer < uiDiff)
        {
            DoScriptText(EMOTE_ENRAGE, m_creature);
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_BURNING_MAUL : H_SPELL_BURNING_MAUL);
            m_uiBurningMaul_Timer = 40000;
            m_uiBlastWave_Timer = 16000;
            m_uiBlastCount = 1;
        }else m_uiBurningMaul_Timer -= uiDiff;

        if (m_uiResetThreat_Timer < uiDiff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                DoYellForThreat();
                DoResetThreat();
                m_creature->AddThreat(target);
            }
            m_uiResetThreat_Timer = urand(25000, 40000);
        }else m_uiResetThreat_Timer -= uiDiff;

        if (m_uiFear_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,SPELL_FEAR);
            m_uiFear_Timer = urand(15000, 35000);
        }else m_uiFear_Timer -= uiDiff;

        if (m_uiThunderClap_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,SPELL_THUNDERCLAP);
            m_uiThunderClap_Timer = urand(15000, 30000);
        }else m_uiThunderClap_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_warbringer_omrogg(Creature* pCreature)
{
    return new boss_warbringer_omroggAI(pCreature);
}

CreatureAI* GetAI_mob_omrogg_heads(Creature* pCreature)
{
    return new mob_omrogg_headsAI(pCreature);
}

void AddSC_boss_warbringer_omrogg()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_warbringer_omrogg";
    newscript->GetAI = &GetAI_boss_warbringer_omrogg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_omrogg_heads";
    newscript->GetAI = &GetAI_mob_omrogg_heads;
    newscript->RegisterSelf();
}
