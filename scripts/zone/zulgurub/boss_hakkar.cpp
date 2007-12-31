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
SDName: boss_hakkar
SD%Complete: 95
SDComment: No spell is working... Need to be fixed. Bloodsiphon will always do dmg.
EndScriptData */

#include "../../sc_defines.h"


#define SPELL_BLOODSIPHON            24322
#define SPELL_CORRUPTEDBLOOD         24328                  //Not working. Need to be fixed.
#define SPELL_CAUSEINSANITY          24327                  //Not working disabled.                      
#define SPELL_WILLOFHAKKAR           24178
#define SPELL_ENRAGE                 24318

// The Aspects of all High Priests
 
#define SPELL_ASPECT_OF_JEKLIK       24687   
#define SPELL_ASPECT_OF_VENOXIS      24688
#define SPELL_ASPECT_OF_MARLI        24686
#define SPELL_ASPECT_OF_THEKAL       24689
#define SPELL_ASPECT_OF_ARLOKK       24690


#define SAY_AGGRO         "PRIDE HERALDS THE END OF YOUR WORLD. COME, MORTALS! FACE THE WRATH OF THE SOULFLAYER!"
#define SAY_SLAY          "Fleeing will do you no good, mortals!" 

#define SOUND_AGGRO       8414

struct MANGOS_DLL_DECL boss_hakkarAI : public ScriptedAI
{
    boss_hakkarAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }
    
    uint32 BloodSiphon_Timer;
    uint32 CorruptedBlood_Timer;
    uint32 CauseInsanity_Timer;
    uint32 WillOfHakkar_Timer;
    uint32 Enrage_Timer;
    
    uint32 CheckJeklik_Timer;
    uint32 CheckVenoxis_Timer;
    uint32 CheckMarli_Timer;
    uint32 CheckThekal_Timer;
    uint32 CheckArlokk_Timer;
    
    uint32 AspectOfJeklik_Timer;
    uint32 AspectOfVenoxis_Timer;
    uint32 AspectOfMarli_Timer;
    uint32 AspectOfThekal_Timer;
    uint32 AspectOfArlokk_Timer;
    
    ScriptedInstance *pInstance;
    
    bool Enraged;
    bool InCombat;

    void EnterEvadeMode()
    {
        BloodSiphon_Timer = 90000;
        CorruptedBlood_Timer = 25000;
        CauseInsanity_Timer = 17000;
        WillOfHakkar_Timer = 17000;
        Enrage_Timer = 600000;
        
        CheckJeklik_Timer = 1000;
        CheckVenoxis_Timer = 2000;
        CheckMarli_Timer = 3000;
        CheckThekal_Timer = 4000;
        CheckArlokk_Timer = 5000;
    
        AspectOfJeklik_Timer = 8000;
        AspectOfVenoxis_Timer = 4000;
        AspectOfMarli_Timer = 22000;
        AspectOfThekal_Timer = 25000;
        AspectOfArlokk_Timer = 16000;        
        
        Enraged = false;
        InCombat = false;

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
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //BloodSiphon_Timer
        if (BloodSiphon_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_BLOODSIPHON);

            //90 seconds until we should cast this agian
            BloodSiphon_Timer = 90000;
        }else BloodSiphon_Timer -= diff;

        //CorruptedBlood_Timer
        if (CorruptedBlood_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CORRUPTEDBLOOD);

            //22-32 seconds until we should cast this agian
            CorruptedBlood_Timer = 30000 + rand()%15000;
        }else CorruptedBlood_Timer -= diff;


        //CauseInsanity_Timer
//        if (CauseInsanity_Timer < diff)
//        {
//
//            Unit* target = NULL;
//            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            //Cast
//            DoCast(target,SPELL_CAUSEINSANITY);

            //12-20 seconds until we should cast this agian
