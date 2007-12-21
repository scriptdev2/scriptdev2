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
SDName: mob_teremus_the_decourer
SD%Complete: 100
SDComment: 
SDCategory: Blasted Lands
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_FLAMEBREATH                20712      
#define SPELL_SOULCONSUMPTION             12667  
#define SPELL_RESISTALL            18114            //Not Sure needs test.

struct MANGOS_DLL_DECL boss_teremusthedevourerAI : public ScriptedAI
{
    boss_teremusthedevourerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FlameBreath_Timer;
    uint32 SoulConsumption_Timer;
    uint32 ResistAll_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        FlameBreath_Timer = 5000;
        SoulConsumption_Timer = 20000;
        ResistAll_Timer = 2000;
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //FlameBreath_Timer
        if (FlameBreath_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FLAMEBREATH);

            //10 seconds
            FlameBreath_Timer = 10000;
        }else FlameBreath_Timer -= diff;

        //SoulConsumption_Timer
        if (SoulConsumption_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SOULCONSUMPTION);

            //18 seconds until we should cast this agian
            SoulConsumption_Timer = 18000;
        }else SoulConsumption_Timer -= diff;


        //ResistAll_Timer
        if (ResistAll_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_RESISTALL);

            //45 seconds until we should cast this agian
            ResistAll_Timer = 45000;
        }else ResistAll_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_boss_teremusthedevourer(Creature *_Creature)
{
    return new boss_teremusthedevourerAI (_Creature);
}


void AddSC_boss_teremusthedevourer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_teremus_the_devourer";
    newscript->GetAI = GetAI_boss_teremusthedevourer;
    m_scripts[nrscripts++] = newscript;
}
