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

//Status: unsure if he should have a water elemental. Ready for SVN

#define SPELL_ARCANEEXPLOSION    10202
#define SPELL_BLASTWAVE    13020
#define SPELL_IMMOLATE    11667

struct MANGOS_DLL_DECL mob_crimson_battle_mageAI : public ScriptedAI
{
    mob_crimson_battle_mageAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ArcaneExplosion_Timer;
    uint32 BlastWave_Timer;
    uint32 Immolate_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ArcaneExplosion_Timer = 18000;
        BlastWave_Timer = 10000;
        Immolate_Timer = 2000;
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

            //ArcaneExplosion
            if (ArcaneExplosion_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_ARCANEEXPLOSION);

                //27 seconds until we should cast this again
                ArcaneExplosion_Timer = 27000;
            }else ArcaneExplosion_Timer -= diff;

            //BlastWave
            if (BlastWave_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BLASTWAVE);

                //21 seconds until we should cast this again
                BlastWave_Timer = 21000;
            }else BlastWave_Timer -= diff;

            //Immolate
            if (Immolate_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_IMMOLATE);

                //23 seconds until we should cast this again
                Immolate_Timer = 23000;
            }else Immolate_Timer -= diff;

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
CreatureAI* GetAI_mob_crimson_battle_mage(Creature *_Creature)
{
    return new mob_crimson_battle_mageAI (_Creature);
}


void AddSC_mob_crimson_battle_mage()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crimson_battle_mage";
    newscript->GetAI = GetAI_mob_crimson_battle_mage;
    m_scripts[nrscripts++] = newscript;
}