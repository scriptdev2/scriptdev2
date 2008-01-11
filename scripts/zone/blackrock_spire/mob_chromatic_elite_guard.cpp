/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Mob_Chromatic_Elite_Guard
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Spire
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_KNOCKDOWN                20276   

struct MANGOS_DLL_DECL mob_chromatic_elite_guardAI : public ScriptedAI
{
    mob_chromatic_elite_guardAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 KnockDown_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        KnockDown_Timer = 20000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && 
                m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
                InCombat = true;

            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //KnockDown_Timer
        if (KnockDown_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_KNOCKDOWN);

            //8 seconds
            KnockDown_Timer = 8000;
        }else KnockDown_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_mob_chromatic_elite_guard(Creature *_Creature)
{
    return new mob_chromatic_elite_guardAI (_Creature);
}


void AddSC_mob_chromatic_elite_guard()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_chromatic_elite_guard";
    newscript->GetAI = GetAI_mob_chromatic_elite_guard;
    m_scripts[nrscripts++] = newscript;
}
