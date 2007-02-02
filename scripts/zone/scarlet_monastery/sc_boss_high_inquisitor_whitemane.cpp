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

#define SPELL_DEEPSLEEP				9256
#define SPELL_SCARLETRESURRECTION	9232

#define SPELL_CRUSADERSTRIKE        17281
#define SPELL_HAMMEROFJUSTICE	    13005
#define SPELL_HOLYSMITE6	        9481
#define SPELL_HOLYFIRE5 		    15265
#define SPELL_MINDBLAST6	        8106

#define SPELL_POWERWORDSHIELD		6065

#define SPELL_RENEW					6078
#define SPELL_FLASHHEAL6	        10916

#define SAY_AGGRO			"There is no escape for you. The Crusade shall destroy all who carry the Scourge's taint."
#define SAY_SPAWN			"What, Mograine has fallen? You shall pay for this treachery! "
#define SAY_RES				"Arise, my champion!"
#define SAY_DEATH		    "The Light has spoken!"

//#define SOUND_AGGRO
#define SOUND_RES			5840
#define SOUND_SPAWN			5838
#define SOUND_DEATH			5839

struct MANGOS_DLL_DECL boss_high_inquisitor_whitemaneAI : public ScriptedAI
{
    boss_high_inquisitor_whitemaneAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
	uint32 Healing_Timer;
	uint32 PowerWordShield_Timer;
    uint32 CrusaderStrike_Timer;
    uint32 HammerOfJustice_Timer;
	uint32 HolySmite6_Timer;
	uint32 HolyFire5_Timer;
	uint32 MindBlast6_Timer;

    void Reset()
    {
        pTarget = NULL;
		Healing_Timer = 80000;
		PowerWordShield_Timer = 60000;
        CrusaderStrike_Timer = 40000;
        HammerOfJustice_Timer = 30000;
		HolySmite6_Timer = 10000;
		HolyFire5_Timer = 45000;
		MindBlast6_Timer = 35000;

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
			DoStartMeleeAttack(who);
			
			//Say our dialog
            DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);

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

            //If we are <75% hp cast healing spells at self and Mograine
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 75 && !m_creature->m_currentSpell)
            {
				
				//Healing_Timer
				if (Healing_Timer < diff)
				{

					DoCast(m_creature,SPELL_FLASHHEAL6);
					return;
	
	                //80 seconds until we should cast this agian
	                Healing_Timer = 80000;
	            }else Healing_Timer -= diff;      
				
				if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 30 && !m_creature->m_currentSpell)
				{

					if (Healing_Timer < diff)
					{
	
						DoCast(m_creature,SPELL_RENEW);
						return;
	
		                //80 seconds until we should cast this agian
		                Healing_Timer = 40000;
		            }else Healing_Timer -= diff;      
				}
                return;
            }
            
            //PowerWordShield_Timer
            if (PowerWordShield_Timer < diff)
            {
                //Cast
				DoCast(m_creature->getVictim(),SPELL_POWERWORDSHIELD);

                //60 seconds until we should cast this agian
                PowerWordShield_Timer = 60000;
            }else PowerWordShield_Timer -= diff;

            //CrusaderStrike_Timer
            if (CrusaderStrike_Timer < diff)
            {
                //Cast
				DoCast(m_creature->getVictim(),SPELL_CRUSADERSTRIKE);

                //40 seconds until we should cast this agian
                CrusaderStrike_Timer = 40000;
            }else CrusaderStrike_Timer -= diff;

            //HammerOfJustice_Timer
            if (HammerOfJustice_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_HAMMEROFJUSTICE);

                //40 seconds until we should cast this agian
                HammerOfJustice_Timer = 30000;
            }else HammerOfJustice_Timer -= diff;

            //HolySmite6_Timer
            if (HolySmite6_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_HOLYSMITE6);

                //30 seconds until we should cast this agian
                HolySmite6_Timer = 10000;
            }else HolySmite6_Timer -= diff;

            //HolyFire5_Timer
            if (HolyFire5_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_HOLYFIRE5);

                //45 seconds until we should cast this agian
                HolyFire5_Timer = 45000;
            }else HolyFire5_Timer -= diff;

            //MindBlast6_Timer
            if (MindBlast6_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_MINDBLAST6);

                //345 seconds until we should cast this agian
                MindBlast6_Timer = 35000;
            }else MindBlast6_Timer -= diff;

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
				DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
				DoPlaySoundToSet(m_creature,SOUND_DEATH);
				return;

                Reset();
            }
        }
    }
}; 
CreatureAI* GetAI_boss_high_inquisitor_whitemane(Creature *_Creature)
{
    return new boss_high_inquisitor_whitemaneAI (_Creature);
}


void AddSC_boss_high_inquisitor_whitemane()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_high_inquisitor_whitemane";
    newscript->GetAI = GetAI_boss_high_inquisitor_whitemane;
    m_scripts[nrscripts++] = newscript;
}