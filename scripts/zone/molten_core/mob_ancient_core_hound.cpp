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

#define SPELL_GROUNDSTOMP                19364	
#define SPELL_ANCIENTDREAD                   19365                     
#define SPELL_CAUTERIZINGFLAMES                   19366 
#define SPELL_WITHERINGHEAT                   19367 
#define SPELL_ANCIENTDESPAIR                   19369 
#define SPELL_CONEOFFIRE                   19630                

    

struct MANGOS_DLL_DECL mob_ancient_core_houndAI : public ScriptedAI
{
    mob_ancient_core_houndAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Debuff_Timer;
    uint32 ConeOfFire_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Debuff_Timer = 10000;
        ConeOfFire_Timer = 7000;
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
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
        
            if (Debuff_Timer < diff)
            {
                    switch(rand()%5)
                    {
                        case 0:
                            DoCast(m_creature->getVictim(),SPELL_GROUNDSTOMP);
                            break;
                        case 1:
                            DoCast(m_creature->getVictim(),SPELL_ANCIENTDREAD);
                            break;
                        case 2:
                            DoCast(m_creature->getVictim(),SPELL_CAUTERIZINGFLAMES);
                            break;
                        case 3:
                            DoCast(m_creature->getVictim(),SPELL_WITHERINGHEAT);
                            break;
                        case 4:
                            DoCast(m_creature->getVictim(),SPELL_ANCIENTDESPAIR);
                            break;
                    }
                Debuff_Timer = 24000;
            }else Debuff_Timer -= diff;


            //ConeOfFire_Timer
            if (ConeOfFire_Timer < diff)
            {

                    DoCast(m_creature->getVictim(),SPELL_CONEOFFIRE);
                
                //7 seconds until we should cast this agian
                ConeOfFire_Timer = 7000;
            }else ConeOfFire_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_ancient_core_hound(Creature *_Creature)
{
    return new mob_ancient_core_houndAI (_Creature);
}


void AddSC_mob_ancient_core_hound()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_ancient_core_hound";
    newscript->GetAI = GetAI_mob_ancient_core_hound;
    m_scripts[nrscripts++] = newscript;
}
