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
SDName: boss_thorim
SD%Complete: 50%
SDComment: Basic script
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO_1                             = -1603138,
    SAY_AGGRO_2                             = -1603139,
    SAY_SPECIAL_1                           = -1603140,
    SAY_SPECIAL_2                           = -1603141,
    SAY_SPECIAL_3                           = -1603142,
    SAY_JUMP                                = -1603143,

    SAY_SLAY_1                              = -1603144,
    SAY_SLAY_2                              = -1603145,
    SAY_BERSERK                             = -1603146,

    SAY_ARENA_WIPE                          = -1603147,
    SAY_DEFEATED                            = -1603148,

    SAY_OUTRO_1                             = -1603149,
    SAY_OUTRO_2                             = -1603150,
    SAY_OUTRO_3                             = -1603151,
    SAY_OUTRO_HARD_1                        = -1603152,
    SAY_OUTRO_HARD_2                        = -1603153,
    SAY_OUTRO_HARD_3                        = -1603154,

    SAY_SIF_BEGIN                           = -1603156,
    SAY_SIF_EVENT                           = -1603157,
    SAY_SIF_DESPAWN                         = -1603158,

    // phase 1 spells
    SPELL_SHEAT_OF_LIGHTNING                = 62276,
    SPELL_STORMHAMMER                       = 62042,
    SPELL_DEAFENING_THUNDER                 = 62470,
    SPELL_LIGHTNING_SHOCK                   = 62017,
    SPELL_CHARGE_ORB                        = 62016,
    SPELL_TOUCH_OF_DOMINION                 = 62507,                    // hard mode timer; triggers 62565 after 2.5 min
    SPELL_BERSERK_1                         = 62560,
    SPELL_SUMMON_LIGHTNING_ORB              = 62391,                    // on berserk
    SPELL_LIGHTNING_DESTRUCTION             = 62393,                    // cast by npc 33138 on berserk

    // phase 2 spells
    SPELL_CHAIN_LIGHTNING                   = 62131,
    SPELL_CHAIN_LIGHTNING_H                 = 64390,
    SPELL_LIGHTNING_CHARGE                  = 62279,
    SPELL_LIGHTNING_CHARGE_DUMMY            = 62466,                    // dummy effect hits npc 33378 and triggers spell 64098; cone target effect hits npc 32780
    SPELL_UNBALANCING_STRIKE                = 62130,
    SPELL_BERSERK_2                         = 62555,
    SPELL_THORIM_CREDIT                     = 64985,                    // kill credit spell; added in spell_template
    SPELL_STORMHAMMER_OUTRO                 = 64767,                    // target npc 33196 and trigger spells 62470, 64909 and 64778 and despawn target in 10 sec
    SPELL_TELEPORT                          = 62940,

    // Lightning related spells
    SPELL_ACTIVATE_LIGHTNING_ORB_PERIODIC   = 62184,                    // cast by npc 32879; starts the whole lightning event
    SPELL_LIGHTNING_ORG_CHARGED             = 62186,                    // cast by npc 33378; makes Thorim to cast 62466
    SPELL_LIGHTNING_PILLAR                  = 62976,                    // cast by npc 33378 to npc 33378 (not same guid); triggers 64098 on Thorim
    SPELL_LIGHTNING_FIELD                   = 64972,                    // cast by npc 32892

    // Sif spells
    SPELL_FROST_BOLT                        = 62583,
    SPELL_FROST_BOLT_H                      = 62601,
    SPELL_FROSTBOLT_VOLLEY                  = 62580,
    SPELL_FROSTBOLT_VOLLEY_H                = 62604,
    SPELL_FROST_NOVA                        = 62597,
    SPELL_FROST_NOVA_H                      = 62605,
    SPELL_BLIZZARD                          = 62576,
    SPELL_BLIZZARD_H                        = 62602,

    // Colossus spells
    SPELL_SMASH                             = 62339,
    //SPELL_SMASH_RIGHT                     = 62414,
    SPELL_RUNIC_SMASH_L                     = 62058,
    SPELL_RUNIC_SMASH_R                     = 62057,
    SPELL_RUNIC_BARRIER                     = 62338,
    SPELL_CHARGE                            = 62613,
    SPELL_CHARGE_H                          = 62614,

    // event npcs
    NPC_LIGHTNING_ORB                       = 33138,                    // spawned on arena berserk
    NPC_DARK_RUNE_CHAMPION                  = 32876,                    // arena npcs
    NPC_DARK_RUNE_WARBRINGER                = 32877,
    NPC_DARK_RUNE_EVOKER                    = 32878,
    NPC_DARK_RUNE_COMMONER                  = 32904,
    NPC_DARK_RUNE_ACOLYTE_ARENA             = 32886,
    NPC_IRON_RING_GUARD                     = 32874,                    // hallway npcs
    NPC_DARK_RUNE_ACOLYTE_HALLWAY           = 33110,
    NPC_IRON_HONOR_GUARD_1                  = 33125,                    // stairs npcs
    NPC_IRON_HONOR_GUARD_2                  = 32875,
    NPC_DARK_RUNE_ACOLYTE_STAIRS            = 32957,
    NPC_TRAP_BUNNY_1                        = 33725,                    // thorim traps; have auras 62241 and 63540
    NPC_TRAP_BUNNY_2                        = 33054,

    FACTION_ID_FRIENDLY                     = 35,
    PHASE_ARENA                             = 1,
    PHASE_SOLO                              = 2,
    PHASE_TRANSITION                        = 3,
};

