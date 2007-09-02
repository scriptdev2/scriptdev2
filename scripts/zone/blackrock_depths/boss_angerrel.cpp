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

#define SPELL_SUNDERARMOR                24317                   
#define SPELL_SHIELDBLOCK            12169
#define SPELL_STRIKE               15580




struct MANGOS_DLL_DECL boss_angerrelAI : public ScriptedAI
{
    boss_angerrelAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 SunderArmor_Timer;
    uint32 ShieldBlock_Timer;
    uint32 Strike_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        SunderArmor_Timer = 8000;
        ShieldBlock_Timer = 15000;
        Strike_Timer = 12000;
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


        //SunderArmor_Timer
        if (SunderArmor_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SUNDERARMOR);

            //28 seconds
            SunderArmor_Timer = 28000;
        }else SunderArmor_Timer -= diff;


        //ShieldBlock_Timer
        if (ShieldBlock_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_SHIELDBLOCK);

            //25 seconds
            ShieldBlock_Timer = 25000;
        }else ShieldBlock_Timer -= diff;

        //Strike_Timer
        if (Strike_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_STRIKE);

            //10 seconds
            Strike_Timer = 10000;
        }else Strike_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_angerrel(Creature *_Creature)
{
    return new boss_angerrelAI (_Creature);
}


void AddSC_boss_angerrel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_angerrel";
    newscript->GetAI = GetAI_boss_angerrel;
    m_scripts[nrscripts++] = newscript;
}
