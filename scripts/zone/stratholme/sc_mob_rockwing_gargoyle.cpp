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

//Status: check spell ID, remove sound if not wanted

#define SOUND_FLYIN    6596
#define SOUND_FLYAWAY    6526

#define SPELL_DAZED    1604
#define SPELL_SUNDERARMOR    27991
#define SPELL_DEAFENINGSCREECH    3589

struct MANGOS_DLL_DECL mob_rockwing_gargoyleAI : public ScriptedAI
{
    mob_rockwing_gargoyleAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Dazed_Timer;
    uint32 SunderArmor_Timer;
    uint32 DeafeningScreech_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Dazed_Timer = 3000;
        SunderArmor_Timer = 7000;
        DeafeningScreech_Timer = 9000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoPlaySoundToSet(m_creature,SOUND_FLYAWAY);
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if (!InCombat)
            {
                DoPlaySoundToSet(m_creature,SOUND_FLYIN);
            }

            DoStartMeleeAttack(who);
            InCombat = true;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!InCombat)
                {
                    DoPlaySoundToSet(m_creature,SOUND_FLYIN);
                    m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                }

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
                InCombat = true;
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

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 20) //20% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //10 seconds until we should try try cast this again
                Dazed_Timer = 10000;
            }else Dazed_Timer -= diff;

            //SunderArmor
            if (SunderArmor_Timer < diff)
            {
                //Cast
                if (rand()%100 < 75) //75% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_SUNDERARMOR);
                }
                //7 seconds until we should try cast this again
                SunderArmor_Timer = 7000;
            }else SunderArmor_Timer -= diff;

            //DeafeningScreech
            if (DeafeningScreech_Timer < diff)
            {
                if (rand()%100 < 55) //55% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_DEAFENINGSCREECH);
                }
                //21 seconds until we should try cast this again
                DeafeningScreech_Timer = 21000;
            }else DeafeningScreech_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
}; 
CreatureAI* GetAI_mob_rockwing_gargoyle(Creature *_Creature)
{
    return new mob_rockwing_gargoyleAI (_Creature);
}


void AddSC_mob_rockwing_gargoyle()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_rockwing_gargoyle";
    newscript->GetAI = GetAI_mob_rockwing_gargoyle;
    m_scripts[nrscripts++] = newscript;
}