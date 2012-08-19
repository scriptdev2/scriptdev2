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
SDName: boss_valithria
SD%Complete: 20%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum SvalnaTexts                                            // TODO Maybe need own file?
{
    SAY_SVALNA_EVENT_1          = -1631130,
    SAY_SVALNA_EVENT_2          = -1631131,
    SAY_SVALNA_EVENT_3          = -1631132,
    SAY_SVALNA_EVENT_4          = -1631133,
    SAY_KILLING_CRUSADERS       = -1631134,
    SAY_RESSURECT               = -1631135,
    SAY_SVALNA_AGGRO            = -1631136,
    SAY_KILL_CAPTAIN            = -1631137,
    SAY_KILL_PLAYER             = -1631138,
    SAY_DEATH                   = -1631139,
};

enum
{
    SAY_AGGRO                   = -1631140,
    SAY_PORTAL                  = -1631141,
    SAY_75_HEALTH               = -1631142,
    SAY_25_HEALTH               = -1631143,
    SAY_0_HEALTH                = -1631144,
    SAY_PLAYER_DIES             = -1631145,
    SAY_BERSERK                 = -1631146,
    SAY_VICTORY                 = -1631147,
};

// Valithria
enum
{
    SPELL_NIGHTMARE_PORTAL_PRE      = 71977,
    SPELL_NIGHTMARE_PORTAL          = 71987,
    SPELL_TWISTED_NIGHTMARES        = 71941,
    SPELL_TWISTED_NIGHTMARES_DOT    = 71940,
    SPELL_NIGHTMARE_CLOUD           = 71970, // Nightmare Clouds cast this on dreaming Valithria - then she deals this dmg to Real Valithria(?)
    SPELL_NIGHTMARE_CLOUD_VISUAL    = 71939,

    SPELL_DREAM_PORTAL_PRE          = 71301,
    SPELL_DREAM_PORTAL              = 71305,
    SPELL_EMERALD_VIGOR             = 70873,
    SPELL_DREAM_CLOUD_VISUAL        = 70876,

    SPELL_DREAM_STATE               = 70766,

    SPELL_DREAMWALKER_RAGE          = 71189,
    SPELL_IMMUNITY                  = 72724,
    SPELL_CORRUPTION                = 70904,
    SPELL_DREAM_SLIP                = 71196,
    SPELL_ICE_SPIKE                 = 70702,

    SPELL_CLEAR_DREAMS_NIGHTMARES   = 75863, // script effect removes Emerald Vigor and Nightmare auras

    // summons
    // TODO:
    // these spells should be applied to dummy npcs in gates
    // they should apply these auras once the encounter has started
    // but how to apply this, which spell on which npc and when?
    // how to handle summon timers speedup?
    SPELL_SUMMON_TIMER_SUPPRESSER   = 70912,
    SPELL_SUMMON_TIMER_SKELETON     = 70913,
    SPELL_SUMMON_TIMER_ZOMBIE       = 70914,
    SPELL_SUMMON_TIMER_ABOMIN       = 70915,
    SPELL_SUMMON_TIMER_ARCHMAGE     = 70916,

    // entries
    NPC_NIGHTMARE_PORTAL_PRE        = 38429,
    NPC_NIGHTMARE_PORTAL            = 38430,
    NPC_NIGHTMARE_CLOUD             = 38421,
    NPC_DREAM_PORTAL_PRE            = 38186,
    NPC_DREAM_PORTAL                = 37945,
    NPC_DREAM_CLOUD                 = 37985,

    // Achievements
    SPELL_ACHIEVEMENT_CREDIT        = 72706,

    // other
    SUMMON_TYPES_NUMBER             = 4
};

// adds
enum
{
    // Risen Archmage
    SPELL_FROSTBOLT_VOLLEY          = 70759,
    SPELL_MANA_VOID                 = 71179,
    SPELL_VOID_BUFF                 = 71085,
    SPELL_COLUMN_OF_FROST           = 70704,
    SPELL_COLUMN_OF_FROST_VISUAL    = 70715,
    SPELL_COLUMN_OF_FROST_TRIG      = 70702,

