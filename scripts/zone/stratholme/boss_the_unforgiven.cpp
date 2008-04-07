/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_the_unforgiven
SD%Complete: 100
SDComment: 
EndScriptData */

#include "sc_creature.h"

// **** This script is still under Developement ****


#define SPELL_FROSTBOLT    17503
#define SPELL_CHILLED    20005

struct MANGOS_DLL_DECL boss_the_unforgivenAI : public ScriptedAI
{
    boss_the_unforgivenAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Frostbolt_Timer;
    uint32 Chilled_Timer;
    bool InCombat;

    void Reset()
    {
        Frostbolt_Timer = 1000;
        Chilled_Timer = 7000;
        InCombat = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Frostbolt
        if (Frostbolt_Timer < diff)
        {
            //Cast
            if (rand()%100 < 95) //95% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_FROSTBOLT);
            }
            //4 seconds until we should cast this again
            Frostbolt_Timer = 4000;
        }else Frostbolt_Timer -= diff;

        //Chilled
        if (Chilled_Timer < diff)
        {
            //Cast
            if (rand()%100 < 45) //45% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_CHILLED);
            }
            //14 seconds until we should cast this again
            Chilled_Timer = 14000;
        }else Chilled_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_the_unforgiven(Creature *_Creature)
{
    return new boss_the_unforgivenAI (_Creature);
}


void AddSC_boss_the_unforgiven()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_the_unforgiven";
    newscript->GetAI = GetAI_boss_the_unforgiven;
    m_scripts[nrscripts++] = newscript;
}
