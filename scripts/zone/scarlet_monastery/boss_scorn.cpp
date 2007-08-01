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



#define SPELL_LICHSLAP                  28873
#define SPELL_FROSTBOLTVOLLEY           8398
#define SPELL_MINDFLAY                  17313
#define SPELL_FROSTNOVA                 15531

struct MANGOS_DLL_DECL boss_scornAI : public ScriptedAI
{
    boss_scornAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 LichSlap_Timer;
    uint32 FrostboltVolley_Timer;
    uint32 MindFlay_Timer;
    uint32 FrostNova_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        LichSlap_Timer = 45000;
        FrostboltVolley_Timer = 30000;
        MindFlay_Timer = 30000;
        FrostNova_Timer = 30000;
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
            //LichSlap_Timer
            if (LichSlap_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_LICHSLAP);

                //45 seconds until we should cast this agian
                LichSlap_Timer = 45000;
            }else LichSlap_Timer -= diff;

            //FrostboltVolley_Timer
            if (FrostboltVolley_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FROSTBOLTVOLLEY);

                //30 seconds until we should cast this agian
                FrostboltVolley_Timer = 20000;
            }else FrostboltVolley_Timer -= diff;

            //MindFlay_Timer
            if (MindFlay_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_MINDFLAY);

                //30 seconds until we should cast this agian
                MindFlay_Timer = 20000;
            }else MindFlay_Timer -= diff;

            //FrostNova_Timer
            if (FrostNova_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FROSTNOVA);

                //30 seconds until we should cast this agian
                FrostNova_Timer = 15000;
            }else FrostNova_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_boss_scorn(Creature *_Creature)
{
    return new boss_scornAI (_Creature);
}


void AddSC_boss_scorn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_scorn";
    newscript->GetAI = GetAI_boss_scorn;
    m_scripts[nrscripts++] = newscript;
}
