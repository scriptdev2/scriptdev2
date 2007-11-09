/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#define SWITCH_RADIUS             15

#define MODEL_CORRUPT             5498
#define MODEL_CLEAN               20162

#define SPELL_WATER_TOMB          38235
#define SPELL_MARK_OF_HYDROSS1    38215
#define SPELL_MARK_OF_HYDROSS2    38216
#define SPELL_MARK_OF_HYDROSS3    38217
#define SPELL_MARK_OF_HYDROSS4    38218
#define SPELL_MARK_OF_HYDROSS5    38231
#define SPELL_MARK_OF_HYDROSS6    40584
#define SPELL_MARK_OF_CORRUPTION1 38219
#define SPELL_MARK_OF_CORRUPTION2 38220
#define SPELL_MARK_OF_CORRUPTION3 38221
#define SPELL_MARK_OF_CORRUPTION4 38222
#define SPELL_MARK_OF_CORRUPTION5 38230
#define SPELL_MARK_OF_CORRUPTION6 40583
#define SPELL_VILE_SLUDGE         38246

#define PURE_SPAWNS_OF_HYDROSS    22035
#define TAINTED_SPAWN_OF_HYDROSS  22036

#define SAY_AGGRO                 "I cannot allow you to interfere!"
#define SAY_SWITCH_TO_CLEAN       "Better, much better."
#define SAY_CLEAN_SLAY1           "They have forced me to this..."
#define SAY_CLEAN_SLAY2           "I have no choice."
#define SAY_CLEAN_DEATH           "I am... released..."
#define SAY_SWITCH_TO_CORRUPT     "Aaghh, the poison..."
#define SAY_CORRUPT_SLAY1         "I will purge you from this place."
#define SAY_CORRUPT_SLAY2         "You are no better than they!"
#define SAY_CORRUPT_DEATH         "You are the disease, not I"

#define SOUND_AGGRO               11289
#define SOUND_SWITCH_TO_CLEAN     11290
#define SOUND_CLEAN_SLAY1         11291
#define SOUND_CLEAN_SLAY2         11292
#define SOUND_CLEAN_DEATH         11293
#define SOUND_SWITCH_TO_CORRUPT   11297
#define SOUND_CORRUPT_SLAY1       11298
#define SOUND_CORRUPT_SLAY2       11299
#define SOUND_CORRUPT_DEATH       11300

#define ADDS_CLEAN                22035
#define ADDS_CORRUPT              22036

#define SPAWN_X_DIFF1             6.934003
#define SPAWN_Y_DIFF1             -11.255012
#define SPAWN_X_DIFF2             -6.934003
#define SPAWN_Y_DIFF2             11.255012
#define SPAWN_X_DIFF3             -12.577011
#define SPAWN_Y_DIFF3             -4.72702
#define SPAWN_X_DIFF4             12.577011
#define SPAWN_Y_DIFF4             4.72702

