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
SDName: Boss_Malacrass
SD%Complete: 70
SDComment: Contain adds and adds selection; Stolen abilities timers need improvement
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"

enum
{
    SAY_AGGRO                   = -1568045,
    SAY_ENRAGE                  = -1568046,
    SAY_KILL1                   = -1568047,
    SAY_KILL2                   = -1568048,
    SAY_SOUL_SIPHON             = -1568049,
    SAY_DRAIN_POWER             = -1568050,
    SAY_SPIRIT_BOLTS            = -1568051,
    SAY_ADD_DIED1               = -1568052,
    SAY_ADD_DIED2               = -1568053,
    SAY_ADD_DIED3               = -1568054,
    SAY_DEATH                   = -1568055,

    /* Notes about the event:
     * The boss casts siphon soul right after he finishes the spirit bolts channel, which takes 10 sec
     * The siphon soul is a channeled spell for 30 sec during which the boss uses some class abilities of the target
     * Basically the boss casts a dummy spell which chooses a random target on which it casts the actuall channel spell
     * The drain power spell acts as a enrage timer. It's cast each 30 seconds after the boss' health is below 80%
     */
    SPELL_SPIRIT_BOLTS          = 43383,
    SPELL_SIPHON_SOUL_DUMMY     = 43498,
    SPELL_SIPHON_SOUL           = 43501,
    SPELL_DRAIN_POWER           = 44131,

    //for various powers he uses after using soul drain
    //Death Knight
    SPELL_DK_DEATH_AND_DECAY    = 61603,
    SPELL_DK_PLAGUE_STRIKE      = 61600,
    SPELL_DK_MARK_OF_BLOOD      = 61606,

    //Druid
    SPELL_DR_THORNS             = 43420,
    SPELL_DR_LIFEBLOOM          = 43421,
    SPELL_DR_MOONFIRE           = 43545,

    //Hunter
    SPELL_HU_EXPLOSIVE_TRAP     = 43444,
    SPELL_HU_FREEZING_TRAP      = 43447,
    SPELL_HU_SNAKE_TRAP         = 43449,

    //Mage
    SPELL_MG_FIREBALL           = 41383,
    SPELL_MG_FROST_NOVA         = 43426,
    SPELL_MG_ICE_LANCE          = 43427,
    SPELL_MG_FROSTBOLT          = 43428,

    //Paladin
    SPELL_PA_CONSECRATION       = 43429,
    SPELL_PA_AVENGING_WRATH     = 43430,
    SPELL_PA_HOLY_LIGHT         = 43451,

    //Priest
    SPELL_PR_HEAL               = 41372,
    SPELL_PR_MIND_BLAST         = 41374,
    SPELL_PR_SW_DEATH           = 41375,
    SPELL_PR_PSYCHIC_SCREAM     = 43432,
    SPELL_PR_MIND_CONTROL       = 43550,
    SPELL_PR_PAIN_SUPP          = 44416,

    //Rogue
    SPELL_RO_WOUND_POISON       = 39665,
    SPELL_RO_BLIND              = 43433,
    SPELL_RO_SLICE_DICE         = 43547,

    //Shaman
    SPELL_SH_CHAIN_LIGHT        = 43435,
    SPELL_SH_FIRE_NOVA          = 43436,
    SPELL_SH_HEALING_WAVE       = 43548,

    //Warlock
    SPELL_WL_CURSE_OF_DOOM      = 43439,
    SPELL_WL_RAIN_OF_FIRE       = 43440,
    SPELL_WL_UNSTABLE_AFFL      = 35183,

    //Warrior
    SPELL_WR_MORTAL_STRIKE      = 43441,
    SPELL_WR_WHIRLWIND          = 43442,
    SPELL_WR_SPELL_REFLECT      = 43443,

    //misc
    TARGET_TYPE_RANDOM          = 0,
    TARGET_TYPE_VICTIM          = 1,
    TARGET_TYPE_SELF            = 2,
    TARGET_TYPE_FRIENDLY        = 3,

