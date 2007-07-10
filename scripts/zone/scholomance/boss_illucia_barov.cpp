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

#define SPELL_CURSEOFAGONY                18671     
#define SPELL_SHADOWSHOCK            20603
#define SPELL_SILENCE                 15487
#define SPELL_FEAR              6215       

       

struct MANGOS_DLL_DECL boss_illuciabarovAI : public ScriptedAI
{
    boss_illuciabarovAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 CurseOfAgony_Timer;
    uint32 ShadowShock_Timer;
    uint32 Silence_Timer;
    uint32 Fear_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        CurseOfAgony_Timer = 18000;
        ShadowShock_Timer = 9000;
        Silence_Timer = 5000;
        Fear_Timer = 45000;
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
            
            //CurseOfAgony_Timer
            if (CurseOfAgony_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CURSEOFAGONY);

                //30 seconds
                CurseOfAgony_Timer = 30000;
            }else CurseOfAgony_Timer -= diff;

            //ShadowShock_Timer
            if (ShadowShock_Timer < diff)
            {
                 //Cast ShadowShock on a Random target
                 Unit* target = NULL;
 
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_SHADOWSHOCK);

                //12 seconds until we should cast this agian
                ShadowShock_Timer = 12000;
            }else ShadowShock_Timer -= diff;

            //Silence_Timer
            if (Silence_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SILENCE);

                //14 seconds until we should cast this agian
                Silence_Timer = 14000;
            }else Silence_Timer -= diff;

            //Fear_Timer
            if (Fear_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FEAR);

                //30 seconds until we should cast this agian
                Fear_Timer = 30000;
            }else Fear_Timer -= diff;


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
CreatureAI* GetAI_boss_illuciabarov(Creature *_Creature)
{
    return new boss_illuciabarovAI (_Creature);
}


void AddSC_boss_illuciabarov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_illucia_barov";
    newscript->GetAI = GetAI_boss_illuciabarov;
    m_scripts[nrscripts++] = newscript;
}
