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
SDName: Boss_Anetheron
SD%Complete: 60
SDComment: Untested
SDCategory: Caverns of time, Mount Hyjal
EndScriptData */

#include "def_hyjal.h"

#define SAY_AGGRO       "You are defenders of a doomed world. Flee here and perhaps you will prolong your pathetic lives."
#define SOUND_AGGRO     10977
#define SAY_SLEEP1      "You look tired."
#define SOUND_SLEEP1    10978
#define SAY_CARRION1    "The swarm is eager to feed."
#define SOUND_CARRION1  10979
#define SAY_CARRION2    "Pestilence upon you!"
#define SOUND_CARRION2  11036
#define SAY_INFERNO     "Let fire rain from above!"
#define SOUND_INFERNO   10980
#define SAY_INFERNO2    "Earth and Sky shall burn!"
#define SOUND_INFERNO2  11036
#define SAY_SLAY1       "Scream for me"
#define SOUND_SLAY1     11038
#define SAY_SLAY2       "Pity, no time for a slow death."
#define SOUND_SLAY2     11039
#define SAY_SLAY3       "Your hopes, are lost."
#define SOUND_SLAY3     10981
#define SAY_DEATH       "The clock... is still... ticking."
#define SOUND_DEATH     10982

// Anetheron's spells
#define SPELL_CARRION_SWARM      31306 // Cone AoE damage at random target. 4-5.5 Shadow damage and -75% healing DONE for 20 seconds.
#define SPELL_VAMPIRIC_AURA      38196 // While under effect of this aura, Anetheron heals 300% of his melee damage done (Hardcoded)
#define SPELL_SLEEP              31548 // 2 random targets put to sleep every minute, using ApplyAura instead of DoCast.
#define SPELL_INFERNO            31299 // Animation of a falling infernal

struct MANGOS_DLL_DECL boss_anetheronAI : public ScriptedAI
{
    boss_anetheronAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;
    
    // Fight timers
    uint32 CarrionSwarmTimer;
    uint32 VampiricAuraTimer;
    uint32 SleepTimer;
    uint32 InfernoTimer;

    void Reset()
    {
        CarrionSwarmTimer = 10000;   
        VampiricAuraTimer = 1000;
        SleepTimer = 60000;   
        InfernoTimer = 30000;
        
        if(pInstance)
            pInstance->SetData(DATA_ANETHERONEVENT, NOT_STARTED);
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
            pInstance->SetData(DATA_ANETHERONEVENT, DONE);

        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
    }

    void Aggro(Unit* who)
    {
        if(pInstance)
            pInstance->SetData(DATA_ANETHERONEVENT, IN_PROGRESS);

        DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_AGGRO);
    }

    void UpdateAI(const uint32 diff)
    {
        // Do not do anything unless we got a current target.
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        // Carrion Swarm
        if(CarrionSwarmTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_CARRION_SWARM, true);
            CarrionSwarmTimer = 45000;
            switch(rand()%2)
            {
                case 0:
                    DoYell(SAY_CARRION1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_CARRION1);
                    break;
                case 1:
                    DoYell(SAY_CARRION2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_CARRION2);
                    break;
            }
        }else CarrionSwarmTimer -= diff;

        if(VampiricAuraTimer < diff)
        {
            DoCast(m_creature, SPELL_VAMPIRIC_AURA);
            VampiricAuraTimer = 5000 + ((rand()%55)*1000);
        }else VampiricAuraTimer -= diff;

        // Sleep
        if(SleepTimer < diff)
        {
            for(uint8 i = 0; i < 3; ++i)
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), SPELL_SLEEP, true);

            DoYell(SAY_SLEEP1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLEEP1);
            SleepTimer = 60000;
        }else SleepTimer -= diff;

        // Inferno
        if(InfernoTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_INFERNO, true);
            switch(rand()%3)
            {
                case 0:
                    DoYell(SAY_INFERNO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_INFERNO);
                    break;
                case 1:
                    DoYell(SAY_INFERNO2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_INFERNO2);
                    break;
            }
            
            InfernoTimer = 60000;
        }else InfernoTimer -= diff;
        
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anetheron(Creature *_Creature)
{
    return new boss_anetheronAI (_Creature);
}

void AddSC_boss_anetheron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_anetheron";
    newscript->GetAI = GetAI_boss_anetheron;
    m_scripts[nrscripts++] = newscript;
}
