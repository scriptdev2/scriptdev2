/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: boss_freya
SD%Complete: 40%
SDComment: Parially complete.
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                           = -1603000,
    SAY_AGGRO_HARD                      = -1603001,
    SAY_ADDS_CONSERVATOR                = -1603002,
    SAY_ADDS_TRIO                       = -1603003,
    SAY_ADDS_LASHER                     = -1603004,
    SAY_SLAY_1                          = -1603005,
    SAY_SLAY_2                          = -1603006,
    SAY_EPILOGUE                        = -1603007,
    SAY_BERSERK                         = -1603008,

    EMOTE_ALLIES_NATURE                 = -1603010,
    EMOTE_LIFEBINDER                    = -1603011,
    EMOTE_TREMOR                        = -1603012,
    EMOTE_IRON_ROOTS                    = -1603013,

    SAY_AGGRO_BRIGHT                    = -1603014,
    SAY_SLAY_1_BRIGHT                   = -1603015,
    SAY_SLAY_2_BRIGHT                   = -1603016,
    SAY_DEATH_BRIGHT                    = -1603017,

    SAY_AGGRO_IRON                      = -1603018,
    SAY_SLAY_1_IRON                     = -1603019,
    SAY_SLAY_2_IRON                     = -1603020,
    SAY_DEATH_IRON                      = -1603021,

    SAY_AGGRO_STONE                     = -1603022,
    SAY_SLAY_1_STONE                    = -1603023,
    SAY_SLAY_2_STONE                    = -1603024,
    SAY_DEATH_STONE                     = -1603025,

    // general spells
    SPELL_FREYA_CREDIT                  = 65074,                // kill credit spell; added in spell_template
    SPELL_TELEPORT                      = 62940,                // despawn on event end
    SPELL_BERSERK                       = 47008,
    // SPELL_FREYA_DUMMY_YELLOW         = 63292,                // dummy spells used to light up the crystals; used in dbscripts_on_creature_move
    // SPELL_FREYA_DUMMY_BLUE           = 63294,
    // SPELL_FREYA_DUMMY_GREEN          = 63295,

    // combat spells
    SPELL_ATTUNED_TO_NATURE             = 62519,
    SPELL_TOUCH_OF_EONAR                = 62528,
    SPELL_TOUCH_OF_EONAR_H              = 62892,
    SPELL_SUNBEAM                       = 62623,
    SPELL_SUNBEAM_H                     = 62872,

    // summon creature spells
    SPELL_SUMMON_ALLIES_OF_NATURE       = 62678,                // triggers random of 62685, 62686 or 62688
    SPELL_SUMMON_ALLIES_OF_NATURE_H     = 62873,                // spell needs to be confirmed; identical to normal mode version
    SPELL_SUMMON_WAVE_1                 = 62685,                // summon npc 33203
    SPELL_SUMMON_WAVE_3                 = 62686,                // summon npcs 33202, 32916 and 32919
    SPELL_SUMMON_WAVE_10                = 62688,                // summon 10 * npc 32918
    SPELL_LIFEBINDERS_GIFT_SUMMON       = 62572,
    SPELL_NATURE_BOMB_SUMMON            = 64604,

    // summon loot spells
    SPELL_SUMMON_CHEST_0                = 62950,                // summon loot chest spells, depending on the number of elders alive
    SPELL_SUMMON_CHEST_1                = 62952,
    SPELL_SUMMON_CHEST_2                = 62953,
    SPELL_SUMMON_CHEST_3                = 62954,
    SPELL_SUMMON_CHEST_0_H              = 62955,
    SPELL_SUMMON_CHEST_1_H              = 62956,
    SPELL_SUMMON_CHEST_2_H              = 62957,
    SPELL_SUMMON_CHEST_3_H              = 62958,

    // hard mode spells
    SPELL_DRAINED_OF_POWER              = 62467,                // stun effect for each elder alive

    SPELL_BRIGHTLEAF_ESSENCE_CHANNEL    = 62485,                // brightleaf
    SPELL_BRIGHTLEAF_ESSENCE_CHANNEL_H  = 65587,
    SPELL_UNSTABLE_SUN_BEAM             = 62450,
    SPELL_UNSTABLE_SUN_BEAM_H           = 62868,

    SPELL_IRONBRANCH_ESSENCE_CHANNEL    = 62484,                // ironbrach
    SPELL_IRONBRANCH_ESSENCE_CHANNEL_H  = 65588,
    SPELL_IRON_ROOTS                    = 62439,
    SPELL_IRON_ROOTS_H                  = 62862,
    // SPELL_STRENGTHEN_IRON_ROOTS      = 62440,                // purpose unk
    // SPELL_STRENGTHEN_IRON_ROOTS_H    = 63601,

    SPELL_STONEBARK_ESSEMCE_CHANNEL     = 62483,                // stonebark
    SPELL_STONEBARK_ESSEMCE_CHANNEL_H   = 65589,
    SPELL_GROUND_TREMOR                 = 62437,
    SPELL_GROUND_TREMOR_H               = 62859,

    // summons spells
    SPELL_SPORE_SUMMON_PERIODIC         = 62566,                // triggers 62582, 62591, 62592, 62593; cast by 33203
    SPELL_HEALTHY_SPORE_VISUAL          = 62538,                // cast by npc 33215
    SPELL_POTENT_PHEROMONES             = 62541,                // cast by npc 33215

    // sun beam spells
    SPELL_UNSTABLE_SUN_BEAM_VISUAL      = 62216,                // cast by npc 33170
    SPELL_UNSTABLE_ENERGY               = 62451,                // cast by npc 33170
    SPELL_UNSTABLE_ENERGY_H             = 62865,

    // nature bomb spells
    SPELL_NATURE_BOMB_GO                = 64600,                // spawns go 194902
    SPELL_NATURE_BOMB                   = 64587,
    SPELL_NATURE_BOMB_H                 = 64650,

    // allies of nature spells
    SPELL_ATTUNED_10_STACKS             = 62525,                // remove 10 stacks of 62519
    SPELL_ATTUNED_2_STACKS              = 62524,                // remove 2 stacks of 62519
    SPELL_ATTUNED_25_STACKS             = 62521,                // remove 25 stacks of 62519

    // eonar's gift spells
    SPELL_LIFEBINDERS_GIFT              = 62584,
    SPELL_LIFEBINDERS_GIFT_H            = 64185,
    SPELL_LIFEBINDERS_GIFT_VISUAL       = 62579,
    SPELL_AUTO_GROW                     = 62559,                // cast by npcs 33228 and 33215
    SPELL_PHEROMONES                    = 62619,

    // allies of nature summons
    NPC_DETONATING_LASHER               = 32918,                // has auras 64481 and 28819
    NPC_ANCIENT_CONSERVATOR             = 33203,
    NPC_WATER_SPIRIT                    = 33202,
    NPC_STORM_LASHER                    = 32919,
    NPC_SNAPLASHER                      = 32916,

    // other summons
    NPC_NATURE_BOMB                     = 34129,
    NPC_SUN_BEAM                        = 33170,
    NPC_UNSTABLE_SUN_BEAM               = 33050,
    NPC_STRENGHENED_IRON_ROOTS          = 33168,
    NPC_EONARS_GIFT                     = 33228,
    NPC_HEALTHY_SPORE                   = 33215,

    // other
    GO_NATURE_BOMB                      = 194902,
    MIN_ATTUNED_NATURE_STACKS           = 25,
    MAX_ALLIES_SPELLS                   = 3,
    MAX_ALLIES_WAVES                    = 6,
};

