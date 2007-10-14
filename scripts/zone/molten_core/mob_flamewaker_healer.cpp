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
#define SPELL_SHADOWBOLT             21077


struct MANGOS_DLL_DECL mob_flamewaker_healerAI : public ScriptedAI
{
    mob_flamewaker_healerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 MagicReflection_Timer;
    uint32 DamageReflection_Timer;
    uint32 ShadowBolt_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        MagicReflection_Timer = 60000;      //Damage reflection first so we alternate
        DamageReflection_Timer = 30000;
        ShadowBolt_Timer = 2000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

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

        //ShadowBolt_Timer
        if (ShadowBolt_Timer < diff)
        {

            //Cast Shadow Bolt on a Random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_SHADOWBOLT);

            //2 seconds until we should cast this agian
            ShadowBolt_Timer = 2000;
        }else ShadowBolt_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_mob_flamewaker_healer(Creature *_Creature)
{
    return new mob_flamewaker_healerAI (_Creature);
}


void AddSC_mob_flamewaker_healer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_flamewaker_healer";
    newscript->GetAI = GetAI_mob_flamewaker_healer;
    m_scripts[nrscripts++] = newscript;
}
