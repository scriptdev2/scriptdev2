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

#define SPELL_DENOUEMENT_WISP      32124 // Wisps use this
#define SPELL_PROTECTION_OF_ELUNE  38528

#define SPELL_DRAIN_WORLD_TREE      39140
#define SPELL_DRAIN_WORLD_TREE_2    39141

#define SPELL_FINGER_OF_DEATH       31984
#define SPELL_HAND_OF_DEATH         35354
#define SPELL_AIR_BURST             32014
#define SPELL_GRIP_OF_THE_LEGION    31972
#define SPELL_DOOMFIRE_SPAWN        32045
#define SPELL_DOOMFIRE_VISUAL       31945
#define SPELL_DOOMFIRE_DAMAGE       31969
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

#define SAY_AIR_BURST2       "Away vermin!"
#define SOUND_AIR_BURST2     11043

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

struct mob_doomfireAI : public ScriptedAI
{
    mob_doomfireAI(Creature* c) : ScriptedAI(c) {}

    uint32 CastTimer;

    void Reset()
    {
        CastTimer = 1000;
    }

    void AttackStart(Unit* who) { return; }

    void MoveInLineOfSight(Unit* who)
    {
        if(!who || who->GetEntry() == m_creature->GetEntry() || who->GetEntry() == 18095 || who->GetEntry() == 17968)
            return;
        if(!CastTimer)
        {
            DoCast(who, SPELL_DOOMFIRE_DAMAGE, true);
            CastTimer = 500;
        }
    }

    void UpdateAI(const uint32 diff) 
    {
        if(CastTimer < diff)
            CastTimer = 0;
        else CastTimer -= diff;
    }
};

struct mob_doomfire_targettingAI : public ScriptedAI
{
    mob_doomfire_targettingAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
    }

    uint32 InitializeTimer;
    std::list<uint64> GUIDlist;

    bool IsWayPoint;
    void Reset()
    {
        InitializeTimer = 1000;

        IsWayPoint = false;

        GUIDlist.clear();
    }

    bool IsInList(uint64 guid)
    {
        if(GUIDlist.empty())
            return false;

        std::list<uint64>::iterator itr = GUIDlist.begin();
        for( ; itr != GUIDlist.end(); ++itr)
        {
            if((*itr) == guid)
                return true;
        }

        return false;
    }
    void AttackStart(Unit* who) { return; }

    void MoveInLineOfSight(Unit* who) 
    {
        if(!who || (who->GetEntry() == m_creature->GetEntry()) || who->GetEntry() == 18095 || who->GetEntry() == 17968 || IsWayPoint)
            return;

        if(m_creature->IsHostileTo(who) && who->isTargetableForAttack() && m_creature->IsWithinDistInMap(who, 5) && IsInList(who->GetGUID()))
            GUIDlist.push_back(who->GetGUID());
    }

    void UseRandomMovement()
    {
        Creature* target = DoSpawnCreature(18104, rand()%25, rand()%25, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 15000);
        if(target)
        {
            ((mob_doomfire_targettingAI*)target->AI())->IsWayPoint = true;
            m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*target));
        }
    }

    void StalkPlayer()
    {
        if(GUIDlist.empty()) return;

        std::list<uint64>::iterator itr = GUIDlist.begin();
        for( ; itr != GUIDlist.end(); ++itr)
        {
            Unit* target = Unit::GetUnit((*m_creature), (*itr));
            if(target && !m_creature->IsWithinDistInMap(target, 5))
                GUIDlist.remove(*itr);
        }
        Unit* target = Unit::GetUnit((*m_creature), (*GUIDlist.begin()));
        if(target)
            m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*target));
    }

    void UpdateAI(const uint32 diff)
    {
        if(IsWayPoint) return;

        if(InitializeTimer < diff)
        {
            if(rand()%2 == 0)
                UseRandomMovement();
            else StalkPlayer();
            InitializeTimer = 1000;
        }else InitializeTimer -= diff;
    }
};

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
                if(Archimonde && (((Archimonde->GetHealth()*100) / Archimonde->GetMaxHealth()) < 1))
                {
                    DoCast(Archimonde, SPELL_DENOUEMENT_WISP);
                }
            }
        }else CheckTimer -= diff;
    }
};

//This is used to sort the players by distance to check if anyone is in melee range or not
struct OrderByDistance : public std::binary_function<const Unit, const Unit, bool>
{
    const Unit* MainTarget;
    OrderByDistance(const Unit* Target) : MainTarget(Target) {};
    // functor for operator "<"
    bool operator()(const Unit* _Left, const Unit* _Right) const
    {
        return (MainTarget->GetDistanceSq(_Left) < MainTarget->GetDistanceSq(_Right));
    }
};

