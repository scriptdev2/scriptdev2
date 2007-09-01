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

#define SPELL_MAGICREFLECTION       20619
#define SPELL_DAMAGEREFLECTION      21075
#define SPELL_BLASTWAVE             20229


struct MANGOS_DLL_DECL mob_flamewaker_eliteAI : public ScriptedAI
{
    mob_flamewaker_eliteAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 MagicReflection_Timer;
    uint32 DamageReflection_Timer;
    uint32 Blastwave_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        MagicReflection_Timer = 60000;      //Damage reflection first so we alternate
        DamageReflection_Timer = 30000;
        Blastwave_Timer = 12000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FIRE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BLEED, true);
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
            
            //MagicReflection_Timer
            if (MagicReflection_Timer < diff)
            {
                //Cast NYI in core
                //DoCast(m_creature,SPELL_MAGICREFLECTION);

                //60 seconds until we should cast this agian
                MagicReflection_Timer = 60000;
            }else MagicReflection_Timer -= diff;

            //DamageReflection_Timer
            if (DamageReflection_Timer < diff)
            {
                //Cast NYI in core
                //DoCast(m_creature,SPELL_DAMAGEREFLECTION);

                //60 seconds until we should cast this agian
                DamageReflection_Timer = 60000;
            }else DamageReflection_Timer -= diff;

            //Blastwave_Timer
            if (Blastwave_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BLASTWAVE);

                //12 seconds until we should cast this agian
                Blastwave_Timer = 12000;
            }else Blastwave_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_flamewaker_elite(Creature *_Creature)
{
    return new mob_flamewaker_eliteAI (_Creature);
}


void AddSC_mob_flamewaker_elite()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_flamewaker_elite";
    newscript->GetAI = GetAI_mob_flamewaker_elite;
    m_scripts[nrscripts++] = newscript;
}
