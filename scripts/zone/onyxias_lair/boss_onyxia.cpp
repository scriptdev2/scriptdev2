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


#include "../../sc_defines.h"
#include "../../../../../game/TargetedMovementGenerator.h"
#include "../../../../../shared/WorldPacket.h"

#define SPELL_WINGBUFFET 18500
#define SPELL_FLAMEBREATH 18435
#define SPELL_CLEAVE 26350
#define SPELL_TAILSWEEP 25653
#define SPELL_ENGULFINGFLAMES 20019                  
#define SPELL_DEEPBREATH 18596
#define SPELL_BELLOWINGROAR 18431

//These spells below arn't supported in the core yet
#define SPELL_SUMMONWHELP  17646
#define SPELL_SUMMON_MULTI_WHELPS 20171

#define SAY_AGGRO           "How fortuitous. Usually, I must leave my lair to feed."
#define SAY_KILL            "Learn your place mortal!"
#define SAY_PHASE_2_TRANS   "I'll incinerate you from above!"
#define SAY_PHASE_3_TRANS   "It seems you'll need another lesson!"


#define ADD_1X -30.127
#define ADD_1Y -254.463
#define ADD_1Z -89.440

#define ADD_2X -30.817
#define ADD_2Y -177.106
#define ADD_2Z -89.258

#define ADD_3X 14.480
#define ADD_3Y -241.560
#define ADD_3Z -85.630

#define ADD_4X 17.372
#define ADD_4Y -190.840
#define ADD_4Z -85.281

struct MANGOS_DLL_DECL boss_onyxiaAI : public ScriptedAI
{
    boss_onyxiaAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

       uint32 swingcounter;
    uint32 flamebreath_timer;
    uint32 cleave_timer;
    uint32 tailsweep_timer;
    uint32 movement_timer;
    uint32 engulfingflames_timer;
    uint32 whelpspawn_timer;
    uint32 bellowingroar_timer;
    uint32 reset_timer;
    uint32 phase;
    uint32 counter;
    Creature* Summoned;
    
    
    void EnterEvadeMode()
    {
                
        swingcounter = 0;
        flamebreath_timer = 20000;
        cleave_timer = 15000;
        tailsweep_timer = 3000;
        movement_timer = 10000;
        engulfingflames_timer = 15000;
        whelpspawn_timer = 45000;
        bellowingroar_timer = 0;
        phase = 1;
        counter = 0;        


//        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 3);
        m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
        m_creature->SetHover(false);
        (*m_creature).GetMotionMaster()->Clear(false);
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        //m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, SPELL_AURA_MOD_TAUNT, true); not working
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FIRE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
    }

