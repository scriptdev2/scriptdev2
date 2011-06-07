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
SDName: Boss_Felblood_Kaelthas
SD%Complete: 80
SDComment: Normal and Heroic Support. Issues: Arcane Spheres do not initially follow targets.
SDCategory: Magisters' Terrace
EndScriptData */

#include "precompiled.h"
#include "magisters_terrace.h"
#include "WorldPacket.h"

enum
{
    SAY_AGGRO                   = -1585023,                 // This yell should be done when the room is cleared. For now, set it as a movelineofsight yell.
    SAY_PHOENIX                 = -1585024,
    SAY_FLAMESTRIKE             = -1585025,
    SAY_GRAVITY_LAPSE           = -1585026,
    SAY_TIRED                   = -1585027,
    SAY_RECAST_GRAVITY          = -1585028,
    SAY_DEATH                   = -1585029,

    // Phase 1 spells
    SPELL_FIREBALL              = 44189,                    // Deals 2700-3300 damage at current target
    SPELL_FIREBALL_H            = 46164,                    //       4950-6050

    SPELL_PHOENIX               = 44194,                    // Summons a phoenix
    SPELL_PHOENIX_BURN          = 44197,                    // A spell Phoenix uses to damage everything around
    SPELL_REBIRTH_DMG           = 44196,                    // DMG if a Phoenix rebirth happen

    SPELL_FLAME_STRIKE_DUMMY    = 44191,                    // Flamestrike indicator before the damage
    SPELL_FLAME_STRIKE          = 44192,                    // Summons the trigger + animation (projectile)

    SPELL_SHOCK_BARRIER         = 46165,                    // Heroic only; 10k damage shield, followed by Pyroblast
    SPELL_PYROBLAST             = 36819,                    // Heroic only; 45-55k fire damage

    // Phase 2 spells
    SPELL_GRAVITY_LAPSE_INITIAL = 44224,                    // Cast at the beginning of every Gravity Lapse
    SPELL_GRAVITY_LAPSE_CHANNEL = 44251,                    // Channeled; blue beam animation to every enemy in range
    SPELL_TELEPORT_CENTER       = 44218,                    // Should teleport people to the center. Requires DB entry in spell_target_position.
    SPELL_GRAVITY_LAPSE_FLY     = 44227,                    // Hastens flyspeed and allows flying for 1 minute. For some reason removes 44226.
    SPELL_GRAVITY_LAPSE_DOT     = 44226,                    // Knocks up in the air and applies a 300 DPS DoT.
    SPELL_ARCANE_SPHERE_PASSIVE = 44263,                    // Passive auras on Arcane Spheres
    SPELL_POWER_FEEDBACK        = 44233,                    // Stuns him, making him take 50% more damage for 10 seconds. Cast after Gravity Lapse

    NPC_FLAME_STRIKE_TRIGGER    = 24666,
    NPC_PHOENIX                 = 24674,
    NPC_PHOENIX_EGG             = 24675,
    NPC_ARCANE_SPHERE           = 24708,
};

/** Locations **/
static const float KaelLocations[3][2]=
{
    {148.744659f, 181.377426f},
    {140.823883f, 195.403046f},
    {156.574188f, 195.650482f},
};

static const float LOCATION_Z   = -16.727455f;

struct MANGOS_DLL_DECL boss_felblood_kaelthasAI : public ScriptedAI
{
    boss_felblood_kaelthasAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFireballTimer;
    uint32 m_uiPhoenixTimer;
    uint32 m_uiFlameStrikeTimer;
    uint32 m_uiCombatPulseTimer;

    // Heroic only
    uint32 m_uiPyroblastTimer;

    uint32 m_uiGravityLapseTimer;
    uint32 m_uiGravityLapsePhase;
    // 0 = No Gravity Lapse
    // 1 = Casting Gravity Lapse visual
    // 2 = Teleported people to self
    // 3 = Knocked people up in the air
    // 4 = Applied an aura that allows them to fly, channeling visual, relased Arcane Orbs.

    bool m_bFirstGravityLapse;
    bool m_bHasTaunted;

    uint8 m_uiPhase;
    // 0 = Not started
    // 1 = Fireball; Summon Phoenix; Flamestrike
    // 2 = Gravity Lapses