    MAX_ACTIVE_ADDS             = 4
};

//Adds X positions
static float m_afAddPosX[4] = {128.279f, 123.261f, 112.084f, 106.473f};

const float ADD_POS_Y       = 921.279f;
const float ADD_POS_Z       = 33.889f;
const float ADD_ORIENT      = 1.527f;

struct SpawnGroup
{
    uint32 m_uiCreatureEntry;
    uint32 m_uiCreatureEntryAlt;
};

SpawnGroup m_auiSpawnEntry[] =
{
    {24240, 24241},                                         //Alyson Antille / Thurg
    {24242, 24243},                                         //Slither / Lord Raadan
    {24244, 24245},                                         //Gazakroth / Fenstalker
    {24246, 24247},                                         //Darkheart / Koragg
};

struct PlayerAbilityStruct
{
    uint32 m_uiSpellId;
    uint8 m_uiTargetType;
    uint32 m_uiInitialTimer, m_uiCooldown;
};

// Classes are in the same order as they are in DBC
static PlayerAbilityStruct m_aMalacrassStolenAbility[][4] =
{
    {   // 0* shadow priest - exception: it seems that the priest has two specs. We use this slot for the shadow priest
        {SPELL_PR_MIND_CONTROL,     TARGET_TYPE_RANDOM,   15000, 30000},
        {SPELL_PR_MIND_BLAST,       TARGET_TYPE_RANDOM,   23000, 30000},
        {SPELL_PR_SW_DEATH,         TARGET_TYPE_RANDOM,   5000,  16000}
    },
    {   // 1 warrior
        {SPELL_WR_SPELL_REFLECT,    TARGET_TYPE_SELF,     2000,  30000},
        {SPELL_WR_WHIRLWIND,        TARGET_TYPE_SELF,     10000, 30000},
        {SPELL_WR_MORTAL_STRIKE,    TARGET_TYPE_VICTIM,   6000,  15000}
    },
    {   // 2 paladin
        {SPELL_PA_CONSECRATION,     TARGET_TYPE_SELF,     10000, 30000},
        {SPELL_PA_HOLY_LIGHT,       TARGET_TYPE_FRIENDLY, 17000, 30000},
        {SPELL_PA_AVENGING_WRATH,   TARGET_TYPE_SELF,     0,     30000}
    },
    {   // 3 hunter
        {SPELL_HU_EXPLOSIVE_TRAP,   TARGET_TYPE_SELF,     12000, 30000},
        {SPELL_HU_FREEZING_TRAP,    TARGET_TYPE_SELF,     3000,  30000},
        {SPELL_HU_SNAKE_TRAP,       TARGET_TYPE_SELF,     21000, 30000}
    },
    {   // 4 rogue
        {SPELL_RO_WOUND_POISON,     TARGET_TYPE_VICTIM,   3000,  17000},
        {SPELL_RO_SLICE_DICE,       TARGET_TYPE_SELF,     17000, 30000},
        {SPELL_RO_BLIND,            TARGET_TYPE_RANDOM,   12000, 30000}
    },
    {   // 5 priest
        {SPELL_PR_PAIN_SUPP,        TARGET_TYPE_FRIENDLY, 24000, 30000},
        {SPELL_PR_HEAL,             TARGET_TYPE_FRIENDLY, 16000, 30000},
        {SPELL_PR_PSYCHIC_SCREAM,   TARGET_TYPE_RANDOM,   8000,  30000}
    },
    {   // 6 death knight
        {SPELL_DK_DEATH_AND_DECAY,  TARGET_TYPE_RANDOM,   25000, 30000},
        {SPELL_DK_PLAGUE_STRIKE,    TARGET_TYPE_VICTIM,   5000,  17000},
        {SPELL_DK_MARK_OF_BLOOD,    TARGET_TYPE_RANDOM,   14000, 30000}
    },
    {   // 7 shaman
        {SPELL_SH_FIRE_NOVA,        TARGET_TYPE_SELF,     25000, 30000},
        {SPELL_SH_HEALING_WAVE,     TARGET_TYPE_FRIENDLY, 15000, 30000},
        {SPELL_SH_CHAIN_LIGHT,      TARGET_TYPE_RANDOM,   4000,  16000}
    },
    {   // 8 mage
        {SPELL_MG_FIREBALL,         TARGET_TYPE_RANDOM,   8000,  30000},
        {SPELL_MG_FROSTBOLT,        TARGET_TYPE_RANDOM,   25000, 30000},
        {SPELL_MG_ICE_LANCE,        TARGET_TYPE_RANDOM,   2000,  18000},
        {SPELL_MG_FROST_NOVA,       TARGET_TYPE_SELF,     17000, 30000}
    },
    {   // 9 warlock
        {SPELL_WL_CURSE_OF_DOOM,    TARGET_TYPE_RANDOM,   0,     30000},
        {SPELL_WL_RAIN_OF_FIRE,     TARGET_TYPE_RANDOM,   16000, 30000},
        {SPELL_WL_UNSTABLE_AFFL,    TARGET_TYPE_RANDOM,   8000,  13000}
    },
    {   // 10 unused - no class in DBC here
    },
    {   // 11 druid
        {SPELL_DR_LIFEBLOOM,        TARGET_TYPE_FRIENDLY, 15000, 30000},
        {SPELL_DR_THORNS,           TARGET_TYPE_SELF,     0,     30000},
        {SPELL_DR_MOONFIRE,         TARGET_TYPE_RANDOM,   8000,  13000}
    }
};

