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
SDName: Boss_Skeram
SD%Complete: 50
SDComment: Mind Control buggy. Spawns splitting too. Check function missing.
EndScriptData */

#include "../../sc_defines.h"
#include "def_temple_of_ahnqiraj.h"

#define SPELL_ARCANE_EXPLOSION      25679
#define SPELL_EARTH_SHOCK           26194       
#define SPELL_TRUE_FULFILLMENT4     26526
#define SPELL_BLINK                 28391

#define SOUND_AGGRO1       8615  //8615 Are you so eager to die? I would be happy to accomodate you.
#define SOUND_AGGRO2       8616  //8616 Cower mortals! The age of darkness is at hand.
#define SOUND_AGGRO3       8621  //8621 Tremble! The end is upon you.
#define SOUND_SLAY1        8617  //8617 Let your death serve as an example!
#define SOUND_SLAY2        8619  //8619 Spineless wretchers you will drown in rivers of blood!
#define SOUND_SLAY3        8620  //8620 The screams of the dying will fill the air. A symphony of terror is about to begin!
#define SOUND_SPLIT        8618  //8618 Prepare for the return of the ancient ones!
#define SOUND_DEATH        8622  //8622 You only delay... the inevetable



struct MANGOS_DLL_DECL boss_skeramAI : public ScriptedAI
{
    boss_skeramAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;
    
    uint32 ArcaneExplosion_Timer;
    uint32 EarthShock_Timer;
    uint32 FullFillment_Timer;
    uint32 Blink_Timer;
    uint32 Invisible_Timer;

    Creature *Image;

    
    bool InCombat;
    bool Images75;
    bool Images50;
    bool Images25;
    bool Invisible;

    void Reset()
    {
        ArcaneExplosion_Timer = 20000;
        EarthShock_Timer = 2000;
        FullFillment_Timer = 8000;
        Blink_Timer = 10000;
        Invisible_Timer = 2000;

        InCombat = false;
        Images75 = false;
        Images50 = false;
        Images25 = false;
        Invisible = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,15345);  // Skeram Model

//      m_creature->RemoveAllAuras();
//      m_creature->DeleteThreatList();
//      m_creature->CombatStop();
//      DoGoHome();
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

    void KilledUnit(Unit* victim)
    {
        switch(rand()%3)
        {
            case 0:  
                   DoPlaySoundToSet(m_creature,SOUND_SLAY1);
            break;

            case 1:
                   DoPlaySoundToSet(m_creature,SOUND_SLAY2);
            break;

            case 2:
                   DoPlaySoundToSet(m_creature,SOUND_SLAY3);
            break;
        }
    }
 

    void JustDied(Unit* Killer)
    {
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
      if (!InCombat)
            {
                    switch(rand()%3)
                    {
                        case 0:  
                               DoPlaySoundToSet(m_creature,SOUND_AGGRO1);
                        break;

                        case 1:
                               DoPlaySoundToSet(m_creature,SOUND_AGGRO2);
                        break;

                        case 2:
                               DoPlaySoundToSet(m_creature,SOUND_AGGRO3);
                        break;
                    }
                InCombat = true;
            }
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
                    switch(rand()%3)
                    {
                        case 0:  
                               DoPlaySoundToSet(m_creature,SOUND_AGGRO1);
                        break;

                        case 1:
                               DoPlaySoundToSet(m_creature,SOUND_AGGRO2);
                        break;

                        case 2:
                               DoPlaySoundToSet(m_creature,SOUND_AGGRO3);
                        break;
                    }

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

        //ArcaneExplosion_Timer
        if (ArcaneExplosion_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION);

            //10-25 seconds until we should cast this agian
            ArcaneExplosion_Timer = 10000 + rand()%15000;
        }else ArcaneExplosion_Timer -= diff;

