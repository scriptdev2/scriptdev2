/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "../../../sc_defines.h"
#include "def_hyjal.h"
#include "../../../../../../game/TargetedMovementGenerator.h"
#include "../../../../../../game/SpellAuras.h"

#define SPELL_DENOUEMENT_WISP      32124 // Wisps use this
#define SPELL_PROTECTION_OF_ELUNE  38528

#define SPELL_DRAIN_WORLD_TREE      39140
#define SPELL_DRAIN_WORLD_TREE_2    39141

#define SPELL_FINGER_OF_DEATH       31984
#define SPELL_HAND_OF_DEATH         35354
#define SPELL_AIR_BURST             32014
#define SPELL_GRIP_OF_THE_LEGION    31972
#define SPELL_DOOMFIRE_SPAWN        32074
#define SPELL_DOOMFIRE_VISUAL       31945
#define SPELL_DOOMFIRE_DAMAGE       31944
#define SPELL_SOUL_CHARGE_YELLOW    32045
#define SPELL_SOUL_CHARGE_GREEN     32051
#define SPELL_SOUL_CHARGE_RED       32052
#define SPELL_UNLEASH_SOUL_YELLOW   32054
#define SPELL_UNLEASH_SOUL_GREEN    32057
#define SPELL_UNLEASH_SOUL_RED      32053
#define SPELL_FEAR                  41150

#define SAY_AGGRO           "Your resistance is insignificant!"
#define SOUND_AGGRO         10987

#define SAY_DOOMFIRE1       "This world will burn!"
#define SOUND_DOOMFIRE1     10990

#define SAY_DOOMFIRE2       "Manach sheek-thrish!"
#define SOUND_DOOMFIRE2     11041

#define SAY_AIR_BURST       "A-kreesh!"
#define SOUND_AIR_BURST     10989

#define SAY_AIR_BURST2      "Away vermin!"
#define SOUND_AIR_BURST2    11043

#define SAY_SLAY1           "All creation will be devoured!"
#define SOUND_SLAY1         11044

#define SAY_SLAY2           "Your soul will languish for eternity."
#define SOUND_SLAY2         10991

#define SAY_SLAY3           "I am the coming of the end!"
#define SOUND_SLAY3         11045

#define SAY_UNK1           "You are mine now."
#define SOUND_UNK1          10988

#define SAY_UNK2            "Bow to my will."
#define SOUND_UNK2          11042

#define SAY_ENRAGE          "At last it is here. Mourn and lament the passing of all you have ever known and all that would have been! Akmin-kurai!"
#define SOUND_ENRAGE        10993

#define SAY_DEATH           "No, it cannot be! Nooo!"
#define SOUND_DEATH         10992

struct mob_ancient_wispAI : public ScriptedAI
{
    mob_ancient_wispAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;
    uint64 ArchimondeGUID;
    uint32 CheckTimer;

    void Reset()
    {
        ArchimondeGUID = 0;
        CheckTimer = 1000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!ArchimondeGUID)
        {
            if(pInstance)
                ArchimondeGUID = pInstance->GetData64(DATA_ARCHIMONDE);
        }

        if(CheckTimer < diff)
        {
            if(ArchimondeGUID)
            {
                Unit* Archimonde = Unit::GetUnit((*m_creature), ArchimondeGUID);
                if(Archimonde && (((Archimonde->GetHealth()*100) / Archimonde->GetMaxHealth()) < 2))
                    DoCast(Archimonde, SPELL_DENOUEMENT_WISP);
            }
            CheckTimer = 1000;
        }else CheckTimer -= diff;
    }
};

/* This script controls the Doomfire mob. Unlike the other Doomfire mob, this one does not stalk players.
   Instead, this doomfire will simply stand in one place after spawning and deal damage to any players that
   are within 3 yards. Another creature called Doomfire Targetting spawns this creature as well as stalks. */
