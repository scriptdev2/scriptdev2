
/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "../../sc_defines.h"

#define SPELL_REPENTANCE          29511
#define SPELL_HOLYFIRE            29522
#define SPELL_DIVINEWRATH         28883
#define SPELL_HOLYGROUNDAURA      29523

#define SAY_AGGRO               "Your behavior will not be tolerated!"
#define SAY_SLAY1               "Anh ah ah..."
#define SAY_SLAY2               "This is for the best."
#define SAY_SLAY3               "Impure thoughts lead to profane actions."
#define SAY_REPENTANCE1         "Cast out your corrupt thoughts"
#define SAY_REPENTANCE2         "Your impurity must be cleansed."
#define SAY_DEATH               "Death comes. Will your conscience be clear?"
#define SOUND_AGGRO             9204
#define SOUND_SLAY1             9207
#define SOUND_SLAY2             9311 
#define SOUND_SLAY3             9312
#define SOUND_REPENTANCE1       9208
#define SOUND_REPENTANCE2       9313
#define SOUND_DEATH             9206

struct MANGOS_DLL_DECL boss_maiden_of_virtueAI : public ScriptedAI
{
    boss_maiden_of_virtueAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Repentance_Timer;
    uint32 Holyfire_Timer;
    uint32 Divinewrath_Timer;
    bool Holyground;
    bool InCombat;

    void EnterEvadeMode()
    {
        Repentance_Timer = 45000;      
        Holyfire_Timer = 20000;
        Divinewrath_Timer = 15000;
        Holyground = false;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void KilledUnit(Unit* Victim)
    {
      
       if(rand()%2)
            return;
                
       switch(rand()%3)
       {
           case 0:
           DoYell(SAY_SLAY1,LANG_UNIVERSAL,Victim);
           DoPlaySoundToSet(m_creature, SOUND_SLAY1);
           break;

           case 1:
           DoYell(SAY_SLAY2,LANG_UNIVERSAL,Victim);
           DoPlaySoundToSet(m_creature, SOUND_SLAY2);
           break;

           case 2:
           DoYell(SAY_SLAY3,LANG_UNIVERSAL,Victim);
           DoPlaySoundToSet(m_creature, SOUND_SLAY3);
           break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
            }
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {         

            if (!Holyground)
            {
              DoCast(m_creature,SPELL_HOLYGROUNDAURA);
              Holyground = true;
            }  

            //Repentance_Timer
            if (Repentance_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_REPENTANCE);

               switch(rand()%2)
               {
                case 0:
                 DoYell(SAY_REPENTANCE1,LANG_UNIVERSAL,NULL);
                 DoPlaySoundToSet(m_creature, SOUND_REPENTANCE1);
                 break;

                case 1:
                 DoYell(SAY_REPENTANCE2,LANG_UNIVERSAL,NULL);
                 DoPlaySoundToSet(m_creature, SOUND_REPENTANCE2);
                 break;
               }
                //45 seconds until we should cast this agian
                Repentance_Timer = 45000;
            }else Repentance_Timer -= diff;

            //Holyfire_Timer
            if (Holyfire_Timer < diff)
            {
                //Select Random Target
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);

                //Cast
                if (target)
                     DoCast(target,SPELL_HOLYFIRE);
                else
                     DoCast(m_creature->getVictim(),SPELL_HOLYFIRE); 

                //Random time until we should cast this again
                Holyfire_Timer = (rand()%50)*1000; 
            }else Holyfire_Timer -= diff;
            
            //Divinewrath_Timer
            if (Divinewrath_Timer < diff)
            { 
                //Select Random Target
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);

                //Cast
                if (target)
                     DoCast(target,SPELL_DIVINEWRATH);
                else
                     DoCast(m_creature->getVictim(),SPELL_DIVINEWRATH); 
                       
                //Random Time until we should cast this agian
                 Divinewrath_Timer = (rand()%50)*1000; 
            }else Divinewrath_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};
CreatureAI* GetAI_boss_maiden_of_virtue(Creature *_Creature)
{
    return new boss_maiden_of_virtueAI (_Creature);
}


void AddSC_boss_maiden_of_virtue()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_maiden_of_virtue";
    newscript->GetAI = GetAI_boss_maiden_of_virtue;
    m_scripts[nrscripts++] = newscript;
}
