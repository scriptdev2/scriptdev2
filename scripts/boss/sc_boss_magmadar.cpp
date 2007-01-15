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

#define SPELL_FRENZY                28371
#define SPELL_MAGMASPIT             19449       //This is actually a buff he gives himself
#define SPELL_LAVABREATH            19272
#define SPELL_PANIC                 19408
#define SPELL_LAVABOMB              19411       //This calls a dummy server side effect that isn't implemented yet
#define SPELL_LAVABOMB_ALT          19428       //This is the spell that the lava bomb casts

struct MANGOS_DLL_DECL boss_magmadarAI : public ScriptedAI
{
    boss_magmadarAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 Frenzy_Timer;
    uint32 LavaBreath_Timer;
    uint32 Panic_Timer;
    uint32 Lavabomb_Timer;

    void Reset()
    {
        pTarget = NULL;
        Frenzy_Timer = 45000;       //Just a guess, been to long since I've killed Magmadar
        LavaBreath_Timer = 7000;
        Panic_Timer = 30000;
        Lavabomb_Timer = 12000;

        if (m_creature)
        {
            m_creature->RemoveAllAuras();
            m_creature->CastSpell(m_creature,SPELL_MAGMASPIT,true);
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
            DoStartMeleeAttack(who);

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
                DoStartMeleeAttack(who);

                pTarget = who;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //If we had a target and it wasn't cleared then it means the player died from some unknown soruce
        //But we still need to reset
        if (m_creature->isAlive() && pTarget && !m_creature->getVictim())
        {
            Reset();
            return;
        }

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Check if we should stop attacking because our victim is no longer attackable
            if (needToStop())
            {
                Reset();
                return;
            }
            
            //Frenzy_Timer
            if (Frenzy_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_FRENZY);
                DoTextEmote("goes into a killing frenzy!",NULL);

                //45 seconds
                Frenzy_Timer = 45000;
            }else Frenzy_Timer -= diff;

            //LavaBreath_Timer
            if (LavaBreath_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_LAVABREATH);

                //7 seconds until we should cast this agian
                LavaBreath_Timer = 7000;
            }else LavaBreath_Timer -= diff;

            //Panic_Timer
            if (Panic_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_PANIC);

                //30 seconds until we should cast this agian
                Panic_Timer = 30000;
            }else Panic_Timer -= diff;

            //Lavabomb_Timer
            if (Lavabomb_Timer < diff)
            {
                //Cast (normally this would be on a random player but since we don't have an aggro system we can't really do that)
                DoCast(m_creature->getVictim(),SPELL_LAVABOMB_ALT);//Casting Alt lava bomb since normal one isn't supported

                //12 seconds until we should cast this agian
                Lavabomb_Timer = 12000;
            }else Lavabomb_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    Unit* newtarget = m_creature->SelectHostilTarget();
                    if(newtarget)
                        AttackStart(newtarget);

                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
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
CreatureAI* GetAI_boss_magmadar(Creature *_Creature)
{
    return new boss_magmadarAI (_Creature);
}


void AddSC_boss_magmadar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_magmadar";
    newscript->GetAI = GetAI_boss_magmadar;
    m_scripts[nrscripts++] = newscript;
}