static const uint32 aAlliesSpawnSpells[MAX_ALLIES_SPELLS] = {SPELL_SUMMON_WAVE_1, SPELL_SUMMON_WAVE_3, SPELL_SUMMON_WAVE_10};

/*######
## boss_freya
######*/

struct MANGOS_DLL_DECL boss_freyaAI : public ScriptedAI
{
    boss_freyaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        // init the Allies of Nature spells
        spawnSpellsVector.reserve(MAX_ALLIES_SPELLS);
        for (uint8 i = 0; i < MAX_ALLIES_SPELLS; ++i)
            spawnSpellsVector.push_back(aAlliesSpawnSpells[i]);

        m_bEventFinished = false;
        m_uiEpilogueTimer = 0;
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bEventFinished;

    uint32 m_uiEpilogueTimer;
    uint32 m_uiBerserkTimer;

    uint32 m_uiAlliesNatureTimer;
    uint8 m_uiAlliesWaveCount;

    uint32 m_uiSunbeamTimer;
    uint32 m_uiNatureBombTimer;
    uint32 m_uiLifebindersGiftTimer;

    std::vector<uint32> spawnSpellsVector;

    void Reset() override
    {
        m_uiBerserkTimer            = 10 * MINUTE * IN_MILLISECONDS;

        m_uiAlliesNatureTimer       = 10000;
        m_uiAlliesWaveCount         = 0;
        m_uiNatureBombTimer         = 65000;
        m_uiSunbeamTimer            = 20000;
        m_uiLifebindersGiftTimer    = 25000;

        // make the spawn spells random
        std::random_shuffle(spawnSpellsVector.begin(), spawnSpellsVector.end());
    }

