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

//Status: Ready for SVN. Probably has to be revised when the egan's blaster is supported in core.

#define SAY_ZAPPED0 "Thanks to Egan"
#define SAY_ZAPPED1 "Rivendare must die"
#define SAY_ZAPPED2 "Who you gonna call?"
#define SAY_ZAPPED3 "Don't cross those beams!"
//Possibly more of these quotes around. Say OnDie

//#define SPELL_SUMMONFREEDSOUL    17408 //might be controlled by blaster item, deactivated for now.

struct MANGOS_DLL_DECL mob_restless_soulAI : public ScriptedAI
{
    mob_restless_soulAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    bool InCombat;

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void JustDied(Unit* Victim)
    {
        switch (rand()%4)
        {
        case 0:
            DoSay(SAY_ZAPPED0,LANG_UNIVERSAL,NULL);
            break;
        case 1:
            DoSay(SAY_ZAPPED1,LANG_UNIVERSAL,NULL);
            break;
        case 2:
            DoSay(SAY_ZAPPED2,LANG_UNIVERSAL,NULL);
            break;
        case 3:
            DoSay(SAY_ZAPPED3,LANG_UNIVERSAL,NULL);
            break;
        }
        //m_creature->CastSpell(m_creature,SPELL_SUMMONFREEDSOUL,true);
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
CreatureAI* GetAI_mob_restless_soul(Creature *_Creature)
{
    return new mob_restless_soulAI (_Creature);
}


void AddSC_mob_restless_soul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_restless_soul";
    newscript->GetAI = GetAI_mob_restless_soul;
    m_scripts[nrscripts++] = newscript;
}