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
SDName: Boss Gatewatcher Ironhand
SD%Complete: 100
SDComment:
EndScriptData */

#include "../../../sc_defines.h"

// Spells to be casted
#define SPELL_SHADOW_POWER 35322
#define SPELL_JACKHAMMER 35326
#define SPELL_STREAM_OF_MACHINE_FLUID 35311

#define SAY_SPELL_JACKHAMMER_1   "With the precise angle and velocity... "
#define SOUND_SPELL_JACKHAMMER_1 11112
#define SAY_SPELL_JACKHAMMER_2  "Low tech yet quiet effective!"
#define SOUND_SPELL_JACKHAMMER_2 11113
// On Aggro
#define SAY_AGGRO_1          "You have approximately five seconds to live."
#define SOUND_SAY_AGGRO_1	11109 

//On Kill Unit
#define SAY_SLAY_1            "A foregone conclusion."
#define SOUND_SLAY_1 11110
#define SAY_SLAY_2 "The processing will continue a schedule!"
#define SOUND_SLAY_2 11111
// On Death
#define SAY_DEATH_1            "My calculations did not... "
#define SOUND_DEATH_1 11114  

// Gatewatcher Iron-Hand AI
struct MANGOS_DLL_DECL boss_gatewatcher_iron_handAI : public ScriptedAI
{	
    boss_gatewatcher_iron_handAI(Creature *c) : ScriptedAI(c)	{ EnterEvadeMode(); } 	

    uint32 Shadow_Power_Timer;	
    uint32 Jackhammer_Timer;
    uint32 Stream_of_Machine_Fluid_Timer;	

    bool InCombat;     

    void EnterEvadeMode()    
    {        
        m_creature->RemoveAllAuras();        
        m_creature->DeleteThreatList();        
        m_creature->CombatStop();
        DoGoHome();        
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);        
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);        
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);        
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);        
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);        
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);        
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);        
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);        

        Shadow_Power_Timer = 25000;		
        Jackhammer_Timer = 45000;
        Stream_of_Machine_Fluid_Timer = 55000;

        InCombat = false;

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
                if (!InCombat)                
                {					
                    DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);					
                    DoPlaySoundToSet(m_creature,SOUND_SAY_AGGRO_1); 					
                    InCombat = true;                
                }                 
                DoStartMeleeAttack(who);            
            }        
        }    
    } 	
    // On Killed Unit    
    void KilledUnit(Unit* victim)    
    {
        if (rand()%2)
            return;

        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_SLAY_1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY_1);
            break;

        case 1:
            DoYell(SAY_SLAY_2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY_2);
            break;        
        }
    }   

    // On Death    
    void JustDied(Unit* Killer)    
    {		
        DoYell(SAY_DEATH_1,LANG_UNIVERSAL,NULL);		
        DoPlaySoundToSet(m_creature, SOUND_DEATH_1);    
    }

    void UpdateAI(const uint32 diff)    
    {       
        //Return since we have no target        
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )            
            return; 		

        //Check for Shadow Power		
        if(Shadow_Power_Timer < diff)		
        {			
            //time to cast			
            DoCast(m_creature,SPELL_SHADOW_POWER);

            //Cast again on time			
            Shadow_Power_Timer = 25000;        		
        }else Shadow_Power_Timer -= diff; 		

        //Check for Jack Hammer		
        if(Jackhammer_Timer < diff)		
        {			
            //time to cast			
            DoCast(m_creature->getVictim(),SPELL_JACKHAMMER); 			

            if (rand()%2)
                return;

            switch(rand()%2)
            {
            case 0:
                DoYell(SAY_SPELL_JACKHAMMER_1, LANG_UNIVERSAL, NULL);					
                DoPlaySoundToSet(m_creature,SOUND_SPELL_JACKHAMMER_1);
                break;

            case 1:
                DoYell(SAY_SPELL_JACKHAMMER_2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature,SOUND_SPELL_JACKHAMMER_2);
                break;
            }		
            //Cast again on time			
            Jackhammer_Timer = 45000;        		
        }else Jackhammer_Timer -= diff;  		

        //Check for Stream of Machine Fluid		
        if(Stream_of_Machine_Fluid_Timer < diff)		
        {			
            //time to cast			
            DoCast(m_creature->getVictim(),SPELL_STREAM_OF_MACHINE_FLUID); 			

            //Cast again on time			
            Stream_of_Machine_Fluid_Timer = 55000;        		
        }else Stream_of_Machine_Fluid_Timer -= diff;

        DoMeleeAttackIfReady();
    } 
};   
CreatureAI* GetAI_boss_gatewatcher_iron_hand(Creature *_Creature)
{    
    return new boss_gatewatcher_iron_handAI (_Creature);
} 

void AddSC_boss_gatewatcher_iron_hand()
{    
    Script *newscript;    
    newscript = new Script;    
    newscript->Name="boss_gatewatcher_iron_hand";    
    newscript->GetAI = GetAI_boss_gatewatcher_iron_hand;    
    m_scripts[nrscripts++] = newscript;
}