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


// Adds NYI

#define SPELL_MAGMASPLASH               13879      
#define SPELL_PYROBLAST                 20228
#define SPELL_EARTHQUAKE                19798

struct MANGOS_DLL_DECL boss_golemaggAI : public ScriptedAI
{
    boss_golemaggAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Pyroblast_Timer;
    uint32 EarthQuake_Timer;
    bool InCombat;
    bool HasAura;

    void EnterEvadeMode()
    {
        Pyroblast_Timer = 7000;      //These times are probably wrong
        EarthQuake_Timer = 0;     
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->CastSpell(m_creature,SPELL_MAGMASPLASH,true);
        HasAura = true;
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
				
				if (!HasAura)
                {
                    m_creature->CastSpell(m_creature,SPELL_MAGMASPLASH,true);
                    HasAura = true;
                }

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
            
            //Pyroblast_Timer
            if (Pyroblast_Timer < diff)
            {
                //Cast
                Unit* target = NULL;

                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_PYROBLAST);

                //7 seconds until we should cast this agian
                Pyroblast_Timer = 7000;
            }else Pyroblast_Timer -= diff;

            //EarthQuake_Timer
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 11 )
            {
                if (EarthQuake_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_EARTHQUAKE);

                    //12 seconds
                    EarthQuake_Timer = 12000;
                }else EarthQuake_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_boss_golemagg(Creature *_Creature)
{
    return new boss_golemaggAI (_Creature);
}


void AddSC_boss_golemagg()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_golemagg";
    newscript->GetAI = GetAI_boss_golemagg;
    m_scripts[nrscripts++] = newscript;
}