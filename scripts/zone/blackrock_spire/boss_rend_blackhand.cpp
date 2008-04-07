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
SDName: Boss_Rend_Blackhand
SD%Complete: 100
SDComment: Intro event NYI
SDCategory: Blackrock Spire
EndScriptData */

#include "sc_creature.h"


#define SPELL_WHIRLWIND                  26038        
#define SPELL_CLEAVE                    20691
#define SPELL_THUNDERCLAP               23931        //Not sure if he cast this spell


struct MANGOS_DLL_DECL boss_rend_blackhandAI : public ScriptedAI
{
    boss_rend_blackhandAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 WhirlWind_Timer;
    uint32 Cleave_Timer;
    uint32 Thunderclap_Timer;
    bool InCombat;

    void Reset()
    {       
        WhirlWind_Timer = 20000;
        Cleave_Timer = 5000;
        Thunderclap_Timer = 9000;
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

        //WhirlWind_Timer
        if (WhirlWind_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_WHIRLWIND);

            //18 seconds
            WhirlWind_Timer = 18000;
        }else WhirlWind_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            //10 seconds until we should cast this agian
            Cleave_Timer = 10000;
        }else Cleave_Timer -= diff;


        //Thunderclap_Timer
        if (Thunderclap_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_THUNDERCLAP);

            //16 seconds until we should cast this agian
            Thunderclap_Timer = 16000;
        }else Thunderclap_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_boss_rend_blackhand(Creature *_Creature)
{
    return new boss_rend_blackhandAI (_Creature);
}


void AddSC_boss_rend_blackhand()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_rend_blackhand";
    newscript->GetAI = GetAI_boss_rend_blackhand;
    m_scripts[nrscripts++] = newscript;
}
