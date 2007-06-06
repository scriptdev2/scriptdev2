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
/*
6/3 20:04:22.453  Plague Ghoul gains Enrage.
6/3 20:04:30.937  Enrage fades from Plague Ghoul.
6/3 20:04:32.203  Plague Ghoul gains Enrage.
6/3 20:07:36.921  Enrage fades from Plague Ghoul.
6/3 20:07:40.218  Plague Ghoul gains Enrage.
6/3 20:08:38.078  Enrage fades from Plague Ghoul.
*/
#define SPELL_CLEAVE    15584
#define SPELL_REND    18200
#define SPELL_DAZED    1604
#define SPELL_HEADCRACK    9791
#define SPELL_GHOULPLAGUE    16458

struct MANGOS_DLL_DECL mob_plague_ghoulAI : public ScriptedAI
{
    mob_plague_ghoulAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Cleave_Timer;
    uint32 Rend_Timer;
    uint32 Dazed_Timer;
    uint32 HeadCrack_Timer;
    uint32 GhoulPlague_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Cleave_Timer = 4000;
        Rend_Timer = 6000;
        Dazed_Timer = 11000;
        HeadCrack_Timer = 13000;
        GhoulPlague_Timer = 15000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
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

            //Cleave
            if (Cleave_Timer < diff)
            {
                //Cast
                if (rand()%100 < 70) //70% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CLEAVE);
                }
                //9 seconds until we should cast this again
                Cleave_Timer = 9000;
            }else Cleave_Timer -= diff;

            //Rend
            if (Rend_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_REND);
                //40 seconds until we should cast this again
                Rend_Timer = 40000;
            }else Rend_Timer -= diff;

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 10) //10% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //11 seconds until we should try cast this again
                Dazed_Timer = 11000;
            }else Dazed_Timer -= diff;

            //HeadCrack
            if (HeadCrack_Timer < diff)
            {
                //Cast
                if (rand()%100 < 20) //20% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_HEADCRACK);
                }
                //13 seconds until we should try cast this again
                HeadCrack_Timer = 13000;
            }else HeadCrack_Timer -= diff;

            //GhoulPlague
            if (GhoulPlague_Timer < diff)
            {
                //Cast
                if (rand()%100 < 15) //15% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_GHOULPLAGUE);
                }
                //15 seconds until we should try cast this again
                GhoulPlague_Timer = 15000;
            }else GhoulPlague_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DIST))
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
CreatureAI* GetAI_mob_plague_ghoul(Creature *_Creature)
{
    return new mob_plague_ghoulAI (_Creature);
}


void AddSC_mob_plague_ghoul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_plague_ghoul";
    newscript->GetAI = GetAI_mob_plague_ghoul;
    m_scripts[nrscripts++] = newscript;
}