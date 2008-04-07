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
SDName: Boss_Baelgar
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Depths
EndScriptData */

#include "sc_creature.h"

#define SPELL_MAGMASPLASH                13880            
#define SPELL_SPAWNSOFBAELGAR            13895


struct MANGOS_DLL_DECL boss_baelgarAI : public ScriptedAI
{
    boss_baelgarAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 MagmaSplash_Timer;
    uint32 SpawnOfBaelgar_Timer;
    bool InCombat;

    void Reset()
    {       
        MagmaSplash_Timer = 12000;
        SpawnOfBaelgar_Timer = 30000;
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

        //MagmaSplash_Timer
        if (MagmaSplash_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MAGMASPLASH);

            //8 seconds cause its a proc
            MagmaSplash_Timer = 8000;
        }else MagmaSplash_Timer -= diff;

        //SpawnOfBaelgar_Timer
        if (SpawnOfBaelgar_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SPAWNSOFBAELGAR);

            //25 seconds until we should cast this agian
            SpawnOfBaelgar_Timer = 25000;
        }else SpawnOfBaelgar_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_baelgar(Creature *_Creature)
{
    return new boss_baelgarAI (_Creature);
}


void AddSC_boss_baelgar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_baelgar";
    newscript->GetAI = GetAI_boss_baelgar;
    m_scripts[nrscripts++] = newscript;
}
