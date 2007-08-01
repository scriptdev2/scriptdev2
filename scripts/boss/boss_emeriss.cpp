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
//#define SPELL_MARKOFNATURE         25040   // Not working
#define SPELL_VOLATILEINFECTION    24928
#define SPELL_CORRUPTIONOFEARTH    24910



struct MANGOS_DLL_DECL boss_emerissAI : public ScriptedAI
{
    boss_emerissAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Sleep_Timer;
    uint32 NoxiousBreath_Timer;
    uint32 TailSweep_Timer;
    //uint32 MarkOfNature_Timer;
    uint32 VolatileInfection_Timer;
    uint32 CorruptionofEarth_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Sleep_Timer = 24000;
        NoxiousBreath_Timer = 12000;
        TailSweep_Timer = 4000;
        //MarkOfNature_Timer = 45000;
        VolatileInfection_Timer = 8000;
        CorruptionofEarth_Timer = 0;
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
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
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


            //Tailsweep every 2 seconds
            if (TailSweep_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);                    
                //Only cast if we are behind
                if (!m_creature->HasInArc( M_PI, target))
                    DoCast(target,SPELL_TAILSWEEP);
                    TailSweep_Timer = 2000;
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

            //VolatileInfection_Timer
            if (VolatileInfection_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_VOLATILEINFECTION);

                //24 seconds until we should cast this agian
                VolatileInfection_Timer = 24000;
            }else VolatileInfection_Timer -= diff;


            //CorruptionofEarth_Timer
            if ( (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5) == 75)
            {
                if (CorruptionofEarth_Timer < diff)
                {
                    //Cast
                   DoCast(m_creature->getVictim(),SPELL_CORRUPTIONOFEARTH);  


                    //1 minutes for next one. Means not again with this health value
                    CorruptionofEarth_Timer = 60000;
                } else CorruptionofEarth_Timer -= diff;
            }

            //CorruptionofEarth_Timer
            if ( (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5) == 50)
            {
                if (CorruptionofEarth_Timer < diff)
                {
                    //Cast
                   DoCast(m_creature->getVictim(),SPELL_CORRUPTIONOFEARTH);  

                    //1 minutes for next one. Means not again with this health value
                    CorruptionofEarth_Timer = 60000;
                } else CorruptionofEarth_Timer -= diff;
            }

            //CorruptionofEarth_Timer
            if ( (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5) == 25)
            {
                if (CorruptionofEarth_Timer < diff)
                {
                    //Cast
                   DoCast(m_creature->getVictim(),SPELL_CORRUPTIONOFEARTH);  

                    //1 minutes for next one. Means not again with this health value
                    CorruptionofEarth_Timer = 60000;
                } else CorruptionofEarth_Timer -= diff;
            }
        }
        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_emeriss(Creature *_Creature)
{
    return new boss_emerissAI (_Creature);
}


void AddSC_boss_emeriss()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_emeriss";
    newscript->GetAI = GetAI_boss_emeriss;
    m_scripts[nrscripts++] = newscript;
}