struct boss_archimondeAI : public ScriptedAI
{
    boss_archimondeAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
    }

    uint32 DrainWorldTreeTimer;
    uint32 FearTimer;
    uint32 AirBurstTimer;
    uint32 GripOfTheLegionTimer;
    uint32 MeleeRangeCheckTimer; // Checks if anyone is in melee range, if not, cast Finger of Death
    uint32 SoulChargeTimer;
    uint32 DoomfireTimer;
    uint32 HandOfDeathTimer;
    uint32 EnrageTimer;
    uint32 SoulCharge[3]; // 0 - Number of red, 1 - Number of Green, 2 - Number of Yellow

    uint32 SummonWispTimer;

    bool Enraged;
    bool TenPercent;
    bool InCombat;
    bool SoulCharged;
    bool HasProtectedWithElune;
    bool HasStartedChanneling;

    void Reset()
    {
        /* These timers may be incorrect */
        FearTimer = 15000;
        AirBurstTimer = 30000;
        GripOfTheLegionTimer = 10000;
        MeleeRangeCheckTimer = 15000;
        SoulChargeTimer = 20000;
        DoomfireTimer = 35000;
        HandOfDeathTimer = 1000;
        EnrageTimer = 180000;
        for(uint8 i = 0; i < 3; i++)
            SoulCharge[i] = 0;

        SummonWispTimer = 5000;

        Enraged = false;
        TenPercent = false;
        SoulCharged = false;
        HasProtectedWithElune = false;
        HasStartedChanneling = false;
        InCombat = false;
    }

    void AttackStart(Unit* who)
    {
        if(!who)
            return;
        
        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                Reset();
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, who);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                
                DoStartMeleeAttack(who);
                    
                if(!InCombat)
                {
                    DoYell(SAY_AGGRO, LANG_UNIVERSAL, who);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }
            }
        }
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%3)
        {
            case 0:
                DoYell(SAY_SLAY1, LANG_UNIVERSAL, victim);
                DoPlaySoundToSet(m_creature, SOUND_SLAY1);
                break;
            case 1:
                DoYell(SAY_SLAY2, LANG_UNIVERSAL, victim);
                DoPlaySoundToSet(m_creature, SOUND_SLAY2);
                break;
            case 2:
                DoYell(SAY_SLAY3, LANG_UNIVERSAL, victim);
                DoPlaySoundToSet(m_creature, SOUND_SLAY3);
                break;
        }

        if(victim->GetTypeId() != TYPEID_PLAYER) return;

        switch(((Player*)victim)->getClass())
        {
            case CLASS_PRIEST:
            case CLASS_PALADIN:
            case CLASS_WARLOCK:
                SoulCharge[0]++;
                victim->CastSpell(m_creature, SPELL_SOUL_CHARGE_RED, true);
                break;
            case CLASS_MAGE:
            case CLASS_ROGUE:
            case CLASS_WARRIOR:
                SoulCharge[2]++;
                victim->CastSpell(m_creature, SPELL_SOUL_CHARGE_YELLOW, true);
                break;
            case CLASS_DRUID:
            case CLASS_SHAMAN:
            case CLASS_HUNTER:
                SoulCharge[1]++;
                victim->CastSpell(m_creature, SPELL_SOUL_CHARGE_GREEN, true);
                break;
        }
        SoulCharged = true;
    }

    void JustDied(Unit* killer)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, killer);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
        InCombat = false;
    }

    bool UseFingerOfDeath()
    {
        /* This is how we will do this: 
        1) Get the threat list.
        2) Make a list out of everyone in the threat list (copy the threat list).
        3) Sort them by distance, closest being at the beginning.
        4) Resize it to get the first.
        5) Check if the first is farther than melee range.
        6) If 4 yields true, cast Finger of Death and set timer, otherwise just set timer. */

        std::list<HostilReference*> m_threatlist = m_creature->getThreatManager().getThreatList(); // Step 1
        if(m_threatlist.empty()) return false; // No use to continue

        std::list<Unit*> targets; 
        std::list<HostilReference*>::iterator itr = m_threatlist.begin();
        for( ; itr!=m_threatlist.end(); ++itr) // Step 2
        {
            Unit* target = NULL;
            target = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
            if(target && target->isAlive() && (target->GetTypeId() == TYPEID_PLAYER)) // Ignore pets, etc.
                targets.push_back(target);
        }

        targets.sort(OrderByDistance(m_creature)); // Step 3
        targets.resize(1); // Step 4
        Unit* target = targets.front();
        if(target && (!(m_creature->IsWithinDistInMap(target, (m_creature->GetAttackDistance(target)))))) // Step 5 and 6
            return true;
     
        return false;
    }

    void SummonDoomfire(Unit* target)
    {
        Creature* Doomfire = DoSpawnCreature(18095, rand()%10, rand()%10, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
        if(Doomfire)
        {
            Doomfire->setFaction(m_creature->getFaction());
            Doomfire->CastSpell(Doomfire, SPELL_DOOMFIRE_VISUAL, true);
            Doomfire->CastSpell(Doomfire, SPELL_DOOMFIRE_SPAWN, true);
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

    void UpdateAI(const uint32 diff)
    {
        if(!InCombat)
        {
            if(DrainWorldTreeTimer < diff)
            {
                Creature* ChannelTarget = m_creature->SummonCreature(22418, 5503.713, -3523.436, 1608.781, 0, TEMPSUMMON_TIMED_DESPAWN, 5000);
                if(ChannelTarget)
                {
                    if(!HasStartedChanneling)
                    {
                        DoCast(ChannelTarget, SPELL_DRAIN_WORLD_TREE, true);
                        HasStartedChanneling = true;
                    }
                    ChannelTarget->CastSpell(m_creature, SPELL_DRAIN_WORLD_TREE_2, true);
                    ChannelTarget->SetVisibility(VISIBILITY_OFF);
                    ChannelTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    DrainWorldTreeTimer = 500;
                }
            }else DrainWorldTreeTimer -= diff;
        }

        if(!m_creature->getVictim() || !m_creature->SelectHostilTarget())
            return;

        if((((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10)) && !TenPercent)
        {
            TenPercent = true;
            HasProtectedWithElune = false;
            HandOfDeathTimer = 5000; // A little leeway so that the raid can be buffed
        }

        if(TenPercent)
        {
            if(!HasProtectedWithElune)
            {
                DoCast(m_creature->getVictim(), SPELL_PROTECTION_OF_ELUNE, true);
                HasProtectedWithElune = true;
            }

            if(HandOfDeathTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_HAND_OF_DEATH);
                HandOfDeathTimer = 1000;
            }else HandOfDeathTimer -= diff;

            if(SummonWispTimer < diff)
            {
                Creature* Wisp = DoSpawnCreature(17946, rand()%30, rand()%30, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                if(Wisp)
                {
                    Wisp->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature));
                    ((mob_ancient_wispAI*)Wisp->AI())->ArchimondeGUID = m_creature->GetGUID();
                }
                SummonWispTimer = 1000 + rand()%3500;
            }else SummonWispTimer -= diff;

            return; // Do not do anything else
        }

        if(SoulCharged)
        {
            if(SoulChargeTimer < diff)
            {
                bool HasCast = false;
                switch(rand()%3)
                {
                    case 0: // Red
                        if(SoulCharge[0])
                        {
                            DoCast(m_creature->getVictim(), SPELL_UNLEASH_SOUL_RED);
                            SoulCharge[0]--;
                            HasCast = true;
                            if(!SoulCharge[0]) // We've decremented number of soul charges, now if it's 0, we unaura ourself
                                m_creature->RemoveAurasDueToSpell(SPELL_SOUL_CHARGE_RED);
                        }
                        break;
                    case 1: // Green
                        if(SoulCharge[1])
                        {
                            DoCast(m_creature->getVictim(), SPELL_UNLEASH_SOUL_GREEN);
                            SoulCharge[1]--;
                            HasCast = true;
                            if(!SoulCharge[1])
                                m_creature->RemoveAurasDueToSpell(SPELL_SOUL_CHARGE_GREEN);
                        }
                        break;
                    case 2:
                        if(SoulCharge[2])
                        {
                            DoCast(m_creature->getVictim(), SPELL_UNLEASH_SOUL_YELLOW);
                            SoulCharge[2]--;
                            HasCast = true;
                            if(!SoulCharge[2])
                                m_creature->RemoveAurasDueToSpell(SPELL_SOUL_CHARGE_YELLOW);
                        }
                        break;
                }
                if(HasCast)
                {
                    SoulChargeTimer = 20000;
                    SoulCharged = false;
                }
            }else SoulChargeTimer -= diff;
        }

        if(MeleeRangeCheckTimer < diff)
        {
            if(UseFingerOfDeath())
            {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_FINGER_OF_DEATH);
                MeleeRangeCheckTimer = 2000;
            }else MeleeRangeCheckTimer = 5000;
        }else MeleeRangeCheckTimer -= diff;

        if(FearTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FEAR);
            FearTimer = 40000;
        }else FearTimer -= diff;

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

            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), SPELL_AIR_BURST);
            AirBurstTimer = 15000;
        }else AirBurstTimer -= diff;

        if(DoomfireTimer < diff)
        {
            // SummonDoomfire(SelectUnit(SELECT_TARGET_RANDOM, 1));
            DoomfireTimer = 20000 + rand()%10000;
        }else DoomfireTimer -= diff;

        if(GripOfTheLegionTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_GRIP_OF_THE_LEGION);
            GripOfTheLegionTimer = 1000 + rand()%7000;
        }else GripOfTheLegionTimer -= diff;

        if(!Enraged)
        {
            if(EnrageTimer < diff)
            {
                DoYell(SAY_ENRAGE, LANG_UNIVERSAL, m_creature);
                DoPlaySoundToSet(m_creature, SOUND_ENRAGE);
                Enraged = true;
            }else EnrageTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_archimonde(Creature* _Creature)
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
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_archimonde";
    newscript->GetAI = GetAI_boss_archimonde;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_doomfire_targetting";
    newscript->GetAI = GetAI_mob_doomfire_targetting;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_doomfire";
    newscript->GetAI = GetAI_mob_doomfire;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_ancient_wisp";
    newscript->GetAI = GetAI_mob_ancient_wisp;
    m_scripts[nrscripts++] = newscript;
}
