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

/* ScriptData
SDName: Boss_Reliquary_of_Souls
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Black Temple
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../shared/WorldPacket.h"
#include "../../../../../game/TargetedMovementGenerator.h"

//Sound'n'speech
//Suffering
#define SUFF_SAY_FREED        "Pain and suffering are all that await you!"
#define SUFF_SOUND_FREED    11415

#define SUFF_SAY_AGGRO        "Don't leave me alone!"
#define SUFF_SOUND_AGGRO    11416

#define SUFF_SAY_SLAY1        "Look at what you make me do!"
#define SUFF_SOUND_SLAY1    11417

#define SUFF_SAY_SLAY2        "I didn't ask for this!"
#define SUFF_SOUND_SLAY2    11418

#define SUFF_SAY_SLAY3        "The pain is only beginning!"
#define SUFF_SOUND_SLAY3    11419

#define SUFF_SAY_RECAP        "I don't want to go back!"
#define SUFF_SOUND_RECAP    11420

#define SUFF_SAY_AFTER        "Now what do I do?"
#define SUFF_SOUND_AFTER    11421


//Desire
#define DESI_SAY_FREED        "You can have anything you desire... for a price."
#define DESI_SOUND_FREED    11408

#define DESI_SAY_SLAY1        "Fulfilment is at hand!"
#define DESI_SOUND_SLAY1    11409

#define DESI_SAY_SLAY2        "Yes... you'll stay with us now..."
#define DESI_SOUND_SLAY2    11410

#define DESI_SAY_SLAY3        "Your reach exceeds your grasp."
#define DESI_SOUND_SLAY3    11412

#define DESI_SAY_SPEC        "Be careful what you wish for..."
#define DESI_SOUND_SPEC        11411

#define DESI_SAY_RECAP        "I'll be waiting..."
#define DESI_SOUND_RECAP    11413

#define DESI_SAY_AFTER        "I won't be far..."
#define DESI_SOUND_AFTER    11414

//Anger
#define ANGER_SAY_FREED        "Beware... I live."
#define ANGER_SOUND_FREED    11399

#define ANGER_SAY_FREED2        "So... foolish."
#define ANGER_SOUND_FREED2    11400

#define ANGER_SOUND_SLAY1    11401

#define ANGER_SAY_SLAY2        "Enough. No more."
#define ANGER_SOUND_SLAY2    11402

#define ANGER_SAY_SPEC        "On your knees!"
#define ANGER_SOUND_SPEC        11403

#define ANGER_SAY_BEFORE        "Beware, coward."
#define ANGER_SOUND_BEFORE    11405

#define ANGER_SAY_DEATH        "I won't... be... ignored."
#define ANGER_SOUND_DEATH    11404



//Spells
#define AURA_OF_SUFFERING    41292
#define AURA_OF_SUFFERING_STAT     42017
#define ESSENCE_OF_SUFFERING_PASSIVE    41296
#define SPELL_ENRAGE        41305
#define SPELL_SOUL_DRAIN    41303
#define SPELL_FIXATE        41295

#define AURA_OF_DESIRE        41350
#define SPELL_RUNE_SHIELD    41431
#define SPELL_DEADEN        41410
#define SPELL_SPIRIT_SHOCK    41426

#define AURA_OF_ANGER        41337
#define SPELL_SELF_SEETHE    41364
#define SPELL_ENEMY_SEETHE    41520
#define SPELL_SOUL_SCREAM    41545
#define SPELL_SPITE        41377

#define SPELL_SOUL_RELEASE    41542
#define SPELL_RESTORE_MANA    32848
#define SPELL_RESTORE_HEALTH    25329

struct Position
{
    float x,y;
};

static Position Coords[]=
{
    {450.4, 212.3},
    {542.1, 212.3},
    {542.1, 168.3},
    {542.1, 137.4},
    {450.4, 137.4},
    {450.4, 168.3}
};

struct MANGOS_DLL_DECL npc_soul_fragmentAI : public ScriptedAI
{
    npc_soul_fragmentAI(Creature *c) : ScriptedAI(c) {SetVariables();}

    uint64 ReliquaryGUID;

    void SetVariables()
    {
        ReliquaryGUID = 0;
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage >= m_creature->GetHealth())
        {
            if(done_by->GetTypeId() == TYPEID_PLAYER)
            {
                done_by->CastSpell(done_by, SPELL_RESTORE_HEALTH, true);
                if(done_by->GetMaxPower(POWER_MANA) > 0)
                {
                    if((done_by->GetPower(POWER_MANA) / done_by->GetMaxPower(POWER_MANA)) < 70)
                    {
                        uint32 mana = done_by->GetPower(POWER_MANA) + (done_by->GetMaxPower(POWER_MANA)*0.3);
                        done_by->SetPower(POWER_MANA, mana);
                    }else done_by->SetPower(POWER_MANA, done_by->GetMaxPower(POWER_MANA));
                }
            }
            DoCast(done_by, SPELL_SOUL_RELEASE);
        }
    }

    void JustDied(Unit *killer);
};

struct MANGOS_DLL_DECL boss_reliquary_of_soulsAI : public ScriptedAI
{
    boss_reliquary_of_soulsAI(Creature *c) : ScriptedAI(c) 
    {
        if(c->GetInstanceData())
            pInstance = ((ScriptedInstance*)c->GetInstanceData());
        else pInstance = NULL;
        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint64 SufferingGUID;
    uint64 DesireGUID;
    uint64 AngerGUID;

    uint32 SpiritDeathCount;
    uint32 Phase; // 0 = Out of Combat, 1 = Not started, 2 = Suffering, 3 = Spirits, 4 = Desire, 5 = Spirits, 6 = Anger
    uint32 SummonEssenceTimer;
    uint32 DespawnEssenceTimer;
    uint32 SpiritCount;
    uint32 SummonSpiritTimer;
    uint32 AnimationTimer;

    bool InCombat;
    bool IsDead;
    bool EndingPhase;

    void SetVariables()
    {
        if(pInstance)
            pInstance->SetData("ReliquaryOfSoulsEvent", 0);

        SufferingGUID = 0;
        DesireGUID = 0;
        AngerGUID = 0;

        SpiritDeathCount = 0;
        Phase = 0;
        SummonEssenceTimer = 8000;
        DespawnEssenceTimer = 2000;
        SpiritCount = 0;
        SummonSpiritTimer = 1000;
        AnimationTimer = 8000;

        IsDead = false;
        EndingPhase = false;
        InCombat = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);
        (*m_creature).GetMotionMaster()->Clear(false);
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        return;
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(who && who->isAlive())
                    m_creature->AddThreat(who, 1.0f);

                if(!InCombat)
                {
                    if(pInstance)
                        pInstance->SetData("ReliquaryOfSoulsEvent", 1);

                    Phase = 1;
                    m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,375);  // I R ANNNGRRRY!
                    SummonEssenceTimer = 8000;
                    AnimationTimer = 5100;
                    InCombat = true;
                }
            }
        }
    }

    void SummonSpirit(uint32 SpiritCount, uint32 id)
    {
        float x = Coords[SpiritCount%7].x;
        float y = Coords[SpiritCount%7].y;
        Creature* Spirit = DoSpawnCreature(id, x, y, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
        Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if (target && Spirit)
        {
            ((npc_soul_fragmentAI*)Spirit->AI())->ReliquaryGUID = m_creature->GetGUID();
            Spirit->AddThreat(target, 1.0f);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(Phase == 0)
            return;

        if(Phase == 1)
        {
            if(AnimationTimer < diff)
            {
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);  // Release teh cube
                DoPlaySoundToSet(m_creature, SUFF_SOUND_FREED);
                AnimationTimer = 8300;
            }else AnimationTimer -= diff;

            if(SummonEssenceTimer < diff)
            {
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,373);  // Ribs: open
                Creature* EssenceSuffering = NULL;
                EssenceSuffering = m_creature->SummonCreature(23418, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 1.57, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);

                if(EssenceSuffering)
                {
                    EssenceSuffering->AddThreat(m_creature->getVictim(), 1.0f);
                    SufferingGUID = EssenceSuffering->GetGUID();
                }

                EndingPhase = false;
                Phase = 2;
            }else SummonEssenceTimer -= diff;
        }

        if(Phase == 2)
        {
            if(SufferingGUID)
            {
                Creature* EssenceSuffering = NULL;
                EssenceSuffering = ((Creature*)Unit::GetUnit((*m_creature), SufferingGUID));

                if(!EssenceSuffering || (!EssenceSuffering->isAlive()))
                    EnterEvadeMode();

                if(!EndingPhase)
                {
                    if(EssenceSuffering)
                    {
                        if(EssenceSuffering->GetHealth() < (EssenceSuffering->GetMaxHealth()*0.1))
                        {
                            EssenceSuffering->RemoveAllAuras();
                            (*EssenceSuffering).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature));
                            EssenceSuffering->Yell(SUFF_SAY_RECAP,LANG_UNIVERSAL,NULL);
                            DoPlaySoundToSet(m_creature, SUFF_SOUND_RECAP);
                            (*EssenceSuffering).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature));
                            EssenceSuffering->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            DespawnEssenceTimer = 4000;
                            AnimationTimer = 2200;
                            EndingPhase = true;
                        }
                    }
                }

                if((EndingPhase) && (EssenceSuffering) && (EssenceSuffering->isAlive()))
                {
                    if(AnimationTimer < diff)
                    {
                        EssenceSuffering->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);  // Return
                        AnimationTimer = 10000;
                    }else AnimationTimer -= diff;

                    if(DespawnEssenceTimer < diff)
                    {
                        EssenceSuffering->DeleteThreatList();
                        EssenceSuffering->Yell(SUFF_SAY_AFTER,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, SUFF_SOUND_AFTER);
                        EssenceSuffering->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        EssenceSuffering->setFaction(35);
                        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);
                        SummonEssenceTimer = 60000;
                        AnimationTimer = 58100;
                        SpiritCount = 0;
                        SummonSpiritTimer = 1000;
                        EndingPhase = false;
                        Phase = 3;
                        SufferingGUID = 0;
                    }else DespawnEssenceTimer -= diff;
                }
            }
        }

        if(Phase == 3)
        {
            if(SpiritCount < 36)
            {
                if(SummonSpiritTimer < diff)
                {
                    SummonSpirit(SpiritCount, 23398);
                    SummonSpiritTimer = 500;
                    SpiritCount++;
                }else SummonSpiritTimer -= diff;
            }

            if(SpiritDeathCount > 10)
            {
                SpiritDeathCount = 0;
                DoYell("Detected death of 10 or more spirits, signal to begin timer for anim and summon", LANG_UNIVERSAL, NULL);
            }

            if(AnimationTimer < diff)
            {
                DoPlaySoundToSet(m_creature, DESI_SOUND_FREED);
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);  // Release teh cube
                AnimationTimer = 10000;
            }else AnimationTimer -= diff;

            if(SummonEssenceTimer < diff)
            {
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,373);  // Ribs: open
                Creature* EssenceDesire = NULL;
                EssenceDesire = m_creature->SummonCreature(23419, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 1.57, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);

                if(EssenceDesire)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                    EssenceDesire->AddThreat(target, 1.0f);
                    DesireGUID = EssenceDesire->GetGUID();
                }

                Phase = 4;
            }else SummonEssenceTimer -= diff;
        }

        if(Phase == 4)
        {
            if(DesireGUID)
            {
                Creature* EssenceDesire = NULL;
                EssenceDesire = ((Creature*)Unit::GetUnit((*m_creature), DesireGUID));

                if(!EssenceDesire || !EssenceDesire->isAlive())
                    EnterEvadeMode();

                if(!EndingPhase && EssenceDesire)
                {
                    if(EssenceDesire->GetHealth() < (EssenceDesire->GetMaxHealth()*0.1))
                    {
                        EssenceDesire->RemoveAllAuras();
                        EssenceDesire->DeleteThreatList();
                        EssenceDesire->Yell(DESI_SAY_RECAP,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, DESI_SOUND_RECAP);
                        EssenceDesire->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        EssenceDesire->setFaction(35);
                        DespawnEssenceTimer = 4000;
                        AnimationTimer = 2200;
                        EndingPhase = true;
                    }
                }            

                if(EndingPhase && EssenceDesire)
                {
                    if(EssenceDesire->isAlive())
                    {
                        if(AnimationTimer < diff)
                        {
                            EssenceDesire->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);  // Return
                            AnimationTimer = 10000;
                        }else AnimationTimer -= diff;

                        if(DespawnEssenceTimer < diff)
                        {
                            EssenceDesire->Yell(DESI_SAY_AFTER, LANG_UNIVERSAL, 0);
                            DoPlaySoundToSet(m_creature, DESI_SOUND_AFTER);
                            EssenceDesire->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);
                            SummonEssenceTimer = 60000;
                            AnimationTimer = 58200;
                            SpiritCount = 0;
                            SummonSpiritTimer = 1000;
                            EndingPhase = false;
                            Phase = 5;
                            DesireGUID = 0;
                        }else DespawnEssenceTimer -= diff;
                    }
                }
            }
        }

        if(Phase == 5)
        {
            if(SpiritCount < 36)
            {
                if(SummonSpiritTimer < diff)
                {
                    SummonSpirit(SpiritCount, 23401);
                    SummonSpiritTimer = 500;
                    SpiritCount++;
                }else SummonSpiritTimer -= diff;
            }

            if(AnimationTimer < diff)
            {
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,374);  // Release teh cube
                AnimationTimer = 10000;
            }else AnimationTimer -= diff;

            if(SummonEssenceTimer < diff)
            {
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,373);  // Ribs: open
                Creature* EssenceAnger = NULL;
                EssenceAnger = m_creature->SummonCreature(23420, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 1.57, TEMPSUMMON_DEAD_DESPAWN, 0);

                if(EssenceAnger)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                    EssenceAnger->AddThreat(target, 1.0f);
                    AngerGUID = EssenceAnger->GetGUID();
                    DoPlaySoundToSet(m_creature, ANGER_SOUND_FREED);
                    EssenceAnger->Yell(ANGER_SAY_FREED, LANG_UNIVERSAL, 0);
                }

                Phase = 6;
            }else SummonEssenceTimer -= diff;
        }

        if(Phase == 6)
        {
            if(AngerGUID)
            {
                Creature* EssenceAnger = NULL;
                EssenceAnger = ((Creature*)Unit::GetUnit((*m_creature), AngerGUID));

                if(!EssenceAnger)
                    EnterEvadeMode();

                if(m_creature->isAlive() && EssenceAnger)
                {
                    if(!EssenceAnger->isAlive())
                    {
                        AngerGUID = 0;
                        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, 383);
                        m_creature->SetHealth(0);
                    }
                }
            }
        }
    }
};

struct MANGOS_DLL_DECL boss_essence_of_sufferingAI : public ScriptedAI
{
    boss_essence_of_sufferingAI(Creature *c) : ScriptedAI(c) {SetVariables();}

    uint64 StatAuraGUID;

    uint32 FixateTimer;
    uint32 EnrageTimer;
    uint32 SoulDrainTimer;

    bool InCombat;

    void SetVariables()
    {
        StatAuraGUID = 0;

        FixateTimer = 5000;
        EnrageTimer = 30000;
        SoulDrainTimer = 150000;

        InCombat = false;
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if((damage >= m_creature->GetHealth()) && (done_by != m_creature))
        {
            damage = 0;
            m_creature->SetHealth(m_creature->GetMaxHealth()*0.1); // 10% of total health, signalling time to return
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!StatAuraGUID || (StatAuraGUID != who->GetGUID()))
            {
                StatAuraGUID = who->GetGUID();
                who->CastSpell(who, AURA_OF_SUFFERING_STAT, true, 0, 0, m_creature->GetGUID());
            }

            if (!InCombat)
            {
                DoCast(who, AURA_OF_SUFFERING, true);
                DoCast(m_creature, ESSENCE_OF_SUFFERING_PASSIVE, true);
                DoYell(SUFF_SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SUFF_SOUND_AGGRO);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    DoCast(who, AURA_OF_SUFFERING, true);
                    DoCast(m_creature, ESSENCE_OF_SUFFERING_PASSIVE);
                    DoYell(SUFF_SAY_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SUFF_SOUND_AGGRO);
                    InCombat = true;
                }
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
        case 0:
            DoYell(SUFF_SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SUFF_SOUND_SLAY1);
            break;
        case 1:
            DoYell(SUFF_SAY_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SUFF_SOUND_SLAY2);
            break;
        case 2:
            DoYell(SUFF_SAY_SLAY3,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SUFF_SOUND_SLAY3);
            break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Supposed to be cast on nearest target
            /*if(FixateTimer < diff)
            {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
            m_creature->AddThreat(target, 5000000.0f);
            FixateTimer = 5000;
            }else FixateTimer -= diff;*/

            if(EnrageTimer < diff)
            {
                DoCast(m_creature, SPELL_ENRAGE);
                EnrageTimer = 60000;
            }else EnrageTimer -= diff;

            if(SoulDrainTimer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                if(target)
                    DoCast(target, SPELL_SOUL_DRAIN);
                SoulDrainTimer = 60000;
            }else SoulDrainTimer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};
