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

#define SPELL_CURSEOFBLOOD          24673
//#define SPELL_ILLUSION              17773

struct MANGOS_DLL_DECL boss_jandicebarovAI : public ScriptedAI
{
    boss_jandicebarovAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 CurseOfBlood_Timer;
    uint32 Illusion_Timer;
    //uint32 Illusioncounter;
    uint32 Invisible_Timer;
    bool InCombat;
    bool Invisible;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void EnterEvadeMode()
    {
        CurseOfBlood_Timer = 15000;
        Illusion_Timer = 30000;
        Invisible_Timer = 3000;                             //Too much too low?
        InCombat = false;
        Invisible = false;

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

    void SummonIllusions(Unit* victim)
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
        Summoned = DoSpawnCreature(11439, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (Invisible && Invisible_Timer < diff)
        {
            //Become visible again 
            m_creature->setFaction(14);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11073);     //Jandice Model
            Invisible = false;
        } else if (Invisible)
               {
                  Invisible_Timer -= diff;
                  //Do nothing while invisible
                  return;
               }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {

            //CurseOfBlood_Timer
            if (CurseOfBlood_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CURSEOFBLOOD);

                //45 seconds
                CurseOfBlood_Timer = 30000;
            }else CurseOfBlood_Timer -= diff;

            //Illusion_Timer
            if (!Invisible && Illusion_Timer < diff)
            {

                //Inturrupt any spell casting
                m_creature->InterruptSpell();
                //Root self
                DoCast(m_creature,23973);
                m_creature->setFaction(35);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                //Cast
                SummonIllusions(m_creature->getVictim());
                SummonIllusions(m_creature->getVictim());
                SummonIllusions(m_creature->getVictim());
                SummonIllusions(m_creature->getVictim());
                SummonIllusions(m_creature->getVictim());
                SummonIllusions(m_creature->getVictim());
                SummonIllusions(m_creature->getVictim());
                SummonIllusions(m_creature->getVictim());
                SummonIllusions(m_creature->getVictim());
                SummonIllusions(m_creature->getVictim());
                Invisible = true;
                Invisible_Timer = 3000;

                //25 seconds until we should cast this agian
                Illusion_Timer = 25000;
            }else Illusion_Timer -= diff;


//            //Illusion_Timer
//            if (Illusion_Timer < diff)
//            {
//                  //Cast
//                DoCast(m_creature->getVictim(),SPELL_ILLUSION);
//                                
//                  //3 Illusion will be summoned
//                  if (Illusioncounter < 3)
//                  {
//                    Illusion_Timer = 500;
//                    Illusioncounter++;
//                  }
//                  else {
//                      //15 seconds until we should cast this again
//                      Illusion_Timer = 15000;
//                      Illusioncounter=0;
//                  }
//                  
//            }else Illusion_Timer -= diff;



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
CreatureAI* GetAI_boss_jandicebarov(Creature *_Creature)
{
    return new boss_jandicebarovAI (_Creature);
}


void AddSC_boss_jandicebarov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_jandice_barov";
    newscript->GetAI = GetAI_boss_jandicebarov;
    m_scripts[nrscripts++] = newscript;
}
