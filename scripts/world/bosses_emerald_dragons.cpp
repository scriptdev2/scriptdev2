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
    SPELL_SEEPING_FOG_R     = 24813,                        // Summons 15224 'Dream Fog'
    SPELL_SEEPING_FOG_L     = 24814,
    SPELL_DREAM_FOG         = 24777,                        // Used by summoned Adds
    SPELL_NOXIOUS_BREATH    = 24818,
    SPELL_TAILSWEEP         = 15847,

    NPC_DREAM_FOG           = 15224,
};

enum SpecialDragonEvent
{
    EVENT_75_HEALTH         = 1,
    EVENT_50_HEALTH         = 2,
    EVENT_25_HEALTH         = 3,
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
    SAY_CASTCORRUPTION          = -1000402,

    //SPELL_MARKOFNATURE        = 25040,                        // Not working
    SPELL_VOLATILEINFECTION     = 24928,
    SPELL_CORRUPTION_OF_EARTH   = 24910
};

struct MANGOS_DLL_DECL boss_emerissAI : public boss_emerald_dragonAI
{
    boss_emerissAI(Creature* pCreature) : boss_emerald_dragonAI(pCreature) { Reset(); }

    //uint32 m_uiMarkOfNature_Timer;
    uint32 m_uiVolatileInfection_Timer;

    void Reset()
    {
        boss_emerald_dragonAI::Reset();

        //m_uiMarkOfNature_Timer = 45000;
        m_uiVolatileInfection_Timer = 12000;
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
            DoScriptText(SAY_CASTCORRUPTION, m_creature);

            // Successfull cast
            return true;
        }
        else
            return false;
    }

    bool UpdateDragonAI(const uint32 uiDiff)
    {
        //MarkOfNature_Timer
        //if (m_uiMarkOfNature_Timer < uiDiff)
        //{
        //    DoCastSpellIfCan(m_creature->getVictim(), SPELL_MARKOFNATURE);
        //    m_uiMarkOfNature_Timer = 45000;
        //}
        //else
            //m_uiMarkOfNature_Timer -= uiDiff;

        //VolatileInfection_Timer
        if (m_uiVolatileInfection_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_VOLATILEINFECTION);
            m_uiVolatileInfection_Timer = urand(7000, 12000);
        }
        else
            m_uiVolatileInfection_Timer -= uiDiff;

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

    //Spells of Taerar
    // SPELL_MARKOFNATURE   = 25040,                        // Not working
    SPELL_ARCANEBLAST       = 24857,
    SPELL_BELLOWINGROAR     = 22686,

    SPELL_SUMMONSHADE_1     = 24841,
    SPELL_SUMMONSHADE_2     = 24842,
    SPELL_SUMMONSHADE_3     = 24843,

    //Spells of Shades of Taerar
    SPELL_POSIONCLOUD       = 24840,
    SPELL_POSIONBREATH      = 20667
};

uint32 m_auiSpellSummonShade[]=
{
    SPELL_SUMMONSHADE_1, SPELL_SUMMONSHADE_2, SPELL_SUMMONSHADE_3
};

struct MANGOS_DLL_DECL boss_taerarAI : public boss_emerald_dragonAI
{
    boss_taerarAI(Creature* pCreature) : boss_emerald_dragonAI(pCreature) { Reset(); }

    //uint32 m_uiMarkOfNature_Timer;
    uint32 m_uiArcaneBlast_Timer;
    uint32 m_uiBellowingRoar_Timer;
    uint32 m_uiShades_Timer;

    bool m_bShades;