struct MANGOS_DLL_DECL boss_essence_of_desireAI : public ScriptedAI
{
    boss_essence_of_desireAI(Creature *c) : ScriptedAI(c) {SetVariables();}

    uint32 RuneShieldTimer;
    uint32 DeadenTimer;
    uint32 SpiritShockTimer;

    bool InCombat;

    void SetVariables()
    {
        RuneShieldTimer = 60000;
        DeadenTimer = 15000;
        SpiritShockTimer = 40000;
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if((damage >= m_creature->GetHealth()) && (done_by != m_creature))
        {
            damage = 0;
            m_creature->SetHealth(m_creature->GetMaxHealth()*0.1); // 10% of total health, signalling time to return
        }
        else
        {
            m_creature->DealDamage(done_by, damage*0.5, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                DoCast(who, AURA_OF_DESIRE);
                InCombat = true;
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
        case 0:
            DoYell(DESI_SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, DESI_SOUND_SLAY1);
            break;
        case 1:
            DoYell(DESI_SAY_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, DESI_SOUND_SLAY2);
            break;
        case 2:
            DoYell(DESI_SAY_SLAY3,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, DESI_SOUND_SLAY3);
            break;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    DoCast(who, AURA_OF_DESIRE);
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(RuneShieldTimer < diff)
        {
            DoCast(m_creature, SPELL_RUNE_SHIELD);
            RuneShieldTimer = 60000;
        }else RuneShieldTimer -= diff;

        if(DeadenTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_DEADEN);
            DeadenTimer = 30000 + rand()%30001;
        }else DeadenTimer -= diff;

        if(SpiritShockTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SPIRIT_SHOCK);
            SpiritShockTimer = 40000;
            switch(rand()%2)
            {
            case 0:
                DoYell(DESI_SAY_SPEC,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, DESI_SOUND_SPEC);
                break;
            }

        }else SpiritShockTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_essence_of_angerAI : public ScriptedAI
{
    boss_essence_of_angerAI(Creature *c) : ScriptedAI(c) {SetVariables();}

    uint64 AggroTargetGUID;

    uint32 CheckTankTimer;
    uint32 SoulScreamTimer;
    uint32 SpiteTimer;

    bool CheckedAggro;
    bool InCombat;

    void SetVariables()
    {
        AggroTargetGUID = 0;

        CheckTankTimer = 5000;
        SoulScreamTimer = 10000;
        SpiteTimer = 30000;

        CheckedAggro = false;
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }


    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                DoYell(ANGER_SAY_FREED2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, ANGER_SOUND_FREED2);
                DoCast(m_creature->getVictim(), AURA_OF_ANGER);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    DoCast(who, AURA_OF_ANGER);
                    DoYell(ANGER_SAY_FREED2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, ANGER_SOUND_FREED2);
                    InCombat = true;
                }
            }
        }
    }

    void JustDied(Unit *victim)
    {
        SetVariables();
        DoYell(ANGER_SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,ANGER_SOUND_DEATH);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
        case 0:
            DoPlaySoundToSet(m_creature, ANGER_SOUND_SLAY1);
            break;
        case 1:
            DoYell(ANGER_SAY_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, ANGER_SOUND_SLAY2);
            break;
        }
    }


    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!CheckedAggro)
        {
            AggroTargetGUID = m_creature->getVictim()->GetGUID();
            CheckedAggro = true;
        }

        if(CheckTankTimer < diff)
        {
            if(m_creature->getVictim()->GetGUID() != AggroTargetGUID)
            {     
                DoYell(ANGER_SAY_BEFORE,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, ANGER_SOUND_BEFORE);
                DoCast(m_creature, SPELL_SELF_SEETHE);
                m_creature->CastSpell(m_creature->getVictim(), SPELL_ENEMY_SEETHE, true);
                AggroTargetGUID = m_creature->getVictim()->GetGUID();
            }
            CheckTankTimer = 2000;
        }else CheckTankTimer -= diff;

        if(SoulScreamTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SOUL_SCREAM);
            SoulScreamTimer = 10000;
        }else SoulScreamTimer -= diff;

        if(SpiteTimer < diff)
        {
            for(uint8 i = 0; i < 4; i++)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                if(target)
                    DoCast(target, SPELL_SPITE);

            }

            SpiteTimer = 30000;
            DoYell(ANGER_SAY_SPEC,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, ANGER_SOUND_SPEC);
        }else SpiteTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

