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
SDName: Boss_Fineous_Darkvine
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****


#define SPELL_HOLYSHOCK                20930    //Not sure if right ID            
#define SPELL_KICK            15614


struct MANGOS_DLL_DECL boss_fineous_darkvireAI : public ScriptedAI
{
    boss_fineous_darkvireAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 HolyShock_Timer;
    uint32 Kick_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        HolyShock_Timer = 5000;
        Kick_Timer = 12000;
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


        //HolyShock_Timer
        if (HolyShock_Timer < diff)
        {

            //Cast Holy Shock on a Random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_HOLYSHOCK);

            // And then cast Holy Shock on himself. Not working cause of cooldown spell
            DoCast(m_creature,SPELL_HOLYSHOCK);

            //16 seconds cause its a proc
            HolyShock_Timer = 16000;
        }else HolyShock_Timer -= diff;

        //Kick_Timer
        if (Kick_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_KICK);

            //7 seconds until we should cast this agian
            Kick_Timer = 7000;
        }else Kick_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_fineous_darkvire(Creature *_Creature)
{
    return new boss_fineous_darkvireAI (_Creature);
}


void AddSC_boss_fineous_darkvire()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_fineous_darkvire";
    newscript->GetAI = GetAI_boss_fineous_darkvire;
    m_scripts[nrscripts++] = newscript;
}
