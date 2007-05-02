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

#define SPELL_SOULBURN              19393
#define SPELL_SUMMONLAVASPAWN       19392

struct MANGOS_DLL_DECL mob_firelordAI : public ScriptedAI
{
    mob_firelordAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 SoulBurn_Timer;
    uint32 SummonLavaSpawn_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        SoulBurn_Timer = 8000;
        SummonLavaSpawn_Timer = 12000;
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

            //SoulBurn
            if (SoulBurn_Timer < diff)
            {
                //Cast
                if (rand()%100 < 70) //70% chance to cast
                {
                DoCast(m_creature->getVictim(),SPELL_SOULBURN);
                }
                //20 seconds until we should cast this again
                SoulBurn_Timer = 20000;
            }else SoulBurn_Timer -= diff;

            //SummonLavaSpawn
            if (SummonLavaSpawn_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_SUMMONLAVASPAWN);
                //30 seconds until we should cast this again
                SummonLavaSpawn_Timer = 30000;
            }else SummonLavaSpawn_Timer -= diff;


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
CreatureAI* GetAI_mob_firelord(Creature *_Creature)
{
    return new mob_firelordAI (_Creature);
}


void AddSC_mob_firelord()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_firelord";
    newscript->GetAI = GetAI_mob_firelord;
    m_scripts[nrscripts++] = newscript;
}