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
// http://www.mangosproject.org/forum/index.php?showtopic=10314

#define SAY_0	"Intruders! More pawns of the Argent Dawn, no doubt. I already count one of their number among my prisoners. Withdraw from my domain before she is executed!"
#define SAY_1	"You're still here? Your foolishness is amusing! The Argent Dawn wench needn't suffer in vain. Leave at once and she shall be spared!"
#define SAY_2	"I shall take great pleasure in taking this poor wretch's life! It's not too late, she needn't suffer in vain. Turn back and her death shall be merciful!"
#define SAY_3	"May this prisoner's death serve as a warning. None shall defy the Scourge and live!"
#define SAY_4	"So you see fit to toy with the Lich King's creations? Ramstein, be sure to give the intruders a proper greeting."
#define SAY_5	"Time to take matters into my own hands. Come. Enter my domain and challenge the might of the Scourge!"

#define ADD_1X 4017.403809
#define ADD_1Y -3339.703369
#define ADD_1Z 115.057655
#define ADD_1O 5.487860

#define ADD_2X 4013.189209
#define ADD_2Y -3351.808350
#define ADD_2Z 115.052254
#define ADD_2O 0.134280

#define ADD_3X 4017.738037
#define ADD_3Y -3363.478016
#define ADD_3Z 115.057274
#define ADD_3O 0.723313

#define ADD_4X 4048.877197
#define ADD_4Y -3363.223633
#define ADD_4Z 115.054253
#define ADD_4O 3.627735

#define ADD_5X 4051.777588
#define ADD_5Y -3350.893311
#define ADD_5Z 115.055351
#define ADD_5O 3.066176

#define ADD_6X 4048.375977
#define ADD_6Y -3339.966309
#define ADD_6Z 115.055222
#define ADD_6O 2.457497

#define SPELL_UNHOLYAURA_AURA	17467
#define SPELL_UNHOLYAURA	17466
#define SPELL_SHADOWBOLT	18164
#define SPELL_CLEAVE	15584
#define SPELL_MORTALSTRIKE	13737

// spell 17473 should trigger -> 17471

//#define SPELL_RAISEDEAD	17475
//#define SPELL_DEATHPACT 17698
//#define SPELL_SUMMONSKELETONS	17274

struct MANGOS_DLL_DECL boss_baron_rivendareAI : public ScriptedAI
{
    boss_baron_rivendareAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 UnholyAura_Timer;
    uint32 ShadowBolt_Timer;
    uint32 Cleave_Timer;
    uint32 MortalStrike_Timer;
    //	uint32 RaiseDead_Timer;
    uint32 SummonSkeletons_Timer;
    Creature *Summoned;
    bool HasYelled;
    bool HasAura;
    bool InCombat;

    void EnterEvadeMode()
    {
        UnholyAura_Timer = 1500;
        ShadowBolt_Timer = 5000;
        Cleave_Timer = 8000;
        MortalStrike_Timer = 12000;
        //		RaiseDead_Timer = 30000;
        SummonSkeletons_Timer = 34000;
        HasYelled = false;
        HasAura = false;
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

                if(!HasYelled)
                {
                    switch (rand()%6)
                    {
                    case 0:
                        DoYell(SAY_0,LANG_UNIVERSAL,NULL);
                        break;
                    case 1:
                        DoYell(SAY_1,LANG_UNIVERSAL,NULL);
                        break;
                    case 2:
                        DoYell(SAY_2,LANG_UNIVERSAL,NULL);
                        break;
                    case 3:
                        DoYell(SAY_3,LANG_UNIVERSAL,NULL);
                        break;
                    case 4:
                        DoYell(SAY_4,LANG_UNIVERSAL,NULL);
                        break;
                    case 5:
                        DoYell(SAY_5,LANG_UNIVERSAL,NULL);
                        break;
                    }
                    HasYelled = true;
                }

                if (!HasAura)
                {
                    DoCast(m_creature,SPELL_UNHOLYAURA_AURA);
                    HasAura = true;
                }

                //Begin melee attack if we are within range
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

            //UnholyAura
            if (UnholyAura_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_UNHOLYAURA);
                //2.5 seconds until we should cast this again
                UnholyAura_Timer = 2500;
            }else UnholyAura_Timer -= diff;

