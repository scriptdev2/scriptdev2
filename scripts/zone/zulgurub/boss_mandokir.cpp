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
SDName: boss_Mandokir
SD%Complete: 90
SDComment: Ohgan function needs improvements.
EndScriptData */


#include "../../sc_defines.h"
#include "def_zulgurub.h"
 
#define SPELL_CHARGE        24315
#define SPELL_CLEAVE        20691
#define SPELL_FEAR        29321
#define SPELL_WHIRLWIND        24236
#define SPELL_MORTAL_STRIKE    24573
#define SPELL_ENRAGE        23537
#define SPELL_WATCH        24314
#define SPELL_LEVEL_UP        24312

//Ohgans Spells

#define SPELL_SUNDERARMOR     24317     

#define SAY_AGGRO               "I'll feed your souls to Hakkar himself!" 
#define SAY_WATCH        "I'm keeping my eye on you"
#define SAY_KILL        "DING!"

#define SOUND_AGGRO             8413 

struct MANGOS_DLL_DECL boss_mandokirAI : public ScriptedAI
{
    boss_mandokirAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }
 
    uint32 Watch_Timer;
    uint32 TargetInRange;
    uint32 Cleave_Timer;
    uint32 Whirlwind_Timer;
    uint32 Fear_Timer;
    uint32 MortalStrike_Timer;
    uint32 Check_Timer;
    float targetX;
    float targetY;
    float targetZ;
    
    ScriptedInstance *pInstance;
        
    Creature *Raptor;
    Unit* watchTarget;
    Unit* target;
 
    bool endWatch;
    bool InCombat;
    bool someWatched;
    bool RaptorDead;
    bool CombatStart;
 
    void EnterEvadeMode()
    {
        Watch_Timer = 33000;
        Cleave_Timer = 7000;
        Whirlwind_Timer = 20000;
        Fear_Timer = 1000;
        MortalStrike_Timer = 1000;
        Check_Timer = 1000;

        targetX = 0.0;
        targetY = 0.0;
        targetZ = 0.0;
        TargetInRange = 0;

 
        watchTarget = NULL;
        target = NULL;
 
        InCombat = false;
        someWatched = false;
        endWatch = false;
        RaptorDead = false;
        CombatStart = false;
 
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(); 
        DoGoHome();
        DoCast(m_creature, 23243); 
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, true);
 
   }
 
    void KilledUnit(Unit* victim)
    {
        DoYell(SAY_KILL, LANG_UNIVERSAL, NULL);
        m_creature->CastSpell(m_creature, SPELL_LEVEL_UP, true);
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
 
                DoStartMeleeAttack(who);
                InCombat = true;
 
            }
        }
    }
 
     void UpdateAI(const uint32 diff)
    {

        //Checking if Ohgan is dead. If yes Mandokir will enrage.
        if(Check_Timer < diff)
        {
            if(pInstance)
            {    
                if (!RaptorDead)
                {
                    if(pInstance->GetData(DATA_OHGANISDEAD))
                    DoCast(m_creature, SPELL_ENRAGE);
                    RaptorDead = true;
                }

            }

            Check_Timer = 1000;
        }else Check_Timer -= diff;

        if (!m_creature->SelectHostilTarget())
            return;
 
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            if(!CombatStart) 
            {
                m_creature->Unmount();   //At combat Start Mandokir is mounted so we must unmount it first
                //And summon his raptor
                Raptor = m_creature->SummonCreature(14988, m_creature->getVictim()->GetPositionX(), m_creature->getVictim()->GetPositionY(), m_creature->getVictim()->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 35000);     
                CombatStart = true;
            }
        
            if (Watch_Timer < diff) //Every 20 Sec Mandokir will check this
            {
                if(watchTarget) //If someone is watched and If the Position of the watched target is different from the one stored, or are attacking, mandokir will charge him
                {
                    if( targetX != watchTarget->GetPositionX() || 
                        targetY != watchTarget->GetPositionY() ||
                        targetZ != watchTarget->GetPositionZ() ||
                        watchTarget->isAttacking())
                    {
                        if(m_creature->IsWithinDistInMap(target, ATTACK_DISTANCE))
                        {
                            DoCast(watchTarget,24316);
                        }
                        else
                        {
                            DoCast(watchTarget,SPELL_CHARGE);
                            m_creature->SendMonsterMove(watchTarget->GetPositionX(), watchTarget->GetPositionY(), watchTarget->GetPositionZ(), 0, true,1);
                            DoStartMeleeAttack(watchTarget);
                        }
                    }
                }
                someWatched = false;
                Watch_Timer = 20000;
            }else Watch_Timer -= diff;
 
 
            if ((Watch_Timer < 8000) && !someWatched) //8 sec(cast time + expire time) before the check for the watch effect mandokir will cast watch debuff on a random target
            {
                watchTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoYell(SAY_WATCH, LANG_UNIVERSAL, watchTarget);
                DoCast(watchTarget, SPELL_WATCH);
                someWatched = true;
                endWatch = true;
            }
 
            if ((Watch_Timer < 1000) && endWatch) //1 sec before the debuf expire, store the target position
            {

                targetX = watchTarget->GetPositionX();
                targetY = watchTarget->GetPositionY();
                targetZ = watchTarget->GetPositionZ();
                endWatch = false;
            }
 
            if(!someWatched)
            {
 

                //Cleave
        if (Cleave_Timer < diff)
        {

                DoCast(m_creature->getVictim(),SPELL_CLEAVE);

        //7 Seconds
        Cleave_Timer = 7000;
        }else Cleave_Timer -= diff;

                //Whirlwind
        if (Whirlwind_Timer < diff)
        {

                DoCast(m_creature,SPELL_WHIRLWIND);

        //18 Seconds
        Whirlwind_Timer = 18000;
        }else Whirlwind_Timer -= diff;


                //If more then 3 targets in melee range mandokir will cast fear
        if (Fear_Timer < diff)    
        {
                    
                TargetInRange = 0;
                
                for(int i=0; i<10; i++)
                {
                target = SelectUnit(SELECT_TARGET_TOPAGGRO,i);
                if(target)
                if(m_creature->IsWithinDistInMap(target, ATTACK_DISTANCE))
                TargetInRange++;
                }
                if(TargetInRange > 3)
                DoCast(m_creature->getVictim(),SPELL_FEAR);
                
                Fear_Timer = 4000;
                }else Fear_Timer -=diff;
                


                //Mortal Strike if target below 50% hp
                if (m_creature->getVictim()->GetHealth() < m_creature->getVictim()->GetMaxHealth()*0.5)
        {
           if (MortalStrike_Timer < diff)
           {
                               
                      DoCast(m_creature->getVictim(),SPELL_MORTAL_STRIKE);
                      
                      MortalStrike_Timer = 18000;
                   }else MortalStrike_Timer -= diff;
                }
                     

 
            }
            
            DoMeleeAttackIfReady();
        }
    }
}; 


