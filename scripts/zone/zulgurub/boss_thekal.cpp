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
SDName: boss_Thekal
SD%Complete: 95
SDComment: Almost finished.
EndScriptData */

#include "../../sc_defines.h"
#include "def_zulgurub.h"

#define SPELL_MORTALCLEAVE        22859
#define SPELL_SILENCE             23207
#define SPELL_FRENZY              23342         
#define SPELL_FORCEPUNCH          24189 
#define SPELL_CHARGE              24408
#define SPELL_ENRAGE              23537
#define SPELL_SUMMONTIGERS        24183
#define SPELL_TIGER_FORM          24169
#define SPELL_RESURRECT           24173            //We will not use this spell.

//Zealot Lor'Khan Spells
#define SPELL_SHIELD              25020   
#define SPELL_BLOODLUST           24185   
#define SPELL_GREATERHEAL         24208   
#define SPELL_DISARM              22691   

//Zealot Lor'Khan Spells
#define SPELL_SWEEPINGSTRIKES     18765   
#define SPELL_SINISTERSTRIKE      15667   
#define SPELL_GOUGE               24698   
#define SPELL_KICK                15614
#define SPELL_BLIND               21060    

#define SAY_AGGRO         "Shirvallah fill me with your rage!"
#define SAY_DEATH         "Hakkar binds me no more. Peace at last."

#define SOUND_AGGRO       8419
#define SOUND_DEATH       8424


struct MANGOS_DLL_DECL boss_thekalAI : public ScriptedAI
{
    boss_thekalAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    uint32 MortalCleave_Timer;
    uint32 Silence_Timer;
    uint32 Frenzy_Timer;
    uint32 ForcePunch_Timer;
    uint32 Charge_Timer;
    uint32 Enrage_Timer;
    uint32 SummonTigers_Timer;
    uint32 Check_Timer;
    uint32 Resurrect_Timer;

    ScriptedInstance *pInstance;
    bool InCombat;
    bool Enraged;
    bool PhaseTwo;
    bool WasDead;

    void EnterEvadeMode()
    {
        MortalCleave_Timer = 4000;
        Silence_Timer = 9000;
        Frenzy_Timer = 30000;
        ForcePunch_Timer = 4000;
        Charge_Timer = 12000;
        Enrage_Timer = 32000;
        SummonTigers_Timer = 25000;
        Check_Timer = 10000; 
        Resurrect_Timer = 10000;

        InCombat = false;
        Enraged = false;
        PhaseTwo = false;
        WasDead = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        if(pInstance)
            pInstance->SetData(DATA_THEKAL_ALIVE, 0);
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
            pInstance->SetData(DATA_THEKAL_DEATH, 0);
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

               
                    
                //Check_Timer for the death of LorKhan and Zath.
                if(!WasDead && Check_Timer < diff)
                {
                    if(pInstance)
                    {    

                    if(pInstance->GetData(DATA_LORKHANISDEAD))
                    {
                    //Resurrect LorKhan
                    Unit *pLorKhan = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_LORKHAN));
                    pLorKhan->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); 
                    pLorKhan->setFaction(14);
                    pLorKhan->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pLorKhan->SetHealth(int(pLorKhan->GetMaxHealth()*1.0));
                    }
                    