struct MANGOS_DLL_DECL mob_doomfireAI : public ScriptedAI
{
    mob_doomfireAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
    }

    uint32 CheckTimer;

    bool TargetSelected;

    uint64 ArchimondeGUID;
    uint64 TargetGUID;

    void Reset()
    {
        CheckTimer = 5000;

        TargetSelected = false;

        ArchimondeGUID = 0;
        TargetGUID = 0;
    }

    void AttackStart(Unit* who) { return; }

    void MoveInLineOfSight(Unit* who)
    {
        if(!who || who == m_creature)
            return;

        if(m_creature->IsWithinDistInMap(who, 3))
        {
            TargetSelected = true;
            TargetGUID = who->GetGUID();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(TargetSelected && TargetGUID)
        {
            Unit* target = Unit::GetUnit((*m_creature), TargetGUID);
            if(target && target->isAlive())
            {
                DoCast(target, SPELL_DOOMFIRE_DAMAGE);
                TargetGUID = 0;
                TargetSelected = false;
            }
        }

        if(CheckTimer < diff)
        {
            if(ArchimondeGUID)
            {
                Unit* Archimonde = Unit::GetUnit((*m_creature), ArchimondeGUID);
                if(!Archimonde || !Archimonde->isAlive())
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
                CheckTimer = 5000;
            }
            else m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
        }else CheckTimer -= diff;
    }
};

/* This is the script for the Doomfire Targetting Mob. This mob simply follows players and spawns the actual Doomfire
   which does damage to anyone that moves close. Unfortunately, we cannot make Doomfire Targetting go in random directions
   so we shall simply make it change from to a new random (except tank) target to another every five or so seconds. */
struct MANGOS_DLL_DECL mob_doomfire_targettingAI : public ScriptedAI
{
    mob_doomfire_targettingAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
    }

    uint32 ChangeTargetTimer;
    uint32 SummonTimer; // This timer will serve as both a summon timer for the doomfire that does damage as well as to check on Archionde

    uint64 ArchimondeGUID;

    void Reset()
    {
        ChangeTargetTimer = 5000;
        SummonTimer = 1000;

        ArchimondeGUID = 0;
    }

    void AttackStart(Unit* who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if(SummonTimer < diff)
        {
            if(ArchimondeGUID)
            {
                Unit* Archimonde = Unit::GetUnit((*m_creature), ArchimondeGUID);
                if(Archimonde && Archimonde->isAlive())
                {
                    Creature* Doomfire = DoSpawnCreature(18095, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
                    if(Doomfire)
                        ((mob_doomfireAI*)Doomfire->AI())->ArchimondeGUID = ArchimondeGUID;
                    SummonTimer = 1000;
                }
                else
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
            }
            else
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);

        }else SummonTimer -= diff;

        if(ChangeTargetTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);
            if(target && target->isAlive())
                m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*target));
        }else ChangeTargetTimer -= diff;
    }

};

/* Finally, Archimonde's script. His script isn't extremely complex, most are simply spells on timers.
   The only complicated aspect of the battle is Finger of Death and Doomfire, with Doomfire being the
   hardest bit to code. Finger of Death is simply a distance check - if no one is in melee range, then
   select a random target and cast the spell on them. However, if someone IS in melee range, and this
   is NOT the main tank (creature's victim), then we aggro that player and they become the new victim.
   For Doomfire, we summon a mob (Doomfire Targetting) that summons another mob (Doomfire every second)
   Doomfire Targetting 'stalks' players whilst Doomfire damages player that are within range. */

// This is used to sort by distance in order to see who is the closest target, when checking for Finger of Death
struct TargetDistanceOrder : public std::binary_function<const Unit, const Unit, bool>
{
    const Unit* MainTarget;
    TargetDistanceOrder(const Unit* Target) : MainTarget(Target) {};
    // functor for operator "<"
    bool operator()(const Unit* _Left, const Unit* _Right) const
    {
        return (MainTarget->GetDistanceSq(_Left) < MainTarget->GetDistanceSq(_Right));
    }
};

