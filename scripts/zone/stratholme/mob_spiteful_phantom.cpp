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
#define SPELL_CHILLINGTOUCH    12531
#define SPELL_DEMORALIZINGSHOUT    23511
#define SPELL_HAUNTINGPHANTOMS    16336

struct MANGOS_DLL_DECL mob_spiteful_phantomAI : public ScriptedAI
{
    mob_spiteful_phantomAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Dazed_Timer;
    uint32 ChillingTouch_Timer;
    uint32 DemoralizingShout_Timer;
    uint32 HauntingPhantoms_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Dazed_Timer = 12000;
        ChillingTouch_Timer = 2000;
        DemoralizingShout_Timer = 6000;
        HauntingPhantoms_Timer = 8000;
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
                if (rand()%100 < 20) //20% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //12 seconds until we should try cast this again
                Dazed_Timer = 12000;
            }else Dazed_Timer -= diff;

            //ChillingTouch
            if (ChillingTouch_Timer < diff)
            {
                //Cast
                if (rand()%100 < 65) //65% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CHILLINGTOUCH);
                }
                //14 seconds until we should cast this again
                ChillingTouch_Timer = 14000;
            }else ChillingTouch_Timer -= diff;

            //DemoralizingShout_Timer
            if (DemoralizingShout_Timer < diff)
            {
                //Cast
                if (rand()%100 < 10) //10% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DEMORALIZINGSHOUT);
                }
                //20 seconds until we should try cast this agian
                DemoralizingShout_Timer = 20000;
            }else DemoralizingShout_Timer -= diff;

            //HauntingPhantoms
            if (HauntingPhantoms_Timer < diff)
            {
                if (rand()%100 < 15) //15% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_HAUNTINGPHANTOMS);
                }
                //24 seconds until we should try cast this again
                HauntingPhantoms_Timer = 24000;
            }else HauntingPhantoms_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_spiteful_phantom(Creature *_Creature)
{
    return new mob_spiteful_phantomAI (_Creature);
}


void AddSC_mob_spiteful_phantom()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_spiteful_phantom";
    newscript->GetAI = GetAI_mob_spiteful_phantom;
    m_scripts[nrscripts++] = newscript;
}
