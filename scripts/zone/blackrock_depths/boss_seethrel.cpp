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


#define SPELL_FROSTBOLT                16799
#define SPELL_FROSTARMOR             15784       //This is actually a buff he gives himself
#define SPELL_BLIZZARD           19099
#define SPELL_FROSTNOVA                 15063
#define SPELL_FROSTWARD              15004       
     

struct MANGOS_DLL_DECL boss_seethrelAI : public ScriptedAI
{
    boss_seethrelAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FrostArmor_Timer;
    uint32 Frostbolt_Timer;
    uint32 Blizzard_Timer;
    uint32 FrostNova_Timer;
    uint32 FrostWard_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        FrostArmor_Timer = 2000;
	Frostbolt_Timer = 6000;
        Blizzard_Timer = 18000;
        FrostNova_Timer = 12000;
        FrostWard_Timer = 25000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->CastSpell(m_creature,SPELL_FROSTARMOR,true);
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
     
            //FrostArmor_Timer
            if (FrostArmor_Timer < diff)
            {
                //Cast
                DoCast(m_creature, SPELL_FROSTARMOR);

                //180 seconds
                FrostArmor_Timer = 180000;
            }else FrostArmor_Timer -= diff;


       
            //Frostbolt_Timer
            if (Frostbolt_Timer < diff)
            {
   
                DoCast(m_creature->getVictim(),SPELL_FROSTBOLT);

                //15 seconds
                Frostbolt_Timer = 15000;
            }else Frostbolt_Timer -= diff;

            //Blizzard_Timer
            if (Blizzard_Timer < diff)
            {
                 //Cast Blizzard on a Random target
                 Unit* target = NULL;
 
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_BLIZZARD);

                //22 seconds until we should cast this agian
                Blizzard_Timer = 22000;
            }else Blizzard_Timer -= diff;


            //FrostNova_Timer
            if (FrostNova_Timer < diff)
            {
                
                DoCast(m_creature->getVictim(),SPELL_FROSTNOVA);

                //14 seconds until we should cast this agian
                FrostNova_Timer = 14000;
            }else FrostNova_Timer -= diff;

            //FrostWard_Timer
            if (FrostWard_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_FROSTWARD);

                //68 seconds until we should cast this agian
                FrostWard_Timer = 68000;
            }else FrostWard_Timer -= diff;

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
CreatureAI* GetAI_boss_seethrel(Creature *_Creature)
{
    return new boss_seethrelAI (_Creature);
}


void AddSC_boss_seethrel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_seethrel";
    newscript->GetAI = GetAI_boss_seethrel;
    m_scripts[nrscripts++] = newscript;
}
