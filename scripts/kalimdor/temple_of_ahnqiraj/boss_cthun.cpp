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
SDName: Boss_Cthun
SD%Complete: 95
SDComment: Darkglare tracking issue
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "temple_of_ahnqiraj.h"

enum
{
    EMOTE_WEAKENED                  = -1531011,

    // ****** Out of Combat ******
    // Random Wispers - No text only sound, maybe handle them in instance script!
    RANDOM_SOUND_WHISPER            = 8663,

    // ***** Phase 1 ********
    NPC_EYE_OF_CTHUN                = 15589,
    MOB_CLAW_TENTACLE               = 15725,
    MOB_EYE_TENTACLE                = 15726,
    MOB_SMALL_PORTAL                = 15904,

    SPELL_GREEN_BEAM                = 26134,
    SPELL_DARK_GLARE                = 26029,
    SPELL_RED_COLORATION            = 22518,                // Probably not the right spell but looks similar

    // Eye Tentacles Spells
    SPELL_MIND_FLAY                 = 26143,

    // Claw Tentacles Spells
    SPELL_GROUND_RUPTURE            = 26139,
    SPELL_HAMSTRING                 = 26141,

    // *****Phase 2******
    // Body spells
    // SPELL_CARAPACE_CTHUN         = 26156,                // Was removed from client dbcs
    SPELL_TRANSFORM                 = 26232,

    // Giant Claw Tentacles
    SPELL_MASSIVE_GROUND_RUPTURE    = 26100,

    // Also casts Hamstring
    SPELL_THRASH                    = 3391,

    // Giant Eye Tentacles
    // CHAIN CASTS "SPELL_GREEN_BEAM"

    // Stomach Spells
    SPELL_MOUTH_TENTACLE            = 26332,
    SPELL_EXIT_STOMACH_KNOCKBACK    = 25383,
    SPELL_DIGESTIVE_ACID            = 26476,

    // Mobs
    MOB_BODY_OF_CTHUN               = 15809,
    MOB_GIANT_CLAW_TENTACLE         = 15728,
    MOB_GIANT_EYE_TENTACLE          = 15334,
    MOB_FLESH_TENTACLE              = 15802,
    MOB_GIANT_PORTAL                = 15910,
};

//Stomach Teleport positions
#define STOMACH_X                           -8562.0f
#define STOMACH_Y                           2037.0f
#define STOMACH_Z                           -70.0f
#define STOMACH_O                           5.05f

//Flesh tentacle positions
#define TENTACLE_POS1_X                     -8571.0f
#define TENTACLE_POS1_Y                     1990.0f
#define TENTACLE_POS1_Z                     -98.0f
#define TENTACLE_POS1_O                     1.22f

#define TENTACLE_POS2_X                     -8525.0f
#define TENTACLE_POS2_Y                     1994.0f
#define TENTACLE_POS2_Z                     -98.0f
#define TENTACLE_POS2_O                     2.12f

//Kick out position -- unneeded - replace this with areatrigger handling of trigger [Trigger 4033]!
#define KICK_X                              -8545.0f
#define KICK_Y                              1984.0f
#define KICK_Z                              -96.0f

struct MANGOS_DLL_DECL flesh_tentacleAI : public ScriptedAI
{
    flesh_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();
    }

    ObjectGuid m_cThunGuid;
    uint32 m_uiCheckTimer;

    void SpawnedByCthun(Creature* pCThun)
    {
        m_cThunGuid = pCThun->GetObjectGuid();
    }

    void Reset()
    {
        m_uiCheckTimer = 1000;
    }

    void UpdateAI(const uint32 diff);

    void JustDied(Unit* killer);
};

