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

//STATUS: bit unsure about this guy, probably ready for SVN

#define SPELL_DEMONARMOR    13787
#define SPELL_FROSTBOLT    12675
#define SPELL_ARCANEBOLT    15254
#define SPELL_ARCANEMISSILES    8418
#define SPELL_ARCANEEXPLOSION    13745

struct MANGOS_DLL_DECL mob_skeletal_guardianAI : public ScriptedAI
{
    mob_skeletal_guardianAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Frostbolt_Timer;
    uint32 ArcaneBolt_Timer;
    uint32 ArcaneMissiles_Timer;
    uint32 ArcaneExplosion_Timer;
    bool HasBuffed;
    bool InCombat;

    void EnterEvadeMode()
    {
        Frostbolt_Timer = 1000;
        ArcaneBolt_Timer = 12000;
        ArcaneMissiles_Timer = 22000;
        ArcaneExplosion_Timer = 10000;
        HasBuffed = false;
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!HasBuffed)
                {
                    m_creature->CastSpell(m_creature,SPELL_DEMONARMOR,true);
                    HasBuffed = true;
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

            //Frostbolt
            if (Frostbolt_Timer < diff)
            {
                //Cast
                if (rand()%100 < 90) //90% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_FROSTBOLT);
                }
                //6 seconds until we should cast this again
                Frostbolt_Timer = 6000;
            }else Frostbolt_Timer -= diff;

            //ArcaneBolt
            if (ArcaneBolt_Timer < diff)
            {
                //Cast
                if (rand()%100 < 45) //45% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_ARCANEBOLT);
                }
                //10 seconds until we should try cast this again
                ArcaneBolt_Timer = 10000;
            }else ArcaneBolt_Timer -= diff;

            //ArcaneMissiles
            if (ArcaneMissiles_Timer < diff)
            {
                //Cast
                if (rand()%100 < 35) //35% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_ARCANEMISSILES);
                }
                //13 seconds until we should try cast this again
                ArcaneMissiles_Timer = 13000;
            }else ArcaneMissiles_Timer -= diff;

            //ArcaneExplosion
            if (ArcaneExplosion_Timer < diff)
            {
                //Cast
                if (rand()%100 < 55) //55% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_ARCANEEXPLOSION);
                }
                //16 seconds until we should try cast this again
                ArcaneExplosion_Timer = 16000;
            }else ArcaneExplosion_Timer -= diff;

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
CreatureAI* GetAI_mob_skeletal_guardian(Creature *_Creature)
{
    return new mob_skeletal_guardianAI (_Creature);
}


void AddSC_mob_skeletal_guardian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_skeletal_guardian";
    newscript->GetAI = GetAI_mob_skeletal_guardian;
    m_scripts[nrscripts++] = newscript;
}