                    if(pInstance->GetData(DATA_ZATHISDEAD))
                    {
                    //Resurrect Zath
                    Unit *pZath = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_ZATH));
                    pZath->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); 
                    pZath->setFaction(14);
                    pZath->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pZath->SetHealth(int(pZath->GetMaxHealth()*1.0));
                    }
                    
                    }

                Check_Timer = 10000;
                }else Check_Timer -= diff;
        
        
                if (!PhaseTwo && MortalCleave_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_MORTALCLEAVE);
                    MortalCleave_Timer = 15000 + rand()%5000;
                }else MortalCleave_Timer -= diff;
                
                if (!PhaseTwo && Silence_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_SILENCE);
                    Silence_Timer = 20000 + rand()%5000;
                }else Silence_Timer -= diff;

        if (!PhaseTwo && !WasDead && m_creature->GetHealth() <= m_creature->GetMaxHealth() * 0.05)
        {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 3);
                m_creature->AttackStop();
                if(pInstance)
                pInstance->SetData(DATA_THEKALFAKE_DEATH, 0);
                WasDead=true;
        }

                //Thekal will transform to Tiger if he died and was not resurrected after 10 seconds.
                if(!PhaseTwo && WasDead)
                {
                    if (Resurrect_Timer < diff)
                    {
                        DoCast(m_creature,SPELL_TIGER_FORM);
                        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.00f);
                        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); 
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        m_creature->SetHealth(int(m_creature->GetMaxHealth()*1.0));                 
                        const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, (cinfo->mindmg +((cinfo->mindmg/100) * 40)));
                        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, (cinfo->maxdmg +((cinfo->maxdmg/100) * 40)));
                        m_creature->UpdateDamagePhysical(BASE_ATTACK);
                        ResetThreat();
                        PhaseTwo = true;
                    }else Resurrect_Timer -= diff;
                }
                

                if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth() == 100) && WasDead)
                {
                     WasDead = false;
                }

                if (PhaseTwo)
                {


                if (Charge_Timer < diff)
                {
                
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    
                    DoCast(target,SPELL_CHARGE);
                    m_creature->SendMonsterMove(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, true,1);
                    DoStartMeleeAttack(target);
                    ResetThreat();
                    
                    Charge_Timer = 15000 + rand()%7000;
                }else Charge_Timer -= diff;
                
                if (Frenzy_Timer < diff)
                {
                    DoCast(m_creature,SPELL_FRENZY);
                    Frenzy_Timer = 30000;
                }else Frenzy_Timer -= diff;
                
                if (ForcePunch_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_SILENCE);
                    ForcePunch_Timer = 16000 + rand()%5000;
                }else ForcePunch_Timer -= diff;
                                
                if (SummonTigers_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_SUMMONTIGERS);
                    SummonTigers_Timer = 10000 + rand()%4000;
                }else SummonTigers_Timer -= diff;

                if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 11) && !Enraged)
                {
                    DoCast(m_creature, SPELL_ENRAGE);
                    Enraged = true;
                }


            }
            DoMeleeAttackIfReady();
        }
    }
}; 

//Zealot Lor'Khan
struct MANGOS_DLL_DECL mob_zealot_lorkhanAI : public ScriptedAI
{
    mob_zealot_lorkhanAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }
    
    uint32 Shield_Timer;
    uint32 BloodLust_Timer;
    uint32 GreaterHeal_Timer;
    uint32 Disarm_Timer;
    uint32 Check_Timer;
    
    bool FakeDeath;
    
    ScriptedInstance *pInstance;
 
    void EnterEvadeMode()
    {
        Shield_Timer = 1000;
        BloodLust_Timer = 16000;
        GreaterHeal_Timer = 32000;
        Disarm_Timer = 6000;
        Check_Timer = 10000;
        
        FakeDeath = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        if(pInstance)
            pInstance->SetData(DATA_LORKHAN_ALIVE, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); 
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        
        
            
        
         
        
        
        
        
        
        
        
        
        
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
        
        //Shield_Timer
        if(Shield_Timer < diff)
        {

            DoCast(m_creature,SPELL_SHIELD);
            Shield_Timer = 61000;
        }else Shield_Timer -= diff;
        
        //BloodLust_Timer
        if(BloodLust_Timer < diff)
        {

            DoCast(m_creature,SPELL_BLOODLUST);
            BloodLust_Timer = 20000+rand()%8000;
        }else BloodLust_Timer -= diff;
        
        //Casting Greaterheal to Thekal or Zath if they are in meele range.
        if(GreaterHeal_Timer < diff)
        {
                    if(pInstance)
                    {    

                     Unit *pThekal = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_THEKAL));
                     Unit *pZath = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_ZATH));



                         switch(rand()%2)
                         {
                            case 0:  
                            if(m_creature->IsWithinDistInMap(pThekal, ATTACK_DISTANCE))
                            DoCast(pThekal, SPELL_GREATERHEAL);
                            break;

                            case 1:
                            if(m_creature->IsWithinDistInMap(pZath, ATTACK_DISTANCE))
                            DoCast(pZath, SPELL_GREATERHEAL);
                            break;
                        }


                    }
                       
            GreaterHeal_Timer = 15000+rand()%5000;
        }else GreaterHeal_Timer -= diff;
        
        //Disarm_Timer
        if(Disarm_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_DISARM);
            Disarm_Timer = 15000+rand()%10000;
        }else Disarm_Timer -= diff;
        
        //Check_Timer for the death of LorKhan and Zath.
        if(!FakeDeath && Check_Timer < diff)
            {
                    if(pInstance)
                    {    

                    if(pInstance->GetData(DATA_THEKALISFAKEDEAD))
                    {
                    //Resurrect Thekal
                    Unit *pThekal = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_THEKAL));
                    pThekal->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); 
                    pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pThekal->setFaction(14);
                    pThekal->SetHealth(int(pThekal->GetMaxHealth()*1.0));
                    }
                    
                    if(pInstance->GetData(DATA_ZATHISDEAD))
                    {
                    //Resurrect Zath
                    Unit *pZath = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_ZATH));
                    pZath->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); 
                    pZath->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pZath->setFaction(14);
                    pZath->SetHealth(int(pZath->GetMaxHealth()*1.0));
                    }
                    }

            Check_Timer = 10000;
        }else Check_Timer -= diff;
        
        if (m_creature->GetHealth() <= m_creature->GetMaxHealth() * 0.05)
        {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 3);
                m_creature->setFaction(35);
                m_creature->AttackStop();
                if(pInstance)
                pInstance->SetData(DATA_LORKHAN_DEATH, 0);
                FakeDeath = true;
        }

        DoMeleeAttackIfReady();
    }
};

