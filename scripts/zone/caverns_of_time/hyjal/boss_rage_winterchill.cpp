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
SDName: Boss_RageWinterchill
SD%Complete: 85
SDComment: Untested
SDCategory: Caverns Of Time, Mount Hjal
EndScriptData */

#include "def_hyjal.h"

#define SAY_AGGRO             "The Legion's final conquest has begun! Once again the subjugation of this world is within our grasp. Let none survive!"
#define SOUND_AGGRO           11022
#define SAY_DECAY1            "Crumble and rot!"
#define SOUND_DECAY1          11023
#define SAY_DECAY2            "Ashes to ashes, dust to dust."
#define SOUND_DECAY2          11055
#define SAY_NOVA1             "Succumb to the icy chill... of death."
#define SOUND_NOVA1           11024
#define SAY_NOVA2             "It will be much colder in your grave."
#define SOUND_NOVA2           11058
#define SAY_SLAY1             "All life must perish!"
#define SOUND_SLAY1           11025
#define SAY_SLAY2             "You world is ours now!"
#define SOUND_SLAY2           11056
#define SAY_SLAY3             "Victory to the Legion!"
#define SOUND_SLAY3           11057
#define SAY_DEATH             "You have won this battle, but not... the... war"
#define SOUND_DEATH           11026

// Rage Winterchill's spells
#define SPELL_ICEBOLT            31249 // Single target (random). 4.5 - 5.5k Frost damage, 4 sec stun + 10k damage over 4 seconds.
#define SPELL_DEATH_DECAY        39658 // Single target (random). ~2 Shadow (?) AoE damage per second at a random unit
#define SPELL_FROST_NOVA         32365 // Self-pointed AoE. Minimal damage, 6 seconds freeze.
#define SPELL_FROST_ARMOR        31256 // Self-buff. Adds armor, and chills attackers - DOESNT WORK

struct MANGOS_DLL_DECL boss_rage_winterchillAI : public ScriptedAI
{
    boss_rage_winterchillAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;
    // Fight timers
    uint32 FrostArmorTimer;
    uint32 IceboltTimer;
    uint32 FrostNovaTimer;
    uint32 DeathDecayTimer;
    
    void Reset()
    {
        IceboltTimer = 18000;   
        DeathDecayTimer = 34000;
        FrostNovaTimer = 11000;   
        FrostArmorTimer = 10000;  
     
        if(pInstance)
            pInstance->SetData(DATA_RAGEWINTERCHILLEVENT, NOT_STARTED);
    }
    
    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
            case 0:
                DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY1);
                break;
            case 1:
                DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY2);
                break;
            case 2:
                DoYell(SAY_SLAY3,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY3);
                break;
        }
    }
    
    void JustDied(Unit *victim)
    {
        if(pInstance)
            pInstance->SetData(DATA_RAGEWINTERCHILLEVENT, DONE);

        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
    }

    void Aggro(Unit* who)
    {
        if(pInstance)
            pInstance->SetData(DATA_RAGEWINTERCHILLEVENT, IN_PROGRESS);

        DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_AGGRO);

        DoZoneInCombat();
    }

    void UpdateAI(const uint32 diff)
    {
        // Do not do anything unless we got a current target.
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;
    
        if(FrostArmorTimer < diff)
        {
            DoCast(m_creature, SPELL_FROST_ARMOR);
            FrostArmorTimer = 45000;                
        }else FrostArmorTimer -= diff;

        // Frost Nova
        if(FrostNovaTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FROST_NOVA);
            
            switch(rand()%2)
            {
                case 0:
                    DoYell(SAY_NOVA1,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_NOVA1);
                    break;
                case 1:
                    DoYell(SAY_NOVA2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_NOVA2);
                    break;
            }
            
            FrostNovaTimer = 45000;
        }else FrostNovaTimer -= diff;

        // Icebolt
        if(IceboltTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_ICEBOLT, true);
            IceboltTimer = 45000;
        }else IceboltTimer -= diff;

        // Death&Decay
        if(DeathDecayTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_DEATH_DECAY, true);

            switch(rand()%2)
            {
                case 0:
                    DoYell(SAY_DECAY1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_DECAY1);
                    break;
                case 1:
                    DoYell(SAY_DECAY2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_DECAY2);
                    break;
            }
            
            DeathDecayTimer = 45000;
        }else DeathDecayTimer -= diff;
        
        DoMeleeAttackIfReady(); // And keep pummeling them with our bone fingers.
    }
};

CreatureAI* GetAI_boss_rage_winterchill(Creature *_Creature)
{
    return new boss_rage_winterchillAI (_Creature);
}

void AddSC_boss_rage_winterchill()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_rage_winterchill";
    newscript->GetAI = GetAI_boss_rage_winterchill;
    m_scripts[nrscripts++] = newscript;
}
