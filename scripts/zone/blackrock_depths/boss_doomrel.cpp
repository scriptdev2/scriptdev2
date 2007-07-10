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

// **** This script is still under Developement ****

#define SPELL_SHADOWBOLTVOLLEY               17228
#define SPELL_IMMOLATE                       15505
#define SPELL_CURSEOFWEAKNESS                17227
#define SPELL_DEMONARMOR                     11735

struct MANGOS_DLL_DECL boss_doomrelAI : public ScriptedAI
{
    boss_doomrelAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ShadowVolley_Timer;
    uint32 Immolate_Timer;
    uint32 CurseOfWeakness_Timer;
    uint32 DemonArmor_Timer;
    bool InCombat;
    bool Voidwalkers;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;


    void EnterEvadeMode()
    {
        ShadowVolley_Timer = 10000;
        Immolate_Timer = 18000;
        CurseOfWeakness_Timer = 5000;
        DemonArmor_Timer = 16000;
        InCombat = false;
        Voidwalkers = false;

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

    void SummonVoidwalkers(Unit* victim)
    {
        Rand = rand()%5;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%5;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(16119, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 120000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            
            //ShadowVolley_Timer
            if (ShadowVolley_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SHADOWBOLTVOLLEY);

                //12 seconds
               ShadowVolley_Timer = 12000;
            }else ShadowVolley_Timer -= diff;

            //Immolate_Timer
            if (Immolate_Timer < diff)
            {
                //Cast Immolate on a Random target
                Unit* target = NULL;
 
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_IMMOLATE);

                //25 seconds
                Immolate_Timer = 25000;
            }else Immolate_Timer -= diff;

            //CurseOfWeakness_Timer
            if (CurseOfWeakness_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CURSEOFWEAKNESS);

                //45 seconds
                CurseOfWeakness_Timer = 45000;
            }else CurseOfWeakness_Timer -= diff;

            //DemonArmor_Timer
            if (DemonArmor_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_DEMONARMOR);

                //5 minutes
                DemonArmor_Timer = 300000;
            }else DemonArmor_Timer -= diff;


            //Summon Voidwalkers
            if (!Voidwalkers && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 51 )
            {
                //Cast
                SummonVoidwalkers(m_creature->getVictim());
                SummonVoidwalkers(m_creature->getVictim());
                SummonVoidwalkers(m_creature->getVictim());
                Voidwalkers = true;
            }

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
CreatureAI* GetAI_boss_doomrel(Creature *_Creature)
{
    return new boss_doomrelAI (_Creature);
}

void AddSC_boss_doomrel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_doomrel";
    newscript->GetAI = GetAI_boss_doomrel;
    m_scripts[nrscripts++] = newscript;
}
