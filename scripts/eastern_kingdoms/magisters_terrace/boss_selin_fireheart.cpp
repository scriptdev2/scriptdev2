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
SDName: Boss_Selin_Fireheart
SD%Complete: 90
SDComment: Heroic and Normal Support. Needs further testing.
SDCategory: Magister's Terrace
EndScriptData */

#include "precompiled.h"
#include "magisters_terrace.h"

enum
{
    SAY_AGGRO                       = -1585000,
    SAY_ENERGY                      = -1585001,
    SAY_EMPOWERED                   = -1585002,
    SAY_KILL_1                      = -1585003,
    SAY_KILL_2                      = -1585004,
    SAY_DEATH                       = -1585005,
    EMOTE_CRYSTAL                   = -1585006,

    // Crystal effect spells
    SPELL_FEL_CRYSTAL_COSMETIC      = 44374,
    SPELL_FEL_CRYSTAL_DUMMY         = 44329,
    SPELL_FEL_CRYSTAL_VISUAL        = 44355,
    SPELL_MANA_RAGE                 = 44320,                // This spell triggers 44321, which changes scale and regens mana Requires an entry in spell_script_target

    // Selin's spells
    SPELL_DRAIN_LIFE                = 44294,
    SPELL_FEL_EXPLOSION             = 44314,

    SPELL_DRAIN_MANA                = 46153,                // Heroic only
};

struct MANGOS_DLL_DECL boss_selin_fireheartAI : public ScriptedAI
{
    boss_selin_fireheartAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_magisters_terrace*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_magisters_terrace* m_pInstance;
    bool m_bIsRegularMode;

    GUIDList m_lCrystalGuids;

    uint32 m_uiDrainLifeTimer;
    uint32 m_uiDrainManaTimer;
    uint32 m_uiFelExplosionTimer;
    uint32 m_uiDrainCrystalTimer;
    uint32 m_uiEmpowerTimer;

    bool m_bIsDraining;
    bool m_bDrainingCrystal;

    ObjectGuid m_crystalGuid;                               // This will help us create a pointer to the crystal we are draining. We store GUIDs, never units in case unit is deleted/offline (offline if player of course).

    void Reset()
    {
        m_uiDrainLifeTimer = urand(3000, 7000);
        m_uiDrainManaTimer = m_uiDrainLifeTimer + 5000;
        m_uiFelExplosionTimer = 2100;
        m_uiDrainCrystalTimer = urand(10000, 15000);
        m_uiDrainCrystalTimer = urand(20000, 25000);
        m_uiEmpowerTimer = 10000;

        m_bIsDraining = false;
        m_bDrainingCrystal = false;
        m_crystalGuid.Clear();
    }

    void SelectNearestCrystal()
    {
        if (m_lCrystalGuids.empty())
            return;

        float ShortestDistance = 0;
        m_crystalGuid.Clear();
        Creature* pCrystal = NULL;
        Creature* pCrystalChosen = NULL;

        for (GUIDList::const_iterator itr = m_lCrystalGuids.begin(); itr != m_lCrystalGuids.end(); ++itr)
        {
            pCrystal = m_creature->GetMap()->GetCreature(*itr);

            if (pCrystal && pCrystal->isAlive())
            {
                // select nearest
                if (!pCrystalChosen || m_creature->GetDistanceOrder(pCrystal, pCrystalChosen, false))
                {
                    m_crystalGuid = pCrystal->GetObjectGuid();
                    pCrystalChosen = pCrystal;               // Store a copy of pCrystal so we don't need to recreate a pointer to closest crystal for the movement and yell.
                }
            }
        }
        if (pCrystalChosen)
        {
            DoScriptText(SAY_ENERGY, m_creature);
            DoScriptText(EMOTE_CRYSTAL, m_creature);

            pCrystalChosen->CastSpell(pCrystalChosen, SPELL_FEL_CRYSTAL_COSMETIC, true);

            float x, y, z;                                  // coords that we move to, close to the crystal.
            pCrystalChosen->GetClosePoint(x, y, z, m_creature->GetObjectBoundingRadius(), CONTACT_DISTANCE);

            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->GetMotionMaster()->MovePoint(1, x, y, z);
            m_bDrainingCrystal = true;
        }
    }

