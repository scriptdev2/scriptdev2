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
SDName: Boss_Luetenant_Drake
SD%Complete: 100
SDComment: 
SDCategory: Caverns of Time, Old Hillsbrad
EndScriptData */

#include "sc_creature.h"

#define WHIRLWIND         40236
#define FEAR              33789
#define MORTAL_STRIKE     40220
#define EXPLODIG_SHOUT    33792

#define  SAY_AGGRO        "I know what you're up to, and I mean to put an end to it, permanently!" 
#define  SAY_SLAY1        "No more middling for you." 
#define  SAY_SLAY2        "You will not interfere!"
#define  SAY_MORTAL       "Time to bleed!" 
#define  SAY_SHOUT        "Run, you blasted cowards!"
#define  SAY_DEATH        "Thrall... must not... go free."

#define  SOUND_ENTER      10428
#define  SOUND_AGGRO      10429
#define  SOUND_SLAY1      10432
#define  SOUND_SLAY2      10433
#define  SOUND_MORTAL     10430
#define  SOUND_SHOUT      10431
#define  SOUND_DEATH      10434

struct MANGOS_DLL_DECL boss_lieutenant_drakeAI : public ScriptedAI
{
    boss_lieutenant_drakeAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Whirlwind_Timer;
    uint32 Fear_Timer;
    uint32 MortalStrike_Timer;
    uint32 ExplodingShout_Timer;

    void Reset()
    {
        Whirlwind_Timer = 20000;
        Fear_Timer = 30000;
        MortalStrike_Timer = 45000;
        ExplodingShout_Timer = 25000;        
    }

    void Aggro(Unit *who)
    {
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
    }    

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY1);
            break;
        case 1:
            DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY2);
            break;
        }
    } 

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Whirlwind
        if (Whirlwind_Timer < diff)
        {
            DoCast(m_creature->getVictim(), WHIRLWIND);

            Whirlwind_Timer = 20000+rand()%5000;
        }else Whirlwind_Timer -= diff;

        //Fear
        if (Fear_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if (target)
                DoCast(target, FEAR);

            Fear_Timer = 30000+rand()%10000;
        }else Fear_Timer -= diff;

        //Mortal Strike
        if (MortalStrike_Timer < diff)
        {
            DoYell(SAY_MORTAL, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_MORTAL);

            DoCast(m_creature->getVictim(), MORTAL_STRIKE);

            MortalStrike_Timer = 45000+rand()%5000;
        }else MortalStrike_Timer -= diff;

        /*
        //This only enabled on heroic?
        //Exploding Shout
        if (m_creature->IsHeroicCreature())
        {
        if (ExplodingShout_Timer < diff)
        {
        Unit* target = NULL;
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        if (target)
        DoYell(SAY_SHOUT, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_SHOUT);
        DoCast(target,EXPLODING_SHOUT);
        ExplodingShout_Timer = 25000+rand()%5000;
        }else ExplodingShout_Timer -= diff;    
        }
        */

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lieutenant_drake(Creature *_Creature)
{
    return new boss_lieutenant_drakeAI (_Creature);
}

void AddSC_boss_lieutenant_drake()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lieutenant_drake";
    newscript->GetAI = GetAI_boss_lieutenant_drake;
    m_scripts[nrscripts++] = newscript;
}
