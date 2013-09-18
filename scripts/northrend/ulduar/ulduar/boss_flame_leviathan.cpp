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
SDName: boss_flame_leviathan
SD%Complete: 20%
SDComment: Basic script only.
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                               = -1603159,
    SAY_SLAY                                = -1603160,
    SAY_DEATH                               = -1603161,

    SAY_CHANGE_1                            = -1603162,
    SAY_CHANGE_2                            = -1603163,
    SAY_CHANGE_3                            = -1603164,
    SAY_PLAYER_RIDE                         = -1603165,
    SAY_OVERLOAD_1                          = -1603166,
    SAY_OVERLOAD_2                          = -1603167,
    SAY_OVERLOAD_3                          = -1603168,

    SAY_HARD_MODE                           = -1603169,

    SAY_TOWER_FROST                         = -1603170,
    SAY_TOWER_FIRE                          = -1603171,
    SAY_TOWER_ENERGY                        = -1603172,
    SAY_TOWER_NATURE                        = -1603173,
    SAY_TOWER_DOWN                          = -1603174,

    EMOTE_PURSUE                            = -1603175,

    // Leviathan spells
    SPELL_INVISIBILITY_DETECTION            = 18950,
    SPELL_PURSUED                           = 62374,
    SPELL_MISSILE_BARRAGE_SELF              = 62401,                    // cast on Leviathan
    SPELL_MISSILE_BARRAGE                   = 62400,
    SPELL_FLAME_VENTS                       = 62396,
    SPELL_BATTERING_RAM                     = 62376,
    SPELL_GATHERING_SPEED                   = 62375,

    // leviathan turret spells
    SPELL_FLAME_CANNON                      = 62395,
    SPELL_CANNON                            = 62397,

    // defense turret spells
    SPELL_OVERLOAD_CIRCUIT                  = 62399,
    SPELL_SEARING_FLAME                     = 62402,
    SPELL_SYSTEMS_SHUTDOWN                  = 62475,                    // sends event 21605 for achiev check

    // leviathan seat spell (on eject)
    SPELL_SMOKE_TRAIL                       = 63575,

    // tower buffs to Leviathan (applied on combat start if the towers are alive)
    SPELL_TOWER_OF_FROST                    = 65077,
    SPELL_TOWER_OF_LIFE                     = 64482,
    SPELL_TOWER_OF_STORMS                   = 65076,
    SPELL_TOWER_OF_FLAMES                   = 65075,

    // tower beacon abilities
    SPELL_HODIR_FURY                        = 62533,                    // tower of frost
    SPELL_FREYA_WARD                        = 62906,                    // tower of life
    SPELL_THORIMS_HAMMER                    = 62911,                    // tower of storms
    SPELL_MIMIRON_INFERNO                   = 62910,                    // tower of flames

    // beacon vehicles visuals
    SPELL_LIGHTNING_SKYBEAM                 = 62897,                    // storm beacon
    SPELL_RED_SKYBEAM                       = 63772,                    // flames beacon
    SPELL_BLUE_SKYBEAM                      = 63769,                    // frost beacon
    SPELL_GREEN_SKYBEAM                     = 62895,                    // life beacon

    // other beacon vehicles spells
    SPELL_BEAM_TARGET_STATE                 = 62898,
    SPELL_TARGET_SEARCH                     = 63761,                    // cast by npc 33369
    SPELL_UNSTABLE_ENERGY                   = 62865,                    // cast by npc 33366
    SPELL_BIRTH                             = 40031,

    // hard mode beacons - they cast the actual damage spells
    NPC_THORIM_HAMMER                       = 33365,
    NPC_MIMIRON_INFERNO                     = 33370,
    NPC_HODIR_FURY                          = 33212,
    NPC_FREYA_WARD                          = 33367,

    // beacon vehicles
    NPC_THORIM_HAMMER_VEHICLE               = 33364,                    // has accessory 33365
    NPC_MIMIRON_INFERNO_VEHICLE             = 33369,                    // has accessory 33370
    NPC_HODIR_FURY_VEHICLE                  = 33108,                    // has accessory 33212
    NPC_FREYA_WARD_VEHICLE                  = 33366,                    // has accessory 33367

    // freya's ward summons
    NPC_WRITHING_LASHER                     = 33387,                    // both spam spell 65062 on target
    NPC_WARD_OF_LIFE                        = 34275,

    // other npcs (spawned at epilogue)
    SPELL_RIDE_VEHICLE                      = 43671,
    NPC_BRANN_FLYING_MACHINE                = 34120,
    NPC_BRANN_BRONZEBEARD                   = 34119,
    NPC_ARCHMANGE_RHYDIAN                   = 33696,
};

/*######
## boss_flame_leviathan
######*/

struct MANGOS_DLL_DECL boss_flame_leviathanAI : public ScriptedAI
{
    boss_flame_leviathanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    void Reset() override
    {
        DoCastSpellIfCan(m_creature, SPELL_INVISIBILITY_DETECTION, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
    }

    void JustDied(Unit* pKiller) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN, DONE);

        DoScriptText(SAY_DEATH, m_creature);

        // start epilogue event
        if (Creature* pFlyMachine = m_creature->SummonCreature(NPC_BRANN_FLYING_MACHINE, 175.2838f, -210.4325f, 501.2375f, 1.42f, TEMPSUMMON_CORPSE_DESPAWN, 0))
        {
            if (Creature* pBrann = m_creature->SummonCreature(NPC_BRANN_BRONZEBEARD, 175.2554f, -210.6305f, 500.7375f, 1.42f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                pBrann->CastSpell(pFlyMachine, SPELL_RIDE_VEHICLE, true);

            pFlyMachine->SetWalk(false);
            pFlyMachine->GetMotionMaster()->MovePoint(1, 229.9419f, -130.3764f, 409.5681f);
        }
    }

    void Aggro(Unit* pWho) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim) override
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN, FAIL);
    }

    void JustSummoned(Creature* pSummoned) override
    {
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMoveType, uint32 uiPointId) override
    {
        if (uiMoveType != POINT_MOTION_TYPE || !uiPointId)
            return;

        if (pSummoned->GetEntry() == NPC_BRANN_FLYING_MACHINE)
        {
            // spawn the Archmange and eject Brann
            if (Creature* pArchmage = m_creature->SummonCreature(NPC_ARCHMANGE_RHYDIAN, 235.5596f, -136.1876f, 409.6508f, 1.78f, TEMPSUMMON_CORPSE_DESPAWN, 0))
            {
                pArchmage->SetWalk(false);
                pArchmage->GetMotionMaster()->MovePoint(1, 239.3158f, -123.6443f, 409.8174f);
            }

            pSummoned->RemoveAllAuras();
        }
        else if (pSummoned->GetEntry() == NPC_ARCHMANGE_RHYDIAN)
        {
            if (Creature* pBrann = GetClosestCreatureWithEntry(pSummoned, NPC_BRANN_BRONZEBEARD, 30.0f))
            {
                // rest will be handled by DB scripts
                pBrann->SetWalk(false);
                pBrann->GetMotionMaster()->MoveWaypoint();
            }
        }
    }

    void MovementInform(uint32 uiMoveType, uint32 uiPointId) override
    {
        if (uiMoveType != POINT_MOTION_TYPE || !uiPointId)
            return;

        // set boss in combat (if not already)
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_flame_leviathan(Creature* pCreature)
{
    return new boss_flame_leviathanAI(pCreature);
}

void AddSC_boss_flame_leviathan()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_flame_leviathan";
    pNewScript->GetAI = GetAI_boss_flame_leviathan;
    pNewScript->RegisterSelf();
}
