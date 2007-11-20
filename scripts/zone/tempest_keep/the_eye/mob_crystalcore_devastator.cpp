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
SDName: mob_crystalcore_devastator
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../../sc_defines.h"

#define SPELL_COUNTERCHARGE         35035
#define SPELL_KNOCKAWAY                22893

struct MANGOS_DLL_DECL mob_crystalcore_devastatorAI : public ScriptedAI
{
    mob_crystalcore_devastatorAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Knockaway_Timer;
    uint32 Countercharge_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Countercharge_Timer = 9000;
        Knockaway_Timer = 25000;

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
            if(!InCombat)
            {
                InCombat = true;
            }

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

        //Check if we have a current target
        //Knockaway_Timer
        if (Knockaway_Timer < diff)
        {        
            m_creature->CastSpell(m_creature->getVictim(),SPELL_KNOCKAWAY, true);

            // current aggro target is knocked away pick new target
            Unit* Target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);

            if (!Target || Target == m_creature->getVictim())
                Target = SelectUnit(SELECT_TARGET_TOPAGGRO, 1);

            if (Target)
                m_creature->TauntApply(Target);

            //23 seconds until we should cast this agian
            Knockaway_Timer = 23000;
        }
        else Knockaway_Timer -= diff;

        //Countercharge_Timer
        if (Countercharge_Timer < diff)
        {
            //Cast
            DoCast(this->m_creature,SPELL_COUNTERCHARGE);

            //45 seconds until we should cast this agian
            Countercharge_Timer = 45000;
        }else Countercharge_Timer -= diff;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_crystalcore_devastator(Creature *_Creature)
{
    return new mob_crystalcore_devastatorAI (_Creature);
}

void AddSC_mob_crystalcore_devastator()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crystalcore_devastator";
    newscript->GetAI = GetAI_mob_crystalcore_devastator;
    m_scripts[nrscripts++] = newscript;
}
