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

#define SPELL_BANSHEEWAIL    16868
#define SPELL_DAZED    1604
#define SPELL_DEAFENINGSCREECH    3589

struct MANGOS_DLL_DECL mob_shrieking_bansheeAI : public ScriptedAI
{
    mob_shrieking_bansheeAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 BansheeWail_Timer;
    uint32 Dazed_Timer;
    uint32 DeafeningScreech_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        BansheeWail_Timer = 1000;
        Dazed_Timer = 12000;
        DeafeningScreech_Timer = 9000;
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

            //BansheeWail
            if (BansheeWail_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BANSHEEWAIL);
                //3.5 seconds until we should cast this again
                BansheeWail_Timer = 3500;
            }else BansheeWail_Timer -= diff;


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

            //DeafeningScreech
            if (DeafeningScreech_Timer < diff)
            {
                if (rand()%100 < 60) //60% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_DEAFENINGSCREECH);
                }
                //23 seconds until we should try cast this again
                DeafeningScreech_Timer = 23000;
            }else DeafeningScreech_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_shrieking_banshee(Creature *_Creature)
{
    return new mob_shrieking_bansheeAI (_Creature);
}


void AddSC_mob_shrieking_banshee()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_shrieking_banshee";
    newscript->GetAI = GetAI_mob_shrieking_banshee;
    m_scripts[nrscripts++] = newscript;
}
