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
SDName: Boss_Gloomrel
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****

#define SPELL_HAMSTRING                9080
#define SPELL_CLEAVE             15579  
#define SPELL_MORTALSTRIKE            15708

struct MANGOS_DLL_DECL boss_gloomrelAI : public ScriptedAI
{
    boss_gloomrelAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 HamString_Timer;
    uint32 Cleave_Timer;
    uint32 MortalStrike_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        HamString_Timer = 19000;
        Cleave_Timer = 6000;
        MortalStrike_Timer = 10000;
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

        //HamString_Timer
        if (HamString_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_HAMSTRING);

            //14 seconds
            HamString_Timer = 14000;
        }else HamString_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            //8 seconds until we should cast this agian
            Cleave_Timer = 8000;
        }else Cleave_Timer -= diff;

        //MortalStrike_Timer
        if (MortalStrike_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MORTALSTRIKE);

            //12 seconds until we should cast this agian
            MortalStrike_Timer = 12000;
        }else MortalStrike_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_gloomrel(Creature *_Creature)
{
    return new boss_gloomrelAI (_Creature);
}


void AddSC_boss_gloomrel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gloomrel";
    newscript->GetAI = GetAI_boss_gloomrel;
    m_scripts[nrscripts++] = newscript;
}