struct MANGOS_DLL_DECL boss_malacrassAI : public ScriptedAI
{
    boss_malacrassAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_lAddsEntryList.clear();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpiritBoltsTimer;
    uint32 m_uiDrainPowerTimer;
    uint32 m_uiSiphonSoulTimer;
    uint32 m_uiPlayerAbilityTimer;
    uint8 m_uiPlayerClass;

    bool m_bCanUsePlayerSpell;

    std::vector<uint32> m_vPlayerSpellTimer;
    std::list<uint32> m_lAddsEntryList;
    ObjectGuid m_aAddGuid[MAX_ACTIVE_ADDS];

    void Reset()
    {
        m_uiSpiritBoltsTimer    = 30000;
        m_uiDrainPowerTimer     = 0;
        m_uiSiphonSoulTimer     = 40000;
        m_uiPlayerAbilityTimer  = 10000;
        m_uiPlayerClass         = 0;

        m_bCanUsePlayerSpell    = false;

        InitializeAdds();
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALACRASS, FAIL);

        for(uint8 i = 0; i < MAX_ACTIVE_ADDS; ++i)
        {
            if (Creature* pAdd = m_creature->GetMap()->GetCreature(m_aAddGuid[i]))
                pAdd->AI()->EnterEvadeMode();
        }
    }

    void InitializeAdds()
    {
        //not if m_creature are dead, so avoid
        if (!m_creature->isAlive())
            return;

        uint8 j = 0;

        //it's empty, so first time
        if (m_lAddsEntryList.empty())
        {
            //fill list with entries from creature array
            for(uint8 i = 0; i < MAX_ACTIVE_ADDS; ++i)
                m_lAddsEntryList.push_back(rand()%2 ? m_auiSpawnEntry[i].m_uiCreatureEntry : m_auiSpawnEntry[i].m_uiCreatureEntryAlt);

            //summon mobs from the list
            for(std::list<uint32>::iterator itr = m_lAddsEntryList.begin(); itr != m_lAddsEntryList.end(); ++itr)
            {
                if (Creature* pAdd = m_creature->SummonCreature((*itr), m_afAddPosX[j], ADD_POS_Y, ADD_POS_Z, ADD_ORIENT, TEMPSUMMON_CORPSE_DESPAWN, 0))
                    m_aAddGuid[j] = pAdd->GetObjectGuid();

                ++j;
            }
        }
        else
        {
            for(std::list<uint32>::iterator itr = m_lAddsEntryList.begin(); itr != m_lAddsEntryList.end(); ++itr)
            {
                Creature* pAdd = m_creature->GetMap()->GetCreature(m_aAddGuid[j]);

                //object already removed, not exist
                if (!pAdd)
                {
                    if (pAdd = m_creature->SummonCreature((*itr), m_afAddPosX[j], ADD_POS_Y, ADD_POS_Z, ADD_ORIENT, TEMPSUMMON_CORPSE_DESPAWN, 0))
                        m_aAddGuid[j] = pAdd->GetObjectGuid();
                }
                ++j;
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        AddsAttack(pWho);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_MALACRASS, IN_PROGRESS);
    }

    void AddsAttack(Unit* pWho)
    {
        for(uint8 i = 0; i < MAX_ACTIVE_ADDS; ++i)
        {
            if (Creature* pAdd = m_creature->GetMap()->GetCreature(m_aAddGuid[i]))
            {
                if (!pAdd->getVictim())
                    pAdd->AI()->AttackStart(pWho);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        CleanAdds();

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_MALACRASS, DONE);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        // Set the player's class when hit with soul siphon
        if (pTarget->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_SIPHON_SOUL)
        {
            m_uiPlayerClass = ((Player*)pTarget)->getClass();
            m_bCanUsePlayerSpell = true;

            // In case the player it's priest we can choose either a holy priest or a shadow priest
            if (m_uiPlayerClass == CLASS_PRIEST)
                m_uiPlayerClass = urand(0, 1) ? CLASS_PRIEST : 0;

            // Init the spell timers
            uint8 m_uiMaxSpells = m_uiPlayerClass == CLASS_MAGE ? 4 : 3;

            m_vPlayerSpellTimer.clear();
            m_vPlayerSpellTimer.reserve(m_uiMaxSpells);
            for (uint8 i = 0; i < m_uiMaxSpells; ++i)
                m_vPlayerSpellTimer.push_back(m_aMalacrassStolenAbility[m_uiPlayerClass][i].m_uiInitialTimer);
        }
    }

    void CleanAdds()
    {
        for(uint8 i = 0; i < MAX_ACTIVE_ADDS; ++i)
        {
            if (Creature* pAdd = m_creature->GetMap()->GetCreature(m_aAddGuid[i]))
            {
                pAdd->AI()->EnterEvadeMode();
                pAdd->SetDeathState(JUST_DIED);
            }
        }

        for (uint8 i = 0; i < MAX_ACTIVE_ADDS; ++i)
            m_aAddGuid[i].Clear();

        m_lAddsEntryList.clear();
    }

    bool CanUseSpecialAbility(uint32 uiSpellIndex)
    {
        Unit* pTarget = NULL;

        switch(m_aMalacrassStolenAbility[m_uiPlayerClass][uiSpellIndex].m_uiTargetType)
        {
            case TARGET_TYPE_SELF:
                pTarget = m_creature;
                break;
            case TARGET_TYPE_VICTIM:
                pTarget = m_creature->getVictim();
                break;
            case TARGET_TYPE_RANDOM:
                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                break;
            case TARGET_TYPE_FRIENDLY:
                pTarget = DoSelectLowestHpFriendly(50.0f);
                break;
        }

        if (pTarget)
        {
            if (DoCastSpellIfCan(pTarget, m_aMalacrassStolenAbility[m_uiPlayerClass][uiSpellIndex].m_uiSpellId, CAST_TRIGGERED) == CAST_OK)
                return true;
        }

        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Acts as an enrage timer
        if (m_creature->GetHealthPercent() < 80.0f)
        {
            if (m_uiDrainPowerTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_DRAIN_POWER) == CAST_OK)
                {
                    DoScriptText(SAY_DRAIN_POWER, m_creature);
                    m_uiDrainPowerTimer = 30000;
                }
            }
            else
                m_uiDrainPowerTimer -= uiDiff;
        }

        if (m_uiSpiritBoltsTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SPIRIT_BOLTS) == CAST_OK)
            {
                DoScriptText(SAY_SPIRIT_BOLTS, m_creature);
                m_bCanUsePlayerSpell = false;
                m_uiSpiritBoltsTimer = 40000;
            }
        }
        else
            m_uiSpiritBoltsTimer -= uiDiff;

        if (m_uiSiphonSoulTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SIPHON_SOUL_DUMMY) == CAST_OK)
            {
                DoScriptText(SAY_SOUL_SIPHON, m_creature);
                m_uiSiphonSoulTimer = 40000;
            }
        }
        else
            m_uiSiphonSoulTimer -= uiDiff;

        // Use abilities only during the siphon soul phases
        if (m_bCanUsePlayerSpell)
        {
            // Loop through all abilities
            for (uint8 i = 0; i < m_vPlayerSpellTimer.size(); ++i)
            {
                if (m_vPlayerSpellTimer[i] < uiDiff)
                {
                    if (CanUseSpecialAbility(i))
                        m_vPlayerSpellTimer[i] = m_aMalacrassStolenAbility[m_uiPlayerClass][i].m_uiCooldown;
                }
                else
                    m_vPlayerSpellTimer[i] -= uiDiff;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_malacrass(Creature* pCreature)
{
    return new boss_malacrassAI(pCreature);
}

//common AI for adds
struct MANGOS_DLL_DECL boss_malacrass_addAI : public ScriptedAI
{
    boss_malacrass_addAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() { }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void MoveInLineOfSight(Unit* pWho)
    {
    }

    void KilledUnit(Unit* pVictim)
    {
        if (!m_pInstance)
            return;

        if (Creature* pMalacrass = m_pInstance->GetSingleCreatureFromStorage(NPC_MALACRASS))
            pMalacrass->AI()->KilledUnit(pVictim);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        if (Creature* pMalacrass = m_pInstance->GetSingleCreatureFromStorage(NPC_MALACRASS))
        {
            switch(urand(0, 2))
            {
                case 0: DoScriptText(SAY_ADD_DIED1, pMalacrass); break;
                case 1: DoScriptText(SAY_ADD_DIED2, pMalacrass); break;
                case 2: DoScriptText(SAY_ADD_DIED3, pMalacrass); break;
            }
        }
    }
};

enum
{
    SPELL_BLOODLUST = 43578,
    SPELL_CLEAVE    = 15496
};

struct MANGOS_DLL_DECL mob_thurgAI : public boss_malacrass_addAI
{
    mob_thurgAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiBloodlustTimer;
    uint32 m_uiCleaveTimer;

    void Reset()
    {
        m_uiBloodlustTimer = 15000;
        m_uiCleaveTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBloodlustTimer < uiDiff)
        {
            std::list<Creature*> lTempList = DoFindFriendlyMissingBuff(50.0f, SPELL_BLOODLUST);

            if (!lTempList.empty())
            {
                Unit* pTarget = *(lTempList.begin());
                DoCastSpellIfCan(pTarget, SPELL_BLOODLUST);
            }

            m_uiBloodlustTimer = 12000;
        }
        else
            m_uiBloodlustTimer -= uiDiff;

        if (m_uiCleaveTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 12000;
        }
        else
            m_uiCleaveTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_thurg(Creature* pCreature)
{
    return new mob_thurgAI(pCreature);
}

enum
{
    SPELL_ARCANE_TORRENT = 33390,
    SPELL_FLASH_HEAL     = 43575,
    SPELL_DISPEL_MAGIC   = 43577
};

const float RANGE_FRIENDLY_TARGET = 40.0;

struct MANGOS_DLL_DECL mob_alyson_antilleAI : public boss_malacrass_addAI
{
    mob_alyson_antilleAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiArcaneTorrentTimer;
    uint32 m_uiFlashHealTimer;
    uint32 m_uiDispelMagicTimer;

    void Reset()
    {
        m_uiArcaneTorrentTimer = 0;
        m_uiFlashHealTimer = 2500;
        m_uiDispelMagicTimer = 10000;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, false))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, 20.0f);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiArcaneTorrentTimer < uiDiff)
        {
            if (m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, uint32(0), SELECT_FLAG_PLAYER | SELECT_FLAG_IN_MELEE_RANGE))
            {
                DoCastSpellIfCan(m_creature, SPELL_ARCANE_TORRENT);
                m_uiArcaneTorrentTimer = 60000;
            }
            else
                m_uiArcaneTorrentTimer = 1000;
        }
        else
            m_uiArcaneTorrentTimer -= uiDiff;

        if (m_uiFlashHealTimer < uiDiff)
        {
            //this will fail if we previously was following target and pTarget is now different than before
            if (Unit* pTarget = DoSelectLowestHpFriendly(RANGE_FRIENDLY_TARGET*2, 30000))
            {
                if (pTarget->IsWithinDistInMap(m_creature, RANGE_FRIENDLY_TARGET))
                {
                    DoCastSpellIfCan(pTarget, SPELL_FLASH_HEAL);

                    //if not already chasing, start chase
                    if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != CHASE_MOTION_TYPE)
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim(), 20.0f);
                }
                else
                {
                    //if chasing, start follow target instead
                    if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
                    {
                        m_creature->GetMotionMaster()->MovementExpired();
                        m_creature->GetMotionMaster()->MoveFollow(pTarget, 20.0f, 0.0f);
                    }
                }
            }

            m_uiFlashHealTimer = 2500;
        }
        else
            m_uiFlashHealTimer -= uiDiff;

        if (m_uiDispelMagicTimer < uiDiff)
        {
            Unit* pTarget = NULL;
            std::list<Creature*> lTempList = DoFindFriendlyCC(RANGE_FRIENDLY_TARGET);

            if (!lTempList.empty())
                pTarget = *(lTempList.begin());
            else
                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

            if (pTarget)
                DoCastSpellIfCan(pTarget, SPELL_DISPEL_MAGIC);

            m_uiDispelMagicTimer = 12000;
        }
        else
            m_uiDispelMagicTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_alyson_antille(Creature* pCreature)
{
    return new mob_alyson_antilleAI(pCreature);
}

