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
SDName: boss_Jeklik
SD%Complete: 85
SDComment: Problem in finding the right flying batriders for spawning and making them fly.
EndScriptData */

#include "../../sc_defines.h"
#include "def_zulgurub.h"

#define SPELL_CHARGE              22911
#define SPELL_SONICBURST          23918  
#define SPELL_SCREECH             6605   
#define SPELL_SHADOW_WORD_PAIN    23952 
#define SPELL_MIND_FLAY           23953
#define SPELL_CHAIN_MIND_FLAY     23849      //Right ID unknown. So disabled
#define SPELL_GREATERHEAL         23954
#define SPELL_BAT_FORM            23966

// Batriders Spell

#define SPELL_BOMB                35276      //Wrong I think but real visual and dmg. School dmg missing.

#define SAY_AGGRO         "Lord Hireek grant me wings of vengance!"
#define SAY_DEATH         "Hireek - Finnaly death. Curse you Hakkar! Curse you!"

#define SOUND_AGGRO       8417
#define SOUND_DEATH       8422

struct MANGOS_DLL_DECL boss_jeklikAI : public ScriptedAI
{
    boss_jeklikAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Charge_Timer;
    uint32 SonicBurst_Timer;
    uint32 Screech_Timer;
    uint32 SpawnBats_Timer;
    uint32 ShadowWordPain_Timer;
    uint32 MindFlay_Timer;
    uint32 ChainMindFlay_Timer;
    uint32 GreaterHeal_Timer;
    uint32 SpawnFlyingBats_Timer;

    Creature *Bat;
    Creature *FlyingBat;
    bool InCombat;
    bool PhaseTwo;

    void EnterEvadeMode()
    {
        Charge_Timer = 20000;
        SonicBurst_Timer = 8000;
        Screech_Timer = 13000;
        SpawnBats_Timer = 60000;
        ShadowWordPain_Timer = 6000;
        MindFlay_Timer = 11000;
        ChainMindFlay_Timer = 26000;
        GreaterHeal_Timer = 50000;
        SpawnFlyingBats_Timer = 10000;

        InCombat = false;
        PhaseTwo = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
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
                DoCast(m_creature,SPELL_BAT_FORM);
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
            pInstance->SetData(DATA_JEKLIK_DEATH, 0);

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
                DoCast(m_creature,SPELL_BAT_FORM);
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
            if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 50))
            {

                if (Charge_Timer < diff)
                {
                
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    
                    DoCast(target,SPELL_CHARGE);
                    m_creature->SendMonsterMove(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, true,1);
                    DoStartMeleeAttack(target);
                    
                    Charge_Timer = 25000 + rand()%5000;
                }else Charge_Timer -= diff;
                
                if (SonicBurst_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_SONICBURST);
                    SonicBurst_Timer = 10000 + rand()%5000;
                }else SonicBurst_Timer -= diff;

                if (Screech_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_SCREECH);
                    Screech_Timer = 18000 + rand()%15000;
                }else Screech_Timer -= diff;


                if (SpawnBats_Timer < diff)
                {

                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    
                    Bat = m_creature->SummonCreature(11368,-12291.6220,-1380.2640,144.8304,5.483, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Bat ) { Bat ->AI()->AttackStart(target); }
                    Bat = m_creature->SummonCreature(11368,-12289.6220,-1380.2640,144.8304,5.483, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Bat ) { Bat ->AI()->AttackStart(target); }
                    Bat = m_creature->SummonCreature(11368,-12293.6220,-1380.2640,144.8304,5.483, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Bat ) { Bat ->AI()->AttackStart(target); }
                    Bat = m_creature->SummonCreature(11368,-12291.6220,-1380.2640,144.8304,5.483, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Bat ) { Bat ->AI()->AttackStart(target); }
                    Bat = m_creature->SummonCreature(11368,-12289.6220,-1380.2640,144.8304,5.483, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Bat ) { Bat ->AI()->AttackStart(target); }
                    Bat = m_creature->SummonCreature(11368,-12293.6220,-1380.2640,144.8304,5.483, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(target && Bat ) { Bat ->AI()->AttackStart(target); }
                                                   
                    SpawnBats_Timer = 60000;
                }else SpawnBats_Timer -= diff;
                
                
            }
            else
            {
                if(!PhaseTwo)
                {
                    m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,15219);
                    ResetThreat();
                    PhaseTwo = true;
                }

                if(PhaseTwo && ShadowWordPain_Timer < diff)
                {

                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);

                    DoCast(target, SPELL_SHADOW_WORD_PAIN);
                    ShadowWordPain_Timer = 12000 + rand()%6000;
                }ShadowWordPain_Timer -=diff;
                
                if(PhaseTwo && MindFlay_Timer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_MIND_FLAY);
                    MindFlay_Timer = 10000;
                }MindFlay_Timer -=diff;
                
//                if(PhaseTwo && ChainMindFlay_Timer < diff)
//                {
//                    DoCast(m_creature->getVictim(), SPELL_CHAIN_MIND_FLAY);
//                    ChainMindFlay_Timer = 27000 + rand()%5000;
//                }ChainMindFlay_Timer -=diff;
                
                if(PhaseTwo && GreaterHeal_Timer < diff)
                {
                    DoCast(m_creature,SPELL_GREATERHEAL);
                    GreaterHeal_Timer = 25000 + rand()%15000;
                }GreaterHeal_Timer -=diff;
                
                if(PhaseTwo && SpawnFlyingBats_Timer < diff)
                {

                    Unit *target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                        
                    FlyingBat = m_creature->SummonCreature(14965, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ()+15, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if(FlyingBat)
                    {
                        if(target)
                            FlyingBat->AI()->AttackStart(target);
                    }
                         
                    SpawnFlyingBats_Timer = 10000 + rand()%8000;
                }SpawnFlyingBats_Timer -=diff;


            }
            DoMeleeAttackIfReady();
        }
    }
}; 

//Flying Bat
struct MANGOS_DLL_DECL mob_batriderAI : public ScriptedAI
{
    mob_batriderAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;
     
    uint32 Bomb_Timer;
    uint32 Check_Timer;
 
    void EnterEvadeMode()
    {
        m_creature->setFaction(14);
        Bomb_Timer = 2000;
        Check_Timer = 1000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
             DoStartRangedAttack(who);
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
     
        //Bomb_Timer
        if(Bomb_Timer < diff)
        {
            Unit *target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            //Casting Bomb on random target
            if(target)
                DoCast(target, SPELL_BOMB);

            Bomb_Timer = 7000;
        }else Bomb_Timer -= diff;

        //Check_Timer
        if(Check_Timer < diff)
        {
            if(pInstance)
            {    

                    if(pInstance->GetData(DATA_JEKLIKISDEAD))
                    //remove
                    m_creature->setDeathState(JUST_DIED);
                    m_creature->RemoveCorpse();
                    m_creature->setFaction(35);
            }

            Check_Timer = 1000;
        }else Check_Timer -= diff;

            DoMeleeAttackIfReady();

    }
};


CreatureAI* GetAI_boss_jeklik(Creature *_Creature)
{
    return new boss_jeklikAI (_Creature);
}
CreatureAI* GetAI_mob_batrider(Creature *_Creature)
{
    return new mob_batriderAI (_Creature);
}


void AddSC_boss_jeklik()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_jeklik";
    newscript->GetAI = GetAI_boss_jeklik;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_batrider";
    newscript->GetAI = GetAI_mob_batrider;
    m_scripts[nrscripts++] = newscript;

}