struct MANGOS_DLL_DECL boss_archimondeAI : public ScriptedAI
{
    boss_archimondeAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;
    
    uint32 FearTimer;
    uint32 AirBurstTimer;
    uint32 GripOfTheLegionTimer;
    uint32 DoomfireTimer;
    uint32 SoulChargeTimer;
    uint32 MeleeRangeCheckTimer;
    uint32 HandOfDeathTimer;
    uint32 SummonWispTimer;
    uint32 GlobalSoulChargeCooldown; // This prevents spamming Soul Charges too quickly.

    bool InCombat;
    bool SoulCharged;
    bool BelowTenPercent;
    bool HasProtected;

    void Reset()
    {
        if(pInstance)
            pInstance->SetData(DATA_ARCHIMONDEEVENT, 0);

        FearTimer = 40000;
        AirBurstTimer = 30000;
        GripOfTheLegionTimer = 5000 + rand()%20000;
        DoomfireTimer = 20000;
        SoulChargeTimer = 15000 + rand()%105000;
        MeleeRangeCheckTimer = 15000;
        HandOfDeathTimer = 5000;
        SummonWispTimer = 1500;
        GlobalSoulChargeCooldown = 0;

        InCombat = false;
        SoulCharged = false;
        BelowTenPercent = false;
        HasProtected = false;
    }

    void AttackStart(Unit *who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);

