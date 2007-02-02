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

// **** This script is still under Developement ****
// Adds NYI and the intro dialog is NYI

#define SPELL_HANDOFRAGNAROS        19780
#define SPELL_WRATHOFRAGNAROS       20566
#define SPELL_LAVABURST             21158

#define SPELL_MAGMABURST            20565       //Ranged attack

#define SAY_AGGRO       "NOW FOR YOU, INSECTS. BOLDLY YOU SAUGHT THE POWER OF RAGNAROS NOW YOU SHALL SEE IT FIRST HAND."
#define SAY_HAND        "BY FIRE BE PURGED!"
#define SAY_WRATH       "TASTE THE FLAMES OF SULFURON!"


#define SOUND_AGGRO     8045
#define SOUND_HAND      8046
#define SOUND_WRATH     8047

struct MANGOS_DLL_DECL boss_ragnarosAI : public ScriptedAI
{
    boss_ragnarosAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 WrathOfRagnaros_Timer;
    uint32 HandOfRagnaros_Timer;
    uint32 LavaBurst_Timer;

    void Reset()
    {
        pTarget = NULL;
        WrathOfRagnaros_Timer = 30000;      //Damage reflection first so we alternate
        HandOfRagnaros_Timer = 25000;
        LavaBurst_Timer = 10000;

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
            //Begin ranged attack because Ragnaros is rooted at all times
            DoStartRangedAttack(who);

            //Say our dialog
            DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_AGGRO);

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

                //Begin ranged attack because Ragnaros is rooted at all times
                DoStartRangedAttack(who);

                //Say our dialog
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGRO);

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

            //WrathOfRagnaros_Timer
            if (WrathOfRagnaros_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_WRATHOFRAGNAROS);
                
                if (rand()%2 == 0)
                {
                    //Say our dialog
                    DoYell(SAY_WRATH,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_WRATH);
                }

                //60 seconds until we should cast this agian
                WrathOfRagnaros_Timer = 30000;
            }else WrathOfRagnaros_Timer -= diff;

            //HandOfRagnaros_Timer
            if (HandOfRagnaros_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_HANDOFRAGNAROS);

                if (rand()%2==0)
                {
                    //Say our dialog
                    DoYell(SAY_HAND,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_HAND);
                }

                //60 seconds until we should cast this agian
                HandOfRagnaros_Timer = 25000;
            }else HandOfRagnaros_Timer -= diff;

            //LavaBurst_Timer
            if (LavaBurst_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_LAVABURST);

                //10 seconds until we should cast this agian
                LavaBurst_Timer = 10000;
            }else LavaBurst_Timer -= diff;

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
            }else
            {
                //Ragnaros doesn't like it when he isn't in melee range so he casts magma burst
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    Unit* newtarget = m_creature->SelectHostilTarget();

                    if(newtarget)
                        AttackStart(newtarget);

                    DoCast(m_creature->getVictim(),SPELL_MAGMABURST);
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
CreatureAI* GetAI_boss_ragnaros(Creature *_Creature)
{
    return new boss_ragnarosAI (_Creature);
}


void AddSC_boss_ragnaros()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ragnaros";
    newscript->GetAI = GetAI_boss_ragnaros;
    m_scripts[nrscripts++] = newscript;
}