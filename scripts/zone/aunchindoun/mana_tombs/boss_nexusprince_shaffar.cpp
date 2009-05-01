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
SDName: Boss_NexusPrince_Shaffar
SD%Complete: 80
SDComment: Need more tuning of spell timers, it should not be as linear fight as current. Also should possibly find a better way to deal with his three initial beacons to make sure all aggro.
SDCategory: Auchindoun, Mana Tombs
EndScriptData */

/* ContentData
boss_nexusprince_shaffar
mob_ethereal_beacon
EndContentData */

#include "precompiled.h"

enum
{
    SAY_INTRO                       = -1557000,
    SAY_AGGRO_1                     = -1557001,
    SAY_AGGRO_2                     = -1557002,
    SAY_AGGRO_3                     = -1557003,
    SAY_SLAY_1                      = -1557004,
    SAY_SLAY_2                      = -1557005,
    SAY_SUMMON                      = -1557006,
    SAY_DEAD                        = -1557007,

    SPELL_BLINK                     = 34605,
    SPELL_FROSTBOLT                 = 32364,
    SPELL_FIREBALL                  = 32363,
    SPELL_FROSTNOVA                 = 32365,

    SPELL_ETHEREAL_BEACON           = 32371,                // Summons NPC_BEACON
    SPELL_ETHEREAL_BEACON_VISUAL    = 32368,

    NPC_BEACON                      = 18431
};

struct MANGOS_DLL_DECL boss_nexusprince_shaffarAI : public ScriptedAI
{
    boss_nexusprince_shaffarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bHasTaunted = false;
        Reset();
    }

    uint32 m_uiBlink_Timer;
    uint32 m_uiBeacon_Timer;
    uint32 m_uiFireBall_Timer;
    uint32 m_uiFrostbolt_Timer;
    uint32 m_uiFrostNova_Timer;

    bool m_bHasTaunted;
    bool m_bCanBlink;

    void Reset()
    {
        m_uiBlink_Timer = 1500;
        m_uiBeacon_Timer = 10000;
        m_uiFireBall_Timer = 8000;
        m_uiFrostbolt_Timer = 4000;
        m_uiFrostNova_Timer = 15000;

        m_bCanBlink = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 100.0f))
        {
            DoScriptText(SAY_INTRO, m_creature);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void Aggro(Unit* pWho)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO_3, m_creature); break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_BEACON)
        {
            pSummoned->CastSpell(pSummoned,SPELL_ETHEREAL_BEACON_VISUAL,false);

            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEAD, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrostNova_Timer < uiDiff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(true);

            DoCast(m_creature,SPELL_FROSTNOVA);
            m_uiFrostNova_Timer  = 17500 + rand()%7500;
            m_bCanBlink = true;
        }else m_uiFrostNova_Timer -= uiDiff;

        if (m_uiFrostbolt_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_FROSTBOLT);
            m_uiFrostbolt_Timer = 4500 + rand()%1500;
        }else m_uiFrostbolt_Timer -= uiDiff;

        if (m_uiFireBall_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_FIREBALL);
            m_uiFireBall_Timer = 4500 + rand()%1500;
        }else m_uiFireBall_Timer -= uiDiff;

        if (m_bCanBlink)
        {
            if (m_uiBlink_Timer < uiDiff)
            {
                if (m_creature->IsNonMeleeSpellCasted(false))
                    m_creature->InterruptNonMeleeSpells(true);

                //expire movement, will prevent from running right back to victim after cast
                //(but should MoveChase be used again at a certain time or should he not move?)
                if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
                    m_creature->GetMotionMaster()->MovementExpired();

                DoCast(m_creature,SPELL_BLINK);

                m_uiBlink_Timer = 1000 + rand()%1500;
                m_bCanBlink = false;
            }else m_uiBlink_Timer -= uiDiff;
        }

        if (m_uiBeacon_Timer < uiDiff)
        {
            if (!urand(0,3))
                DoScriptText(SAY_SUMMON, m_creature);

            DoCast(m_creature,SPELL_ETHEREAL_BEACON,true);

            m_uiBeacon_Timer = 10000;
        }else m_uiBeacon_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_nexusprince_shaffar(Creature* pCreature)
{
    return new boss_nexusprince_shaffarAI(pCreature);
}

enum
{
    SPELL_ARCANE_BOLT               = 15254,
    SPELL_ETHEREAL_APPRENTICE       = 32372                 // Summon 18430
};

struct MANGOS_DLL_DECL mob_ethereal_beaconAI : public ScriptedAI
{
    mob_ethereal_beaconAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bHeroicMode = pCreature->GetMap()->IsHeroic();
        Reset();
    }

    bool m_bHeroicMode;

    uint32 m_uiApprentice_Timer;
    uint32 m_uiArcaneBolt_Timer;

    void Reset()
    {
        m_uiApprentice_Timer = m_bHeroicMode ? 10000 : 20000;
        m_uiArcaneBolt_Timer = 1000;
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (m_creature->getVictim())
            pSummoned->AI()->AttackStart(m_creature->getVictim());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_uiArcaneBolt_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_ARCANE_BOLT);
            m_uiArcaneBolt_Timer = 2000 + rand()%2500;
        }else m_uiArcaneBolt_Timer -= uiDiff;

        if (m_uiApprentice_Timer < uiDiff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(true);

            m_creature->CastSpell(m_creature,SPELL_ETHEREAL_APPRENTICE,true);

            //we are summoned and will despawn after some time
            if (m_creature->GetOwner())
            {
                m_creature->SetVisibility(VISIBILITY_OFF);
                EnterEvadeMode();
            }
            //else, we are regular, so remove us in a different way
            else
            {
                m_creature->RemoveAllAuras();
                m_creature->setDeathState(JUST_DIED);
                m_creature->SetHealth(0);
                m_creature->CombatStop(true);
                m_creature->DeleteThreatList();
            }
            return;

        }else m_uiApprentice_Timer -= uiDiff;

        //should they do meele?
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ethereal_beacon(Creature* pCreature)
{
    return new mob_ethereal_beaconAI(pCreature);
}

void AddSC_boss_nexusprince_shaffar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_nexusprince_shaffar";
    newscript->GetAI = &GetAI_boss_nexusprince_shaffar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ethereal_beacon";
    newscript->GetAI = &GetAI_mob_ethereal_beacon;
    newscript->RegisterSelf();
}
