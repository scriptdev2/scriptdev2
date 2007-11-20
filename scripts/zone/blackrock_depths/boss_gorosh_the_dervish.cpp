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
SDName: Boss_Gorosh_the_Dervish
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****

#define SPELL_WHIRLWIND                15589
#define SPELL_MORTALSTRIKE            24573

struct MANGOS_DLL_DECL boss_gorosh_the_dervishAI : public ScriptedAI
{
    boss_gorosh_the_dervishAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 WhirlWind_Timer;
    uint32 MortalStrike_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        WhirlWind_Timer = 12000;
        MortalStrike_Timer = 22000;
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;


        //WhirlWind_Timer
        if (WhirlWind_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_WHIRLWIND);

            //15 seconds
            WhirlWind_Timer = 15000;
        }else WhirlWind_Timer -= diff;


        //MortalStrike_Timer
        if (MortalStrike_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MORTALSTRIKE);

            //15 seconds until we should cast this agian
            MortalStrike_Timer = 15000;
        }else MortalStrike_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_gorosh_the_dervish(Creature *_Creature)
{
    return new boss_gorosh_the_dervishAI (_Creature);
}


void AddSC_boss_gorosh_the_dervish()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gorosh_the_dervish";
    newscript->GetAI = GetAI_boss_gorosh_the_dervish;
    m_scripts[nrscripts++] = newscript;
}
