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
SDName: Boss_DarkWeaver_Syth
SD%Complete: 100
SDComment: 
SDCategory: Auchindoun / Sethekk Halls
EndScriptData */

#include "sc_creature.h"

#define SPELL_FROST_SHOCK           37865
#define SPELL_FLAME_SHOCK           34354
#define SPELL_SHADOW_SHOCK          30138
#define SPELL_ARCANE_SHOCK          37132
#define SPELL_CHAIN_LIGHTNING       39945

#define SPELL_SUMMON_SYTH_FIRE      33537 // Spawns 19203
#define SPELL_SUMMON_SYTH_ARCANE    33538 // Spawns 19205
#define SPELL_SUMMON_SYTH_FROST     33539 // Spawns 19204
#define SPELL_SUMMON_SYTH_SHADOW    33540 // Spawns 19206

#define SPELL_FLAME_BUFFET          33526
#define SPELL_ARCANE_BUFFET         33527
#define SPELL_FROST_BUFFET          33528
#define SPELL_SHADOW_BUFFET         33529

#define SAY_SUMMON                  "I have pets..<squawk>..of my own!"
#define SOUND_SUMMON                10502

#define SAY_AGGRO_1                 "Hrrmm.. Time to.. hrrm.. make my move."
#define SOUND_AGGRO_1               10503
#define SAY_AGGRO_2                 "Nice pets..hrm.. Yes! <squawking>"
#define SOUND_AGGRO_2               10504
#define SAY_AGGRO_3                 "Nice pets have.. weapons. No so..<squawk>..nice."
#define SOUND_AGGRO_3               10505

#define SAY_SLAY_1                  "Death.. meeting life is.. <squawking>"
#define SOUND_SLAY_1                10506
#define SAY_SLAY_2                  "Uhn.. Be free..<squawk>"
#define SOUND_SLAY_2                10507

#define SAY_DEATH                   "No more life..hrm. No more pain. <squawks weakly>"
#define SOUND_DEATH                 10508


struct MANGOS_DLL_DECL boss_darkweaver_sythAI : public ScriptedAI
{
    boss_darkweaver_sythAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 flameshock_timer;
    uint32 arcaneshock_timer;
    uint32 frostshock_timer;
    uint32 shadowshock_timer;
    
    uint32 chainlightning_timer;
    
    bool summon75;
    bool summon50;
    bool summon25;
    
    //uint32 sythflame_timer;
    //uint32 sytharcane_timer;
    //uint32 sythfrost_timer;
    //uint32 sythshadow_timer;

    bool InCombat;

    void Reset()
    {   
        flameshock_timer = 2000;
        arcaneshock_timer = 3000;
        frostshock_timer = 4000;
        shadowshock_timer = 5000;
        chainlightning_timer = 10000;
        
        summon75 = false;
        summon50 = false;
        summon25 = false;
        InCombat = false;
    }

    void JustDied(Unit* Killer)
    { 
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
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

    float DoCalculateRandomLocation()
    {
        float Loc;
        float Rand = rand()%6;

        switch(rand()%2)
        {
            case 0:
            Loc = 0 + Rand;
            break;

            case 1:
            Loc = 0 - Rand;
            break;
        }

        return Loc;
    }

    void SythSummoning()
    {
        DoYell(SAY_SUMMON, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_SUMMON);
        Creature* Summoned = NULL;
        Unit* target = NULL;
          
        Summoned = DoSpawnCreature(19203,5,5,0,0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000);
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        if (Summoned && target)
                    Summoned->AI()->AttackStart(target);
        Summoned = DoSpawnCreature(19205,-5,5,0,0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000);
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        if (Summoned && target)
                    Summoned->AI()->AttackStart(target);
        Summoned = DoSpawnCreature(19204,-5,-5,0,0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000);
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        if (Summoned && target)
                    Summoned->AI()->AttackStart(target);
        Summoned = DoSpawnCreature(19206, 5,-5,0,0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000);
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        if (Summoned && target)
                    Summoned->AI()->AttackStart(target);
        //DoCast(m_creature,SPELL_SUMMON_SYTH_FIRE);
        //DoCast(m_creature,SPELL_SUMMON_SYTH_ARCANE);
        //DoCast(m_creature,SPELL_SUMMON_SYTH_FROST);
        //DoCast(m_creature,SPELL_SUMMON_SYTH_SHADOW);
        //SPELLS DOSENT WORK ... I TAKE SUMMAN INSTEAD ... WORKS IN SAME WAY
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
            
        if( ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 75) && !summon75)
        {
        SythSummoning();
        summon75 = true;
        }
        
        if( ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 50) && !summon50)
        {
        SythSummoning();
        summon50 = true;
        }
        
