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
SDName: Boss_Harbinger_Skyriss
SD%Complete: 30
SDComment: Placeholder, only contain set/get instance data and some speech. Intro, combatAI etc not implemented yet.
SDCategory: Tempest Keep, Arcatraz
EndScriptData */

#include "def_arcatraz.h"

#define SAY_INTRO       "It is a small matter to control the mind of the weak... for I bear allegiance to powers untouched by time, unmoved by fate. No force on this world or beyond harbors the strength to bend our knee... not even the mighty Legion!"
#define SOUND_INTRO     11122

#define SAY_AGGRO       "Bear witness to the agent of your demise!"
#define SOUND_AGGRO     11123

#define SAY_KILL_1      "Your fate is written!"
#define SOUND_KILL_1    11124
#define SAY_KILL_2      "The chaos I have sown here is but a taste...."
#define SOUND_KILL_2    11125

#define SAY_MIND_1      "You will do my bidding, weakling."
#define SOUND_MIND_1    11127
#define SAY_MIND_2      "Your will is no longer your own."
#define SOUND_MIND_2    11128

#define SAY_FEAR_1      "Flee in terror."
#define SOUND_FEAR_1    11129
#define SAY_FEAR_2      "I will show you horrors undreamed of."
#define SOUND_FEAR_2    11130

#define SAY_IMAGE       "We span the universe, as countless as the stars!"
#define SOUND_IMAGE     11131

#define SAY_DEATH       "I am merely one of... infinite multitudes."
#define SOUND_DEATH     11126

#define SPELL_MIND_REND         36924
#define SPELL_FEAR              39415
#define SPELL_DOMINATION_FAR    39019
#define SPELL_DOMINATION_CLOSE  37162
#define HERO_SPELL_MANA_BURN    39020

struct MANGOS_DLL_DECL boss_harbinger_skyrissAI : public ScriptedAI
{
    boss_harbinger_skyrissAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;
    uint64 MellicharGUID;

    uint32 MindRend_Timer;
    uint32 Fear_Timer;
    uint32 Domination_Timer;

    void Reset()
    {
        MellicharGUID = 0;

        MindRend_Timer = 0;
        Fear_Timer = 0;
        Domination_Timer = 0;
    }

    void JustDied(Unit* Killer)
    { 
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
        if( pInstance )
            pInstance->SetData(TYPE_HARBINGERSKYRISS,DONE);
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        { 
        case 0:
            DoYell(SAY_KILL_1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_1);
            break;
        case 1:
            DoYell(SAY_KILL_2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_2);
            break; 
        }
    }

    void Aggro(Unit *who)
    {
        DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_AGGRO);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_harbinger_skyrissAI(Creature *_Creature)
{
    return new boss_harbinger_skyrissAI (_Creature);
}

void AddSC_boss_harbinger_skyriss()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_harbinger_skyriss";
    newscript->GetAI = GetAI_boss_harbinger_skyrissAI;
    m_scripts[nrscripts++] = newscript;
}
