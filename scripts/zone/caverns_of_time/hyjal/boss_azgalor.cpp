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
SDName: Boss_Azgalor
SD%Complete: 100
SDComment: 
SDCategory: Caverns Of Time, Mount Hyjal
EndScriptData */

#include "def_hyjal.h"

#define SPELL_RAIN_OF_FIRE  31340
#define SPELL_DOOM          31347
#define SPELL_DOOM_SUMMON   31350
#define SPELL_HOWL          31344
#define SPELL_CLEAVE        31345

#define SAY_AGGRO           "Abandon all hope! The legion has returned to finish what was begun so many years ago. This time there will be no escape!"
#define SOUND_AGGRO         10999
#define SAY_DOOM1           "Suffer you despicable insect!"
#define SOUND_DOOM1         11000
#define SAY_DOOM2           "Just a taste... of what awaits you."
#define SOUND_DOOM2         11046
#define SAY_SLAY1           "Reesh, hokta!"
#define SOUND_SLAY1         11001
#define SAY_SLAY2           "No one is going to save you."
#define SOUND_SLAY2         11047
#define SAY_SLAY3           "Don't fight it."
#define SOUND_SLAY3         11048
#define SAY_DEATH           "Your time is almost... up."
#define SOUND_DEATH         11002

#define SOUND_ARCHIMONDE   10986
#define SAY_ARCHIMONDE     "All of your efforts have been in vain, for the draining of the World Tree has already begun! Soon, the heart of your world shall beat no more!"

struct MANGOS_DLL_DECL boss_azgalorAI : public ScriptedAI
{
    boss_azgalorAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 DoomTimer;
    uint32 HowlTimer;
    uint32 RainOfFireTimer;
    uint32 CleaveTimer;

    void Reset()
    {
        DoomTimer = 50000;
        HowlTimer = 30000;
        RainOfFireTimer = 20000;
        CleaveTimer = 10000;

        if(pInstance)
            pInstance->SetData(DATA_AZGALOREVENT, NOT_STARTED);
    }

    void Aggro(Unit* who)
    {
        DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_AGGRO);

        if(pInstance)
            pInstance->SetData(DATA_AZGALOREVENT, IN_PROGRESS);

        DoZoneInCombat();
    }

    void JustDied(Unit* killer)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);

        if(pInstance)
            pInstance->SetData(DATA_AZGALOREVENT, DONE);
        
        Creature* Echo = DoSpawnCreature(13083, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 35000);
        if(Echo)
            Echo->SetVisibility(VISIBILITY_OFF);
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%3)
        {
           case 0:
               DoYell(SAY_SLAY1, LANG_UNIVERSAL, NULL);
               DoPlaySoundToSet(m_creature, SOUND_SLAY1);
               break;
           case 1:
               DoYell(SAY_SLAY2, LANG_UNIVERSAL, NULL);
               DoPlaySoundToSet(m_creature, SOUND_SLAY2);
               break;
           case 2:
               DoYell(SAY_SLAY3, LANG_UNIVERSAL, NULL);
               DoPlaySoundToSet(m_creature, SOUND_SLAY3);
               break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(DoomTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);
            if(target && target->GetTypeId() == TYPEID_PLAYER)
            {
                DoCast(target, SPELL_DOOM);
                switch(rand()%2)
                {
                   case 0:
                       DoYell(SAY_DOOM1, LANG_UNIVERSAL, NULL);
                       DoPlaySoundToSet(m_creature, SOUND_DOOM1);
                       break;
                   case 1:
                       DoYell(SAY_DOOM2, LANG_UNIVERSAL, NULL);
                       DoPlaySoundToSet(m_creature, SOUND_DOOM2);
                       break;
                }
                DoomTimer = 60000;
            }
        }else DoomTimer -= diff;

        if(HowlTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_HOWL);
            HowlTimer = 30000;
        }else HowlTimer -= diff;

        if(RainOfFireTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), SPELL_RAIN_OF_FIRE);
            RainOfFireTimer = 20000 + rand()%15000;
        }else RainOfFireTimer -= diff;

        if(CleaveTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            CleaveTimer = 10000 + rand()%5000;
        }else CleaveTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_echo_of_archimondeAI : public ScriptedAI
{
    mob_echo_of_archimondeAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
    }

    uint32 YellTimer;
    void Reset() { YellTimer = 15000; }
    void Aggro(Unit* who) {}
    
    void UpdateAI(const uint32 diff)
    {
        if(YellTimer < diff)
        {
            DoYell(SAY_ARCHIMONDE, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_ARCHIMONDE);
            YellTimer = 50000;
        }else YellTimer -= diff;

        ScriptedAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_boss_azgalor(Creature *_Creature)
{
    return new boss_azgalorAI(_Creature);
};

CreatureAI* GetAI_mob_echo_of_archimonde(Creature *_Creature)
{
    return new mob_echo_of_archimondeAI(_Creature);
};

void AddSC_boss_azgalor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_azgalor";
    newscript->GetAI = GetAI_boss_azgalor;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_echo_of_archimonde";
    newscript->GetAI = GetAI_mob_echo_of_archimonde;
    m_scripts[nrscripts++] = newscript;
}
