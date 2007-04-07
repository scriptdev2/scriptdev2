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
// Adds NYI

#define SPELL_ANTIMAGICPULSE        19492
#define SPELL_MAGMASHACKLES         19496

struct MANGOS_DLL_DECL boss_garrAI : public ScriptedAI
{
    boss_garrAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 AntiMagicPulse_Timer;
    uint32 MagmaShackles_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        AntiMagicPulse_Timer = 25000;      //These times are probably wrong
        MagmaShackles_Timer = 15000;
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
            //Begin melee attack if we are within range
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
            
            //AntiMagicPulse_Timer
            if (AntiMagicPulse_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_ANTIMAGICPULSE);

                //20 seconds until we should cast this agian
                AntiMagicPulse_Timer = 20000;
            }else AntiMagicPulse_Timer -= diff;

            //MagmaShackles_Timer
            if (MagmaShackles_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_MAGMASHACKLES);

                //15 seconds until we should cast this agian
                MagmaShackles_Timer = 15000;
            }else MagmaShackles_Timer -= diff;

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
CreatureAI* GetAI_boss_garr(Creature *_Creature)
{
    return new boss_garrAI (_Creature);
}


void AddSC_boss_garr()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_garr";
    newscript->GetAI = GetAI_boss_garr;
    m_scripts[nrscripts++] = newscript;
}