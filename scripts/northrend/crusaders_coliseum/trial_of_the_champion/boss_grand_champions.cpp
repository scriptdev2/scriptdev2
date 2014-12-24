/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
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
SDName: grand_champions
SD%Complete: 40
SDComment: No Combat support; only basic encounter setup
SDCategory: Crusader Coliseum, Trial of the Champion
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_champion.h"
#include "TransportSystem.h"

enum
{
    // common spells
    SPELL_DEFEND_DUMMY              = 64101,                        // triggers 63130, 63131 and 63132
    SPELL_DEFEND                    = 62719,

    SPELL_SHIELD_BREAKER            = 68504,
    SPELL_CHARGE                    = 68301,                        // triggers 68321 - unk mechanics
    SPELL_FULL_HEAL                 = 43979,
    SPELL_RIDE_ARGENT_VEHICLE       = 69692,
};

/*######
## trial_companion_common
######*/

struct trial_companion_commonAI : public ScriptedAI
{
    trial_companion_commonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_champion*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_trial_of_the_champion* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiShieldBreakerTimer;
    uint32 m_uiChargeTimer;
    uint32 m_uiDefeatedTimer;

    bool m_bDefeated;

    ObjectGuid m_newMountGuid;

    void Reset() override
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ARENA_CHALLENGE) == DONE)
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            else
            {
                DoCastSpellIfCan(m_creature, SPELL_DEFEND, CAST_TRIGGERED);
                DoCastSpellIfCan(m_creature, SPELL_DEFEND_DUMMY, CAST_TRIGGERED);
            }
        }

        m_uiShieldBreakerTimer  = urand(3000, 5000);
        m_uiChargeTimer         = urand(1000, 3000);
        m_uiDefeatedTimer       = 0;

        m_bDefeated             = false;
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ARENA_CHALLENGE) == DONE)
                m_pInstance->SetData(TYPE_GRAND_CHAMPIONS, IN_PROGRESS);
        }
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ARENA_CHALLENGE) == DONE)
                m_pInstance->SetData(TYPE_GRAND_CHAMPIONS, FAIL);
        }
    }

    void AttackStart(Unit* pWho) override
    {
        ScriptedAI::AttackStart(pWho);

        // Set Mount control
        if (m_creature->GetTransportInfo() && m_creature->GetTransportInfo()->IsOnVehicle())
        {
            if (Creature* pMount = (Creature*)m_creature->GetTransportInfo()->GetTransport())
                pMount->AI()->AttackStart(pWho);
        }
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        // no aggro during the intro
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void DamageTaken(Unit* /*pDealer*/, uint32& uiDamage) override
    {
        if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;

            if (m_bDefeated)
                return;

            if (m_pInstance)
            {
                // second part of the champions challenge
                if (m_pInstance->GetData(TYPE_ARENA_CHALLENGE) == DONE)
                {
                    m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                    m_creature->SetHealth(1);

                    // no movement
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveIdle();

                    // check if the other champions are wounded and set instance data
                    if (m_pInstance->IsArenaChallengeComplete(TYPE_GRAND_CHAMPIONS))
                        m_pInstance->SetData(TYPE_GRAND_CHAMPIONS, DONE);
                }
                // first part of the champions challenge (arena encounter)
                else
                {
                    // unmount
                    if (Creature* pMount = (Creature*)m_creature->GetTransportInfo()->GetTransport())
                    {
                        pMount->RemoveSpellsCausingAura(SPELL_AURA_CONTROL_VEHICLE);
                        pMount->ForcedDespawn();
                    }

                    m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
                    m_creature->SetHealth(1);

                    // no movement
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveIdle();

                    m_uiDefeatedTimer = 15000;
                }
            }

            m_bDefeated = true;
        }
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId) override
    {
        if (uiMotionType != POINT_MOTION_TYPE || !m_pInstance)
            return;

        switch (uiPointId)
        {
            case POINT_ID_MOUNT:
            {
                // mount the closest vehicle and start attacking
                uint32 uiMountEntry = m_pInstance->GetMountEntryForChampion();

                // search for the vehicle again, just in case the previous one was taken
                Creature* pMount = m_creature->GetMap()->GetCreature(m_newMountGuid);
                if (pMount->HasAura(SPELL_RIDE_ARGENT_VEHICLE))
                    pMount = GetClosestCreatureWithEntry(m_creature, uiMountEntry, 60.0f);

                // if we don't have any mount send an error
                if (!pMount)
                {
                    script_error_log("Instance Trial of the Champion: ERROR Failed to get a mount replacement for champion %u.", m_creature->GetEntry());
                    return;
                }

                DoCastSpellIfCan(pMount, SPELL_RIDE_ARGENT_VEHICLE, CAST_TRIGGERED);

                if (m_creature->getVictim())
                    pMount->AI()->AttackStart(m_creature->getVictim());

                m_bDefeated = false;
                break;
            }
            case POINT_ID_EXIT:
                // mark the first part as complete if required
                if (m_pInstance->GetData(TYPE_GRAND_CHAMPIONS) != DONE)
                    m_pInstance->SetData(TYPE_ARENA_CHALLENGE, DONE);

                m_creature->ForcedDespawn();
                break;
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId) override
    {
        if (uiMotionType != POINT_MOTION_TYPE || uiPointId != POINT_ID_HOME || !m_pInstance)
            return;

        if (Creature* pCenterTrigger = m_pInstance->GetSingleCreatureFromStorage(NPC_WORLD_TRIGGER))
        {
            pSummoned->SetRespawnCoord(pSummoned->GetPositionX(), pSummoned->GetPositionY(), pSummoned->GetPositionZ(), pSummoned->GetAngle(pCenterTrigger));
            pSummoned->SetFacingToObject(pCenterTrigger);
        }
    }

    // function that will make the champion to use the nearby available mount
    void DoUseNearbyMountIfCan()
    {
        if (!m_pInstance)
            return;

        // set instance data as special if first part is completed
        if (m_pInstance->IsArenaChallengeComplete(TYPE_ARENA_CHALLENGE))
            m_pInstance->SetData(TYPE_ARENA_CHALLENGE, SPECIAL);
        else
        {
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);

            float fX, fY, fZ;
            uint32 uiMountEntry = m_pInstance->GetMountEntryForChampion();

            if (Creature* pMount = GetClosestCreatureWithEntry(m_creature, uiMountEntry, 60.0f))
            {
                pMount->GetContactPoint(m_creature, fX, fY, fZ);
                m_creature->SetWalk(true);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MovePoint(POINT_ID_MOUNT, fX, fY, fZ);

                m_newMountGuid = pMount->GetObjectGuid();
            }
        }
    }

    // Return true to handle shared timers and MeleeAttack
    virtual bool UpdateChampionAI(const uint32 /*uiDiff*/) { return true; }

    void UpdateAI(const uint32 uiDiff) override
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // timer for other champions check
        if (m_uiDefeatedTimer)
        {
            if (m_uiDefeatedTimer <= uiDiff)
            {
                DoUseNearbyMountIfCan();
                m_uiDefeatedTimer = 0;
            }
            else
                m_uiDefeatedTimer -= uiDiff;
        }

        // no combat after defeated
        if (m_bDefeated)
            return;

        if (!m_pInstance)
            return;

        // arena battle - on vehicles
        if (m_pInstance->GetData(TYPE_ARENA_CHALLENGE) == IN_PROGRESS)
        {
            if (m_uiShieldBreakerTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHIELD_BREAKER) == CAST_OK)
                    m_uiShieldBreakerTimer = urand(2000, 8000);
            }
            else
                m_uiShieldBreakerTimer -= uiDiff;

            if (m_uiChargeTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHARGE) == CAST_OK)
                    m_uiChargeTimer = urand(4000, 10000);
            }
            else
                m_uiChargeTimer -= uiDiff;
        }
        // arena challenge complete - start normal battle
        else if (m_pInstance->GetData(TYPE_ARENA_CHALLENGE) == DONE)
        {
            // Call specific virtual function
            if (!UpdateChampionAI(uiDiff))
                return;

            DoMeleeAttackIfReady();
        }
    }
};

