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


// Adds NYI

#define SPELL_DEMORALIZINGSHOUT     19778
#define SPELL_INSPIRE               19779
#define SPELL_KNOCKDOWN             19780
#define SPELL_FLAMESPEAR            19781       //Might be NYI in core

struct MANGOS_DLL_DECL boss_sulfuronAI : public ScriptedAI
{
    boss_sulfuronAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 DemoralizingShout_Timer;
    uint32 Inspire_Timer;
    uint32 Knockdown_Timer;
    uint32 Flamespear_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        DemoralizingShout_Timer = 8000;      //These times are probably wrong
        Inspire_Timer = 30000;
        Knockdown_Timer = 16000;
        Flamespear_Timer = 12000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BLEED, true);
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //DemoralizingShout_Timer
        if (DemoralizingShout_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_DEMORALIZINGSHOUT);

            //20-25 seconds until we should cast this agian
            DemoralizingShout_Timer = 20000 + rand()%5000;
        }else DemoralizingShout_Timer -= diff;

        //Inspire_Timer
        if (Inspire_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_INSPIRE);

            //28-32 seconds until we should cast this agian
            Inspire_Timer = 28000 + rand()%4000;
        }else Inspire_Timer -= diff;

        //Knockdown_Timer
        if (Knockdown_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_KNOCKDOWN);

            //17-20 seconds until we should cast this agian
            Knockdown_Timer = 17000 + rand()%3000;
        }else Knockdown_Timer -= diff;

        //Flamespear_Timer
        if (Flamespear_Timer < diff)
        {
            //Cast
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_FLAMESPEAR);

            //12-16 seconds until we should cast this agian
            Flamespear_Timer = 12000 + rand()%4000;
        }else Flamespear_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_sulfuron(Creature *_Creature)
{
    return new boss_sulfuronAI (_Creature);
}


void AddSC_boss_sulfuron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_sulfuron";
    newscript->GetAI = GetAI_boss_sulfuron;
    m_scripts[nrscripts++] = newscript;
}
