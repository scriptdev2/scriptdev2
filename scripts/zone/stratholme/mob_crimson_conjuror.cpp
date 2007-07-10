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



#define SPELL_FROSTNOVA    22645
#define SPELL_FROSTBOLT    17503
#define SPELL_SCORCH    17195
#define SPELL_SUMMONWATERELEMENTAL    17162

struct MANGOS_DLL_DECL mob_crimson_conjurorAI : public ScriptedAI
{
    mob_crimson_conjurorAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Frostbolt_Timer;
    uint32 FrostNova_Timer;
    uint32 Scorch_Timer;
    uint32 SummonWaterElemental_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Frostbolt_Timer = 1000;
        FrostNova_Timer = 9000;
        Scorch_Timer = 16000;
        SummonWaterElemental_Timer = 13000;
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

            //Frostbolt
            if (Frostbolt_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FROSTBOLT);

                //5 seconds until we should cast this again
                Frostbolt_Timer = 5000;
            }else Frostbolt_Timer -= diff;

            //FrostNova
            if (FrostNova_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FROSTNOVA);
                //26 seconds until we should cast this again
                FrostNova_Timer = 26000;
            }else FrostNova_Timer -= diff;

            //Scorch
            if (Scorch_Timer < diff)
            {
                //Cast
                if (rand()%100 < 75) //75% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_SCORCH);
                }
                //17 seconds until we should cast this again
                Scorch_Timer = 17000;
            }else Scorch_Timer -= diff;

            //SummonWaterElemental
            if (SummonWaterElemental_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_SUMMONWATERELEMENTAL);
                //70 seconds until we should cast this again
                SummonWaterElemental_Timer = 70000;
            }else SummonWaterElemental_Timer -= diff;

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
CreatureAI* GetAI_mob_crimson_conjuror(Creature *_Creature)
{
    return new mob_crimson_conjurorAI (_Creature);
}


void AddSC_mob_crimson_conjuror()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crimson_conjuror";
    newscript->GetAI = GetAI_mob_crimson_conjuror;
    m_scripts[nrscripts++] = newscript;
}