enum
{
    // warrior spells
};

/*######
## boss_champion_warrior
######*/

struct boss_champion_warriorAI : public trial_companion_commonAI
{
    boss_champion_warriorAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_warrior(Creature* pCreature)
{
    return new boss_champion_warriorAI(pCreature);
}

enum
{
    // mage spells
};

/*######
## boss_champion_mage
######*/

struct boss_champion_mageAI : public trial_companion_commonAI
{
    boss_champion_mageAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_mage(Creature* pCreature)
{
    return new boss_champion_mageAI(pCreature);
}

enum
{
    // shaman spells
};

/*######
## boss_champion_shaman
######*/

struct boss_champion_shamanAI : public trial_companion_commonAI
{
    boss_champion_shamanAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_shaman(Creature* pCreature)
{
    return new boss_champion_shamanAI(pCreature);
}

enum
{
    // hunter spells
};

/*######
## boss_champion_hunter
######*/

struct boss_champion_hunterAI : public trial_companion_commonAI
{
    boss_champion_hunterAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_hunter(Creature* pCreature)
{
    return new boss_champion_hunterAI(pCreature);
}

enum
{
    // rogue spells
};

/*######
## boss_champion_rogue
######*/

struct boss_champion_rogueAI : public trial_companion_commonAI
{
    boss_champion_rogueAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateCompanionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_rogue(Creature* pCreature)
{
    return new boss_champion_rogueAI(pCreature);
}

/*######
## npc_champion_mount
######*/

enum
{

};

struct npc_champion_mountAI : public ScriptedAI
{
    npc_champion_mountAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_champion*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_champion* m_pInstance;

    void Reset() override { }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId) override
    {
        if (uiMotionType != POINT_MOTION_TYPE || !m_pInstance)
            return;

        switch (uiPointId)
        {
            case POINT_ID_CENTER:
                m_pInstance->MoveChampionToHome(m_creature);
                break;
            case POINT_ID_HOME:
                if (Creature* pCenterTrigger = m_pInstance->GetSingleCreatureFromStorage(NPC_WORLD_TRIGGER))
                {
                    m_creature->SetRespawnCoord(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetAngle(pCenterTrigger));
                    m_creature->SetFacingToObject(pCenterTrigger);
                }
                m_pInstance->InformChampionReachHome();
                break;
        }
    }

    void MoveInLineOfSight(Unit* /*pWho*/) override { }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_npc_champion_mount(Creature* pCreature)
{
    return new npc_champion_mountAI(pCreature);
}

void AddSC_boss_grand_champions()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_warrior";
    pNewScript->GetAI = &GetAI_boss_champion_warrior;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_mage";
    pNewScript->GetAI = &GetAI_boss_champion_mage;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_shaman";
    pNewScript->GetAI = &GetAI_boss_champion_shaman;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_hunter";
    pNewScript->GetAI = &GetAI_boss_champion_hunter;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_rogue";
    pNewScript->GetAI = &GetAI_boss_champion_rogue;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_champion_mount";
    pNewScript->GetAI = &GetAI_npc_champion_mount;
    pNewScript->RegisterSelf();
}
