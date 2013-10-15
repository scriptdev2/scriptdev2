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
SDName: boss_mimiron
SD%Complete: 10%
SDComment: Very basic script.
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                               = -1603176,
    SAY_HARD_MODE                           = -1603177,
    SAY_BERSERK                             = -1603178,

    SAY_TANK_ACTIVE                         = -1603179,
    SAY_TANK_SLAY_1                         = -1603180,
    SAY_TANK_SLAY_2                         = -1603181,
    SAY_TANK_DEATH                          = -1603182,

    SAY_TORSO_ACTIVE                        = -1603183,
    SAY_TORSO_SLAY_1                        = -1603184,
    SAY_TORSO_SLAY_2                        = -1603185,
    SAY_TORSO_DEATH                         = -1603186,

    SAY_HEAD_ACTIVE                         = -1603187,
    SAY_HEAD_SLAY_1                         = -1603188,
    SAY_HEAD_SLAY_2                         = -1603189,
    SAY_HEAD_DEATH                          = -1603190,

    SAY_ROBOT_ACTIVE                        = -1603191,
    SAY_ROBOT_SLAY_1                        = -1603192,
    SAY_ROBOT_SLAY_2                        = -1603193,
    SAY_ROBOT_DEATH                         = -1603194,

    EMOTE_PLASMA_BLAST                      = -1603196,

    // generic spells
    SPELL_WELD                              = 63339,
    SPELL_TELEPORT_VISUAL                   = 41232,
    SPELL_RIDE_VEHICLE_MIMIRON              = 52391,                    // seat 0
    SPELL_RIDE_VEHICLE_MIMIRON_1            = 63313,                    // seat 1
    SPELL_RIDE_VEHICLE_MIMIRON_2            = 63314,                    // seat 2
    SPELL_RIDE_VEHICLE_MIMIRON_3            = 63315,                    // seat 3
    SPELL_RIDE_VEHICLE_MIMIRON_4            = 63316,                    // seat 4
    SPELL_RIDE_VEHICLE_MIMIRON_5            = 63344,                    // seat 5
    SPELL_RIDE_VEHICLE_MIMIRON_6            = 63345,                    // seat 6
    SPELL_JET_PACK                          = 63341,
    SPELL_MIMIRON_SLEEP_VISUAL              = 64393,
    SPELL_MIMIRON_SLEEP_VISUAL_2            = 64394,
    SPELL_SELF_REPAIR                       = 64383,
    SPELL_BERSERK                           = 26662,

    // Leviathan spells
    SPELL_FREEZE_ANIM                       = 16245,
    SPELL_PROXIMITY_MINES                   = 63027,                    // triggers 65347
    SPELL_NAPALM_SHELL                      = 64539,                    // triggers 63667 which casts 63666 or 65026
    SPELL_PLASMA_BLAST                      = 62997,                    // cast by the turret
    SPELL_PLASMA_BLAST_H                    = 64529,
    SPELL_SHOCK_BLAST                       = 63631,
    SPELL_FLAME_SUPPRESSANT                 = 64570,                    // hard mode spells
    SPELL_EMERGENCY_MODE_LEVIATHAN          = 65101,

    // VX001 spells
    SPELL_RAPID_BURST_SUMMON                = 64840,
    SPELL_RAPID_BURST_LEFT                  = 63387,                    // used in the VX phase; each spell has a different robot animation
    SPELL_RAPID_BURST_RIGHT                 = 64019,
    SPELL_RAPID_BURST_LEFT_H                = 64531,
    SPELL_RAPID_BURST_RIGHT_H               = 64532,

    SPELL_LASER_BARRAGE                     = 63274,
    SPELL_SPINNING_UP                       = 63414,                    // laser barrage spell for the final phase
    SPELL_ROCKET_STRIKE                     = 64402,                    // targets npc 34050; triggers 63681 and 63036 from target
    SPELL_HEAT_WAVE                         = 63679,
    SPELL_HEAT_WAVE_H                       = 64534,
    SPELL_HAND_PULSE_LEFT                   = 64348,                    // spells used only in the final phase
    SPELL_HAND_PULSE_RIGHT                  = 64352,                    // each as a different visual for robot hand animation
    SPELL_HAND_PULSE_LEFT_H                 = 64536,
    SPELL_HAND_PULSE_RIGHT_H                = 64537,
    SPELL_FLAME_SUPPRESSANT_CLOSE           = 65192,                    // hard mode spell
    SPELL_FROST_BOMB_SUMMON                 = 64627,                    // hard mode spell; summon npc 34149

    // Aerial unit spells
    SPELL_PLASMA_BALL_FLY                   = 63689,                    // used during the air phase
    SPELL_PLASMA_BALL_FLY_H                 = 64535,
    SPELL_PLASMA_BALL                       = 65647,                    // used during the final phase
    SPELL_PLASMA_BALL_H                     = 65648,
    SPELL_SUMMON_ASSAULT_BOT                = 64427,                    // summon npc 34057
    SPELL_SUMMON_SCRAP_BOT                  = 63819,                    // summon npc 33855
    SPELL_BOMB_BOT_SUMMON                   = 63811,                    // summon npc 33836
    SPELL_SUMMON_FIRE_BOT                   = 64622,                    // hard mode spell; summon npc 34147

    // hard mode spells
    SPELL_SELF_DESTRUCTION                  = 64613,
    SPELL_SELF_DESTRUCTION_DAMANGE          = 64610,
    SPELL_EMERGENCY_MODE                    = 64582,

    // fire spells
    SPELL_SUMMON_FLAMES_INITIAL             = 64563,                    // cast by npc 21252
    SPELL_FLAMES                            = 64561,                    // cast by npcs 34363 and 34121
    SPELL_SUMMON_FLAMES_SPREAD              = 64562,                    // cast by npc 34363

    // frost bomb spells
    SPELL_EXPLOSION                         = 64626,
    SPELL_FROST_BOMB_VISUAL                 = 64624,

    // summoned
    NPC_PROXIMITY_MINE                      = 34362,                    // has aura 65345
    NPC_ROCKET_VISUAL                       = 34050,                    // mounted on vehicle 33651
    NPC_ROCKET_STRIKE                       = 34047,                    // has aura 64064
    NPC_BURST_TARGET                        = 34211,                    // casts 64841 on VX001 which triggers 63382
    NPC_ASSALT_BOT                          = 34057,
    NPC_BOMB_BOT                            = 33836,                    // has aura 63767
    NPC_MAGNETIC_CORE                       = 34068,                    // has auras 64438 and 64436
    NPC_JUNK_BOT                            = 33855,

    // hard mode summoned
    NPC_FLAME_INITIAL                       = 34363,
    NPC_FLAME_SPREAD                        = 34121,
    NPC_FROST_BOMB                          = 34149,
    NPC_EMERGENCY_FIRE_BOT                  = 34147,

    // other
    SEAT_ID_TURRET                          = 4,
};