enum
{
    SPELL_FIREBOLT = 43584
};

struct MANGOS_DLL_DECL mob_gazakrothAI : public boss_malacrass_addAI
{
    mob_gazakrothAI(Creature* pCreature) : boss_malacrass_addAI(pCreature){ Reset(); }

    uint32 m_uiFireboltTimer;

    void Reset()
    {
        m_uiFireboltTimer = 1000;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, false))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, 20.0f);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFireboltTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIREBOLT);
            m_uiFireboltTimer = 1000;
        }
        else
            m_uiFireboltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_gazakroth(Creature* pCreature)
{
    return new mob_gazakrothAI(pCreature);
}

enum
{
    SPELL_FLAME_BREATH = 43582,
    SPELL_THUNDERCLAP  = 43583
};

struct MANGOS_DLL_DECL mob_lord_raadanAI : public boss_malacrass_addAI
{
    mob_lord_raadanAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiFlameBreathTimer;
    uint32 m_uiThunderclapTimer;

    void Reset()
    {
        m_uiFlameBreathTimer = 8000;
        m_uiThunderclapTimer = 13000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiThunderclapTimer < uiDiff)
        {
            if (m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0, SPELL_THUNDERCLAP, SELECT_FLAG_PLAYER))
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_THUNDERCLAP);
                m_uiThunderclapTimer = 12000;
            }
            else
                m_uiThunderclapTimer = 1000;
        }
        else
            m_uiThunderclapTimer -= uiDiff;

        if (m_uiFlameBreathTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAME_BREATH);
            m_uiFlameBreathTimer = 12000;
        }
        else
            m_uiFlameBreathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_lord_raadan(Creature* pCreature)
{
    return new mob_lord_raadanAI(pCreature);
}

