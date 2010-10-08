/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Grand_Warlock_Nethekurse
SD%Complete: 75
SDComment: encounter not fully completed. missing part where boss kill minions.
SDCategory: Hellfire Citadel, Shattered Halls
EndScriptData */

/* ContentData
boss_grand_warlock_nethekurse
mob_fel_orc_convert
mob_lesser_shadow_fissure
EndContentData */

#include "precompiled.h"
#include "shattered_halls.h"

struct Say
{
    int32 id;
};

static Say PeonAttacked[]=
{
    {-1540001},
    {-1540002},
    {-1540003},
    {-1540004},
};

static Say PeonDies[]=
{
    {-1540005},
    {-1540006},
    {-1540007},
    {-1540008},
};

enum
{
    SAY_INTRO              = -1540000,
    SAY_TAUNT_1            = -1540009,
    SAY_TAUNT_2            = -1540010,
    SAY_TAUNT_3            = -1540011,
    SAY_AGGRO_1            = -1540012,
    SAY_AGGRO_2            = -1540013,
    SAY_AGGRO_3            = -1540014,
    SAY_SLAY_1             = -1540015,
    SAY_SLAY_2             = -1540016,
    SAY_DIE                = -1540017,

    SPELL_DEATH_COIL       = 30500,
    SPELL_DARK_SPIN        = 30502,                         // core bug spell attack caster :D
    SPELL_SHADOW_FISSURE   = 30496,                         // Summon the ShadowFissure NPC

    SPELL_SHADOW_CLEAVE    = 30495,
    H_SPELL_SHADOW_SLAM    = 35953,

    SPELL_HEMORRHAGE       = 30478,

    SPELL_CONSUMPTION      = 30497,
    SPELL_TEMPORARY_VISUAL = 39312                          // this is wrong, a temporary solution. spell consumption already has the purple visual, but doesn't display as it should
};

struct MANGOS_DLL_DECL boss_grand_warlock_nethekurseAI : public ScriptedAI
{
    boss_grand_warlock_nethekurseAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIntroOnce;
    bool m_bIsIntroEvent;
    bool m_bIsMainEvent;
    bool m_bSpinOnce;
    //bool m_bHasTaunted;
    bool m_bPhase;

    uint32 m_uiPeonEngagedCount;
    uint32 m_uiPeonKilledCount;

    uint32 m_uiIntroEventTimer;
    uint32 m_uiDeathCoilTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiCleaveTimer;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        m_bIsIntroEvent = false;
        m_bIntroOnce = false;
        m_bIsMainEvent = false;
        //m_bHasTaunted = false;
        m_bSpinOnce = false;
        m_bPhase = false;

        m_uiPeonEngagedCount = 0;
        m_uiPeonKilledCount = 0;

