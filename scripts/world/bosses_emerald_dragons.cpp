/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: bosses_emerald_dragons
SD%Complete: 70
SDComment: Emeriss 90, Lethon 0, Taerar 80, Ysondre 90 - Player-Teleport function missing, Mark of Nature needs core support
SDCategory: Emerald Dragon Bosses
EndScriptData */

/* ContentData
boss_emerald_dragon -- Superclass for the four dragons
boss_emeriss
boss_taerar
boss_shade_of_taerar -- TODO move to Acid
boss_ysondre
mob_dementeddruids; -- TODO move to Acid
EndContentData */

#include "precompiled.h"

/*######
## boss_emerald_dragon -- Superclass for the four dragons
######*/

enum
{
    SPELL_MARK_OF_NATURE_PLAYER     = 25040,
    SPELL_MARK_OF_NATURE_AURA       = 25041,
    SPELL_SEEPING_FOG_R             = 24813,                // Summons 15224 'Dream Fog'
    SPELL_SEEPING_FOG_L             = 24814,
    SPELL_DREAM_FOG                 = 24777,                // Used by summoned Adds
    SPELL_NOXIOUS_BREATH            = 24818,
    SPELL_TAILSWEEP                 = 15847,
    SPELL_SUMMON_PLAYER             = 24776,                // NYI

    NPC_DREAM_FOG                   = 15224,
};

enum SpecialDragonEvent
{
    EVENT_75_HEALTH                 = 1,
    EVENT_50_HEALTH                 = 2,
    EVENT_25_HEALTH                 = 3,
};

struct MANGOS_DLL_DECL boss_emerald_dragonAI : public ScriptedAI
{
    boss_emerald_dragonAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiEventCounter;

    uint32 m_uiSeepingFogTimer;
    uint32 m_uiNoxiousBreathTimer;
    uint32 m_uiTailsweepTimer;

    void Reset()
    {
        m_uiEventCounter = EVENT_75_HEALTH;

        m_uiSeepingFogTimer = urand(15000, 20000);
        m_uiNoxiousBreathTimer = 8000;
        m_uiTailsweepTimer = 4000;
    }

    void EnterCombat(Unit* pEnemy)
    {
        DoCastSpellIfCan(m_creature, SPELL_MARK_OF_NATURE_AURA);

        ScriptedAI::EnterCombat(pEnemy);
    }

    void KilledUnit(Unit* pVictim)
    {
        // Mark killed players with Mark of Nature
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            pVictim->CastSpell(pVictim, SPELL_MARK_OF_NATURE_PLAYER, true);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);

        if (pSummoned->GetEntry() == NPC_DREAM_FOG)
            pSummoned->CastSpell(pSummoned, SPELL_DREAM_FOG, true, NULL, NULL, m_creature->GetObjectGuid());
    }

    // Return true, if succeeded
    virtual bool DoSpecialDragonAbility(SpecialDragonEvent uiEvent) = 0;

    // Return true to handle shared timers and MeleeAttack
    virtual bool UpdateDragonAI(const uint32 uiDiff) { return true; }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Trigger special ability function at 75, 50 and 25% health
        if (m_creature->GetHealthPercent() < 100.0f - m_uiEventCounter * 25.0f && DoSpecialDragonAbility((SpecialDragonEvent) m_uiEventCounter))
            ++m_uiEventCounter;

        // Call dragon specific virtual function
        if (!UpdateDragonAI(uiDiff))
            return;

        if (m_uiSeepingFogTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SEEPING_FOG_R, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_SEEPING_FOG_L, CAST_TRIGGERED);
            m_uiSeepingFogTimer = urand(8000, 16000);
        }
        else
            m_uiSeepingFogTimer -= uiDiff;

        if (m_uiNoxiousBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_NOXIOUS_BREATH) == CAST_OK)
                m_uiNoxiousBreathTimer = urand(14000, 20000);
        }
        else
            m_uiNoxiousBreathTimer -= uiDiff;

        if (m_uiTailsweepTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_TAILSWEEP) == CAST_OK)
                m_uiTailsweepTimer = 2000;
        }
        else
            m_uiTailsweepTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## boss_emeriss
######*/

enum
{
    SAY_EMERISS_AGGRO           = -1000401,
    SAY_CAST_CORRUPTION         = -1000402,

    SPELL_VOLATILE_INFECTION    = 24928,
    SPELL_CORRUPTION_OF_EARTH   = 24910
};