struct MANGOS_DLL_DECL eye_of_cthunAI : public ScriptedAI
{
    eye_of_cthunAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);

        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        if (!m_pInstance)
            error_log("SD2: No Instance eye_of_cthunAI");

        Reset();
    }

    ScriptedInstance* m_pInstance;

    // Global variables
    uint32 m_uiPhaseTimer;

    // Eye beam phase
    uint32 m_uiBeamTimer;
    uint32 m_uiEyeTentacleTimer;
    uint32 m_uiClawTentacleTimer;

    // Dark Glare phase
    uint32 m_uiDarkGlareTick;
    uint32 m_uiDarkGlareTickTimer;
    float m_fDarkGlareAngle;
    bool m_bClockWise;

    void Reset()
    {
        // Phase information
        m_uiPhaseTimer = 50000;                             // First dark glare in 50 seconds

        // Eye beam phase 50 seconds
        m_uiBeamTimer = 3000;
        m_uiEyeTentacleTimer = 45000;                       // Always spawns 5 seconds before Dark Beam
        m_uiClawTentacleTimer = 12500;                      // 4 per Eye beam phase (unsure if they spawn durring Dark beam)

        // Dark Beam phase 35 seconds (each tick = 1 second, 35 ticks)
        m_uiDarkGlareTick = 0;
        m_uiDarkGlareTickTimer = 1000;
        m_fDarkGlareAngle = 0;
        m_bClockWise = false;

        // Reset flags
        m_creature->RemoveAurasDueToSpell(SPELL_RED_COLORATION);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == MOB_EYE_TENTACLE)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void SpawnEyeTentacle(float x, float y)
    {
        m_creature->SummonCreature(MOB_EYE_TENTACLE, m_creature->GetPositionX()+x ,m_creature->GetPositionY()+y, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 500);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_EYE_NORMAL);
    }

    void EnterEvadeMode()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_NOT_STARTED);

        ScriptedAI::EnterEvadeMode();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Check if we have a target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // No instance
        if (!m_pInstance)
            return;

        switch (m_pInstance->GetData(TYPE_CTHUN_PHASE))
        {
            case PHASE_EYE_NORMAL:
            {
                // m_uiBeamTimer
                if (m_uiBeamTimer < uiDiff)
                {
                    // SPELL_GREEN_BEAM
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        m_creature->InterruptNonMeleeSpells(false);
                        DoCastSpellIfCan(pTarget, SPELL_GREEN_BEAM);

                        // Correctly update our target
                        m_creature->SetTargetGuid(pTarget->GetObjectGuid());
                    }

                    //Beam every 3 seconds
                    m_uiBeamTimer = 3000;
                }
                else
                    m_uiBeamTimer -= uiDiff;

                // m_uiClawTentacleTimer
                if (m_uiClawTentacleTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        // Spawn claw tentacle on the random target
                        if (Creature* pSummoned = m_creature->SummonCreature(MOB_CLAW_TENTACLE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 500))
                            pSummoned->AI()->AttackStart(pTarget);
                    }

                    // One claw tentacle every 12.5 seconds
                    m_uiClawTentacleTimer = 12500;
                }
                else
                    m_uiClawTentacleTimer -= uiDiff;

                // m_uiEyeTentacleTimer
                if (m_uiEyeTentacleTimer < uiDiff)
                {
                    //Spawn the 8 Eye Tentacles in the corret spots
                    SpawnEyeTentacle(0, 20);                //south
                    SpawnEyeTentacle(10, 10);               //south west
                    SpawnEyeTentacle(20, 0);                //west
                    SpawnEyeTentacle(10, -10);              //north west

                    SpawnEyeTentacle(0, -20);               //north
                    SpawnEyeTentacle(-10, -10);             //north east
                    SpawnEyeTentacle(-20, 0);               // east
                    SpawnEyeTentacle(-10, 10);              // south east

                    // No point actually putting a timer here since
                    // These shouldn't trigger agian until after phase shifts
                    m_uiEyeTentacleTimer = 45000;
                }
                else
                    m_uiEyeTentacleTimer -= uiDiff;

                // m_uiPhaseTimer
                if (m_uiPhaseTimer < uiDiff)
                {
                    //Switch to Dark Beam
                    m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_EYE_DARK_GLARE);

                    m_creature->InterruptNonMeleeSpells(false);

                    // Select random target for dark beam to start on
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        // Correctly update our target
                        m_creature->SetTargetGuid(pTarget->GetObjectGuid());

                        // Face our target
                        m_fDarkGlareAngle = m_creature->GetAngle(pTarget);
                        m_uiDarkGlareTickTimer = 1000;
                        m_uiDarkGlareTick = 0;
                        m_bClockWise = urand(0, 1);
                    }

                    // Add red coloration to C'thun
                    DoCastSpellIfCan(m_creature, SPELL_RED_COLORATION);

                    // Freeze animation

                    // Darkbeam for 35 seconds
                    m_uiPhaseTimer = 35000;
                }
                else
                    m_uiPhaseTimer -= uiDiff;

                break;
            }
            case PHASE_EYE_DARK_GLARE:
            {
                // Dark Glare
                if (m_uiDarkGlareTick < 35)
                {
                    if (m_uiDarkGlareTickTimer < uiDiff)
                    {
                        // Remove any target
                        m_creature->SetTargetGuid(ObjectGuid());

                        // Set angle and cast
                        m_creature->SetOrientation(m_fDarkGlareAngle + (m_bClockWise ? 1 : -1) * ((float)m_uiDarkGlareTick*M_PI_F/35));

                        m_creature->StopMoving();

                        // Actual dark glare cast, maybe something missing here?
                        DoCastSpellIfCan(m_creature, SPELL_DARK_GLARE);

                        // Increase tick
                        ++m_uiDarkGlareTick;

                        // 1 second per tick
                        m_uiDarkGlareTickTimer = 1000;
                    }
                    else
                        m_uiDarkGlareTickTimer -= uiDiff;
                }

                // m_uiPhaseTimer
                if (m_uiPhaseTimer < uiDiff)
                {
                    // Switch to Eye Beam
                    m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_EYE_NORMAL);

                    m_uiBeamTimer = 3000;
                    m_uiEyeTentacleTimer = 45000;               //Always spawns 5 seconds before Dark Beam
                    m_uiClawTentacleTimer = 12500;              //4 per Eye beam phase (unsure if they spawn durring Dark beam)

                    m_creature->InterruptNonMeleeSpells(false);

                    // Remove Red coloration from C'thun
                    m_creature->RemoveAurasDueToSpell(SPELL_RED_COLORATION);

                    // Freeze animation
                    m_creature->SetUInt32Value(UNIT_FIELD_FLAGS, 0);

                    // Eye Beam for 50 seconds
                    m_uiPhaseTimer = 50000;
                }
                else
                    m_uiPhaseTimer -= uiDiff;

                break;
            }
            // Transition phase
            case PHASE_TRANSITION:
            {
                // Remove any target
                m_creature->SetTargetGuid(ObjectGuid());
                m_creature->SetHealth(0);

                // Do not kill yet, to be able to reset on evade
                break;
            }

            // Dead phase
            case PHASE_FINISH:
            {
                m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                return;
            }
        }
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        // No instance
        if (!m_pInstance)
            return;

        switch (m_pInstance->GetData(TYPE_CTHUN_PHASE))
        {
            case PHASE_EYE_NORMAL:
            case PHASE_EYE_DARK_GLARE:
            {
                // Only if it will kill
                if (uiDamage < m_creature->GetHealth())
                    return;

                // Fake death in phase 0 or 1 (green beam or dark glare phase)
                m_creature->InterruptNonMeleeSpells(false);

                // Remove Red coloration from c'thun
                m_creature->RemoveAurasDueToSpell(SPELL_RED_COLORATION);

                // Reset to normal emote state and prevent select and attack
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                // Remove Target field
                m_creature->SetTargetGuid(ObjectGuid());

                // Death animation/ respawning;
                m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_TRANSITION);
                // Ensure CThun is in combat
                if (Creature* pCThun = m_pInstance->GetSingleCreatureFromStorage(NPC_CTHUN))
                    pCThun->AI()->AttackStart(pDealer);

                m_creature->SetHealth(0);
                uiDamage = 0;

                m_creature->InterruptNonMeleeSpells(true);
                m_creature->RemoveAllAuras();

                break;
            }
            case PHASE_FINISH:
            {
                // Allow death here
                return;
            }

            default:
            {
                // Prevent death in this phase
                uiDamage = 0;
                return;
            }
            break;
        }
    }
};

