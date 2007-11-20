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
SDName: Boss_Shirrak_the_dead_watcher
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../../sc_defines.h"

#define SPELL_INHABITMAGIC          32264            
#define SPELL_ATTRACTMAGIC          32265
#define SPELL_CARNIVOROUSBITE       41092

struct MANGOS_DLL_DECL boss_shirrak_the_dead_watcherAI : public ScriptedAI
{
    boss_shirrak_the_dead_watcherAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Inhabitmagic_Timer;
    uint32 Attractmagic_Timer;
    uint32 Carnivorousbite_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Inhabitmagic_Timer = 2000;
        Attractmagic_Timer = 10000;
        Carnivorousbite_Timer = 11000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
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

        //Inhabitmagic_Timer
        if (Inhabitmagic_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_INHABITMAGIC);

            //30 seconds
            Inhabitmagic_Timer = 2000;
        }else Inhabitmagic_Timer -= diff;

        //Attractmagic_Timer
        if (Attractmagic_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_ATTRACTMAGIC);

            //15 seconds until we should cast this agian
            Attractmagic_Timer = 10000;
        }else Attractmagic_Timer -= diff;

        //Carnivorousbite_Timer
        if (Carnivorousbite_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CARNIVOROUSBITE);

            //15 seconds until we should cast this agian
            Carnivorousbite_Timer = 11000;
        }else Carnivorousbite_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_shirrak_the_dead_watcher(Creature *_Creature)
{
    return new boss_shirrak_the_dead_watcherAI (_Creature);
}


void AddSC_boss_shirrak_the_dead_watcher()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_shirrak_the_dead_watcher";
    newscript->GetAI = GetAI_boss_shirrak_the_dead_watcher;
    m_scripts[nrscripts++] = newscript;
}
