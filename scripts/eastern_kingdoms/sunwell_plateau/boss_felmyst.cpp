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
SDName: boss_felmyst
SD%Complete: 50%
SDComment: Only ground phase spells
SDCategory: Sunwell Plateau
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

enum
{
    SAY_INTRO           = -1580036,
    SAY_KILL_1          = -1580037,
    SAY_KILL_2          = -1580038,
    SAY_DEATH           = -1580042,
    SAY_TAKEOFF         = -1580040,
    SAY_BREATH          = -1580039,
    SAY_BERSERK         = -1580041,

    SPELL_FELBLAZE_VISUAL       = 45068,        // Visual transform aura
    SPELL_NOXIOUS_FUMES         = 47002,
    SPELL_BERSERK               = 26662,

    // ground phase
    SPELL_CLEAVE                = 19983,
    SPELL_CORROSION             = 45866,
    SPELL_GAS_NOVA              = 45855,
    SPELL_ENCAPSULATE           = 45665,
    SPELL_ENCAPSULATE_CHANNEL   = 45661,

    // flight phase
    SPELL_DEMONIC_VAPOR         = 45399,
    SPELL_VAPOR_BEAM_VISUAL     = 45389,
    SPELL_FOG_CORRUPTION        = 45582,
    SPELL_SOUL_SEVER            = 45918,        // kills all charmed targets at wipe - script effect for 45917
    SPELL_SUMMON_VAPOR          = 45391,
    SPELL_SUMMON_VAPOR_TRIAL    = 45410,
    SPELL_SUMMON_BLAZING_DEAD   = 45400,

    // npcs
    NPC_UNYELDING_DEAD          = 25268,        // spawned during flight phase
    NPC_DEMONIC_VAPOR           = 25265,
    NPC_DEMONIC_VAPOR_TRAIL     = 25267,

    // phases
    PHASE_GROUND                = 1,
    PHASE_AIR                   = 2,
};

struct MANGOS_DLL_DECL boss_felmystAI : public ScriptedAI
{
    boss_felmystAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_sunwell_plateau*)pCreature->GetInstanceData();
        m_bHasTransformed = false;
        Reset();
    }

    instance_sunwell_plateau* m_pInstance;

    bool m_bHasTransformed;

    uint8 m_uiPhase;
    uint32 m_uiBerserkTimer;

    // Ground Phase timers
    uint32 m_uiFlyPhaseTimer;
    uint32 m_uiCorrosionTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiEncapsulateTimer;
    uint32 m_uiGasNovaTimer;

    void Reset()
    {
        // Transform into Felmyst dragon
        DoCastSpellIfCan(m_creature, SPELL_FELBLAZE_VISUAL);

        m_uiPhase               = PHASE_GROUND;
        m_uiBerserkTimer        = 10*MINUTE*IN_MILLISECONDS;

        // Ground Phase
        m_uiCorrosionTimer      = 30000;
        m_uiCleaveTimer         = urand(2000, 5000);
        m_uiGasNovaTimer        = 17000;
        m_uiEncapsulateTimer    = urand(30000, 40000);
        m_uiFlyPhaseTimer       = 60000;        // flight phase after 1 min
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTransformed)
        {
            if (pWho->GetTypeId() == TYPEID_PLAYER && pWho->IsWithinLOSInMap(m_creature) && pWho->IsWithinDistInMap(m_creature, 100.0f))
            {
                DoScriptText(SAY_INTRO, m_creature);
                m_bHasTransformed = true;
            }
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);

        // Add the visual aura back when evading - workaround because there is no way to remove only the negative auras
        DoCastSpellIfCan(m_creature, SPELL_FELBLAZE_VISUAL, CAST_TRIGGERED);

        if (m_creature->isAlive())
            m_creature->GetMotionMaster()->MoveTargetedHome();

        m_creature->SetLootRecipient(NULL);

        Reset();
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_NOXIOUS_FUMES);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FELMYST, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FELMYST, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FELMYST, FAIL);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pTarget->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_ENCAPSULATE_CHANNEL)
            pTarget->CastSpell(pTarget, SPELL_ENCAPSULATE, true, NULL, NULL, m_creature->GetObjectGuid());
    }

    void UpdateAI(const uint32 uiDiff)
    {
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

        if (m_uiPhase == PHASE_GROUND)
        {
            if (m_uiCleaveTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                    m_uiCleaveTimer = urand(2000, 5000);
            }
            else
                m_uiCleaveTimer -= uiDiff;

            if (m_uiCorrosionTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CORROSION) == CAST_OK)
                    m_uiCorrosionTimer = 30000;
            }
            else
                m_uiCorrosionTimer -= uiDiff;

            if (m_uiGasNovaTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_GAS_NOVA) == CAST_OK)
                    m_uiGasNovaTimer = 23000;
            }
            else
                m_uiGasNovaTimer -= uiDiff;

            if (m_uiEncapsulateTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_ENCAPSULATE_CHANNEL) == CAST_OK)
                        m_uiEncapsulateTimer = urand(30000, 40000);
                }
            }
            else
                m_uiEncapsulateTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        // ToDo: add flight phase here
    }
};

CreatureAI* GetAI_boss_felmyst(Creature* pCreature)
{
    return new boss_felmystAI(pCreature);
}

void AddSC_boss_felmyst()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_felmyst";
    pNewScript->GetAI = &GetAI_boss_felmyst;
    pNewScript->RegisterSelf();
}
