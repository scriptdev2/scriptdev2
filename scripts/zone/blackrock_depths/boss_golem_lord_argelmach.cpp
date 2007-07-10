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

#define SPELL_CHAINLIGHTNING                16033            
#define SPELL_SHOCK            16034
#define SPELL_LIGHTNINGSHIELD               10432

struct MANGOS_DLL_DECL boss_golemlordargelmachAI : public ScriptedAI
{
    boss_golemlordargelmachAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ChainLightning_Timer;
    uint32 Shock_Timer;
    uint32 LightningShield_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        ChainLightning_Timer = 14000;
        Shock_Timer = 6000;
        LightningShield_Timer = 2000;
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
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

            //ChainLightning_Timer
            if (ChainLightning_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CHAINLIGHTNING);

                //15 seconds until we should cast this agian
                ChainLightning_Timer = 15000;
            }else ChainLightning_Timer -= diff;


            //Shock_Timer
            if (Shock_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SHOCK);

                //7 seconds until we should cast this agian
                Shock_Timer = 7000;
            }else Shock_Timer -= diff;

            //LightningShield_Timer
            if (LightningShield_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_LIGHTNINGSHIELD);

                //45 seconds
                LightningShield_Timer = 45000;
            }else LightningShield_Timer -= diff;



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
CreatureAI* GetAI_boss_golemlordargelmach(Creature *_Creature)
{
    return new boss_golemlordargelmachAI (_Creature);
}


void AddSC_boss_golemlordargelmach()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_golem_lord_argelmach";
    newscript->GetAI = GetAI_boss_golemlordargelmach;
    m_scripts[nrscripts++] = newscript;
}
