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

//*** NEEDS ADDITIOINAL REVIEW ***

//Razorgore Phase 2 Script
//Phase 1 script with adds NYI

#include "../../sc_defines.h"

#define SAY_NPC_DEATH "If I fall into the abyss I'll take all of you mortals with me..."
#define SOUND_NPC_DEATH 8278
#define SAY_EGGS_BREAK3 "No! Not another one! I'll have your heads for this atrocity. "
#define SOUND_EGGS_BREAK3 8277

#define SPELL_CLEAVE                20691
#define SPELL_WARSTOMP              27758       //Warstomp causes 1k dmg but doesn't knockback
#define SPELL_FIREBALLVOLLEY        29958       //Our fireball doesn't leave a dot
#define SPELL_CONFLAGRATION         16805

struct MANGOS_DLL_DECL boss_razorgoreAI : public ScriptedAI
{
    boss_razorgoreAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Cleave_Timer;
    uint32 WarStomp_Timer;
    uint32 FireballVolley_Timer;
    uint32 Conflagration_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Cleave_Timer = 15000;      //These times are probably wrong
        WarStomp_Timer = 35000;
        FireballVolley_Timer = 20000;
        Conflagration_Timer = 30000;
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
            if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
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
            
            //Cleave_Timer
            if (Cleave_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CLEAVE);

                //15 seconds until we should cast this agian
                Cleave_Timer = 15000;
            }else Cleave_Timer -= diff;

            //WarStomp_Timer
            if (WarStomp_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_WARSTOMP);

                //35 seconds until we should cast this agian
                WarStomp_Timer = 35000;
            }else WarStomp_Timer -= diff;
            
            //FireballVolley_Timer
            if (FireballVolley_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FIREBALLVOLLEY);

                //20 seconds until we should cast this agian
                FireballVolley_Timer = 20000;
            }else FireballVolley_Timer -= diff;
            
                        //Conflagration_Timer
            if (Conflagration_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CONFLAGRATION);

                //30 seconds until we should cast this agian
                Conflagration_Timer = 30000;
            }else Conflagration_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
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
CreatureAI* GetAI_boss_razorgore(Creature *_Creature)
{
    return new boss_razorgoreAI (_Creature);
}


void AddSC_boss_razorgore()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_razorgore";
    newscript->GetAI = GetAI_boss_razorgore;
    m_scripts[nrscripts++] = newscript;
}