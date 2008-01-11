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
SDName: Boss_Lord_Incendius
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Depths
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****


#define SPELL_FIRESTORM               13899            
#define SPELL_MIGHTYBLOW           14099
#define SPELL_CURSE           26977


struct MANGOS_DLL_DECL boss_lord_incendiusAI : public ScriptedAI
{
    boss_lord_incendiusAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FireStorm_Timer;
    uint32 MightyBlow_Timer;
    uint32 Curse_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        FireStorm_Timer = 6000;
        MightyBlow_Timer = 22000;
        Curse_Timer = 16000;
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

        //FireStorm_Timer
        if (FireStorm_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FIRESTORM);

            //20 seconds 
            FireStorm_Timer = 18000;
        }else FireStorm_Timer -= diff;

        //MightyBlow_Timer
        if (MightyBlow_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MIGHTYBLOW);

            //12 seconds until we should cast this agian
            MightyBlow_Timer = 12000;
        }else MightyBlow_Timer -= diff;

        //Curse_Timer
        if (Curse_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CURSE);

            //28 seconds until we should cast this agian
            Curse_Timer = 22000;
        }else Curse_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_lord_incendius(Creature *_Creature)
{
    return new boss_lord_incendiusAI (_Creature);
}


void AddSC_boss_lord_incendius()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_incendius";
    newscript->GetAI = GetAI_boss_lord_incendius;
    m_scripts[nrscripts++] = newscript;
}
