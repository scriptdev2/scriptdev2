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

/* ScriptData
SDName: Boss_Pyroguard_Emberseer
SD%Complete: 100
SDComment: Event to activate Emberseer NYI
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_FIRENOVA                23462    
#define SPELL_FLAMEBUFFET             23341               
#define SPELL_PYROBLAST            17274

struct MANGOS_DLL_DECL boss_pyroguard_emberseerAI : public ScriptedAI
{
    boss_pyroguard_emberseerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FireNova_Timer;
    uint32 FlameBuffet_Timer;
    uint32 PyroBlast_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        FireNova_Timer = 6000;
        FlameBuffet_Timer = 3000;
        PyroBlast_Timer = 14000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //FireNova_Timer
        if (FireNova_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FIRENOVA);

            //6 seconds
            FireNova_Timer = 6000;
        }else FireNova_Timer -= diff;


        //FlameBuffet_Timer
        if (FlameBuffet_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FLAMEBUFFET);

            //20 seconds until we should cast this agian
            FlameBuffet_Timer = 14000;
        }else FlameBuffet_Timer -= diff;

        //PyroBlast_Timer
        if (PyroBlast_Timer < diff)
        {
            //Cast Immolate on a Random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_PYROBLAST);

            //20 seconds until we should cast this agian
            PyroBlast_Timer = 15000;
        }else PyroBlast_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_pyroguard_emberseer(Creature *_Creature)
{
    return new boss_pyroguard_emberseerAI (_Creature);
}


void AddSC_boss_pyroguard_emberseer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_pyroguard_emberseer";
    newscript->GetAI = GetAI_boss_pyroguard_emberseer;
    m_scripts[nrscripts++] = newscript;
}
