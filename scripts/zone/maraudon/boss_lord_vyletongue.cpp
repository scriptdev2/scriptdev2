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
SDName: Boss_Lord_Vyletogue
SD%Complete: 100
SDComment: 
EndScriptData */


#include "../../sc_defines.h"

#define SPELL_PUTRIDBREATH             21080        
#define SPELL_SMOKEBOMB                  8817              

struct MANGOS_DLL_DECL boss_lord_vyletongueAI : public ScriptedAI
{
    boss_lord_vyletongueAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 PutridBreath_Timer;
    uint32 SmokeBomb_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        PutridBreath_Timer = 14000;
        SmokeBomb_Timer = 8000;
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

        //PutridBreath_Timer
        if (PutridBreath_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_PUTRIDBREATH);

            //16 seconds
            PutridBreath_Timer = 16000;
        }else PutridBreath_Timer -= diff;

        //SmokeBomb_Timer
        if (SmokeBomb_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SMOKEBOMB);

            //12 seconds until we should cast this agian
            SmokeBomb_Timer = 12000;
        }else SmokeBomb_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_lord_vyletongue(Creature *_Creature)
{
    return new boss_lord_vyletongueAI (_Creature);
}


void AddSC_boss_lord_vyletongue()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_vyletongue";
    newscript->GetAI = GetAI_boss_lord_vyletongue;
    m_scripts[nrscripts++] = newscript;
}