            //ShadowBolt
            if (ShadowBolt_Timer < diff)
            {
                //Cast
                if (rand()%100 < 50) //50% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_SHADOWBOLT);
                }
                //10 seconds until we should cast this again
                ShadowBolt_Timer = 10000;
            }else ShadowBolt_Timer -= diff;

            //Cleave
            if (Cleave_Timer < diff)
            {
                //Cast
                if (rand()%100 < 25) //25% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_CLEAVE);
                }
                //13 seconds until we should cast this again
                Cleave_Timer = 12000;
            }else Cleave_Timer -= diff;

            //MortalStrike
            if (MortalStrike_Timer < diff)
            {
                //Cast
                if (rand()%100 < 20) //20% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_MORTALSTRIKE);
                }
                //16 seconds until we should cast this again
                MortalStrike_Timer = 16000;
            }else MortalStrike_Timer -= diff;

            //RaiseDead
            //            if (RaiseDead_Timer < diff)
            //            {
            //Cast
            //                DoCast(m_creature,SPELL_RAISEDEAD);
            //				DoSay("summon triggered",LANG_UNIVERSAL,NULL); //just a checkpoint
            //45 seconds until we should cast this again
            //                RaiseDead_Timer = 45000;
            //            }else RaiseDead_Timer -= diff;

            //SummonSkeletons
            //Creature* Unit::SummonCreature(uint32 id, float x, float y, float z, float ang,TempSummonType spwtype,uint32 despwtime);

            if (SummonSkeletons_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);

                //Cast
                Summoned = m_creature->SummonCreature(11197,ADD_1X,ADD_1Y,ADD_1Z,ADD_1O,TEMPSUMMON_TIMED_DESPAWN,29000);
                ((CreatureAI*)Summoned->AI())->AttackStart(target);
                Summoned = m_creature->SummonCreature(11197,ADD_2X,ADD_2Y,ADD_2Z,ADD_2O,TEMPSUMMON_TIMED_DESPAWN,29000);
                ((CreatureAI*)Summoned->AI())->AttackStart(target);
                Summoned = m_creature->SummonCreature(11197,ADD_3X,ADD_3Y,ADD_3Z,ADD_3O,TEMPSUMMON_TIMED_DESPAWN,29000);
                ((CreatureAI*)Summoned->AI())->AttackStart(target);
                Summoned = m_creature->SummonCreature(11197,ADD_4X,ADD_4Y,ADD_4Z,ADD_4O,TEMPSUMMON_TIMED_DESPAWN,29000);
                ((CreatureAI*)Summoned->AI())->AttackStart(target);
                Summoned = m_creature->SummonCreature(11197,ADD_5X,ADD_5Y,ADD_5Z,ADD_5O,TEMPSUMMON_TIMED_DESPAWN,29000);
                ((CreatureAI*)Summoned->AI())->AttackStart(target);
                Summoned = m_creature->SummonCreature(11197,ADD_6X,ADD_6Y,ADD_6Z,ADD_6O,TEMPSUMMON_TIMED_DESPAWN,29000);
                ((CreatureAI*)Summoned->AI())->AttackStart(target);

                //34 seconds until we should cast this again
                SummonSkeletons_Timer = 40000;
            }else SummonSkeletons_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_baron_rivendare(Creature *_Creature)
{
    return new boss_baron_rivendareAI (_Creature);
}


void AddSC_boss_baron_rivendare()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_baron_rivendare";
    newscript->GetAI = GetAI_boss_baron_rivendare;
    m_scripts[nrscripts++] = newscript;
}