    // Blazing Skeleton
    SPELL_FIREBALL                  = 70754,
    SPELL_LAY_WASTE                 = 69325,

    // Suppresser
    SPELL_SUPPRESSION               = 70588, // NOTE: must stack from many casters

    // Blistering Zombie
    SPELL_CORROSION                 = 70749,
    SPELL_ACID_BURST                = 70744, // 750ms cast time. seems like it is cast always before dying

    // Gluttonous Abomination
    SPELL_GUT_SPRAY                 = 71283,
    SPELL_ROT_WORM_SPAWNER          = 70675, // cast around 30% HP or something?
    SPELL_ROT_WORM                  = 72962,

    // entries
    NPC_RISEN_ARCHMAGE              = 37868,
    NPC_COLUMN_OF_FROST             = 37918,
    NPC_MANA_VOID                   = 38068,
    NPC_SUPPRESSER                  = 37863,
    NPC_BLAZING_SKELETON            = 36791,
    NPC_BLISTERING_ZOMBIE           = 37934,
    NPC_GLUTTONOUS_ABOMINATION      = 37886,
    NPC_ROT_WORM                    = 37907
};

static uint32 m_uiSummonedAddsEntries[SUMMON_TYPES_NUMBER] =
{
    NPC_GLUTTONOUS_ABOMINATION,
    NPC_RISEN_ARCHMAGE,
    NPC_BLISTERING_ZOMBIE,
    NPC_BLAZING_SKELETON
};

static const float fValithriaPosition[5][3] =
{
    {4203.470215f, 2484.500000f, 364.872009f},  // 0 Valithria
    {4240.688477f, 2405.794678f, 364.868591f},  // 1 Valithria Room 1
    {4239.579102f, 2566.753418f, 364.868439f},  // 2 Valithria Room 2
    {4166.216797f, 2564.197266f, 364.873047f},  // 3 Valithria Room 3
    {4165.112305f, 2405.872559f, 364.872925f}   // 4 Valithria Room 4
};

struct MANGOS_DLL_DECL boss_valithria_dreamwalkerAI : public ScriptedAI
{
    boss_valithria_dreamwalkerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    void Reset()
    {
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VALITHRIA, FAIL);
    }

    // actually, when summoned creature kills a player
    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_PLAYER_DIES, m_creature, pVictim);
    }

    void JustDied(Unit *pKiller)
    {
        DoScriptText(SAY_0_HEALTH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VALITHRIA, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_valithria_dreamwalker(Creature* pCreature)
{
    return new boss_valithria_dreamwalkerAI(pCreature);
};

// Gluttonous Abomination
struct MANGOS_DLL_DECL mob_gluttonous_abominationAI : public ScriptedAI
{
    mob_gluttonous_abominationAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiGutSprayTimer;

    void Reset()
    {
        m_uiGutSprayTimer = urand(3000, 5000);
    }

    void JustDied(Unit *pKiller)
    {
        DoCastSpellIfCan(m_creature, SPELL_ROT_WORM_SPAWNER, CAST_TRIGGERED);
    }

    void JustSummoned(Creature *pCreature)
    {
        pCreature->SetInCombatWithZone();
        pCreature->CastSpell(pCreature, SPELL_ROT_WORM, true);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Gut Spray
        if (m_uiGutSprayTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_GUT_SPRAY) == CAST_OK)
                m_uiGutSprayTimer = urand(5000, 10000);
        }
        else
            m_uiGutSprayTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_gluttonous_abomination(Creature *pCreature)
{
    return new mob_gluttonous_abominationAI(pCreature);
};

// Blistering Zombie
struct MANGOS_DLL_DECL mob_blistering_zombieAI : public ScriptedAI
{
    mob_blistering_zombieAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    bool m_bHasDied;

    void Reset()
    {
        m_bHasDied = false;
    }

    void Aggro(Unit *pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_CORROSION, CAST_TRIGGERED);
    }

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            
            if (!m_bHasDied)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ACID_BURST) == CAST_OK)
                {
                    SetCombatMovement(false);
                    m_bHasDied = true;
                    m_creature->ForcedDespawn(2000);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bHasDied)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_blistering_zombie(Creature *pCreature)
{
    return new mob_blistering_zombieAI(pCreature);
};

