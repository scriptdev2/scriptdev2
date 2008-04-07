/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_veknilash
SD%Complete: 75
SDComment: HP not linked. Spells buggy.
EndScriptData */

#include "sc_creature.h"
#include "def_temple_of_ahnqiraj.h"

#define SPELL_HEAL_BROTHER          7393
#define SPELL_TWIN_TELEPORT         800     //Visual only

#define SPELL_UPPERCUT              26007
#define SPELL_UNBALANCING_STRIKE    26613
#define SPELL_MUTILATE_BUG          802     //Buggy. Same as Golemaggs trust spell. Its casted on gamers not on the right creature...



//8660 - Death - Feel
//8661 - Aggro - Let none
//8661 - Kill - your fate
//8630 - ah lambs
//8631 - join me
//8632 - let none
//8633 - look brother
//8634 - shall be
//8635 - your fate
//8636 - feel

struct MANGOS_DLL_DECL boss_veknilashAI : public ScriptedAI
{
    boss_veknilashAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 Heal_Timer;
    uint32 Teleport_Timer;
    uint32 UpperCut_Timer;
    uint32 UnbalancingStrike_Timer;
    uint32 Scarabs_Timer;
    int Rand;
    int RandX;
    int RandY;

    Creature* Summoned;


    void Reset()
    {
        Heal_Timer = 25000 + rand()%15000;
        Teleport_Timer = 12000 + rand()%12000;
        UpperCut_Timer = 14000 + rand()%15000;
        UnbalancingStrike_Timer = 8000 + rand()%10000;
        Scarabs_Timer = 7000 + rand()%7000;

        m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, IMMUNE_DAMAGE_MAGIC, true);  //Added. Can be removed if its included in DB.

//        m_creature->RemoveAllAuras();
//        m_creature->DeleteThreatList();
//        m_creature->CombatStop();
//        DoGoHome();
    }

    void JustDied(Unit* Killer)
    {
    }

    void SummonScarabs(Unit* victim)
    {
        Rand = 10 + (rand()%5);
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = NULL;
        Rand = 10 + (rand()%5);
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }		
        Rand = 0;
        Summoned = DoSpawnCreature(15316, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
        if(Summoned)
	    {
	        DoCast(Summoned,SPELL_MUTILATE_BUG);
                ((CreatureAI*)Summoned->AI())->AttackStart(victim);
	    }
    }

    

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

            //UnbalancingStrike_Timer
            if (UnbalancingStrike_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_UNBALANCING_STRIKE);
                UnbalancingStrike_Timer = 8000+rand()%12000;
            }else UnbalancingStrike_Timer -= diff;

	    //If we are within range melee the target
            if( m_creature->getVictim() && m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                //Check if our attack isShock ready (swing timer)
                if( m_creature->isAttackReady() )
                {                 
                    //Send our melee swing and then reset our attack timer
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();

                   //UpperCut_Timer
                   if (UpperCut_Timer < diff)
                   {
                       DoCast(m_creature->getVictim(),SPELL_UPPERCUT);
                       DoResetThreat();
                       UpperCut_Timer = 15000+rand()%15000;
                   }else UpperCut_Timer -= diff;
                }
            }

            //Summon Scarabs
            if (Scarabs_Timer < diff)
            {                 
                SummonScarabs(SelectUnit(SELECT_TARGET_RANDOM,0));				
                Scarabs_Timer = 10000+rand()%5000;
            }else Scarabs_Timer -= diff;

            //Casting Heal to brother
            if(Heal_Timer < diff)
            {
                    if(pInstance)
                    {    

                     Unit *pVeklor = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_VEKLOR));
                     DoCast(pVeklor, SPELL_HEAL_BROTHER);


                    }
                       
                Heal_Timer = 3000;
            }else Heal_Timer -= diff;

            //Teleporting to brother
            if(Teleport_Timer < diff)
            {
                if(pInstance)
                {    
                    DoCast(m_creature, SPELL_TWIN_TELEPORT);
                    Unit *pVeklor = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_VEKLOR));
                    m_creature->Relocate(pVeklor->GetPositionX(), pVeklor->GetPositionY(), pVeklor->GetPositionZ(), 0);    
                    DoResetThreat();   

                    Teleport_Timer = 30000;
                }else Teleport_Timer -= diff;
            }



            DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_veknilash(Creature *_Creature)
{
    return new boss_veknilashAI (_Creature);
}


void AddSC_boss_veknilash()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_veknilash";
    newscript->GetAI = GetAI_boss_veknilash;
    m_scripts[nrscripts++] = newscript;
}