    void Aggro(Unit* /*pWho*/) override
    {
        // don't attack again after being defeated
        if (m_bEventFinished)
            return;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FREYA, IN_PROGRESS);

        DoCastSpellIfCan(m_creature, SPELL_ATTUNED_TO_NATURE, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TOUCH_OF_EONAR : SPELL_TOUCH_OF_EONAR_H, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);

        DoScriptText(SAY_AGGRO, m_creature);
        FetchElders();
    }

    void AttackStart(Unit* pWho) override
    {
        // don't attack again after being defeated
        if (m_bEventFinished)
            return;

        ScriptedAI::AttackStart(pWho);
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        // don't attack again after being defeated
        if (m_bEventFinished)
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FREYA, FAIL);
    }

    void EnterEvadeMode() override
    {
        m_creature->RemoveAllAurasOnEvade();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);

        if (m_creature->isAlive() && !m_bEventFinished)
            m_creature->GetMotionMaster()->MoveTargetedHome();

        m_creature->SetLootRecipient(NULL);

        Reset();
    }

    void DamageTaken(Unit* /*pDoneBy*/, uint32& uiDamage) override
    {
        if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;

            if (!m_bEventFinished)
            {
                if (m_pInstance)
                {
                    m_pInstance->SetData(TYPE_FREYA, DONE);

                    // spawn chest loot
                    switch (m_pInstance->GetData(TYPE_FREYA_HARD))
                    {
                        case 0: DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SUMMON_CHEST_0 : SPELL_SUMMON_CHEST_0_H, CAST_TRIGGERED); break;
                        case 1: DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SUMMON_CHEST_1 : SPELL_SUMMON_CHEST_1_H, CAST_TRIGGERED); break;
                        case 2: DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SUMMON_CHEST_2 : SPELL_SUMMON_CHEST_2_H, CAST_TRIGGERED); break;
                        case 3: DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SUMMON_CHEST_3 : SPELL_SUMMON_CHEST_3_H, CAST_TRIGGERED); break;
                    }

                    // check aura stacks for achiev
                    if (SpellAuraHolder* pNatureAura = m_creature->GetSpellAuraHolder(SPELL_ATTUNED_TO_NATURE))
                    {
                        if (pNatureAura && pNatureAura->GetStackAmount() >= MIN_ATTUNED_NATURE_STACKS)
                            m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_BACK_NATURE, true);
                    }
                }

                DoScriptText(SAY_EPILOGUE, m_creature);
                m_creature->CastSpell(m_creature, SPELL_FREYA_CREDIT, true);

                m_uiEpilogueTimer = 10000;
                m_bEventFinished = true;
                EnterEvadeMode();
            }
        }
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_EONARS_GIFT:
                pSummoned->CastSpell(pSummoned, SPELL_LIFEBINDERS_GIFT_VISUAL, true);
                pSummoned->CastSpell(pSummoned, SPELL_AUTO_GROW, true);
                pSummoned->CastSpell(pSummoned, SPELL_PHEROMONES, true);
                break;
            case NPC_DETONATING_LASHER:
            case NPC_ANCIENT_CONSERVATOR:
            case NPC_WATER_SPIRIT:
            case NPC_STORM_LASHER:
            case NPC_SNAPLASHER:
                pSummoned->AI()->AttackStart(m_creature->getVictim());
                break;
            case NPC_NATURE_BOMB:
                pSummoned->CastSpell(pSummoned, SPELL_NATURE_BOMB_GO, true);
                break;
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned) override
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_DETONATING_LASHER:
                pSummoned->CastSpell(m_creature, SPELL_ATTUNED_2_STACKS, true);
                break;
            case NPC_ANCIENT_CONSERVATOR:
                pSummoned->CastSpell(m_creature, SPELL_ATTUNED_25_STACKS, true);
                break;
            case NPC_WATER_SPIRIT:
            case NPC_STORM_LASHER:
            case NPC_SNAPLASHER:
                pSummoned->CastSpell(m_creature, SPELL_ATTUNED_10_STACKS, true);
                break;
        }
    }

    void ReceiveAIEvent(AIEventType eventType, Creature* /*pSender*/, Unit* /*pInvoker*/, uint32 /*uiMiscValue*/) override
    {
        // handle Allies of Nature spawn
        if (eventType == AI_EVENT_CUSTOM_A)
        {
            // adjust the index to the size of the vector
            uint8 uiIndex = m_uiAlliesWaveCount;
            if (uiIndex >= MAX_ALLIES_SPELLS)
                uiIndex = m_uiAlliesWaveCount - MAX_ALLIES_SPELLS;

            switch (spawnSpellsVector[uiIndex])
            {
                case SPELL_SUMMON_WAVE_1:  DoScriptText(SAY_ADDS_CONSERVATOR, m_creature); break;
                case SPELL_SUMMON_WAVE_3:  DoScriptText(SAY_ADDS_TRIO, m_creature);        break;
                case SPELL_SUMMON_WAVE_10: DoScriptText(SAY_ADDS_LASHER, m_creature);      break;
            }

            DoCastSpellIfCan(m_creature, spawnSpellsVector[uiIndex], CAST_TRIGGERED);

            ++m_uiAlliesWaveCount;

            // re-shuffle the spells
            if (m_uiAlliesWaveCount == MAX_ALLIES_SPELLS)
            {
                uint32 uiLastSpell = spawnSpellsVector[MAX_ALLIES_SPELLS - 1];
                std::random_shuffle(spawnSpellsVector.begin(), spawnSpellsVector.end());

                // make sure we won't repeat the last spell
                while (spawnSpellsVector[0] == uiLastSpell)
                    std::random_shuffle(spawnSpellsVector.begin(), spawnSpellsVector.end());
            }
        }
    }

    // check for all elders alive
    void FetchElders()
    {
        if (!m_pInstance)
            return;

        uint8 uiEldersAlive = 0;

        if (Creature* pElder = m_pInstance->GetSingleCreatureFromStorage(NPC_ELDER_BRIGHTLEAF))
        {
            if (pElder->isAlive())
                ++uiEldersAlive;
        }
        if (Creature* pElder = m_pInstance->GetSingleCreatureFromStorage(NPC_ELDER_IRONBRACH))
        {
            if (pElder->isAlive())
                ++uiEldersAlive;
        }
        if (Creature* pElder = m_pInstance->GetSingleCreatureFromStorage(NPC_ELDER_STONEBARK))
        {
            if (pElder->isAlive())
                ++uiEldersAlive;
        }

        // store the info about the elders alive
        m_pInstance->SetData(TYPE_FREYA_HARD, uiEldersAlive);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiEpilogueTimer)
        {
            if (m_uiEpilogueTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_TELEPORT, CAST_TRIGGERED) == CAST_OK)
                {
                    m_creature->ForcedDespawn(2000);
                    m_uiEpilogueTimer = 0;
                }
            }
            else
                m_uiEpilogueTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    m_uiBerserkTimer = 0;
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        if (m_uiAlliesWaveCount < MAX_ALLIES_WAVES)
        {
            if (m_uiAlliesNatureTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SUMMON_ALLIES_OF_NATURE : SPELL_SUMMON_ALLIES_OF_NATURE_H) == CAST_OK)
                {
                    DoScriptText(EMOTE_ALLIES_NATURE, m_creature);
                    m_uiAlliesNatureTimer = 60000;
                }
            }
            else
                m_uiAlliesNatureTimer -= uiDiff;
        }
        else
        {
            if (m_uiNatureBombTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_NATURE_BOMB_SUMMON) == CAST_OK)
                    m_uiNatureBombTimer = 15000;
            }
            else
                m_uiNatureBombTimer -= uiDiff;
        }

        if (m_uiSunbeamTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_SUNBEAM : SPELL_SUNBEAM_H) == CAST_OK)
                    m_uiSunbeamTimer = 15000;
            }
        }
        else
            m_uiSunbeamTimer -= uiDiff;

        if (m_uiLifebindersGiftTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_LIFEBINDERS_GIFT_SUMMON) == CAST_OK)
            {
                DoScriptText(EMOTE_LIFEBINDER, m_creature);
                m_uiLifebindersGiftTimer = 40000;
            }
        }
        else
            m_uiLifebindersGiftTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_freya(Creature* pCreature)
{
    return new boss_freyaAI(pCreature);
}

