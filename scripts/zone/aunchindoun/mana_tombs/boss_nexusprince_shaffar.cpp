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
SDName: Boss_NexusPrince_Shaffar
SD%Complete: 50
SDComment: Missing some spells
SDCategory: Auchindoun, Mana Tombs
EndScriptData */

#include "sc_creature.h"

#define SPELL_ETHEREAL_BEACON           32371 // Summon 18431
#define SPELL_FROSTBOLT                 32364
#define SPELL_FIREBOLT                  32363
#define SPELL_FROSTNOVA                 32365

//Spells for Beacon
#define SPELL_ARCANE_BOLT              36712
#define SPELL_ETHEREAL_APPRENTICE       32372 // Summon 18430
#define SPELL_VISUAL_EFFECT             32368

//Spells for Apprentice
#define SPELL_ETHEREAL_FIREBALL         32369
#define SPELL_ETHEREAL_FROSTBOLT        32370

#define SAY_INTRO       "What is this? You must forgive me, but I was not expecting company. As you can see, we are somewhat preoccupied right now. But no matter. As I am a gracious host, I will tend to you... personally."     
#define SOUND_INTRO     10539

#define SAY_AGGRO_1        "We have not yet been properly introduced."
#define SOUND_AGGRO_1    10541
#define SAY_AGGRO_2        "An epic battle. How exciting!"
#define    SOUND_AGGRO_2    10542
#define SAY_AGGRO_3        "I have longed for a good adventure."
#define    SOUND_AGGRO_3    10543        

#define SAY_SLAY_1        "It has been... entertaining."
#define SOUND_SLAY_1    10544    
#define SAY_SLAY_2        "And now we part company."
#define SOUND_SLAY_2    10545        

#define SAY_SUMMON        "I have such fascinating things to show you."
#define SOUND_SUMMON    10540

#define SAY_DEAD        "I must bid you... farewell."
#define SOUND_DEAD        10546


struct MANGOS_DLL_DECL boss_nexusprince_shaffarAI : public ScriptedAI
{
    boss_nexusprince_shaffarAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 beacon_timer;
    uint32 firebolt_timer;
    uint32 frostbolt_timer;
    uint32 frostnova_timer;

    bool HasTaunted;

    void Reset()
    {   

        beacon_timer = 10000;
        firebolt_timer = 8000;
        frostbolt_timer = 4000;
        frostnova_timer = 7000;

        HasTaunted = false;
    }

    void JustDied(Unit* Killer)
    { 
        DoYell(SAY_DEAD, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEAD);
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
                DoStartMeleeAttack(who);
                
                Aggro(who);
            }
            else if (!HasTaunted && m_creature->IsWithinDistInMap(who, 60.0f))
            {
                DoYell(SAY_INTRO, LANG_UNIVERSAL, NULL);

                HasTaunted = true;
            }
        }
    }  

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(frostbolt_timer < diff)
        {   
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target) 
            {
                DoCast(target,SPELL_FROSTBOLT);
                frostbolt_timer = 8000 + rand()%5000;
            }
        }else frostbolt_timer -= diff;

        if(firebolt_timer < diff)
        {   
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target) 
            {
                DoCast(target,SPELL_FROSTBOLT);
                firebolt_timer = 8000 + rand()%5000;
            }
        }else firebolt_timer -= diff;

        if(frostnova_timer < diff)
        {   
            DoCast(m_creature,SPELL_FROSTNOVA);
            frostnova_timer  = 10000 + rand()%5000;
        }else frostnova_timer -= diff;

        if(beacon_timer < diff)
        {     
            DoYell(SAY_SUMMON, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SUMMON);
            //DoCast(m_creature,SPELL_ETHEREAL_BEACON);

            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            Creature* Summoned = NULL;
            Summoned = DoSpawnCreature(18431, 0,0,0,0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 21000);
            if (Summoned && target)
                Summoned->AI()->AttackStart(target);

            beacon_timer = 10000;

        }else beacon_timer -= diff;

        DoMeleeAttackIfReady();
    }
};



CreatureAI* GetAI_boss_nexusprince_shaffarAI(Creature *_Creature)
{
    return new boss_nexusprince_shaffarAI (_Creature);
}


struct MANGOS_DLL_DECL mob_ethereal_beaconAI : public ScriptedAI
{
    mob_ethereal_beaconAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 apprentice_timer;
    uint32 arcanebolt_timer;

    bool onlyonce;
    bool apprentice;

    void Reset()
    {   
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();

        apprentice_timer = 20000;
        arcanebolt_timer = 3000;
        onlyonce = true;
        apprentice = false;
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
        if(onlyonce)
        {
            DoCast(m_creature,SPELL_VISUAL_EFFECT );
            onlyonce = false;
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
        if(!apprentice)
        {
            if(apprentice_timer < diff)
            {   
                //DoCast(m_creature,SPELL_ETHEREAL_APPRENTICE);
                apprentice = true;
                //m_creature->RemoveFromWorld();
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                Creature* Summoned = NULL;
                Summoned = DoSpawnCreature(18430, 0,0,0,0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000);
                if (Summoned && target)
                    Summoned->AI()->AttackStart(target);
            }else apprentice_timer -= diff;

            if(arcanebolt_timer < diff)
            {   
                DoCast(m_creature->getVictim(),SPELL_ARCANE_BOLT);
                arcanebolt_timer = 2000 + rand()%5000;
            }else arcanebolt_timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};



CreatureAI* GetAI_mob_ethereal_beaconAI(Creature *_Creature)
{
    return new mob_ethereal_beaconAI (_Creature);
}

struct MANGOS_DLL_DECL mob_ethereal_apprenticeAI : public ScriptedAI
{
    mob_ethereal_apprenticeAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 etherealfireball_timer;
    uint32 etherealfrostbolt_timer;


    void Reset()
    {   

        etherealfireball_timer = 3000;
        etherealfrostbolt_timer = 6000;
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(etherealfireball_timer < diff)
        {   
            DoCast(m_creature->getVictim(),SPELL_ETHEREAL_FIREBALL);
            etherealfireball_timer = 6000;
        }else etherealfireball_timer -= diff;

        if(etherealfrostbolt_timer < diff)
        {   
            DoCast(m_creature->getVictim(),SPELL_ETHEREAL_FROSTBOLT);
            etherealfrostbolt_timer = 6000;
        }else etherealfrostbolt_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ethereal_apprenticeAI(Creature *_Creature)
{
    return new mob_ethereal_apprenticeAI (_Creature);
}

void AddSC_boss_nexusprince_shaffar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_nexusprince_shaffar";
    newscript->GetAI = GetAI_boss_nexusprince_shaffarAI;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_ethereal_beacon";
    newscript->GetAI = GetAI_mob_ethereal_beaconAI;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_ethereal_apprentice";
    newscript->GetAI = GetAI_mob_ethereal_apprenticeAI;
    m_scripts[nrscripts++] = newscript;
}