struct MANGOS_DLL_DECL boss_hydross_the_unstableAI : public ScriptedAI
{
    boss_hydross_the_unstableAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Invisible = NULL;
        EnterEvadeMode();
    }

    ScriptedInstance* pInstance; 

    uint32 PosCheck_Timer;
    uint32 MarkOfHydross_Timer;
    uint32 MarkOfCorruption_Timer;
    uint32 WaterTomb_Timer;
    uint32 VileSludge_Timer;
    uint32 Invisible_Timer;
    uint32 MarkOfHydross_Count;
    uint32 MarkOfCorruption_Count;

    bool InCombat;
    bool CorruptedForm;
    bool HasSpawnedInvisible;

    Creature *Invisible;

    void EnterEvadeMode()
    {
        PosCheck_Timer = 5000;
        MarkOfHydross_Timer = 20000;
        MarkOfCorruption_Timer = 20000;
        WaterTomb_Timer = 7000;
        VileSludge_Timer = 15000;
        Invisible_Timer = 2000;
        MarkOfHydross_Count = 0;
        MarkOfCorruption_Count = 0;

        // despawn invisible trigger
        DespawnCreatureIfExists(Invisible);

        InCombat = false;
        CorruptedForm = false;
        HasSpawnedInvisible = false;

        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_CLEAN);

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        if(pInstance)
            pInstance->SetData("HydrossTheUnstableEvent", 0); // 0 = NOT_STARTED

        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FROST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_NATURE, false); // disabled
    }

    void StartEvent()
    {
        DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_AGGRO);

        if(pInstance)
            pInstance->SetData("HydrossTheUnstableEvent", 1); // 1 = IN_PROGRESS

        InCombat = true;
    }

    void KilledUnit(Unit *victim)
    {
        if(CorruptedForm)
            switch(rand()%2)
            {
                case 0:
                DoYell(SAY_CORRUPT_SLAY1, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_CORRUPT_SLAY1);
                break;

                case 1:
                DoYell(SAY_CORRUPT_SLAY2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_CORRUPT_SLAY2);
                break;
            }
        else
            switch(rand()%2)
            {
                case 0:
                DoYell(SAY_CLEAN_SLAY1, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_CLEAN_SLAY1);
                break;

                case 1:
                DoYell(SAY_CLEAN_SLAY2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_CLEAN_SLAY2);
                break;
            }
    }

    void JustDied(Unit *victim)
    {
        if(CorruptedForm)
        {
            DoYell(SAY_CORRUPT_DEATH, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_CORRUPT_DEATH);
        }
        else
        {
            DoYell(SAY_CLEAN_DEATH, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_CLEAN_DEATH);
        }

        if(pInstance)
            pInstance->SetData("HydrossTheUnstableEvent", 3); // 3 = DONE

        // despawn invisible trigger
        DespawnCreatureIfExists(Invisible);
    }

    void AttackStart(Unit *who)
    {
        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
            if(!InCombat)
                StartEvent();
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

                DoStartMeleeAttack(who);
                if(!InCombat)
                    StartEvent();
            }
        }
    }

    void DespawnCreatureIfExists(Creature *pUnit)
    {
        if(pUnit && pUnit->isAlive())
            pUnit->DealDamage(pUnit, pUnit->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
    }

    void UpdateAI(const uint32 diff)
    {
        // Invisible trigger to track his original position
        if(!HasSpawnedInvisible)
            if(Invisible_Timer < diff)
            {
                Invisible = m_creature->SummonCreature(12999, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);

                if(Invisible)
                {
                    //Root self
                    Invisible->CastSpell(Invisible, 33356, true);

                    //Make super invis
                    Invisible->CastSpell(Invisible, 8149, true);

                }else DoYell("Unable to spawn invisible trigger", LANG_UNIVERSAL, NULL);

                HasSpawnedInvisible = true;
            }else Invisible_Timer -= diff;

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        // corrupted form
        if(CorruptedForm)
        {
            //MarkOfCorruption_Timer
            if(MarkOfCorruption_Timer < diff)
            {
                if(MarkOfCorruption_Count <= 4)
                {
                    uint32 mark_spell;

                    switch(MarkOfCorruption_Count)
                    {
                        case 0:
                        mark_spell = SPELL_MARK_OF_CORRUPTION1;
                        break;

                        case 1:
                        mark_spell = SPELL_MARK_OF_CORRUPTION2;
                        break;

                        case 2:
                        mark_spell = SPELL_MARK_OF_CORRUPTION3;
                        break;

                        case 3:
                        mark_spell = SPELL_MARK_OF_CORRUPTION4;
                        break;

                        case 4:
                        mark_spell = SPELL_MARK_OF_CORRUPTION5;
                        break;
                    }

                    DoCast(m_creature->getVictim(), mark_spell);
                    MarkOfCorruption_Count++;
                }

                MarkOfCorruption_Timer = 20000;
            }else MarkOfCorruption_Timer -= diff;

            //VileSludge_Timer
            if(VileSludge_Timer < diff)
            {
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target)
                    DoCast(target, SPELL_VILE_SLUDGE);

                VileSludge_Timer = 15000;
            }else VileSludge_Timer -= diff;

            //PosCheck_Timer
            if(PosCheck_Timer < diff)
            {
                if(Invisible && m_creature->IsWithinDistInMap(Invisible, SWITCH_RADIUS))
                {
                    // switch to clean form
                    m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_CLEAN);
                    CorruptedForm = false;
                    MarkOfHydross_Count = 0;

                    DoYell(SAY_SWITCH_TO_CLEAN, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SWITCH_TO_CLEAN);

                    // spawn 4 adds
                    Creature *Adds;
                    Adds = DoSpawnCreature(ADDS_CLEAN, SPAWN_X_DIFF1, SPAWN_Y_DIFF1, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);
                    Adds = DoSpawnCreature(ADDS_CLEAN, SPAWN_X_DIFF2, SPAWN_Y_DIFF2, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);
                    Adds = DoSpawnCreature(ADDS_CLEAN, SPAWN_X_DIFF3, SPAWN_Y_DIFF3, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);
                    Adds = DoSpawnCreature(ADDS_CLEAN, SPAWN_X_DIFF4, SPAWN_Y_DIFF4, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);

                    m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FROST, true);
                    m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_NATURE, false); // disabled
                }

                PosCheck_Timer = 5000;
            }else PosCheck_Timer -=diff;
        }
        // clean form
        else
        {
            //MarkOfHydross_Timer
            if(MarkOfHydross_Timer < diff)
            {
                if(MarkOfHydross_Count <= 4)
                {
                    uint32 mark_spell;

                    switch(MarkOfHydross_Count)
                    {
                        case 0:
                        mark_spell = SPELL_MARK_OF_HYDROSS1;
                        break;

                        case 1:
                        mark_spell = SPELL_MARK_OF_HYDROSS2;
                        break;

                        case 2:
                        mark_spell = SPELL_MARK_OF_HYDROSS3;
                        break;

                        case 3:
                        mark_spell = SPELL_MARK_OF_HYDROSS4;
                        break;

                        case 4:
                        mark_spell = SPELL_MARK_OF_HYDROSS5;
                        break;
                    }

                    DoCast(m_creature->getVictim(), mark_spell);
                    MarkOfHydross_Count++;
                }

                MarkOfHydross_Timer = 20000;
            }else MarkOfHydross_Timer -= diff;

            //WaterTomb_Timer
            if(WaterTomb_Timer < diff)
            {
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target)
                    DoCast(target, SPELL_WATER_TOMB);

                WaterTomb_Timer = 7000;
            }else WaterTomb_Timer -= diff;

            //PosCheck_Timer
            if(PosCheck_Timer < diff)
            {
                if(Invisible && !m_creature->IsWithinDistInMap(Invisible, SWITCH_RADIUS))
                {
                    // switch to corrupted form
                    m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_CORRUPT);
                    MarkOfCorruption_Count = 0;
                    CorruptedForm = true;

                    DoYell(SAY_SWITCH_TO_CORRUPT, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SWITCH_TO_CORRUPT);

                    // spawn 4 adds
                    Creature *Adds;
                    Adds = DoSpawnCreature(ADDS_CORRUPT, SPAWN_X_DIFF1, SPAWN_Y_DIFF1, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);
                    Adds = DoSpawnCreature(ADDS_CORRUPT, SPAWN_X_DIFF2, SPAWN_Y_DIFF2, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);
                    Adds = DoSpawnCreature(ADDS_CORRUPT, SPAWN_X_DIFF3, SPAWN_Y_DIFF3, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);
                    Adds = DoSpawnCreature(ADDS_CORRUPT, SPAWN_X_DIFF4, SPAWN_Y_DIFF4, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);

                    m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FROST, false); // disabled
                    m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_NATURE, true);
                }

                PosCheck_Timer = 5000;
            }else PosCheck_Timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_hydross_the_unstable(Creature *_Creature)
{
    return new boss_hydross_the_unstableAI (_Creature);
}

void AddSC_boss_hydross_the_unstable()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_hydross_the_unstable";
    newscript->GetAI = GetAI_boss_hydross_the_unstable;
    m_scripts[nrscripts++] = newscript;
}