    void Reset()
    {
        // TODO: Timers
        m_uiFireballTimer = 0;
        m_uiPhoenixTimer = 10000;
        m_uiFlameStrikeTimer = 25000;
        m_uiCombatPulseTimer = 0;

        m_uiPyroblastTimer = 60000;

        m_uiGravityLapseTimer = 0;
        m_uiGravityLapsePhase = 0;

        m_bFirstGravityLapse = true;
        m_bHasTaunted = false;

        m_uiPhase = 0;
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KAELTHAS, DONE);
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
            RemoveGravityLapse();                           // Remove Gravity Lapse so that players fall to ground if they kill him when in air.
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KAELTHAS, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KAELTHAS, FAIL);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && m_creature->IsWithinDistInMap(pWho, 40.0))
        {
            DoScriptText(SAY_AGGRO, m_creature);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_FLAME_STRIKE_TRIGGER:
                pSummoned->CastSpell(pSummoned, SPELL_FLAME_STRIKE_DUMMY, false, NULL, NULL, m_creature->GetObjectGuid());
                break;
            case NPC_PHOENIX:
                // TODO ThreatList and Flag removal likely wrong
                pSummoned->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_NON_ATTACKABLE);
                SetThreatList(pSummoned);

                if (m_creature->getVictim())
                    pSummoned->AI()->AttackStart(m_creature->getVictim());
                break;
        }
    }

    void SetThreatList(Creature* SummonedUnit)
    {
        if (!SummonedUnit)
            return;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit((*i)->getUnitGuid());
            if (pUnit && pUnit->isAlive())
            {
                float threat = m_creature->getThreatManager().getThreat(pUnit);
                SummonedUnit->AddThreat(pUnit, threat);
            }
        }
    }

    void TeleportPlayersToSelf()
    {
        float x = KaelLocations[0][0];
        float y = KaelLocations[0][1];

        DoCastSpellIfCan(m_creature, SPELL_TELEPORT_CENTER, CAST_TRIGGERED);

        std::vector<ObjectGuid> vGuids;
        m_creature->FillGuidsListFromThreatList(vGuids);
        for (std::vector<ObjectGuid>::const_iterator i = vGuids.begin();i != vGuids.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit(*i);

            if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
                pUnit->CastSpell(pUnit, SPELL_TELEPORT_CENTER, true);
        }
    }

    void CastGravityLapseKnockUp()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit((*i)->getUnitGuid());

            // Knockback into the air
            if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
                pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE_DOT, true, 0, 0, m_creature->GetObjectGuid());
        }
    }

    // players can't cast "fly" spells unless in map 530. Has to be done a while after they get knocked into the air...
    void CastGravityLapseFly()
    {
        std::vector<ObjectGuid> vGuids;
        m_creature->FillGuidsListFromThreatList(vGuids);
        for (std::vector<ObjectGuid>::const_iterator i = vGuids.begin();i != vGuids.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit(*i);

            // Also needs an exception in spell system.
            if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
                pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE_FLY, true, 0, 0, m_creature->GetObjectGuid());
        }
    }

    void RemoveGravityLapse()
    {
        std::vector<ObjectGuid> vGuids;
        m_creature->FillGuidsListFromThreatList(vGuids);
        for (std::vector<ObjectGuid>::const_iterator i = vGuids.begin();i != vGuids.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit(*i);

            if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
            {
                pUnit->RemoveAurasDueToSpell(SPELL_GRAVITY_LAPSE_FLY);
                pUnit->RemoveAurasDueToSpell(SPELL_GRAVITY_LAPSE_DOT);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_uiPhase)
        {
            case 0:
            {
                // *Heroic mode only:
                if (!m_bIsRegularMode)
                {
                    if (m_uiPyroblastTimer < uiDiff)
                    {
                        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
                        m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                        DoCastSpellIfCan(m_creature, SPELL_SHOCK_BARRIER, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature->getVictim(), SPELL_PYROBLAST);
                        m_uiPyroblastTimer = 60000;
                    }
                    else
                        m_uiPyroblastTimer -= uiDiff;
                }

                if (m_uiFireballTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H) == CAST_OK)
                        m_uiFireballTimer = urand(2000, 6000);
                }
                else
                    m_uiFireballTimer -= uiDiff;

                if (m_uiPhoenixTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_PHOENIX) == CAST_OK)
                    {
                        DoScriptText(SAY_PHOENIX, m_creature);
                        m_uiPhoenixTimer = 60000;
                    }
                }
                else
                    m_uiPhoenixTimer -= uiDiff;

                if (m_uiFlameStrikeTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        DoCastSpellIfCan(pTarget, SPELL_FLAME_STRIKE, CAST_INTERRUPT_PREVIOUS);
                        DoScriptText(SAY_FLAMESTRIKE, m_creature);
                    }
                    m_uiFlameStrikeTimer = urand(15000, 25000);
                }
                else
                    m_uiFlameStrikeTimer -= uiDiff;

                // Below 50%
                if (m_creature->GetHealthPercent() < 50.0f)
                {
                    m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
                    m_creature->StopMoving();
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveIdle();
                    m_uiGravityLapseTimer = 0;
                    m_uiGravityLapsePhase = 0;
                    m_uiPhase = 1;

                    return;
                }

                DoMeleeAttackIfReady();

                break;
            }
            case 1:
            {
                if (m_uiGravityLapseTimer < uiDiff)
                {
                    switch (m_uiGravityLapsePhase)
                    {
                        case 0:
                            if (m_bFirstGravityLapse)       // Different yells at 50%, and at every following Gravity Lapse
                            {
                                DoScriptText(SAY_GRAVITY_LAPSE, m_creature);
                                m_bFirstGravityLapse = false;

                                if (m_pInstance)
                                {
                                    if (GameObject* pKaelLeft = m_pInstance->GetSingleGameObjectFromStorage(GO_KAEL_STATUE_LEFT))
                                        pKaelLeft->SetGoState(GO_STATE_ACTIVE);

                                    if (GameObject* pKaelRight =  m_pInstance->GetSingleGameObjectFromStorage(GO_KAEL_STATUE_RIGHT))
                                        pKaelRight->SetGoState(GO_STATE_ACTIVE);
                                }
                            }
                            else
                            {
                                DoScriptText(SAY_RECAST_GRAVITY, m_creature);
                            }

                            DoCastSpellIfCan(m_creature, SPELL_GRAVITY_LAPSE_INITIAL);
                            m_uiGravityLapseTimer = 2000 + uiDiff;// Don't interrupt the visual spell
                            m_uiGravityLapsePhase = 1;
                            break;

                        case 1:
                            TeleportPlayersToSelf();
                            m_uiGravityLapseTimer = 1000;
                            m_uiGravityLapsePhase = 2;
                            break;

                        case 2:
                            CastGravityLapseKnockUp();
                            m_uiGravityLapseTimer = 1000;
                            m_uiGravityLapsePhase = 3;
                            break;

                        case 3:
                            CastGravityLapseFly();
                            m_uiGravityLapseTimer = 30000;
                            m_uiGravityLapsePhase = 4;

                            for(uint8 i = 0; i < 3; ++i)
                            {
                                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                                {
                                    if (Creature* Orb = DoSpawnCreature(NPC_ARCANE_SPHERE, 5, 5, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                                        Orb->AI()->AttackStart(pTarget);
                                }
                            }

                            DoCastSpellIfCan(m_creature, SPELL_GRAVITY_LAPSE_CHANNEL);
                            break;

                        case 4:
                            m_creature->InterruptNonMeleeSpells(false);
                            DoScriptText(SAY_TIRED, m_creature);
                            DoCastSpellIfCan(m_creature, SPELL_POWER_FEEDBACK);
                            RemoveGravityLapse();
                            m_uiGravityLapseTimer = 10000;
                            m_uiGravityLapsePhase = 0;
                            break;
                    }
                }
                else
                    m_uiGravityLapseTimer -= uiDiff;

                break;
            }
        }
    }
};

