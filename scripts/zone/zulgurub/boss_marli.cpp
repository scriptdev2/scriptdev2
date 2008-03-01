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
SDName: boss_Marli
SD%Complete: 80
SDComment: Charging healers and casters not working. Perhaps wrong Spell Timers.
EndScriptData */

#include "../../sc_defines.h"
#include "def_zulgurub.h"

#define SPELL_CHARGE              22911
#define SPELL_ASPECT_OF_MARLI     24686      // A stun spell      
#define SPELL_ENVOLWINGWEB        24110 
#define SPELL_POISONVOLLEY        24099
#define SPELL_SPIDER_FORM         24084


//The Spider Spells
#define SPELL_LEVELUP             24312     //Not right Spell.

#define SAY_AGGRO         "Draw me to your web mistress Shadra. Unleash your venom!"
#define SAY_DEATH         "ShadraDeath - Bless you mortal for this release. Hakkar controls me no longer..."

#define SOUND_AGGRO       8418
#define SOUND_DEATH       8423


struct MANGOS_DLL_DECL boss_marliAI : public ScriptedAI
{
    boss_marliAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 SpawnStartSpiders_Timer;
    uint32 PoisonVolley_Timer;
    uint32 SpawnSpider_Timer;
    uint32 Charge_Timer;
    uint32 Aspect_Timer;
    uint32 Transform_Timer;
    uint32 TransformBack_Timer;

    Creature *Spider;
    bool InCombat;
    bool Spawned;
    bool PhaseTwo;

    void EnterEvadeMode()
    {
        SpawnStartSpiders_Timer = 1000;
        PoisonVolley_Timer = 15000;
        SpawnSpider_Timer = 30000;
        Charge_Timer = 2500;
        Aspect_Timer = 12000;
        Transform_Timer = 45000;
        TransformBack_Timer = 25000;

        InCombat = false;
        Spawned = false;
        PhaseTwo = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);    
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);   
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

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
            //Say our dialog on initial aggro
      if (!InCombat)
            {
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGRO);
                InCombat = true;
            }
        }
    }
    
    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
        ScriptedInstance *pInstance = (m_creature->GetInstanceData()) ? ((ScriptedInstance*)m_creature->GetInstanceData()) : NULL;
        if(pInstance)
            pInstance->SetData(DATA_MARLI_DEATH, 0);
    }
    
  void ResetThreat()
  {
    std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
 
    for(uint32 i = 0; i <= (m_threatlist.size()-1); i++)
    {
      Unit* pUnit = SelectUnit(SELECT_TARGET_TOPAGGRO, i);
      if(pUnit)
        (m_creature->getThreatManager()).modifyThreatPercent(pUnit, -99);
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
                      //Say our dialog on initial aggro
                if (!InCombat)
                {
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGRO);
                InCombat = true;
                }

            }
        }
    }

     void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget())
            return;

        if( m_creature->getVictim() && m_creature->isAlive())
        {

                if (PoisonVolley_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_POISONVOLLEY);
                    PoisonVolley_Timer = 15000 + rand()%10000;
                }else PoisonVolley_Timer -= diff;

                if (!PhaseTwo && Aspect_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_ASPECT_OF_MARLI);
                    Aspect_Timer = 18000 + rand()%5000;
                }else Aspect_Timer -= diff;


                if (!Spawned && SpawnStartSpiders_Timer < diff)
                {

                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    
                    Spider = m_creature->SummonCreature(15041,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Spider ) { Spider ->AI()->AttackStart(target); }
                    Spider = m_creature->SummonCreature(15041,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Spider ) { Spider ->AI()->AttackStart(target); }
                    Spider = m_creature->SummonCreature(15041,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Spider ) { Spider ->AI()->AttackStart(target); }
                    Spider = m_creature->SummonCreature(15041,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Spider ) { Spider ->AI()->AttackStart(target); }
                                                   
                    Spawned = true;
                }else SpawnStartSpiders_Timer -= diff;

                if (SpawnSpider_Timer < diff)
                {

                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    
                    Spider = m_creature->SummonCreature(15041,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Spider ) { Spider ->AI()->AttackStart(target); }
                                                   
                    SpawnSpider_Timer = 12000 + rand()%5000;
                }else SpawnSpider_Timer -= diff;
                
           

                if(!PhaseTwo && Transform_Timer < diff)
                {
                    DoCast(m_creature,SPELL_SPIDER_FORM);   
  
                    const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                    m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, (cinfo->mindmg +((cinfo->mindmg/100) * 35)));
                    m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, (cinfo->maxdmg +((cinfo->maxdmg/100) * 35)));
                    m_creature->UpdateDamagePhysical(BASE_ATTACK);
                    DoCast(m_creature,SPELL_ENVOLWINGWEB); 
                    ResetThreat();
                    PhaseTwo = true;
                    Transform_Timer = 35000 + rand()%25000;
                }else Transform_Timer -= diff;
                



                if (PhaseTwo)
                {


                if (Charge_Timer < diff)
                {
                
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    
                    DoCast(target, SPELL_CHARGE);
                    m_creature->SendMonsterMove(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, true,1);
                    DoStartMeleeAttack(target);
                   
                    Charge_Timer = 8000;
                }else Charge_Timer -= diff;


                if (TransformBack_Timer < diff)
                {      
                    m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,15220);
                    const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                    m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, (cinfo->mindmg +((cinfo->mindmg/100) * 1)));
                    m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, (cinfo->maxdmg +((cinfo->maxdmg/100) * 1)));
                    m_creature->UpdateDamagePhysical(BASE_ATTACK);

                    PhaseTwo = false;                    
                    TransformBack_Timer = 25000 + rand()%15000;
                }else TransformBack_Timer -= diff;




            }
            DoMeleeAttackIfReady();
        }
    }
}; 

//Spawn of Marli
struct MANGOS_DLL_DECL mob_spawn_of_marliAI : public ScriptedAI
{
    mob_spawn_of_marliAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
    uint32 LevelUp_Timer;
 
    void EnterEvadeMode()
    {
        LevelUp_Timer = 3000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
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
     
        //LevelUp_Timer
        if(LevelUp_Timer < diff)
        {

            DoCast(m_creature,SPELL_LEVELUP);
            LevelUp_Timer = 3000;
        }else LevelUp_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_marli(Creature *_Creature)
{
    return new boss_marliAI (_Creature);
}
CreatureAI* GetAI_mob_spawn_of_marli(Creature *_Creature)
{
    return new mob_spawn_of_marliAI (_Creature);
}

void AddSC_boss_marli()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_marli";
    newscript->GetAI = GetAI_boss_marli;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_spawn_of_marli";
    newscript->GetAI = GetAI_mob_spawn_of_marli;
    m_scripts[nrscripts++] = newscript;

}
