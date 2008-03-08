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
SDName: boss_skul
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****


#define SPELL_FROSTBOLT    21369
#define SPELL_CHILLED    20005
#define SPELL_FROSTNOVA    22645
#define SPELL_ARCANEBOLT     15451

struct MANGOS_DLL_DECL boss_skulAI : public ScriptedAI
{
    boss_skulAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Frostbolt_Timer;
    uint32 Chilled_Timer;
    uint32 FrostNova_Timer;
    uint32 ArcaneBolt_Timer;
    bool InCombat;

    void Reset()
    {
        Frostbolt_Timer = 1000;
        Chilled_Timer = 23000;
        FrostNova_Timer = 8000;
        ArcaneBolt_Timer = 11000;
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

                //Begin melee attack if we are within range
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

        //Frostbolt
        if (Frostbolt_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FROSTBOLT);
            //6 seconds until we should cast this again
            Frostbolt_Timer = 6000;
        }else Frostbolt_Timer -= diff;

        //Chilled
        if (Chilled_Timer < diff)
        {
            //Cast
            if (rand()%100 < 20) //20% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_CHILLED);
            }
            //23 seconds until we should cast this again
            Chilled_Timer = 23000;
        }else Chilled_Timer -= diff;

        //FrostNova
        if (FrostNova_Timer < diff)
        {
            //Cast
            if (rand()%100 < 50) //50% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_FROSTNOVA);
            }
            //16 seconds until we should cast this again
            FrostNova_Timer = 16000;
        }else FrostNova_Timer -= diff;

        //ArcaneBolt
        if (ArcaneBolt_Timer < diff)
        {
            //Cast
            if (rand()%100 < 40) //40% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_ARCANEBOLT);
            }
            //11 seconds until we should cast this again
            ArcaneBolt_Timer = 11000;
        }else ArcaneBolt_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_skul(Creature *_Creature)
{
    return new boss_skulAI (_Creature);
}


void AddSC_boss_skul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_skul";
    newscript->GetAI = GetAI_boss_skul;
    m_scripts[nrscripts++] = newscript;
}
