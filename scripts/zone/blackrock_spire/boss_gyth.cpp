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

#define SPELL_CORROSIVEACID      20667
#define SPELL_FREEZE             20712
#define SPELL_FLAMEBREATH        18763


struct MANGOS_DLL_DECL boss_gythAI : public ScriptedAI
{
    boss_gythAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Aggro_Timer;
    uint32 Dragons_Timer;
    uint32 Orc_Timer;
    uint32 CorrosiveAcid_Timer;
    uint32 Freeze_Timer;
    uint32 Flamebreath_Timer;
    bool Event;
    bool SummonedDragons;
    bool SummonedOrcs;
    bool SummonedRend;
    bool Aggro;
    bool InCombat;
    bool RootSelf;
    Creature *SummonedCreature;

    void EnterEvadeMode()
    {
        Dragons_Timer = 3000;
        Orc_Timer = 30000;
        Aggro_Timer = 30000;
        CorrosiveAcid_Timer = 8000;
        Freeze_Timer = 20000;
        Flamebreath_Timer = 4000;
        Event = false;
        SummonedDragons = false;
        SummonedOrcs= false;
        SummonedRend = false;
        Aggro = false;
        InCombat = false;
        RootSelf = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  //Invisible for event start
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
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

    void UpdateAI(const uint32 diff)
    {
        if (Event)
        {
            if (!Aggro && Aggro_Timer < diff)
            {
                m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,9723);  // Visible now!
                m_creature->m_canMove = true; 
                m_creature->setFaction(14);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                Aggro = true;
            } Aggro_Timer -= diff;
        }

        if (!Event && !RootSelf)
        {
            //Root self
            m_creature->m_canMove = false; 
            RootSelf = true;
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {

            //Summon Dragon pack. 5 Dragons
            if (!Event && !SummonedDragons)
            {
                if (Dragons_Timer < diff) {

                    Unit* target = NULL;
                    Unit* SummonedLine1 = NULL;

                    SummonedLine1 = m_creature->SummonCreature(10372,m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    SummonedLine1 = m_creature->SummonCreature(10372,m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    SummonedLine1 = m_creature->SummonCreature(10442, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    SummonedLine1 = m_creature->SummonCreature(10442, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    SummonedLine1 = m_creature->SummonCreature(10442, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    if (SummonedLine1)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedLine1->AddThreat(target,1.0f);
                    }
                    SummonedDragons = true;
                } else Dragons_Timer -= diff;
            }


            //Summon Orc pack. 1 Orc Handler and 2 Elite Dragonkin
            if (!Event && SummonedDragons && !SummonedOrcs)
            {
                if (Orc_Timer < diff) {
                    Unit* target = NULL;
                    Unit* SummonedLine2 = NULL;

                    SummonedLine2 = m_creature->SummonCreature(10447, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    SummonedLine2 = m_creature->SummonCreature(10447, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    SummonedLine2 = m_creature->SummonCreature(10317, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                    if (SummonedLine2)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedLine2->AddThreat(target,1.0f);
                    }
                    SummonedOrcs = true;
                    Event = true;
                } else Orc_Timer -= diff;
            }


            //CorrosiveAcid_Timer
            if (Aggro && CorrosiveAcid_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CORROSIVEACID);

                //14 seconds
                CorrosiveAcid_Timer = 7000;
            } else CorrosiveAcid_Timer -= diff;

            //Freeze_Timer
            if (Aggro && Freeze_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FREEZE);

                //10 seconds until we should cast this agian
                Freeze_Timer = 16000;
            } else Freeze_Timer -= diff;


            //Flamebreath_Timer
            if (Aggro && Flamebreath_Timer < diff)
            {

                DoCast(m_creature->getVictim(),SPELL_FLAMEBREATH);

                //6 seconds until we should cast this agian
                Flamebreath_Timer = 10500;
            } else Flamebreath_Timer -= diff;



            //Summon Rend
            if (Aggro && !SummonedRend && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 11 )
            {

                // summon Rend and Change model to normal Gyth
                //Inturrupt any spell casting
                m_creature->InterruptSpell();
                m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,9806);     //Gyth model
                m_creature->SummonCreature(10429, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,900000);
                SummonedRend = true;
            }



            //If we are within range melee the target
            if( Aggro && m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
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

CreatureAI* GetAI_boss_gyth(Creature *_Creature)
{
    return new boss_gythAI (_Creature);
}


void AddSC_boss_gyth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gyth";
    newscript->GetAI = GetAI_boss_gyth;
    m_scripts[nrscripts++] = newscript;
}
