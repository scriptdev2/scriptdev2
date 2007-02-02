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

#define SPELL_POLYMORPH	            38896
#define SPELL_AOESILENCE            8988
#define SPELL_ARCANEEXPLOSION3      8438
#define SPELL_ARCANEEXPLOSION4      8439
#define SPELL_FIREAOE		        25049
#define SPELL_BLINK                 1953
#define SPELL_FIREBALL              21162
#define SPELL_MANASHIELD4           10191
#define SPELL_ARCANEBUBBLE          9438

#define SAY_AGGRO			"You will not defile these mysteries!"
#define SAY_SPECIALAE	    "Burn in righteous fire!"

#define SOUND_AGGRO			5842
#define SOUND_SPECIALAE		5843

struct MANGOS_DLL_DECL boss_arcanist_doanAI : public ScriptedAI
{
    boss_arcanist_doanAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
	uint32 SpecialAoE_Timer;
    uint32 AoESilence_Timer;
    uint32 ArcaneExplosion3_Timer;
    uint32 ArcaneExplosion4_Timer;
	uint32 Blink_Timer;
	uint32 Fireball_Timer;
	uint32 ManaShield4_Timer;

    void Reset()
    {
        pTarget = NULL;
		SpecialAoE_Timer = 180000;
        AoESilence_Timer = 60000;
        ArcaneExplosion3_Timer = 30000;
        ArcaneExplosion4_Timer = 35000;
		Blink_Timer = 60000;
		Fireball_Timer = 30000;
		ManaShield4_Timer = 65000;

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
                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                pTarget = who;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //If we had a target and it wasn't cleared then it means the player died from some unknown source
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

            //If we are <50% hp cast Arcane Bubble and start casting SPECIAL FIRE AOE
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 50 && !m_creature->m_currentSpell)
            {
				//SpecialAoE_Timer
				if (SpecialAoE_Timer < diff)
				{
					DoCast(m_creature->getVictim(),SPELL_POLYMORPH);
					DoYell(SAY_SPECIALAE,LANG_UNIVERSAL,NULL);
					DoPlaySoundToSet(m_creature,SOUND_SPECIALAE);
					DoCast(m_creature,SPELL_ARCANEBUBBLE);
					DoCast(m_creature->getVictim(),SPELL_FIREAOE);
					return;
	
	                //180 seconds until we should cast this agian
	                SpecialAoE_Timer = 180000;
	            }else SpecialAoE_Timer -= diff;
            }
            
            //AoESilence_Timer
            if (AoESilence_Timer < diff)
            {
                //Cast
				DoCast(m_creature->getVictim(),SPELL_AOESILENCE);

                //60 seconds until we should cast this agian
                AoESilence_Timer = 60000;
            }else AoESilence_Timer -= diff;

            //ArcaneExplosion3_Timer
            if (ArcaneExplosion3_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_ARCANEEXPLOSION3);

                //30 seconds until we should cast this agian
                ArcaneExplosion3_Timer = 30000;
            }else ArcaneExplosion3_Timer -= diff;

            //ArcaneExplosion4_Timer
            if (ArcaneExplosion4_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_ARCANEEXPLOSION4);

                //35 seconds until we should cast this agian
                ArcaneExplosion4_Timer = 35000;
            }else ArcaneExplosion4_Timer -= diff;

            //Blink_Timer
            if (Blink_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BLINK);

                //35 seconds until we should cast this agian
                Blink_Timer = 60000;
            }else Blink_Timer -= diff;

            //Fireball_Timer
            if (Fireball_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FIREBALL);

                //35 seconds until we should cast this agian
                Fireball_Timer = 30000;
            }else Fireball_Timer -= diff;

            //ManaShiled4_Timer
            if (ManaShield4_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_MANASHIELD4);

                //35 seconds until we should cast this agian
                ManaShield4_Timer = 65000;
            }else ManaShield4_Timer -= diff;

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
CreatureAI* GetAI_boss_arcanist_doan(Creature *_Creature)
{
    return new boss_arcanist_doanAI (_Creature);
}


void AddSC_boss_arcanist_doan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_arcanist_doan";
    newscript->GetAI = GetAI_boss_arcanist_doan;
    m_scripts[nrscripts++] = newscript;
}