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

//Status: not too sure about the KnockAway (but i do know he's using some spell knocking back enemy) send to SVN

#define SPELL_SHIELDCHARGE    15749
#define SPELL_SHIELDBASH    33871
#define SPELL_DISARM    11879
#define SPELL_KNOCKAWAY    10101

struct MANGOS_DLL_DECL mob_crimson_guardsmanAI : public ScriptedAI
{
    mob_crimson_guardsmanAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ShieldBash_Timer;
    uint32 Disarm_Timer;
    uint32 KnockAway_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ShieldBash_Timer = 2000;
        Disarm_Timer = 10000;
        KnockAway_Timer = 19000;
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

                if (!InCombat)
                {
                    m_creature->CastSpell(m_creature->getVictim(),SPELL_SHIELDCHARGE,true);
                }

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

            //ShieldBash
            if (ShieldBash_Timer < diff)
            {
                //Cast
                if (rand()%100 < 85) //85% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_SHIELDBASH);
                }
                //9 seconds until we should cast this again
                ShieldBash_Timer = 9000;
            }else ShieldBash_Timer -= diff;

            //Disarm
            if (Disarm_Timer < diff)
            {
                //Cast
                if (rand()%100 < 90) //90% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DISARM);
                }
                //15 seconds until we should cast this again
                Disarm_Timer = 15000;
            }else Disarm_Timer -= diff;

            //KnockAway
            if (KnockAway_Timer < diff)
            {
                //Cast
                if (rand()%100 < 90) //90% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_KNOCKAWAY);
                }
                //19 seconds until we should cast this again
                KnockAway_Timer = 19000;
            }else KnockAway_Timer -= diff;

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
CreatureAI* GetAI_mob_crimson_guardsman(Creature *_Creature)
{
    return new mob_crimson_guardsmanAI (_Creature);
}


void AddSC_mob_crimson_guardsman()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crimson_guardsman";
    newscript->GetAI = GetAI_mob_crimson_guardsman;
    m_scripts[nrscripts++] = newscript;
}