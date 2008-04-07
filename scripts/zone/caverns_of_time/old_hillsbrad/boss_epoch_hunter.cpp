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
SDName: Boss_Epoch_Hunter
SD%Complete: 100
SDComment: 
EndScriptData */

#include "sc_creature.h"

#define SPELL_SAND_BREATH       31914
#define SPELL_IMPENDING_DOOM    19702
#define SPELL_KNOCKBACK         26027
#define SPELL_MDA               33834

#define SAY_ENTER1              "Thrall! Come outside and face your fate!"
#define SAY_ENTER2              "Taretha's life hangs in the balance. Surely you care for her. Surely you wish to save her..."
#define SAY_ENTER3              "Ah, there you are. I had hoped to accomplish this with a bit of subtlety, but I suppose direct confrontation was inevitable. Your future, Thrall, must not come to pass and so...you and your troublesome friends must die!"
#define SAY_AGGRO1              "Enough! I will erase your very existence!"
#define SAY_AGGRO2              "You cannot fight fate!"
#define SAY_SLAY1               "You are...irrelevant."
#define SAY_SLAY2               "Thrall will remain a slave. Taretha will die. You have failed."
#define SAY_BREATH1             "Not so fast!" 
#define SAY_BREATH2             "Struggle as much as you like!"
#define SAY_DEATH               "No!...The master... will not... be pleased."

#define SOUND_ENTER1            10418
#define SOUND_ENTER2            10419
#define SOUND_ENTER3            10420
#define SOUND_AGGRO1            10421
#define SOUND_AGGRO2            10422
#define SOUND_SLAY1             10425
#define SOUND_SLAY2             10426
#define SOUND_BREATH1           10423
#define SOUND_BREATH2           10424
#define SOUND_DEATH             10427

struct MANGOS_DLL_DECL boss_epoch_hunterAI : public ScriptedAI
{
    boss_epoch_hunterAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 SandBreath_Timer;
    uint32 ImpendingDoom_Timerr;
    uint32 Knockback_Timer;
    uint32 Mda_Timer;

    bool InCombat;

    void Reset()
    {
        SandBreath_Timer = 25000;
        ImpendingDoom_Timerr = 30000;
        Knockback_Timer = 35000;
        Mda_Timer = 40000;

        InCombat = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
    }

    void Aggro(Unit *who)
    {
                switch(rand()%2)
                {
                case 0:
                    DoYell( SAY_AGGRO1,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
                    break;
                case 1:
                    DoYell( SAY_AGGRO2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
                    break;
                }
    }    

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
        case 0:
            DoYell( SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY1);
            break;
        case 1:
            DoYell( SAY_SLAY2,LANG_UNIVERSAL,NULL);
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

            //Sand Breath
            if (SandBreath_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)
                {
                    DoCast(target,SPELL_SAND_BREATH);
                    switch(rand()%2)
                    {
                    case 0:
                        DoYell( SAY_BREATH1,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, SOUND_BREATH1);
                        break;
                    case 1:
                        DoYell( SAY_BREATH2,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, SOUND_BREATH2);
                        break;
                    }
                }
                SandBreath_Timer = 25000+rand()%5000;
            }else SandBreath_Timer -= diff;

            //Impending Doom
            if (ImpendingDoom_Timerr < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)
                    DoCast(target,SPELL_IMPENDING_DOOM);
                ImpendingDoom_Timerr = 30000+rand()%5000;
            }else ImpendingDoom_Timerr -= diff;

            //Knockback
            if (Knockback_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)
                    DoCast(target,SPELL_KNOCKBACK);
                Knockback_Timer = 35000+rand()%10000;
            }else Knockback_Timer -= diff;

            //Mortal Strike
            if (Mda_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)
                    DoCast(target,SPELL_MDA);

                Mda_Timer = 40000+rand()%10000;
            }else Mda_Timer -= diff;

            DoMeleeAttackIfReady();
        }
};

CreatureAI* GetAI_boss_epoch_hunter(Creature *_Creature)
{
    return new boss_epoch_hunterAI (_Creature);
}

void AddSC_boss_epoch_hunter()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_epoch_hunter";
    newscript->GetAI = GetAI_boss_epoch_hunter;
    m_scripts[nrscripts++] = newscript;
}
