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


#define SPELL_SHADOWBOLT                17228  
#define SPELL_CURSEOFTONGUES               15470 
#define SPELL_CURSEOFWEAKNESS                17227            
#define SPELL_DEMONARMOR                11735
#define SPELL_ENVELOPINGWEB               15471

     

      
struct MANGOS_DLL_DECL boss_anubshiahAI : public ScriptedAI
{
    boss_anubshiahAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ShadowBolt_Timer;
    uint32 CurseOfTongues_Timer;
    uint32 CurseOfWeakness_Timer;
    uint32 DemonArmor_Timer;
    uint32 EnvelopingWeb_Timer;
    bool InCombat;


    void EnterEvadeMode()
    {       
        ShadowBolt_Timer = 7000;
        CurseOfTongues_Timer = 24000;
        CurseOfWeakness_Timer = 12000;
        DemonArmor_Timer = 3000;
	EnvelopingWeb_Timer = 16000;
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
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
            
            //ShadowBolt_Timer
            if (ShadowBolt_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SHADOWBOLT);

                //7 seconds
               ShadowBolt_Timer = 7000;
            }else ShadowBolt_Timer -= diff;

            //CurseOfTongues_Timer
            if (CurseOfTongues_Timer < diff)
            {
                 //Cast Curse Of Tongues on a Random target
                 Unit* target = NULL;
 
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_CURSEOFTONGUES);

                //18 seconds
               CurseOfTongues_Timer = 18000;
            }else CurseOfTongues_Timer -= diff;

            //CurseOfWeakness_Timer
            if (CurseOfWeakness_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CURSEOFWEAKNESS);

                //45 seconds
               CurseOfWeakness_Timer = 45000;
            }else CurseOfWeakness_Timer -= diff;

            //DemonArmor_Timer
            if (DemonArmor_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_DEMONARMOR);

                //5 minutes
               DemonArmor_Timer = 300000;
            }else DemonArmor_Timer -= diff;

            //EnvelopingWeb_Timer
            if (EnvelopingWeb_Timer < diff)
            {
                 //Cast EnvelopingWeb  on a Random target
                 Unit* target = NULL;
 
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_ENVELOPINGWEB);

                //12 seconds
               EnvelopingWeb_Timer = 12000;
            }else EnvelopingWeb_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
}; 
CreatureAI* GetAI_boss_anubshiah(Creature *_Creature)
{
    return new boss_anubshiahAI (_Creature);
}


void AddSC_boss_anubshiah()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_anubshiah";
    newscript->GetAI = GetAI_boss_anubshiah;
    m_scripts[nrscripts++] = newscript;
}
