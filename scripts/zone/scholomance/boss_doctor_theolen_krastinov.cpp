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
SDName: Boss_Doctor_Theolen_Krastinov
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_REND              18106
#define SPELL_CLEAVE            15584
#define SPELL_FRENZY            28371

struct MANGOS_DLL_DECL boss_theolenkrastinovAI : public ScriptedAI
{
    boss_theolenkrastinovAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Rend_Timer;
    uint32 Cleave_Timer;
    uint32 Frenzy_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Rend_Timer = 8000;
        Cleave_Timer = 9000;
        Frenzy_Timer =0;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void JustDied(Unit *killer)
    {
        ScriptedInstance *pInstance = (m_creature->GetInstanceData()) ? ((ScriptedInstance*)m_creature->GetInstanceData()) : NULL;
        if(pInstance)
        {
            pInstance->SetData("DoctorTheolenKrastinov_Death", 0);

            if(pInstance->GetData("CanSpawnGandling"))
            {
                Unit *gandling = NULL;
                gandling = m_creature->SummonCreature(1853, 180.73, -9.43856, 75.507, 1.61399, TEMPSUMMON_DEAD_DESPAWN, 0);
            }
        }
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Rend_Timer
        if (Rend_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_REND);

            //10 seconds
            Rend_Timer = 10000;
        }else Rend_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            //10 seconds until we should cast this agian
            Cleave_Timer = 10000;
        }else Cleave_Timer -= diff;

        //Frenzy_Timer
        if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 26 )
        {
            if (Frenzy_Timer < diff)
            {
                DoCast(m_creature,SPELL_FRENZY);
                DoTextEmote("goes into a killing frenzy!",NULL);

                //8 seconds
                Frenzy_Timer = 8000;
            }else Frenzy_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_theolenkrastinov(Creature *_Creature)
{
    return new boss_theolenkrastinovAI (_Creature);
}


void AddSC_boss_theolenkrastinov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_doctor_theolen_krastinov";
    newscript->GetAI = GetAI_boss_theolenkrastinov;
    m_scripts[nrscripts++] = newscript;
}
