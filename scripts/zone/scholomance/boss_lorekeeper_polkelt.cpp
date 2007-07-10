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

#include "../../sc_defines.h"

#define SPELL_VOLATILEINFECTION                24928 	
#define SPELL_DARKPLAGUE             18270 
#define SPELL_CORROSIVEACID               23313
#define SPELL_NOXIOUSCATALYST             18151
           
struct MANGOS_DLL_DECL boss_lorekeeperpolkeltAI : public ScriptedAI
{
    boss_lorekeeperpolkeltAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 VolatileInfection_Timer;
    uint32 Darkplague_Timer;
    uint32 CorrosiveAcid_Timer;
    uint32 NoxiousCatalyst_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        VolatileInfection_Timer = 38000;
        Darkplague_Timer = 8000;
        CorrosiveAcid_Timer = 45000;
        NoxiousCatalyst_Timer = 35000;
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

            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            
            //VolatileInfection_Timer
            if (VolatileInfection_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_VOLATILEINFECTION);

                //32 seconds
                VolatileInfection_Timer = 32000;
            }else VolatileInfection_Timer -= diff;

            //Darkplague_Timer
            if (Darkplague_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_DARKPLAGUE);

                //8 seconds until we should cast this agian
                Darkplague_Timer = 8000;
            }else Darkplague_Timer -= diff;

            //CorrosiveAcid_Timer
            if (CorrosiveAcid_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CORROSIVEACID);

                //25 seconds until we should cast this agian
                CorrosiveAcid_Timer = 25000;
            }else CorrosiveAcid_Timer -= diff;

            //NoxiousCatalyst_Timer
            if (NoxiousCatalyst_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_NOXIOUSCATALYST);

                //38 seconds until we should cast this agian
                NoxiousCatalyst_Timer = 38000;
            }else NoxiousCatalyst_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
}; 
CreatureAI* GetAI_boss_lorekeeperpolkelt(Creature *_Creature)
{
    return new boss_lorekeeperpolkeltAI (_Creature);
}


void AddSC_boss_lorekeeperpolkelt()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lorekeeper_polkelt";
    newscript->GetAI = GetAI_boss_lorekeeperpolkelt;
    m_scripts[nrscripts++] = newscript;
}
