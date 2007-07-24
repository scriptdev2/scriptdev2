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

#define SPELL_PUNCTURE                21911    
#define SPELL_CLEAVE             15584     
#define SPELL_THORNVOLLEY            21749
    

struct MANGOS_DLL_DECL razorlashAI : public ScriptedAI
{
    razorlashAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Puncture_Timer;
    uint32 Cleave_Timer;
    uint32 ThornVolley_Timer;
    bool InCombat;


    void EnterEvadeMode()
    {       
        Puncture_Timer = 12000;
        Cleave_Timer = 6000;
        ThornVolley_Timer = 19000;
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
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {

            //Puncture_Timer
            if (Puncture_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_PUNCTURE);

                //20 seconds
                Puncture_Timer = 12000;
            }else Puncture_Timer -= diff;

            //Cleave_Timer
            if (Cleave_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CLEAVE);

                //8 seconds until we should cast this agian
                Cleave_Timer = 8000;
            }else Cleave_Timer -= diff;


            //ThornVolley_Timer
            if (ThornVolley_Timer < diff)
            {

                DoCast(m_creature->getVictim(),SPELL_THORNVOLLEY);

                //30 seconds until we should cast this agian
                ThornVolley_Timer = 12000;
            }else ThornVolley_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_razorlash(Creature *_Creature)
{
    return new razorlashAI (_Creature);
}


void AddSC_razorlash()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="razorlash";
    newscript->GetAI = GetAI_razorlash;
    m_scripts[nrscripts++] = newscript;
}
