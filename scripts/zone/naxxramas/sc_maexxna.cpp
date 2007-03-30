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

#define SPELL_WEBTRAP           28622   //Spell is normally used by the webtrap on the wall NOT by Maexxna
#define SPELL_WEBSPRAY          29484
#define SPELL_POISONSHOCK       28741
#define SPELL_NECROTICPOISON    28776
#define SPELL_ENRAGE            28747
#define SPELL_SUMMON_SPIDERLING 29434

struct MANGOS_DLL_DECL boss_maexxnaAI : public ScriptedAI
{
    boss_maexxnaAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 WebTrap_Timer;
    uint32 WebSpray_Timer;
    uint32 PoisonShock_Timer;
    uint32 NecroticPoison_Timer;
    uint32 SummonSpiderling_Timer;
    bool Enraged;
    bool InCombat;

    void EnterEvadeMode()
    {
        WebTrap_Timer = 20000;          //20 sec init, 40 sec normal
        WebSpray_Timer = 40000;         //40 seconds
        PoisonShock_Timer = 20000;      //20 seconds
        NecroticPoison_Timer = 30000;   //30 seconds
        SummonSpiderling_Timer = 30000; //30 sec init, 40 sec normal
        InCombat = false;
        Enraged = false;

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
            if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
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
            
            //WebTrap_Timer
            if (WebTrap_Timer < diff)
            {
                //Cast WebTrap on a Random target
                //NYI since we can't select random target
                //DoCast(m_creature->getVictim(),SPELL_WEBTRAP);

                //40 seconds until we should cast this agian
                WebTrap_Timer = 40000;
            }else WebTrap_Timer -= diff;

            //WebSpray_Timer
            if (WebSpray_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(), SPELL_WEBSPRAY);

                //40 seconds until we should cast this agian
                WebSpray_Timer = 40000;
            }else WebSpray_Timer -= diff;

            //PoisonShock_Timer
            if (PoisonShock_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(), SPELL_POISONSHOCK);

                //20 seconds until we should cast this agian
                PoisonShock_Timer = 20000;
            }else PoisonShock_Timer -= diff;

            //NecroticPoison_Timer
            if (NecroticPoison_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(), SPELL_NECROTICPOISON);

                //30 seconds until we should cast this agian
                NecroticPoison_Timer = 30000;
            }else NecroticPoison_Timer -= diff;

            //SummonSpiderling_Timer
            if (SummonSpiderling_Timer < diff)
            {
                //Cast
                DoCast(m_creature, SPELL_SUMMON_SPIDERLING);

                //40 seconds until we should cast this agian
                SummonSpiderling_Timer = 40000;
            }else SummonSpiderling_Timer -= diff;

            //Enrage if not already enraged and below 30%
            if (!Enraged && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30)
            {
                DoCast(m_creature,SPELL_ENRAGE);
                Enraged = true;
            }

            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
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
CreatureAI* GetAI_boss_maexxna(Creature *_Creature)
{
    return new boss_maexxnaAI (_Creature);
}


void AddSC_boss_maexxna()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_maexxna";
    newscript->GetAI = GetAI_boss_maexxna;
    m_scripts[nrscripts++] = newscript;
}