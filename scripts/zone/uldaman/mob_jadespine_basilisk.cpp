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

/* ScriptData
SDName: mob_jadespine_basilisk
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_CSLUMBER        3636

struct MANGOS_DLL_DECL mob_jadespine_basiliskAI : public ScriptedAI
{
    mob_jadespine_basiliskAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Cslumber_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Cslumber_Timer = 2000;
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
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Cslumber_Timer
        if (Cslumber_Timer < diff)
        {
            //Cast
            // DoCast(m_creature->getVictim(),SPELL_CSLUMBER);
            m_creature->CastSpell(m_creature->getVictim(),SPELL_CSLUMBER, true);

            //Stop attacking target thast asleep and pick new target
            //10 seconds until we should cast this agian
            Cslumber_Timer = 28000;

            Unit* Target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);

            if (!Target || Target == m_creature->getVictim())
                Target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if (Target)
                m_creature->TauntApply(Target);

        }else Cslumber_Timer -= diff;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_jadespine_basilisk(Creature *_Creature)
{
    return new mob_jadespine_basiliskAI (_Creature);
}


void AddSC_mob_jadespine_basilisk()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_jadespine_basilisk";
    newscript->GetAI = GetAI_mob_jadespine_basilisk;
    m_scripts[nrscripts++] = newscript;
}
