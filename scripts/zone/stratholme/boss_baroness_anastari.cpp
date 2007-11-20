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
SDName: boss_baroness_anastari
SD%Complete: 90
SDComment: MC disabled
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****

//Status: possess disabled due to malfunction

#define SPELL_BANSHEEWAIL   16565
#define SPELL_BANSHEECURSE    16867
#define SPELL_SILENCE    18327
//#define SPELL_POSSESS   17244

struct MANGOS_DLL_DECL boss_baroness_anastariAI : public ScriptedAI
{
    boss_baroness_anastariAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 BansheeWail_Timer;
    uint32 BansheeCurse_Timer;
    uint32 Silence_Timer;
    //uint32 Possess_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        BansheeWail_Timer = 1000;
        BansheeCurse_Timer = 11000;
        Silence_Timer = 13000;
        //Possess_Timer = 35000;
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
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //BansheeWail
        if (BansheeWail_Timer < diff)
        {
            //Cast
            if (rand()%100 < 95) //95% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_BANSHEEWAIL);
            }
            //4 seconds until we should cast this again
            BansheeWail_Timer = 4000;
        }else BansheeWail_Timer -= diff;

        //BansheeCurse
        if (BansheeCurse_Timer < diff)
        {
            //Cast
            if (rand()%100 < 75) //75% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_BANSHEECURSE);
            }
            //18 seconds until we should cast this again
            BansheeCurse_Timer = 18000;
        }else BansheeCurse_Timer -= diff;

        //Silence
        if (Silence_Timer < diff)
        {
            //Cast
            if (rand()%100 < 80) //80% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_SILENCE);
            }
            //13 seconds until we should cast this again
            Silence_Timer = 13000;
        }else Silence_Timer -= diff;

        //Possess
        /*            if (Possess_Timer < diff)
        {
        //Cast
        if (rand()%100 < 65) //65% chance to cast
        {
        Unit* target = NULL;
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        if (target)DoCast(target,SPELL_POSSESS);
        }
        //50 seconds until we should cast this again
        Possess_Timer = 50000;
        }else Possess_Timer -= diff;
        */

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_baroness_anastari(Creature *_Creature)
{
    return new boss_baroness_anastariAI (_Creature);
}


void AddSC_boss_baroness_anastari()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_baroness_anastari";
    newscript->GetAI = GetAI_boss_baroness_anastari;
    m_scripts[nrscripts++] = newscript;
}