struct MANGOS_DLL_DECL cthunAI : public ScriptedAI
{
    cthunAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);

        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        if (!m_pInstance)
            error_log("SD2: No Instance eye_of_cthunAI");

        Reset();
    }

    ScriptedInstance* m_pInstance;

    // Out of combat whisper timer
    uint32 m_uiWisperTimer;

    // Global variables
    uint32 m_uiPhaseTimer;

    //-------------------

    // Body Phase
    uint32 m_uiEyeTentacleTimer;
    uint8 m_uiFleshTentaclesKilled;
    uint32 m_uiGiantClawTentacleTimer;
    uint32 m_uiGiantEyeTentacleTimer;
    uint32 m_uiStomachAcidTimer;
    uint32 m_uiStomachEnterTimer;
    uint32 m_uiStomachEnterVisTimer;

    ObjectGuid m_stomachEnterTargetGuid;

    // Stomach map, bool = true then in stomach
    typedef UNORDERED_MAP<ObjectGuid, bool> StomachMap;
    StomachMap m_mStomachMap;

    void Reset()
    {
        // One random wisper every 90 - 300 seconds
        m_uiWisperTimer = 90000;

        // Phase information
        m_uiPhaseTimer = 10000;                             // Emerge in 10 seconds

        // Body Phase
        m_uiEyeTentacleTimer = 30000;
        m_uiFleshTentaclesKilled = 0;
        m_uiGiantClawTentacleTimer = 15000;                 // 15 seconds into body phase (1 min repeat)
        m_uiGiantEyeTentacleTimer = 45000;                  // 15 seconds into body phase (1 min repeat)
        m_uiStomachAcidTimer = 4000;                        // Every 4 seconds
        m_uiStomachEnterTimer = 10000;                      // Every 10 seconds
        m_uiStomachEnterVisTimer = 0;                       // Always 3.5 seconds after Stomach Enter Timer
        m_stomachEnterTargetGuid.Clear();                   // Target to be teleported to stomach

        // Clear players in stomach and outside
        m_mStomachMap.clear();

        // Reset flags
        m_creature->RemoveAurasDueToSpell(SPELL_TRANSFORM);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == MOB_EYE_TENTACLE)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void SpawnEyeTentacle(float x, float y)
    {
        m_creature->SummonCreature(MOB_EYE_TENTACLE, m_creature->GetPositionX()+x ,m_creature->GetPositionY()+y, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 500);
    }

    Player* SelectRandomNotStomach()
    {
        if (m_mStomachMap.empty())
            return NULL;

        std::vector<Player*> vTempTargets;
        vTempTargets.reserve(m_mStomachMap.size());

        // Get all players in map
        for (StomachMap::const_iterator itr = m_mStomachMap.begin(); itr != m_mStomachMap.end(); ++itr)
        {
            // Check for valid player
            Player* pPlayer = m_creature->GetMap()->GetPlayer(itr->first);

            // Only units out of stomach
            if (pPlayer && itr->second == false)
                vTempTargets.push_back(pPlayer);
        }

        if (vTempTargets.empty())
            return NULL;

        // Get random but only if we have more than one unit on threat list
        return vTempTargets[urand(0, vTempTargets.size()- 1)];
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Check if we have a target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            // No target so we'll use this section to do our random wispers instance wide
            // WisperTimer
            if (m_uiWisperTimer < uiDiff)
            {
                if (!m_creature->GetMap()->IsDungeon())
                    return;

                // Play random sound to the map
                m_creature->GetMap()->PlayDirectSoundToMap(RANDOM_SOUND_WHISPER);

                // One random wisper every 90 - 300 seconds
                m_uiWisperTimer = urand(90000, 300000);
            }
            else
                m_uiWisperTimer -= uiDiff;

            return;
        }

        m_creature->SetTargetGuid(ObjectGuid());

        // No instance
        if (!m_pInstance)
            return;

        switch (m_pInstance->GetData(TYPE_CTHUN_PHASE))
        {
            // Transition phase
            case PHASE_TRANSITION:
            {
                // PhaseTimer
                if (m_uiPhaseTimer < uiDiff)
                {
                    // Switch
                    m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_CTHUN);

                    // Switch to C'thun model
                    m_creature->InterruptNonMeleeSpells(false);
                    DoCastSpellIfCan(m_creature, SPELL_TRANSFORM);
                    m_creature->SetHealth(m_creature->GetMaxHealth());

                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                    // Emerging phase
                    m_creature->SetInCombatWithZone();

                    // Place all units in threat list on outside of stomach
                    m_mStomachMap.clear();

                    ThreatList const& tList = m_creature->getThreatManager().getThreatList();
                    for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
                    {
                        // Outside stomach, only players
                        if ((*i)->getUnitGuid().IsPlayer())
                            m_mStomachMap[(*i)->getUnitGuid()] = false;
                    }

                    // Spawn 2 flesh tentacles
                    m_uiFleshTentaclesKilled = 0;

                    // Spawn flesh tentacle
                    Creature* pSpawned = m_creature->SummonCreature(MOB_FLESH_TENTACLE, TENTACLE_POS1_X, TENTACLE_POS1_Y, TENTACLE_POS1_Z, TENTACLE_POS1_O, TEMPSUMMON_CORPSE_DESPAWN, 0);

                    if (!pSpawned)
                        ++m_uiFleshTentaclesKilled;
                    else
                    {
                        if (flesh_tentacleAI* pTentacleAI = dynamic_cast<flesh_tentacleAI*>(pSpawned->AI()))
                            pTentacleAI->SpawnedByCthun(m_creature);
                    }

                    // Spawn flesh tentacle
                    pSpawned = m_creature->SummonCreature(MOB_FLESH_TENTACLE, TENTACLE_POS2_X, TENTACLE_POS2_Y, TENTACLE_POS2_Z, TENTACLE_POS2_O, TEMPSUMMON_CORPSE_DESPAWN, 0);

                    if (!pSpawned)
                        ++m_uiFleshTentaclesKilled;
                    else
                    {
                        if (flesh_tentacleAI* pTentacleAI = dynamic_cast<flesh_tentacleAI*>(pSpawned->AI()))
                            pTentacleAI->SpawnedByCthun(m_creature);
                    }

                    m_uiPhaseTimer = 0;
                }
                else
                    m_uiPhaseTimer -= uiDiff;

                break;

            }
            // Body Phase
            case PHASE_CTHUN:
            {
                // Weaken
                if (m_uiFleshTentaclesKilled > 1)
                {
                    m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_CTHUN_WEAKENED);

                    DoScriptText(EMOTE_WEAKENED, m_creature);
                    m_uiPhaseTimer = 45000;

                    DoCastSpellIfCan(m_creature, SPELL_RED_COLORATION, CAST_TRIGGERED);

                    // Kick all players out of stomach
                    for (StomachMap::iterator itr = m_mStomachMap.begin(); itr != m_mStomachMap.end(); ++itr)
                    {
                        // Check for valid player
                        Player* pPlayer = m_creature->GetMap()->GetPlayer(itr->first);

                        if (pPlayer && itr->second == true)
                        {
                            // Teleport each player out
                            DoTeleportPlayer(pPlayer, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()+10, rand()%6);

                            // Cast knockback on them
                            DoCastSpellIfCan(pPlayer, SPELL_EXIT_STOMACH_KNOCKBACK, CAST_TRIGGERED);

                            // Remove the acid debuff
                            pPlayer->RemoveAurasDueToSpell(SPELL_DIGESTIVE_ACID);

                            itr->second = false;
                        }
                    }

                    return;
                }

                // Stomach acid
                if (m_uiStomachAcidTimer < uiDiff)
                {
                    // Apply aura to all players in stomach
                    for (StomachMap::iterator itr = m_mStomachMap.begin(); itr != m_mStomachMap.end(); ++itr)
                    {
                        // Check for valid player
                        Player* pPlayer = m_creature->GetMap()->GetPlayer(itr->first);

                        // Only apply to units in stomach
                        if (pPlayer && itr->second == true)
                        {
                            // Cast digestive acid on them
                            DoCastSpellIfCan(pPlayer, SPELL_DIGESTIVE_ACID, CAST_TRIGGERED);

                            // Check if player should be kicked from stomach
                            if (pPlayer->IsWithinDist3d(KICK_X, KICK_Y, KICK_Z, 10.0f))
                            {
                                // Teleport each player out
                                DoTeleportPlayer(pPlayer, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()+10, rand()%6);

                                // Cast knockback on them
                                DoCastSpellIfCan(pPlayer, SPELL_EXIT_STOMACH_KNOCKBACK, CAST_TRIGGERED);

                                // Remove the acid debuff
                                pPlayer->RemoveAurasDueToSpell(SPELL_DIGESTIVE_ACID);

                                itr->second = false;
                            }
                        }
                    }

                    m_uiStomachAcidTimer = 4000;
                }
                else
                    m_uiStomachAcidTimer -= uiDiff;

                // Stomach Enter Timer
                if (m_uiStomachEnterTimer < uiDiff)
                {
                    if (Player* pTarget = SelectRandomNotStomach())
                    {
                        // Set target in stomach
                        m_mStomachMap[pTarget->GetObjectGuid()] = true;
                        pTarget->InterruptNonMeleeSpells(false);
                        pTarget->CastSpell(pTarget, SPELL_MOUTH_TENTACLE, true, NULL, NULL, m_creature->GetObjectGuid());
                        m_stomachEnterTargetGuid = pTarget->GetObjectGuid();
                        m_uiStomachEnterVisTimer = 3800;
                    }

                    m_uiStomachEnterTimer = 13800;
                }
                else
                    m_uiStomachEnterTimer -= uiDiff;

                if (m_uiStomachEnterVisTimer && m_stomachEnterTargetGuid)
                {
                    if (m_uiStomachEnterVisTimer <= uiDiff)
                    {
                        // Check for valid player
                        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_stomachEnterTargetGuid))
                            DoTeleportPlayer(pPlayer, STOMACH_X, STOMACH_Y, STOMACH_Z, STOMACH_O);

                        m_stomachEnterTargetGuid.Clear();
                        m_uiStomachEnterVisTimer = 0;

                        // Note that actually C'Thun cannot be soloed, so kill all players, if no player left outside of stomach
                        bool bKillAllPlayer = true;
                        for (StomachMap::iterator itr = m_mStomachMap.begin(); itr != m_mStomachMap.end(); ++itr)
                        {
                            Player* pPlayer = m_creature->GetMap()->GetPlayer(itr->first);
                            if (itr->second == false && pPlayer)
                            {
                                bKillAllPlayer = false;
                                break;
                            }
                        }
                        if (bKillAllPlayer)
                        {
                            for (StomachMap::iterator itr = m_mStomachMap.begin(); itr != m_mStomachMap.end(); ++itr)
                            {
                                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(itr->first))
                                    m_creature->DealDamage(pPlayer, pPlayer->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                            }
                            EnterEvadeMode();
                            return;
                        }
                    }
                    else
                        m_uiStomachEnterVisTimer -= uiDiff;
                }

                // GientClawTentacleTimer
                if (m_uiGiantClawTentacleTimer < uiDiff)
                {
                    if (Player* pTarget = SelectRandomNotStomach())
                    {
                        Creature* Spawned = NULL;

                        // Spawn claw tentacle on the random target
                        if (Creature* pSummoned = m_creature->SummonCreature(MOB_GIANT_CLAW_TENTACLE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 500))
                            pSummoned->AI()->AttackStart(pTarget);
                    }

                    // One giant claw tentacle every minute
                    m_uiGiantClawTentacleTimer = 60000;
                }
                else
                    m_uiGiantClawTentacleTimer -= uiDiff;

                // GiantEyeTentacleTimer
                if (m_uiGiantEyeTentacleTimer < uiDiff)
                {
                    if (Player* pTarget = SelectRandomNotStomach())
                    {
                        // Spawn claw tentacle on the random target
                        if (Creature* pSummoned = m_creature->SummonCreature(MOB_GIANT_EYE_TENTACLE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 500))
                            pSummoned->AI()->AttackStart(pTarget);
                    }

                    // One giant eye tentacle every minute
                    m_uiGiantEyeTentacleTimer = 60000;
                }
                else
                    m_uiGiantEyeTentacleTimer -= uiDiff;

                // EyeTentacleTimer
                if (m_uiEyeTentacleTimer < uiDiff)
                {
                    // Spawn the 8 Eye Tentacles in the corret spots
                    SpawnEyeTentacle(0, 25);                //south
                    SpawnEyeTentacle(12, 12);               //south west
                    SpawnEyeTentacle(25, 0);                //west
                    SpawnEyeTentacle(12, -12);              //north west

                    SpawnEyeTentacle(0, -25);               //north
                    SpawnEyeTentacle(-12, -12);             //north east
                    SpawnEyeTentacle(-25, 0);               // east
                    SpawnEyeTentacle(-12, 12);              // south east

                    //These spawn at every 30 seconds
                    m_uiEyeTentacleTimer = 30000;
                }
                else
                    m_uiEyeTentacleTimer -= uiDiff;

                break;

            }
            // Weakened state
            case PHASE_CTHUN_WEAKENED:
            {
                // PhaseTimer
                if (m_uiPhaseTimer < uiDiff)
                {
                    // Switch
                    m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_CTHUN);

                    // Remove red coloration
                    m_creature->RemoveAurasDueToSpell(SPELL_RED_COLORATION);

                    // Spawn 2 flesh tentacles
                    m_uiFleshTentaclesKilled = 0;

                    // Spawn flesh tentacle
                    Creature* pSpawned = m_creature->SummonCreature(MOB_FLESH_TENTACLE, TENTACLE_POS1_X, TENTACLE_POS1_Y, TENTACLE_POS1_Z, TENTACLE_POS1_O, TEMPSUMMON_CORPSE_DESPAWN, 0);

                    if (!pSpawned)
                        ++m_uiFleshTentaclesKilled;
                    else
                    {
                        if (flesh_tentacleAI* pTentacleAI = dynamic_cast<flesh_tentacleAI*>(pSpawned->AI()))
                            pTentacleAI->SpawnedByCthun(m_creature);
                    }

                    pSpawned = NULL;
                    // Spawn flesh tentacle
                    pSpawned = m_creature->SummonCreature(MOB_FLESH_TENTACLE, TENTACLE_POS2_X, TENTACLE_POS2_Y, TENTACLE_POS2_Z, TENTACLE_POS2_O, TEMPSUMMON_CORPSE_DESPAWN, 0);

                    if (!pSpawned)
                        ++m_uiFleshTentaclesKilled;
                    else
                    {
                        if (flesh_tentacleAI* pTentacleAI = dynamic_cast<flesh_tentacleAI*>(pSpawned->AI()))
                            pTentacleAI->SpawnedByCthun(m_creature);
                    }

                    m_uiPhaseTimer = 0;
                }
                else
                    m_uiPhaseTimer -= uiDiff;
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        // Switch
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_FINISH);
    }

    void EnterEvadeMode()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CTHUN_PHASE, PHASE_NOT_STARTED);

        ScriptedAI::EnterEvadeMode();
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        // No instance
        if (!m_pInstance)
            return;

        switch (m_pInstance->GetData(TYPE_CTHUN_PHASE))
        {
            case PHASE_CTHUN:
            {
                // Not weakened so reduce damage by 99%
                if (uiDamage / 99 > 0) uiDamage/= 99;
                else
                    uiDamage = 1;

                // Prevent death in non-weakened state
                if (uiDamage >= m_creature->GetHealth())
                    uiDamage = 0;

                return;
            }
            break;

            case PHASE_CTHUN_WEAKENED:
            {
                //Weakened - takes normal damage
                return;
            }

            default:
                uiDamage = 0;
                break;
        }
    }

    void FleshTentcleKilled()
    {
        ++m_uiFleshTentaclesKilled;
    }
};

