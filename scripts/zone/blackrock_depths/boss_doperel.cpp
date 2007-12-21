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
SDName: Boss_Doperel
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Depths
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****

#define SPELL_SINISTERSTRIKE                15581
#define SPELL_BACKSTAB             15582  
#define SPELL_GOUGE            13579

struct MANGOS_DLL_DECL boss_doperelAI : public ScriptedAI
{
    boss_doperelAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 SinisterStrike_Timer;
    uint32 BackStab_Timer;
    uint32 Gouge_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        SinisterStrike_Timer = 8000;
        BackStab_Timer = 12000;
        Gouge_Timer = 6000;
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

        //SinisterStrike_Timer
        if (SinisterStrike_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SINISTERSTRIKE);

            //7 seconds
            SinisterStrike_Timer = 7000;
        }else SinisterStrike_Timer -= diff;

        //BackStab_Timer
        if (BackStab_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_BACKSTAB);

            //6 seconds until we should cast this agian
            BackStab_Timer = 6000;
        }else BackStab_Timer -= diff;

        //Gouge_Timer
        if (Gouge_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_GOUGE);

            //8 seconds until we should cast this agian
            Gouge_Timer = 8000;
        }else Gouge_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_doperel(Creature *_Creature)
{
    return new boss_doperelAI (_Creature);
}


void AddSC_boss_doperel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_doperel";
    newscript->GetAI = GetAI_boss_doperel;
    m_scripts[nrscripts++] = newscript;
}
