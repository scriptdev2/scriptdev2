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
// #define SPELL_MARKOFNATURE      25040   // Not working
#define SPELL_LIGHTNINGWAVE        24819
#define SPELL_SUMMONDRUIDS         24795



struct MANGOS_DLL_DECL boss_ysondreAI : public ScriptedAI
{
    boss_ysondreAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Sleep_Timer;
    uint32 NoxiousBreath_Timer;
    uint32 TailSweep_Timer;
    //    uint32 MarkOfNature_Timer;
    uint32 LightningWave_Timer;
    uint32 SummonDruids_Timer;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Sleep_Timer = 30000;
        NoxiousBreath_Timer = 15000;
        TailSweep_Timer = 20000;
        //      MarkOfNature_Timer = 45000;
        LightningWave_Timer = 8000;
        SummonDruids_Timer = 0;
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
                InCombat = true;

            }
        }
    }


    void SummonDruids(Unit* victim)
    {
        Rand = rand()%20;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%20;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(15260, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 300000);
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

            //Sleep_Timer
            if (Sleep_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SLEEP);

                //16 seconds
                Sleep_Timer = 16000;
            }else Sleep_Timer -= diff;

            //NoxiousBreath_Timer
            if (NoxiousBreath_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_NOXIOUSBREATH);

                //28 seconds until we should cast this agian
                NoxiousBreath_Timer = 28000;
            }else NoxiousBreath_Timer -= diff;


            //TailSweep_Timer
            if (TailSweep_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_TAILSWEEP);

                //20 seconds until we should cast this agian
                TailSweep_Timer = 20000;
            }else TailSweep_Timer -= diff;

            //             //MarkOfNature_Timer
            //            if (MarkOfNature_Timer < diff)
            //             {
            //                 //Cast
            //                 DoCast(m_creature->getVictim(),SPELL_MARKOFNATURE);
            // 
            //                 //45 seconds until we should cast this agian
            //                 MarkOfNature_Timer = 45000;
            //             }else MarkOfNature_Timer -= diff;

            //LightningWave_Timer
            if (LightningWave_Timer < diff)
            {
                //Cast LIGHTNINGWAVE on a Random target
                Unit* target = NULL;

                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_LIGHTNINGWAVE);

                //18 seconds until we should cast this agian
                LightningWave_Timer = 18000;
            }else LightningWave_Timer -= diff;


            //SummonDruids_Timer
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 26 )
            {
                if (SummonDruids_Timer < diff)
                {
                    // summon 6 druids every 45s  
                    SummonDruids(m_creature->getVictim());
                    SummonDruids(m_creature->getVictim());
                    SummonDruids(m_creature->getVictim());
                    SummonDruids(m_creature->getVictim());
                    SummonDruids(m_creature->getVictim());
                    SummonDruids(m_creature->getVictim());

                    //45 seconds until we should cast this agian
                    SummonDruids_Timer = 45000;
                } else SummonDruids_Timer -= diff;
            }

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DIST))
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
CreatureAI* GetAI_boss_ysondre(Creature *_Creature)
{
    return new boss_ysondreAI (_Creature);
}


void AddSC_boss_ysondre()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ysondre";
    newscript->GetAI = GetAI_boss_ysondre;
    m_scripts[nrscripts++] = newscript;
}
