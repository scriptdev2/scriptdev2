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

#include "../sc_defines.h"

#define SPELL_SLEEP                24777
#define SPELL_NOXIOUSBREATH        24818
#define SPELL_TAILSWEEP            15847
//#define SPELL_MARKOFNATURE         25040                    // Not working
#define SPELL_ARCANEBLAST          24857
#define SPELL_BELLOWINGROAR        22686
#define SPELL_SUMMONSHADE          24843



struct MANGOS_DLL_DECL boss_taerarAI : public ScriptedAI
{
    boss_taerarAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Sleep_Timer;
    uint32 NoxiousBreath_Timer;
    uint32 TailSweep_Timer;
    //uint32 MarkOfNature_Timer;
    uint32 ArcaneBlast_Timer;
    uint32 BellowingRoar_Timer;
    uint32 Shades_Timer;
    uint32 Summon_Timer;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;
    bool InCombat;
    bool Shades;

    void EnterEvadeMode()
    {       
        Sleep_Timer = 45000;
        NoxiousBreath_Timer = 15000;
        TailSweep_Timer = 20000;
        //MarkOfNature_Timer = 45000;
        ArcaneBlast_Timer = 8000;
        BellowingRoar_Timer = 30000;
	Summon_Timer = 0;
        Shades_Timer = 60000;                               //The time that Taerar is banished
        InCombat = false;
        Shades = false; 

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

    void SummonShades(Unit* victim)
    {
        Rand = rand()%15;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%15;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(15302, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 300000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (Shades && Shades_Timer < diff)
        {
            //Become unbanished again 
            m_creature->setFaction(14);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->m_canMove = true; 
            Shades = false;
        } else if (Shades)
               {
                  Shades_Timer -= diff;
                  //Do nothing while banished
                  return;
               }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {

            //Sleep_Timer
            if (Sleep_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SLEEP);

                //16 seconds
                Sleep_Timer = 16000;
            } else Sleep_Timer -= diff;

            //NoxiousBreath_Timer
            if (NoxiousBreath_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_NOXIOUSBREATH);

                //28 seconds until we should cast this agian
                NoxiousBreath_Timer = 28000;
            } else NoxiousBreath_Timer -= diff;


            //TailSweep_Timer
            if (TailSweep_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_TAILSWEEP);

                //20 seconds until we should cast this agian
                TailSweep_Timer = 20000;
            } else TailSweep_Timer -= diff;

            //             //MarkOfNature_Timer
            //            if (MarkOfNature_Timer < diff)
            //             {
            //                 //Cast
            //                 DoCast(m_creature->getVictim(),SPELL_MARKOFNATURE);
            // 
            //                 //45 seconds until we should cast this agian
            //                 MarkOfNature_Timer = 45000;
            //             }else MarkOfNature_Timer -= diff;

            //ArcaneBlast_Timer
            if (ArcaneBlast_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_ARCANEBLAST);

                //18 seconds until we should cast this agian
                ArcaneBlast_Timer = 18000;
            }else ArcaneBlast_Timer -= diff;

            //BellowingRoar_Timer
            if (BellowingRoar_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BELLOWINGROAR);

                //18 seconds until we should cast this agian
                BellowingRoar_Timer = 45000;
            } else BellowingRoar_Timer -= diff;


            //Summon 3 Shades
            if ( !Shades  && (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5) == 75)
            {

		if (Summon_Timer < diff)
		{

                //Inturrupt any spell casting
                 m_creature->InterruptSpell();
                 m_creature->m_canMove = false; 
                m_creature->setFaction(35);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                //Cast
                SummonShades(m_creature->getVictim());
                SummonShades(m_creature->getVictim());
                SummonShades(m_creature->getVictim());
                Summon_Timer = 120000;
                } else Summon_Timer -= diff;
	    }

            //Summon 3 Shades
            if ( !Shades  && (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5) == 50)
            {

		if (Summon_Timer < diff)
		{

                //Inturrupt any spell casting
                 m_creature->InterruptSpell();
                 m_creature->m_canMove = false; 
                m_creature->setFaction(35);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                //Cast
                SummonShades(m_creature->getVictim());
                SummonShades(m_creature->getVictim());
                SummonShades(m_creature->getVictim());
                Summon_Timer = 120000;
                } else Summon_Timer -= diff;
	    }



            //Summon 3 Shades
            if ( !Shades  && (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5) == 25)
            {

		if (Summon_Timer < diff)
		{

                //Inturrupt any spell casting
                 m_creature->InterruptSpell();
                 m_creature->m_canMove = false; 
                m_creature->setFaction(35);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                //Cast
                SummonShades(m_creature->getVictim());
                SummonShades(m_creature->getVictim());
                SummonShades(m_creature->getVictim());
                Summon_Timer = 120000;
                } else Summon_Timer -= diff;
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
CreatureAI* GetAI_boss_taerar(Creature *_Creature)
{
    return new boss_taerarAI (_Creature);
}


void AddSC_boss_taerar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_taerar";
    newscript->GetAI = GetAI_boss_taerar;
    m_scripts[nrscripts++] = newscript;
}
