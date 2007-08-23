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



#define SPELL_SOULBURN              19393
#define SPELL_SUMMONLAVASPAWN       19392

struct MANGOS_DLL_DECL mob_firelordAI : public ScriptedAI
{
    mob_firelordAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 SoulBurn_Timer;
    uint32 SummonLavaSpawn_Timer;
    bool InCombat;
    Creature *Summoned;

    void EnterEvadeMode()
    {
        SoulBurn_Timer = 6000;
        SummonLavaSpawn_Timer = 10000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
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

                DoStartMeleeAttack(who);
                InCombat = true;

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

            //SoulBurn
            if (SoulBurn_Timer < diff)
            {
                //Cast
                if (rand()%100 < 70) //70% chance to cast
                {
                    Unit* target = NULL;

                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if (target)DoCast(target,SPELL_SOULBURN);
                }
                //20 seconds until we should cast this again
                SoulBurn_Timer = 20000;
            }else SoulBurn_Timer -= diff;

//            //SummonLavaSpawn
//            if (SummonLavaSpawn_Timer < diff)
//            {
//                //Cast
//                DoCast(m_creature,SPELL_SUMMONLAVASPAWN);
//                //30 seconds until we should cast this again
//                SummonLavaSpawn_Timer = 30000;
//            }else SummonLavaSpawn_Timer -= diff;

            //SummonLavaSpawn
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 0 && SummonLavaSpawn_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);

                Summoned = m_creature->SummonCreature(12265, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                ((CreatureAI*)Summoned->AI())->AttackStart(target);
                SummonLavaSpawn_Timer = 30000;
            }else SummonLavaSpawn_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};
CreatureAI* GetAI_mob_firelord(Creature *_Creature)
{
    return new mob_firelordAI (_Creature);
}


void AddSC_mob_firelord()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_firelord";
    newscript->GetAI = GetAI_mob_firelord;
    m_scripts[nrscripts++] = newscript;
}