    void Reset()
    {
        boss_emerald_dragonAI::Reset();

        //m_uiMarkOfNature_Timer = 45000;
        m_uiArcaneBlast_Timer = 12000;
        m_uiBellowingRoar_Timer = 30000;
        m_uiShades_Timer = 60000;                               //The time that Taerar is banished

        m_bShades = false;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_TAERAR_AGGRO, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
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

                int iSize = sizeof(m_auiSpellSummonShade) / sizeof(uint32);

                for(int i = 0; i < iSize; ++i)
                    m_creature->CastSpell(pTarget, m_auiSpellSummonShade[i], true);

                m_bShades = true;
                m_uiShades_Timer = 60000;

                return true;
            }
        }
        return false;
    }

    bool UpdateDragonAI(const uint32 uiDiff)
    {
        // <<<<<<<<< TODO - FIXME - This code was called also OOC
        if (m_bShades && m_uiShades_Timer < uiDiff)
        {
            //Become unbanished again
            m_creature->setFaction(14);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_bShades = false;
        }
        else if (m_bShades)
        {
            m_uiShades_Timer -= uiDiff;
            //Do nothing while banished
            return false;
        }
        // >>>>>>>>> end of FIXME

        //MarkOfNature_Timer
        //if (m_uiMarkOfNature_Timer < uiDiff)
        //{
        //    DoCastSpellIfCan(m_creature->getVictim(), SPELL_MARKOFNATURE);
        //    m_uiMarkOfNature_Timer = 45000;
        //}
        //else
            //m_uiMarkOfNature_Timer -= uiDiff;

        //ArcaneBlast_Timer
        if (m_uiArcaneBlast_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANEBLAST);
            m_uiArcaneBlast_Timer = urand(7000, 12000);
        }
        else
            m_uiArcaneBlast_Timer -= uiDiff;

        //BellowingRoar_Timer
        if (m_uiBellowingRoar_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_BELLOWINGROAR);
            m_uiBellowingRoar_Timer = urand(20000, 30000);
        }
        else
            m_uiBellowingRoar_Timer -= uiDiff;

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
    SAY_SUMMONDRUIDS        = -1000361,

    //SPELL_MARKOFNATURE   = 25040,                         // Not working
    SPELL_LIGHTNINGWAVE     = 24819,
    SPELL_SUMMONDRUIDS      = 24795,

    SPELL_SUMMON_PLAYER     = 24776,

    //druid spells
    SPELL_MOONFIRE          = 21669
};

// Ysondre script
struct MANGOS_DLL_DECL boss_ysondreAI : public boss_emerald_dragonAI
{
    boss_ysondreAI(Creature* pCreature) : boss_emerald_dragonAI(pCreature) { Reset(); }

    //uint32 m_uiMarkOfNature_Timer;
    uint32 m_uiLightningWave_Timer;

    void Reset()
    {
        boss_emerald_dragonAI::Reset();

        //m_uiMarkOfNature_Timer = 45000;
        m_uiLightningWave_Timer = 12000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_YSONDRE_AGGRO, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    // Summon Druids - TODO FIXME (spell not understood)
    bool DoSpecialDragonAbility(SpecialDragonEvent uiEvent)
    {
        DoScriptText(SAY_SUMMONDRUIDS, m_creature);

        for(int i = 0; i < 10; ++i)
            DoCastSpellIfCan(m_creature, SPELL_SUMMONDRUIDS, CAST_TRIGGERED);

        return true;
    }

    bool UpdateDragonAI(const uint32 uiDiff)
    {
        //MarkOfNature_Timer
        //if (m_uiMarkOfNature_Timer < uiDiff)
        //{
        //    DoCastSpellIfCan(m_creature->getVictim(), SPELL_MARKOFNATURE);
        //    m_uiMarkOfNature_Timer = 45000;
        //}
        //else
            //m_uiMarkOfNature_Timer -= uiDiff;

        //LightningWave_Timer
        if (m_uiLightningWave_Timer < uiDiff)
        {
            //Cast LIGHTNINGWAVE on a Random target
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_LIGHTNINGWAVE);

            m_uiLightningWave_Timer = urand(7000, 12000);
        }
        else
            m_uiLightningWave_Timer -= uiDiff;

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
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_emeriss";
    newscript->GetAI = &GetAI_boss_emeriss;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_taerar";
    newscript->GetAI = &GetAI_boss_taerar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_shade_of_taerar";
    newscript->GetAI = &GetAI_boss_shadeoftaerar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_ysondre";
    newscript->GetAI = &GetAI_boss_ysondre;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dementeddruids";
    newscript->GetAI = &GetAI_mob_dementeddruids;
    newscript->RegisterSelf();
}
