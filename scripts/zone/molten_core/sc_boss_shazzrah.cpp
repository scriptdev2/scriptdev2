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
// Teleporting NYI

#define SPELL_ARCANEEXPLOSION           19712
#define SPELL_SHAZZRAHCURSE             19713
#define SPELL_DEADENMAGIC               19714
#define SPELL_COUNTERSPELL              19715

struct MANGOS_DLL_DECL boss_shazzrahAI : public ScriptedAI
{
    boss_shazzrahAI(Creature *c) : ScriptedAI(c) {Reset();}

    Unit *pTarget;
    uint32 ArcaneExplosion_Timer;
    uint32 ShazzrahCurse_Timer;
    uint32 DeadenMagic_Timer;
    uint32 Countspell_Timer;

    void Reset()
    {
        pTarget = NULL;
        ArcaneExplosion_Timer = 6000;      //These times are probably wrong
        ShazzrahCurse_Timer = 30000;
        DeadenMagic_Timer = 45000;
        Countspell_Timer = 20000;

        if (m_creature)
        {
            EnterEvadeMode();
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (m_creature->getVictim() == NULL && who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            pTarget = who;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);

                pTarget = who;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //If we had a target and it wasn't cleared then it means the target died from some unknown soruce
        //But we still need to reset
        if ((!m_creature->SelectHostilTarget() || !m_creature->getVictim()) && pTarget)
        {
            Reset();
            return;
        }

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Check if we should stop attacking because our victim is no longer attackable
            if (needToStop())
            {
                Reset();
                return;
            }
            
            //ArcaneExplosion_Timer
            if (ArcaneExplosion_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_ARCANEEXPLOSION);

                //6 seconds until we should cast this agian
                ArcaneExplosion_Timer = 6000;
            }else ArcaneExplosion_Timer -= diff;

            //ShazzrahCurse_Timer
            if (ShazzrahCurse_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SHAZZRAHCURSE);

                //30 seconds until we should cast this agian
                ShazzrahCurse_Timer = 30000;
            }else ShazzrahCurse_Timer -= diff;

            //DeadenMagic_Timer
            if (DeadenMagic_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_DEADENMAGIC);

                //45 seconds until we should cast this agian
                DeadenMagic_Timer = 45000;
            }else DeadenMagic_Timer -= diff;

            //Countspell_Timer
            if (Countspell_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_COUNTERSPELL);

                //20 seconds until we should cast this agian
                Countspell_Timer = 20000;
            }else Countspell_Timer -= diff;

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
CreatureAI* GetAI_boss_shazzrah(Creature *_Creature)
{
    return new boss_shazzrahAI (_Creature);
}


void AddSC_boss_shazzrah()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_shazzrah";
    newscript->GetAI = GetAI_boss_shazzrah;
    m_scripts[nrscripts++] = newscript;
}