struct MANGOS_DLL_DECL eye_tentacleAI : public ScriptedAI
{
    eye_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();

        if (Unit* pPortal = m_creature->SummonCreature(MOB_SMALL_PORTAL, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
            m_portalGuid = pPortal->GetObjectGuid();
    }

    uint32 m_uiMindflayTimer;
    uint32 m_uiKillSelfTimer;
    ObjectGuid m_portalGuid;

    void JustDied(Unit*)
    {
        if (Creature* pCreature = m_creature->GetMap()->GetCreature(m_portalGuid))
            pCreature->DealDamage(pCreature, pCreature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
    }

    void Reset()
    {
        // Mind flay half a second after we spawn
        m_uiMindflayTimer = 500;

        // This prevents eyes from overlapping
        m_uiKillSelfTimer = 35000;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Check if we have a target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // KillSelfTimer
        if (m_uiKillSelfTimer < uiDiff)
        {
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);

            return;
        }
        else
            m_uiKillSelfTimer -= uiDiff;

        // MindflayTimer
        if (m_uiMindflayTimer < uiDiff)
        {
            Unit* target = NULL;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (target && !target->HasAura(SPELL_DIGESTIVE_ACID, EFFECT_INDEX_0))
                DoCastSpellIfCan(target, SPELL_MIND_FLAY);

            //Mindflay every 10 seconds
            m_uiMindflayTimer = 10100;
        }
        else
            m_uiMindflayTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL claw_tentacleAI : public ScriptedAI
{
    claw_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();

        if (Unit* pPortal = m_creature->SummonCreature(MOB_SMALL_PORTAL, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
            m_portalGuid = pPortal->GetObjectGuid();
    }

    uint32 m_uiGroundRuptureTimer;
    uint32 m_uiHamstringTimer;
    uint32 m_uiEvadeTimer;
    ObjectGuid m_portalGuid;

    void JustDied(Unit*)
    {
        if (Creature* pCreature = m_creature->GetMap()->GetCreature(m_portalGuid))
            pCreature->DealDamage(pCreature, pCreature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
    }

    void Reset()
    {
        // First rupture should happen half a second after we spawn
        m_uiGroundRuptureTimer = 500;
        m_uiHamstringTimer = 2000;
        m_uiEvadeTimer = 5000;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Check if we have a target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // EvadeTimer
        if (!m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            if (m_uiEvadeTimer < uiDiff)
            {
                if (Creature* pCreature = m_creature->GetMap()->GetCreature(m_portalGuid))
                    pCreature->DealDamage(pCreature, pCreature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);

                // Dissapear and reappear at new position
                m_creature->SetVisibility(VISIBILITY_OFF);

                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                if (!pTarget)
                {
                    m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                    return;
                }

                if (!pTarget->HasAura(SPELL_DIGESTIVE_ACID, EFFECT_INDEX_0))
                {
                    m_creature->GetMap()->CreatureRelocation(m_creature, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0);

                    if (Unit* pPortal = m_creature->SummonCreature(MOB_SMALL_PORTAL, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                        m_portalGuid = pPortal->GetObjectGuid();

                    m_uiGroundRuptureTimer = 500;
                    m_uiHamstringTimer = 2000;
                    m_uiEvadeTimer = 5000;
                    AttackStart(pTarget);
                }

                m_creature->SetVisibility(VISIBILITY_ON);
            }
            else
                m_uiEvadeTimer -= uiDiff;
        }

        // GroundRuptureTimer
        if (m_uiGroundRuptureTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_GROUND_RUPTURE);
            m_uiGroundRuptureTimer = 30000;
        }
        else
            m_uiGroundRuptureTimer -= uiDiff;

        // HamstringTimer
        if (m_uiHamstringTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_HAMSTRING);
            m_uiHamstringTimer = 5000;
        }
        else
            m_uiHamstringTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL giant_claw_tentacleAI : public ScriptedAI
{
    giant_claw_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();

        if (Unit* pPortal = m_creature->SummonCreature(MOB_GIANT_PORTAL, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
            m_portalGuid = pPortal->GetObjectGuid();
    }

    uint32 m_uiGroundRuptureTimer;
    uint32 m_uiThrashTimer;
    uint32 m_uiHamstringTimer;
    uint32 m_uiEvadeTimer;
    ObjectGuid m_portalGuid;

    void JustDied(Unit*)
    {
        if (Creature* pCreature = m_creature->GetMap()->GetCreature(m_portalGuid))
            pCreature->DealDamage(pCreature, pCreature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
    }

    void Reset()
    {
        //First rupture should happen half a second after we spawn
        m_uiGroundRuptureTimer = 500;
        m_uiHamstringTimer = 2000;
        m_uiThrashTimer = 5000;
        m_uiEvadeTimer = 5000;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Check if we have a target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // EvadeTimer
        if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            if (m_uiEvadeTimer < uiDiff)
            {
                if (Creature* pCreature = m_creature->GetMap()->GetCreature(m_portalGuid))
                    pCreature->DealDamage(pCreature, pCreature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);

                // Dissapear and reappear at new position
                m_creature->SetVisibility(VISIBILITY_OFF);

                Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                if (!target)
                {
                    m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                    return;
                }

                if (!target->HasAura(SPELL_DIGESTIVE_ACID, EFFECT_INDEX_0))
                {
                    m_creature->GetMap()->CreatureRelocation(m_creature, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0);

                    if (Unit* pPortal = m_creature->SummonCreature(MOB_GIANT_PORTAL, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                        m_portalGuid = pPortal->GetObjectGuid();

                    m_uiGroundRuptureTimer = 500;
                    m_uiHamstringTimer = 2000;
                    m_uiThrashTimer = 5000;
                    m_uiEvadeTimer = 5000;
                    AttackStart(target);
                }

                m_creature->SetVisibility(VISIBILITY_ON);

            }
            else
                m_uiEvadeTimer -= uiDiff;
        }

        // GroundRuptureTimer
        if (m_uiGroundRuptureTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_GROUND_RUPTURE);
            m_uiGroundRuptureTimer = 30000;
        }
        else
            m_uiGroundRuptureTimer -= uiDiff;

        // ThrashTimer
        if (m_uiThrashTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_THRASH);
            m_uiThrashTimer = 10000;
        }
        else
            m_uiThrashTimer -= uiDiff;

        // HamstringTimer
        if (m_uiHamstringTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_HAMSTRING);
            m_uiHamstringTimer = 10000;
        }
        else
            m_uiHamstringTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL giant_eye_tentacleAI : public ScriptedAI
{
    giant_eye_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();

        if (Unit* pPortal = m_creature->SummonCreature(MOB_GIANT_PORTAL, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
            m_portalGuid = pPortal->GetObjectGuid();
    }

    uint32 m_uiBeamTimer;
    ObjectGuid m_portalGuid;

    void JustDied(Unit*)
    {
        if (Creature* pCreature = m_creature->GetMap()->GetCreature(m_portalGuid))
            pCreature->DealDamage(pCreature, pCreature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
    }

    void Reset()
    {
        //Green Beam half a second after we spawn
        m_uiBeamTimer = 500;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Check if we have a target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // BeamTimer
        if (m_uiBeamTimer < uiDiff)
        {
            Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            if (target && !target->HasAura(SPELL_DIGESTIVE_ACID, EFFECT_INDEX_0))
                DoCastSpellIfCan(target,SPELL_GREEN_BEAM);

            // Beam every 2 seconds
            m_uiBeamTimer = 2100;
        }
        else
            m_uiBeamTimer -= uiDiff;
    }
};

// Flesh tentacle functions
void flesh_tentacleAI::UpdateAI(const uint32 uiDiff)
{
    // Check if we have a target
    if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        return;

    if (m_cThunGuid)
    {
        if (m_uiCheckTimer < uiDiff)
        {
            Creature* pParent = m_creature->GetMap()->GetCreature(m_cThunGuid);

            if (!pParent || !pParent->isAlive() || !pParent->isInCombat())
            {
                m_cThunGuid.Clear();
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                return;
            }

            m_uiCheckTimer = 1000;
        }
        else
            m_uiCheckTimer -= uiDiff;
    }

    DoMeleeAttackIfReady();
}

void flesh_tentacleAI::JustDied(Unit* killer)
{
    if (!m_cThunGuid)
    {
        error_log("SD2: flesh_tentacle: No m_cThunGuid variable");
        return;
    }

    if (Creature* pCthun = m_creature->GetMap()->GetCreature(m_cThunGuid))
    {
        if (cthunAI* pCthunAI = dynamic_cast<cthunAI*>(pCthun->AI()))
            pCthunAI->FleshTentcleKilled();
    }
    else
        error_log("SD2: flesh_tentacle: No Cthun");
}

// GetAIs
CreatureAI* GetAI_eye_of_cthun(Creature* pCreature)
{
    return new eye_of_cthunAI(pCreature);
}

CreatureAI* GetAI_cthun(Creature* pCreature)
{
    return new cthunAI(pCreature);
}

CreatureAI* GetAI_eye_tentacle(Creature* pCreature)
{
    return new eye_tentacleAI(pCreature);
}

CreatureAI* GetAI_claw_tentacle(Creature* pCreature)
{
    return new claw_tentacleAI(pCreature);
}

CreatureAI* GetAI_giant_claw_tentacle(Creature* pCreature)
{
    return new giant_claw_tentacleAI(pCreature);
}

CreatureAI* GetAI_giant_eye_tentacle(Creature* pCreature)
{
    return new giant_eye_tentacleAI(pCreature);
}

CreatureAI* GetAI_flesh_tentacle(Creature* pCreature)
{
    return new flesh_tentacleAI(pCreature);
}

void AddSC_boss_cthun()
{
    Script* pNewScript;

    //Eye
    pNewScript = new Script;
    pNewScript->Name = "boss_eye_of_cthun";
    pNewScript->GetAI = &GetAI_eye_of_cthun;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_cthun";
    pNewScript->GetAI = &GetAI_cthun;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_eye_tentacle";
    pNewScript->GetAI = &GetAI_eye_tentacle;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_claw_tentacle";
    pNewScript->GetAI = &GetAI_claw_tentacle;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_giant_claw_tentacle";
    pNewScript->GetAI = &GetAI_giant_claw_tentacle;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_giant_eye_tentacle";
    pNewScript->GetAI = &GetAI_giant_eye_tentacle;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_giant_flesh_tentacle";
    pNewScript->GetAI = &GetAI_flesh_tentacle;
    pNewScript->RegisterSelf();
}
