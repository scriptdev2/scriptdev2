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

#define SPELL_GOBLINGUN             21910   // NOt sure about right id! 	
#define SPELL_BOMB                  22334              

#define SAY_AGGRO1      "Mine! Mine! Mine! Gizlock is the ruler of this domain! You shall never reveal my presence!"    

struct MANGOS_DLL_DECL boss_tinkerergizlockAI : public ScriptedAI
{
    boss_tinkerergizlockAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 GoblinGun_Timer;
    uint32 Bomb_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        GoblinGun_Timer = 6000;
        Bomb_Timer = 12000;
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
            DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
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

            //GoblinGun_Timer
            if (GoblinGun_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_GOBLINGUN);

                //15 seconds
                GoblinGun_Timer = 16000;
            }else GoblinGun_Timer -= diff;

            //Bomb_Timer
            if (Bomb_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BOMB);

                //17 seconds until we should cast this agian
                Bomb_Timer = 14000;
            }else Bomb_Timer -= diff;

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
CreatureAI* GetAI_boss_tinkerergizlock(Creature *_Creature)
{
    return new boss_tinkerergizlockAI (_Creature);
}


void AddSC_boss_tinkerergizlock()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_tinkerer_gizlock";
    newscript->GetAI = GetAI_boss_tinkerergizlock;
    m_scripts[nrscripts++] = newscript;
}