void npc_soul_fragmentAI::JustDied(Unit *killer)
{
    if(ReliquaryGUID)
    {
        Creature* Reliquary = ((Creature*)Unit::GetUnit((*m_creature), ReliquaryGUID));
        if(Reliquary)
        {
            DoYell("Reliquary located, incrementing death count", LANG_UNIVERSAL, NULL);
            ((boss_reliquary_of_soulsAI*)Reliquary->AI())->SpiritDeathCount++;
        }
    }
}

CreatureAI* GetAI_boss_reliquary_of_souls(Creature *_Creature)
{
    return new boss_reliquary_of_soulsAI (_Creature);
}

CreatureAI* GetAI_boss_essence_of_suffering(Creature *_Creature)
{
    return new boss_essence_of_sufferingAI (_Creature);
}

CreatureAI* GetAI_boss_essence_of_desire(Creature *_Creature)
{
    return new boss_essence_of_desireAI (_Creature);
}

CreatureAI* GetAI_boss_essence_of_anger(Creature *_Creature)
{
    return new boss_essence_of_angerAI (_Creature);
}

CreatureAI* GetAI_npc_soul_fragment(Creature *_Creature)
{
    return new npc_soul_fragmentAI (_Creature);
}

void AddSC_boss_reliquary_of_souls()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_reliquary_of_souls";
    newscript->GetAI = GetAI_boss_reliquary_of_souls;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_essence_of_suffering";
    newscript->GetAI = GetAI_boss_essence_of_suffering;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_essence_of_desire";
    newscript->GetAI = GetAI_boss_essence_of_desire;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_essence_of_anger";
    newscript->GetAI = GetAI_boss_essence_of_anger;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_soul_fragment";
    newscript->GetAI = GetAI_npc_soul_fragment;
    m_scripts[nrscripts++] = newscript;

}