        //If we are within range melee the target
        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            //Make sure our attack is ready and we arn't currently casting
            if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }else
        {
            //EarthShock_Timer
            if (EarthShock_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_EARTH_SHOCK);

                //1 seconds until we should cast this agian
                EarthShock_Timer = 1000;
            }else EarthShock_Timer -= diff;
        }

        //Blink_Timer
        if (Blink_Timer < diff)
        {
            //Cast
            //DoCast(m_creature, SPELL_BLINK);
            switch(rand()%3)
            {
                case 0:  
                       m_creature->Relocate(-8340.782227,2119.878418,1118.175102,0);
                       ResetThreat();
                break;

                case 1:
                       m_creature->Relocate(-8319.326172,2057.827637,118.175087,0);
                       ResetThreat();
                break;
                
                case 2:
                       m_creature->Relocate(-8349.873047,2079.848145,88.152451,0);
                       ResetThreat();
                break;
           }

            //20-40 seconds until we should cast this agian
            Blink_Timer= 20000 + rand()%20000;
        }else Blink_Timer -= diff;
        
        //Summoning 2 Images and teleporting to a random position on 75% health
        if ( (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5) == 75 && !Images75 )
        {
            DoPlaySoundToSet(m_creature,SOUND_SPLIT);
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            switch(rand()%3)
            {
                case 0:  
                       m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                       m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                       m_creature->Relocate(-8340.782227,2119.878418,1118.175102,0);
                       Invisible = true;
                       ResetThreat();

                       Image = m_creature->SummonCreature(15263,-8319.326172,2057.827637,118.175087,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 

                       Image = m_creature->SummonCreature(15263,-8349.873047,2079.848145,88.152451,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target);            
                break;

                case 1:
                       m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                       m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                       m_creature->Relocate(-8319.326172,2057.827637,118.175087,0);
                       Invisible = true;
                       ResetThreat();

                       Image = m_creature->SummonCreature(15263,-8340.782227,2119.878418,1118.175102,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 
                       
                       Image = m_creature->SummonCreature(15263,-8349.873047,2079.848145,88.152451,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 
                break;
                
                case 2:
                       m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                       m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                       m_creature->Relocate(-8349.873047,2079.848145,88.152451,0);
                       Invisible = true;
                       ResetThreat();

                       Image = m_creature->SummonCreature(15263,-8340.782227,2119.878418,1118.175102,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 

                       Image = m_creature->SummonCreature(15263,-8319.326172,2057.827637,118.175087,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 
                break;
           }
            Invisible = true;
            Images75 = true;
        }
        
        //Summoning 2 Images and teleporting to a random position on 50% health
        if ( (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5) == 50 && !Images50 )
        {
            DoPlaySoundToSet(m_creature,SOUND_SPLIT);
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
        
            switch(rand()%3)
            {
                case 0:  
                       m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                       m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                       m_creature->Relocate(-8340.782227,2119.878418,1118.175102,0);
                       Invisible = true;
                       ResetThreat();

                       Image = m_creature->SummonCreature(15263,-8319.326172,2057.827637,118.175087,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 

                       Image = m_creature->SummonCreature(15263,-8349.873047,2079.848145,88.152451,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target);                
                break;

                case 1:
                       m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                       m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                       m_creature->Relocate(-8319.326172,2057.827637,118.175087,0);
                       Invisible = true;
                       ResetThreat();

                       Image = m_creature->SummonCreature(15263,-8340.782227,2119.878418,1118.175102,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 

                       Image = m_creature->SummonCreature(15263,-8349.873047,2079.848145,88.152451,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 
                break;
                
                case 2:
                       m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                       m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                       m_creature->Relocate(-8349.873047,2079.848145,88.152451,0);
                       Invisible = true;
                       ResetThreat();

                       Image = m_creature->SummonCreature(15263,-8340.782227,2119.878418,1118.175102,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 

                       Image = m_creature->SummonCreature(15263,-8319.326172,2057.827637,118.175087,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 
                break;
           }
            Invisible = true;
            Images50 = true;
        }

        //Summoning 2 Images and teleporting to a random position on 25% health
        if ( (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5) == 25 && !Images25 )
        {
            DoPlaySoundToSet(m_creature,SOUND_SPLIT);
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
        
            switch(rand()%3)
            {
                case 0:  
                       m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                       m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                       m_creature->Relocate(-8340.782227,2119.878418,1118.175102,0);
                       Invisible = true;
                       ResetThreat();

                       Image = m_creature->SummonCreature(15263,-8319.326172,2057.827637,118.175087,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 

                       Image = m_creature->SummonCreature(15263,-8349.873047,2079.848145,88.152451,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target);                   
                break;

                case 1:
                       m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                       m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                       m_creature->Relocate(-8319.326172,2057.827637,118.175087,0);
                       Invisible = true;
                       ResetThreat();

                       Image = m_creature->SummonCreature(15263,-8340.782227,2119.878418,1118.175102,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 

                       Image = m_creature->SummonCreature(15263,-8349.873047,2079.848145,88.152451,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 
                break;
                
                case 2:
                       m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                       m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                       m_creature->Relocate(-8349.873047,2079.848145,88.152451,0);
                       Invisible = true;
                       ResetThreat();

                       Image = m_creature->SummonCreature(15263,-8340.782227,2119.878418,1118.175102,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 20);
                       Image->AI()->AttackStart(target); 
                       
                       Image = m_creature->SummonCreature(15263,-8319.326172,2057.827637,118.175087,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                       Image->SetMaxHealth(m_creature->GetMaxHealth() / 5);
                       Image->SetHealth(m_creature->GetHealth() / 5);
                       Image->AI()->AttackStart(target); 
                break;
           }
            Invisible = true;
            Images25 = true;
        }
        
        //Invisible_Timer
        if (Invisible && Invisible_Timer < diff)
        {
            //Making Skeram visible after telporting
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,15345);  // Skeram Model
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            //10-25 seconds until we should cast this agian
            Invisible_Timer = 2000;
            Invisible = false;
        }else Invisible_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 

CreatureAI* GetAI_boss_skeram(Creature *_Creature)
{
    return new boss_skeramAI (_Creature);
}


void AddSC_boss_skeram()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_skeram";
    newscript->GetAI = GetAI_boss_skeram;
    m_scripts[nrscripts++] = newscript;

}