//Ohgan
struct MANGOS_DLL_DECL mob_ohganAI : public ScriptedAI
{
    mob_ohganAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }
    
    uint32 SunderArmor_Timer;
    ScriptedInstance *pInstance;
 
    void EnterEvadeMode()
    {
        SunderArmor_Timer = 5000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
    }

    void AttackStart(Unit *who)
    {
        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
        }
    }
    
    void JustDied(Unit* Killer)
    {
        ScriptedInstance *pInstance = (m_creature->GetInstanceData()) ? ((ScriptedInstance*)m_creature->GetInstanceData()) : NULL;
        if(pInstance)
            pInstance->SetData(DATA_OHGAN_DEATH, 0);
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
            }
        }
    }
 
    void UpdateAI (const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;
     
        //SunderArmor_Timer
        if(SunderArmor_Timer < diff)
        {

           DoCast(m_creature->getVictim(), SPELL_SUNDERARMOR);
            
            SunderArmor_Timer = 10000 + rand()%5000;
        }else SunderArmor_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_mandokir(Creature *_Creature)
{
    return new boss_mandokirAI (_Creature);
}
CreatureAI* GetAI_mob_ohgan(Creature *_Creature)
{
    return new mob_ohganAI (_Creature);
}

void AddSC_boss_mandokir()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_mandokir";
    newscript->GetAI = GetAI_boss_mandokir;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_ohgan";
    newscript->GetAI = GetAI_mob_ohgan;
    m_scripts[nrscripts++] = newscript;

}