        if( ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 25) && !summon25)
        {
        SythSummoning();
        summon25 = true;
        }
        
        if(flameshock_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_FLAME_SHOCK);
                flameshock_timer = 10000 + rand()%5000;
            }
        }else flameshock_timer -= diff;
        
        if(arcaneshock_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_ARCANE_SHOCK);
                arcaneshock_timer = 10000 + rand()%5000;
            }
        }else arcaneshock_timer -= diff;
        
        if(frostshock_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_FROST_SHOCK);
                frostshock_timer = 10000 + rand()%5000;
            }
        }else frostshock_timer -= diff;
        
        if(shadowshock_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_SHADOW_SHOCK);
                shadowshock_timer = 10000 + rand()%5000;
            }
        }else shadowshock_timer -= diff;
        
        if(chainlightning_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_CHAIN_LIGHTNING );
                chainlightning_timer = 25000;
            }
        }else chainlightning_timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_darkweaver_sythAI(Creature *_Creature)
{
    return new boss_darkweaver_sythAI (_Creature);
}

/* ELEMENTALS */

struct MANGOS_DLL_DECL mob_syth_fireAI : public ScriptedAI
{
    mob_syth_fireAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 flameshock_timer;
    uint32 flamebuffet_timer;

    bool InCombat;

    void Reset()
    {   
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
        
        
        

        flameshock_timer = 2500;
        flamebuffet_timer = 5000;
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
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
                   
        if(flameshock_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_FLAME_SHOCK);
                flameshock_timer = 5000;
            }
        }else flameshock_timer -= diff;
        
        if(flamebuffet_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_FLAME_BUFFET );
                flamebuffet_timer = 5000;
            }
        }else flamebuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_mob_syth_fireAI(Creature *_Creature)
{
    return new mob_syth_fireAI (_Creature);
}

struct MANGOS_DLL_DECL mob_syth_arcaneAI : public ScriptedAI
{
    mob_syth_arcaneAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 arcaneshock_timer;
    uint32 arcanebuffet_timer;

    bool InCombat;

    void Reset()
    {   
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
        
        
        

        arcaneshock_timer = 2500;
        arcanebuffet_timer = 5000;
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
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
                   
        if(arcaneshock_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_ARCANE_SHOCK);
                arcaneshock_timer = 5000;
            }
        }else arcaneshock_timer -= diff;
        
        if(arcanebuffet_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_ARCANE_BUFFET );
                arcanebuffet_timer = 5000;
            }
        }else arcanebuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_mob_syth_arcaneAI(Creature *_Creature)
{
    return new mob_syth_arcaneAI (_Creature);
}

struct MANGOS_DLL_DECL mob_syth_frostAI : public ScriptedAI
{
    mob_syth_frostAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 frostshock_timer;
    uint32 frostbuffet_timer;

    bool InCombat;

    void Reset()
    {   
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
        
        
        

        frostshock_timer = 2500;
        frostbuffet_timer = 5000;
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
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
                   
        if(frostshock_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_FROST_SHOCK);
                frostshock_timer = 5000;
            }
        }else frostshock_timer -= diff;
        
        if(frostbuffet_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_FROST_BUFFET );
                frostbuffet_timer = 5000;
            }
        }else frostbuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_mob_syth_frostAI(Creature *_Creature)
{
    return new mob_syth_frostAI (_Creature);
}

struct MANGOS_DLL_DECL mob_syth_shadowAI : public ScriptedAI
{
    mob_syth_shadowAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 shadowshock_timer;
    uint32 shadowbuffet_timer;

    bool InCombat;

    void Reset()
    {   
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
        
        
        

        shadowshock_timer = 2500;
        shadowbuffet_timer = 5000;
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
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
                   
        if(shadowshock_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_SHADOW_SHOCK);
                shadowshock_timer = 5000;
            }
        }else shadowshock_timer -= diff;
        
        if(shadowbuffet_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_SHADOW_BUFFET );
                shadowbuffet_timer = 5000;
            }
        }else shadowbuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_mob_syth_shadowAI(Creature *_Creature)
{
    return new mob_syth_shadowAI (_Creature);
}

void AddSC_boss_darkweaver_syth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_darkweaver_syth";
    newscript->GetAI = GetAI_boss_darkweaver_sythAI;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="mob_syth_fire";
    newscript->GetAI = GetAI_mob_syth_arcaneAI;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="mob_syth_arcane";
    newscript->GetAI = GetAI_mob_syth_arcaneAI;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="mob_syth_frost";
    newscript->GetAI = GetAI_mob_syth_frostAI;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="mob_syth_shadow";
    newscript->GetAI = GetAI_mob_syth_shadowAI;
    m_scripts[nrscripts++] = newscript;
}
