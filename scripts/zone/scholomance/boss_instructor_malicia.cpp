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
SDName: Boss_Jandice_Barov
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_CALLOFGRAVES         17831
#define SPELL_CORRUPTION           11672
#define SPELL_FLASHHEAL            10917
#define SPELL_RENEW                10929
#define SPELL_HEALINGTOUCH         9889

struct MANGOS_DLL_DECL boss_instructormaliciaAI : public ScriptedAI
{
    boss_instructormaliciaAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 CallOfGraves_Timer;
    uint32 Corruption_Timer;
    uint32 FlashHeal_Timer;
    uint32 Renew_Timer;
    uint32 HealingTouch_Timer;
    uint32 FlashCounter;
    uint32 TouchCounter;

    bool InCombat;

    void EnterEvadeMode()
    {       
        CallOfGraves_Timer = 4000;
        Corruption_Timer = 8000;
        FlashHeal_Timer = 38000;
        Renew_Timer = 32000;
        HealingTouch_Timer = 45000;
        FlashCounter = 0;
        TouchCounter = 0;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SHACKLE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);     
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
    }

    void JustDied(Unit *killer)
    {
        ScriptedInstance *pInstance = (m_creature->GetInstanceData()) ? ((ScriptedInstance*)m_creature->GetInstanceData()) : NULL;
        if(pInstance)
        {
            pInstance->SetData("InstructorMalicia_Death", 0);

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

        //CallOfGraves_Timer
        if (CallOfGraves_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CALLOFGRAVES);

            //65 seconds
            CallOfGraves_Timer = 65000;
        }else CallOfGraves_Timer -= diff;

        //Corruption_Timer
        if (Corruption_Timer < diff)
        {
            //Cast VoidBolt on a Random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_CORRUPTION);

            //20 seconds until we should cast this agian
            Corruption_Timer = 24000;
        }else Corruption_Timer -= diff;


        //Renew_Timer
        if (Renew_Timer < diff)
        {
            //Cast
            DoCast(m_creature, SPELL_RENEW);

            //10 seconds until we should cast this agian
            Renew_Timer = 10000;
        }else Renew_Timer -= diff;


        //FlashHeal_Timer
        if (FlashHeal_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_FLASHHEAL);

            //5 Flashheals will be casted
            if (FlashCounter < 2)
            {
                FlashHeal_Timer = 5000;
                FlashCounter++;
            }
            else {
                FlashCounter=0;
                //30 seconds until we should cast this again
                FlashHeal_Timer = 30000;
            }

        }else FlashHeal_Timer -= diff;

        //HealingTouch_Timer
        if (HealingTouch_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_HEALINGTOUCH);

            //3 Healingtouchs will be casted
            if (HealingTouch_Timer < 2)
            {
                HealingTouch_Timer = 5500;
                TouchCounter++;
            }
            else {
                TouchCounter=0;
                //30 seconds until we should cast this again
                HealingTouch_Timer = 30000;
            }

        }else HealingTouch_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_instructormalicia(Creature *_Creature)
{
    return new boss_instructormaliciaAI (_Creature);
}


void AddSC_boss_instructormalicia()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_instructor_malicia";
    newscript->GetAI = GetAI_boss_instructormalicia;
    m_scripts[nrscripts++] = newscript;
}