                if(!InCombat)
                {
                    DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    pInstance->SetData(DATA_ARCHIMONDEEVENT, 1);
                    InCombat = true;
                }
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
           case 0:
                DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY1);
                break;
            case 1:
                DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY2);
                break;
            case 2:
                DoYell(SAY_SLAY3,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY3);
                break;
        }

        if(victim && (victim->GetTypeId() == TYPEID_PLAYER))
            GainSoulCharge(((Player*)victim));
    }
    
    void GainSoulCharge(Player* victim)
    {
        switch(victim->getClass())
        {
            case CLASS_PRIEST:
            case CLASS_PALADIN:
            case CLASS_WARLOCK:
                victim->CastSpell(m_creature, SPELL_SOUL_CHARGE_RED, true);
                break;
            case CLASS_MAGE:
            case CLASS_ROGUE:
            case CLASS_WARRIOR:
                victim->CastSpell(m_creature, SPELL_SOUL_CHARGE_YELLOW, true);
                break;
            case CLASS_DRUID:
            case CLASS_SHAMAN:
            case CLASS_HUNTER:
                victim->CastSpell(m_creature, SPELL_SOUL_CHARGE_GREEN, true);
                break;
        }

        SoulCharged = true;
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);

        if(pInstance)
            pInstance->SetData(DATA_ARCHIMONDEEVENT, 3);
    }

    bool CanUseFingerOfDeath()
    {
        // First we check if our current victim is in melee range or not.
        Unit* victim = m_creature->getVictim();
        if(victim && m_creature->IsWithinDistInMap(victim, m_creature->GetAttackDistance(victim)))
            return false;

        std::list<HostilReference*> m_threatlist = m_creature->getThreatManager().getThreatList();
        if(m_threatlist.empty())
            return false;

        std::list<Unit*> targets;
        std::list<HostilReference*>::iterator itr = m_threatlist.begin();
        for( ; itr != m_threatlist.end(); ++itr)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
            if(pUnit && pUnit->isAlive())
                targets.push_back(pUnit);
        }

        if(targets.empty())
            return false;

        targets.sort(TargetDistanceOrder(m_creature));
        Unit* target = targets.front();
        if(target)
        {
            if(!m_creature->IsWithinDistInMap(target, m_creature->GetAttackDistance(target)))
                return true; // Cast Finger of Death
            else // This target is closest, he is our new tank
                m_creature->AddThreat(target, m_creature->getThreatManager().getThreat(m_creature->getVictim()));
        }

        return false;
    }

    void SetThreatList(Creature* target)
    {
        if(!target)
            return;

        std::list<HostilReference*> m_threatlist = m_creature->getThreatManager().getThreatList();
        
        if(m_threatlist.empty())
            return;

        std::list<HostilReference*>::iterator itr = m_threatlist.begin();
        for( ; itr != m_threatlist.end(); ++itr)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
            if(pUnit && pUnit->isAlive() && (pUnit->GetTypeId() == TYPEID_PLAYER)) // Only copy PLAYERS' threat.
                target->AddThreat(pUnit, m_creature->getThreatManager().getThreat(pUnit));
        }
    }

    void SummonDoomfire(Unit* target)
    {    
        Creature* Doomfire = DoSpawnCreature(18104, rand()%30, rand()%30, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
        if(Doomfire)
        {
            m_creature->SetLootRecipient(NULL);
            ((mob_doomfire_targettingAI*)Doomfire->AI())->ArchimondeGUID = m_creature->GetGUID();
            SetThreatList(Doomfire);
            Doomfire->setFaction(m_creature->getFaction());
            DoCast(Doomfire, SPELL_DOOMFIRE_SPAWN);
            Doomfire->CastSpell(Doomfire, SPELL_DOOMFIRE_VISUAL, true);
            if(target)
                Doomfire->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*target));
            if(rand()%2 == 0)
            {
                DoYell(SAY_DOOMFIRE1, LANG_UNIVERSAL, m_creature);
                DoPlaySoundToSet(m_creature, SOUND_DOOMFIRE1);
            }else
            {
                DoYell(SAY_DOOMFIRE2, LANG_UNIVERSAL, m_creature);
                DoPlaySoundToSet(m_creature, SOUND_DOOMFIRE2);
            }
        }
    }

    void UnleashSoulCharge()
    {
        bool HasCast = false;
        uint8 stacks = 0;
        uint8 totalStacks = 0;
        switch(rand()%3)
        {
            case 0:
                if(m_creature->HasAura(SPELL_SOUL_CHARGE_RED, 0))
                {
                    stacks = m_creature->GetAura(SPELL_SOUL_CHARGE_RED, 0)->GetAuraSlot();
                    totalStacks += stacks-1;
                    if(stacks > 2)
                        m_creature->RemoveSingleAuraFromStack(SPELL_SOUL_CHARGE_RED, 0);
                    else
                        m_creature->RemoveAurasDueToSpell(SPELL_SOUL_CHARGE_RED);
                    DoCast(m_creature->getVictim(), SPELL_UNLEASH_SOUL_RED);
                    HasCast = true;
                }
            case 1:
                if(m_creature->HasAura(SPELL_SOUL_CHARGE_GREEN, 0))
                {
                    stacks = m_creature->GetAura(SPELL_SOUL_CHARGE_GREEN, 0)->GetAuraSlot();
                    totalStacks += stacks-1;
                    if(stacks > 2)
                        m_creature->RemoveSingleAuraFromStack(SPELL_SOUL_CHARGE_GREEN, 0);
                    else
                        m_creature->RemoveAurasDueToSpell(SPELL_SOUL_CHARGE_GREEN);
                    DoCast(m_creature->getVictim(), SPELL_UNLEASH_SOUL_GREEN);
                    HasCast = true;
                }
            case 2:
                if(m_creature->HasAura(SPELL_SOUL_CHARGE_YELLOW, 0))
                {
                    stacks = m_creature->GetAura(SPELL_SOUL_CHARGE_YELLOW, 0)->GetAuraSlot();
                    totalStacks += stacks-1;
                    if(stacks > 2)
                        m_creature->RemoveSingleAuraFromStack(SPELL_SOUL_CHARGE_YELLOW, 0);
                    else
                        m_creature->RemoveAurasDueToSpell(SPELL_SOUL_CHARGE_YELLOW);
                    DoCast(m_creature->getVictim(), SPELL_UNLEASH_SOUL_YELLOW);
                    HasCast = true;
                }
        }
        if(HasCast)
        {
            uint32 Divider = 1;
            if(!totalStacks)
                SoulCharged = false;
            else Divider = totalStacks;

            SoulChargeTimer = (15000 + rand()%105000)/Divider;
            if(Divider > 4)
                GlobalSoulChargeCooldown += 5000;
            else GlobalSoulChargeCooldown += 1000;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
    
        if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10) && !BelowTenPercent)
            BelowTenPercent = true;
        
        if(BelowTenPercent)
        {
            if(!HasProtected)
            {
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->Idle();
                DoCast(m_creature->getVictim(), SPELL_PROTECTION_OF_ELUNE);
                HasProtected = true;
            }

            if(HandOfDeathTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_HAND_OF_DEATH);
                HandOfDeathTimer = 1000;
            }else HandOfDeathTimer -= diff;

            if(SummonWispTimer < diff)
            {
                Creature* Wisp = DoSpawnCreature(17946, rand()%40, rand()%40, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if(Wisp)
                {
                    Wisp->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature));
                    ((mob_ancient_wispAI*)Wisp->AI())->ArchimondeGUID = m_creature->GetGUID();
                }
                SummonWispTimer = 5000;
            }else SummonWispTimer -= diff;
            return; // Don't do anything after this point.
        }

        if(SoulCharged)
        {
            if(GlobalSoulChargeCooldown < diff)
            {
                if(SoulChargeTimer < diff)
                    UnleashSoulCharge();
                else SoulChargeTimer -= diff;
            }else GlobalSoulChargeCooldown -= diff;
        }

        if(GripOfTheLegionTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_GRIP_OF_THE_LEGION);
            GripOfTheLegionTimer = 5000 + rand()%20000;
        }else GripOfTheLegionTimer -= diff;

        if(AirBurstTimer < diff)
        {
            if(rand()%2 == 0)
            {
                DoYell(SAY_AIR_BURST, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AIR_BURST);
            }else
            {
                DoYell(SAY_AIR_BURST2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AIR_BURST2);
            }

            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_AIR_BURST);
            AirBurstTimer = 25000 + rand()%15000;
        }else AirBurstTimer -= diff;

        if(FearTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FEAR);
            FearTimer = 40000;
        }else FearTimer -= diff;

        if(DoomfireTimer < diff)
        {
            SummonDoomfire(SelectUnit(SELECT_TARGET_RANDOM, 1));
            DoomfireTimer = 30000;
        }else DoomfireTimer -= diff;

        if(MeleeRangeCheckTimer < diff)
        {
            if(CanUseFingerOfDeath())
            {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_FINGER_OF_DEATH);
                MeleeRangeCheckTimer = 1000;
            }

            MeleeRangeCheckTimer = 5000;
        }else MeleeRangeCheckTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_archimonde(Creature *_Creature)
{
    return new boss_archimondeAI (_Creature);
}

CreatureAI* GetAI_mob_doomfire(Creature* _Creature)
{
    return new mob_doomfireAI(_Creature);
}

CreatureAI* GetAI_mob_doomfire_targetting(Creature* _Creature)
{
    return new mob_doomfire_targettingAI(_Creature);
}

CreatureAI* GetAI_mob_ancient_wisp(Creature* _Creature)
{
    return new mob_ancient_wispAI(_Creature);
}

void AddSC_boss_archimonde()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_archimonde";
    newscript->GetAI = GetAI_boss_archimonde;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name = "mob_doomfire";
    newscript->GetAI = GetAI_mob_doomfire;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_doomfire_targetting";
    newscript->GetAI = GetAI_mob_doomfire_targetting;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_ancient_wisp";
    newscript->GetAI = GetAI_mob_ancient_wisp;
    m_scripts[nrscripts++] = newscript;
}