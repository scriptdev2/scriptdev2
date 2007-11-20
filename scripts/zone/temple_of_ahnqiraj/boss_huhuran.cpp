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
SDName: boss_huhuran
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_FRENZY 26051
#define SPELL_BERSERK 26068
#define SPELL_NOXIOUSPOISON 26053
#define SPELL_WYVERNSTING 26180
#define SPELL_ACIDSPIT 26050

struct MANGOS_DLL_DECL boss_huhuranAI : public ScriptedAI
{
    boss_huhuranAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Frenzy_Timer;
    uint32 Wyvern_Timer;
    uint32 Spit_Timer;
    uint32 Poison_Timer;
    uint32 phase;
    bool InCombat;

    void EnterEvadeMode()
    {
        Frenzy_Timer = 30000; //These times are probably wrong
        Wyvern_Timer = 35000;
        Spit_Timer = 15000;
        Poison_Timer = 300000;
        phase = 1;
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
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                InCombat = true;
            }
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

                if (!InCombat)
                {
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;


        //Frenzy_Timer
        if (Frenzy_Timer < diff && phase == 1)
        {
            //Cast
            DoCast(m_creature, SPELL_FRENZY);

            //30 seconds until we should cast this agian
            Frenzy_Timer = 30000;
        }else Frenzy_Timer -= diff;

        // Wyvern Timer
        if (Wyvern_Timer < diff && phase == 1)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_WYVERNSTING);

            //35 seconds until we should cast this again
            Wyvern_Timer = 35000;
        }else Wyvern_Timer -= diff;

        //Spit Timer
        if (Spit_Timer < diff && phase == 1 )
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_ACIDSPIT);
            //15 seconds until we should cast this agian
            Spit_Timer = 15000;
        }else Spit_Timer -= diff;

        //Poison Timer
        if (Poison_Timer < diff && (phase == 1 || phase == 2))
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_NOXIOUSPOISON);

            //Cast first after 5 minutes, then 5 seconds until we should cast this agian
            Poison_Timer = 5000;
            phase = 2;
        }else Poison_Timer -= diff;

        if ( (phase == 2 || phase == 1) && (m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 30)
        {
            phase = 3;
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            DoTextEmote("is going berserk", NULL);
            DoCast(m_creature, SPELL_BERSERK);
        }

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_huhuran(Creature *_Creature)
{
    return new boss_huhuranAI (_Creature);
}


void AddSC_boss_huhuran()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_huhuran";
    newscript->GetAI = GetAI_boss_huhuran;
    m_scripts[nrscripts++] = newscript;
}
