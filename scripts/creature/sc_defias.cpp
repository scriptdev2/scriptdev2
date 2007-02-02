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

//*** Generic Defias script ***

#include "../sc_defines.h"

#define SOUND_AGGRO 1263 //Defias aggro sound

#define SAY_RANDOM_0    "The brotherhood will not tolerate your actions."
#define SAY_RANDOM_1    "Feel the power of the brotherhood. "
#define SAY_RANDOM_2    "Ah, an opportunity to test my freshly sharpened balde."
#define SAY_RANDOM_3    "So these fools still have sent us one again..."

// **** This script is still under Developement ****

#define GENERIC_CREATURE_COOLDOWN 5000

struct MANGOS_DLL_DECL defiasAI : public ScriptedAI
{
    defiasAI(Creature *c) : ScriptedAI(c) {Reset();}

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
            m_creature->RemoveAllAuras();
            DoStopAttack();
            DoGoHome();
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

            //30% chance to Play Aggro sound if the attacker is a player
            if (who->GetTypeId() == TYPEID_PLAYER && rand()%3 == 0)
            {
                DoPlaySoundToSet(m_creature,SOUND_AGGRO);

                //Switch between 4 different chats
                switch (rand()%4)
                    {
                    case 0:
                        DoSay(SAY_RANDOM_0,LANG_UNIVERSAL,pTarget);
                        break;
                    
                    case 1:
                        DoSay(SAY_RANDOM_1,LANG_UNIVERSAL,pTarget);
                        break;

                    case 2:
                        DoSay(SAY_RANDOM_2,LANG_UNIVERSAL,pTarget);
                        break;

                    case 3:
                        DoSay(SAY_RANDOM_3,LANG_UNIVERSAL,pTarget);
                        break;
                    }
            }

            pTarget = who;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            if ( m_creature->getVictim() == NULL)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                if (m_creature->IsWithinDist(who, ATTACK_DIST))
                    DoStartMeleeAttack(who);
                else DoStartRangedAttack(who);

                //30% chance to Play Aggro sound if the attacker is a player
                if (who->GetTypeId() == TYPEID_PLAYER && rand()%3 == 0)
                {
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO);

                    //Switch between 4 different chats
                    switch (rand()%4)
                        {
                        case 0:
                            DoSay(SAY_RANDOM_0,LANG_UNIVERSAL,pTarget);
                            break;
                    
                        case 1:
                            DoSay(SAY_RANDOM_1,LANG_UNIVERSAL,pTarget);
                            break;

                        case 2:
                            DoSay(SAY_RANDOM_2,LANG_UNIVERSAL,pTarget);
                            break;

                        case 3:
                            DoSay(SAY_RANDOM_3,LANG_UNIVERSAL,pTarget);
                            break;
                        }
                }

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

        //If we had a target and it wasn't cleared then it means the player died from some unknown soruce
        //But we still need to reset
        if (m_creature->isAlive() && pTarget && !m_creature->getVictim())
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
                    Unit* newtarget = m_creature->SelectHostilTarget();
                    if(newtarget)
                        AttackStart(newtarget);

                    bool Healing = false;
                    SpellEntry const *info = NULL;

                    //Select a healing spell if less than 30% hp
                    if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 30)
                        info = SelectSpell(m_creature, NULL, NULL, SELECT_TARGET_ANY_FRIEND, NULL, NULL, NULL, NULL, SELECT_EFFECT_HEALING);

                    //No healing spell available, select a hostile spell
                    if (info) Healing = true;
                        else info = SelectSpell(m_creature->getVictim(), NULL, NULL, SELECT_TARGET_ANY_ENEMY, NULL, NULL, NULL, NULL, SELECT_EFFECT_DONTCARE);

                    //20% chance to replace our white hit with a spell
                    if (info && rand() % 5  == 0 && !GlobalCooldown)
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
            //If we are still alive and we lose our victim it means we killed them
            if(m_creature->isAlive() && !m_creature->getVictim())
            {
                Reset();
            }
        }
    }
}; 


CreatureAI* GetAI_defias(Creature *_Creature)
{
    return new defiasAI (_Creature);
}


void AddSC_defias()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="defias";
    newscript->GetAI = GetAI_defias;
    m_scripts[nrscripts++] = newscript;
}