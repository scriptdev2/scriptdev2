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
SDName: Boss_Pyromancer_Loregrain
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Depths
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****

#define SPELL_FLAMESHOCK                10448            
#define SPELL_MOLTENBLAST            15095
#define SPELL_FIREWARD               15041
#define SPELL_SEARINGTOTEM               10438

struct MANGOS_DLL_DECL boss_pyromancer_loregrainAI : public ScriptedAI
{
    boss_pyromancer_loregrainAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FlameShock_Timer;
    uint32 MoltenBlast_Timer;
    uint32 FireWard_Timer;
    uint32 SearingTotem_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        FlameShock_Timer = 13000;
        MoltenBlast_Timer = 20000;
        FireWard_Timer = 8000;
        SearingTotem_Timer = 2000;
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

        //FlameShock_Timer
        if (FlameShock_Timer < diff)
        {
            //Cast Flam Shock on a Random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_FLAMESHOCK);

            //8 seconds until we should cast this agian
            FlameShock_Timer = 8000;
        }else FlameShock_Timer -= diff;

        //MoltenBlast_Timer
        if (MoltenBlast_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MOLTENBLAST);

            //12 seconds until we should cast this agian
            MoltenBlast_Timer = 12000;
        }else MoltenBlast_Timer -= diff;

        //FireWard_Timer
        if (FireWard_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FIREWARD);

            //16 seconds
            FireWard_Timer = 16000;
        }else FireWard_Timer -= diff;

        //SearingTotem_Timer
        if (SearingTotem_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SEARINGTOTEM);

            //30 seconds
            SearingTotem_Timer = 25000;
        }else SearingTotem_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_pyromancer_loregrain(Creature *_Creature)
{
    return new boss_pyromancer_loregrainAI (_Creature);
}


void AddSC_boss_pyromancer_loregrain()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_pyromancer_loregrain";
    newscript->GetAI = GetAI_boss_pyromancer_loregrain;
    m_scripts[nrscripts++] = newscript;
}
