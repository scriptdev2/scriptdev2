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
SDName: Boss_Baron_Geddon
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"


#define SPELL_INFERNO               19695
#define SPELL_IGNITEMANA            19659
#define SPELL_LIVINGBOMB            20475
#define SPELL_ARMAGEDDOM            20479

struct MANGOS_DLL_DECL boss_baron_geddonAI : public ScriptedAI
{
    boss_baron_geddonAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Inferno_Timer;
    uint32 IgniteMana_Timer;
    uint32 LivingBomb_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Inferno_Timer = 45000;      //These times are probably wrong
        IgniteMana_Timer = 30000;
        LivingBomb_Timer = 35000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
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


        //If we are <2% hp cast Armageddom
        if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 2 && !m_creature->IsNonMeleeSpellCasted(false))
        {
            DoCast(m_creature,SPELL_ARMAGEDDOM);
            DoTextEmote("performs one last service for Ragnaros.",NULL);
            return;
        }

        //Inferno_Timer
        if (Inferno_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_INFERNO);

            //7 seconds until we should cast this agian
            Inferno_Timer = 45000;
        }else Inferno_Timer -= diff;

        //IgniteMana_Timer
        if (IgniteMana_Timer < diff)
        {
            //Cast
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            if (target)DoCast(target,SPELL_IGNITEMANA);

            //35 seconds until we should cast this agian
            IgniteMana_Timer = 30000;
        }else IgniteMana_Timer -= diff;

        //LivingBomb_Timer
        if (LivingBomb_Timer < diff)
        {
            //Cast
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_LIVINGBOMB);

            //30 seconds until we should cast this agian
            LivingBomb_Timer = 35000;
        }else LivingBomb_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_baron_geddon(Creature *_Creature)
{
    return new boss_baron_geddonAI (_Creature);
}


void AddSC_boss_baron_geddon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_baron_geddon";
    newscript->GetAI = GetAI_boss_baron_geddon;
    m_scripts[nrscripts++] = newscript;
}
