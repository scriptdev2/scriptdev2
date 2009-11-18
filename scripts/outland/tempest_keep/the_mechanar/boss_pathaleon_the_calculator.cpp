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
SDName: Boss Pathaleon the Calculator
SD%Complete: 50
SDComment: Event missing. Script for himself 99% blizzlike.
SDCategory: Tempest Keep, The Mechanar
EndScriptData */

#include "precompiled.h"

#define SAY_AGGRO                       -1554020
#define SAY_DOMINATION_1                -1554021
#define SAY_DOMINATION_2                -1554022
#define SAY_SUMMON                      -1554023
#define SAY_ENRAGE                      -1554024
#define SAY_SLAY_1                      -1554025
#define SAY_SLAY_2                      -1554026
#define SAY_DEATH                       -1554027

// Spells to be casted
#define SPELL_MANA_TAP                  36021
#define SPELL_ARCANE_TORRENT            36022
#define SPELL_DOMINATION                35280
#define H_SPELL_ARCANE_EXPLOSION        15453
#define SPELL_FRENZY                    36992

#define SPELL_SUMMON_NETHER_WRAITH_1    35285               //Spells work, but not implemented
#define SPELL_SUMMON_NETHER_WRAITH_2    35286
#define SPELL_SUMMON_NETHER_WRAITH_3    35287
#define SPELL_SUMMON_NETHER_WRAITH_4    35288

// Add Spells
#define SPELL_DETONATION                35058
#define SPELL_ARCANE_MISSILES           35034

struct MANGOS_DLL_DECL boss_pathaleon_the_calculatorAI : public ScriptedAI
{
    boss_pathaleon_the_calculatorAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsHeroicMode = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        Reset();
    }

    bool m_bIsHeroicMode;

    uint32 Summon_Timer;
    uint32 ManaTap_Timer;
    uint32 ArcaneTorrent_Timer;
    uint32 Domination_Timer;
    uint32 ArcaneExplosion_Timer;
    bool Enraged;

    uint32 Counter;

    void Reset()
    {
        Summon_Timer = 30000;
        ManaTap_Timer = urand(12000, 20000);
        ArcaneTorrent_Timer = urand(16000, 25000);
        Domination_Timer = urand(25000, 40000);
        ArcaneExplosion_Timer = urand(8000, 13000);

        Enraged = false;
        Counter = 0;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Summon_Timer < diff)
        {
            for(int i = 0; i < 3; ++i)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                Creature* Wraith = m_creature->SummonCreature(21062,m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                if (target && Wraith)
                    Wraith->AI()->AttackStart(target);
            }

            DoScriptText(SAY_SUMMON, m_creature);

            Summon_Timer = urand(30000, 45000);
        }else Summon_Timer -= diff;

        if (ManaTap_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MANA_TAP);
            ManaTap_Timer = urand(14000, 22000);
        }else ManaTap_Timer -= diff;

        if (ArcaneTorrent_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_ARCANE_TORRENT);
            ArcaneTorrent_Timer = urand(12000, 18000);
        }else ArcaneTorrent_Timer -= diff;

        if (Domination_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,1))
            {
                DoScriptText(urand(0, 1) ? SAY_DOMINATION_1 : SAY_DOMINATION_2, m_creature);
                DoCast(target,SPELL_DOMINATION);
            }

            Domination_Timer = urand(25000, 30000);
        }else Domination_Timer -= diff;

        //Only casting if Heroic Mode is used
        if (m_bIsHeroicMode)
        {
            if (ArcaneExplosion_Timer < diff)
            {
                DoCast(m_creature->getVictim(),H_SPELL_ARCANE_EXPLOSION);
                ArcaneExplosion_Timer = urand(10000, 14000);
            }else ArcaneExplosion_Timer -= diff;
        }

        if (!Enraged && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 21)
        {
            DoCast(m_creature, SPELL_FRENZY);
            DoScriptText(SAY_ENRAGE, m_creature);
            Enraged = true;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_pathaleon_the_calculator(Creature* pCreature)
{
    return new boss_pathaleon_the_calculatorAI(pCreature);
}

struct MANGOS_DLL_DECL mob_nether_wraithAI : public ScriptedAI
{
    mob_nether_wraithAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    ScriptedInstance* m_pInstance;

    uint32 ArcaneMissiles_Timer;
    uint32 Detonation_Timer;
    uint32 Die_Timer;
    bool Detonation;

    void Reset()
    {
        ArcaneMissiles_Timer = urand(1000, 4000);
        Detonation_Timer = 20000;
        Die_Timer = 2200;
        Detonation = false;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (ArcaneMissiles_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,1))
                DoCast(target,SPELL_ARCANE_MISSILES);
            else
                DoCast(m_creature->getVictim(),SPELL_ARCANE_MISSILES);

            ArcaneMissiles_Timer = urand(5000, 10000);
        }else ArcaneMissiles_Timer -=diff;

        if (!Detonation)
        {
            if (Detonation_Timer < diff)
            {
                DoCast(m_creature,SPELL_DETONATION);
                Detonation = true;
            }else Detonation_Timer -= diff;
        }

        if (Detonation)
        {
            if (Die_Timer < diff)
            {
                m_creature->setDeathState(JUST_DIED);
                m_creature->RemoveCorpse();
            }else Die_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mob_nether_wraith(Creature* pCreature)
{
    return new mob_nether_wraithAI(pCreature);
}

void AddSC_boss_pathaleon_the_calculator()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_pathaleon_the_calculator";
    newscript->GetAI = &GetAI_boss_pathaleon_the_calculator;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_nether_wraith";
    newscript->GetAI = &GetAI_mob_nether_wraith;
    newscript->RegisterSelf();
}
