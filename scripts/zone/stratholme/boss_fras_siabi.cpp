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
SDName: boss_fras_siabi
SD%Complete: 100
SDComment: 
EndScriptData */

#include "sc_creature.h"

// **** This script is still under Developement ****

#define SPELL_SMOKEBOMB    7964
#define SPELL_DAZED    1604
#define SPELL_FLAMEBREATH    20712
#define SPELL_DEMORALIZINGSHOUT    23511

struct MANGOS_DLL_DECL boss_fras_siabiAI : public ScriptedAI
{
    boss_fras_siabiAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 SmokeBomb_Timer;
    uint32 Dazed_Timer;
    uint32 FlameBreath_Timer;
    uint32 DemoralizingShout_Timer;
    bool InCombat;

    void Reset()
    {
        SmokeBomb_Timer = 2000;
        Dazed_Timer = 11000;
        FlameBreath_Timer = 15000;
        DemoralizingShout_Timer = 6000;
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

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //SmokeBomb_Timer
            if (SmokeBomb_Timer < diff)
            {
                //Cast
                if (rand()%100 < 70) //70% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_SMOKEBOMB);
                }
                //9 seconds until we should cast this agian
                SmokeBomb_Timer = 9000;
            }else SmokeBomb_Timer -= diff;

            //Dazed_Timer
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 40) //40% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //11 seconds until we should cast this agian
                Dazed_Timer = 11000;
            }else Dazed_Timer -= diff;

            //FlameBreath_Timer
            if (FlameBreath_Timer < diff)
            {
                //Cast
                if (rand()%100 < 70) //70% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_FLAMEBREATH);
                }
                //30 seconds until we should cast this agian
                FlameBreath_Timer = 30000;
            }else FlameBreath_Timer -= diff;

            //DemoralizingShout_Timer
            if (DemoralizingShout_Timer < diff)
            {
                //Cast
                if (rand()%100 < 20) //20% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DEMORALIZINGSHOUT);
                }
                //20 seconds until we should try cast this agian
                DemoralizingShout_Timer = 20000;
            }else DemoralizingShout_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_boss_fras_siabi(Creature *_Creature)
{
    return new boss_fras_siabiAI (_Creature);
}


void AddSC_boss_fras_siabi()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_fras_siabi";
    newscript->GetAI = GetAI_boss_fras_siabi;
    m_scripts[nrscripts++] = newscript;
}