struct MANGOS_DLL_DECL boss_emerissAI : public boss_emerald_dragonAI
{
    boss_emerissAI(Creature* pCreature) : boss_emerald_dragonAI(pCreature) { Reset(); }

    uint32 m_uiVolatileInfectionTimer;

    void Reset()
    {
        boss_emerald_dragonAI::Reset();

        m_uiVolatileInfectionTimer = 12000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_EMERISS_AGGRO, m_creature);
    }

    // Corruption of Earth at 75%, 50% and 25%
    bool DoSpecialDragonAbility(SpecialDragonEvent uiEvent)
    {
        if (DoCastSpellIfCan(m_creature, SPELL_CORRUPTION_OF_EARTH) == CAST_OK)
        {
            DoScriptText(SAY_CAST_CORRUPTION, m_creature);

            // Successfull cast
            return true;
        }
        else
            return false;
    }

    bool UpdateDragonAI(const uint32 uiDiff)
    {
        // Volatile Infection Timer
        if (m_uiVolatileInfectionTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget && DoCastSpellIfCan(pTarget, SPELL_VOLATILE_INFECTION) == CAST_OK)
                m_uiVolatileInfectionTimer = urand(7000, 12000);
        }
        else
            m_uiVolatileInfectionTimer -= uiDiff;

        return true;
    }
};

CreatureAI* GetAI_boss_emeriss(Creature* pCreature)
{
    return new boss_emerissAI(pCreature);
}

/*######
## boss_lethon
######*/

// TODO

/*######
## boss_taerar
######*/

enum
{
    SAY_TAERAR_AGGRO        = -1000399,
    SAY_SUMMONSHADE         = -1000400,

    SPELL_ARCANE_BLAST      = 24857,
    SPELL_BELLOWING_ROAR    = 22686,

    SPELL_SUMMON_SHADE_1    = 24841,
    SPELL_SUMMON_SHADE_2    = 24842,
    SPELL_SUMMON_SHADE_3    = 24843,

    //Spells of Shades of Taerar
    SPELL_POSIONCLOUD       = 24840,
    SPELL_POSIONBREATH      = 20667
};

static const uint32 auiSpellSummonShade[]=
{
    SPELL_SUMMON_SHADE_1, SPELL_SUMMON_SHADE_2, SPELL_SUMMON_SHADE_3
};

struct MANGOS_DLL_DECL boss_taerarAI : public boss_emerald_dragonAI
{
    boss_taerarAI(Creature* pCreature) : boss_emerald_dragonAI(pCreature) { Reset(); }

    uint32 m_uiArcaneBlastTimer;
    uint32 m_uiBellowingRoarTimer;
    uint32 m_uiShadesTimer;

    bool m_bShades;

    void Reset()
    {
        boss_emerald_dragonAI::Reset();

        m_uiArcaneBlastTimer = 12000;
        m_uiBellowingRoarTimer = 30000;
        m_uiShadesTimer = 60000;                            // The time that Taerar is banished

        m_bShades = false;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_TAERAR_AGGRO, m_creature);
    }

    // Summon 3 Shades at 75%, 50% and 25%
    bool DoSpecialDragonAbility(SpecialDragonEvent uiEvent)
    {
        if (!m_bShades)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                //Inturrupt any spell casting
                m_creature->InterruptNonMeleeSpells(false);

                //horrible workaround, need to fix
                m_creature->setFaction(35);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                DoScriptText(SAY_SUMMONSHADE, m_creature);

                int iSize = sizeof(auiSpellSummonShade) / sizeof(uint32);

                for(int i = 0; i < iSize; ++i)
                    m_creature->CastSpell(pTarget, auiSpellSummonShade[i], true);

                m_bShades = true;
                m_uiShadesTimer = 60000;

                return true;
            }
        }
        return false;
    }

    bool UpdateDragonAI(const uint32 uiDiff)
    {
        // <<<<<<<<< TODO - FIXME - This code was called also OOC
        if (m_bShades && m_uiShadesTimer < uiDiff)
        {
            //Become unbanished again
            m_creature->setFaction(14);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_bShades = false;
        }
        else if (m_bShades)
        {
            m_uiShadesTimer -= uiDiff;
            //Do nothing while banished
            return false;
        }
        // >>>>>>>>> end of FIXME

        // Arcane Blast Timer
        if (m_uiArcaneBlastTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget && DoCastSpellIfCan(pTarget, SPELL_ARCANE_BLAST) == CAST_OK)
                m_uiArcaneBlastTimer = urand(7000, 12000);
        }
        else
            m_uiArcaneBlastTimer -= uiDiff;

        // Bellowing Roar Timer
        if (m_uiBellowingRoarTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BELLOWING_ROAR) == CAST_OK)
                m_uiBellowingRoarTimer = urand(20000, 30000);
        }
        else
            m_uiBellowingRoarTimer -= uiDiff;

        return true;
    }
};