/*######
## boss_leviathan_mk2
######*/

struct MANGOS_DLL_DECL boss_leviathan_mk2AI : public ScriptedAI
{
    boss_leviathan_mk2AI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_uiMountTimer = 1000;
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMountTimer;

    void Reset() override
    {
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MIMIRON, IN_PROGRESS);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MIMIRON, FAIL);
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_TANK_SLAY_1 : SAY_TANK_SLAY_2, m_creature);
    }

    void JustSummoned(Creature* pSummoned) override
    {
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_pInstance)
        {
            script_error_log("Instance Ulduar: ERROR Failed to load instance data for this instace.");
            return;
        }

        // Mount Mimiron and the Turret manually
        if (m_uiMountTimer)
        {
            if (m_uiMountTimer <= uiDiff)
            {
                if (Creature* pTurret = m_pInstance->GetSingleCreatureFromStorage(NPC_LEVIATHAN_MK_TURRET))
                {
                    int32 iSeat = (int32)SEAT_ID_TURRET;
                    pTurret->CastCustomSpell(m_creature, SPELL_RIDE_VEHICLE_HARDCODED, &iSeat, NULL, NULL, true);
                }

                if (Creature* pMimiron = m_pInstance->GetSingleCreatureFromStorage(NPC_MIMIRON))
                {
                    pMimiron->CastSpell(m_creature, SPELL_RIDE_VEHICLE_MIMIRON, true);
                    pMimiron->CastSpell(pMimiron, SPELL_WELD, true);
                }

                m_uiMountTimer = 0;
            }
            else
                m_uiMountTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_leviathan_mk2(Creature* pCreature)
{
    return new boss_leviathan_mk2AI(pCreature);
}

void AddSC_boss_mimiron()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_leviathan_mk2";
    pNewScript->GetAI = GetAI_boss_leviathan_mk2;
    pNewScript->RegisterSelf();
}
