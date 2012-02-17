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
SDName: Boss_Darkweaver_Syth
SD%Complete: 85
SDComment: Shock spells/times need more work.
SDCategory: Auchindoun, Sethekk Halls
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_SUMMON                  = -1556000,
    SAY_AGGRO_1                 = -1556001,
    SAY_AGGRO_2                 = -1556002,
    SAY_AGGRO_3                 = -1556003,
    SAY_SLAY_1                  = -1556004,
    SAY_SLAY_2                  = -1556005,
    SAY_DEATH                   = -1556006,

    // TODO Heroic spells (shock, chain lightning) missing, unknown if IDs are correct
    SPELL_FROST_SHOCK           = 37865,
    SPELL_FLAME_SHOCK           = 34354,
    SPELL_SHADOW_SHOCK          = 30138,
    SPELL_ARCANE_SHOCK          = 37132,

    SPELL_CHAIN_LIGHTNING       = 39945,

    SPELL_SUMMON_SYTH_FIRE      = 33537,                    // Spawns 19203
    SPELL_SUMMON_SYTH_ARCANE    = 33538,                    // Spawns 19205
    SPELL_SUMMON_SYTH_FROST     = 33539,                    // Spawns 19204
    SPELL_SUMMON_SYTH_SHADOW    = 33540,                    // Spawns 19206

    // Npc entries
    NPC_FIRE_ELEMENTAL          = 19203,
    NPC_FROST_ELEMENTAL         = 19204,
    NPC_ARCANE_ELEMENTAL        = 19205,
    NPC_SHADOW_ELEMENTAL        = 19206,

    SPELL_FLAME_BUFFET          = 33526,
    SPELL_FLAME_BUFFET_H        = 38141,
    SPELL_ARCANE_BUFFET         = 33527,
    SPELL_ARCANE_BUFFET_H       = 38138,
    SPELL_FROST_BUFFET          = 33528,
    SPELL_FROST_BUFFET_H        = 38142,
    SPELL_SHADOW_BUFFET         = 33529,
    SPELL_SHADOW_BUFFET_H       = 38143,
};

struct MANGOS_DLL_DECL boss_darkweaver_sythAI : public ScriptedAI
{
    boss_darkweaver_sythAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiFlameshockTimer;
    uint32 m_uiArcaneshockTimer;
    uint32 m_uiFrostshockTimer;
    uint32 m_uiShadowshockTimer;
    uint32 m_uiChainlightningTimer;

    float m_fHpCheck;

    void Reset()
    {
        m_uiFlameshockTimer     = 2000;
        m_uiArcaneshockTimer    = 4000;
        m_uiFrostshockTimer     = 6000;
        m_uiShadowshockTimer    = 8000;
        m_uiChainlightningTimer = 15000;

        m_fHpCheck              = 90.0f;
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

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_FIRE_ELEMENTAL:
                pSummoned->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
                break;
            case NPC_FROST_ELEMENTAL:
                pSummoned->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                break;
            case NPC_ARCANE_ELEMENTAL:
                pSummoned->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_ARCANE, true);
                break;
            case NPC_SHADOW_ELEMENTAL:
                pSummoned->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_SHADOW, true);
                break;
        }

        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    // Wrapper to handle the elementals summon
    void SythSummoning()
    {
        DoScriptText(SAY_SUMMON, m_creature);

        if (m_creature->IsNonMeleeSpellCasted(false))
            m_creature->InterruptNonMeleeSpells(false);

        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SYTH_ARCANE, CAST_TRIGGERED); //front
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SYTH_FIRE,   CAST_TRIGGERED); //back
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SYTH_FROST,  CAST_TRIGGERED); //left
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SYTH_SHADOW, CAST_TRIGGERED); //right
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Summon elementals at 90%, 50% and 10% health
        if (m_creature->GetHealthPercent() < m_fHpCheck)
        {
            SythSummoning();
            m_fHpCheck -= 40.0f;
        }

        if (m_uiFlameshockTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_FLAME_SHOCK) == CAST_OK)
                    m_uiFlameshockTimer = urand(10000, 15000);
            }
        }
        else
            m_uiFlameshockTimer -= uiDiff;

        if (m_uiArcaneshockTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_ARCANE_SHOCK) == CAST_OK)
                    m_uiArcaneshockTimer = urand(10000, 15000);
            }
        }
        else
            m_uiArcaneshockTimer -= uiDiff;

        if (m_uiFrostshockTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_FROST_SHOCK) == CAST_OK)
                    m_uiFrostshockTimer = urand(10000, 15000);
            }
        }
        else
            m_uiFrostshockTimer -= uiDiff;

        if (m_uiShadowshockTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_SHOCK) == CAST_OK)
                    m_uiShadowshockTimer = urand(10000, 15000);
            }
        }
        else
            m_uiShadowshockTimer -= uiDiff;

        if (m_uiChainlightningTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_CHAIN_LIGHTNING) == CAST_OK)
                    m_uiChainlightningTimer = 25000;
            }
        }
        else
            m_uiChainlightningTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_darkweaver_syth(Creature* pCreature)
{
    return new boss_darkweaver_sythAI(pCreature);
}