static const DialogueEntry aThorimDialogue[] =
{
    {SAY_AGGRO_1,               NPC_THORIM,     9000},
    {SAY_AGGRO_2,               NPC_THORIM,     7000},
    {NPC_SIF,                   0,              5000},
    {SPELL_TOUCH_OF_DOMINION,   0,              0},
    {SAY_JUMP,                  NPC_THORIM,     10000},
    {PHASE_SOLO,                0,              0},
    {SAY_DEFEATED,              NPC_THORIM,     3000},
    {SAY_OUTRO_1,               NPC_THORIM,     10000},
    {SAY_OUTRO_2,               NPC_THORIM,     12000},
    {SAY_OUTRO_3,               NPC_THORIM,     10000},
    {SPELL_TELEPORT,            0,              0},
    {SPELL_STORMHAMMER_OUTRO,   0,              3000},
    {SAY_OUTRO_HARD_1,          NPC_THORIM,     6000},
    {SAY_OUTRO_HARD_2,          NPC_THORIM,     12000},
    {SAY_OUTRO_HARD_3,          NPC_THORIM,     10000},
    {SPELL_THORIM_CREDIT,       0,              0},
    {0, 0, 0},
};

static const float afSifSpawnLoc[4] = {2148.301f, -297.8453f, 438.3308f, 2.68f};
static const float afArenaCenterLoc[3] = {2134.8f, -263.056f, 419.983f};

/*######
## boss_thorim
######*/

struct MANGOS_DLL_DECL boss_thorimAI : public ScriptedAI, private DialogueHelper
{
    boss_thorimAI(Creature* pCreature) : ScriptedAI(pCreature),
        DialogueHelper(aThorimDialogue)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        InitializeDialogueHelper(m_pInstance);
        m_bEventFinished = false;
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bEventFinished;

    uint32 m_uiBerserkTimer;
    uint8 m_uiPhase;

    uint32 m_uiHardModeCheckTimer;

