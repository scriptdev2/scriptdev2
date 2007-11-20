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
SDName: Boss_Overlord_Wyrmthalak
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_BLASTWAVE                11130   
#define SPELL_SHOUT             23511     
#define SPELL_CLEAVE            20691
#define SPELL_KNOCKAWAY                 20686

#define ADD_1X -39.355381
#define ADD_1Y -513.456482
#define ADD_1Z 88.472046
#define ADD_1O 4.679872

#define ADD_2X -49.875881
#define ADD_2Y -511.896942
#define ADD_2Z 88.195160
#define ADD_2O 4.613114    

struct MANGOS_DLL_DECL boss_overlordwyrmthalakAI : public ScriptedAI
{
    boss_overlordwyrmthalakAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 BlastWave_Timer;
    uint32 Shout_Timer;
    uint32 Cleave_Timer;
    uint32 Knockaway_Timer;
    bool Summoned;
    bool InCombat;
    Creature *SummonedCreature;

    void EnterEvadeMode()
    {       
        BlastWave_Timer = 20000;
        Shout_Timer = 2000;
        Cleave_Timer = 6000;
        Knockaway_Timer = 12000;
        Summoned = false;
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

                DoStartMeleeAttack(who);
                InCombat = true;

            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //BlastWave_Timer
        if (BlastWave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_BLASTWAVE);

            //20 seconds
            BlastWave_Timer = 20000;
        }else BlastWave_Timer -= diff;

        //Shout_Timer
        if (Shout_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SHOUT);

            //10 seconds until we should cast this agian
            Shout_Timer = 10000;
        }else Shout_Timer -= diff;


        //Cleave_Timer
        if (Cleave_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            //7 seconds until we should cast this agian
            Cleave_Timer = 7000;
        }else Cleave_Timer -= diff;

        //Knockaway_Timer
        if (Knockaway_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_KNOCKAWAY);

            //14 seconds until we should cast this agian
            Knockaway_Timer = 14000;
        }else Knockaway_Timer -= diff;


        //Summon two Beserks
        if ( !Summoned && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 51 )
        {

            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            //Cast
            SummonedCreature = m_creature->SummonCreature(9216,ADD_1X,ADD_1Y,ADD_1Z,ADD_1O,TEMPSUMMON_TIMED_DESPAWN,300000);
            ((CreatureAI*)SummonedCreature->AI())->AttackStart(target);
            SummonedCreature = m_creature->SummonCreature(9268,ADD_2X,ADD_2Y,ADD_2Z,ADD_2O,TEMPSUMMON_TIMED_DESPAWN,300000);
            ((CreatureAI*)SummonedCreature->AI())->AttackStart(target);
            Summoned = true; 
        }

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_boss_overlordwyrmthalak(Creature *_Creature)
{
    return new boss_overlordwyrmthalakAI (_Creature);
}


void AddSC_boss_overlordwyrmthalak()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_overlord_wyrmthalak";
    newscript->GetAI = GetAI_boss_overlordwyrmthalak;
    m_scripts[nrscripts++] = newscript;
}