enum
{
    SPELL_PSYCHIC_WAIL = 43590
};

struct MANGOS_DLL_DECL mob_darkheartAI : public boss_malacrass_addAI
{
    mob_darkheartAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiPsychicWailTimer;

    void Reset()
    {
        m_uiPsychicWailTimer = 8000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPsychicWailTimer < uiDiff)
        {
            if (m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0, uint32(0), SELECT_FLAG_PLAYER | SELECT_FLAG_IN_MELEE_RANGE))
            {
                DoCastSpellIfCan(m_creature, SPELL_PSYCHIC_WAIL);
                m_uiPsychicWailTimer = 12000;
            }
            else
                m_uiPsychicWailTimer = 1000;
        }
        else
            m_uiPsychicWailTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_darkheart(Creature* pCreature)
{
    return new mob_darkheartAI(pCreature);
}

enum
{
    SPELL_VENOM_SPIT = 43579
};

struct MANGOS_DLL_DECL mob_slitherAI : public boss_malacrass_addAI
{
    mob_slitherAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiVenomSpitTimer;

    void Reset()
    {
        m_uiVenomSpitTimer = 4000;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, false))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, 20.0f);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiVenomSpitTimer < uiDiff)
        {
            if (Unit* pVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pVictim, SPELL_VENOM_SPIT);

            m_uiVenomSpitTimer = 2500;
        }
        else
            m_uiVenomSpitTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_slither(Creature* pCreature)
{
    return new mob_slitherAI(pCreature);
}

