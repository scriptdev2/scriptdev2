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
SDName: boss_sartura
SD%Complete: 0
SDComment: VERIFY SCRIPT
EndScriptData */

/*
To do:
Change to random attack for 5seconds
*/

#include "../../sc_defines.h"

#define SPELL_WHIRLWIND 37641
#define SPELL_ENRAGE 28798


struct MANGOS_DLL_DECL boss_sarturaAI : public ScriptedAI
{
    boss_sarturaAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Whirlwind_Timer;
    bool Enraged;

    void Reset()
    {
        Whirlwind_Timer = 30000;
        Enraged = false; 

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
            if (m_creature->IsWithinDistInMap(who, ATTACK_DISTANCE))
                DoStartMeleeAttack(who);
            else DoStartRangedAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //If he is 20% enrage
        if (!Enraged)
            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 20 && !m_creature->IsNonMeleeSpellCasted(false))
            {
                DoCast(m_creature->getVictim(),SPELL_ENRAGE); 
                Enraged = true;
            }


            //Whirlwind_Timer (only in phase2)
            if (Whirlwind_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_WHIRLWIND);
                Whirlwind_Timer = 30000;
            }else Whirlwind_Timer -= diff;

            DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_sartura(Creature *_Creature)
{
    return new boss_sarturaAI (_Creature);
}


void AddSC_boss_sartura()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_sartura";
    newscript->GetAI = GetAI_boss_sartura;
    m_scripts[nrscripts++] = newscript;
}