// Shades of Taerar Script
struct MANGOS_DLL_DECL boss_shadeoftaerarAI : public ScriptedAI
{
    boss_shadeoftaerarAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiPoisonCloud_Timer;
    uint32 m_uiPosionBreath_Timer;

    void Reset()
    {
        m_uiPoisonCloud_Timer = 8000;
        m_uiPosionBreath_Timer = 12000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //PoisonCloud_Timer
        if (m_uiPoisonCloud_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_POSIONCLOUD);
            m_uiPoisonCloud_Timer = 30000;
        }
        else
            m_uiPoisonCloud_Timer -= uiDiff;

        //PosionBreath_Timer
        if (m_uiPosionBreath_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_POSIONBREATH);
            m_uiPosionBreath_Timer = 12000;
        }
        else
            m_uiPosionBreath_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_taerar(Creature* pCreature)
{
    return new boss_taerarAI(pCreature);
}

CreatureAI* GetAI_boss_shadeoftaerar(Creature* pCreature)
{
    return new boss_shadeoftaerarAI(pCreature);
}

/*######
## boss_ysondre
######*/

enum
{
    SAY_YSONDRE_AGGRO       = -1000360,
    SAY_SUMMON_DRUIDS       = -1000361,

    SPELL_LIGHTNING_WAVE    = 24819,
    SPELL_SUMMON_DRUIDS     = 24795,

    //druid spells
    SPELL_MOONFIRE          = 21669
};

// Ysondre script
struct MANGOS_DLL_DECL boss_ysondreAI : public boss_emerald_dragonAI
{
    boss_ysondreAI(Creature* pCreature) : boss_emerald_dragonAI(pCreature) { Reset(); }

    uint32 m_uiLightningWaveTimer;

    void Reset()
    {
        boss_emerald_dragonAI::Reset();

        m_uiLightningWaveTimer = 12000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_YSONDRE_AGGRO, m_creature);
    }

    // Summon Druids - TODO FIXME (spell not understood)
    bool DoSpecialDragonAbility(SpecialDragonEvent uiEvent)
    {
        DoScriptText(SAY_SUMMON_DRUIDS, m_creature);

        for(int i = 0; i < 10; ++i)
            DoCastSpellIfCan(m_creature, SPELL_SUMMON_DRUIDS, CAST_TRIGGERED);

        return true;
    }

    bool UpdateDragonAI(const uint32 uiDiff)
    {
        // Lightning Wave Timer
        if (m_uiLightningWaveTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget && DoCastSpellIfCan(pTarget, SPELL_LIGHTNING_WAVE) == CAST_OK)
                m_uiLightningWaveTimer = urand(7000, 12000);
        }
        else
            m_uiLightningWaveTimer -= uiDiff;

        return true;
    }
};

// Summoned druid script
struct MANGOS_DLL_DECL mob_dementeddruidsAI : public ScriptedAI
{
    mob_dementeddruidsAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiMoonFire_Timer;

    void Reset()
    {
        m_uiMoonFire_Timer = 3000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //MoonFire_Timer
        if (m_uiMoonFire_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MOONFIRE);
            m_uiMoonFire_Timer = 5000;
        }
        else
            m_uiMoonFire_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ysondre(Creature* pCreature)
{
    return new boss_ysondreAI(pCreature);
}

CreatureAI* GetAI_mob_dementeddruids(Creature* pCreature)
{
    return new mob_dementeddruidsAI(pCreature);
}

void AddSC_bosses_emerald_dragons()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_emeriss";
    pNewScript->GetAI = &GetAI_boss_emeriss;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_taerar";
    pNewScript->GetAI = &GetAI_boss_taerar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_shade_of_taerar";
    pNewScript->GetAI = &GetAI_boss_shadeoftaerar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_ysondre";
    pNewScript->GetAI = &GetAI_boss_ysondre;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_dementeddruids";
    pNewScript->GetAI = &GetAI_mob_dementeddruids;
    pNewScript->RegisterSelf();
}