enum
{
    SPELL_VOLATILE_INFECTION = 43586
};

struct MANGOS_DLL_DECL mob_fenstalkerAI : public boss_malacrass_addAI
{
    mob_fenstalkerAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiVolatileInfectionTimer;

    void Reset()
    {
        m_uiVolatileInfectionTimer = 15000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiVolatileInfectionTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_VOLATILE_INFECTION);
            m_uiVolatileInfectionTimer = 12000;
        }
        else
            m_uiVolatileInfectionTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_fenstalker(Creature* pCreature)
{
    return new mob_fenstalkerAI(pCreature);
}

enum
{
    SPELL_COLD_STARE  = 43593,
    SPELL_MIGHTY_BLOW = 43592,
};

struct MANGOS_DLL_DECL mob_koraggAI : public boss_malacrass_addAI
{
    mob_koraggAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiColdStareTimer;
    uint32 m_uiMightyBlowTimer;

    void Reset()
    {
        m_uiColdStareTimer = 15000;
        m_uiMightyBlowTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMightyBlowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            m_uiMightyBlowTimer = 12000;
        }
        else
            m_uiMightyBlowTimer -= uiDiff;

        if (m_uiColdStareTimer < uiDiff)
        {
            if (Unit* pVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pVictim, SPELL_COLD_STARE);

            m_uiColdStareTimer = 12000;
        }
        else
            m_uiColdStareTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_koragg(Creature* pCreature)
{
    return new mob_koraggAI(pCreature);
}

void AddSC_boss_malacrass()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_malacrass";
    pNewScript->GetAI = &GetAI_boss_malacrass;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_thurg";
    pNewScript->GetAI = &GetAI_mob_thurg;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_gazakroth";
    pNewScript->GetAI = &GetAI_mob_gazakroth;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_lord_raadan";
    pNewScript->GetAI = &GetAI_mob_lord_raadan;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_darkheart";
    pNewScript->GetAI = &GetAI_mob_darkheart;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_slither";
    pNewScript->GetAI = &GetAI_mob_slither;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_fenstalker";
    pNewScript->GetAI = &GetAI_mob_fenstalker;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_koragg";
    pNewScript->GetAI = &GetAI_mob_koragg;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_alyson_antille";
    pNewScript->GetAI = &GetAI_mob_alyson_antille;
    pNewScript->RegisterSelf();
}