/* ELEMENTALS */

struct MANGOS_DLL_DECL mob_syth_fireAI : public ScriptedAI
{
    mob_syth_fireAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 flameshock_timer;
    uint32 flamebuffet_timer;

    void Reset()
    {
        flameshock_timer = 2500;
        flamebuffet_timer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (flameshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_FLAME_SHOCK);

            flameshock_timer = 5000;
        }else flameshock_timer -= diff;

        if (flamebuffet_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
             DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_FLAME_BUFFET : SPELL_FLAME_BUFFET_H);

            flamebuffet_timer = 5000;

        }else flamebuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_syth_fire(Creature* pCreature)
{
    return new mob_syth_fireAI(pCreature);
}

struct MANGOS_DLL_DECL mob_syth_arcaneAI : public ScriptedAI
{
    mob_syth_arcaneAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 arcaneshock_timer;
    uint32 arcanebuffet_timer;

    void Reset()
    {
        arcaneshock_timer = 2500;
        arcanebuffet_timer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (arcaneshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_ARCANE_SHOCK);

            arcaneshock_timer = 5000;
        }else arcaneshock_timer -= diff;

        if (arcanebuffet_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_ARCANE_BUFFET : SPELL_ARCANE_BUFFET_H);

            arcanebuffet_timer = 5000;
        }else arcanebuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_syth_arcane(Creature* pCreature)
{
    return new mob_syth_arcaneAI(pCreature);
}

struct MANGOS_DLL_DECL mob_syth_frostAI : public ScriptedAI
{
    mob_syth_frostAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 frostshock_timer;
    uint32 frostbuffet_timer;

    void Reset()
    {
        frostshock_timer = 2500;
        frostbuffet_timer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (frostshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_FROST_SHOCK);

            frostshock_timer = 5000;
        }else frostshock_timer -= diff;

        if (frostbuffet_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_FROST_BUFFET : SPELL_FROST_BUFFET_H);

            frostbuffet_timer = 5000;
        }else frostbuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_syth_frost(Creature* pCreature)
{
    return new mob_syth_frostAI(pCreature);
}

struct MANGOS_DLL_DECL mob_syth_shadowAI : public ScriptedAI
{
    mob_syth_shadowAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 shadowshock_timer;
    uint32 shadowbuffet_timer;

    void Reset()
    {
        shadowshock_timer = 2500;
        shadowbuffet_timer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (shadowshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_SHADOW_SHOCK);

            shadowshock_timer = 5000;
        }else shadowshock_timer -= diff;

        if (shadowbuffet_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_SHADOW_BUFFET : SPELL_SHADOW_BUFFET_H);

            shadowbuffet_timer = 5000;
        }else shadowbuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_syth_shadow(Creature* pCreature)
{
    return new mob_syth_shadowAI(pCreature);
}

void AddSC_boss_darkweaver_syth()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_darkweaver_syth";
    pNewScript->GetAI = &GetAI_boss_darkweaver_syth;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_syth_fire";
    pNewScript->GetAI = &GetAI_mob_syth_fire;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_syth_arcane";
    pNewScript->GetAI = &GetAI_mob_syth_arcane;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_syth_frost";
    pNewScript->GetAI = &GetAI_mob_syth_frost;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_syth_shadow";
    pNewScript->GetAI = &GetAI_mob_syth_shadow;
    pNewScript->RegisterSelf();
}