bool EffectScriptEffectCreature_boss_freya(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget, ObjectGuid /*originalCasterGuid*/)
{
    if ((uiSpellId == SPELL_SUMMON_ALLIES_OF_NATURE || uiSpellId == SPELL_SUMMON_ALLIES_OF_NATURE_H) && uiEffIndex == EFFECT_INDEX_0)
    {
        if (pCreatureTarget->GetEntry() == NPC_FREYA)
            pCreatureTarget->AI()->SendAIEvent(AI_EVENT_CUSTOM_A, pCaster, pCreatureTarget);

        return true;
    }

    return false;
}

/*######
## npc_eonars_gift
######*/

struct MANGOS_DLL_DECL npc_eonars_giftAI : public Scripted_NoMovementAI
{
    npc_eonars_giftAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiGiftTimer;

    void Reset() override
    {
        m_uiGiftTimer = 10000;
    }

    void AttackStart(Unit* /*pWho*/) override { }
    void MoveInLineOfSight(Unit* /*pWho*/) override { }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiGiftTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LIFEBINDERS_GIFT : SPELL_LIFEBINDERS_GIFT_H) == CAST_OK)
                m_uiGiftTimer = 10000;
        }
        else
            m_uiGiftTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_eonars_gift(Creature* pCreature)
{
    return new npc_eonars_giftAI(pCreature);
}