//Zealot Zath
struct MANGOS_DLL_DECL mob_zealot_zathAI : public ScriptedAI
{
    mob_zealot_zathAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }
    
    uint32 SweepingStrikes_Timer;
    uint32 SinisterStrike_Timer;
    uint32 Gouge_Timer;
    uint32 Kick_Timer;
    uint32 Blind_Timer;
    uint32 Check_Timer;
    
    bool FakeDeath;
    
    ScriptedInstance *pInstance;
 
    void EnterEvadeMode()
    {
        SweepingStrikes_Timer = 13000;
        SinisterStrike_Timer = 8000;
        Gouge_Timer = 25000;
        Kick_Timer = 18000;
        Blind_Timer = 5000;
        Check_Timer = 10000;
        
        FakeDeath = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        if(pInstance)
            pInstance->SetData(DATA_ZATH_ALIVE, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); 
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        
        
            
           
        
        
        
        
        
        
        
        
        
        
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
        
        //SweepingStrikes_Timer
        if(SweepingStrikes_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_SWEEPINGSTRIKES);
            SweepingStrikes_Timer = 22000+rand()%4000;
        }else SweepingStrikes_Timer -= diff;
        
        //SinisterStrike_Timer
        if(SinisterStrike_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_SINISTERSTRIKE);
            SinisterStrike_Timer = 8000+rand()%8000;
        }else SinisterStrike_Timer -= diff;

        //Gouge_Timer
        if(Gouge_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_GOUGE);
            m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-100);
            Gouge_Timer = 17000+rand()%10000;
        }else Gouge_Timer -= diff;
        
        //Kick_Timer
        if(Kick_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_KICK);
            Kick_Timer = 15000+rand()%10000;
        }else Kick_Timer -= diff;
        
        //Blind_Timer
        if(Blind_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_BLIND);
            Blind_Timer = 10000+rand()%10000;
        }else Blind_Timer -= diff;
        
        //Check_Timer for the death of LorKhan and Zath.
        if(!FakeDeath && Check_Timer < diff)
            {
                    if(pInstance)
                    {    

                    if(pInstance->GetData(DATA_LORKHANISDEAD))
                    {
                    //Resurrect LorKhan
                    Unit *pLorKhan = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_LORKHAN));
                    pLorKhan->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); 
                    pLorKhan->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pLorKhan->setFaction(14);
                    pLorKhan->SetHealth(int(pLorKhan->GetMaxHealth()*1.0));
                    }
                    
                    if(pInstance->GetData(DATA_THEKALISFAKEDEAD))
                    {
                    //Resurrect Thekal
                    Unit *pThekal = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_THEKAL));
                    pThekal->SetUInt32Value(UNIT_FIELD_BYTES_1, 0); 
                    pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pThekal->setFaction(14);
                    pThekal->SetHealth(int(pThekal->GetMaxHealth()*1.0));
                    }
                    }

            Check_Timer = 10000;
        }else Check_Timer -= diff;
        
        if (m_creature->GetHealth() <= m_creature->GetMaxHealth() * 0.05)
        {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 3);
                m_creature->setFaction(35);
                m_creature->AttackStop();
                if(pInstance)
                pInstance->SetData(DATA_ZATH_DEATH, 0);
                FakeDeath = true;
        }

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_thekal(Creature *_Creature)
{
    return new boss_thekalAI (_Creature);
}
CreatureAI* GetAI_mob_zealot_lorkhan(Creature *_Creature)
{
    return new mob_zealot_lorkhanAI (_Creature);
}
CreatureAI* GetAI_mob_zealot_zath(Creature *_Creature)
{
    return new mob_zealot_zathAI (_Creature);
}

void AddSC_boss_thekal()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_thekal";
    newscript->GetAI = GetAI_boss_thekal;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_zealot_lorkhan";
    newscript->GetAI = GetAI_mob_zealot_lorkhan;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="mob_zealot_zath";
    newscript->GetAI = GetAI_mob_zealot_zath;
    m_scripts[nrscripts++] = newscript;

}
