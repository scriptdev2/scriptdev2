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

#define SPELL_SUMMONSKELETON    17204
#define SPELL_SOULTAP    16430
#define SPELL_BONEARMOR    11445

struct MANGOS_DLL_DECL mob_thuzadin_necromancerAI : public ScriptedAI
{
    mob_thuzadin_necromancerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 SummonSkeleton_Timer;
    uint32 SoulTap_Timer;
    uint32 BoneArmor_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        SummonSkeleton_Timer = 6000;
        SoulTap_Timer = 11000;
        BoneArmor_Timer = 1000;
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

            //SummonSkeleton
            if (SummonSkeleton_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SUMMONSKELETON);
                SummonSkeleton_Timer = 60000;
            }else SummonSkeleton_Timer -= diff;

            //SoulTap
            if (SoulTap_Timer < diff)
            {
                //Cast
                if (rand()%100 < 80) //80% chance to cast
                {
                DoCast(m_creature->getVictim(),SPELL_SOULTAP);
                }
                //15 seconds until we should try cast this again
                SoulTap_Timer = 15000;
            }else SoulTap_Timer -= diff;
            
            //BoneArmor
            if (BoneArmor_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BONEARMOR);
                //120 seconds until we should cast this again
                BoneArmor_Timer = 120000;
            }else BoneArmor_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_thuzadin_necromancer(Creature *_Creature)
{
    return new mob_thuzadin_necromancerAI (_Creature);
}


void AddSC_mob_thuzadin_necromancer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_thuzadin_necromancer";
    newscript->GetAI = GetAI_mob_thuzadin_necromancer;
    m_scripts[nrscripts++] = newscript;
}
