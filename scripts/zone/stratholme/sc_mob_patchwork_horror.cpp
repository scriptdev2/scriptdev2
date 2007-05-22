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

// Status: check disease cloud, ready for SVN

#define SPELL_DISEASECLOUD    27793
#define SPELL_KNOCKAWAY    10101
#define SPELL_DAZED    1604
#define SPELL_WEAKENEDSOUL    16144

struct MANGOS_DLL_DECL mob_patchwork_horrorAI : public ScriptedAI
{
    mob_patchwork_horrorAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 KnockAway_Timer;
    uint32 Dazed_Timer;
    uint32 WeakenedSoul_Timer;
    bool HasAura;
    bool InCombat;

    void EnterEvadeMode()
    {
        KnockAway_Timer = 14000;
        Dazed_Timer = 5000;
        WeakenedSoul_Timer = 18000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        m_creature->CastSpell(m_creature,SPELL_DISEASECLOUD,true);
        HasAura = true;
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

                if (!HasAura)
                {
                    m_creature->CastSpell(m_creature,SPELL_DISEASECLOUD,true);
                    HasAura = true;
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

            //KnockAway
            if (KnockAway_Timer < diff)
            {
                //Cast
                if (rand()%100 < 75) //75% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_KNOCKAWAY);
                }
                //14 seconds until we should cast this again
                KnockAway_Timer = 14000;
            }else KnockAway_Timer -= diff;

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 25) //25% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //16 seconds until we should try cast this again
                Dazed_Timer = 16000;
            }else Dazed_Timer -= diff;

            //WeakenedSoul
            if (WeakenedSoul_Timer < diff)
            {
                //Cast
                if (rand()%100 < 25) //25% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_WEAKENEDSOUL);
                }
                //18 seconds until we should try cast this again
                WeakenedSoul_Timer = 18000;
            }else WeakenedSoul_Timer -= diff;

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
CreatureAI* GetAI_mob_patchwork_horror(Creature *_Creature)
{
    return new mob_patchwork_horrorAI (_Creature);
}


void AddSC_mob_patchwork_horror()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_patchwork_horror";
    newscript->GetAI = GetAI_mob_patchwork_horror;
    m_scripts[nrscripts++] = newscript;
}