    void ShatterRemainingCrystals()
    {
        if (m_lCrystalGuids.empty())
            return;

        for (GUIDList::const_iterator itr = m_lCrystalGuids.begin(); itr != m_lCrystalGuids.end(); ++itr)
        {
            //Creature* pCrystal = m_creature->GetMap()->GetCreature(FelCrystals[i]);
            Creature* pCrystal = m_creature->GetMap()->GetCreature(*itr);

            if (pCrystal && pCrystal->isAlive())
                pCrystal->DealDamage(pCrystal, pCrystal->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SELIN, IN_PROGRESS);

            if (m_lCrystalGuids.empty())
            {
                // Get and output crystals
                m_pInstance->GetFelCrystalList(m_lCrystalGuids);
                for (GUIDList::const_iterator itr = m_lCrystalGuids.begin(); itr != m_lCrystalGuids.end(); ++itr)
                    debug_log("SD2: Selin: Added Fel Crystal %s to list", ObjectGuid(*itr).GetString().c_str());
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SELIN, FAIL);

            for (GUIDList::const_iterator itr = m_lCrystalGuids.begin(); itr != m_lCrystalGuids.end(); ++itr)
            {
                if (Creature* pCrystal = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (!pCrystal->isAlive())
                        pCrystal->Respawn();                // Let MaNGOS handle setting death state, etc.

                    // Only need to set unselectable flag. You can't attack unselectable units so non_attackable flag is not necessary here.
                    pCrystal->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType == POINT_MOTION_TYPE && uiPointId == 1)
        {
            Creature* pCrystalChosen = m_creature->GetMap()->GetCreature(m_crystalGuid);
            if (pCrystalChosen && pCrystalChosen->isAlive())
            {
                // Make the crystal attackable
                // We also remove NON_ATTACKABLE in case the database has it set.
                pCrystalChosen->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_SELECTABLE);
                pCrystalChosen->CastSpell(m_creature, SPELL_MANA_RAGE, true);
                m_bIsDraining = true;
            }
            else
            {
                // Make an error message in case something weird happened here
                error_log("SD2: Selin Fireheart unable to drain crystal as the crystal is either dead or deleted..");
                m_bDrainingCrystal = false;
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_SELIN, DONE);             // Encounter complete!

        ShatterRemainingCrystals();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bDrainingCrystal)
        {
            uint32 uiMaxPowerMana = m_creature->GetMaxPower(POWER_MANA);
            if (uiMaxPowerMana && ((m_creature->GetPower(POWER_MANA)*100 / uiMaxPowerMana) < 10))
            {
                if (m_uiDrainLifeTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_DRAIN_LIFE) == CAST_OK)
                            m_uiDrainLifeTimer = 10000;
                    }
                }
                else
                    m_uiDrainLifeTimer -= uiDiff;

                // Heroic only
                if (!m_bIsRegularMode)
                {
                    if (m_uiDrainManaTimer < uiDiff)
                    {
                        Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
                        if (!pTarget)
                            pTarget = m_creature->getVictim();

                        if (DoCastSpellIfCan(pTarget, SPELL_DRAIN_MANA) == CAST_OK)
                            m_uiDrainManaTimer = 10000;
                    }
                    else
                        m_uiDrainManaTimer -= uiDiff;
                }
            }

            if (m_uiFelExplosionTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FEL_EXPLOSION) == CAST_OK)
                    m_uiFelExplosionTimer = 2000;
            }
            else
                m_uiFelExplosionTimer -= uiDiff;

            // If below 10% mana, start recharging
            uiMaxPowerMana = m_creature->GetMaxPower(POWER_MANA);
            if (uiMaxPowerMana && ((m_creature->GetPower(POWER_MANA)*100 / uiMaxPowerMana) < 10))
            {
                if (m_uiDrainCrystalTimer < uiDiff)
                {
                    SelectNearestCrystal();

                    if (m_bIsRegularMode)
                        m_uiDrainCrystalTimer = urand(20000, 25000);
                    else
                        m_uiDrainCrystalTimer = urand(10000, 15000);

                }
                else
                    m_uiDrainCrystalTimer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }
        else                                                // if m_bDrainingCrystal
        {
            if (m_bIsDraining)
            {
                if (m_uiEmpowerTimer < uiDiff)
                {
                    m_bIsDraining = false;
                    m_bDrainingCrystal = false;

                    DoScriptText(SAY_EMPOWERED, m_creature);

                    Creature* CrystalChosen = m_creature->GetMap()->GetCreature(m_crystalGuid);
                    if (CrystalChosen && CrystalChosen->isAlive())
                        // Use Deal Damage to kill it, not SetDeathState.
                        CrystalChosen->DealDamage(CrystalChosen, CrystalChosen->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                    m_crystalGuid.Clear();

                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                }
                else
                    m_uiEmpowerTimer -= uiDiff;
            }
        }
    }
};

CreatureAI* GetAI_boss_selin_fireheart(Creature* pCreature)
{
    return new boss_selin_fireheartAI(pCreature);
};

struct MANGOS_DLL_DECL mob_fel_crystalAI : public ScriptedAI
{
    mob_fel_crystalAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() {}
    void AttackStart(Unit* pWho) {}
    void MoveInLineOfSight(Unit* pWho) {}
    void UpdateAI(const uint32 uiDiff) {}

    void JustDied(Unit* killer)
    {
        if (ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData())
        {
            Creature* pSelin = pInstance->GetSingleCreatureFromStorage(NPC_SELIN_FIREHEART);

            if (pSelin && pSelin->isAlive())
            {
                boss_selin_fireheartAI* pSelinAI = dynamic_cast<boss_selin_fireheartAI*>(pSelin->AI());

                if (pSelinAI && pSelinAI->m_crystalGuid == m_creature->GetObjectGuid())
                {
                    // Set this to false if we are the creature that Selin is draining so his AI flows properly
                    pSelinAI->m_bDrainingCrystal = false;
                    pSelinAI->m_bIsDraining = false;
                    pSelinAI->m_uiEmpowerTimer = 10000;

                    if (pSelin->getVictim())
                    {
                        pSelin->AI()->AttackStart(pSelin->getVictim());
                        pSelin->GetMotionMaster()->MoveChase(pSelin->getVictim());
                    }
                }
            }
        }
    }
};

CreatureAI* GetAI_mob_fel_crystal(Creature* pCreature)
{
    return new mob_fel_crystalAI(pCreature);
};

void AddSC_boss_selin_fireheart()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_selin_fireheart";
    pNewScript->GetAI = &GetAI_boss_selin_fireheart;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_fel_crystal";
    pNewScript->GetAI = &GetAI_mob_fel_crystal;
    pNewScript->RegisterSelf();
}