//            CauseInsanity_Timer = 35000 + rand()%8000;
//        }else CauseInsanity_Timer -= diff;

        //WillOfHakkar_Timer
        if (WillOfHakkar_Timer < diff)
        {

            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            //Cast
            DoCast(target,SPELL_WILLOFHAKKAR);

            //12-20 seconds until we should cast this agian
            WillOfHakkar_Timer = 25000 + rand()%10000;
        }else WillOfHakkar_Timer -= diff;

         
        if (!Enraged && Enrage_Timer < diff)
        {
            DoCast(m_creature, SPELL_ENRAGE);
            Enraged = true;
        }else Enrage_Timer -= diff;
        
        
        
        
        //Checking if Jeklik is dead. If not we cast her Aspect
        if(CheckJeklik_Timer < diff)
        {
            if(pInstance)
            {    
                if(!pInstance->GetData("JeklikIsDead"))
                {
                
                  if (AspectOfJeklik_Timer < diff)
                  {
                       //Cast
                       DoCast(m_creature->getVictim(),SPELL_ASPECT_OF_JEKLIK);

                       //14-21 seconds until we should cast this agian
                       AspectOfJeklik_Timer = 14000 + rand()%7000;
                 }else AspectOfJeklik_Timer -= diff;
                 
              }
            }

            CheckJeklik_Timer = 1000;
        }else CheckJeklik_Timer -= diff;
        
        //Checking if Venoxis is dead. If not we cast his Aspect
        if(CheckVenoxis_Timer < diff)
        {
            if(pInstance)
            {    
                if(!pInstance->GetData("VenoxisIsDead"))
                {
                
                  if (AspectOfVenoxis_Timer < diff)
                  {
                       //Cast
                       DoCast(m_creature->getVictim(),SPELL_ASPECT_OF_VENOXIS);

                       //17-20 seconds until we should cast this agian
                       AspectOfVenoxis_Timer = 17000 + rand()%3000;
                 }else AspectOfVenoxis_Timer -= diff;
                 
              }
            }

            CheckVenoxis_Timer = 1000;
        }else CheckVenoxis_Timer -= diff;
        
        //Checking if Marli is dead. If not we cast her Aspect
        if(CheckMarli_Timer < diff)
        {
            if(pInstance)
            {    
                if(!pInstance->GetData("MarliIsDead"))
                {
                
                  if (AspectOfMarli_Timer < diff)
                  {
                       //Cast
                       DoCast(m_creature->getVictim(),SPELL_ASPECT_OF_MARLI);

                       //12-21 seconds until we should cast this agian
                       AspectOfMarli_Timer = 12000 + rand()%9000;
                 }else AspectOfMarli_Timer -= diff;
                 
              }
            }

            CheckJeklik_Timer = 1000;
        }else CheckJeklik_Timer -= diff;
        
        //Checking if Thekal is dead. If not we cast his Aspect
        if(CheckThekal_Timer < diff)
        {
            if(pInstance)
            {    
                if(!pInstance->GetData("ThekalIsDead"))
                {
                
                  if (AspectOfThekal_Timer < diff)
                  {
                       //Cast
                       DoCast(m_creature,SPELL_ASPECT_OF_THEKAL);

                       //14-21 seconds until we should cast this agian
                       AspectOfThekal_Timer = 30000;
                 }else AspectOfThekal_Timer -= diff;
                 
              }
            }

            CheckThekal_Timer = 1000;
        }else CheckThekal_Timer -= diff;
        
        //Checking if Arlokk is dead. If yes we cast her Aspect
        if(CheckArlokk_Timer < diff)
        {
            if(pInstance)
            {    
                if(!pInstance->GetData("ArlokkIsDead"))
                {
                
                  if (AspectOfArlokk_Timer < diff)
                  {
                       //Cast
                       DoCast(m_creature,SPELL_ASPECT_OF_ARLOKK);
                       ResetThreat();

                       //28-32 seconds until we should cast this agian
                       AspectOfArlokk_Timer = 28000 + rand()%4000;
                 }else AspectOfArlokk_Timer -= diff;
                 
              }
            }

            CheckArlokk_Timer = 1000;
        }else CheckArlokk_Timer -= diff;
        
        
        


        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_hakkar(Creature *_Creature)
{
    return new boss_hakkarAI (_Creature);
}


void AddSC_boss_hakkar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_hakkar";
    newscript->GetAI = GetAI_boss_hakkar;
    m_scripts[nrscripts++] = newscript;
}
