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
    
#define SPELL_SHADOWBOLT                17483         //Not sure if right ID            
#define SPELL_MANABURN            10876
#define SPELL_SHADOWSHIELD               22417
#define SPELL_STRIKE               15580

     

      
struct MANGOS_DLL_DECL boss_haterelAI : public ScriptedAI
{
    boss_haterelAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ShadowBolt_Timer;
    uint32 ManaBurn_Timer;
    uint32 ShadowShield_Timer;
    uint32 Strike_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        ShadowBolt_Timer = 15000;
        ManaBurn_Timer = 3000;
        ShadowShield_Timer = 8000;
        Strike_Timer = 12000;
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
                 //Cast Shadow Bolt on a Random target
                 Unit* target = NULL;
 
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_SHADOWBOLT);

                //10 seconds until we should cast this agian
                ShadowBolt_Timer = 7000;
            }else ShadowBolt_Timer -= diff;

            //ManaBurn_Timer
            if (ManaBurn_Timer < diff)
            {
                 //Cast Mana Burn on a Random target
                 Unit* target = NULL;

                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_MANABURN);

                //14 seconds until we should cast this agian
                ManaBurn_Timer = 13000;
            }else ManaBurn_Timer -= diff;


            //ShadowShield_Timer
            if (ShadowShield_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_SHADOWSHIELD);

                //25 seconds
               ShadowShield_Timer = 25000;
            }else ShadowShield_Timer -= diff;

            //Strike_Timer
            if (Strike_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_STRIKE);

                //10 seconds
               Strike_Timer = 10000;
            }else Strike_Timer -= diff;



            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DIST))
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
CreatureAI* GetAI_boss_haterel(Creature *_Creature)
{
    return new boss_haterelAI (_Creature);
}


void AddSC_boss_haterel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_haterel";
    newscript->GetAI = GetAI_boss_haterel;
    m_scripts[nrscripts++] = newscript;
}
