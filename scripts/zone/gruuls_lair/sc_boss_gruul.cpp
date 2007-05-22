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

#define SPELL_GROWTH            36300
#define SPELL_CAVEIN            36240
#define SPELL_GROUNDSLAM        33525
#define SPELL_SHATTER           33671
#define SPELL_HURTFULSTRIKE     33813
#define SPELL_REVERBERATION     36297

struct MANGOS_DLL_DECL boss_gruulAI : public ScriptedAI
{
    boss_gruulAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    Unit *pTarget;
    uint32 Growth_Timer;
    uint32 CaveIn_Timer;
    uint32 GroundSlam_Timer;
    uint32 HurtfuStrike_Timer;
    uint32 Reverberation_Timer;

    void EnterEvadeMode()
    {
        pTarget = NULL;
        Growth_Timer= 30000;
        CaveIn_Timer= 35000;
        GroundSlam_Timer= 45000;;
        HurtfuStrike_Timer= 20000;
        Reverberation_Timer= 25000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            //Begin attack
            DoStartMeleeAttack(who);
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

                //Begin attack
                DoStartMeleeAttack(who);
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
            //Check if we should stop attacking because our victim is no longer attackable
            if (needToStop())
            {
                EnterEvadeMode();
                return;
            }

            //WHIRLWIND_Timer (only in phase2)
            if (Growth_Timer < diff)
            {
                DoCast(m_creature,SPELL_GROWTH);
                Growth_Timer = 30000;
            }else Growth_Timer -= diff;

            if (CaveIn_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_CAVEIN);
                CaveIn_Timer =35000;
            }else CaveIn_Timer -= diff;

            if (GroundSlam_Timer < diff)
            {
                DoCast(m_creature,SPELL_GROUNDSLAM);
                GroundSlam_Timer =45000;
            }else GroundSlam_Timer -= diff;

            if (HurtfuStrike_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_TOPAGGRO,0);
                if (target)DoCast(target,SPELL_HURTFULSTRIKE);

                HurtfuStrike_Timer= 20000;
            }else HurtfuStrike_Timer -= diff;

            if (Reverberation_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_REVERBERATION);

                Reverberation_Timer = 20000;
            }else Reverberation_Timer -= diff;

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

CreatureAI* GetAI_boss_gruul(Creature *_Creature)
{
    return new boss_gruulAI (_Creature);
}


void AddSC_boss_gruul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gruul";
    newscript->GetAI = GetAI_boss_gruul;
    m_scripts[nrscripts++] = newscript;
}