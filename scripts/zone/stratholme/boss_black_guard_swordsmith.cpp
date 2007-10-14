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



// Unique NPC, only summoned after attempt to open item "Blacksmithing Plans", not a regular mob always around in instance
// The "Blacksmithing Plans" appear to the right of the stairs of the first ziggurat (not the same as the one for Crimson Hammersmith)

#define SPELL_DAZED    1604
#define SPELL_BANSHEECURSE    16867

struct MANGOS_DLL_DECL boss_black_guard_swordsmithAI : public ScriptedAI
{
    boss_black_guard_swordsmithAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Dazed_Timer;
    uint32 BansheeCurse_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Dazed_Timer = 6000;
        BansheeCurse_Timer = 10000;
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Dazed
        if (Dazed_Timer < diff)
        {
            //Cast
            if (rand()%100 < 75) //75% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_DAZED);
            }
            //20 seconds until we should cast this again
            Dazed_Timer = 20000;
        }else Dazed_Timer -= diff;

        //BansheeCurse
        if (BansheeCurse_Timer < diff)
        {
            //Cast
            if (rand()%100 < 10) //10% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_BANSHEECURSE);
            }
            //15 seconds until we should cast this again
            BansheeCurse_Timer = 15000;
        }else BansheeCurse_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_black_guard_swordsmith(Creature *_Creature)
{
    return new boss_black_guard_swordsmithAI (_Creature);
}


void AddSC_boss_black_guard_swordsmith()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_black_guard_swordsmith";
    newscript->GetAI = GetAI_boss_black_guard_swordsmith;
    m_scripts[nrscripts++] = newscript;
}
