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

//Status: ready for SVN

#define SPELL_ARCANEBOLT    15254
#define SPELL_CHILLED   16927
#define SPELL_POLYMORPH    15534
#define SPELL_FIREBLAST    16144

struct MANGOS_DLL_DECL mob_crimson_sorcererAI : public ScriptedAI
{
    mob_crimson_sorcererAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ArcaneBolt_Timer;
    uint32 Chilled_Timer;
    uint32 Polymorph_Timer;
    uint32 FireBlast_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ArcaneBolt_Timer = 1000;
        Chilled_Timer = 7000;
        Polymorph_Timer = 38000;
        FireBlast_Timer = 26000;
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

            //ArcaneBolt
            if (ArcaneBolt_Timer < diff)
            {
                //Cast
                if (rand()%100 < 90) //90% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_ARCANEBOLT);
                }
                //4 seconds until we should cast this again
                ArcaneBolt_Timer = 4000;
            }else ArcaneBolt_Timer -= diff;

            //Chilled
            if (Chilled_Timer < diff)
            {
                //Cast
                if (rand()%100 < 65) //65% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CHILLED);
                }
                //10 seconds until we should cast this again
                Chilled_Timer = 10000;
            }else Chilled_Timer -= diff;

            //Polymorph
            if (Polymorph_Timer < diff)
            {
                //Cast
                if (rand()%100 < 65) //65% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_POLYMORPH);
                }
                //15 seconds until we should cast this again
                Polymorph_Timer = 15000;
            }else Polymorph_Timer -= diff;

            //FireBlast
            if (FireBlast_Timer < diff)
            {
                //Cast
                if (rand()%100 < 50) //50% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_FIREBLAST);
                }
                //19 seconds until we should cast this again
                FireBlast_Timer = 19000;
            }else FireBlast_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_crimson_sorcerer(Creature *_Creature)
{
    return new mob_crimson_sorcererAI (_Creature);
}


void AddSC_mob_crimson_sorcerer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crimson_sorcerer";
    newscript->GetAI = GetAI_mob_crimson_sorcerer;
    m_scripts[nrscripts++] = newscript;
}
