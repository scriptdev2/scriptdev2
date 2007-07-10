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



#define SPELL_DAZED    1604
#define SPELL_FROSTBOLT    11538
#define SPELL_DEMORALIZINGSHOUT    16244
#define SPELL_CADAVERWORMS    16143

struct MANGOS_DLL_DECL mob_mangled_cadaverAI : public ScriptedAI
{
    mob_mangled_cadaverAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Dazed_Timer;
    uint32 DemoralizingShout_Timer;
    uint32 CadaverWorms_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Dazed_Timer = 3000;
        DemoralizingShout_Timer = 10000;
        CadaverWorms_Timer = 11000;
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

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 80) //80% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //14 seconds until we should try cast this again
                Dazed_Timer = 14000;
            }else Dazed_Timer -= diff;

            //DemoralizingShout
            if (DemoralizingShout_Timer < diff)
            {
                if (rand()%100 < 65) //65% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_DEMORALIZINGSHOUT);
                }
                //19 seconds until we should try cast this again
                DemoralizingShout_Timer = 19000;
            }else DemoralizingShout_Timer -= diff;

            //CadaverWorms
            if (CadaverWorms_Timer < diff)
            {
                //Cast
                if (rand()%100 < 25) //25% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CADAVERWORMS);
                }
                //21 seconds until we should try cast this again
                CadaverWorms_Timer = 21000;
            }else CadaverWorms_Timer -= diff;

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
CreatureAI* GetAI_mob_mangled_cadaver(Creature *_Creature)
{
    return new mob_mangled_cadaverAI (_Creature);
}


void AddSC_mob_mangled_cadaver()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_mangled_cadaver";
    newscript->GetAI = GetAI_mob_mangled_cadaver;
    m_scripts[nrscripts++] = newscript;
}