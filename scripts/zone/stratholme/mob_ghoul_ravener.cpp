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

#define SPELL_RAVENOUSCLAW    15608
#define SPELL_REND    18200
#define SPELL_DAZED    1604
#define SPELL_HEADCRACK    9791
#define SPELL_GHOULPLAGUE    16458

struct MANGOS_DLL_DECL mob_ghoul_ravenerAI : public ScriptedAI
{
    mob_ghoul_ravenerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 RavenousClaw_Timer;
    uint32 Rend_Timer;
    uint32 Dazed_Timer;
    uint32 HeadCrack_Timer;
    uint32 GhoulPlague_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        RavenousClaw_Timer = 3000;
        Rend_Timer = 2000;
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
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

            //RavenousClaw
            if (RavenousClaw_Timer < diff)
            {
                //Cast
                if (rand()%100 < 25) //25% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_RAVENOUSCLAW);
                }
                //3 seconds until we should cast this again
                RavenousClaw_Timer = 3000;
            }else RavenousClaw_Timer -= diff;

            //Rend
            if (Rend_Timer < diff)
            {
                //Cast
                if (rand()%100 < 85) //85% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_REND);
                }
                //30 seconds until we should cast this again
                Rend_Timer = 30000;
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
                if (rand()%100 < 35) //35% chance to cast
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
                if (rand()%100 < 10) //10% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_GHOULPLAGUE);
                }
                //14 seconds until we should try cast this again
                GhoulPlague_Timer = 14000;
            }else GhoulPlague_Timer -= diff;

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
CreatureAI* GetAI_mob_ghoul_ravener(Creature *_Creature)
{
    return new mob_ghoul_ravenerAI (_Creature);
}


void AddSC_mob_ghoul_ravener()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_ghoul_ravener";
    newscript->GetAI = GetAI_mob_ghoul_ravener;
    m_scripts[nrscripts++] = newscript;
}