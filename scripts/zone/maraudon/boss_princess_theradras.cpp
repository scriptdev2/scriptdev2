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
SDName: Boss_Princess_Theradras
SD%Complete: 100
SDComment: 
EndScriptData */


#include "../../sc_defines.h"

#define SPELL_DUSTFIELD                21909   
#define SPELL_BOULDER             21832     
#define SPELL_KNOCKDOWN            19128
#define SPELL_REPULSIVEGAZE                 21869      


struct MANGOS_DLL_DECL boss_ptheradrasAI : public ScriptedAI
{
    boss_ptheradrasAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Dustfield_Timer;
    uint32 Boulder_Timer;
    uint32 Knockdown_Timer;
    uint32 RepulsiveGaze_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Dustfield_Timer = 8000;
        Boulder_Timer = 2000;
        Knockdown_Timer = 18000;
        RepulsiveGaze_Timer = 23000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        //m_creature->CastSpell(m_creature,SPELL_ICEARMOR,true);
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

        //Dustfield_Timer
        if (Dustfield_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_DUSTFIELD);

            //14 seconds
            Dustfield_Timer = 14000;
        }else Dustfield_Timer -= diff;

        //Boulder_Timer
        if (Boulder_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_BOULDER);

            //10 seconds until we should cast this agian
            Boulder_Timer = 10000;
        }else Boulder_Timer -= diff;


        //Knockdown_Timer
        if (Knockdown_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_KNOCKDOWN);

            //12 seconds until we should cast this agian
            Knockdown_Timer = 12000;
        }else Knockdown_Timer -= diff;

        //RepulsiveGaze_Timer
        if (RepulsiveGaze_Timer < diff)
        {
            //Cast Repulsive Gaze on a Random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_REPULSIVEGAZE);

            //20 seconds until we should cast this agian
            RepulsiveGaze_Timer = 20000;
        }else RepulsiveGaze_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_boss_ptheradras(Creature *_Creature)
{
    return new boss_ptheradrasAI (_Creature);
}


void AddSC_boss_ptheradras()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_princess_theradras";
    newscript->GetAI = GetAI_boss_ptheradras;
    m_scripts[nrscripts++] = newscript;
}
