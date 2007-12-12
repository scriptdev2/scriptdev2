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
SDName: Boss_the_ravenian
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_TRAMPLE           15550
#define SPELL_CLEAVE            20691
#define SPELL_SUNDERINCLEAVE    25174
#define SPELL_KNOCKAWAY         10101

#define SAY_AGGRO1              "Mine! Mine! Mine! Gizlock is the ruler of this domain! You shall never reveal my presence!"

struct MANGOS_DLL_DECL boss_theravenianAI : public ScriptedAI
{
    boss_theravenianAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Trample_Timer;
    uint32 Cleave_Timer;
    uint32 SunderingCleave_Timer;
    uint32 KnockAway_Timer;
    bool HasYelled;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Trample_Timer = 24000;
        Cleave_Timer = 15000;
        SunderingCleave_Timer = 40000;
        KnockAway_Timer = 32000;
        HasYelled = false;
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
            pInstance->SetData("TheRavenian_Death", 0);

            if(pInstance->GetData("CanSpawnGandling"))
                m_creature->SummonCreature(1853, 180.73, -9.43856, 75.507, 1.61399, TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if (!HasYelled)
            {
                DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
                HasYelled = true;
            }

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

                if (!HasYelled)
                {
                    DoYell(SAY_AGGRO1,LANG_UNIVERSAL,NULL);
                    HasYelled = true;
                }

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

        //Trample_Timer
        if (Trample_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_TRAMPLE);

            //10 seconds
            Trample_Timer = 10000;
        }else Trample_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            //7 seconds until we should cast this agian
            Cleave_Timer = 7000;
        }else Cleave_Timer -= diff;

        //SunderingCleave_Timer
        if (SunderingCleave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SUNDERINCLEAVE);

            //20 seconds until we should cast this agian
            SunderingCleave_Timer = 20000;
        }else SunderingCleave_Timer -= diff;

        //KnockAway_Timer
        if (KnockAway_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_KNOCKAWAY);

            //12 seconds until we should cast this agian
            KnockAway_Timer = 12000;
        }else KnockAway_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_theravenian(Creature *_Creature)
{
    return new boss_theravenianAI (_Creature);
}


void AddSC_boss_theravenian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_the_ravenian";
    newscript->GetAI = GetAI_boss_theravenian;
    m_scripts[nrscripts++] = newscript;
}