    void Reset() override
    {
        m_uiPhase               = PHASE_ARENA;
        m_uiBerserkTimer        = 5 * MINUTE * IN_MILLISECONDS;

        m_uiHardModeCheckTimer  = 0;

        SetCombatMovement(false);
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
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
        // switch to phase 2 as soon as it's hit by any damage
        if (m_uiPhase == PHASE_ARENA && uiDamage > 0)
        {
            StartNextDialogueText(SAY_JUMP);
            m_uiPhase = PHASE_TRANSITION;

            // prepare the hard mode if necessary
            if (m_pInstance && m_pInstance->GetData(TYPE_THORIM_HARD) != FAIL)
            {
                if (Creature* pSif = m_pInstance->GetSingleCreatureFromStorage(NPC_SIF))
                    pSif->InterruptNonMeleeSpells(false);

                m_pInstance->SetData(TYPE_THORIM_HARD, DONE);
            }
            return;
        }

        // handle outro
        if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;

            if (!m_bEventFinished)
            {
                if (m_pInstance)
                {
                    m_pInstance->SetData(TYPE_THORIM, DONE);

                    // start a different outro version for hard mode
                    if (m_pInstance->GetData(TYPE_THORIM_HARD) == DONE)
                        StartNextDialogueText(SPELL_STORMHAMMER_OUTRO);
                    else
                        StartNextDialogueText(SAY_DEFEATED);
                }

                m_creature->CastSpell(m_creature, SPELL_THORIM_CREDIT, true);
                m_creature->setFaction(FACTION_ID_FRIENDLY);
                m_bEventFinished = true;
                EnterEvadeMode();
            }
        }
    }

    void Aggro(Unit* pWho) override
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_THORIM, IN_PROGRESS);
            m_pInstance->SetData(TYPE_THORIM_HARD, NOT_STARTED);
        }

        StartNextDialogueText(SAY_AGGRO_1);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THORIM, FAIL);
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId) override
    {
        if (uiMotionType != EFFECT_MOTION_TYPE || !uiPointId)
            return;

        m_uiPhase = PHASE_SOLO;
        SetCombatMovement(true);
        DoStartMovement(m_creature->getVictim());
        m_creature->RemoveAurasDueToSpell(SPELL_SHEAT_OF_LIGHTNING);

        // make Sif attack too if hard mode is active
        if (m_pInstance && m_pInstance->GetData(TYPE_THORIM_HARD) == DONE)
        {
            if (Creature* pSif = m_pInstance->GetSingleCreatureFromStorage(NPC_SIF))
            {
                DoScriptText(SAY_SIF_EVENT, pSif);
                pSif->AI()->AttackStart(m_creature->getVictim());
            }
        }
    }

    void JustSummoned(Creature* pSummoned) override
    {
        // the lightning orb should clean out the whole hallway on arena berserk
        if (pSummoned->GetEntry() == NPC_LIGHTNING_ORB)
            pSummoned->CastSpell(pSummoned, SPELL_LIGHTNING_DESTRUCTION, true);
    }

    void JustDidDialogueStep(int32 iEntry) override
    {
        if (!m_pInstance)
            return;

        switch (iEntry)
        {
            case NPC_SIF:
                DoCastSpellIfCan(m_creature, SPELL_SHEAT_OF_LIGHTNING, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
                if (Creature* pSif = m_creature->SummonCreature(NPC_SIF, afSifSpawnLoc[0], afSifSpawnLoc[1], afSifSpawnLoc[2], afSifSpawnLoc[3], TEMPSUMMON_CORPSE_DESPAWN, 0))
                    DoScriptText(SAY_SIF_BEGIN, pSif);
                break;
            case SPELL_TOUCH_OF_DOMINION:
                if (Creature* pSif = m_pInstance->GetSingleCreatureFromStorage(NPC_SIF))
                    pSif->CastSpell(m_creature, SPELL_TOUCH_OF_DOMINION, false);
                m_uiHardModeCheckTimer = 150000;
                break;
            case PHASE_SOLO:
                m_creature->GetMotionMaster()->MoveJump(afArenaCenterLoc[0], afArenaCenterLoc[1], afArenaCenterLoc[2], 45.55969f, 5.0f, 1);
                break;
            case SPELL_STORMHAMMER_OUTRO:
                DoScriptText(SAY_DEFEATED, m_creature);
                break;
            case SAY_OUTRO_HARD_1:
                DoCastSpellIfCan(m_creature, SPELL_STORMHAMMER_OUTRO);
                break;
            case SPELL_TELEPORT:
            case SPELL_THORIM_CREDIT:
                if (DoCastSpellIfCan(m_creature, SPELL_TELEPORT) == CAST_OK)
                    m_creature->ForcedDespawn(2000);
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        DialogueUpdate(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_uiPhase)
        {
            // arena phase abilities
            case PHASE_ARENA:

                if (m_uiHardModeCheckTimer)
                {
                    if (m_uiHardModeCheckTimer <= uiDiff)
                    {
                        // hard mode is failed; despawn Sif
                        if (m_pInstance)
                        {
                            m_pInstance->SetData(TYPE_THORIM_HARD, FAIL);

                            if (Creature* pSif = m_pInstance->GetSingleCreatureFromStorage(NPC_SIF))
                            {
                                DoScriptText(SAY_SIF_DESPAWN, pSif);
                                pSif->ForcedDespawn(1000);
                            }
                        }
                        m_uiHardModeCheckTimer = 0;
                    }
                    else
                        m_uiHardModeCheckTimer -= uiDiff;
                }

                if (m_uiBerserkTimer)
                {
                    if (m_uiBerserkTimer <= uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_BERSERK_1) == CAST_OK)
                        {
                            DoCastSpellIfCan(m_creature, SPELL_SUMMON_LIGHTNING_ORB, CAST_TRIGGERED);
                            DoScriptText(SAY_ARENA_WIPE, m_creature);
                            m_uiBerserkTimer = 0;
                        }
                    }
                    else
                        m_uiBerserkTimer -= uiDiff;
                }

                // no break; same combat abilities, but in transition
            case PHASE_TRANSITION:

                break;
                // solo phase abilities
            case PHASE_SOLO:

                if (m_uiBerserkTimer)
                {
                    if (m_uiBerserkTimer <= uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_BERSERK_2) == CAST_OK)
                        {
                            DoScriptText(SAY_BERSERK, m_creature);
                            m_uiBerserkTimer = 0;
                        }
                    }
                    else
                        m_uiBerserkTimer -= uiDiff;
                }

                DoMeleeAttackIfReady();
                break;
        }
    }
};

CreatureAI* GetAI_boss_thorim(Creature* pCreature)
{
    return new boss_thorimAI(pCreature);
}

void AddSC_boss_thorim()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_thorim";
    pNewScript->GetAI = GetAI_boss_thorim;
    pNewScript->RegisterSelf();
}