    void KilledUnit(Unit* victim)
    {
        if (rand()%5)
            return;

        DoYell(SAY_KILL,LANG_UNIVERSAL,NULL);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            //Begin attack
//            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 1);
            DoStartMeleeAttack(who);
            
            if (counter == 0)
            {            
                //Initial aggro speach
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                counter = 1;
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
                    
                //Begin attack
//                m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 1);
                DoStartMeleeAttack(who);

                if (counter == 0)
                {            
                    //Initial aggro speach
                    DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                    counter = 1;
                }
             }
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
            //Cast phase 1 spells in phase 1 and phase 3
            if (phase == 1 || phase == 3)
            {
                //Flamebreath every 15 seconds
                if (flamebreath_timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_FLAMEBREATH);
                    flamebreath_timer = 15000;
                }else flamebreath_timer -= diff;

                //Cleave every 7 seconds
                if (cleave_timer < diff)
                {    
                    DoCast(m_creature->getVictim(),SPELL_CLEAVE);
                    cleave_timer = 7000;
                }else cleave_timer -= diff;

                //Tailsweep every 2 seconds
                if (tailsweep_timer < diff)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);                    
                    //Only cast if we are behind
                    if (!m_creature->HasInArc( M_PI, target))
                        DoCast(target,SPELL_TAILSWEEP);
                    tailsweep_timer = 2000;

                }else tailsweep_timer -= diff;
            }
                
                
            //Phase 2 only spells
            if (phase == 2)
            {
                //Replay the take off animation if we aren't hovering
                if (!m_creature->isHover())
                {
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                    m_creature->SetHover(true);
                }

                //Random movement every 25 seconds
                if (movement_timer < diff)
                {
                    //Inturrupt whatever we are casting then move to random position
                    m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                    uint32 position = rand()%9;

                    switch (position)
                    {
                        case 0:
                            SpecialMove(-65.8444,-213.809,-60.2985,5000);
                            m_creature->Relocate(-65,-213,-60,0);
                            break;
                        case 1:
                            SpecialMove(22.8739,-217.152,-60.0548,5000);
                            m_creature->Relocate(22,-217,-60,0);
                            break;
                        case 2:
                            SpecialMove(-33.5561,-182.682,-60.9457,5000);
                            m_creature->Relocate(-33,-182,-60,0);
                            break;
                        case 3:
                            SpecialMove(-31.4963,-250.123,-60.1278,5000);
                            m_creature->Relocate(-31,-250,-60,0);
                            break;
                        case 4:
                            SpecialMove(-2.78999,-181.431,-60.8962,5000);
                            m_creature->Relocate(-2,-181,-60,0);
                            break;
                        case 5:
                            SpecialMove(-54.9415,-232.242,-60.5555,5000);
                            m_creature->Relocate(-54,-232,-60,0);
                            break;
                        case 6:
                            SpecialMove(-65.2653,-194.879,-60.6718,5000);
                            m_creature->Relocate(-65,-194,-60,0);
                            break;
                        case 7:
                            SpecialMove(10.5665,-241.478,-60.9426,5000);    
                            m_creature->Relocate(10,-241,-60,0);
                            break;
                        case 8:
                            //1 in 9 chance that we cast deepbreath instead of moving
                               if ( rand() % 10 <= 4 )
                            {
                                 DoTextEmote("takes a deep breath...",NULL);
                                 DoCast(m_creature->getVictim(),SPELL_DEEPBREATH);
                               }
                             break;
                    }

                    DoCast(m_creature,11010);//hover?
                    movement_timer = 25000;
                }else movement_timer -= diff;

                //Random Engulfing Flames every 8 seconds
                if (engulfingflames_timer < diff)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if ( rand() % 10 <= 4 )
                        DoCast(target,SPELL_ENGULFINGFLAMES);
                    engulfingflames_timer = 8000;
                }else engulfingflames_timer -= diff;

                //Spawn 6 whelps every 45 seconds
                if (whelpspawn_timer < diff)
                {
                    //No core support yet so just say some text
                    DoYell("Come my children!!",LANG_UNIVERSAL,NULL);

                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);

                    Summoned = m_creature->SummonCreature(11262,ADD_1X,ADD_1Y,ADD_1Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_1X,ADD_1Y,ADD_1Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_2X,ADD_2Y,ADD_2Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_2X,ADD_2Y,ADD_2Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_3X,ADD_3Y,ADD_3Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_3X,ADD_3Y,ADD_3Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_3X,ADD_3Y,ADD_3Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_3X,ADD_3Y,ADD_3Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_4X,ADD_4Y,ADD_4Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_4X,ADD_4Y,ADD_4Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_4X,ADD_4Y,ADD_4Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    Summoned = m_creature->SummonCreature(11262,ADD_4X,ADD_4Y,ADD_4Z,0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);


                    whelpspawn_timer = 45000;
                }else whelpspawn_timer -= diff;
            }


           //Phase 3 only spells
            if (phase == 3)
            {
                //Bellowing roar every 20-30 seconds in phase 3
                if (bellowingroar_timer < diff && phase == 3)
                {

                    DoCast(m_creature->getVictim(),SPELL_BELLOWINGROAR);
                    bellowingroar_timer = 20000 + rand()%10000;
                }else bellowingroar_timer -= diff;
            }
            
            //Phase 1 to Phase 2 transition at 60%
            if ( phase == 1 && (m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 61)
            {
                phase = 2;
                m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                (*m_creature).GetMotionMaster()->Clear(false);
                (*m_creature).GetMotionMaster()->Idle();
                DoCast(m_creature,11010);//hover?
                m_creature->SetHover(true);
                DoCast(m_creature,18430);//Dragon hover?
                DoYell(SAY_PHASE_2_TRANS,LANG_UNIVERSAL,NULL);
                SpecialMove(-65.8444,-213.809,-60.2985,5000);
                m_creature->Relocate(-65,-213,-60,0);
           }

            //Phase 2 to Phase 3 transition at 40%
            if ( phase == 2 && (m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 41)
            {
                phase = 3;
                m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                 SpecialMove(-65.8444,-213.809,-85.2985,5000);
                m_creature->Relocate(-65,-213,-85,0);
                //m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_STATE_STAND);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                m_creature->SetHover(false);
                (*m_creature).GetMotionMaster()->Clear(false);
                (*m_creature).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim()));
                DoYell(SAY_PHASE_3_TRANS,LANG_UNIVERSAL,NULL);
            }

            //If we are within range melee the target and not in phase 2
            if( phase!=2 && m_creature->getVictim() && m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                if( m_creature->isAttackReady() )
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();

                    //Do a wing buffet once every 12 attacks in phase 1 and 3
                    if (swingcounter > 12 && m_creature->getVictim())
                    {
                        DoCast(m_creature->getVictim(),SPELL_WINGBUFFET);
                        swingcounter = 0;
                    }else swingcounter++;
                }
            }
        }
    }

    void SpecialMove(float X, float Y, float Z, uint32 Time)
    {
        WorldPacket data( SMSG_MONSTER_MOVE, (41+m_creature->GetPackGUID().size()) );
        data.append(m_creature->GetPackGUID());

        data << m_creature->GetPositionX() << m_creature->GetPositionY() << m_creature->GetPositionZ();
        // unknown field - unrelated to orientation
        // seems to increment about 1000 for every 1.7 seconds
        // for now, we'll just use mstime
        data << getMSTime();

        data << uint8(0);                                // walkback when walking from A to B
        data << uint32(0x0200);          // flags
        /* Flags:
        512: Floating, moving without walking/running
        */
        data << Time;                                           // Time in between points
        data << uint32(1);                                      // 1 single waypoint
        data << X << Y << Z;                  // the single waypoint Point B
        m_creature->SendMessageToSet( &data, true );
    }
};
CreatureAI* GetAI_boss_onyxiaAI(Creature *_Creature)
{
    return new boss_onyxiaAI (_Creature);
}


void AddSC_boss_onyxia()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_onyxia";
    newscript->GetAI = GetAI_boss_onyxiaAI;
    m_scripts[nrscripts++] = newscript;
}
