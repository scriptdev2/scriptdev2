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

#define SPELL_HOLYSTRIKE    17284
#define SPELL_CRUSADERSTRIKE    14518
#define SPELL_DEVOTIONAURA    17232

struct MANGOS_DLL_DECL mob_crimson_gallantAI : public ScriptedAI
{
    mob_crimson_gallantAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 HolyStrike_Timer;
    uint32 CrusaderStrike_Timer;
    bool HasAura;    
    bool InCombat;

    void EnterEvadeMode()
    {
        HolyStrike_Timer = 2000;
        CrusaderStrike_Timer = 8000;
        HasAura = false;
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

                if (!HasAura)
                {
                    m_creature->CastSpell(m_creature,SPELL_DEVOTIONAURA,true);
                    HasAura = true;
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

            //HolyStrike
            if (HolyStrike_Timer < diff)
            {
                //Cast
                if (rand()%100 < 85) //85% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_HOLYSTRIKE);
                }
                //5 seconds until we should cast this again
                HolyStrike_Timer = 5000;
            }else HolyStrike_Timer -= diff;

            //CrusaderStrike
            if (CrusaderStrike_Timer < diff)
            {
                //Cast
                if (rand()%100 < 60) //60% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CRUSADERSTRIKE);
                }
                //8 seconds until we should cast this again
                CrusaderStrike_Timer = 8000;
            }else CrusaderStrike_Timer -= diff;

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
CreatureAI* GetAI_mob_crimson_gallant(Creature *_Creature)
{
    return new mob_crimson_gallantAI (_Creature);
}


void AddSC_mob_crimson_gallant()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crimson_gallant";
    newscript->GetAI = GetAI_mob_crimson_gallant;
    m_scripts[nrscripts++] = newscript;
}