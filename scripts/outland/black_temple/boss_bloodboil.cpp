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
SDName: Boss_Bloodboil
SD%Complete: 85
SDComment: Bloodboil not working correctly
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "black_temple.h"

//Speech'n'Sound
#define SAY_AGGRO               -1564029
#define SAY_SLAY1               -1564030
#define SAY_SLAY2               -1564031
#define SAY_SPECIAL1            -1564032
#define SAY_SPECIAL2            -1564033
#define SAY_ENRAGE1             -1564034
#define SAY_ENRAGE2             -1564035
#define SAY_DEATH               -1564036

//Spells
#define SPELL_ACID_GEYSER        40630
#define SPELL_ACIDIC_WOUND       40481
#define SPELL_ARCING_SMASH       40599
#define SPELL_BLOODBOIL          42005                      // This spell is AoE whereas it shouldn't be
#define SPELL_FEL_ACID           40508
#define SPELL_FEL_RAGE_SELF      40594
#define SPELL_FEL_RAGE_TARGET    40604
#define SPELL_FEL_RAGE_2         40616
#define SPELL_FEL_RAGE_3         41625
#define SPELL_BEWILDERING_STRIKE 40491
#define SPELL_EJECT1             40486                      // 1000 Physical damage + knockback + script effect (should handle threat reduction I think)
#define SPELL_EJECT2             40597                      // 1000 Physical damage + Stun (used in phase 2?)
#define SPELL_TAUNT_GURTOGG      40603
#define SPELL_INSIGNIFIGANCE     40618
#define SPELL_BERSERK            45078
#define SPELL_ENRAGE             27680

struct MANGOS_DLL_DECL boss_gurtogg_bloodboilAI : public ScriptedAI
{
    boss_gurtogg_bloodboilAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 TargetGUID;

    float TargetThreat;

    uint32 BloodboilTimer;
    uint32 BloodboilCount;
    uint32 AcidGeyserTimer;
    uint32 AcidicWoundTimer;
    uint32 ArcingSmashTimer;
    uint32 EnrageTimer;
    uint32 FelAcidTimer;
    uint32 EjectTimer;
    uint32 BewilderingStrikeTimer;
    uint32 PhaseChangeTimer;
    uint32 m_uiEnrageTimer;

    bool Phase1;

    void Reset()
    {
        TargetGUID = 0;

        TargetThreat = 0;

        BloodboilTimer = 10000;
        BloodboilCount = 0;
        AcidGeyserTimer = 1000;
        AcidicWoundTimer = 6000;
        ArcingSmashTimer = 19000;
        EnrageTimer = 600000;
        FelAcidTimer = 25000;
        EjectTimer = 10000;
        BewilderingStrikeTimer = 15000;
        PhaseChangeTimer = 60000;
        m_uiEnrageTimer = 600000;

        Phase1 = true;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLOODBOIL, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLOODBOIL, IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit *victim)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLOODBOIL, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    // Note: This seems like a very complicated fix. The fix needs to be handled by the core, as implementation of limited-target AoE spells are still not limited.
    void CastBloodboil()
    {
        // Get the Threat List
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();

        // He doesn't have anyone in his threatlist, useless to continue
        if (tList.empty())
            return;

        std::list<Unit *> targets;

        //store the threat list in a different container
        for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
        {
            Unit *target = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());

            //only on alive players
            if (target && target->isAlive() && target->GetTypeId() == TYPEID_PLAYER)
                targets.push_back(target);
        }

        //Sort the list of players
        targets.sort(ObjectDistanceOrderReversed(m_creature));
        //Resize so we only get top 5
        targets.resize(5);