struct MANGOS_DLL_DECL mob_felkael_phoenixAI : public ScriptedAI
{
    mob_felkael_phoenixAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiBurnTimer;
    uint32 m_uiDeathTimer;
    bool m_bRebirth;
    bool m_bFakeDeath;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        m_creature->CastSpell(m_creature, SPELL_PHOENIX_BURN, true);

        m_uiBurnTimer = 2000;
        m_uiDeathTimer = 3000;
        m_bRebirth = false;
        m_bFakeDeath = false;
    }

    void DamageTaken(Unit* pKiller, uint32& uiDamage)
    {
        if (uiDamage < m_creature->GetHealth())
            return;

        // Prevent glitch if in fake death
        if (m_bFakeDeath)
        {
            uiDamage = 0;
            return;

        }
        // Don't really die in all phases of Kael'Thas
        if (m_pInstance && m_pInstance->GetData(TYPE_KAELTHAS) == IN_PROGRESS)
        {
            // prevent death
            uiDamage = 0;
            m_bFakeDeath = true;

            m_creature->InterruptNonMeleeSpells(false);
            m_creature->SetHealth(0);
            m_creature->StopMoving();
            m_creature->ClearComboPointHolders();
            m_creature->RemoveAllAurasOnDeath();
            m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
            m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_35_PERCENT, false);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->ClearAllReactives();
            m_creature->SetTargetGuid(ObjectGuid());
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
       }
    }

    void UpdateAI(const uint32 uiDiff)
    {

        // If we are fake death, we cast rebirth and after that we kill the phoenix to spawn the egg.
        if (m_bFakeDeath)
        {
            if (!m_bRebirth)
            {
                DoCastSpellIfCan(m_creature, SPELL_REBIRTH_DMG);
                m_bRebirth = true;
            }

            if (m_bRebirth)
            {

                if (m_uiDeathTimer < uiDiff)
                {
                    m_creature->SummonCreature(NPC_PHOENIX_EGG, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000);
                    m_creature->SetDeathState(JUST_DIED);
                    m_creature->RemoveCorpse();
                    m_bRebirth = false;
                }
                else
                    m_uiDeathTimer -= uiDiff;
            }

            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBurnTimer < uiDiff)
        {
            //spell Burn should possible do this, but it doesn't, so do this for now.
            uint32 dmg = urand(1650,2050);
            m_creature->DealDamage(m_creature, dmg, 0, DOT, SPELL_SCHOOL_MASK_FIRE, NULL, false);
            m_uiBurnTimer += 2000;
        }
        else
            m_uiBurnTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_felkael_phoenix_eggAI : public ScriptedAI
{
    mob_felkael_phoenix_eggAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiHatchTimer;

    void Reset()
    {
        m_uiHatchTimer = 10000;
    }

    void MoveInLineOfSight(Unit* pWho) {}
    void AttackStart(Unit* pWho) {}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiHatchTimer < uiDiff)
        {
            m_creature->SummonCreature(NPC_PHOENIX, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
        else
            m_uiHatchTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_arcane_sphereAI : public ScriptedAI
{
    mob_arcane_sphereAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiDespawnTimer;
    uint32 m_uiChangeTargetTimer;

    void Reset()
    {
        m_uiDespawnTimer = 30000;
        m_uiChangeTargetTimer = urand(6000, 12000);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        DoCastSpellIfCan(m_creature, SPELL_ARCANE_SPHERE_PASSIVE, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiDespawnTimer < uiDiff)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            return;
        }
        else
            m_uiDespawnTimer -= uiDiff;

        // Note: This check was reversed for some unknown reason, changed to usual behaviour!
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiChangeTargetTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                m_creature->TauntApply(pTarget);            // Useless Workaround
                AttackStart(pTarget);
            }

            m_uiChangeTargetTimer = urand(5000, 15000);
        }
        else
            m_uiChangeTargetTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_felblood_kaelthas(Creature* pCreature)
{
    return new boss_felblood_kaelthasAI(pCreature);
}

CreatureAI* GetAI_mob_arcane_sphere(Creature* pCreature)
{
    return new mob_arcane_sphereAI(pCreature);
}

CreatureAI* GetAI_mob_felkael_phoenix(Creature* pCreature)
{
    return new mob_felkael_phoenixAI(pCreature);
}

CreatureAI* GetAI_mob_felkael_phoenix_egg(Creature* pCreature)
{
    return new mob_felkael_phoenix_eggAI(pCreature);
}

void AddSC_boss_felblood_kaelthas()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_felblood_kaelthas";
    newscript->GetAI = &GetAI_boss_felblood_kaelthas;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_arcane_sphere";
    newscript->GetAI = &GetAI_mob_arcane_sphere;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_felkael_phoenix";
    newscript->GetAI = &GetAI_mob_felkael_phoenix;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_felkael_phoenix_egg";
    newscript->GetAI = &GetAI_mob_felkael_phoenix_egg;
    newscript->RegisterSelf();
}
