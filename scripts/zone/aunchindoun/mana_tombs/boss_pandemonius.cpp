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
SDName: Boss_Pandemonius
SD%Complete: 100
SDComment: Small chance of 1 player being hit by all 5 blasts (1/num_players^5)
SDCategory: Auchindoun, Mana Tombs
EndScriptData */

#include "sc_creature.h"

#define SPELL_VOID_BLAST                32325
#define SPELL_DARK_SHELL                32358

#define SAY_AGGRO_1                     "I will feed on your soul."
#define SOUND_AGGRO_1                   10561
#define SAY_AGGRO_2                     "So... full of life!"
#define SOUND_AGGRO_2                   10562
#define SAY_AGGRO_3                     "Do not... resist."
#define SOUND_AGGRO_3                   10563           

#define SAY_KILL_1                      "Yes! I am... empowered!"
#define SOUND_KILL_1                    10564
#define SAY_KILL_2                      "More... I must have more!"
#define SOUND_KILL_2                    10565

#define SAY_DEATH                       "To the void... once... more.."
#define SOUND_DEATH                     10566

struct MANGOS_DLL_DECL boss_pandemoniusAI : public ScriptedAI
{
    boss_pandemoniusAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 voidblast_timer;
    uint32 darkshell_timer;
    uint32 voidblast_counter;

    bool InCombat;

    void Reset()
    {   
        voidblast_timer = 10000;
        darkshell_timer = 15000;
        voidblast_counter = 0;

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

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(voidblast_timer < diff)
        {     
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(target,SPELL_VOID_BLAST);
            voidblast_timer = 500;
            voidblast_counter++;
            if(voidblast_counter == 5)
            {
                voidblast_timer = 25000+rand()%10000;
                voidblast_counter = 0;
            }
           }else voidblast_timer -= diff;
 
        if(voidblast_counter == 0)
        if(darkshell_timer < diff)
        {                   
            DoCast(m_creature,SPELL_DARK_SHELL);
            darkshell_timer = 15000;
        }else darkshell_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_pandemoniusAI(Creature *_Creature)
{
    return new boss_pandemoniusAI (_Creature);
}

void AddSC_boss_pandemonius()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_pandemonius";
    newscript->GetAI = GetAI_boss_pandemoniusAI;
    m_scripts[nrscripts++] = newscript;
}
