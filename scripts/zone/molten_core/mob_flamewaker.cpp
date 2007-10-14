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

#define SPELL_FISTOFRAGNAROS               20277  	
#define SPELL_SUNDERARMOR                  21081                     
#define SPELL_STRIKE                   19730


struct MANGOS_DLL_DECL mob_flamewakerAI : public ScriptedAI
{
    mob_flamewakerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FistOfRagnaros_Timer;
    uint32 SunderArmor_Timer;
    uint32 Strike_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        FistOfRagnaros_Timer = 12000;
        SunderArmor_Timer = 5000;
        Strike_Timer = 15000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FIRE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;


        //FistOfRagnaros_Timer
        if (FistOfRagnaros_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_FISTOFRAGNAROS);

            //12 seconds until we should cast this again
            FistOfRagnaros_Timer = 12000;
        }else FistOfRagnaros_Timer -= diff;


        //SunderArmor_Timer
        if (SunderArmor_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SUNDERARMOR);
            //14 seconds until we should cast this agian
            SunderArmor_Timer = 10000 + rand()%8000;
        }else SunderArmor_Timer -= diff;

        //Strike_Timer
        if (Strike_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_STRIKE);
            //7-12 seconds until we should cast this again
            Strike_Timer = 7000 + rand()%5000;
        }else Strike_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_mob_flamewaker(Creature *_Creature)
{
    return new mob_flamewakerAI (_Creature);
}


void AddSC_mob_flamewaker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_flamewaker";
    newscript->GetAI = GetAI_mob_flamewaker;
    m_scripts[nrscripts++] = newscript;
}