// Risen Archmage
struct MANGOS_DLL_DECL mob_risen_archmageAI : public ScriptedAI
{
    mob_risen_archmageAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiFrostboltVolleyTimer;
    uint32 m_uiColumnOfFrostTimer;
    uint32 m_uiManaVoidTimer;

    void Reset()
    {
        m_uiFrostboltVolleyTimer = urand(3000, 5000);
        m_uiColumnOfFrostTimer = urand(3000, 5000);
        m_uiManaVoidTimer = urand(5000, 10000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Frostbolt Volley
        if (m_uiFrostboltVolleyTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FROSTBOLT_VOLLEY) == CAST_OK)
                m_uiFrostboltVolleyTimer = urand(5000, 15000);
        }
        else
            m_uiFrostboltVolleyTimer -= uiDiff;

        // Column of Frost
        if (m_uiColumnOfFrostTimer <= uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_COLUMN_OF_FROST, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_COLUMN_OF_FROST) == CAST_OK)
                    m_uiColumnOfFrostTimer = urand(10000, 15000);
            }
        }
        else
            m_uiColumnOfFrostTimer -= uiDiff;

        // Mana Void
        if (m_uiManaVoidTimer <= uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_MANA_VOID, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_MANA_VOID) == CAST_OK)
                    m_uiManaVoidTimer = urand(10000, 15000);
            }
        }
        else
            m_uiManaVoidTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_risen_archmage(Creature *pCreature)
{
    return new mob_risen_archmageAI(pCreature);
};

// Blazing Skeleton
struct MANGOS_DLL_DECL mob_blazing_skeletonAI : public ScriptedAI
{
    mob_blazing_skeletonAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiFireballTimer;
    uint32 m_uiLayWasteTimer;

    void Reset()
    {
        m_uiFireballTimer = urand(3000, 5000);
        m_uiLayWasteTimer = urand(15000, 30000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Fireball
        if (m_uiFireballTimer <= uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_FIREBALL, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_FIREBALL) == CAST_OK)
                    m_uiFireballTimer = urand(3000, 5000);
            }
        }
        else
            m_uiFireballTimer -= uiDiff;

        // Lay Waste
        if (m_uiLayWasteTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_LAY_WASTE) == CAST_OK)
                m_uiLayWasteTimer = urand(20000, 25000);
        }
        else
            m_uiLayWasteTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_blazing_skeleton(Creature *pCreature)
{
    return new mob_blazing_skeletonAI(pCreature);
};

// Suppresser
struct MANGOS_DLL_DECL mob_suppresserAI : public ScriptedAI
{
    mob_suppresserAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
    }

    instance_icecrown_citadel* m_pInstance;

    void Reset(){}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_VALITHRIA) == FAIL)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->getVictim()->GetEntry() == NPC_VALITHRIA)
        {
            if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SUPPRESSION);
        }
    }
};

CreatureAI* GetAI_mob_suppresser(Creature *pCreature)
{
    return new mob_suppresserAI(pCreature);
};

void AddSC_boss_valithria_dreamwalker()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_valithria_dreamwalker";
    pNewscript->GetAI = &GetAI_boss_valithria_dreamwalker;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_gluttonous_abomination";
    pNewscript->GetAI = &GetAI_mob_gluttonous_abomination;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_blistering_zombie";
    pNewscript->GetAI = &GetAI_mob_blistering_zombie;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_risen_archmage";
    pNewscript->GetAI = &GetAI_mob_risen_archmage;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_blazing_skeleton";
    pNewscript->GetAI = &GetAI_mob_blazing_skeleton;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_suppresser";
    pNewscript->GetAI = &GetAI_mob_suppresser;
    pNewscript->RegisterSelf();
}
