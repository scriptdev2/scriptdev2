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


#include "../sc_defines.h"

// **** This script is for use within every single guard to save coding time ****

#define GENERIC_CREATURE_COOLDOWN 5000

struct MANGOS_DLL_DECL guardAI : public ScriptedAI
{
    guardAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 GlobalCooldown;      //This variable acts like the global cooldown that players have (1.5 seconds)
    uint32 BuffTimer;           //This variable keeps track of buffs

    void Reset()
    {
        pTarget = NULL;
        GlobalCooldown = 0;
        BuffTimer = 0;          //Rebuff as soon as we can

        if (m_creature)
        {
            EnterEvadeMode();
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (m_creature->getVictim() == NULL && who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            if (m_creature->IsWithinDist(who, ATTACK_DIST))
                DoStartMeleeAttack(who);
            else DoStartRangedAttack(who);

            pTarget = who;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                if (m_creature->IsWithinDist(who, ATTACK_DIST))
                    DoStartMeleeAttack(who);
                else DoStartRangedAttack(who);

                pTarget = who;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Always decrease our global cooldown first
        if (GlobalCooldown > diff)
            GlobalCooldown -= diff;
        else GlobalCooldown = 0;

        //If we had a target and it wasn't cleared then it means the target died from some unknown soruce
        //But we still need to reset
        if ((!m_creature->SelectHostilTarget() || !m_creature->getVictim()) && pTarget)
        {
            Reset();
            return;
        }

        //Buff timer (only buff when we are alive and not in combat
        if (m_creature->isAlive() && !m_creature->getVictim())
            if (BuffTimer < diff )
            {
                //Find a spell that targets friendly and applies an aura (these are generally buffs)
                SpellEntry const *info = SelectSpell(m_creature, NULL, NULL, SELECT_TARGET_ANY_FRIEND, NULL, NULL, NULL, NULL, SELECT_EFFECT_AURA);

                if (info && !GlobalCooldown)
                {
                    //Cast the buff spell
                    DoCastSpell(m_creature, info);

                    //Set our global cooldown
                    GlobalCooldown = GENERIC_CREATURE_COOLDOWN;

                    //Set our timer to 10 minutes before rebuff
                    BuffTimer = 600000;
                }//Try agian in 30 seconds
                else BuffTimer = 30000;
            }else BuffTimer -= diff;


        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Check if we should stop attacking because our victim is no longer attackable or we are to far from spawn point
            if (needToStop() || CheckTether())
            {
                Reset();
                return;
            }
            
            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    bool Healing = false;
                    SpellEntry const *info = NULL;

                    //Select a healing spell if less than 30% hp
                    if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 30)
                        info = SelectSpell(m_creature, NULL, NULL, SELECT_TARGET_ANY_FRIEND, NULL, NULL, NULL, NULL, SELECT_EFFECT_HEALING);

                    //No healing spell available, select a hostile spell
                    if (info) Healing = true;
                        else info = SelectSpell(m_creature->getVictim(), NULL, NULL, SELECT_TARGET_ANY_ENEMY, NULL, NULL, NULL, NULL, SELECT_EFFECT_DONTCARE);

                    //20% chance to replace our white hit with a spell
                    if (info && rand() % 5 == 0 && !GlobalCooldown)
                    {
                        //Cast the spell
                        if (Healing)DoCastSpell(m_creature, info);
                            else DoCastSpell(m_creature->getVictim(), info);

                        //Set our global cooldown
                        GlobalCooldown = GENERIC_CREATURE_COOLDOWN;
                    }
                    else m_creature->AttackerStateUpdate(m_creature->getVictim());

                    m_creature->resetAttackTimer();
                }
            }
            else 
            {
                //Only run this code if we arn't already casting
                if (!m_creature->m_currentSpell)
                {
                    bool Healing = false;
                    SpellEntry const *info = NULL;

                    //Select a healing spell if less than 30% hp ONLY 33% of the time
                    if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 30 && rand() % 3 == 0)
                        info = SelectSpell(m_creature, NULL, NULL, SELECT_TARGET_ANY_FRIEND, NULL, NULL, NULL, NULL, SELECT_EFFECT_HEALING);

                    //No healing spell available, See if we can cast a ranged spell (Range must be greater than ATTACK_DIST)
                    if (info) Healing = true;
                        else info = SelectSpell(m_creature->getVictim(), NULL, NULL, SELECT_TARGET_ANY_ENEMY, NULL, NULL, ATTACK_DIST, NULL, SELECT_EFFECT_DONTCARE);
               
                    //Found a spell, check if we arn't on cooldown
                    if (info && !GlobalCooldown)
                    {
                        //If we are currently moving stop us and set the movement generator
                        if ((*m_creature)->top()->GetMovementGeneratorType()!=IDLE_MOTION_TYPE)
                        {
                            (*m_creature)->Clear();
                            (*m_creature)->Idle();
                        }

                        //Face target
                        DoFaceTarget(m_creature->getVictim());

                        //Cast spell
                        if (Healing) DoCastSpell(m_creature,info);
                            else DoCastSpell(m_creature->getVictim(),info);

                        //Set our global cooldown
                        GlobalCooldown = GENERIC_CREATURE_COOLDOWN;
                        

                    }//If no spells available and we arn't moving run to target
                    else if ((*m_creature)->top()->GetMovementGeneratorType()!=TARGETED_MOTION_TYPE)
                    {
                        //Cancel our current spell and then mutate new movement generator
                        m_creature->InterruptSpell();
                        (*m_creature)->Clear();
                        (*m_creature)->Mutate(new TargetedMovementGenerator(*m_creature->getVictim()));
                    }
                }
            }
        }
    }
}; 