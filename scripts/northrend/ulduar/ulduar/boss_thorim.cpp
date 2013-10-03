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
SD%Complete: 20%
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

    // phase 2 spells
    SPELL_CHAIN_LIGHTNING                   = 62131,
    SPELL_CHAIN_LIGHTNING_H                 = 64390,
    SPELL_LIGHTNING_CHARGE                  = 62279,
    SPELL_LIGHTNING_CHARGE_DUMMY            = 62466,                    // dummy effect hits npc 33378 and triggers spell 64098; cone target effect hits npc 32780
    SPELL_UNBALANCING_STRIKE                = 62130,
    SPELL_BERSERK_2                         = 62555,
    SPELL_THORIM_CREDIT                     = 64985,                    // kill credit spell; added in spell_template

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
    NPC_SIF                                 = 33196,
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
};

/*######
## boss_thorim
######*/

struct MANGOS_DLL_DECL boss_thorimAI : public ScriptedAI
{
    boss_thorimAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bEventFinished = false;
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bEventFinished;

    uint32 m_uiBerserkTimer;

    void Reset() override
    {
        m_uiBerserkTimer = 5 * MINUTE * IN_MILLISECONDS;

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
        if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;

            if (!m_bEventFinished)
            {
                if (m_pInstance)
                    m_pInstance->SetData(TYPE_THORIM, DONE);

                // ToDo: start outro dialogue
                m_creature->CastSpell(m_creature, SPELL_THORIM_CREDIT, true);

                m_creature->ForcedDespawn(10000);
                m_creature->setFaction(FACTION_ID_FRIENDLY);
                m_bEventFinished = true;
                EnterEvadeMode();
            }
        }
    }

    void Aggro(Unit* pWho) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THORIM, IN_PROGRESS);

        // ToDo: start intro dialogue
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THORIM, FAIL);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
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
