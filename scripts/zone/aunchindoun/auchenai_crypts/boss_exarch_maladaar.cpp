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
SDName: Boss_Exarch_maladaar
SD%Complete: 100
SDComment: 
SDCategory: Auchindoun, Auchenai Crypts
EndScriptData */

#include "precompiled.h"

#define SPELL_RIBBON_OF_SOULS       32422
#define SPELL_FEAR                  32421
#define SPELL_SUMMON_DARK_SIDE      32346 
#define SPELL_SOUL_STEAL            32395 
#define SPELL_AVATAR_OF_MARTYRED    32424
#define SPELL_MORTAL_STRIKE         32736

#define SAY_INTRO                   "You have defiled the resting place of our ancestors. For this offense, there can be but one punishment. It is fitting that you have come to a place of the dead... for you will soon be joining them."
#define SOUND_INTRO                 10509
#define SAY_SUMMON                  "Rise my fallen brothers. Take form and fight!"
#define SOUND_SUMMON                10512

#define SAY_AGGRO_1                 "You will pay with your life!"
#define SOUND_AGGRO_1               10513
#define SAY_AGGRO_2                 "There's no turning back now!"
#define SOUND_AGGRO_2               10514
#define SAY_AGGRO_3                 "Serve your penitence!"
#define SOUND_AGGRO_3               10515

#define SAY_ROAR                    "Let your mind be clouded."
#define SOUND_ROAR                  10510
#define SAY_SOUL_CLEAVE             "Stare into the darkness of your soul."
#define SOUND_SOUL_CLEAVE           10511

#define SAY_SLAY_1                  "These walls will be your doom."
#define SOUND_SLAY_1                10516
#define SAY_SLAY_2                  "<laugh> Now, you'll stay for eternity!"
#define SOUND_SLAY_2                10517

#define SAY_DEATH                   "This is... where.. I belong..."
#define SOUND_DEATH                 10518

//Exarch Maladaar
struct MANGOS_DLL_DECL boss_exarch_maladaarAI : public ScriptedAI
{
    boss_exarch_maladaarAI(Creature *c) : ScriptedAI(c) {Reset();}    

    uint32 Fear_timer;
    uint32 Ribbon_of_Souls_timer;
    uint32 Summon_Dark_Side_timer;
   
    bool HasTaunted;
    bool HasYelled;
    bool Avatar_summoned;
    
    void Reset()
    {   
        Fear_timer = 20000;
        Ribbon_of_Souls_timer = 5000;
        Summon_Dark_Side_timer = 15000;

        HasTaunted = false;
        HasYelled = false;
        Avatar_summoned = false;
        
    }

    void JustDied(Unit* Killer)
    { 
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
        DoSpawnCreature(19412,0,0,0,0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000); //When Exarch Maladar is defeated D'ore appear.
    }

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

    void Aggro(Unit *who)
    {
                switch(rand()%3)
                {
                case 0:
                    DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                    break;

                case 1:
                    DoYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
                    break; 

                case 2:
                    DoYell(SAY_AGGRO_3, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_3);
                    break;
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
                DoStartAttackAndMovement(who);

                Aggro(who);
            }
            else if (!HasTaunted && m_creature->IsWithinDistInMap(who, 150.0f))
            {
                DoYell(SAY_INTRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature,SOUND_INTRO);

                HasTaunted = true;
            }
        }
    }

   
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
            
        if(((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 25) && !Avatar_summoned)
        {
            DoYell(SAY_SUMMON, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SUMMON);
            DoCast(m_creature, SPELL_AVATAR_OF_MARTYRED);
             Avatar_summoned = true;
        }
        
        if(Ribbon_of_Souls_timer < diff)
        {    
            if(!HasYelled)
            {
            DoYell(SAY_ROAR, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_ROAR);
            HasYelled = true;
            }

            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_RIBBON_OF_SOULS);
                Ribbon_of_Souls_timer = 5000 + (rand()%20 * 1000);
            }
        }else Ribbon_of_Souls_timer -= diff;

        if(Summon_Dark_Side_timer < diff && !Avatar_summoned)//When Avatar of Martyred summoned Exarch Maladar don't cast this spell
        {
            DoYell(SAY_SOUL_CLEAVE, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SOUL_CLEAVE);
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
            // DoCast(target,SPELL_SOUL_STEAL);
            DoCast(target,SPELL_SUMMON_DARK_SIDE);
            Summon_Dark_Side_timer = 5000 + (rand()%10 * 5000);
            }
            
        }else Summon_Dark_Side_timer -= diff;

        if(Fear_timer < diff)
        {
            DoCast(m_creature,SPELL_FEAR);
            Fear_timer = 20000 + rand()% 10000;

        }else Fear_timer -= diff;
        
       
        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_exarch_maladaarAI(Creature *_Creature)
{
    return new boss_exarch_maladaarAI (_Creature);
}

//Avatar of Martyred

struct MANGOS_DLL_DECL mob_avatar_of_martyredAI : public ScriptedAI
{
    mob_avatar_of_martyredAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 Mortal_Strike_timer;

    void Reset()
    {   

        Mortal_Strike_timer = 10000;
     }
    
    void Aggro(Unit *who)
    {
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
                DoStartAttackAndMovement(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
                   
        if(Mortal_Strike_timer < diff)
        {     
                DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE);
                Mortal_Strike_timer = 10000 + rand()%20 * 1000;
           
        }else Mortal_Strike_timer -= diff;
        
        DoMeleeAttackIfReady();
    }
 
};

CreatureAI* GetAI_mob_avatar_of_martyredAI(Creature *_Creature)
{
    return new mob_avatar_of_martyredAI (_Creature);
}
     


void AddSC_boss_exarch_maladaar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_exarch_maladaar";
    newscript->GetAI = GetAI_boss_exarch_maladaarAI;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="mob_avatar_of_martyred";
    newscript->GetAI = GetAI_mob_avatar_of_martyredAI;
    m_scripts[nrscripts++] = newscript;
     
}