        m_uiIntroEventTimer = 90000;                        // how long before getting bored and kills his minions?
        m_uiDeathCoilTimer = 20000;
        m_uiShadowFissureTimer = 8000;
        m_uiCleaveTimer = 5000;
    }

    void DoYellForPeonAggro()
    {
        if (m_uiPeonEngagedCount >= 4)
            return;

        DoScriptText(PeonAttacked[m_uiPeonEngagedCount].id, m_creature);
        ++m_uiPeonEngagedCount;
    }

    void DoYellForPeonDeath()
    {
        if (m_uiPeonKilledCount >= 4)
            return;

        DoScriptText(PeonDies[m_uiPeonKilledCount].id, m_creature);
        ++m_uiPeonKilledCount;

        if (m_uiPeonKilledCount == 4)
        {
            m_bIsIntroEvent = false;
            m_bIsMainEvent = true;
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void DoTauntPeons()
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_TAUNT_1, m_creature); break;
            case 1: DoScriptText(SAY_TAUNT_2, m_creature); break;
            case 2: DoScriptText(SAY_TAUNT_3, m_creature); break;
        }

        // TODO: kill the peons first
        m_bIsIntroEvent = false;
        m_uiPeonEngagedCount = 4;
        m_uiPeonKilledCount = 4;
        m_bIsMainEvent = true;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void AttackStart(Unit* pWho)
    {
        if (m_bIsIntroEvent || !m_bIsMainEvent)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if (m_bPhase)
                DoStartNoMovement(pWho);
            else
                DoStartMovement(pWho);
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bIntroOnce && m_creature->IsWithinDistInMap(pWho, 50.0f))
        {
            if (pWho->GetTypeId() != TYPEID_PLAYER)
                return;

            DoScriptText(SAY_INTRO, m_creature);
            m_bIntroOnce = true;
            m_bIsIntroEvent = true;

            if (m_pInstance)
                m_pInstance->SetData(TYPE_NETHEKURSE, IN_PROGRESS);
        }

        if (m_bIsIntroEvent || !m_bIsMainEvent)
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO_3, m_creature); break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->setFaction(16);
        pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        // triggered spell of consumption does not properly show it's SpellVisual, wrong spellid?
        pSummoned->CastSpell(pSummoned, SPELL_TEMPORARY_VISUAL, true);
        pSummoned->CastSpell(pSummoned, SPELL_CONSUMPTION, false, 0, 0, m_creature->GetGUID());
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DIE, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_NETHEKURSE, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsIntroEvent)
        {
            if (!m_pInstance)
                return;

            if (m_pInstance->GetData(TYPE_NETHEKURSE) == IN_PROGRESS)
            {
                if (m_uiIntroEventTimer < uiDiff)
                    DoTauntPeons();
                else
                    m_uiIntroEventTimer -= uiDiff;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsMainEvent)
            return;

        if (m_bPhase)
        {
            if (!m_bSpinOnce)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_DARK_SPIN);
                m_bSpinOnce = true;
            }

            if (m_uiCleaveTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_CLEAVE : H_SPELL_SHADOW_SLAM);
                m_uiCleaveTimer = urand(6000, 8500);
            }
            else
                m_uiCleaveTimer -= uiDiff;
        }
        else
        {
            if (m_uiShadowFissureTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, SPELL_SHADOW_FISSURE);
                m_uiShadowFissureTimer = urand(7500, 15000);
            }
            else
                m_uiShadowFissureTimer -= uiDiff;

            if (m_uiDeathCoilTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, SPELL_DEATH_COIL);
                m_uiDeathCoilTimer = urand(15000, 20000);
            }
            else
                m_uiDeathCoilTimer -= uiDiff;

            if (m_creature->GetHealthPercent() <= 20.0f)
                m_bPhase = true;

            DoMeleeAttackIfReady();
        }
    }
};

struct MANGOS_DLL_DECL mob_fel_orc_convertAI : public ScriptedAI
{
    mob_fel_orc_convertAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiHemorrhageTimer;

    void Reset()
    {
        m_creature->SetNoCallAssistance(true);              // we don't want any assistance (WE R HEROZ!)
        m_uiHemorrhageTimer = 3000;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        return;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            Creature* pKurse = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_NETHEKURSE));
            if (pKurse && m_creature->IsWithinDist(pKurse, 45.0f))
            {
                if (boss_grand_warlock_nethekurseAI* pKurseAI = dynamic_cast<boss_grand_warlock_nethekurseAI*>(pKurse->AI()))
                    pKurseAI->DoYellForPeonAggro();

                if (m_pInstance->GetData(TYPE_NETHEKURSE) == IN_PROGRESS)
                    return;
                else
                    m_pInstance->SetData(TYPE_NETHEKURSE, IN_PROGRESS);
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_NETHEKURSE) != IN_PROGRESS)
                return;

            if (Creature* pKurse = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_NETHEKURSE)))
            {
                if (boss_grand_warlock_nethekurseAI* pKurseAI = dynamic_cast<boss_grand_warlock_nethekurseAI*>(pKurse->AI()))
                    pKurseAI->DoYellForPeonAggro();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHemorrhageTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HEMORRHAGE);
            m_uiHemorrhageTimer = 15000;
        }
        else
            m_uiHemorrhageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// NOTE: this creature are also summoned by other spells, for different creatures
struct MANGOS_DLL_DECL mob_lesser_shadow_fissureAI : public ScriptedAI
{
    mob_lesser_shadow_fissureAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() { }
    void MoveInLineOfSight(Unit* pWho) { }
    void AttackStart(Unit* pWho) { }
};

CreatureAI* GetAI_boss_grand_warlock_nethekurse(Creature* pCreature)
{
    return new boss_grand_warlock_nethekurseAI(pCreature);
}

CreatureAI* GetAI_mob_fel_orc_convert(Creature* pCreature)
{
    return new mob_fel_orc_convertAI(pCreature);
}

CreatureAI* GetAI_mob_lesser_shadow_fissure(Creature* pCreature)
{
    return new mob_lesser_shadow_fissureAI(pCreature);
}

void AddSC_boss_grand_warlock_nethekurse()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_grand_warlock_nethekurse";
    pNewScript->GetAI = &GetAI_boss_grand_warlock_nethekurse;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_fel_orc_convert";
    pNewScript->GetAI = &GetAI_mob_fel_orc_convert;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_lesser_shadow_fissure";
    pNewScript->GetAI = &GetAI_mob_lesser_shadow_fissure;
    pNewScript->RegisterSelf();
}