        //Aura each player in the targets list with Bloodboil. Aura code copied+pasted from Aura command in Level3.cpp
        /*SpellEntry const *spellInfo = GetSpellStore()->LookupEntry(SPELL_BLOODBOIL);
        if (spellInfo)
        {
            for(std::list<Unit *>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                Unit* target = *itr;
                if (!target) return;
                for(uint32 i = 0;i<3; ++i)
                {
                    uint8 eff = spellInfo->Effect[i];
                    if (eff>=TOTAL_SPELL_EFFECTS)
                        continue;

                    Aura *Aur = new Aura(spellInfo, i, NULL, target);
                    target->AddAura(Aur);
                }
            }
        }*/
    }

    void RevertThreatOnTarget(uint64 guid)
    {
        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(guid))
        {
            if (m_creature->getThreatManager().getThreat(pPlayer))
                m_creature->getThreatManager().modifyThreatPercent(pPlayer, -100);

            if (TargetThreat)
                m_creature->AddThreat(pPlayer, TargetThreat);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (ArcingSmashTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCING_SMASH);
            ArcingSmashTimer = 10000;
        }else ArcingSmashTimer -= diff;

        if (FelAcidTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FEL_ACID);
            FelAcidTimer = 25000;
        }else FelAcidTimer -= diff;

        if (!m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            if (EnrageTimer < diff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                    DoScriptText(urand(0, 1) ? SAY_ENRAGE1 : SAY_ENRAGE2, m_creature);
            }else EnrageTimer -= diff;
        }

        if (Phase1)
        {
            if (BewilderingStrikeTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_BEWILDERING_STRIKE);
                float mt_threat = m_creature->getThreatManager().getThreat(m_creature->getVictim());

                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 1))
                    m_creature->AddThreat(target, mt_threat);

                BewilderingStrikeTimer = 20000;
            }else BewilderingStrikeTimer -= diff;

            if (EjectTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_EJECT1);
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(), -40);
                EjectTimer = 15000;
            }else EjectTimer -= diff;

            if (AcidicWoundTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_ACIDIC_WOUND);
                AcidicWoundTimer = 10000;
            }else AcidicWoundTimer -= diff;

            if (BloodboilTimer < diff)
            {
                if (BloodboilCount < 5)                     // Only cast it five times.
                {
                    //CastBloodboil(); // Causes issues on windows, so is commented out.
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLOODBOIL);
                    ++BloodboilCount;
                    BloodboilTimer = 10000*BloodboilCount;
                }
            }else BloodboilTimer -= diff;
        }

        if (!Phase1)
        {
            if (AcidGeyserTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_ACID_GEYSER);
                AcidGeyserTimer = 30000;
            }else AcidGeyserTimer -= diff;

            if (EjectTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_EJECT2);
                EjectTimer = 15000;
            }else EjectTimer -= diff;
        }

        if (PhaseChangeTimer < diff)
        {
            if (Phase1)
            {
                Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                if (target && target->isAlive())
                {
                    Phase1 = false;

                    TargetThreat = m_creature->getThreatManager().getThreat(target);
                    TargetGUID = target->GetGUID();
                    target->CastSpell(m_creature, SPELL_TAUNT_GURTOGG, true);

                    if (m_creature->getThreatManager().getThreat(target))
                        m_creature->getThreatManager().modifyThreatPercent(target, -100);

                    m_creature->AddThreat(target, 50000000.0f);

                    // If VMaps are disabled, this spell can call the whole instance
                    DoCastSpellIfCan(m_creature, SPELL_INSIGNIFIGANCE, CAST_TRIGGERED);
                    DoCastSpellIfCan(target, SPELL_FEL_RAGE_TARGET, CAST_TRIGGERED);
                    DoCastSpellIfCan(target, SPELL_FEL_RAGE_2, CAST_TRIGGERED);

                    /* These spells do not work, comment them out for now.
                    DoCastSpellIfCan(target, SPELL_FEL_RAGE_2, CAST_TRIGGERED);
                    DoCastSpellIfCan(target, SPELL_FEL_RAGE_3, CAST_TRIGGERED);*/

                    //Cast this without triggered so that it appears in combat logs and shows visual.
                    DoCastSpellIfCan(m_creature, SPELL_FEL_RAGE_SELF);

                    DoScriptText(urand(0, 1) ? SAY_SPECIAL1 : SAY_SPECIAL2, m_creature);

                    AcidGeyserTimer = 1000;
                    PhaseChangeTimer = 30000;
                }
            }else                                           // Encounter is a loop pretty much. Phase 1 -> Phase 2 -> Phase 1 -> Phase 2 till death or enrage
            {
                if (TargetGUID)
                    RevertThreatOnTarget(TargetGUID);

                TargetGUID = 0;
                Phase1 = true;
                BloodboilTimer = 10000;
                BloodboilCount = 0;
                AcidicWoundTimer += 2000;
                ArcingSmashTimer += 2000;
                FelAcidTimer += 2000;
                EjectTimer += 2000;
                PhaseChangeTimer = 60000;
            }
        }else PhaseChangeTimer -= diff;

        //Enrage
        if (m_uiEnrageTimer < diff)
        {
            DoCast(m_creature, SPELL_ENRAGE);
            m_uiEnrageTimer = 60000;
        }else m_uiEnrageTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gurtogg_bloodboil(Creature* pCreature)
{
    return new boss_gurtogg_bloodboilAI(pCreature);
}

void AddSC_boss_gurtogg_bloodboil()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_gurtogg_bloodboil";
    newscript->GetAI = &GetAI_boss_gurtogg_bloodboil;
    newscript->RegisterSelf();
}
