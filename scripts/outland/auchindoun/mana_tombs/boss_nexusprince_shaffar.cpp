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

    uint32 m_uiBlinkTimer;
    uint32 m_uiBeaconTimer;
    uint32 m_uiFireBallTimer;
    uint32 m_uiFrostboltTimer;
    uint32 m_uiFrostNovaTimer;

    bool m_bHasTaunted;

    void Reset()
    {
        m_uiBlinkTimer      = 30000;
        m_uiBeaconTimer     = 10000;
        m_uiFireBallTimer   = 8000;
        m_uiFrostboltTimer  = 4000;
        m_uiFrostNovaTimer  = 15000;
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
        switch(urand(0, 2))
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
            pSummoned->CastSpell(pSummoned, SPELL_ETHEREAL_BEACON_VISUAL, false);

            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEAD, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrostNovaTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FROSTNOVA) == CAST_OK)
                m_uiFrostNovaTimer = urand(17500, 25000);
        }
        else
            m_uiFrostNovaTimer -= uiDiff;

        if (m_uiFrostboltTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROSTBOLT) == CAST_OK)
                m_uiFrostboltTimer = urand(4500, 6000);
        }
        else
            m_uiFrostboltTimer -= uiDiff;

        if (m_uiFireBallTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIREBALL) == CAST_OK)
                m_uiFireBallTimer = urand(4500, 6000);
        }
        else
            m_uiFireBallTimer -= uiDiff;

        if (m_uiBlinkTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BLINK) == CAST_OK)
            {
                //expire movement, will prevent from running right back to victim after cast
                //(but should MoveChase be used again at a certain time or should he not move?)
                if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
                    m_creature->GetMotionMaster()->MovementExpired();

                m_uiBlinkTimer = urand(25000, 30000);
            }
        }
        else
            m_uiBlinkTimer -= uiDiff;

        if (m_uiBeaconTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ETHEREAL_BEACON) == CAST_OK)
            {
                if (!urand(0,3))
                    DoScriptText(SAY_SUMMON, m_creature);

                m_uiBeaconTimer = 10000;
            }
        }
        else
            m_uiBeaconTimer -= uiDiff;

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
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;

    uint32 m_uiApprenticeTimer;
    uint32 m_uiArcaneBoltTimer;

    void Reset()
    {
        m_uiApprenticeTimer = m_bIsRegularMode ? 20000 : 10000;
        m_uiArcaneBoltTimer = 1000;
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (m_creature->getVictim())
            pSummoned->AI()->AttackStart(m_creature->getVictim());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiArcaneBoltTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_BOLT);
            m_uiArcaneBoltTimer = urand(2000, 4500);
        }
        else
            m_uiArcaneBoltTimer -= uiDiff;

        if (m_uiApprenticeTimer)
        {
            if (m_uiApprenticeTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ETHEREAL_APPRENTICE) == CAST_OK)
                {
                    // despawn in 2 sec because of the spell visual
                    m_creature->ForcedDespawn(2000);
                    m_uiApprenticeTimer = 0;
                }
            }
            else
                m_uiApprenticeTimer -= uiDiff;
        }

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
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_nexusprince_shaffar";
    pNewScript->GetAI = &GetAI_boss_nexusprince_shaffar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_ethereal_beacon";
    pNewScript->GetAI = &GetAI_mob_ethereal_beacon;
    pNewScript->RegisterSelf();
}
