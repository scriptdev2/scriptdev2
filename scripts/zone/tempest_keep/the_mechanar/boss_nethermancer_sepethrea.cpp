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
SDName: Boss Nethermancer Sepethrea
SD%Complete: 100
SDComment: 
SDCategory: Tempest Keep, The Mechanar
EndScriptData */

#include "sc_creature.h"
#include "TargetedMovementGenerator.h"

// Spells to be casted
#define SPELL_SUMMON_RAGIN_FLAMES       35275
#define SPELL_INFERNO                   19695
#define SPELL_FIRE_TAIL                 35278

#define SPELL_FROST_ATTACK              35263
#define SPELL_ARCANE_BLAST              35314
#define SPELL_DRAGONS_BREATH            35250
#define SPELL_KNOCKBACK                 37317
#define SPELL_SOLARBURN                 35267

#define SAY_SPELL_DRAGONS_BREATH_1      "Think you can take the heat?"
#define SOUND_SPELL_DRAGONS_BREATH_1    11189
#define SAY_SPELL_DRAGONS_BREATH_2      "Anar'endal dracon!"
#define SOUND_SPELL_DRAGONS_BREATH_2    11190
// On Aggro
#define SAY_AGGRO                       "Don't value your life very much, do you?"
#define SOUND_SAY_AGGRO                 11186

//On Kill Unit
#define SAY_SLAY                        "And don't come back!"
#define SOUND_SLAY                      11187

// On Death
#define SAY_DEATH                       "Anu... bala belore...alon."
#define SOUND_DEATH                     11192

struct MANGOS_DLL_DECL boss_nethermancer_sepethreaAI : public ScriptedAI
{
    boss_nethermancer_sepethreaAI(Creature *c) : ScriptedAI(c) { EnterEvadeMode(); } 

    uint32 frost_attack_Timer;
    uint32 arcane_blast_Timer;
    uint32 dragons_breath_Timer;
    uint32 knockback_Timer;
    uint32 solarburn_Timer;

    void Reset()    
    {
        frost_attack_Timer = 10000;        
        arcane_blast_Timer = 15000;
        dragons_breath_Timer = 20000;
        knockback_Timer = 25000;
        solarburn_Timer = 30000;
    }  

    void Aggro(Unit *who)    
    {        
    }     

    // On Killed Unit    
    void KilledUnit(Unit* victim)    
    {
        DoYell(SAY_SLAY, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_SLAY);
    }

    // On Death    
    void JustDied(Unit* Killer)    
    {        
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);  
    }

    void UpdateAI(const uint32 diff)
    {
        
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )            
            return;  

        //Check for Frost Attack        
        if(frost_attack_Timer < diff)        
        {            
            //time to cast            
            DoCast(m_creature->getVictim(),SPELL_FROST_ATTACK);

            //Cast again on time            
            frost_attack_Timer = 10000;                
        }else frost_attack_Timer -= diff;

        //Check for Arcane Blast        
        if(arcane_blast_Timer < diff)        
        {            
            //time to cast            
            DoCast(m_creature->getVictim(),SPELL_ARCANE_BLAST);

            //Cast again on time            
            arcane_blast_Timer = 15000;                
        }else arcane_blast_Timer -= diff;

        //Check for Dragons Breath        
        if(dragons_breath_Timer < diff)        
        {            
            //time to cast            
            DoCast(m_creature->getVictim(),SPELL_DRAGONS_BREATH);

            {

                if (rand()%2)
                    return;

                switch(rand()%2)
                {
                case 0:
                    DoYell(SAY_SPELL_DRAGONS_BREATH_1, LANG_UNIVERSAL, NULL);                    
                    DoPlaySoundToSet(m_creature,SOUND_SPELL_DRAGONS_BREATH_1);
                    break;

                case 1:
                    DoYell(SAY_SPELL_DRAGONS_BREATH_2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_SPELL_DRAGONS_BREATH_2);
                    break;
                }
            }
            //Cast again on time            
            dragons_breath_Timer = 20000;                

        }else dragons_breath_Timer -= diff;          

        //Check for Knockback        
        if(knockback_Timer < diff)        
        {            

            //time to cast            
            DoCast(m_creature->getVictim(),SPELL_KNOCKBACK);             

            //Cast again on time            
            knockback_Timer = 25000;                

        }else knockback_Timer -= diff;

        //Check for Solarburn        
        if(solarburn_Timer < diff)        

        {            

            //time to cast            
            DoCast(m_creature->getVictim(),SPELL_SOLARBURN);             

            //Cast again on time            
            solarburn_Timer = 30000;                

        }else solarburn_Timer -= diff;

        DoMeleeAttackIfReady();

    } 
};

CreatureAI* GetAI_boss_nethermancer_sepethrea(Creature *_Creature)
{    
    return new boss_nethermancer_sepethreaAI (_Creature);
}
struct MANGOS_DLL_DECL mob_ragin_flamesAI : public ScriptedAI
{
    mob_ragin_flamesAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 inferno_Timer;
    uint32 flame_timer;

    bool onlyonce;

    void Reset()
    {   
        inferno_Timer = 10000;
        flame_timer = 200;
        onlyonce = false;
    }

    void Aggro(Unit* who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        Unit* target = NULL;

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (!onlyonce)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*target));
            onlyonce = true;
        }

        if(inferno_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_INFERNO); 
            inferno_Timer = 10000;

            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*target));
        }else inferno_Timer -= diff;

        if(flame_timer < diff)
        {
            DoCast(m_creature,SPELL_FIRE_TAIL);
            flame_timer = 200;

        }else flame_timer -=diff;

        DoMeleeAttackIfReady();
    }

};
CreatureAI* GetAI_mob_ragin_flames(Creature *_Creature)
{
    return new mob_ragin_flamesAI (_Creature);
}
void AddSC_boss_nethermancer_sepethrea()
{    
    Script *newscript;    
    newscript = new Script;    
    newscript->Name="boss_nethermancer_sepethrea";    
    newscript->GetAI = GetAI_boss_nethermancer_sepethrea;    
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;    
    newscript->Name="mob_ragin_flames";    
    newscript->GetAI = GetAI_mob_ragin_flames;    
    m_scripts[nrscripts++] = newscript;
}
