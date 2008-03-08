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
SDName: Boss_Drakkisath
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Spire
EndScriptData */

#include "../../sc_defines.h"


#define SPELL_FIRENOVA                  23462        
#define SPELL_CLEAVE                    20691
#define SPELL_CONFLIGURATION            16805
#define SPELL_THUNDERCLAP               15548        //Not sure if right ID. 23931 would be a harder possibility.

struct MANGOS_DLL_DECL boss_drakkisathAI : public ScriptedAI
{
    boss_drakkisathAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 FireNova_Timer;
    uint32 Cleave_Timer;
    uint32 Confliguration_Timer;
    uint32 Thunderclap_Timer;
    bool InCombat;

    void Reset()
    {       
        FireNova_Timer = 6000;
        Cleave_Timer = 8000;
        Confliguration_Timer = 15000;
        Thunderclap_Timer = 17000;
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

            //10 seconds
            FireNova_Timer = 10000;
        }else FireNova_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            //8 seconds until we should cast this agian
            Cleave_Timer = 8000;
        }else Cleave_Timer -= diff;

        //Confliguration_Timer
        if (Confliguration_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CONFLIGURATION);

            //18 seconds until we should cast this agian
            Confliguration_Timer = 18000;
        }else Confliguration_Timer -= diff;

        //Thunderclap_Timer
        if (Thunderclap_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_THUNDERCLAP);

            //22 seconds until we should cast this agian
            Thunderclap_Timer = 20000;
        }else Thunderclap_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_boss_drakkisath(Creature *_Creature)
{
    return new boss_drakkisathAI (_Creature);
}


void AddSC_boss_drakkisath()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_drakkisath";
    newscript->GetAI = GetAI_boss_drakkisath;
    m_scripts[nrscripts++] = newscript;
}
