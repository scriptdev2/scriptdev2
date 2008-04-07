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
SDName: Boss_Verek
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Depths
EndScriptData */

#include "sc_creature.h"

// **** This script is still under Developement ****


#define SPELL_FROSTBOLT                12737    //Not sure if right ID            
#define SPELL_FROSTNOVA            12748        //Not sure if right ID


struct MANGOS_DLL_DECL boss_verekAI : public ScriptedAI
{
    boss_verekAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 FrostBolt_Timer;
    uint32 FrostNova_Timer;
    bool InCombat;

    void Reset()
    {       
        FrostBolt_Timer = 8000;
        FrostNova_Timer = 2000;
        InCombat = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
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

        //FrostBolt_Timer
        if (FrostBolt_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FROSTBOLT);

            //7 seconds until we should cast this agian
            FrostBolt_Timer = 4000;
        }else FrostBolt_Timer -= diff;

        //FrostNova_Timer
        if (FrostNova_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FROSTNOVA);

            //5 seconds until we should cast this agian
            FrostNova_Timer = 8000;
        }else FrostNova_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_verek(Creature *_Creature)
{
    return new boss_verekAI (_Creature);
}


void AddSC_boss_verek()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_verek";
    newscript->GetAI = GetAI_boss_verek;
    m_scripts[nrscripts++] = newscript;
}
