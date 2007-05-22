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

//Status: Ready for SVN

#define SPELL_ENVELOPINGWEB    15471
#define SPELL_DAZED    1604
#define SPELL_DEAFENINGSCREECH    3589

struct MANGOS_DLL_DECL mob_crypt_crawlerAI : public ScriptedAI
{
    mob_crypt_crawlerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 EnvelopingWeb_Timer;
    uint32 Dazed_Timer;
    uint32 DeafeningScreech_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        EnvelopingWeb_Timer = 4000;
        Dazed_Timer = 11000;
        DeafeningScreech_Timer = 23000;
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
            //EnvelopingWeb
            if (EnvelopingWeb_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_ENVELOPINGWEB);
                //23 seconds until we should cast this again
                EnvelopingWeb_Timer = 23000;
            }else EnvelopingWeb_Timer -= diff;

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 35) //35% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //11 seconds until we should cast this again
                Dazed_Timer = 11000;
            }else Dazed_Timer -= diff;

            //DeafeningScreech
            if (DeafeningScreech_Timer < diff)
            {
                //Cast
                if (rand()%100 < 25) //25% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DEAFENINGSCREECH);
                }
                //32 seconds until we should cast this again
                DeafeningScreech_Timer = 32000;
            }else DeafeningScreech_Timer -= diff;

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
CreatureAI* GetAI_mob_crypt_crawler(Creature *_Creature)
{
    return new mob_crypt_crawlerAI (_Creature);
}


void AddSC_mob_crypt_crawler()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crypt_crawler";
    newscript->GetAI = GetAI_mob_crypt_crawler;
    m_scripts[nrscripts++] = newscript;
}