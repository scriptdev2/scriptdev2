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

#define SPELL_STRIKE                11976
#define SPELL_DAZED                 1604
#define SPELL_DEMORALIZINGSHOUT     16244
#define SPELL_CADAVERWORMS          16143

struct MANGOS_DLL_DECL mob_ravaged_cadaverAI : public ScriptedAI
{
    mob_ravaged_cadaverAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Strike_Timer;
    uint32 Dazed_Timer;
    uint32 DemoralizingShout_Timer;
    uint32 CadaverWorms_Timer;
    uint32 SummonBrokenCadaver_Timer;
    bool InCombat;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void EnterEvadeMode()
    {
        Strike_Timer = 2000;
        Dazed_Timer = 7000;
        DemoralizingShout_Timer = 11000;
        CadaverWorms_Timer = 13000;
        SummonBrokenCadaver_Timer = 9000;
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
                InCombat = true;
            }
        }
    }

    void SummonBrokenCadaver(Unit* victim)
    {
        Rand = rand()%10;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%10;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(10383, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000);
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

            //Strike
            if (Strike_Timer < diff)
            {
                //Cast
                if (rand()%100 < 65) //65% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_STRIKE);
                }
                //6 seconds until we should cast this again
                Strike_Timer = 6000;
            }else Strike_Timer -= diff;

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 25) //25% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //8 seconds until we should try cast this again
                Dazed_Timer = 8000;
            }else Dazed_Timer -= diff;

            //DemoralizingShout
            if (DemoralizingShout_Timer < diff)
            {
                if (rand()%100 < 80) //80% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_DEMORALIZINGSHOUT);
                }
                //19 seconds until we should try cast this again
                DemoralizingShout_Timer = 19000;
            }else DemoralizingShout_Timer -= diff;

            //CadaverWorms
            if (CadaverWorms_Timer < diff)
            {
                //Cast
                if (rand()%100 < 35) //35% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CADAVERWORMS);
                }
                //17 seconds until we should try cast this again
                CadaverWorms_Timer = 17000;
            }else CadaverWorms_Timer -= diff;

            //SummonBrokenCadaver
            if (SummonBrokenCadaver_Timer < diff)
            {
                //Cast
                SummonBrokenCadaver(m_creature->getVictim());
                //35 seconds until we should cast this again
                SummonBrokenCadaver_Timer = 35000;
            }else SummonBrokenCadaver_Timer -= diff;

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
CreatureAI* GetAI_mob_ravaged_cadaver(Creature *_Creature)
{
    return new mob_ravaged_cadaverAI (_Creature);
}


void AddSC_mob_ravaged_cadaver()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_ravaged_cadaver";
    newscript->GetAI = GetAI_mob_ravaged_cadaver;
    m_scripts[nrscripts++] = newscript;
}