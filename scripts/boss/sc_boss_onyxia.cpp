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

// **** This script is still under Developement ****

#define SPELL_WINGBUFFET 18500
#define SPELL_FLAMEBREATH 18435
#define SPELL_CLEAVE 26350
#define SPELL_TAILSWEEP 25653
#define SPELL_FIREBALL 20692
#define SPELL_DEEPBREATH 18596
#define SPELL_BELLOWINGROAR 18431

//These spells below arn't supported in the core yet
#define SPELL_SUMMONWHELP  17646
#define SPELL_SUMMON_MULTI_WHELPS 20171

struct MANGOS_DLL_DECL boss_onyxiaAI : public ScriptedAI
{
    boss_onyxiaAI(Creature *c) : ScriptedAI(c) {}

    uint32 swingcounter;
    uint32 flamebreath_timer;
    uint32 cleave_timer;
    uint32 tailsweep_timer;
    uint32 movement_timer;
    uint32 fireball_timer;
    uint32 whelpspawn_timer;
    uint32 bellowingroar_timer;
    uint32 phase;

    Unit* pTarget;

    void ResetTimers()
    {
        DoCast(m_creature,28237);
        swingcounter = 0;
        flamebreath_timer = 20000;
        cleave_timer = 15000;
        tailsweep_timer = 10000;
        movement_timer = 10000;
        fireball_timer = 5000;
        whelpspawn_timer = 45000;
        bellowingroar_timer = 0;
        phase = 1;
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (m_creature->getVictim() == NULL && who->isTargetableForAttack() && who != m_creature)
        {
            //Begin attack
            DoStartMeleeAttack(who);

            //Initial aggro speach
            DoSay("How fortuitous. Usually, I must leave my lair to feed.",LANG_UNIVERSAL);
            ResetTimers();
            pTarget = who;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who)
            return;

        //Predefined aggro radius
        if (m_creature->GetDistanceSq(who) > 1600.0f)
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            if ( m_creature->getVictim() == NULL)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin attack
                DoStartMeleeAttack(who);

                //Initial aggro speach
                DoSay("How fortuitous. Usually, I must leave my lair to feed.",LANG_UNIVERSAL);
                ResetTimers();
                pTarget = who;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_creature->isAlive() && pTarget && !m_creature->getVictim())
        {
            pTarget = NULL;
            DoStopAttack();
            DoGoHome();
            return;
        }

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Check if we should stop attacking because our victim is no longer attackable
            if (needToStop())
            {
                m_creature->InterruptSpell();
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_STATE_STAND);
                m_creature->SetHover(false);
                (*m_creature)->Clear();
                pTarget = NULL;
                DoStopAttack();
                DoGoHome();
                return;
            }
            
            //Cast phase 1 spells in phase 1 and phase 3
            if (phase == 1 || phase == 3)
            {
                //Flamebreath every 15 seconds
                if (flamebreath_timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_FLAMEBREATH);
                    flamebreath_timer = 15000;

                }else flamebreath_timer -= diff;

                //Cleave every 20 seconds
                if (cleave_timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_CLEAVE);
                    cleave_timer = 20000;

                }else cleave_timer -= diff;

                //Tailsweep every 10 seconds
                if (tailsweep_timer < diff)
                {
                    //Only cast if we are behind
                    if (!m_creature->HasInArc( M_PI, m_creature->getVictim()))
                        DoCast(m_creature->getVictim(),SPELL_TAILSWEEP);

                    tailsweep_timer = 20000;

                }else tailsweep_timer -= diff;
            }

            //Phase 2 only spells
            if (phase == 2)
            {
                //Random movement every 10 seconds
                if (movement_timer < diff)
                {
                    //Inturrupt whatever we are casting then move to random position
                    m_creature->InterruptSpell();
                    uint32 position = rand()%8;

                    switch (position)
                    {
                        case 0:
                            m_creature->SendMonsterMove(-65.8444,-213.809,-84.2985,false,false,5000);
                            m_creature->Relocate(-65,-213,-84,0);
                            break;
                        case 1:
                            m_creature->SendMonsterMove(22.8739,-217.152,-85.0548,false,false,5000);
                            m_creature->Relocate(22,-217,-85,0);
                            break;
                        case 2:
                            m_creature->SendMonsterMove(-33.5561,-182.682,-88.9457,false,false,5000);
                            m_creature->Relocate(-33,-182,-88,0);
                            break;
                        case 3:
                            m_creature->SendMonsterMove(-31.4963,-250.123,-89.1278,false,false,5000);
                            m_creature->Relocate(-31,-250,-89,0);
                            break;
                        case 4:
                            m_creature->SendMonsterMove(-2.78999,-181.431,-86.8962,false,false,5000);
                            m_creature->Relocate(-2,-181,-86,0);
                            break;
                        case 5:
                            m_creature->SendMonsterMove(-54.9415,-232.242,-85.5555,false,false,5000);
                            m_creature->Relocate(-54,-232,-85,0);
                            break;
                        case 6:
                            m_creature->SendMonsterMove(-65.2653,-194.879,-84.6718,false,false,5000);
                            m_creature->Relocate(-65,-194,-84,0);
                            break;
                        case 7:
                            m_creature->SendMonsterMove(10.5665,-241.478,-85.9426,false,false,5000);    
                            m_creature->Relocate(10,-241,-85,0);
                            break;
                    }

                    //Random chance to cast deep breath after we arrive will go here later

                    movement_timer = 10000;
                }else movement_timer -= diff;

                //Random fireball every 5 seconds
                if (fireball_timer < diff)
                {
                    //Should be a random target but since we don't really have a hatelist just do it on current target
                    DoCast(m_creature->getVictim(),SPELL_FIREBALL);
                    fireball_timer = 5000;
                }else fireball_timer -= diff;

                //Spawn whelps every 45 seconds
                if (whelpspawn_timer < diff)
                {
                    //No core support yet so just say some text
                    DoSay("Spawning whelps!",LANG_UNIVERSAL);
                    whelpspawn_timer = 450000;
                }else whelpspawn_timer -= diff;
            }

            //Phase 3 only spells
            if (phase == 3)
            {
                //Bellowing roar every 10-30 seconds in phase 3
                if (bellowingroar_timer < diff && phase == 3)
                {
                    //Bellowing roar is bugged pretty bad right now so disable it
                    //DoCast(m_creature->getVictim(),SPELL_BELLOWINGROAR);
                    bellowingroar_timer = 10000 + rand()%20000;
                }else bellowingroar_timer -= diff;
            }
            
            //Phase 1 to Phase 2 transition at 60%
            if ( phase == 1 && (m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 60)
            {
                phase = 2;
                m_creature->InterruptSpell();
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                m_creature->SetHover(true);
                (*m_creature)->Clear();
                (*m_creature)->Idle();
                DoSay("I'll incinerate you from above!",LANG_UNIVERSAL);
            }

            //Phase 2 to Phase 3 transition at 40%
            if ( phase == 2 && (m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 40)
            {
                phase = 3;
                m_creature->InterruptSpell();
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_STATE_STAND);
                m_creature->SetHover(false);
                (*m_creature)->Clear();
                (*m_creature)->Mutate(new TargetedMovementGenerator(*m_creature->getVictim()));
                DoSay("Learn your place mortal!",LANG_UNIVERSAL);
            }

            //If we are within range melee the target and not in phase 2
            if( phase!=2 && m_creature->getVictim() && m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                if( m_creature->isAttackReady() )
                {
                    Unit* newtarget = m_creature->SelectHostilTarget();
                    if(newtarget)
                        AttackStart(newtarget);

                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();

                    //Do a wing buffet once every 5 attacks in phase 1 and 3
                    if (swingcounter > 5 && m_creature->getVictim())
                    {
                        DoCast(m_creature->getVictim(),SPELL_WINGBUFFET);
                        swingcounter = 0;
                    }else swingcounter++;
                }
            }

            //If we are still alive and we lose our victim it means we killed them
            if(m_creature->isAlive() && !m_creature->getVictim())
            {
                m_creature->InterruptSpell();
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_STATE_STAND);
                m_creature->SetHover(false);
                (*m_creature)->Clear();
                pTarget = NULL;
                DoStopAttack();
                DoGoHome();
            }
        }
    }
}; 
CreatureAI* GetAI_boss_onyxiaAI(Creature *_Creature)
{
    return new boss_onyxiaAI (_Creature);
}


void AddSC_boss_onyxiaAI()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_onyxia";
    newscript->GetAI = GetAI_boss_onyxiaAI;
    m_scripts[nrscripts++] = newscript;
}