/*######
## npc_nature_bomb
######*/

struct MANGOS_DLL_DECL npc_nature_bombAI : public Scripted_NoMovementAI
{
    npc_nature_bombAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiNatureBombTimer;

    ObjectGuid m_natureBombGuid;

    void Reset() override
    {
        m_uiNatureBombTimer = 10000;
    }

    void JustSummoned(GameObject* pGo) override
    {
        m_natureBombGuid = pGo->GetObjectGuid();
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiNatureBombTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_NATURE_BOMB : SPELL_NATURE_BOMB_H) == CAST_OK)
            {
                if (GameObject* pBomb = m_creature->GetMap()->GetGameObject(m_natureBombGuid))
                    pBomb->Use(m_creature);

                m_creature->ForcedDespawn(2000);
                m_uiNatureBombTimer = 10000;
            }
        }
        else
            m_uiNatureBombTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_nature_bomb(Creature* pCreature)
{
    return new npc_nature_bombAI(pCreature);
}

void AddSC_boss_freya()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_freya";
    pNewScript->GetAI = GetAI_boss_freya;
    pNewScript->pEffectScriptEffectNPC = &EffectScriptEffectCreature_boss_freya;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_eonars_gift";
    pNewScript->GetAI = GetAI_npc_eonars_gift;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_nature_bomb";
    pNewScript->GetAI = GetAI_npc_nature_bomb;
    pNewScript->RegisterSelf();
}
